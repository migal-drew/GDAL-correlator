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
		GDALFeaturePoint *poPoint_1, GDALFeaturePoint *poPoint_2)
{
	double sum = 0;

	for (int i = 0; i < GDALFeaturePoint::DESC_SIZE; i++)
		sum += ((*poPoint_1)[i] - (*poPoint_2)[i])
			* ((*poPoint_1)[i] - (*poPoint_2)[i]);

	return sqrt(sum);
}

void GDALSimpleSURF::SetDescriptor(
		GDALFeaturePoint *poPoint, GDALIntegralImage *poImg)
{
	//Affects to the descriptor area
	const int haarScale = 20;

	//Side of the Haar wavelet
	int haarFilterSize = 2 * poPoint->GetScale();

	//Length of the side of the descriptor area
	int descSide = haarScale * poPoint->GetScale();

	//Side of the quadrant in 4x4 grid
	int quadStep = descSide / 4;

	//Side of the sub-quadrant in 5x5 regular grid of quadrant
	int subQuadStep = quadStep / 5;

	int leftTop_row = poPoint->GetY() - (descSide / 2);
	int leftTop_col = poPoint->GetX() - (descSide / 2);

	int count = 0;

	for (int r = leftTop_row; r < leftTop_row + descSide; r += quadStep)
		for (int c = leftTop_col; c < leftTop_col + descSide; c += quadStep)
		{
			double dx = 0;
			double dy = 0;
			double abs_dx = 0;
			double abs_dy = 0;

			for (int sub_r = r; sub_r < r + quadStep; sub_r += subQuadStep)
				for (int sub_c = c; sub_c < c + quadStep; sub_c += subQuadStep)
				{
					//Approximate center of sub quadrant
					int cntr_r = sub_r + subQuadStep / 2;
					int cntr_c = sub_c + subQuadStep / 2;

					//Left top point for Haar wavelet computation
					int cur_r = cntr_r - haarFilterSize / 2;
					int cur_c = cntr_c - haarFilterSize / 2;

					//Gradients
					double cur_dx = poImg->HaarWavelet_X(cur_r, cur_c, haarFilterSize);
					double cur_dy = poImg->HaarWavelet_Y(cur_r, cur_c, haarFilterSize);

					dx += cur_dx;
					dy += cur_dy;
					abs_dx += fabs(cur_dx);
					abs_dy += fabs(cur_dy);
				}

			//Fills point's descriptor
			(*poPoint)[count++] = dx;
			(*poPoint)[count++] = dy;
			(*poPoint)[count++] = abs_dx;
			(*poPoint)[count++] = abs_dy;
		}
}

void GDALSimpleSURF::MatchFeaturePoints(GDALFeaturePointsCollection *poColllect_1,
			GDALFeaturePointsCollection *poColllect_2, double dfThreshold)
{

}

GDALSimpleSURF::~GDALSimpleSURF()
{
	delete poOctMap;
}

