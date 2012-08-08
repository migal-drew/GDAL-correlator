#ifndef GDALFEATUREPOINT_H_
#define GDALFEATUREPOINT_H_

class GDALFeaturePoint
{
public:
	GDALFeaturePoint();
	GDALFeaturePoint(int nX, int nY, int nScale, int nRadius, int nSign);
	virtual ~GDALFeaturePoint();

	//Descriptor length
	static const int DESC_SIZE = 64;
	//Access to descriptor
	double& operator[](int i);

	int  GetX();
	void SetX(int nX);

	int  GetY();
	void SetY(int nY);

	int  GetScale();
	void SetScale(int nScale);

	int  GetRadius();
	void SetRadius(int nRadius);

	int  GetSign();
	void SetSign(int nSign);

private:
	//Coordinates of point in image
	int nX;
	int nY;
	//--------------------
	int nScale;
	int nRadius;
	int nSign;

	double *padfDescriptor;
};

#endif /* GDALFEATUREPOINT_H_ */
