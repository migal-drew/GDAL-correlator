#include "GDALMatchedPointsCollection.h"

GDALMatchedPointsCollection::GDALMatchedPointsCollection()
{
	poCollect_1 = new GDALFeaturePointsCollection();
	poCollect_2 = new GDALFeaturePointsCollection();
}

void GDALMatchedPointsCollection::AddPoints(
		GDALFeaturePoint *poFirstPoint, GDALFeaturePoint *poSecondPoint)
{
	poCollect_1->AddPoint(poFirstPoint);
	poCollect_2->AddPoint(poSecondPoint);
}

void GDALMatchedPointsCollection::GetPoints(
		int nIndex, GDALFeaturePoint *poFirstPoint, GDALFeaturePoint *poSecondPoint)
{
	int size = this->GetSize();

	if (poFirstPoint == NULL || poSecondPoint == NULL ||
			nIndex < 0 || nIndex >= size)
		return;

	// Copy points
	*poFirstPoint = *(poCollect_1->GetPoint(nIndex));
	*poSecondPoint = *(poCollect_2->GetPoint(nIndex));
}

int GDALMatchedPointsCollection::GetSize() const
{
	return poCollect_1->GetSize();
}

GDALMatchedPointsCollection::~GDALMatchedPointsCollection()
{
	delete poCollect_1;
	delete poCollect_2;
}
