#include "gdal.h"
#include "gdal_priv.h"
#include "cpl_conv.h"
#include "cpl_vsi.h"

#define CPLFree VSIFree

#include <iostream>

using namespace std;

void printInfo(GDALDataset *poDataset)
{
	double adfGeoTransform[6];
	printf( "Driver: %s/%s\n",
			poDataset->GetDriver()->GetDescription(),
			poDataset->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME ) );
	printf( "Size %dx%dx%d\n",
			poDataset->GetRasterXSize(), poDataset->GetRasterYSize(),
			poDataset->GetRasterCount() );
	if( poDataset->GetProjectionRef()  != NULL )
		printf( "Projection \"%s\"\n", poDataset->GetProjectionRef() );
}

CPLErr GDALConvertRGBToLuminosity(GDALRasterBand *red, GDALRasterBand *green, GDALRasterBand *blue,
		int nXSize, int nYSize, double **padfImg, int nHeight, int nWidth)
{
    const double forRed = 0.21;
    const double forGreen = 0.72;
    const double forBlue = 0.07;

	if (red == NULL || green == NULL || blue == NULL)
	{
		CPLError(CE_Failure, CPLE_AppDefined, "Raster bands are not specified");
		return CE_Failure;
	}

	if (padfImg == NULL)
	{
		CPLError(CE_Failure, CPLE_AppDefined, "Buffer isn't specified");
		return CE_Failure;
	}

	GDALDataType eRedType = red->GetRasterDataType();
	GDALDataType eGreenType = green->GetRasterDataType();
	GDALDataType eBlueType = blue->GetRasterDataType();

	int dataRedSize = GDALGetDataTypeSize(eRedType) / 8;
	int dataGreenSize = GDALGetDataTypeSize(eGreenType) / 8;
	int dataBlueSize = GDALGetDataTypeSize(eBlueType) / 8;

	void *paRedLayer = CPLMalloc(dataRedSize * nWidth * nHeight);
	void *paGreenLayer = CPLMalloc(dataGreenSize * nWidth * nHeight);
	void *paBlueLayer = CPLMalloc(dataBlueSize * nWidth * nHeight);

	red->RasterIO(GF_Read, 0, 0, nXSize, nYSize, paRedLayer, nWidth, nHeight, eRedType, 0, 0);
	green->RasterIO(GF_Read, 0, 0, nXSize, nYSize, paGreenLayer, nWidth, nHeight, eGreenType, 0, 0);
	blue->RasterIO(GF_Read, 0, 0, nXSize, nYSize, paBlueLayer, nWidth, nHeight, eBlueType, 0, 0);

	for (int row = 0; row < nHeight; row++)
		for (int col = 0; col < nWidth; col++)
		{
			//Get RGB values
			double dfRedVal = SRCVAL(paRedLayer, eRedType, nWidth * row + col * dataRedSize);
			double dfGreenVal = SRCVAL(paGreenLayer, eGreenType, nWidth * row + col * dataGreenSize);
			double dfBlueVal = SRCVAL(paBlueLayer, eBlueType, nWidth * row + col * dataBlueSize);
			//Compute luminosity value
			padfImg[row][col] = dfRedVal * forRed + dfGreenVal * forGreen + dfBlueVal * forBlue;
		}

	CPLFree(paRedLayer);
	CPLFree(paGreenLayer);
	CPLFree(paBlueLayer);

	return CE_None;
}

int main(int argc, char* argv[])
{
	cout << "Helloy wolrd!" << endl;

    GDALAllRegister();

    GDALDataset  *poDataset;

    poDataset = (GDALDataset *) GDALOpen( "/home/andrew/workspace/GDAL-correlator/Debug/lenna.jpg", GA_ReadOnly );
	if( poDataset == NULL )
       return 0;

	GDALRasterBand *poRstRedBand = poDataset->GetRasterBand(1);
	GDALRasterBand *poRstGreenBand = poDataset->GetRasterBand(2);
	GDALRasterBand *poRstBlueBand = poDataset->GetRasterBand(3);

	int nWidth = 512;
	int nHeight = 512;
	double **padfImg = NULL;

	padfImg = new double*[nHeight];
	for (int i = 0; i < nHeight; i++)
		padfImg[i] = new double[nWidth];

	GDALConvertRGBToLuminosity(poRstRedBand, poRstGreenBand, poRstBlueBand, nWidth, nHeight,
			padfImg, nHeight, nWidth);

	cout << poRstRedBand->GetMaximum();

	/*
	double dfRedVal_0 = SRCVAL(paLayer, eType, 0);
	double dfRedVal_1 = SRCVAL(paLayer, eType, 1);
	double dfRedVal_2 = SRCVAL(paLayer, eType, 2);
	double dfRedVal_3 = SRCVAL(paLayer, eType, 3);
	double dfRedVal_4 = SRCVAL(paLayer, eType, 4);
	double dfRedVal_5 = SRCVAL(paLayer, eType, 5);
	double dfRedVal_6 = SRCVAL(paLayer, eType, 6);
	double dfRedVal_7 = SRCVAL(paLayer, eType, 7);
	double dfRedVal_8 = SRCVAL(paLayer, eType, 8);
	double dfRedVal_9 = SRCVAL(paLayer, eType, 9);
	*/

	/*
	unsigned char a1 = ((unsigned char*)paLayer)[0];
	unsigned char a2 = ((unsigned char*)paLayer)[1];
	unsigned char a3 = ((unsigned char*)paLayer)[2];
	unsigned char a4 = ((unsigned char*)paLayer)[3];
	unsigned char a5 = ((unsigned char*)paLayer)[4];
	unsigned char a6 = ((unsigned char*)paLayer)[5];
	unsigned char a7 = ((unsigned char*)paLayer)[6];
	unsigned char a8 = ((unsigned char*)paLayer)[7];
	unsigned char a9 = ((unsigned char*)paLayer)[8];
	unsigned char a10 = ((unsigned char*)paLayer)[9];
	unsigned char a11 = ((unsigned char*)paLayer)[10];
	*/

	delete poDataset;

	return 0;
}
