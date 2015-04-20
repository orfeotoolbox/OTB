#include <iostream>
#include <stdexcept>
#include <gdal.h>
#include <gdal_priv.h>
#include "cpl_string.h"


int main(int argc, char * argv[])
{	
    const char *pszFormat = argv[1];
    GDALDriver *poDriver,*poDriver2;
    char **papszCreateOptions=NULL;
    
    if(argc==5)
    {
		papszCreateOptions = CSLAddString( papszCreateOptions, argv[4] );
		std::cout << "argv[4] = " << argv[4] << std::endl;
	}	

	GDALAllRegister();
	
	GDALDriverH drv = NULL;
	
	std::cout << GDALGetDriverShortName(GDALGetDriverByName("GTiff")) << std::endl;
    std::cout << GDALVersionInfo("RELEASE_NAME") << std::endl;
      
    // ------------------- step 1 -------------------  
    GDALDataset *poSrcDS = 
       (GDALDataset *) GDALOpen( argv[1], GA_ReadOnly );
    GDALDataset *poDstDS;
   
    //poDriver2 = (GDALDriver *) GDALGetDriver(19);
    poDriver2 = (GDALDriver *) GDALGetDriverByName(argv[3]);
    poDstDS = poDriver2->CreateCopy( argv[2], poSrcDS, FALSE, papszCreateOptions, NULL, NULL );

    // ------------------- step 2 ------------------- 
    if( poDstDS != NULL )
        GDALClose( (GDALDatasetH) poDstDS );
        
    return 1; // SUCCESS

}
//GDALDataset * 	CreateCopy (const char *, GDALDataset *, int, char **, GDALProgressFunc pfnProgress, void *pProgressData) CPL_WARN_UNUSED_RESULT
//GDALCreateCopy (GDALDriverH, const char *, GDALDatasetH, int, char **, GDALProgressFunc, void *) 	
