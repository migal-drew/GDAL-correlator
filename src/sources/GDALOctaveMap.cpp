#include "GDALOctaveMap.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "cpl_vsi.h"

#include <algorithm>

GDALOctaveMap::GDALOctaveMap(int nOctaveStart, int nOctaveEnd)
{
	this->octaveStart = nOctaveStart;
	this->octaveEnd = nOctaveEnd;

	pMap = new GDALOctaveLayer**[octaveEnd];

	for (int i = 0; i < nOctaveEnd; i++)
		pMap[i] = new GDALOctaveLayer*[INTERVALS];


	for (int oct = octaveStart; oct <= octaveEnd; oct++)
		for (int i = 1; i <= INTERVALS; i++)
			pMap[oct - 1][i - 1] = new GDALOctaveLayer(oct, i);

}

void GDALOctaveMap::CopyCommonData(GDALOctaveLayer *source, GDALOctaveLayer *dest)
{
	int width  = source->width;
	int height = source->height;

	dest->width  = width;
	dest->height = height;
	dest->detHessians = source->detHessians;
	dest->signs = source->signs;

	/*
	dest->detHessians = new double*[source->height];
	dest->signs = new int*[source->width];

	for (int i = 0; i < height; i++)
	{
		dest->detHessians[i] = new double[width];
		dest->signs[i] = new int[width];

		/*
		std::copy(&source->detHessians[i][0], &source->detHessians[i][width - 1],
				&dest->detHessians[i][0]);
		std::copy(&source->signs[i][0], &source->signs[i][width - 1],
				&dest->signs[i][0]);
	}
	*/
}

void GDALOctaveMap::ComputeMap(GDALIntegralImage *poImg)
{
    for (int oct = octaveStart; oct <= octaveEnd; oct++)
        for (int i = 1; i <= INTERVALS; i++)
        {
        	if (oct == octaveStart || i > 2)
        	{
				//pMap[oct - 1][i - 1] = new GDALOctaveLayer(oct, i);
				pMap[oct - 1][i - 1]->ComputeLayer(poImg);
        	}
        	else
        	{
        		if (i == 1)
				{
        			/*
					pMap[oct - 1][i - 1]->detHessians = pMap[oct - 2][i]->detHessians;
					pMap[oct - 1][i - 1]->signs = pMap[oct - 2][i]->signs;
					pMap[oct - 1][i - 1]->width = pMap[oct - 2][i]->width;
					pMap[oct - 1][i - 1]->height = pMap[oct - 2][i]->height;
					*/
					CopyCommonData(pMap[oct - 2][i], pMap[oct - 1][i - 1]);
				}
        		else if (i == 2)
        		{
        			/*
					pMap[oct - 1][i - 1]->detHessians = pMap[oct - 2][i + 1]->detHessians;
					pMap[oct - 1][i - 1]->signs = pMap[oct - 2][i + 1]->signs;
					pMap[oct - 1][i - 1]->width = pMap[oct - 2][i + 1]->width;
					pMap[oct - 1][i - 1]->height = pMap[oct - 2][i + 1]->height;
					*/
					CopyCommonData(pMap[oct - 2][i + 1], pMap[oct - 1][i - 1]);
        		}
        	}
        }
}

bool GDALOctaveMap::PointIsExtremum(int row, int col, GDALOctaveLayer *bot,
		GDALOctaveLayer *mid, GDALOctaveLayer *top, double threshold)
{
	//Check that point in middle layer has all neighbours
	if (row <= top->radius || col <= top->radius ||
		row + top->radius >= top->height || col + top->radius >= top->width)
		return false;

	double curPoint = mid->detHessians[row][col];

	//Hessian should be higher than threshold
	if (curPoint < threshold)
		return false;

	//Hessian should be higher than hessians of all neighbours
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
		{
			double topPoint = top->detHessians[row + i][col + j];
			double midPoint = mid->detHessians[row + i][col + j];
			double botPoint = bot->detHessians[row + i][col + j];

			if (topPoint >= curPoint || botPoint >= curPoint)
				return false;

			if (i != 0 || j != 0)
				if (midPoint >= curPoint)
					return false;
		}

	return true;
}

GDALOctaveMap::~GDALOctaveMap()
{
	// Clean up Octave layers
	for (int oct = octaveStart - 1; oct < octaveEnd; oct++)
		for(int i = 0; i < INTERVALS; i++)
		{
			if (oct != octaveStart - 1 && i < 2)
			{
				// We have already deleted this memory. Set to NULL to safe
				// call OctaveLayer destructor
				pMap[oct][i]->detHessians = NULL;
				pMap[oct][i]->signs = NULL;
			}
			delete pMap[oct][i];
		}

	/*
	// Clean up Octave layers
	for (int oct = octaveStart - 1; oct < octaveEnd; oct++)
		for(int i = 0; i < INTERVALS; i++)
			delete pMap[oct][i];
	*/

	//Clean up allocated memory
	for (int oct = 0; oct < octaveEnd; oct++)
		delete[] pMap[oct];

	delete[] pMap;
}
