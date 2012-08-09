#ifndef GDALOCTAVELAYER_H_
#define GDALOCTAVELAYER_H_

#include "GDALIntegralImage.h"
#include <math.h>

class GDALOctaveLayer
{
public:
	GDALOctaveLayer();
	GDALOctaveLayer(int nOctave, int nInterval);
	virtual ~GDALOctaveLayer();

	void ComputeLayer(GDALIntegralImage *poImg);

    //Octave which contains this layer (1,2,3...)
    int octaveNum;
    //Length of the side of filter
    int filterSize;
    //Length of the border
    int radius;
    //Scale for this layer
    int scale;
    //Image width in pixels
    int width;
    //Image height in pixels
    int height;
    //Hessian values for image pixels
    double **detHessians;
    //Hessian signs for speeded matching
    int **signs;
};

#endif /* GDALOCTAVELAYER_H_ */
