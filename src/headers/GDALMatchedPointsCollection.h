#ifndef GDALMATCHEDPOINTSCOLLECTION_H_
#define GDALMATCHEDPOINTSCOLLECTION_H_

class GDALMatchedPointsCollection
{
public:
	GDALMatchedPointsCollection();
	virtual ~GDALMatchedPointsCollection();

	void AddPoints(GDALFeaturePoint *poPoint_1, GDALFeaturePoint *poPoint_2);
	void GetPoints(int nIndex, GDALFeaturePoint *poPoint_1, GDALFeaturePoint *poPoint_2);
	void Clear();
};

#endif /* GDALMATCHEDPOINTSCOLLECTION_H_ */
