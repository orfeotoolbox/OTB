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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

/*!
 *
 * PURPOSE:
 *
 * Application pour projeter une rï¿½gion d'une image en coordonnï¿½es gï¿½ographiques 
 * en utilisant un Interpolator+regionextractor et un Iterator. 
 * 
 */

// iostream is used for general output
#include <iostream>
#include <iterator>
#include <stdlib.h>

#include "otbMacro.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "itkExceptionObject.h"
#include "otbInverseSensorModel.h"
#include "otbForwardSensorModel.h"

#include "init/ossimInit.h"

int otbCreateInverseForwardSensorModel( int argc, char* argv[] )
{
  try
    {
        ossimInit::instance()->initialize(argc, argv);

        if(argc!=2)
        {
                std::cout << argv[0] <<" <input filename>" << std::endl;
                return EXIT_FAILURE;
        }
   
        typedef otb::Image<unsigned int, 2>     ImageType;
        typedef otb::ImageFileReader<ImageType> ReaderType;
        typedef otb::InverseSensorModel<double> InverseModelType;
        typedef otb::ForwardSensorModel<double> ForwardModelType;

        //Allocate pointer
        InverseModelType::Pointer               inverse_model= InverseModelType::New();
        ForwardModelType::Pointer               forward_model= ForwardModelType::New();
        ReaderType::Pointer	                reader=ReaderType::New();
        
        // Set parameters ...
        reader->SetFileName(argv[1]);

        // Read meta data (ossimKeywordlist)
        reader->GenerateOutputInformation();
        ImageType::Pointer inputImage = reader->GetOutput();

        //Leve une exception si le model n'est pas créé
        otbGenericMsgDebugMacro(<< "Inverse model creation..." ); 
        inverse_model->SetImageGeometry(inputImage->GetImageKeywordlist());
        otbGenericMsgDebugMacro(<< "Foreward model creation..." ); 
        forward_model->SetImageGeometry(inputImage->GetImageKeywordlist());

    } 
  catch( itk::ExceptionObject & err ) 
    { 
    std::cout << "Exception itk::ExceptionObject levee !" << std::endl; 
    std::cout << err << std::endl; 
    return EXIT_FAILURE;
    } 
  catch( std::bad_alloc & err ) 
    { 
    std::cout << "Exception bad_alloc : "<<(char*)err.what()<< std::endl; 
    return EXIT_FAILURE;
    } 
  catch( ... ) 
    { 
    std::cout << "Exception levee inconnue !" << std::endl; 
    return EXIT_FAILURE;
    } 
  return EXIT_SUCCESS;



}//Fin main()

