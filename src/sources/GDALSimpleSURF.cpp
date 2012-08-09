#include "GDALSimpleSURF.h"

GDALSimpleSURF::GDALSimpleSURF(int nOctaveStart, int nOctaveEnd)
{
	this->octaveStart = nOctaveStart;
	this->octaveEnd = nOctaveEnd;
}

void GDALSimpleSURF::ExtractFeaturePoints(GDALIntegralImage *poImg,
			GDALFeaturePointsCollection *poCollection, double dfThreshold)
{
	//Initialize Octave map with custom range
	poOctMap = new GDALOctaveMap(octaveStart, octaveEnd);

	//Calc Hessian values for layers
	poOctMap->ComputeMap(poImg);

	//Search for exremum points
	for (int oct = octaveStart; oct <= octaveEnd; oct++)
	{
		for (int k = 0; k < GDALOctaveMap::INTERVALS - 2; k++)
		{
			GDALOctaveLayer *bot = poOctMap->pMap[oct - 1][k];
			GDALOctaveLayer *mid = poOctMap->pMap[oct - 1][k + 1];
			GDALOctaveLayer *top = poOctMap->pMap[oct - 1][k + 2];

			for (int i = 0; i < mid->height; i++)
				for (int j = 0; j < mid->width; j++)
					if (poOctMap->PointIsExtremum(i, j, bot, mid, top, dfThreshold))
						poCollection->Add(new GDALFeaturePoint(j, i, mid->scale,
										mid->radius, mid->signs[i][j]));
		}
	}
}

double GDALSimpleSURF::GetEuclideanDistance(
		GDALFeaturePoint *poPoint_1, GDALFeaturePoint *poPoint_2,int size)
{
	double sum = 0;

	for (int i = 0; i < size; i++)
		sum += (poPoint_1[i] - poPoint_2[i]) * (poPoint_1[i] - poPoint_2[i]);

	return sqrt(sum);
}

void GDALSimpleSURF::MatchFeaturePoints(GDALFeaturePointsCollection *poColllect_1,
			GDALFeaturePointsCollection *poColllect_2, double dfThreshold)
{

}

GDALSimpleSURF::~GDALSimpleSURF()
{
	delete poOctMap;
}

