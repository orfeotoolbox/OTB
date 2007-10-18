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
#include "otbInverseSensorModel.h"
#include "otbStreamingResampleImageFilter.h"

#include "itkExceptionObject.h"
#include "itkExtractImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkLinearInterpolateImageFunction.h"

#include "otbOrthoRectificationFilter.h"
#include "otbMapProjections.h"

#include "init/ossimInit.h"



int otbOrthoRectificationFilterWithDEM( int argc, char* argv[] )
{
  try 
    {        

        ossimInit::instance()->initialize(argc, argv);

        if(argc!=12)
        {
                std::cout << argv[0] <<" <input filename> <output filename>	<latitude de l'origine>	<longitude de l'origine> <taille_x>	<taille_y>";
								std::cout << " <NumberOfstreamDivisions> <x_spacing> <y_spacing> <srtm directory> <DEM Image Filename>" << std::endl;
							 
							  return EXIT_FAILURE;
        }
   
   
        typedef otb::Image<unsigned int, 2>     ImageType;
        typedef otb::ImageFileReader<ImageType>  ReaderType;
        typedef otb::StreamingImageFileWriter<ImageType>  WriterType;
				typedef itk::Transform<double,2,2> TransformType;
        
			  typedef itk::LinearInterpolateImageFunction<ImageType, double > InterpolatorType;
				
				typedef otb::StreamingResampleImageFilter<ImageType,ImageType> ResamplerType;
				typedef otb::UtmProjection UtmMapProjectionType ;
			  typedef otb::OrthoRectificationFilter<ImageType, ImageType, UtmMapProjectionType> OrthoRectifFilterType ;
			  typedef otb::InverseSensorModel<double> ModelType;
				
        //Allocate pointer
        ReaderType::Pointer     	reader=ReaderType::New();
        WriterType::Pointer	    	writer=WriterType::New();
				InterpolatorType::Pointer interpolator=InterpolatorType::New();
				ModelType::Pointer                      model= ModelType::New();

				OrthoRectifFilterType::Pointer     	orthoRectifFilter=OrthoRectifFilterType::New();
				UtmMapProjectionType::Pointer utmMapProjection = UtmMapProjectionType::New();
				
        // Set parameters ...
        reader->SetFileName(argv[1]);
        writer->SetFileName(argv[2]);
   
        // Read meta data (ossimKeywordlist)
        reader->GenerateOutputInformation();
        model->SetImageGeometry(reader->GetOutput()->GetImageKeywordlist());

				orthoRectifFilter->SetInput(reader->GetOutput());
				
				ImageType::IndexType start;
				start[0]=0;
				start[1]=0;
				orthoRectifFilter->SetOutputStartIndex(start);
				
				ImageType::SizeType size;
				size[0]=atoi(argv[5]);      //Taille en X.
  			size[1]=atoi(argv[6]);	    //Taille en Y.
				orthoRectifFilter->SetSize(size);
				
				ImageType::SpacingType spacing;
  			spacing[0]=atof(argv[8]);
  			spacing[1]=atof(argv[9]);
				orthoRectifFilter->SetOutputSpacing(spacing);
				
				ImageType::PointType origin;
				origin[0]=strtod(argv[3], NULL);         //latitude de l'origine.
			  origin[1]=strtod(argv[4], NULL);         //longitude de l'origine.
				orthoRectifFilter->SetOutputOrigin(origin);
				
				utmMapProjection->SetZone(31);
				utmMapProjection->SetHemisphere('N');
				orthoRectifFilter->SetMapProjection(utmMapProjection);
				
				std::string srtmDirectory(argv[10]);
				orthoRectifFilter->SetDEMDirectory(srtmDirectory);

        writer->SetInput(orthoRectifFilter->GetOutput());
				
				writer->SetTilingStreamDivisions();

        otbGenericMsgDebugMacro(<< "Update writer ..." ); 
        writer->Update();

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

 } //End main()

