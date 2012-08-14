#ifndef GDALCORRELATOR_H_
#define GDALCORRELATOR_H_

#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "cpl_vsi.h"

#include "GDALFeaturePointsCollection.h"
#include "GDALMatchedPointsCollection.h"
#include "GDALSimpleSURF.h"
#include "GDALIntegralImage.h"

class GDALCorrelator
{
public:
	GDALCorrelator();
	virtual ~GDALCorrelator();

	CPLErr GatherFeaturePoints(GDALDataset* poDataset, int* panBands,
				GDALFeaturePointsCollection* poCollection,
				int nOctaveStart, int nOctaveEnd, double dfThreshold);

	CPLErr MatchFeaturePoints(
				GDALMatchedPointsCollection* poMatched,
				GDALFeaturePointsCollection* poCollection_1,
				GDALFeaturePointsCollection* poCollection_2,
				double dfThreshold);
};

#endif /* GDALCORRELATOR_H_ */
