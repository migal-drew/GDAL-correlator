#ifndef GDALFEATUREPOINTSCOLLECTION_H_
#define GDALFEATUREPOINTSCOLLECTION_H_

#include <vector>

using namespace std;

class GDALFeaturePointsCollection
{
public:
	GDALFeaturePointsCollection();
	virtual ~GDALFeaturePointsCollection();

	void Add(GDALFeaturePoint *fp);
	int GetSize() const;
	void Clear();

	GDALFeaturePoint& operator[](int i);

private:
	vector<GDALFeaturePoint*> *pPoints;
};

#endif /* GDALFEATUREPOINTSCOLLECTION_H_ */
