#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "cpl_vsi.h"

#include "GDALIntegralImage.h"
#include "GDALSimpleSURF.h"
#include "GDALFeaturePointsCollection.h"

#include <fstream>

using namespace std;

void printInfo(GDALDataset *poDataset_1)
{
	printf( "Driver: %s/%s\n",
			poDataset_1->GetDriver()->GetDescription(),
			poDataset_1->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME ) );
	printf( "Size %dx%dx%d\n",
			poDataset_1->GetRasterXSize(), poDataset_1->GetRasterYSize(),
			poDataset_1->GetRasterCount() );
	if( poDataset_1->GetProjectionRef()  != NULL )
		printf( "Projection \"%s\"\n", poDataset_1->GetProjectionRef() );
}



int main(int argc, char* argv[])
{
    GDALAllRegister();

    GDALDataset  *poDataset_1;
    GDALDataset  *poDataset_2;

    poDataset_1 = (GDALDataset *) GDALOpen( "/home/andrew/workspace/GDAL-correlator/Debug/1.jpg", GA_ReadOnly );
	if( poDataset_1 == NULL )
       return 0;

	poDataset_2 = (GDALDataset *) GDALOpen( "/home/andrew/workspace/GDAL-correlator/Debug/2.jpg", GA_ReadOnly );
	if( poDataset_2 == NULL )
		return 0;

	GDALRasterBand *poRstRedBand = poDataset_1->GetRasterBand(1);
	GDALRasterBand *poRstGreenBand = poDataset_1->GetRasterBand(2);
	GDALRasterBand *poRstBlueBand = poDataset_1->GetRasterBand(3);

	int nWidth = poRstRedBand->GetXSize();
	int nHeight = poRstRedBand->GetYSize();
	double **padfImg = NULL;

	padfImg = new double*[nHeight];
	for (int i = 0; i < nHeight; i++)
		padfImg[i] = new double[nWidth];

	GDALRasterBand *poRstRedBand_2 = poDataset_2->GetRasterBand(1);
	GDALRasterBand *poRstGreenBand_2 = poDataset_2->GetRasterBand(2);
	GDALRasterBand *poRstBlueBand_2 = poDataset_2->GetRasterBand(3);

	int nWidth_2 = poRstRedBand_2->GetXSize();
	int nHeight_2 = poRstRedBand_2->GetYSize();;
	double **padfImg_2 = NULL;

	padfImg_2 = new double*[nHeight_2];
	for (int i = 0; i < nHeight_2; i++)
		padfImg_2[i] = new double[nWidth_2];

	GDALSimpleSURF::GDALConvertRGBToLuminosity(poRstRedBand, poRstGreenBand, poRstBlueBand, nWidth, nHeight,
			padfImg, nHeight, nWidth);
	GDALSimpleSURF::GDALConvertRGBToLuminosity(poRstRedBand_2, poRstGreenBand_2, poRstBlueBand_2, nWidth_2, nHeight_2,
			padfImg_2, nHeight_2, nWidth_2);

	GDALIntegralImage *poImg = new GDALIntegralImage();
	poImg->Initialize(padfImg, nHeight, nWidth);

	GDALIntegralImage *poImg_2 = new GDALIntegralImage();
	poImg_2->Initialize(padfImg_2, nHeight_2, nWidth_2);

	GDALFeaturePointsCollection *poFPCollection = new GDALFeaturePointsCollection();
	GDALFeaturePointsCollection *poFPCollection_2 = new GDALFeaturePointsCollection();

	GDALSimpleSURF *poSurf = new GDALSimpleSURF(2, 2);
	poSurf->ExtractFeaturePoints(poImg, poFPCollection, 0.001);
	poSurf->ExtractFeaturePoints(poImg_2, poFPCollection_2, 0.001);


	GDALMatchedPointsCollection *result = new GDALMatchedPointsCollection();
	poSurf->MatchFeaturePoints(result, poFPCollection, poFPCollection_2, 0.015);

	ofstream out;
	out.open("points_1.txt");
	for (int i = 0; i < poFPCollection->GetSize(); i++)
		out << (*poFPCollection)[i].GetX() << " "
			<< (*poFPCollection)[i].GetY() << endl;
	out.close();

	out.open("points_2.txt");
	for (int i = 0; i < poFPCollection_2->GetSize(); i++)
		out << (*poFPCollection_2)[i].GetX() << " "
			<< (*poFPCollection_2)[i].GetY() << endl;

	out.close();


	out.open("matched_points.txt");
	for (int i = 0; i < result->GetSize(); i++)
	{
		GDALFeaturePoint *point_1 = new GDALFeaturePoint();
		GDALFeaturePoint *point_2 = new GDALFeaturePoint();

		result->GetPoints(i, point_1, point_2);

		out << endl;
		out << point_1->GetX() << " " << point_1->GetY() << " ";
		out << "  ||   ";
		out << point_2->GetX() << " " << point_2->GetY() << " ";
		out << endl;
	}
	out.close();

	delete poDataset_1;
	delete poDataset_2;

	return 0;
}
