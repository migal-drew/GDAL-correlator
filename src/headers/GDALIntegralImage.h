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
 * @brief Integral image class (summed area table).
 */

#ifndef GDALINTEGRALIMAGE_H_
#define GDALINTEGRALIMAGE_H_

/**
 * @author Andrew Migal migal.drew@gmail.com
 * @brief Integral image class (summed area table).
 * @details Integral image is a table for fast computing the sum of
 * values in rectangular subarea. In more detail, for 2-dimensional array
 * of numbers this class provides capabilty to get sum of values in
 * rectangular arbitrary area with any size in constant time.
 * Integral image is constructed from grayscale picture.
 */
class GDALIntegralImage
{
public:
	GDALIntegralImage();
	virtual ~GDALIntegralImage();

	/**
	 * Compute integral image for specified array. Result is stored internally.
	 *
	 * @param padfImg Pointer to 2-dimensional array of values
	 * @param nHeight Number of rows in array
	 * @param nWidth Number of columns in array
	 */
	void Initialize(const double **padfImg, int nHeight, int nWidth);

	/**
	 * Fetch value of specified position in integral image.
	 *
	 * @param nRow Row of this position
	 * @param nCol Column of this position
	 *
	 * @return Value in specified position or zero if parameters are out of range.
	 */
	double GetValue(int nRow, int nCol);

	/**
	 * Get sum of values in specified rectangular grid. Rectangle is constructed
	 * from left top point.
	 *
	 * @param nRow Row of left top point of rectangle
	 * @param nCol Column of left top point of rectangle
	 * @param nWidth Width of rectangular area (number of columns)
	 * @param Height Heigth of rectangular area (number of rows)
	 *
	 * @return Sum of values in specified grid.
	 */
	double GetRectangleSum(int nRow, int nCol, int nWidth, int nHeight);

	/**
	 * Get value of horizontal Haar wavelet in specified square grid.
	 *
	 * @param nRow Row of left top point of square
	 * @param nCol Column of left top point of square
	 * @param nSize Side of the square
	 *
	 * @return Value of horizontal Haar wavelet in specified square grid.
	 */
	double HaarWavelet_X(int nRow, int nCol, int nSize);

	/**
	 * Get value of vertical Haar wavelet in specified square grid.
	 *
	 * @param nRow Row of left top point of square
	 * @param nCol Column of left top point of square
	 * @param nSize Side of the square
	 *
	 * @return Value of vertical Haar wavelet in specified square grid.
	 */
	double HaarWavelet_Y(int nRow, int nCol, int nSize);

	/**
	 * Fetch height of integral image.
	 *
	 * @return Height of integral image (number of rows).
	 */
	int GetHeight();

	/**
	 * Fetch width of integral image.
	 *
	 * @return Width of integral image (number of columns).
	 */
	int GetWidth();

private:
	double **pMatrix;
	int nWidth;
	int nHeight;
};

#endif /* GDALINTEGRALIMAGE_H_ */
