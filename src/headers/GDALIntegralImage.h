/**
 * @file
 * @author Andrew Migal migal.drew@gmail.com
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
