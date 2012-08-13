#include "GDALSimpleSURF.h"

GDALSimpleSURF::GDALSimpleSURF(int nOctaveStart, int nOctaveEnd)
{
	this->octaveStart = nOctaveStart;
	this->octaveEnd = nOctaveEnd;
}

CPLErr GDALSimpleSURF::GDALConvertRGBToLuminosity(GDALRasterBand *red, GDALRasterBand *green, GDALRasterBand *blue,
		int nXSize, int nYSize, double **padfImg, int nHeight, int nWidth)
{
    const double forRed = 0.21;
    const double forGreen = 0.72;
    const double forBlue = 0.07;

	if (red == NULL || green == NULL || blue == NULL)
	{
		CPLError(CE_Failure, CPLE_AppDefined, "Raster bands are not specified");
		return CE_Failure;
	}

	if (padfImg == NULL)
	{
		CPLError(CE_Failure, CPLE_AppDefined, "Buffer isn't specified");
		return CE_Failure;
	}

	GDALDataType eRedType = red->GetRasterDataType();
	GDALDataType eGreenType = green->GetRasterDataType();
	GDALDataType eBlueType = blue->GetRasterDataType();

	int dataRedSize = GDALGetDataTypeSize(eRedType) / 8;
	int dataGreenSize = GDALGetDataTypeSize(eGreenType) / 8;
	int dataBlueSize = GDALGetDataTypeSize(eBlueType) / 8;

	void *paRedLayer = CPLMalloc(dataRedSize * nWidth * nHeight);
	void *paGreenLayer = CPLMalloc(dataGreenSize * nWidth * nHeight);
	void *paBlueLayer = CPLMalloc(dataBlueSize * nWidth * nHeight);

	red->RasterIO(GF_Read, 0, 0, nXSize, nYSize, paRedLayer, nWidth, nHeight, eRedType, 0, 0);
	green->RasterIO(GF_Read, 0, 0, nXSize, nYSize, paGreenLayer, nWidth, nHeight, eGreenType, 0, 0);
	blue->RasterIO(GF_Read, 0, 0, nXSize, nYSize, paBlueLayer, nWidth, nHeight, eBlueType, 0, 0);

	double maxValue = 255.0;
	for (int row = 0; row < nHeight; row++)
		for (int col = 0; col < nWidth; col++)
		{
			//Get RGB values
			double dfRedVal = SRCVAL(paRedLayer, eRedType, nWidth * row + col * dataRedSize);
			double dfGreenVal = SRCVAL(paGreenLayer, eGreenType, nWidth * row + col * dataGreenSize);
			double dfBlueVal = SRCVAL(paBlueLayer, eBlueType, nWidth * row + col * dataBlueSize);
			//Compute luminosity value
			padfImg[row][col] = (dfRedVal * forRed + dfGreenVal * forGreen
					+ dfBlueVal * forBlue) / maxValue;
		}

	CPLFree(paRedLayer);
	CPLFree(paGreenLayer);
	CPLFree(paBlueLayer);

	return CE_None;
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
					{
						GDALFeaturePoint *poFP = new GDALFeaturePoint(j, i, mid->scale,
								mid->radius, mid->signs[i][j]);
						SetDescriptor(poFP, poImg);

						poCollection->Add(poFP);
						delete poFP;
					}
		}
	}
}

double GDALSimpleSURF::GetEuclideanDistance(
		GDALFeaturePoint &poPoint_1, GDALFeaturePoint &poPoint_2)
{
	double sum = 0;

	for (int i = 0; i < GDALFeaturePoint::DESC_SIZE; i++)
		sum += (poPoint_1[i] - poPoint_2[i]) * (poPoint_1[i] - poPoint_2[i]);

	return sqrt(sum);
}

void GDALSimpleSURF::NormalizeDistances(list<MatchedPointPairInfo> *poList)
{
	double max = 0;

	list<MatchedPointPairInfo>::iterator i;
	for (i = poList->begin(); i != poList->end(); i++)
		if ((*i).euclideanDist > max)
			max = (*i).euclideanDist;

	//Normalize distances to one
	if (max != 0)
	{
		for (i = poList->begin(); i != poList->end(); i++)
			(*i).euclideanDist /= max;
	}
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

void GDALSimpleSURF::MatchFeaturePoints(GDALMatchedPointsCollection *poMatched,
		GDALFeaturePointsCollection *poCollect_1,
		GDALFeaturePointsCollection *poCollect_2,
		double dfThreshold)
{
	//Affects to pruning part
	const double ratioThreshold = 0.8;

	int len_1 = poCollect_1->GetSize();
	int len_2 = poCollect_2->GetSize();

	int minLength = (len_1 < len_2) ? len_1 : len_2;

	//Tmp pointers. Used to swap collections
	//and make p_1 minimal collection
	GDALFeaturePointsCollection *p_1;
	GDALFeaturePointsCollection *p_2;

	bool isSwap = false;

	//Assign p_1 - collection with minimal number of points
	if (minLength == len_2)
	{
		p_1 = poCollect_2;
		p_2 = poCollect_1;

		int tmp = 0;
		tmp = len_1;
		len_1 = len_2;
		len_2 = tmp;
		isSwap = true;
	}
	else
	{
		//Assignment 'as is'
		p_1 = poCollect_1;
		p_2 = poCollect_2;
		isSwap = false;
	}

	/*
	 * Stores matched point indexes and
	 * their euclidean distances
	 * 1st point | 2nd point | distance
	 */
	list<MatchedPointPairInfo> *poPairInfoList =
			new list<MatchedPointPairInfo>();

	/*
	 * Flags that points in the 2nd collection are matched or not
	 */
	bool *alreadyMatched = new bool[len_2];
	for (int i = 0; i < len_2; i++)
		alreadyMatched[i] = false;

	for (int i = 0; i < len_1; i++)
	{
		//Distance to the nearest point
		double bestDist = -1;
		//Index of the nearest point in p_2 collection
		int bestIndex = -1;

		//Distance to the 2nd nearest point
		double bestDist_2 = -1;

		//Find the nearest and 2nd nearest points
		for (int j = 0; j < len_2; j++)
			if (!alreadyMatched[j])
				if ((*p_1)[i].GetSign() == (*p_2)[j].GetSign())
				{
					//Gets distance between two feature points
					double curDist = GetEuclideanDistance((*p_1)[i], (*p_2)[j]);

					if (bestDist == -1)
					{
						bestDist = curDist;
						bestIndex = j;
					}
					else
					{
						if (curDist < bestDist)
						{
							bestDist = curDist;
							bestIndex = j;
						}
					}

					/*
					 * Findes the 2nd nearest point
					 */
					if (bestDist_2 < 0)
						bestDist_2 = curDist;
					else
						if (curDist > bestDist && curDist < bestDist_2)
							bestDist_2 = curDist;
				}
		/*
		 * FALSE DETECTION PRUNING
		 * If ratio (1st / 2nd) > 0.8 - this is the false detection
		 */
		if (bestDist_2 > 0 && bestDist >= 0)
			if (bestDist / bestDist_2 < ratioThreshold)
			{
				MatchedPointPairInfo info(i, bestIndex, bestDist);
				poPairInfoList->push_back(info);
				alreadyMatched[bestIndex] = true;
			}
	}

	NormalizeDistances(poPairInfoList);

	//Pruning based on the custom threshold (0..1)
	list<MatchedPointPairInfo>::const_iterator iter;
	for (iter = poPairInfoList->begin(); iter != poPairInfoList->end(); iter++)
	{
		if ((*iter).euclideanDist <= dfThreshold)
		{
			int i_1 = (*iter).ind_1;
			int i_2 = (*iter).ind_2;

			GDALFeaturePoint *poPoint_1 = &((*p_1)[i_1]);
			GDALFeaturePoint *poPoint_2 = &((*p_2)[i_2]);

			if(!isSwap)
			{
				poMatched->AddPoints(poPoint_1, poPoint_2);
			}
			else
			{
				poMatched->AddPoints(poPoint_2, poPoint_1);
			}
		}
	}
}

GDALSimpleSURF::~GDALSimpleSURF()
{
	delete poOctMap;
}

