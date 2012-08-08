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

GDALFeaturePoint::GDALFeaturePoint(int nX, int nY,
		int nScale, int nRadius, int nSign)
{
	this->nX = nX;
	this->nY = nY;
	this->nScale = nScale;
	this->nRadius = nRadius;
	this->nSign = nSign;
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

double& GDALFeaturePoint::operator [](int i)
{
	return padfDescriptor[i];
}

GDALFeaturePoint::~GDALFeaturePoint() {
	delete[] padfDescriptor;
}

