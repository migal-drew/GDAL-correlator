#ifndef GDALOCTAVEMAP_H_
#define GDALOCTAVEMAP_H_

#include "GDALIntegralImage.h"
#include "GDALOctaveLayer.h"

class GDALOctaveMap
{
public:
	GDALOctaveMap(int nOctaveStart, int nOctaveEnd);
	virtual ~GDALOctaveMap();

	void ComputeMap(GDALIntegralImage *poImg);

	bool PointIsExtremum(int row, int col, GDALOctaveLayer *bot,
			GDALOctaveLayer *mid, GDALOctaveLayer *top, double threshold);

	//2-dimensional array of octave layers
	GDALOctaveLayer ***pMap;

	static const int INTERVALS = 4;

	int octaveStart;
	int octaveEnd;
};

#endif /* GDALOCTAVEMAP_H_ */
