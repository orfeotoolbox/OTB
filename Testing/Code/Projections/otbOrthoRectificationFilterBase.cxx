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



int otbOrthoRectificationFilterBase( int argc, char* argv[] )
{
        ossimInit::instance()->initialize(argc, argv);

        if(argc!=10)
        {
                std::cout << argv[0] <<" <input filename> <output filename> <latitude de l'origine> <longitude de l'origine> <taille_x> <taille_y> <NumberOfstreamDivisions>" 
                << std::endl;

                return EXIT_FAILURE;
        }
   
        typedef otb::Image<double, 2>     ImageType;
        typedef otb::ImageFileReader<ImageType>  ReaderType;
        typedef otb::StreamingImageFileWriter<ImageType>  WriterType;
        typedef otb::UtmInverseProjection UtmMapProjectionType ;
        typedef otb::OrthoRectificationFilterBase<ImageType, ImageType, UtmMapProjectionType> OrthoRectifFilterType ;
              
        //Allocate pointer
        ReaderType::Pointer             reader = ReaderType::New();
        WriterType::Pointer             writer = WriterType::New();

        OrthoRectifFilterType::Pointer  orthoRectifFilter = OrthoRectifFilterType::New();
        UtmMapProjectionType::Pointer   utmMapProjection = UtmMapProjectionType::New();
                                
        // Set parameters ...
        reader->SetFileName(argv[1]);
        writer->SetFileName(argv[2]);
   
        
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
        size[1]=atoi(argv[6]);            //Taille en Y.
        orthoRectifFilter->SetSize(size);
        
        ImageType::SpacingType spacing;
        spacing[0]=atof(argv[8]);
        spacing[1]=atof(argv[9]);
        orthoRectifFilter->SetOutputSpacing(spacing);
        
        ImageType::PointType origin;
        origin[0]=strtod(argv[3], NULL);         //Coord x en mètres de l'origine.
        origin[1]=strtod(argv[4], NULL);         //Coord y en mètres de l'origine.
        orthoRectifFilter->SetOutputOrigin(origin);
        orthoRectifFilter->SetMapProjection(utmMapProjection);

        writer->SetInput(orthoRectifFilter->GetOutput());
                          
        writer->SetTilingStreamDivisions();
        writer->Update();

        return EXIT_SUCCESS;
 }//Fin main()

