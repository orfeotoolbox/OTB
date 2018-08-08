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
#include <itkConstantBoundaryCondition.h>
#include <otbConvolutionImageFilter.h>
#include <itkArray.h>
#include <otbPerBandVectorImageFilter.h>

#include "otbConvertionRGBToGrayLevelImageFilter.h"


#include "otbWeightsGuidedFilter.h"
#include "otbMeanVectorImageFilter.h"
#include "otbConvertDisparityValue.h"



  
int otbGuidedFilterTest(int argc, char *argv[])
  {

  if(argc < 12) 
    {
    std::cerr << "Usage: " << argv[0] << " leftImage rightImage minDisp maxDisp alpha tau1L tau2L tau1R tau2R radiusGF sense outputPathFolder" << std::endl;
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
  std::cout <<"Left"<< inLeft->GetFileName()<< std::endl;
  inLeft->UpdateOutputInformation();
  
  FloatVectorImageReaderType::Pointer inRight = FloatVectorImageReaderType::New();
  inRight->SetFileName(argv[2]);//RightImage  
  std::cout <<"Right"<< inRight->GetFileName()<< std::endl;
  inRight->UpdateOutputInformation();
  int dispMin = atoi(argv[3]);
  int dispMax  = atoi(argv[4]);

  float alpha = atof(argv[5]);
  float tau1L = atof(argv[6]);
  float tau2L = atof(argv[7]);

  float tau1R = atof(argv[8]);
  float tau2R = atof(argv[9]);


  unsigned int r = atoi(argv[10]) ;
  int sense = atoi(argv[11]) ;

  std::string argv12 = std::string(argv[12]);
  #define FILENAME(n) std::string( argv12 + std::string(n)).c_str()


  
  typedef itk::ConstantBoundaryCondition<FloatImageType> BoundaryConditionType;
  typedef otb::ConvolutionImageFilter<FloatImageType, FloatImageType, BoundaryConditionType> ConvFilterType;
  typedef otb::ConvertionRGBToGrayLevelImageFilter<FloatVectorImageType,FloatVectorImageType> RGBTograylevelFilter ;
  typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, ConvFilterType> VectorFilterType;
  typedef otb::CostVolumeFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType >  CostVolumeType;  
  typedef otb::MinimumVectorImageFilter<  FloatVectorImageType, IntImageType> MinCostVolume;    
  typedef otb::WeightsGuidedFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > Weights_ak_bk;
  typedef otb::MeanVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > MeanVectorImage;
  typedef otb::ConvertDisparityValue<IntImageType, IntImageType > ConvertToDisparityValue ;

  ConvFilterType::Pointer m_convFilterXLeft = ConvFilterType::New() ;
  ConvFilterType::Pointer m_convFilterXRight = ConvFilterType::New() ;
  RGBTograylevelFilter::Pointer m_LeftGrayVectorImage = RGBTograylevelFilter::New() ;
  RGBTograylevelFilter::Pointer m_RightGrayVectorImage = RGBTograylevelFilter::New() ;
  VectorFilterType::Pointer m_GradientXLeft = VectorFilterType::New();
  VectorFilterType::Pointer m_GradientXRight = VectorFilterType::New();
  CostVolumeType::Pointer m_LeftCost = CostVolumeType::New() ;
  CostVolumeType::Pointer m_RightCost = CostVolumeType::New() ;

  MinCostVolume::Pointer m_minLeftGF = MinCostVolume::New() ;
  MinCostVolume::Pointer m_minRightGF = MinCostVolume::New() ;

  Weights_ak_bk::Pointer m_meanLeftCost = Weights_ak_bk::New() ; 
  Weights_ak_bk::Pointer m_meanRightCost = Weights_ak_bk::New() ; 
  MeanVectorImage::Pointer m_meanLeftWeights = MeanVectorImage::New() ;
  MeanVectorImage::Pointer m_meanRightWeights = MeanVectorImage::New() ;

  ConvertToDisparityValue::Pointer m_LeftDisparity = ConvertToDisparityValue::New();
  ConvertToDisparityValue::Pointer m_RightDisparity = ConvertToDisparityValue::New();


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

  //WEIGHTS  
     // --- LEFT
  m_meanLeftCost->SetInput1(inLeft->GetOutput());
  m_meanLeftCost->SetInput2(m_LeftCost->GetOutput());  
  m_meanLeftCost->SetRadius(0,r);  
  m_meanLeftCost->UpdateOutputInformation(); 
   // --- RIGHT
  m_meanRightCost->SetInput1(inRight->GetOutput());
  m_meanRightCost->SetInput2(m_RightCost->GetOutput());  
  m_meanRightCost->SetRadius(0,r);  
  m_meanRightCost->UpdateOutputInformation(); 
  //MEAN WEIGHTS   
  //  --- LEFT
  m_meanLeftWeights->SetInput1(m_meanLeftCost->GetOutput());
  m_meanLeftWeights->SetInput2(inLeft->GetOutput());
  m_meanLeftWeights->SetRadius(0,r);  
  m_meanLeftWeights->UpdateOutputInformation(); 
    // --- RIGHT
  m_meanRightWeights->SetInput1(m_meanRightCost->GetOutput());
  m_meanRightWeights->SetInput2(inRight->GetOutput());
  m_meanRightWeights->SetRadius(0,r); 
  m_meanRightWeights->UpdateOutputInformation(); 
  //DISPARITY MAP
    // --- LEFT   
  m_minLeftGF->SetInput(m_meanLeftWeights->GetOutput()); 
  m_minLeftGF->SetSide('l');
  m_minLeftGF->UpdateOutputInformation(); 
      // --- RIGHT
  m_minRightGF->SetInput(m_meanRightWeights->GetOutput());  
  m_minRightGF->SetSide('r');
  m_minRightGF->UpdateOutputInformation(); 

  m_LeftDisparity->SetInput(m_minLeftGF->GetOutput());
  m_RightDisparity->SetInput(m_minRightGF->GetOutput());

  if(sense==0)
    {
    m_LeftDisparity->SetDisp(dispMax);
    m_RightDisparity->SetDisp(-dispMax);
    }
  else
    {
    m_LeftDisparity->SetDisp(-dispMin);
    m_RightDisparity->SetDisp(dispMin);
    }

  IntImageWriterType::Pointer writer_m_LeftDisparity = IntImageWriterType::New();
  writer_m_LeftDisparity->SetFileName( FILENAME("LeftDisparity.tif"));
  writer_m_LeftDisparity->SetInput(m_LeftDisparity->GetOutput());
  writer_m_LeftDisparity->Update();
  
  IntImageWriterType::Pointer writer_m_RightDisparity = IntImageWriterType::New();
  writer_m_RightDisparity->SetFileName( FILENAME("RightDisparity.tif"));
  writer_m_RightDisparity->SetInput(m_RightDisparity->GetOutput());
  writer_m_RightDisparity->Update();

  return EXIT_SUCCESS;

  }
