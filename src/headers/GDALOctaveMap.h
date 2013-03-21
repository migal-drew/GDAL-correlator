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
 * @brief Class for handling octave layers in SURF-based algorithm.
 */

#ifndef GDALOCTAVEMAP_H_
#define GDALOCTAVEMAP_H_

#include "GDALIntegralImage.h"
#include "GDALOctaveLayer.h"

/**
 * @author Andrew Migal migal.drew@gmail.com
 * @brief Class for handling octave layers in SURF-based algorithm.
 * @details Class contains OctaveLayers and provides capability to construct octave space and distinguish
 * feature points. Normally this class is used only by SURF-based algorithm.
 */
class GDALOctaveMap
{
public:
	/**
	 * Create octave space. Octave numbers are start with one. (1, 2, 3, 4, ... )
	 *
	 * @param nOctaveStart Number of bottom octave
	 * @param nOctaveEnd Number of top octave. Should be equal or greater than OctaveStart
	 */
	GDALOctaveMap(int nOctaveStart, int nOctaveEnd);
	virtual ~GDALOctaveMap();

	/**
	 * Calculate Hessian values for octave space
	 * (for all stored octave layers) using specified integral image
	 * @param poImg Integral image instance which provides necessary data
	 * @see GDALOctaveLayer
	 */
	void ComputeMap(GDALIntegralImage *poImg);

	/**
	 * Method makes decision that specified point
	 * in middle octave layer is maximum among all points
	 * from 3x3x3 neighbourhood (surrounding points in
	 * bottom, middle and top layers). Provided layers should be from the same octave's interval.
	 * Detects feature points.
	 *
	 * @param row Row of point, which is candidate to be feature point
	 * @param col Column of point, which is candidate to be feature point
	 * @param bot Bottom octave layer
	 * @param mid Middle octave layer
	 * @param top Top octave layer
	 * @param threshold Threshold for feature point recognition. Detected feature point
	 * will have Hessian value greater than this provided threshold.
	 *
	 * @return TRUE if candidate was evaluated as feature point or FALSE otherwise.
	 */
	bool PointIsExtremum(int row, int col, GDALOctaveLayer *bot,
			GDALOctaveLayer *mid, GDALOctaveLayer *top, double threshold);

	/**
	 * 2-dimensional array of octave layers
	 */
	GDALOctaveLayer ***pMap;

	/**
	 * Value for constructing internal octave space
	 */
	static const int INTERVALS = 4;

	/**
	 * Number of bottom octave
	 */
	int octaveStart;

	/**
	 * Number of top octave. Should be equal or greater than OctaveStart
	 */
	int octaveEnd;

	/**
	 * Helper method. Copies pointers to common data from one OctaveLayer to another.
	 * (in general, Hessian values and their signs)
	 */
private:
	void CopyCommonData(GDALOctaveLayer *source, GDALOctaveLayer *destination);
};

#endif /* GDALOCTAVEMAP_H_ */
