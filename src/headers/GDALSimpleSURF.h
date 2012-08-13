#ifndef GDALSIMPLESURF_H_
#define GDALSIMPLESURF_H_

#include "GDALOctaveMap.h"
#include "GDALIntegralImage.h"
#include "GDALFeaturePoint.h"
#include "GDALFeaturePointsCollection.h"
#include "GDALMatchedPointsCollection.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_vsi.h"

#include <list>
#include <math.h>

#define CPLFree VSIFree

class GDALSimpleSURF
{
private:
	class MatchedPointPairInfo
	{
	public:
		MatchedPointPairInfo(int nInd_1, int nInd_2, double dfDist)
		{
			ind_1 = nInd_1;
			ind_2 = nInd_2;
			euclideanDist = dfDist;
		}

		int ind_1;
		int ind_2;
		double euclideanDist;
	};

public:
	GDALSimpleSURF(int nOctaveStart, int nOctaveEnd);
	virtual ~GDALSimpleSURF();

	static CPLErr GDALConvertRGBToLuminosity(GDALRasterBand *red, GDALRasterBand *green, GDALRasterBand *blue,
			int nXSize, int nYSize, double **padfImg, int nHeight, int nWidth);

	void ExtractFeaturePoints(GDALIntegralImage *poImg,
			GDALFeaturePointsCollection *poCollection, double dfThreshold);

	void MatchFeaturePoints(GDALMatchedPointsCollection *poMatched,
			GDALFeaturePointsCollection *poCollect_1,
			GDALFeaturePointsCollection *poCollect_2,
			double dfThreshold);

private:
	double GetEuclideanDistance(
				GDALFeaturePoint &poPoint_1, GDALFeaturePoint &poPoint_2);

	void NormalizeDistances(list<MatchedPointPairInfo> *poList);

	void SetDescriptor(GDALFeaturePoint *poPoint, GDALIntegralImage *poImg);


private:
	int octaveStart;
	int octaveEnd;
	GDALOctaveMap *poOctMap;
};

#endif /* GDALSIMPLESURF_H_ */
