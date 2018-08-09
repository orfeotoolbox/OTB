/*
 * Copyright (C) 2017-2018 CS Systemes d'Information (CS SI)
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
 */


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
#include "otbCostVolumeFilter.h"
#include "otbMinimumVectorImageFilter.h"
#include "itkConstantBoundaryCondition.h"
#include "otbConvolutionImageFilter.h"
#include "itkArray.h"
#include "otbPerBandVectorImageFilter.h"

#include "otbConvertionRGBToGrayLevelImageFilter.h"

  
int otbCostVolumeFiltersTest(int argc, char *argv[])
  {

  if(argc < 11) {
    std::cerr << "Usage: " << argv[0] << " leftImage rightImage minDisp maxDisp alpha tau1L tau2L tau1R tau2R sense outputPathFolder" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 2;
  typedef otb::VectorImage<float> FloatVectorImageType;
  typedef otb::Image< float, Dimension > FloatImageType;  
  typedef otb::ImageFileReader<FloatVectorImageType> FloatVectorImageReaderType;  
  typedef otb::Image< int, Dimension > IntImageType;
  typedef otb::ImageFileWriter< IntImageType > IntImageWriterType;  
   typedef otb::ImageFileWriter< FloatVectorImageType > FloatVectorImageWriterType;  

  FloatVectorImageReaderType::Pointer inLeft = FloatVectorImageReaderType::New();
  inLeft->SetFileName(argv[1]); //LeftImage  
  inLeft->UpdateOutputInformation();
  FloatVectorImageReaderType::Pointer inRight = FloatVectorImageReaderType::New();
  inRight->SetFileName(argv[2]);//RightImage
  inRight->UpdateOutputInformation();
  int dispMin = atoi(argv[3]);
  int dispMax  = atoi(argv[4]);
  float alpha = atof(argv[5]);
  float tau1L = atof(argv[6]);
  float tau2L = atof(argv[7]);
  float tau1R = atof(argv[8]);
  float tau2R = atof(argv[9]);
  int sense = atoi(argv[10]) ;

  std::string argv11 = std::string(argv[11]);
  #define FILENAME(n) std::string( argv11 + std::string(n)).c_str()

  typedef itk::ConstantBoundaryCondition<FloatImageType> BoundaryConditionType;
  typedef otb::ConvolutionImageFilter<FloatImageType, FloatImageType, BoundaryConditionType> ConvFilterType;
  typedef otb::ConvertionRGBToGrayLevelImageFilter<FloatVectorImageType,FloatVectorImageType> RGBTograylevelFilter ;
  typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, ConvFilterType> VectorFilterType;
  typedef otb::CostVolumeFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType >  CostVolumeType;  
  typedef otb::MinimumVectorImageFilter<  FloatVectorImageType, IntImageType> MinCostVolume;  


  ConvFilterType::Pointer m_convFilterXLeft = ConvFilterType::New();
  ConvFilterType::Pointer m_convFilterXRight = ConvFilterType::New() ;
  RGBTograylevelFilter::Pointer m_LeftGrayVectorImage = RGBTograylevelFilter::New() ;
  RGBTograylevelFilter::Pointer m_RightGrayVectorImage = RGBTograylevelFilter::New() ;
  VectorFilterType::Pointer m_GradientXLeft = VectorFilterType::New();
  VectorFilterType::Pointer m_GradientXRight = VectorFilterType::New();
  CostVolumeType::Pointer m_LeftCost = CostVolumeType::New();
  CostVolumeType::Pointer m_RightCost = CostVolumeType::New() ;
  MinCostVolume::Pointer m_minLeftCost = MinCostVolume::New();
  MinCostVolume::Pointer m_minRightCost = MinCostVolume::New();

  // GRADIENT CALCULATION
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
  
  //--Left---------------  
  m_GradientXLeft->SetFilter(m_convFilterXLeft);
  if( inLeft->GetOutput()->GetNumberOfComponentsPerPixel() > 1)
    {
    m_LeftGrayVectorImage->SetInput(inLeft->GetOutput());
    m_GradientXLeft->SetInput(m_LeftGrayVectorImage->GetOutput());
    }
  else
    {
    m_GradientXLeft->SetInput(inLeft->GetOutput());
    }
  m_GradientXLeft->UpdateOutputInformation(); 

  //--Right--------------- 
  m_GradientXRight->SetFilter(m_convFilterXRight);
  if( inRight->GetOutput()->GetNumberOfComponentsPerPixel() > 1)
    {
    m_RightGrayVectorImage->SetInput(inRight->GetOutput());
    m_GradientXRight->SetInput(m_RightGrayVectorImage->GetOutput());
    }
  else
    {
    m_GradientXRight->SetInput(inRight->GetOutput());
    }
  m_GradientXRight->UpdateOutputInformation(); 


  // COST VOLUME 
  m_LeftCost->SetLeftInputImage(inLeft->GetOutput());
  m_LeftCost->SetRightInputImage(inRight->GetOutput());  
  m_LeftCost->SetSide('l') ;
  m_LeftCost->SetLeftGradientXInput(m_GradientXLeft->GetOutput() ); 
  m_LeftCost->SetRightGradientXInput(m_GradientXRight->GetOutput() );  

  m_RightCost->SetLeftInputImage(inRight->GetOutput());
  m_RightCost->SetRightInputImage(inLeft->GetOutput() );  
  m_RightCost->SetSide('r');
  m_RightCost->SetLeftGradientXInput(m_GradientXRight->GetOutput() ); 
  m_RightCost->SetRightGradientXInput(m_GradientXLeft->GetOutput() ); 

  if(sense==0)
    {
    m_LeftCost->SetMinDisp(dispMin);
    m_LeftCost->SetMaxDisp(dispMax);
    m_RightCost->SetMinDisp(-dispMax);
    m_RightCost->SetMaxDisp(-dispMin);
    }
  else
    {
    m_LeftCost->SetMinDisp(-dispMax);
    m_LeftCost->SetMaxDisp(-dispMin); 
    m_RightCost->SetMinDisp(dispMin);
    m_RightCost->SetMaxDisp(dispMax); 
    }

  m_LeftCost->SetAlpha(alpha);
  m_LeftCost->SetTau1(tau1L);
  m_LeftCost->SetTau2(tau2L); 
  m_LeftCost->UpdateOutputInformation(); 
         
  m_RightCost->SetAlpha(alpha);
  m_RightCost->SetTau1(tau1R);
  m_RightCost->SetTau2(tau2R);    
  m_RightCost->UpdateOutputInformation(); 

  // EXTRACTION OF THE MIN OF THE COST VOLUME
  m_minLeftCost->SetInput(m_LeftCost->GetOutput());
  m_minLeftCost->SetSide('l');
  m_minRightCost->SetInput(m_RightCost->GetOutput());  
  m_minRightCost->SetSide('r');

  // OUTPUTS 
  IntImageWriterType::Pointer writer_m_minLeftCost = IntImageWriterType::New();
  writer_m_minLeftCost->SetFileName( FILENAME("MinLeftCost.tif"));
  writer_m_minLeftCost->SetInput(m_minLeftCost->GetOutput());
  writer_m_minLeftCost->Update();

  IntImageWriterType::Pointer writer_m_minRightCost = IntImageWriterType::New();
  writer_m_minRightCost->SetFileName( FILENAME("MinRightCost.tif"));
  writer_m_minRightCost->SetInput(m_minRightCost->GetOutput());
  writer_m_minRightCost->Update();
  
  return EXIT_SUCCESS;

  }
