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
#include "otbWeightsGuidedFilter.h"
#include "otbMeanVectorImageFilter.h"
#include "otbConcatenateVectorImageFilter.h"
#include "otbWeightedMedianImageFilter.h"
#include "otbCovarianceMatrix.h"

#include <otbImageToVectorImageCastFilter.h>
#include <itkMedianImageFilter.h>
#include <otbPerBandVectorImageFilter.h>

#include <itkConstantBoundaryCondition.h>
#include <otbConvolutionImageFilter.h>
#include <itkArray.h>

//

#include "otbBijectionCoherencyFilter.h"
#include "otbFillOcclusionDisparityImageFilter.h"
#include "otbImageToVectorImageCastFilter.h"
#include "otbFillOcclusionImageFilter.h"
#include "otbConvertValueFrom0To255.h"


  
int testAllFilters(int argc, char *argv[])
  {


  if(argc < 7) {
    std::cerr << "Usage: " << argv[0] << " leftImage rightImage minDisp maxDisp radius rwmf outputPathFolder" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 2;
  typedef otb::VectorImage<float> FloatVectorImageType;
  // typedef otb::VectorImage<int> IntVectorImageType;

  typedef otb::Image< float, Dimension > FloatImageType;
  typedef otb::Image< int, Dimension > IntImageType;

  typedef otb::ImageFileReader<FloatVectorImageType> FloatVectorImageReaderType;
  // typedef otb::ImageFileWriter<FloatVectorImageType> FloatVectorImageWriterType;

  // typedef otb::ImageFileReader<IntVectorImageType> IntVectorImageReaderType;
  // typedef otb::ImageFileWriter<IntVectorImageType> IntVectorImageWriterType;
  
  // typedef otb::ImageFileReader< IntImageType > IntImageReaderType;
  typedef otb::ImageFileWriter< IntImageType > IntImageWriterType;  

  // typedef otb::ImageFileReader< FloatImageType > FloatImageReaderType;
  // typedef otb::ImageFileWriter< FloatImageType > FloatImageWriterType;  


  FloatVectorImageReaderType::Pointer inLeft = FloatVectorImageReaderType::New();
  inLeft->SetFileName(argv[1]); //LeftImage  
  inLeft->UpdateOutputInformation();

  FloatVectorImageReaderType::Pointer inRight = FloatVectorImageReaderType::New();
  inRight->SetFileName(argv[2]);//RightImage
  inRight->UpdateOutputInformation();

  int dispMin = atoi(argv[3]);
  int dispMax  = atoi(argv[4]);
  long unsigned int r = atoi(argv[5]);  
  unsigned int rwmf = atoi(argv[6]);

  std::cout << "disMin : " << dispMin << std::endl ;
   std::cout << "disMax : " << dispMax << std::endl ;


  std::string argv7 = std::string(argv[7]);
  #define FILENAME(n) std::string( argv7 + std::string(n)).c_str()



  typedef itk::ConstantBoundaryCondition<FloatImageType> BoundaryConditionType;
  typedef otb::ConvolutionImageFilter<FloatImageType, FloatImageType, BoundaryConditionType> ConvFilterType;
  ConvFilterType::Pointer m_convFilterXLeft = ConvFilterType::New();
  ConvFilterType::Pointer m_convFilterXRight = ConvFilterType::New();

  // Gradient X  
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
  typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, ConvFilterType> VectorFilterType;
  VectorFilterType::Pointer m_GradientXLeft = VectorFilterType::New();
  m_GradientXLeft->SetFilter(m_convFilterXLeft);
  m_GradientXLeft->SetInput(inLeft->GetOutput());
  m_GradientXLeft->UpdateOutputInformation();
 
  VectorFilterType::Pointer m_GradientXRight = VectorFilterType::New();
  m_GradientXRight->SetFilter(m_convFilterXRight);
  m_GradientXRight->SetInput(inRight->GetOutput());
  m_GradientXRight->UpdateOutputInformation();

  
  // COST VOLUME  
  typedef otb::LeftCostVolumeImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType >  LeftCostVolumeType; 
    // --- LEFT
  LeftCostVolumeType::Pointer m_LeftCost = LeftCostVolumeType::New();
  m_LeftCost->SetLeftInputImage(inLeft->GetOutput() );
  m_LeftCost->SetRightInputImage(inRight->GetOutput() );  
  m_LeftCost->SetLeftGradientXInput(m_GradientXLeft->GetOutput() ); 
  m_LeftCost->SetRightGradientXInput(m_GradientXRight->GetOutput() );      
  m_LeftCost->SetMinDisp(dispMin);
  m_LeftCost->SetMaxDisp(dispMax);
  m_LeftCost->UpdateOutputInformation();

  typedef otb::RightCostVolumeImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > RightCostVolumeType; 
    // --- RIGHT
  RightCostVolumeType::Pointer m_RightCost = RightCostVolumeType::New();
  m_RightCost->SetLeftInputImage(inRight->GetOutput() );
  m_RightCost->SetRightInputImage(inLeft->GetOutput() );  
  m_RightCost->SetLeftGradientXInput(m_GradientXRight->GetOutput() ); 
  m_RightCost->SetRightGradientXInput(m_GradientXLeft->GetOutput() );      
  m_RightCost->SetMinDisp(-dispMax);
  m_RightCost->SetMaxDisp(-dispMin);
  m_RightCost->UpdateOutputInformation();

  //WEIGHTS  
  typedef otb::WeightsGuidedFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > Weights_ak_bk;
    // --- LEFT
  Weights_ak_bk::Pointer m_meanLeftCost = Weights_ak_bk::New(); 
  m_meanLeftCost->SetInput1(inLeft->GetOutput());
  m_meanLeftCost->SetInput2(m_LeftCost->GetOutput());  
  m_meanLeftCost->SetRadius(0,r);
   // --- RIGHT
  Weights_ak_bk::Pointer m_meanRightCost = Weights_ak_bk::New(); 
  m_meanRightCost->SetInput1(inRight->GetOutput());
  m_meanRightCost->SetInput2(m_RightCost->GetOutput());  
  m_meanRightCost->SetRadius(0,r);
  


  //MEAN WEIGHTS   
  typedef otb::MeanVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > MeanVectorImage;
    // --- LEFT
  MeanVectorImage::Pointer m_meanLeftWeights = MeanVectorImage::New();
  m_meanLeftWeights->SetInput1(m_meanLeftCost->GetOutput());
  m_meanLeftWeights->SetInput2(inLeft->GetOutput());
  m_meanLeftWeights->SetRadius(0,r);
    // --- RIGHT
  MeanVectorImage::Pointer m_meanRightWeights = MeanVectorImage::New();
  m_meanRightWeights->SetInput1(m_meanRightCost->GetOutput());
  m_meanRightWeights->SetInput2(inRight->GetOutput());
  m_meanRightWeights->SetRadius(0,r);
  


  //DISPARITY MAP
    // --- LEFT
  typedef otb::MinimumNBandsImageFilter<  FloatVectorImageType, IntImageType> MinCostVolume;  
  MinCostVolume::Pointer m_LeftDisparity = MinCostVolume::New();
  m_LeftDisparity->SetInput(m_meanLeftWeights->GetOutput());
      // --- RIGHT
  typedef otb::MaximumNBandsImageFilter< FloatVectorImageType, IntImageType > MaxCostVolume;  
  MaxCostVolume::Pointer m_RightDisparity = MaxCostVolume::New();
  m_RightDisparity->SetInput(m_meanRightWeights->GetOutput());
  


  //FILTRAGE LEFT DISPARITY PAR FILTRE MEDIAN
  typedef otb::ImageToVectorImageCastFilter<IntImageType,FloatVectorImageType> CastIntImageIntoFloatVecImageFilter;
  CastIntImageIntoFloatVecImageFilter::Pointer m_CastLeftDisparity = CastIntImageIntoFloatVecImageFilter::New();
  m_CastLeftDisparity-> SetInput(  m_LeftDisparity->GetOutput());



  typedef itk::MedianImageFilter< FloatImageType, FloatImageType > MedianFilterType;
  MedianFilterType::Pointer LMedian = MedianFilterType::New();
  MedianFilterType::RadiusType MedianRadius = {{1,1}};
  LMedian->SetRadius(MedianRadius); 

  typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, MedianFilterType> PerBand;  
      // ---LEFT
  PerBand::Pointer m_LeftMedianFilter = PerBand::New();
  m_LeftMedianFilter->SetFilter(LMedian);
  m_LeftMedianFilter->SetInput(inLeft->GetOutput()); 

  typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType> ConcatenateVectorImageFilterType;  
  ConcatenateVectorImageFilterType::Pointer m_ConcatenateDispEndInLeftImage = ConcatenateVectorImageFilterType::New();
  m_ConcatenateDispEndInLeftImage->SetInput1(m_CastLeftDisparity->GetOutput());
  m_ConcatenateDispEndInLeftImage->SetInput2(m_LeftMedianFilter->GetOutput());


  typedef  otb::WeightMedianImageFilter< FloatVectorImageType, IntImageType > WeightMedianType;
   //Left
   WeightMedianType::Pointer m_LeftDispMedian = WeightMedianType::New();
   m_LeftDispMedian-> SetInput(m_ConcatenateDispEndInLeftImage->GetOutput());    
   
  FloatVectorImageType::SizeType radiusWM;
  radiusWM[0] = r;
  radiusWM[1] = r;   
  m_LeftDispMedian->SetRadius(radiusWM) ;

  // IntImageWriterType::Pointer writer_LeftDispMedian = IntImageWriterType::New();
  // writer_LeftDispMedian->SetFileName( FILENAME("LeftMedianDisparity.tif"));
  // writer_LeftDispMedian->SetInput(m_LeftDispMedian->GetOutput());
  // writer_LeftDispMedian->Update(); 

  //FAIRE PAREIL POUR RIGHT DISPARITY
  CastIntImageIntoFloatVecImageFilter::Pointer m_CastRightDisparity = CastIntImageIntoFloatVecImageFilter::New();
  m_CastRightDisparity-> SetInput( m_RightDisparity->GetOutput() );


  PerBand::Pointer m_RightMedianFilter = PerBand::New();
  m_RightMedianFilter->SetFilter(LMedian);
  m_RightMedianFilter->SetInput(inRight->GetOutput()); 

  ConcatenateVectorImageFilterType::Pointer m_ConcatenateDispEndInRightImage = ConcatenateVectorImageFilterType::New();
  m_ConcatenateDispEndInRightImage->SetInput1(m_CastRightDisparity->GetOutput());
  m_ConcatenateDispEndInRightImage->SetInput2(m_RightMedianFilter->GetOutput());

  WeightMedianType::Pointer m_RightDispMedian = WeightMedianType::New();
  m_RightDispMedian-> SetInput(m_ConcatenateDispEndInRightImage->GetOutput());    
  m_RightDispMedian->SetRadius(radiusWM) ;



  typedef otb::BijectionCoherencyFilter< IntImageType, IntImageType > OcclusionType;
  OcclusionType::Pointer m_OcclusionFilter = OcclusionType::New();  
  m_OcclusionFilter->SetDirectHorizontalDisparityMapInput(m_LeftDispMedian->GetOutput()); 
  m_OcclusionFilter->SetReverseHorizontalDisparityMapInput(m_RightDispMedian->GetOutput()); 
   
  m_OcclusionFilter->SetMaxHDisp(dispMax);
  m_OcclusionFilter->SetMinHDisp(dispMin);
  m_OcclusionFilter->SetMinVDisp(0);
  m_OcclusionFilter->SetMaxVDisp(0);
  m_OcclusionFilter->SetTolerance(2);  
   // IntImageWriterType::Pointer OcclusionWriter = IntImageWriterType::New(); 
   // OcclusionWriter->SetFileName( FILENAME("Occlusions.tif"));
   // OcclusionWriter->SetInput( m_OcclusionFilter->GetOutput() );  


   typedef otb::FillOcclusionDisparityImageFilter<IntImageType, IntImageType, IntImageType> FillOcclusionFilter ;
   FillOcclusionFilter::Pointer m_FillOccDisparityMap = FillOcclusionFilter::New();
   m_FillOccDisparityMap->SetInput1(m_OcclusionFilter->GetOutput() );
   m_FillOccDisparityMap->SetInput2(m_LeftDispMedian->GetOutput() );
   m_FillOccDisparityMap->SetRadius(0,1);
   // IntImageWriterType::Pointer writer_FillOcclusions = IntImageWriterType::New(); 
   // writer_FillOcclusions->SetFileName( FILENAME("FillOcclusions.tif"));
   // writer_FillOcclusions->SetInput( m_FillOccDisparityMap->GetOutput() );  
   // writer_FillOcclusions->Update(); 

   typedef otb::ImageToVectorImageCastFilter<IntImageType,FloatVectorImageType> CastImageFilter;
  CastImageFilter::Pointer m_CastOccMap = CastImageFilter::New();
  m_CastOccMap-> SetInput( const_cast <IntImageType *>( m_FillOccDisparityMap->GetOutput() ));



  typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType> ConcatenateVectorImageFilterType;  
  ConcatenateVectorImageFilterType::Pointer m_ConcatenateCastOccMapAndLeftImage = ConcatenateVectorImageFilterType::New();
  m_ConcatenateCastOccMapAndLeftImage->SetInput1(m_CastOccMap->GetOutput());
  m_ConcatenateCastOccMapAndLeftImage->SetInput2(m_LeftMedianFilter->GetOutput());


  typedef  otb::WeightMedianImageFilter< FloatVectorImageType, FloatVectorImageType > WeightMedianFilter;
  WeightMedianFilter::Pointer m_WeightOccMapAndLeftImageFilter = WeightMedianFilter::New();
  m_WeightOccMapAndLeftImageFilter->SetInput(m_ConcatenateCastOccMapAndLeftImage->GetOutput());

  FloatVectorImageType::SizeType radiusM;
  radiusM[0] = rwmf;
  radiusM[1] = rwmf;   
  m_WeightOccMapAndLeftImageFilter->SetRadius(radiusM);


 //  FloatVectorImageWriterType::Pointer writer_smoothFillDisparity = FloatVectorImageWriterType::New();
 //  writer_smoothFillDisparity->SetFileName( FILENAME("SmoothFillDisparity.tif"));
 // writer_smoothFillDisparity->SetInput( m_WeightOccMapAndLeftImageFilter->GetOutput() );  
 // writer_smoothFillDisparity->Update(); 

  // typedef otb::ConvertValueFrom0To255<FloatVectorImageType, IntImageType > ConvertValue ;
  // ConvertValue::Pointer m_convertSmoothDisparity = ConvertValue::New();
  // m_convertSmoothDisparity->SetInput(m_WeightOccMapAndLeftImageFilter->GetOutput());
  // m_convertSmoothDisparity->SetDispMin(dispMin);
  // m_convertSmoothDisparity->SetDispMax(dispMax);

  // IntImageWriterType::Pointer writer_convertSmoothFillDisparity = IntImageWriterType::New();
  // writer_convertSmoothFillDisparity->SetFileName( FILENAME("ConvertSmoothFillDisparity.tif"));
  // writer_convertSmoothFillDisparity->SetInput( m_convertSmoothDisparity->GetOutput() ); 
  // writer_convertSmoothFillDisparity->Update();




  return EXIT_SUCCESS;

  }
