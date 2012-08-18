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
 * Detect feature points on provided image.
 *
 * @param poDataset Image on which feature points will be detected
 * @param panBands Array of 3 raster bands numbers, for Red, Green, Blue bands (int that order)
 * @param poCollection Feaure point collection where detected points will be stored
 * @param nOctaveStart Number of bottom octave. Octave numbers starts from one.
 * This value directly and strongly affects to amount of recognized points
 * @param nOctaveEnd Number of top octave. Should be equal or greater than octaveStart
 * @param dfThreshold Value from 0 to 1. Threshold for feature point recognition.
 * Number of detected points is larger if threshold is higher
 *
 * @see GDALSimpleSURF class for detailed implementation.
 *
 * @note For example, for 640x480 images it's good to use octave numbers (bottom and top)
 * 1 and 3, or 2 and 2.
 * Be free to experiment with it, because character, robustness and number of points
 * entirely depend on provided range of octaves and threshold.
 * If method finds nothing, try to decrease octaveStart number or (and) threshold.
 * Conversely, if too many feature points, try to slightly increase threshold.
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

	if (nOctaveStart < 0 || nOctaveEnd < 0 ||
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
 * matched points. If threshold is lower than amount of corresponding
 * points is larger, and vice versa
 *
 * @note This method invokes function from GDALSimpleSURF class.
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
