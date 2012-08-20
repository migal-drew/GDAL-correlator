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
 * @brief  Collection for storing pointers to instances of GDALFeaturePoint class
 */

#ifndef GDALFEATUREPOINTSCOLLECTION_H_
#define GDALFEATUREPOINTSCOLLECTION_H_

#include "GDALFeaturePoint.h"

#include "gdal.h"
#include "gdal_priv.h"

#include <vector>

using namespace std;

/**
 * @author Andrew Migal migal.drew@gmail.com
 * @brief Collection for storing pointers to instances of GDALFeaturePoint class.
 */
class GDALFeaturePointsCollection
{
public:
	GDALFeaturePointsCollection();

	/**
	 *	Create instance of class.
	 *
	 *	@param poDataset GDALDataset class instance which owns this collection
	 */
	GDALFeaturePointsCollection(GDALDataset* poDataset);

	/**
	 * Destroy collection and all stored objects.
	 */
	virtual ~GDALFeaturePointsCollection();

	/**
	 * Fetch GDALDatset instance which is owner of this collection.
	 *
	 * @return GDALDataset instance or NULL.
	 */
	GDALDataset* GetDataset();

	/**
	 * Set GDALDataset instance which is owner of this collection.
	 *
	 * @param poDataset GDALDataset instance
	 */
	void SetDataset(GDALDataset* poDataset);

	/**
	 * Add GDALFeaturePoint to collection. Doesn't create a copy and stores only pointer.
	 *
	 * @param fp Pointer to class instance
	 */
	void AddPoint(GDALFeaturePoint *fp);

	/**
	 * Fetch stored point.
	 *
	 * @param nIndex Index of object. Should be from zero to size of collection
	 * @return Pointer to object or NULL if index is out of range.
	 */
	GDALFeaturePoint* GetPoint(int nIndex);

	/**
	 * Get number of stored objects.
	 *
	 * @return Number of stored objects.
	 */
	int GetSize() const;

	/**
	 * Empty collection and delete all stored objects
	 */
	void Clear();

private:
	GDALDataset* poDataset;
	vector<GDALFeaturePoint*> *pPoints;
};

#endif /* GDALFEATUREPOINTSCOLLECTION_H_ */
