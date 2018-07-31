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


#include "otbLocalGradientVectorImageFilter.h"
#include "otbLeftCostVolumeImageFilter.h"
#include "otbRightCostVolumeImageFilter.h"
#include "otbMinimumNBandsImageFilter.h"
#include "otbMaximumNBandsImageFilter.h"

#include <itkConstantBoundaryCondition.h>
#include <otbConvolutionImageFilter.h>
#include <itkArray.h>
#include <otbPerBandVectorImageFilter.h>

#include "otbConvertionRGBToGrayLevelImageFilter.h"

  
int testCostVolumeFilters(int argc, char *argv[])
  {

  if(argc < 9) {
    std::cerr << "Usage: " << argv[0] << " leftImage rightImage minDisp maxDisp alpha tau1 tau2 sense outputPathFolder" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 2;
  typedef otb::VectorImage<float> FloatVectorImageType;
  typedef otb::Image< float, Dimension > FloatImageType;  
  typedef otb::ImageFileReader<FloatVectorImageType> FloatVectorImageReaderType;  
  typedef otb::Image< int, Dimension > IntImageType;
  typedef otb::ImageFileWriter< IntImageType > IntImageWriterType;  

  FloatVectorImageReaderType::Pointer inLeft = FloatVectorImageReaderType::New();
  inLeft->SetFileName(argv[1]); //LeftImage  
  inLeft->UpdateOutputInformation();
  FloatVectorImageReaderType::Pointer inRight = FloatVectorImageReaderType::New();
  inRight->SetFileName(argv[2]);//RightImage
  inRight->UpdateOutputInformation();
  int dispMin = atoi(argv[3]);
  int dispMax  = atoi(argv[4]);
  float alpha = atoi(argv[5]);
  float tau1 = atoi(argv[6]);
  float tau2 = atoi(argv[7]);
  int sense = atoi(argv[8]) ;

  std::string argv9 = std::string(argv[9]);
  #define FILENAME(n) std::string( argv9 + std::string(n)).c_str()

  typedef itk::ConstantBoundaryCondition<FloatImageType> BoundaryConditionType;
  typedef otb::ConvolutionImageFilter<FloatImageType, FloatImageType, BoundaryConditionType> ConvFilterType;
  typedef otb::ConvertionRGBToGrayLevelImageFilter<FloatVectorImageType,FloatVectorImageType> RGBTograylevelFilter ;
  typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, ConvFilterType> VectorFilterType;
  typedef otb::LeftCostVolumeImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType >  LeftCostVolumeType;  
  typedef otb::RightCostVolumeImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > RightCostVolumeType; 
  typedef otb::MinimumNBandsImageFilter<  FloatVectorImageType, IntImageType> MinCostVolume;  
  typedef otb::MaximumNBandsImageFilter< FloatVectorImageType, IntImageType > MaxCostVolume; 

  ConvFilterType::Pointer m_convFilterXLeft = ConvFilterType::New();
  ConvFilterType::Pointer m_convFilterXRight = ConvFilterType::New() ;
  RGBTograylevelFilter::Pointer m_LeftGrayVectorImage = RGBTograylevelFilter::New() ;
  RGBTograylevelFilter::Pointer m_RightGrayVectorImage = RGBTograylevelFilter::New() ;
  VectorFilterType::Pointer m_GradientXLeft = VectorFilterType::New();
  VectorFilterType::Pointer m_GradientXRight = VectorFilterType::New();
  LeftCostVolumeType::Pointer m_LeftCost = LeftCostVolumeType::New();
  RightCostVolumeType::Pointer m_RightCost = RightCostVolumeType::New() ;
  MinCostVolume::Pointer m_LeftDisparity = MinCostVolume::New();
  MaxCostVolume::Pointer m_RightDisparity = MaxCostVolume::New();

  // GRADIENT CALCULATIONS
  ConvFilterType::InputSizeType radiusG;
  radiusG[0] = 1;
  radiusG[1] = 0;
  itk::Array< double > filterCoeffsX;
  filterCoeffsX.SetSize((2 * radiusG[0] + 1) ) ;
  filterCoeffsX.Fill(0.5);
  filterCoeffsX[0] = -0.5;
  filterCoeffsX[1] = 0;
  m_convFilterXLeft->SetRadius(radiusG);
  m_convFilterXLeft->SetFilter(filterCoeffsX);  
  m_convFilterXRight->SetRadius(radiusG);
  m_convFilterXRight->SetFilter(filterCoeffsX);
  m_LeftGrayVectorImage->SetInput(inLeft->GetOutput());
  m_RightGrayVectorImage->SetInput(inRight->GetOutput());  
  //--Left---------------  
  m_GradientXLeft->SetFilter(m_convFilterXLeft);
  m_GradientXLeft->SetInput(inLeft->GetOutput());
  // m_GradientXLeft->SetInput(m_LeftGrayVectorImage->GetOutput());
  m_GradientXLeft->UpdateOutputInformation(); 
  //--Right--------------- 
  m_GradientXRight->SetFilter(m_convFilterXRight);
  m_GradientXRight->SetInput(inRight->GetOutput());
  // m_GradientXRight->SetInput(m_RightGrayVectorImage->GetOutput());
  m_GradientXRight->UpdateOutputInformation(); 

  // COST VOLUME  
  if(sense==0)
    {          // --- LEFT
    m_LeftCost->SetLeftInputImage(inLeft->GetOutput());
    m_LeftCost->SetRightInputImage(inRight->GetOutput());  
    m_LeftCost->SetLeftGradientXInput(m_GradientXLeft->GetOutput() ); 
    m_LeftCost->SetRightGradientXInput(m_GradientXRight->GetOutput() );      
    m_LeftCost->SetMinDisp(dispMin);
    m_LeftCost->SetMaxDisp(dispMax);
    m_LeftCost->SetAlpha(alpha);
    m_LeftCost->SetTau1(tau1);
    m_LeftCost->SetTau2(tau2);
    m_LeftCost->UpdateOutputInformation(); 
    //   // --- RIGHT
    m_RightCost->SetLeftInputImage(inRight->GetOutput());
    m_RightCost->SetRightInputImage(inLeft->GetOutput() );  
    m_RightCost->SetLeftGradientXInput(m_GradientXRight->GetOutput() ); 
    m_RightCost->SetRightGradientXInput(m_GradientXLeft->GetOutput() );      
    m_RightCost->SetMinDisp(-dispMax);
    m_RightCost->SetMaxDisp(-dispMin);   
    m_RightCost->SetAlpha(alpha);
    m_RightCost->SetTau1(tau1);
    m_RightCost->SetTau2(tau2);  
    m_RightCost->UpdateOutputInformation();    
    }
  else
    {
    m_LeftCost->SetLeftInputImage(inLeft->GetOutput());
    m_LeftCost->SetRightInputImage(inRight->GetOutput());  
    m_LeftCost->SetLeftGradientXInput(m_GradientXLeft->GetOutput() ); 
    m_LeftCost->SetRightGradientXInput(m_GradientXRight->GetOutput() );      
    m_LeftCost->SetMinDisp(-dispMax);
    m_LeftCost->SetMaxDisp(-dispMin); 
    m_LeftCost->SetAlpha(alpha);
    m_LeftCost->SetTau1(tau1);
    m_LeftCost->SetTau2(tau2);
    m_LeftCost->UpdateOutputInformation(); 
    //   // --- RIGHT
    m_RightCost->SetLeftInputImage(inRight->GetOutput());
    m_RightCost->SetRightInputImage(inLeft->GetOutput());  
    m_RightCost->SetLeftGradientXInput(m_GradientXRight->GetOutput() ); 
    m_RightCost->SetRightGradientXInput(m_GradientXLeft->GetOutput() );      
    m_RightCost->SetMinDisp(dispMin);
    m_RightCost->SetMaxDisp(dispMax);  
    m_RightCost->SetAlpha(alpha);
    m_RightCost->SetTau1(tau1);
    m_RightCost->SetTau2(tau2);     
    m_RightCost->UpdateOutputInformation();   
    }
  m_LeftDisparity->SetInput(m_LeftCost->GetOutput());
  m_RightDisparity->SetInput(m_RightCost->GetOutput());  
  IntImageWriterType::Pointer writer_m_minLeftCost = IntImageWriterType::New();
  writer_m_minLeftCost->SetFileName( FILENAME("MinLeftCost.tif"));
  writer_m_minLeftCost->SetInput(m_LeftDisparity->GetOutput());
  writer_m_minLeftCost->Update();
  IntImageWriterType::Pointer writer_m_minRightCost = IntImageWriterType::New();
  writer_m_minRightCost->SetFileName( FILENAME("MinRightCost.tif"));
  writer_m_minRightCost->SetInput(m_RightDisparity->GetOutput());
  writer_m_minRightCost->Update();
  
  return EXIT_SUCCESS;

  }
