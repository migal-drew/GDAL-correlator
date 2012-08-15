#include "GDALCorrelator.h"

GDALCorrelator::GDALCorrelator()
{
	//
}

GDALCorrelator::~GDALCorrelator()
{
	//
}

CPLErr GDALCorrelator::GatherFeaturePoints(
			GDALDataset* poDataset, int* panBands,
			GDALFeaturePointsCollection* poCollection,
			int nOctaveStart, int nOctaveEnd, double dfThreshold)
{
	if (poDataset == NULL)
	{
		CPLError(CE_Failure, CPLE_AppDefined, "GDALDataset isn't specified");
		return CE_Failure;
	}

	if (poCollection == NULL)
	{
		CPLError(CE_Failure, CPLE_AppDefined,
				"GDALFeaturePointsCollection isn't specified");
		return CE_Failure;
	}

	if (panBands == NULL)
	{
		CPLError(CE_Failure, CPLE_AppDefined,
						"Raster bands are not specified");
		return CE_Failure;
	}

	if (nOctaveStart < 0 || nOctaveEnd < 0 ||
			nOctaveStart > nOctaveEnd)
	{
		CPLError(CE_Failure, CPLE_AppDefined,
						"Octave numbers are invalid");
		return CE_Failure;
	}

	if (dfThreshold < 0)
	{
		CPLError(CE_Failure, CPLE_AppDefined,
				"Threshold have to be greater than zero");
		return CE_Failure;
	}

	GDALRasterBand *poRstRedBand = poDataset->GetRasterBand(panBands[0]);
	GDALRasterBand *poRstGreenBand = poDataset->GetRasterBand(panBands[1]);
	GDALRasterBand *poRstBlueBand = poDataset->GetRasterBand(panBands[2]);

	int nWidth = poRstRedBand->GetXSize();
	int nHeight = poRstRedBand->GetYSize();

	// Allocate memory for grayscale image
	double **padfImg = NULL;
	padfImg = new double*[nHeight];
	for (int i = 0; i < nHeight; i++)
		padfImg[i] = new double[nWidth];

	// Create grayscale image
	GDALSimpleSURF::ConvertRGBToLuminosity(
			poRstRedBand, poRstGreenBand, poRstBlueBand, nWidth, nHeight,
			padfImg, nHeight, nWidth);

	// Prepare integral image
	GDALIntegralImage *poImg = new GDALIntegralImage();
	poImg->Initialize(padfImg, nHeight, nWidth);

	// Get feature points
	GDALSimpleSURF *poSurf = new GDALSimpleSURF(nOctaveStart, nOctaveEnd);
	poSurf->ExtractFeaturePoints(poImg, poCollection, dfThreshold);

	// Clean up
	delete poImg;
	delete poSurf;

	for (int i = 0; i < nHeight; i++)
		delete[] padfImg[i];

	delete[] padfImg;

	return CE_None;
}

CPLErr GDALCorrelator::MatchFeaturePoints(
			GDALMatchedPointsCollection* poMatched,
			GDALFeaturePointsCollection* poCollection_1,
			GDALFeaturePointsCollection* poCollection_2,
			double dfThreshold)
{
	GDALSimpleSURF::MatchFeaturePoints(poMatched,
			poCollection_1, poCollection_2, dfThreshold);

	return CE_None;
}
