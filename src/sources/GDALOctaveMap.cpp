#include "GDALOctaveMap.h"

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

void GDALOctaveMap::ComputeMap(GDALIntegralImage *poImg)
{
    for (int oct = octaveStart; oct <= octaveEnd; oct++)
        for (int i = 1; i <= INTERVALS; i++)
            pMap[oct - 1][i - 1]->ComputeLayer(poImg);
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
	for (int i = 0; i < octaveEnd; i++)
	{
		for(int j = 0; j < INTERVALS; j++)
			delete pMap[i][j];

		delete[] pMap[i];
	}

	delete[] pMap;
}
