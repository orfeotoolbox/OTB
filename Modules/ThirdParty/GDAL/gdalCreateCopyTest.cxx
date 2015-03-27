#include <iostream>
#include <stdexcept>
#include <gdal.h>
#include <gdal_priv.h>


int main(int argc, char * argv[])
{
    const char *pszFormat = argv[1];
    GDALDriver *poDriver,*poDriver2;
    char **papszMetadata;

	GDALAllRegister();
	
	GDALDriverH drv = NULL;
	unsigned int count = GDALGetDriverCount();
	std::cout << "Nb of drivers : " << count << std::endl;
	for (unsigned int i = 0; i < count; i++) 
	{
        drv = GDALGetDriver(i);
        std::cout << "i = " << i << " " << GDALGetDriverShortName(drv) << std::endl;
	}
	
	std::cout << GDALGetDriverShortName(GDALGetDriverByName("GTiff")) << std::endl;
    std::cout << GDALVersionInfo("RELEASE_NAME") << std::endl;
      
    // ------------------- step 1 -------------------  
    GDALDataset *poSrcDS = 
       (GDALDataset *) GDALOpen( argv[1], GA_ReadOnly );
    GDALDataset *poDstDS;
   
    //poDriver2 = (GDALDriver *) GDALGetDriver(19);
    poDriver2 = (GDALDriver *) GDALGetDriverByName(argv[3]);
    poDstDS = poDriver2->CreateCopy( argv[2], poSrcDS, FALSE, NULL, NULL, NULL );

    // ------------------- step 2 ------------------- 
    if( poDstDS != NULL )
        GDALClose( (GDALDatasetH) poDstDS );
        
    return 1; // SUCCESS

}
