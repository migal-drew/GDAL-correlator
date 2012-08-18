/**
 * @file
 * @author Andrew Migal migal.drew@gmail.com
 * @brief Class for computation of Hessian values in SURF-based algorithm.
 */

#ifndef GDALOCTAVELAYER_H_
#define GDALOCTAVELAYER_H_

#include "GDALIntegralImage.h"
#include <math.h>

/**
 * @author Andrew Migal migal.drew@gmail.com
 * @brief Class for computation and storage of Hessian values in SURF-based algorithm.
 *
 * @details SURF-based algorithm normally uses this class for searching
 * feature points on raster images. Class also contains traces of Hessian matrices
 * to provide fast computations.
 */
class GDALOctaveLayer
{
public:
	GDALOctaveLayer();

	/**
	 * Create instance with provided parameters.
	 *
	 * @param nOctave Number of octave which contains this layer
	 * @param nInterval Number of position in octave
	 *
	 * @note Normally constructor is invoked only by SURF-based algorithm.
	 */
	GDALOctaveLayer(int nOctave, int nInterval);
	virtual ~GDALOctaveLayer();

	/**
	 * Perform calculation of Hessian determinats and their signs
	 * for specified integral image. Result is stored internally.
	 *
	 * @param poImg Integral image object, which provides all necessary
	 * data for computation
	 *
	 * @note Normally method is invoked only by SURF-based algorithm.
	 */
	void ComputeLayer(GDALIntegralImage *poImg);

    /**
     * Octave which contains this layer (1,2,3...)
     */
    int octaveNum;
    /**
     * Length of the side of filter
     */
    int filterSize;
    /**
     * Length of the border
     */
    int radius;
    /**
     * Scale for this layer
     */
    int scale;
    /**
     * Image width in pixels
     */
    int width;
    /**
     * Image height in pixels
     */
    int height;
    /**
     * Hessian values for image pixels
     */
    double **detHessians;
    /**
     * Hessian signs for speeded matching
     */
    int **signs;
};

#endif /* GDALOCTAVELAYER_H_ */
