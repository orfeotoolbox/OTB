/*
 *  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 * See OTBCopyright.txt for details.
 *
 *
 *     This software is distributed WITHOUT ANY WARRANTY; without even
 *     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *    PURPOSE.  See the above copyright notices for more information.
 *
 *
 * Copyright (C) CS SI
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 
=========================================================================*/

#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageList.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbBijectionCoherencyFilter.h"
#include "otbPerBandVectorImageFilter.h"
#include "otbImageToVectorImageCastFilter.h"
#include "otbConcatenateVectorImageFilter.h"
#include "otbFillOcclusionPixel.h"

#include "otbConvertValueFrom0To255.h"

#include "itkMedianImageFilter.h"

  
int otbFillOcclusionsFilterTest(int argc, char *argv[])
  {

  if(argc < 11) 
    {
    std::cerr << "Usage: " << argv[0] << "leftImage rightImage leftDisparityMap rightDisparityMap dmin dmax tolerance radiusWMF sense conversionFrom0To255 outputPathFolder" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 2;
  typedef otb::VectorImage<float> FloatVectorImageType;
  typedef otb::ImageFileReader<FloatVectorImageType> FloatVectorImageReaderType; 
  typedef otb::ImageFileWriter<FloatVectorImageType> FloatVectorImageWriterType ;
  
  typedef otb::Image< int, Dimension > IntImageType;
  typedef otb::Image< float, Dimension > FloatImageType;

  typedef otb::ImageFileReader<IntImageType> IntImageReaderType; 
  typedef otb::ImageFileWriter< IntImageType > IntImageWriterType;  

  FloatVectorImageReaderType::Pointer inLeft = FloatVectorImageReaderType::New();
  inLeft->SetFileName(argv[1]); //LeftImage  
  inLeft->UpdateOutputInformation();
  FloatVectorImageReaderType::Pointer inRight = FloatVectorImageReaderType::New();
  inRight->SetFileName(argv[2]);//RightImage
  inRight->UpdateOutputInformation();


  IntImageReaderType::Pointer m_LeftDisparity = IntImageReaderType::New();
  m_LeftDisparity->SetFileName(argv[3]); //LeftImage  
  m_LeftDisparity->UpdateOutputInformation();

  IntImageReaderType::Pointer m_RightDisparity = IntImageReaderType::New();
  m_RightDisparity->SetFileName(argv[4]);//RightImage
  m_RightDisparity->UpdateOutputInformation();

  int dispMin = atoi(argv[5]);
  int dispMax  = atoi(argv[6]);

  unsigned int tol = atoi(argv[7]);
  unsigned int rwmf = atoi(argv[8]) ;

  bool sense = atoi(argv[9]); //'0' corresponds to left to right camera mortion, '1' to right to left camera motion
  bool conversion = atoi(argv[10]) ; //'0' no conversion, '1' value conversion from 0 to 255 

  std::string argv11 = std::string(argv[11]);
  #define FILENAME(n) std::string( argv11 + std::string(n)).c_str()

  typedef otb::BijectionCoherencyFilter< IntImageType, IntImageType > OcclusionType;
  OcclusionType::Pointer m_OcclusionFilter = OcclusionType::New();  

  m_OcclusionFilter->SetDirectHorizontalDisparityMapInput(m_LeftDisparity->GetOutput()); 
  m_OcclusionFilter->SetReverseHorizontalDisparityMapInput(m_RightDisparity->GetOutput());
  m_OcclusionFilter->SetMaxHDisp(dispMax);
  m_OcclusionFilter->SetMinHDisp(dispMin); 
  m_OcclusionFilter->SetMinVDisp(0);
  m_OcclusionFilter->SetMaxVDisp(0);
  m_OcclusionFilter->SetTolerance(tol);


  //MEDIAN IMAGES
  typedef itk::MedianImageFilter< FloatImageType, FloatImageType > MedianFilterType;
  MedianFilterType::Pointer LMedian = MedianFilterType::New();
  MedianFilterType::RadiusType MedianRadius = {{rwmf,rwmf}};
  LMedian->SetRadius(MedianRadius); 

  typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, MedianFilterType> PerBand;
  PerBand::Pointer m_LeftMedianFilter = PerBand::New();
  m_LeftMedianFilter->SetFilter(LMedian);
  m_LeftMedianFilter->SetInput(inLeft->GetOutput()); 
  PerBand::Pointer m_RightMedianFilter = PerBand::New();
  m_RightMedianFilter->SetFilter(LMedian);
  m_RightMedianFilter->SetInput(inRight->GetOutput()); 


  typedef otb::ImageToVectorImageCastFilter<IntImageType,FloatVectorImageType> CastIntImageIntoFloatVecImageFilter;  
  CastIntImageIntoFloatVecImageFilter::Pointer m_CastOccMap = CastIntImageIntoFloatVecImageFilter::New(); 
  CastIntImageIntoFloatVecImageFilter::Pointer m_CastLeftDisparity = CastIntImageIntoFloatVecImageFilter::New();
  CastIntImageIntoFloatVecImageFilter::Pointer m_CastRightDisparity = CastIntImageIntoFloatVecImageFilter::New();
  m_CastOccMap-> SetInput( m_OcclusionFilter->GetOutput() ); 
  m_CastLeftDisparity-> SetInput( m_LeftDisparity->GetOutput() );
  m_CastRightDisparity->SetInput( m_RightDisparity->GetOutput() );

  typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType> ConcatenateVectorImageFilterType;
  ConcatenateVectorImageFilterType::Pointer m_ConcatenateMedianImageAndOccMap = ConcatenateVectorImageFilterType::New();
  ConcatenateVectorImageFilterType::Pointer m_ConcatenateCastOccMapAndInputImage = ConcatenateVectorImageFilterType::New() ;

  if(sense==0)
   {        
   m_ConcatenateMedianImageAndOccMap->SetInput1(m_LeftMedianFilter->GetOutput());
   m_ConcatenateMedianImageAndOccMap->SetInput2(m_CastOccMap->GetOutput());
   m_ConcatenateCastOccMapAndInputImage->SetInput1(m_CastLeftDisparity->GetOutput());
   m_ConcatenateCastOccMapAndInputImage->SetInput2(m_ConcatenateMedianImageAndOccMap->GetOutput());
   }
  else
   {      
   m_ConcatenateMedianImageAndOccMap->SetInput1(m_RightMedianFilter->GetOutput());
   m_ConcatenateMedianImageAndOccMap->SetInput2(m_CastOccMap->GetOutput());
   m_ConcatenateCastOccMapAndInputImage->SetInput1(m_CastRightDisparity->GetOutput());
   m_ConcatenateCastOccMapAndInputImage->SetInput2(m_ConcatenateMedianImageAndOccMap->GetOutput());
   } 

  typedef otb::FillPixelFilter< FloatVectorImageType, IntImageType > FillOccFilter ;
  FillOccFilter::Pointer m_FillOcc = FillOccFilter::New();

  m_FillOcc->SetInput(m_ConcatenateCastOccMapAndInputImage->GetOutput());
  FloatVectorImageType::SizeType radiusM;
  radiusM[0] = rwmf;
  radiusM[1] = rwmf;   
  m_FillOcc->SetRadius(radiusM);


  typedef otb::ConvertValueFrom0To255<IntImageType, IntImageType > ConvertValue ;
  ConvertValue::Pointer m_convertSmoothDisparity = ConvertValue::New() ;

  if(conversion==1)
    {   
    m_convertSmoothDisparity->SetInput(m_FillOcc->GetOutput());
    m_convertSmoothDisparity->SetDispMin(dispMin);
    m_convertSmoothDisparity->SetDispMax(dispMax);       
    m_convertSmoothDisparity->SetOffset(0);

    IntImageWriterType::Pointer writer_FillOcclusions = IntImageWriterType::New();
    writer_FillOcclusions->SetFileName( FILENAME("ConvertFillOcclusions.tif"));
    writer_FillOcclusions->SetInput(m_convertSmoothDisparity->GetOutput());
    writer_FillOcclusions->Update();
    }     
  else
    {
    IntImageWriterType::Pointer writer_FillOcclusions = IntImageWriterType::New();
    writer_FillOcclusions->SetFileName( FILENAME("FillOcclusions.tif"));
    writer_FillOcclusions->SetInput(m_FillOcc->GetOutput());
    writer_FillOcclusions->Update();
    }
  
  return EXIT_SUCCESS;

  }
