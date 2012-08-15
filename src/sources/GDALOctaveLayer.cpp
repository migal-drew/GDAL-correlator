#include "GDALOctaveLayer.h"

GDALOctaveLayer::GDALOctaveLayer(int nOctave, int nInterval)
{
	this->octaveNum = nOctave;
	this->filterSize = 3 * ((int)pow(2, nOctave) * nInterval + 1);
	this->radius = (this->filterSize - 1) / 2;
	this->scale = (int)pow(2, nOctave);
	this->width = 0;
	this->height = 0;

	this->detHessians = 0;
	this->signs = 0;
}

void GDALOctaveLayer::ComputeLayer(GDALIntegralImage *poImg)
{
	this->width = poImg->GetWidth();
	this->height = poImg->GetHeight();

	//Allocate memory for arrays
	this->detHessians = new double*[this->height];
	this->signs = new int*[this->height];

	for (int i = 0; i < this->height; i++)
	{
		this->detHessians[i] = new double[this->width];
		this->signs[i] = new int[this->width];
	}
	//End Allocate memory for arrays

	//Values of Fast Hessian filters
	double dxx, dyy, dxy;
	// 1/3 of filter side
	int lobe = filterSize / 3;

	//Length of the longer side of the lobe in dxx and dyy filters
	int longPart = 2 * lobe - 1;

	int normalization = filterSize * filterSize;

	//Loop over image pixels
	//Filter should remain into image borders
	for (int r = radius; r <= height - radius; r++)
		for (int c = radius; c <= width - radius; c++)
		{
			dxx = poImg->GetRectangleSum(r - lobe + 1, c - radius, filterSize, longPart)
				- 3 * poImg->GetRectangleSum(r - lobe + 1, c - (lobe - 1) / 2, lobe, longPart);
			dyy = poImg->GetRectangleSum(r - radius, c - lobe - 1, longPart, filterSize)
				- 3 * poImg->GetRectangleSum(r - lobe + 1, c - lobe + 1, longPart, lobe);
			dxy = poImg->GetRectangleSum(r - lobe, c - lobe, lobe, lobe)
				+ poImg->GetRectangleSum(r + 1, c + 1, lobe, lobe)
				- poImg->GetRectangleSum(r - lobe, c + 1, lobe, lobe)
				- poImg->GetRectangleSum(r + 1, c - lobe, lobe, lobe);

			dxx /= normalization;
			dyy /= normalization;
			dxy /= normalization;

			//Memorize Hessian values and their signs
			detHessians[r][c] = dxx * dyy - 0.9 * 0.9 * dxy * dxy;
			signs[r][c] = (dxx + dyy >= 0) ? 1 : -1;

			double t;
			t = detHessians[r][c];

			int tmp = 0;
		}
}

GDALOctaveLayer::~GDALOctaveLayer()
{
	for (int i = 0; i < height; i++)
	{
		delete[] detHessians[i];
		delete[] signs[i];
	}

	delete[] detHessians;
	delete[] signs;
}

