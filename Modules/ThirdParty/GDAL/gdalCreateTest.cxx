#include <iostream>
#include <stdexcept>
#include <gdal.h>
#include <gdal_priv.h>
#include <ogr_spatialref.h>


int main(int argc, char * argv[])
{
    const char *pszFormat = argv[1];
    GDALDriver *poDriver;
    char **papszMetadata;

	GDALAllRegister();

	// ------------------- step 1 -------------------
    poDriver = GetGDALDriverManager()->GetDriverByName(pszFormat);

    if( poDriver == NULL )
    {
		std::cout << "poDriver NULL" << std::endl;
		
        return 0; //FAIL
	}

    papszMetadata = poDriver->GetMetadata();
    if( CSLFetchBoolean( papszMetadata, GDAL_DCAP_CREATE, FALSE ) )
        printf( "Driver %s supports Create() method.\n", pszFormat );
    if( CSLFetchBoolean( papszMetadata, GDAL_DCAP_CREATECOPY, FALSE ) )
        printf( "Driver %s supports CreateCopy() method.\n", pszFormat );
      
    // ------------------- step 2 -------------------  
    GDALDataset *poDstDS;       
    char **papszOptions = NULL;

    poDstDS = poDriver->Create( argv[2], 512, 512, 1, GDT_Byte, 
                                papszOptions );
                                
  	// ------------------- step 3 -------------------                              
    double adfGeoTransform[6] = { 444720, 30, 0, 3751320, 0, -30 };
    OGRSpatialReference oSRS;
    char *pszSRS_WKT = NULL;
    GDALRasterBand *poBand;
    GByte abyRaster[512*512];

    poDstDS->SetGeoTransform( adfGeoTransform );
    
    oSRS.SetUTM( 11, TRUE );
    oSRS.SetWellKnownGeogCS( "NAD27" );
    oSRS.exportToWkt( &pszSRS_WKT );
    poDstDS->SetProjection( pszSRS_WKT );
    CPLFree( pszSRS_WKT );

    poBand = poDstDS->GetRasterBand(1);
    poBand->RasterIO( GF_Write, 0, 0, 512, 512, 
                      abyRaster, 512, 512, GDT_Byte, 0, 0 );    

	// ------------------- step 4 -------------------
    GDALClose( (GDALDatasetH) poDstDS );
    
    return 1; // SUCCESS
}
