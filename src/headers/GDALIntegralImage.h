#ifndef GDALINTEGRALIMAGE_H_
#define GDALINTEGRALIMAGE_H_

class GDALIntegralImage
{
public:
	GDALIntegralImage();
	virtual ~GDALIntegralImage();

	void Initialize(double **padfImg, int nHeight, int nWidth);
	double GetValue(int nRow, int nCol);
	double GetRectangleSum(int nRow, int nCol, int nWidth, int nHeight);
	double HaarWavelet_X(int nRow, int nCol, int nSize);
	double HaarWavelet_Y(int nRow, int nCol, int nSize);

	int GetHeight();
	int GetWidth();

private:
	double **pMatrix;
	int nWidth;
	int nHeight;
};

#endif /* GDALINTEGRALIMAGE_H_ */
