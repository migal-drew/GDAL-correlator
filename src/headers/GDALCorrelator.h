/******************************************************************************
 * Project:  GDAL
 * Purpose:  Correlator
 * Author:   Andrew Migal, migal.drew@gmail.com
 *
 ******************************************************************************
 * Copyright (c) 2012, Andrew Migal
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ****************************************************************************/

/**
 * @file
 * @author Andrew Migal migal.drew@gmail.com
 * @brief Algorithms for searching corresponding points on images.
 * @details This implementation is  based on an simplified version
 * of SURF algorithm (Speeded Up Robust Features).
 * Provides capability for detection feature points
 * and finding equal points on different images.
 * As original, this realization is scale invariant, but sensitive to rotation.
 * Images should have similar rotation angles (maximum difference is up to 10-15 degrees),
 * otherwise algorithm produces incorrect and very unstable results.
 */

#ifndef GDALCORRELATOR_H_
#define GDALCORRELATOR_H_

#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "cpl_vsi.h"

#include "GDALFeaturePoint.h"
#include "GDALFeaturePointsCollection.h"
#include "GDALMatchedPointsCollection.h"
#include "GDALSimpleSURF.h"
#include "GDALIntegralImage.h"

/**
 * Detect feature points on provided image. Please carefully read documentation below.
 *
 * @param poDataset Image on which feature points will be detected
 * @param panBands Array of 3 raster bands numbers, for Red, Green, Blue bands (in that order)
 * @param poCollection Feaure point collection where detected points will be stored
 * @param nOctaveStart Number of bottom octave. Octave numbers starts from one.
 * This value directly and strongly affects to amount of recognized points
 * @param nOctaveEnd Number of top octave. Should be equal or greater than octaveStart
 * @param dfThreshold Value from 0 to 1. Threshold for feature point recognition.
 * Number of detected points is larger if threshold is lower
 *
 * @see GDALFeaturePoint, GDALSimpleSURF class for detailes.
 *
 * @note Every octave finds points in specific size. For small images
 * use small octave numbers, for high resolution - large.
 * For 1024x1024 images it's normal to use any octave numbers from range 1-6.
 * (for example, octave start - 1, octave end - 3, or octave start - 2, octave end - 2.)
 * For larger images, try 1-10 range or even higher.
 * Pay attention that number of detected point decreases quickly per octave
 * for particular image. Algorithm finds more points in case of small octave number.
 * If method detects nothing, reduce octave start value.
 * In addition, if many feature points are required (the largest possible amount),
 * use the lowest octave start value (1) and wide octave range.
 *
 * @note Typical threshold's value is 0,001. It's pretty good for all images.
 * But this value depends on image's nature and may be various in each particular case.
 * For example, value can be 0,002 or 0,005.
 * Notice that number of detected points is larger if threshold is lower.
 * But with high threshold feature points will be better - "stronger", more "unique" and distinctive.
 *
 * Feel free to experiment with parameters, because character, robustness and number of points
 * entirely depend on provided range of octaves and threshold.
 *
 * NOTICE that every octave requires time to compute. Use a little range
 * or only one octave, if execution time is significant.
 *
 * @return CE_None or CE_Failure if error occurs.
 */
CPLErr GatherFeaturePoints(GDALDataset* poDataset, int* panBands,
			GDALFeaturePointsCollection* poCollection,
			int nOctaveStart, int nOctaveEnd, double dfThreshold)
{
	if (poDataset == NULL)
	{
		CPLError(CE_Failure, CPLE_AppDefined, "GDALDataset isn't specified");
		return CE_Failure;
	}

	if (poCollection == NULL)
	{
		CPLError(CE_Failure, CPLE_AppDefined,
				"GDALFeaturePointsCollection isn't specified");
		return CE_Failure;
	}

	if (panBands == NULL)
	{
		CPLError(CE_Failure, CPLE_AppDefined,
						"Raster bands are not specified");
		return CE_Failure;
	}

	if (nOctaveStart <= 0 || nOctaveEnd < 0 ||
			nOctaveStart > nOctaveEnd)
	{
		CPLError(CE_Failure, CPLE_AppDefined,
						"Octave numbers are invalid");
		return CE_Failure;
	}

	if (dfThreshold < 0)
	{
		CPLError(CE_Failure, CPLE_AppDefined,
				"Threshold have to be greater than zero");
		return CE_Failure;
	}

	GDALRasterBand *poRstRedBand = poDataset->GetRasterBand(panBands[0]);
	GDALRasterBand *poRstGreenBand = poDataset->GetRasterBand(panBands[1]);
	GDALRasterBand *poRstBlueBand = poDataset->GetRasterBand(panBands[2]);

	int nWidth = poRstRedBand->GetXSize();
	int nHeight = poRstRedBand->GetYSize();

	// Allocate memory for grayscale image
	double **padfImg = NULL;
	padfImg = new double*[nHeight];
	for (int i = 0; i < nHeight; i++)
		padfImg[i] = new double[nWidth];

	// Create grayscale image
	GDALSimpleSURF::ConvertRGBToLuminosity(
			poRstRedBand, poRstGreenBand, poRstBlueBand, nWidth, nHeight,
			padfImg, nHeight, nWidth);

	// Prepare integral image
	GDALIntegralImage *poImg = new GDALIntegralImage();
	poImg->Initialize((const double**)padfImg, nHeight, nWidth);

	// Get feature points
	GDALSimpleSURF *poSurf = new GDALSimpleSURF(nOctaveStart, nOctaveEnd);
	poSurf->ExtractFeaturePoints(poImg, poCollection, dfThreshold);

	// Clean up
	delete poImg;
	delete poSurf;

	for (int i = 0; i < nHeight; i++)
		delete[] padfImg[i];

	delete[] padfImg;

	return CE_None;
}

/**
 * Find corresponding points (equal points in two collections).
 *
 * @param poMatched Resulting collection for matched points
 * @param poFirstCollection Points on the first image
 * @param poSecondCollection Points on the second image
 * @param dfThreshold Value from 0 to 1. Threshold affects to number of
 * matched points. If threshold is higher, amount of corresponding
 * points is larger, and vice versa
 *
 * @note Typical threshold's value is 0,1. BUT it's a very approximate guide.
 * It can be 0,001 or even 1. This threshold provides direct adjustment
 * of point matching.
 * NOTICE that if threshold is lower, matches are more robust and correct, but number of
 * matched points is smaller. Therefore if algorithm performs many false
 * detections and produces bad results, reduce threshold.
 * Otherwise, if algorithm finds nothing, increase threshold.
 *
 * @return CE_None or CE_Failure if error occurs.
 */
CPLErr MatchFeaturePoints(
			GDALMatchedPointsCollection* poMatched,
			GDALFeaturePointsCollection* poFirstCollection,
			GDALFeaturePointsCollection* poSecondCollection,
			double dfThreshold)
{
	GDALSimpleSURF::MatchFeaturePoints(poMatched,
			poFirstCollection, poSecondCollection, dfThreshold);

	return CE_None;
}

#endif /* GDALCORRELATOR_H_ */
