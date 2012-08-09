#include "GDALIntegralImage.h"

GDALIntegralImage::GDALIntegralImage()
{
	pMatrix = 0;
	nHeight = 0;
	nWidth = 0;
}

int GDALIntegralImage::GetHeight() { return nHeight; }

int GDALIntegralImage::GetWidth() { return nWidth; }

/*
 * Calculation of the integral image (summed area table)
 * padfImg - 2-dimensional array of grayscale image pixel values
 * height x width
 */
void GDALIntegralImage::Initialize(double **padfImg, int nHeight, int nWidth)
{
	//Memory allocation
	pMatrix = new double*[nHeight];
	for (int i = 0; i < nHeight; i++)
		pMatrix[i] = new double[nWidth];

	this->nHeight = nHeight;
	this->nWidth = nWidth;

	//Integral image calculation
	for (int i = 0; i < nHeight; i++)
		for (int j = 0; j < nWidth; j++)
		{
			double val = padfImg[i][j];
			double a = 0, b = 0, c = 0;

			if (i - 1 >= 0 && j - 1 >= 0)
				a = pMatrix[i - 1][j - 1];
			if (j - 1 >= 0)
				b = pMatrix[i][j - 1];
			if (i - 1 >= 0)
				c = pMatrix[i - 1][j];

			//New value based on previous calculations
			pMatrix[i][j] = val - a + b + c;
		}
}

/*
 * Returns value of specified cell
 */
double GDALIntegralImage::GetValue(int nRow, int nCol)
{
	if ((nRow >= 0 && nRow < nHeight) && (nCol >= 0 && nCol < nWidth))
		return pMatrix[nRow][nCol];
	else
		return 0;
}

double GDALIntegralImage::GetRectangleSum(int nRow, int nCol, int nWidth, int nHeight)
{
	double a = 0, b = 0, c = 0, d = 0;

	//Left top point of rectangle is first
	int w = nWidth - 1;
	int h = nHeight - 1;

	int row = nRow;
	int col = nCol;

	//Left top point
	int lt_row = (row <= this->nHeight) ? (row - 1) : -1;
	int lt_col = (col <= this->nWidth) ? (col - 1) : -1;
	//Right bottom point of the rectangle
	int rb_row = (row + h < this->nHeight) ? (row + h) : (this->nHeight - 1);
	int rb_col = (col + w < this->nWidth) ? (col + w) : (this->nWidth - 1);

	if (lt_row >= 0 && lt_col >= 0)
		a = this->GetValue(lt_row, lt_col);

	if (lt_row >= 0 && rb_col >= 0)
		b = this->GetValue(lt_row, rb_col);

	if (rb_row >= 0 && rb_col >= 0)
		c = this->GetValue(rb_row, rb_col);

	if (rb_row >= 0 && lt_col >= 0)
		d = this->GetValue(rb_row, lt_col);

	double res = a + c - b - d;

	return (res > 0) ? res : 0;
}

double GDALIntegralImage::HaarWavelet_X(int nRow, int nCol, int nSize)
{
	return GetRectangleSum(nRow, nCol + nSize / 2, nSize / 2, nSize)
			- GetRectangleSum(nRow, nCol, nSize / 2, nSize);
}

double GDALIntegralImage::HaarWavelet_Y(int nRow, int nCol, int nSize)
{
	return GetRectangleSum(nRow + nSize / 2, nCol, nSize, nSize / 2)
			- GetRectangleSum(nRow, nCol, nSize, nSize / 2);
}

GDALIntegralImage::~GDALIntegralImage()
{
	//Clean up memory
	for (int i = 0; i < nHeight; i++)
		delete[] pMatrix[i];

	delete[] pMatrix;
}

