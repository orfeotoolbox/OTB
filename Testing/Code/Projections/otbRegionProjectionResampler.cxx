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
#include "otbStreamingResampleImageFilter.h"

#include "itkExceptionObject.h"
#include "itkExtractImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkTranslationTransform.h"

#include "otbInverseSensorModel.h"
#include "otbCompositeTransform.h"

#include "init/ossimInit.h"



int otbRegionProjectionResampler( int argc, char* argv[] )
{
  try 
    {        

        ossimInit::instance()->initialize(argc, argv);

        if(argc!=10)
        {
                std::cout << argv[0] <<" <input filename> <output filename> <latitude de l'origine> <longitude de l'origine> <taille_x> <taille_y> <NumberOfstreamDivisions>" 
                << std::endl;

                return EXIT_FAILURE;
        }
   
   
        typedef otb::Image<unsigned char, 2>    CharImageType;
        typedef otb::Image<unsigned int, 2>     ImageType;
        typedef otb::ImageFileReader<ImageType>  ReaderType;
//        typedef otb::ImageFileWriter<ImageType>  WriterType;
        typedef otb::StreamingImageFileWriter<ImageType>  WriterType;
        typedef otb::InverseSensorModel<double> ModelType;
        typedef itk::LinearInterpolateImageFunction< 
                       ImageType, double >      InterpolatorType;
        typedef itk::RescaleIntensityImageFilter<ImageType,CharImageType>  
                                                RescalerType;
//       typedef itk::ResampleImageFilter< ImageType, ImageType  > 
//                                                ResamplerType;
				typedef otb::StreamingResampleImageFilter< ImageType, ImageType  > 
                                               ResamplerType;
				typedef itk::TranslationTransform<double,2> TransformType;
				
				typedef otb::CompositeTransform<ModelType,TransformType> CompositeType;																							 
																							 
        ImageType::IndexType  			start;
        ImageType::SizeType  			size;
        ImageType::SpacingType  		spacing;
        ImageType::PointType			origin;
        ImageType::RegionType                   region;

        //Allocate pointer
        ModelType::Pointer                      model= ModelType::New();
        ReaderType::Pointer	                reader=ReaderType::New();
        WriterType::Pointer	                writer=WriterType::New();
        InterpolatorType::Pointer	        interpolator=InterpolatorType::New();
        RescalerType::Pointer	                rescaler=RescalerType::New();
        ResamplerType::Pointer                  resampler = ResamplerType::New();
//        TransformType::Pointer translationTransform = TransformType::New();
//				CompositeType::Pointer compositeTransform = CompositeType::New();
				
        // Set parameters ...
        reader->SetFileName(argv[1]);
        writer->SetFileName(argv[2]);
        rescaler->SetOutputMinimum(50);
        rescaler->SetOutputMaximum(500);

        // Read meta data (ossimKeywordlist)
        reader->GenerateOutputInformation();
        ImageType::ConstPointer inputImage = reader->GetOutput();

//        otb::ImageKeywordlist ImageKeyworlist = reader->GetOutput()->GetImageKeywordlist();
        model->SetImageGeometry(reader->GetOutput()->GetImageKeywordlist());

/*				compositeTransform->SetFirstTransform(model);
				
				itk::Vector<double,2> offset;
				offset[0]=atof(argv[10]);
				offset[1]=atof(argv[11]);
				translationTransform->SetOffset(offset);
				
				compositeTransform->SetSecondTransform(translationTransform);*/

        start[0]=0;     
        start[1]=0;     
        size[0]=atoi(argv[5]);      //Taille en X.
        size[1]=atoi(argv[6]);	    //Taille en Y.

        region.SetSize(size);
        region.SetIndex(start);

        spacing[0]=atof(argv[8]);
        spacing[1]=atof(argv[9]);

        origin[0]=strtod(argv[3], NULL);         //latitude de l'origine.
        origin[1]=strtod(argv[4], NULL);         //longitude de l'origine.

/*        
        spacing = inputImage->GetSpacing();
        origin  = inputImage->GetOrigin();
//        region = inputImage->GetBufferedRegion();

*/
				std::cout << "Origin " << origin << std::endl;

        resampler->SetOutputSpacing( spacing );
        resampler->SetOutputOrigin(  origin  );
        resampler->SetSize( region.GetSize() );
        resampler->SetOutputStartIndex( start /*reader->GetOutput()->GetBufferedRegion().GetIndex() */ );

        //Connect pipeline
        resampler->SetInput( reader->GetOutput() );
        resampler->SetTransform( model );
        resampler->SetInterpolator( interpolator );

				otbGenericMsgDebugMacro(<< "Romain Sensor Model :" << model);

//        otbGenericMsgDebugMacro(<< "Resampler initialized !! " ); 
//        resampler->Update();

//        rescaler->SetInput( resampler->GetOutput() );
        
        writer->SetInput(resampler->GetOutput());
//        writer->SetNumberOfStreamDivisions(1000);
//			writer->SetTilingStreamDivisions(atoi(argv[7]));
				writer->SetTilingStreamDivisions(10);
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

 }//Fin main()

