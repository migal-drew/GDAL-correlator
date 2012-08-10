#ifndef GDALMATCHEDPOINTSCOLLECTION_H_
#define GDALMATCHEDPOINTSCOLLECTION_H_

#include "gdal.h"
#include "GDALFeaturePoint.h"
#include "GDALFeaturePointsCollection.h"

class GDALMatchedPointsCollection
{
public:
	GDALMatchedPointsCollection();
	virtual ~GDALMatchedPointsCollection();

	void AddPoints(GDALFeaturePoint *poPoint_1, GDALFeaturePoint *poPoint_2);

	void GetPoints(int nIndex, GDALFeaturePoint *poPoint_1,
			GDALFeaturePoint *poPoint_2);

	int GetSize() const;

	void Clear();

private:
	GDALFeaturePointsCollection *poCollect_1;
	GDALFeaturePointsCollection *poCollect_2;
};

#endif /* GDALMATCHEDPOINTSCOLLECTION_H_ */
