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
#include "itkChangeInformationImageFilter.h"

#include "otbOrthoRectificationFilterBase.h"
#include "otbMapProjections.h"

#include "init/ossimInit.h"



int otbOrthoRectificationFilterBaseWithDEM( int argc, char* argv[] )
{
        ossimInit::instance()->initialize(argc, argv);

        if(argc!=12)
        {
                std::cout << argv[0] <<" <input filename> <output filename>	<latitude de l'origine>	<longitude de l'origine> <taille_x>	<taille_y>";
								std::cout << " <NumberOfstreamDivisions> <x_spacing> <y_spacing> <srtm directory> <DEM Image Filename>" << std::endl;
							 
							  return EXIT_FAILURE;
        }
   
   
        typedef otb::Image<double, 2>     ImageType;
        typedef otb::ImageFileReader<ImageType>  ReaderType;
        typedef otb::StreamingImageFileWriter<ImageType>  WriterType;
       
				typedef otb::UtmInverseProjection UtmMapProjectionType ;
			  typedef otb::OrthoRectificationFilterBase<ImageType, ImageType, UtmMapProjectionType> OrthoRectifFilterType ;
				
        //Allocate pointer
        ReaderType::Pointer     	reader=ReaderType::New();
        WriterType::Pointer	    	writer=WriterType::New();

				OrthoRectifFilterType::Pointer     	orthoRectifFilter=OrthoRectifFilterType::New();
				UtmMapProjectionType::Pointer utmMapProjection = UtmMapProjectionType::New();
				
        // Set parameters ...
        reader->SetFileName(argv[1]);
        writer->SetFileName(argv[2]);
   
        // Read meta data (ossimKeywordlist)
//        reader->GenerateOutputInformation();
//        model->SetImageGeometry(reader->GetOutput()->GetImageKeywordlist());

				// image origin modification
  			typedef itk::ChangeInformationImageFilter<ImageType > ChangeInfoFilterType;
  			ChangeInfoFilterType::Pointer changeInfo = ChangeInfoFilterType::New();
  			changeInfo->SetInput(reader->GetOutput());
  			changeInfo->ChangeOriginOn();
  			ImageType::PointType originNull;
  			originNull[0]=0;
  			originNull[1]=0;
  			changeInfo->SetOutputOrigin(originNull);
	
  			changeInfo->GenerateOutputInformation();
			
				orthoRectifFilter->SetInput(changeInfo->GetOutput());
				
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
				origin[0]=strtod(argv[3], NULL);         //longitude de l'origine.
			  origin[1]=strtod(argv[4], NULL);         //latitude de l'origine.
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

        return EXIT_SUCCESS;

 } //End main()

