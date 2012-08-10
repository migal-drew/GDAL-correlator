#include "GDALMatchedPointsCollection.h"

GDALMatchedPointsCollection::GDALMatchedPointsCollection()
{
	poCollect_1 = new GDALFeaturePointsCollection();
	poCollect_2 = new GDALFeaturePointsCollection();
}

void GDALMatchedPointsCollection::AddPoints(
		GDALFeaturePoint *poPoint_1, GDALFeaturePoint *poPoint_2)
{
	poCollect_1->Add(poPoint_1);
	poCollect_2->Add(poPoint_2);
}

void GDALMatchedPointsCollection::GetPoints(
		int nIndex, GDALFeaturePoint *poPoint_1, GDALFeaturePoint *poPoint_2)
{
	int size = this->GetSize();

	if (poPoint_1 == NULL || poPoint_2 == NULL || nIndex < 0 || nIndex >= size)
		return;

	(*poPoint_1) = (*poCollect_1)[nIndex];
	(*poPoint_2) = (*poCollect_2)[nIndex];
}

int GDALMatchedPointsCollection::GetSize() const
{
	return poCollect_1->GetSize();
}

GDALMatchedPointsCollection::~GDALMatchedPointsCollection()
{

}
