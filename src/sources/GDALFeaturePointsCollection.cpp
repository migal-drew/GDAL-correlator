#include "GDALFeaturePointsCollection.h"

GDALFeaturePointsCollection::GDALFeaturePointsCollection()
{
	pPoints = new vector<GDALFeaturePoint*>();
}

void GDALFeaturePointsCollection::Add(GDALFeaturePoint *poPoint)
{
	//Copies Feature point
	GDALFeaturePoint *poCopyPoint = new GDALFeaturePoint(*poPoint);

	pPoints->push_back(poCopyPoint);
}

GDALFeaturePoint& GDALFeaturePointsCollection::operator [] (int i)
{
	GDALFeaturePoint *p = (*pPoints)[i];

	return (*p);
}

int GDALFeaturePointsCollection::GetSize() const
{
	return pPoints->size();
}

void GDALFeaturePointsCollection::Clear()
{
	for (int i = 0; i < pPoints->size(); i++)
			delete (*pPoints)[i];

	pPoints->clear();
}

GDALFeaturePointsCollection::~GDALFeaturePointsCollection()
{
	for (int i = 0; i < pPoints->size(); i++)
		delete (*pPoints)[i];

	delete pPoints;
}

