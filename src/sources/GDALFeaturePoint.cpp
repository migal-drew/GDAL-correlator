#include "GDALFeaturePoint.h"

GDALFeaturePoint::GDALFeaturePoint()
{
	nX =      -1;
	nY =      -1;
	nScale =  -1;
	nRadius = -1;
	nSign =   -1;

	padfDescriptor = new double[DESC_SIZE];
}

GDALFeaturePoint::GDALFeaturePoint(const GDALFeaturePoint& fp)
{
	nX = fp.nX;
	nY = fp.nY;
	nScale = fp.nScale;
	nRadius = fp.nRadius;
	nSign = fp.nSign;

	padfDescriptor = new double[DESC_SIZE];
	for (int i = 0; i < DESC_SIZE; i++)
		padfDescriptor[i] = fp.padfDescriptor[i];
}

GDALFeaturePoint::GDALFeaturePoint(int nX, int nY,
		int nScale, int nRadius, int nSign)
{
	this->nX = nX;
	this->nY = nY;
	this->nScale = nScale;
	this->nRadius = nRadius;
	this->nSign = nSign;
}

GDALFeaturePoint& GDALFeaturePoint::operator = (const GDALFeaturePoint& point)
{
	if (this != &point)
	{
		nX = point.nX;
		nY = point.nY;
		nScale = point.nScale;
		nRadius = point.nRadius;
		nSign = point.nSign;

		//Frees memory
		delete[] padfDescriptor;

		//Copies descriptor values
		padfDescriptor = new double[DESC_SIZE];
		for (int i = 0; i < DESC_SIZE; i++)
			padfDescriptor[i] = point.padfDescriptor[i];
	}

	return *this;
}

int  GDALFeaturePoint::GetX() { return nX; }
void GDALFeaturePoint::SetX(int nX) { this->nX = nX; }

int  GDALFeaturePoint::GetY() { return nY; }
void GDALFeaturePoint::SetY(int nY) { this->nY = nY; }

int  GDALFeaturePoint::GetScale() { return nScale; }
void GDALFeaturePoint::SetScale(int nScale) { this->nScale = nScale; }

int  GDALFeaturePoint::GetRadius() { return nRadius; }
void GDALFeaturePoint::SetRadius(int nRadius) { this->nRadius = nRadius; }

int  GDALFeaturePoint::GetSign() { return nSign; }
void GDALFeaturePoint::SetSign(int nSign) { this->nSign = nSign; }

double& GDALFeaturePoint::operator [] (int i)
{
	return padfDescriptor[i];
}

GDALFeaturePoint::~GDALFeaturePoint() {
	delete[] padfDescriptor;
}

