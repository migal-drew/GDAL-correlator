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
 * @brief Class for searching corresponding points on images.
 */

#ifndef GDALSIMPLESURF_H_
#define GDALSIMPLESURF_H_

#include "GDALOctaveMap.h"
#include "GDALIntegralImage.h"
#include "GDALFeaturePoint.h"
#include "GDALFeaturePointsCollection.h"
#include "GDALMatchedPointsCollection.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_vsi.h"

#include <list>
#include <math.h>

#define CPLFree VSIFree

/**
 * @author Andrew Migal migal.drew@gmail.com
 * @brief Class for searching corresponding points on images.
 * @details Provides capability for detection feature points
 * and finding equal points on different images.
 * Class implements simplified version of SURF algorithm (Speeded Up Robust Features).
 * As original, this realization is scale invariant, but sensitive to rotation.
 * Images should have similar rotation angles (maximum difference is up to 10-15 degrees),
 * otherwise algorithm produces incorrect and very unstable results.
 */
class GDALSimpleSURF
{
private:
	/**
	 * Class stores indexes of pair of point
	 * and distance between them.
	 */
	class MatchedPointPairInfo
	{
	public:
		MatchedPointPairInfo(int nInd_1, int nInd_2, double dfDist)
		{
			ind_1 = nInd_1;
			ind_2 = nInd_2;
			euclideanDist = dfDist;
		}

		int ind_1;
		int ind_2;
		double euclideanDist;
	};

public:
	/**
	 * Prepare class according to specified parameters. Octave numbers affects
	 * to amount of detected points and their robustness.
	 * Range between bottom and top octaves also affects to required time of detection points
	 * (if range is large, algorithm should perform more operations).
	 * @param nOctaveStart Number of bottom octave. Octave numbers starts with one
	 * @param nOctaveEnd Number of top octave. Should be equal or greater than OctaveStart
	 *
	 */
	GDALSimpleSURF(int nOctaveStart, int nOctaveEnd);
	virtual ~GDALSimpleSURF();

	/**
	 * Convert image with RGB channels to grayscale using "luminosity" method.
	 * Result is used in SURF-based algorithm, but may be used anywhere where
	 * grayscale images with nice contrast are required.
	 *
	 * @param red Image's red channel
	 * @param green Image's green channel
	 * @param blue Image's blue channel
	 * @param nXSize Width of initial image
	 * @param nYSize Height of initial image
	 * @param padfImg Array for resulting grayscale image
	 * @param nHeight Height of resulting image
	 * @param nWidth Width of resulting image
	 *
	 * @return CE_None or CE_Failure if error occurs.
	 */
	static CPLErr ConvertRGBToLuminosity(
				GDALRasterBand *red,
				GDALRasterBand *green,
				GDALRasterBand *blue,
				int nXSize, int nYSize,
				double **padfImg, int nHeight, int nWidth);

	/**
	 * Find feature points using specified integral image.
	 *
	 * @param poImg Integral image to be used
	 * @param poCollection Collection for storage detected feature points
	 * @param dfThreshold Threshold for feature point recognition. Detected feature point
	 * will have Hessian value greater than this provided threshold.
	 */
	void ExtractFeaturePoints(GDALIntegralImage *poImg,
			GDALFeaturePointsCollection *poCollection, double dfThreshold);

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
	 * @return CE_None or CE_Failure if error occurs.
	 */
	static CPLErr MatchFeaturePoints(
				GDALMatchedPointsCollection *poMatched,
				GDALFeaturePointsCollection *poFirstCollect,
				GDALFeaturePointsCollection *poSecondCollect,
				double dfThreshold);

private:
	/**
	 * Compute euclidean distance between descriptors of two feature points.
	 * It's used in comparison and matching of points.
	 *
	 * @param firstPoint First feature point to be compared
	 * @param secondPoint Second feature point to be compared
	 *
	 * @return Euclidean distance between descriptors.
	 */
	static double GetEuclideanDistance(
			GDALFeaturePoint &firstPoint, GDALFeaturePoint &secondPoint);

	/**
	 * Set provided distance values to range from 0 to 1.
	 *
	 * @param poList List of distances to be normalized
	 */
	static void NormalizeDistances(list<MatchedPointPairInfo> *poList);

	/**
	 * Compute descriptor for specified feature point.
	 *
	 * @param poPoint Feature point instance
	 * @param Integral image where feature point was found
	 */
	void SetDescriptor(GDALFeaturePoint *poPoint, GDALIntegralImage *poImg);


private:
	int octaveStart;
	int octaveEnd;
	GDALOctaveMap *poOctMap;
};

#endif /* GDALSIMPLESURF_H_ */
