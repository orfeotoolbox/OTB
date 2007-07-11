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
 * Application pour projeter une r�gion d'une image en coordonn�es g�ographiques 
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
#include "otbImageFileWriter.h"
#include "otbStreamingImageFileWriter.h"


#include "itkExceptionObject.h"
#include "itkExtractImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkLinearInterpolateImageFunction.h"

#include "otbImageGeometryHandler.h"
#include "otbInverseSensorModel.h"

// this is the most important class and is called as the first line of all applications.
// without this all the important factories are not created.
//#include "init/ossimInit.h"



int otbCreateProjectionWithOTB( int argc, char* argv[] )
{
  try 
    {        

        ossimInit::instance()->initialize(argc, argv);

        if(argc!=2)
        {
                std::cout << argv[0] <<" <input filename> " << std::endl;

                return EXIT_FAILURE;
        }
        
        typedef otb::Image<unsigned int, 2>     ImageType;
        typedef otb::ImageFileReader<ImageType>  ReaderType;
        ReaderType::Pointer	                 reader=ReaderType::New();
        reader->SetFileName(argv[1]);

        //Read meta data (ossimKeywordlist)
        reader->GenerateOutputInformation();

        otbGenericMsgDebugMacro(<< "Read ossim Keywordlist..." );
        otb::ImageKeywordlist otb_image_keywordlist = reader->GetOutput()->GetImageKeywordlist();

        ossimKeywordlist geom;
        otb_image_keywordlist.convertToOSSIMKeywordlist(geom);

        otbGenericMsgDebugMacro(<< "ossim Keywordlist:"<<geom ); 

/*
        typedef otb::InverseSensorModel<double>  ModelType;
        ModelType::Pointer   model= ModelType::New();
        otbGenericMsgDebugMacro(<< "Model set geometry " ); 
      model->SetImageGeometry(geom_kwl); //Notre mod�le est cr�� � ce niveau.
         if(!model)
      {
       otbGenericMsgDebugMacro(<< "Unable to create a model");
         return 1;
      }
*/
        ossimGpt ossimGPoint(0,0);
        ossimDpt ossimDPoint;
        ossimProjection * model = NULL;
        otbGenericMsgDebugMacro(<< "Creating projection..." );
        model = ossimProjectionFactoryRegistry::instance()->createProjection(geom);
        if( model == NULL)
        {
                itkGenericExceptionMacro(<<"Invalid Model * == NULL !");
        }

        otbGenericMsgDebugMacro(<< "Creating RefPtr of projection..." );
        ossimRefPtr<ossimProjection> ptrmodel = model;
        if( ptrmodel.valid() == false )
        {
                itkGenericExceptionMacro(<<"Invalid Model pointer .valid() == false !");
        }

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

