/******************************************************************************
 * Project:  GDAL
 * Purpose:  Correlator
 * Author:   Andrew Migal, migal.drew@gmail.com
 *
 ******************************************************************************
 * Copyright (c) 2012, Andrew Migal
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ****************************************************************************/

/**
 * @file
 * @author Andrew Migal migal.drew@gmail.com
 * @brief Class for computation of Hessian values in SURF-based algorithm.
 */

#ifndef GDALOCTAVELAYER_H_
#define GDALOCTAVELAYER_H_

#include "GDALIntegralImage.h"

#include <math.h>
#include "gdal.h"

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
