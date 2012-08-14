#ifndef GDALFEATUREPOINTSCOLLECTION_H_
#define GDALFEATUREPOINTSCOLLECTION_H_

#include "GDALFeaturePoint.h"

#include "gdal.h"
#include "gdal_priv.h"

#include <vector>

using namespace std;

class GDALFeaturePointsCollection
{
public:
	GDALFeaturePointsCollection();
	GDALFeaturePointsCollection(GDALDataset* poDataset);
	virtual ~GDALFeaturePointsCollection();

	void AddPoint(GDALFeaturePoint *fp);
	GDALFeaturePoint* GetPoint(int nIndex);
	int GetSize() const;
	void Clear();

private:
	GDALDataset* poDataset;
	vector<GDALFeaturePoint*> *pPoints;
};

#endif /* GDALFEATUREPOINTSCOLLECTION_H_ */
