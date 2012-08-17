#include "GDALFeaturePointsCollection.h"

GDALFeaturePointsCollection::GDALFeaturePointsCollection()
{
	pPoints = new vector<GDALFeaturePoint*>();
	poDataset = NULL;
}

GDALFeaturePointsCollection::GDALFeaturePointsCollection(GDALDataset* poDataset)
{
	this->pPoints = new vector<GDALFeaturePoint*>();
	this->poDataset = poDataset;
}

GDALDataset* GDALFeaturePointsCollection::GetDataset()
{
	return poDataset;
}

void GDALFeaturePointsCollection::SetDataset(GDALDataset *poDataset)
{
	this->poDataset = poDataset;
}

void GDALFeaturePointsCollection::AddPoint(GDALFeaturePoint *poPoint)
{
	if (poPoint != NULL)
		pPoints->push_back(poPoint);
}

GDALFeaturePoint* GDALFeaturePointsCollection::GetPoint(int nIndex)
{
	if (nIndex < 0 || nIndex >= this->GetSize())
		return NULL;

	return pPoints->at(nIndex);
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

