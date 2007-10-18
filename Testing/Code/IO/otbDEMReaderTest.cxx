/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include "itkExceptionObject.h"

#include "itkExceptionObject.h"
#include "otbDEMReader.h"
#include "otbImage.h"
#include "otbImageFileWriter.h"
#include "otbMapProjections.h"

int otbDEMReaderTest(int argc, char * argv[])
{
  try
    {
        const unsigned int Dimension = 2;
        char * srtm_directory(argv[1]);
        const char * outputfilename(argv[2]);
        double height(0.);
        bool bOpenDirectory(false);
      
        typedef otb::Image<float,Dimension> ImageType;
        typedef otb::DEMReader DEMReaderType;

        // Instantiating object
        DEMReaderType::Pointer demReader = DEMReaderType::New();
        bOpenDirectory = demReader->OpenDEMDirectory(srtm_directory);
        if( bOpenDirectory == false )
        {
                itkGenericExceptionMacro(<<" OpenDEMDirectory return flase value !!");
        }
      
        typedef otb::UtmProjection                      utmProjection;
        typedef utmProjection::InputPointType	        InputPoint;
        InputPoint                                      geoPoint;
        geoPoint[0] = 44.08;
        geoPoint[1] = 3.6999;

        height=demReader->GetHeightAboveMSL(geoPoint); 

				std::ofstream file;
				file.open(outputfilename);
				file << "--- HEIGHT ABOVE MSL TEST ---" << std::endl;
				file << " geoPoint: "<<geoPoint[0]<<" ; "<<geoPoint[1]<< std::endl;
        file << " -> Height: "<<height<< std::endl;
				file.close();

        std::cout << "Height: "<<height<<std::endl;
     
    }

  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject thrown !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 

  catch( ... ) 
    { 
    std::cout << "Unknown exception thrown !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;
}
