#ifndef GDALSIMPLESURF_H_
#define GDALSIMPLESURF_H_

#include "GDALOctaveMap.h"
#include "GDALIntegralImage.h"
#include "GDALFeaturePoint.h"
#include "GDALFeaturePointsCollection.h"

#include <math.h>

class GDALSimpleSURF
{
public:
	GDALSimpleSURF(int nOctaveStart, int nOctaveEnd);
	virtual ~GDALSimpleSURF();

	void ExtractFeaturePoints(GDALIntegralImage *poImg,
			GDALFeaturePointsCollection *poCollection, double dfThreshold);

	double GetEuclideanDistance(
			GDALFeaturePoint *poPoint_1, GDALFeaturePoint *poPoint_2);

	void SetDescriptor(GDALFeaturePoint *poPoint, GDALIntegralImage *poImg);

	void MatchFeaturePoints(GDALFeaturePointsCollection *poColllect_1,
			GDALFeaturePointsCollection *poColllect_2, double dfThreshold);

private:
	int octaveStart;
	int octaveEnd;
	GDALOctaveMap *poOctMap;
};

#endif /* GDALSIMPLESURF_H_ */
