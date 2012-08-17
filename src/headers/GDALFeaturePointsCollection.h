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
