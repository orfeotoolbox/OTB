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

	
int testDisparityMap(int argc, char *argv[])
  {


  if(argc < 7) {
  	std::cerr << "Usage: " << argv[0] << " leftImage rightImage minDisp maxDisp radius outputPathFolder" << std::endl;
  	return EXIT_FAILURE;
  	}

  const unsigned int Dimension = 2;
  typedef otb::VectorImage<float> FloatVectorImageType;
    typedef otb::VectorImage<int> IntVectorImageType;

  typedef otb::Image< float, Dimension > FloatImageType;
  typedef otb::Image< int, Dimension > IntImageType;



  typedef otb::ImageFileReader<FloatVectorImageType> ReaderType;
  typedef otb::ImageFileWriter<FloatVectorImageType> ImageWriterType;

  typedef otb::ImageFileReader<IntVectorImageType> IntVectorImageReaderType;
  typedef otb::ImageFileWriter<IntVectorImageType> IntVectorImageWriterType;
  
  typedef otb::ImageFileReader< IntImageType > IntImageReaderType;
  typedef otb::ImageFileWriter< IntImageType > IntImageWriterType;  

    typedef otb::ImageFileReader< FloatImageType > FloatImageReaderType;
  typedef otb::ImageFileWriter< FloatImageType > FloatImageWriterType;  




  ReaderType::Pointer inLeft = ReaderType::New();
  inLeft->SetFileName(argv[1]); //LeftImage  
  inLeft->UpdateOutputInformation();

  ReaderType::Pointer inRight = ReaderType::New();
  inRight->SetFileName(argv[2]);//RightImage
  inRight->UpdateOutputInformation();

  unsigned int dispMin = atoi(argv[3]);
	unsigned int dispMax  = atoi(argv[4]);
  long unsigned int r = atoi(argv[5]);	


  std::string argv6 = std::string(argv[6]);
  #define FILENAME(n) std::string( argv6 + std::string(n)).c_str()



  // // CALCUL GRADIENT
  // typedef otb::LocalGradientVectorImageFilter<FloatVectorImageType, FloatVectorImageType> GradientType;
  // GradientType::Pointer gradL = GradientType::New();
  // GradientType::Pointer gradR = GradientType::New();
  // gradL->SetInput(inLeft->GetOutput());
  // gradR->SetInput(inRight->GetOutput());

  
  // ImageWriterType::Pointer writer_gradL = ImageWriterType::New();
  // writer_gradL->SetFileName( FILENAME("LeftGradient.tif"));
  // writer_gradL->SetInput(gradL->GetOutput());
  // writer_gradL->Update();

  
  // ImageWriterType::Pointer writer_gradR = ImageWriterType::New();
  // writer_gradR->SetFileName( FILENAME("RightGradient.tif"));
  // writer_gradR->SetInput(gradR->GetOutput());
  // writer_gradR->Update();




  ReaderType::Pointer gradL = ReaderType::New();
  gradL->SetFileName("/home/julie/Documents/PROJETS/CVF/results/scene/testCostVolumeFilter/GradientXLeft.tif"); //LeftImage  
  gradL->UpdateOutputInformation();

  ReaderType::Pointer gradR = ReaderType::New();
  gradR->SetFileName("/home/julie/Documents/PROJETS/CVF/results/scene/testCostVolumeFilter/GradientXRight.tif"); //LeftImage  
  gradR->UpdateOutputInformation();

 


  // COST VOLUME  
  typedef otb::LeftCostVolumeImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType >  LeftCostVolumeType; 
    // --- LEFT
  LeftCostVolumeType::Pointer m_LeftCost = LeftCostVolumeType::New();
  m_LeftCost->SetLeftInputImage(inLeft->GetOutput() );
  m_LeftCost->SetRightInputImage(inRight->GetOutput() );  
  m_LeftCost->SetLeftGradientXInput(gradL->GetOutput() ); 
  m_LeftCost->SetRightGradientXInput(gradR->GetOutput() );      
  m_LeftCost->SetMinDisp(dispMin);
  m_LeftCost->SetMaxDisp(dispMax);
  m_LeftCost->Update();


  
  // // ImageWriterType::Pointer writer_LeftCost = ImageWriterType::New();
  // // writer_LeftCost->SetFileName( FILENAME("LeftCost.tif"));
  // // writer_LeftCost->SetInput(m_LeftCost->GetOutput());
  // // writer_LeftCost->Update();  




  typedef otb::RightCostVolumeImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > RightCostVolumeType; 
    // --- RIGHT
  RightCostVolumeType::Pointer m_RightCost = RightCostVolumeType::New();
  m_RightCost->SetLeftInputImage(inRight->GetOutput() );
  m_RightCost->SetRightInputImage(inLeft->GetOutput() );  
  m_RightCost->SetLeftGradientXInput(gradR->GetOutput() ); 
  m_RightCost->SetRightGradientXInput(gradL->GetOutput() );      
  m_RightCost->SetMinDisp(-dispMax);
  m_RightCost->SetMaxDisp(-dispMin);
  m_RightCost->Update();
      
  // ImageWriterType::Pointer writer_RightCost = ImageWriterType::New();
  // writer_RightCost->SetFileName( FILENAME("RightCost.tif"));
  // writer_RightCost->SetInput(m_RightCost->GetOutput());
  // writer_RightCost->Update();



//   typedef otb::MinimumNBandsImageFilter<  FloatVectorImageType, IntImageType> MinCostVolume;  
//   MinCostVolume::Pointer m_minCost = MinCostVolume::New();
//   m_minCost->SetInput(m_LeftCost->GetOutput());


//   // IntImageWriterType::Pointer writer_m_minCost = IntImageWriterType::New();
//   // writer_m_minCost->SetFileName( FILENAME("MinLeftCost.tif"));
//   // writer_m_minCost->SetInput(m_minCost->GetOutput());
//   // writer_m_minCost->Update();


//   //COVARIANCE VISUALIZATION
//   typedef otb::CovarianceMatrixFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > CovarianceMatrixType;
//     // --- LEFT
//   CovarianceMatrixType::Pointer m_LeftCovarianceMatrix = CovarianceMatrixType::New(); 
//   m_LeftCovarianceMatrix->SetInput1(inLeft->GetOutput());
//   m_LeftCovarianceMatrix->SetInput2(m_LeftCost->GetOutput());  
//   m_LeftCovarianceMatrix->SetRadius(0,r);

//   ImageWriterType::Pointer writer_LeftCovarianceMatrix = ImageWriterType::New();
//   writer_LeftCovarianceMatrix->SetFileName( FILENAME("LeftCovarianceMatrix.tif"));
//   writer_LeftCovarianceMatrix->SetInput(m_LeftCovarianceMatrix->GetOutput());
//   writer_LeftCovarianceMatrix->Update();
 




  //WEIGHTS  
  typedef otb::WeightsGuidedFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > Weights_ak_bk;
    // --- LEFT
  Weights_ak_bk::Pointer m_meanLeftCost = Weights_ak_bk::New(); 
  m_meanLeftCost->SetInput1(inLeft->GetOutput());
  m_meanLeftCost->SetInput2(m_LeftCost->GetOutput());  
  m_meanLeftCost->SetRadius(0,r);


  
  // ImageWriterType::Pointer writer_meanLeftCost = ImageWriterType::New();
  // writer_meanLeftCost->SetFileName( FILENAME("LeftWeights.tif"));
  // writer_meanLeftCost->SetInput(m_meanLeftCost->GetOutput());
  // writer_meanLeftCost->Update();
 

   // --- RIGHT
  Weights_ak_bk::Pointer m_meanRightCost = Weights_ak_bk::New(); 
  m_meanRightCost->SetInput1(inRight->GetOutput());
  m_meanRightCost->SetInput2(m_RightCost->GetOutput());  
  m_meanRightCost->SetRadius(0,r);
  

  // ImageWriterType::Pointer writer_meanRightCost = ImageWriterType::New();
  // writer_meanRightCost->SetFileName( FILENAME("RightWeights.tif"));
  // writer_meanRightCost->SetInput(m_meanRightCost->GetOutput());
  // writer_meanRightCost->Update();
  


  //MEAN WEIGHTS   
  typedef otb::MeanVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > MeanVectorImage;
    // --- LEFT
  MeanVectorImage::Pointer m_meanLeftWeights = MeanVectorImage::New();
  m_meanLeftWeights->SetInput1(m_meanLeftCost->GetOutput());
  m_meanLeftWeights->SetInput2(inLeft->GetOutput());
  m_meanLeftWeights->SetRadius(0,r);
  
  // ImageWriterType::Pointer writer_meanLeftWeights = ImageWriterType::New();
  // writer_meanLeftWeights->SetFileName( FILENAME("LeftMeanWeights.tif"));
  // writer_meanLeftWeights->SetInput(m_meanLeftWeights->GetOutput());
  // writer_meanLeftWeights->Update();  


    // --- RIGHT
  MeanVectorImage::Pointer m_meanRightWeights = MeanVectorImage::New();
  m_meanRightWeights->SetInput1(m_meanRightCost->GetOutput());
  m_meanRightWeights->SetInput2(inRight->GetOutput());
  m_meanRightWeights->SetRadius(0,r);
  
  // ImageWriterType::Pointer writer_meanRightWeights = ImageWriterType::New();
  // writer_meanRightWeights->SetFileName( FILENAME("RightMeanWeights.tif"));
  // writer_meanRightWeights->SetInput(m_meanRightWeights->GetOutput());
  // writer_meanRightWeights->Update(); 


  //DISPARITY MAP
    // --- LEFT
  typedef otb::MinimumNBandsImageFilter<  FloatVectorImageType, IntImageType> MinCostVolume;  
  MinCostVolume::Pointer m_LeftDisparity = MinCostVolume::New();
  m_LeftDisparity->SetInput(m_meanLeftWeights->GetOutput());
  
  //typedef otb::ImageFileWriter< IntImageType > IntImageWriterType; 

  IntImageWriterType::Pointer writer_LeftDisparity = IntImageWriterType::New();
  writer_LeftDisparity->SetFileName( FILENAME("LeftDisparity.tif"));
  writer_LeftDisparity->SetInput(m_LeftDisparity->GetOutput());
  writer_LeftDisparity->Update(); 
  
      // --- RIGHT

  typedef otb::MaximumNBandsImageFilter< FloatVectorImageType, IntImageType > MaxCostVolume;  
  MaxCostVolume::Pointer m_RightDisparity = MaxCostVolume::New();
  m_RightDisparity->SetInput(m_meanRightWeights->GetOutput());


  
  IntImageWriterType::Pointer writer_RightDisparity = IntImageWriterType::New();
  writer_RightDisparity->SetFileName( FILENAME("RightDisparity.tif"));
  writer_RightDisparity->SetInput(m_RightDisparity->GetOutput());
  writer_RightDisparity->Update(); 


  //FILTRAGE LEFT DISPARITY PAR FILTRE MEDIAN
  typedef otb::ImageToVectorImageCastFilter<IntImageType,FloatVectorImageType> CastIntImageIntoFloatVecImageFilter;
  CastIntImageIntoFloatVecImageFilter::Pointer m_CastLeftDisparity = CastIntImageIntoFloatVecImageFilter::New();
  m_CastLeftDisparity-> SetInput( const_cast <IntImageType *>( m_LeftDisparity->GetOutput() ));

  ImageWriterType::Pointer writer_FloatVectLeftDisparity = ImageWriterType::New();
  writer_FloatVectLeftDisparity->SetFileName( FILENAME("FloatVectLeftDisparity.tif"));
  writer_FloatVectLeftDisparity->SetInput(m_CastLeftDisparity->GetOutput());
  writer_FloatVectLeftDisparity->Update(); 

  typedef itk::MedianImageFilter< FloatImageType, FloatImageType > MedianFilterType;
  MedianFilterType::Pointer LMedian = MedianFilterType::New();
  MedianFilterType::RadiusType MedianRadius = {{1,1}};
  LMedian->SetRadius(MedianRadius); 

  typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, MedianFilterType> PerBand;  
      // ---LEFT
  PerBand::Pointer m_LeftMedianFilter = PerBand::New();
  m_LeftMedianFilter->SetFilter(LMedian);
  m_LeftMedianFilter->SetInput(inLeft->GetOutput()); 

  ImageWriterType::Pointer LeftMedianwriter = ImageWriterType::New();
  LeftMedianwriter->SetFileName( FILENAME("LeftMedianVectorImage.tif"));
  LeftMedianwriter->SetInput( m_LeftMedianFilter->GetOutput() );
  LeftMedianwriter->Update();

  typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType> ConcatenateVectorImageFilterType;  
  ConcatenateVectorImageFilterType::Pointer m_ConcatenateDispEndInLeftImage = ConcatenateVectorImageFilterType::New();
  m_ConcatenateDispEndInLeftImage->SetInput1(m_CastLeftDisparity->GetOutput());
  m_ConcatenateDispEndInLeftImage->SetInput2(m_LeftMedianFilter->GetOutput());


  typedef  otb::WeightMedianImageFilter< FloatVectorImageType, FloatVectorImageType > WeightMedianType;
   //Left
   WeightMedianType::Pointer m_LeftDispMedian = WeightMedianType::New();
   m_LeftDispMedian-> SetInput(m_ConcatenateDispEndInLeftImage->GetOutput());
    
   
  FloatVectorImageType::SizeType radiusM;
  radiusM[0] = 19;
  radiusM[1] = 19;   
  m_LeftDispMedian->SetRadius(radiusM) ;

  ImageWriterType::Pointer writer_LeftDispMedian = ImageWriterType::New();
  writer_LeftDispMedian->SetFileName( FILENAME("LeftMedianDisparity.tif"));
  writer_LeftDispMedian->SetInput(m_LeftDispMedian->GetOutput());
  writer_LeftDispMedian->Update(); 

  //FAIRE PAREIL POUR RIGHT DISPARITY
  CastIntImageIntoFloatVecImageFilter::Pointer m_CastRightDisparity = CastIntImageIntoFloatVecImageFilter::New();
  m_CastRightDisparity-> SetInput( const_cast <IntImageType *>( m_RightDisparity->GetOutput() ));

  ImageWriterType::Pointer writer_FloatVectRightDisparity = ImageWriterType::New();
  writer_FloatVectRightDisparity->SetFileName( FILENAME("FloatVectRightDisparity.tif"));
  writer_FloatVectRightDisparity->SetInput(m_CastRightDisparity->GetOutput());
  writer_FloatVectRightDisparity->Update(); 

  PerBand::Pointer m_RightMedianFilter = PerBand::New();
  m_RightMedianFilter->SetFilter(LMedian);
  m_RightMedianFilter->SetInput(inRight->GetOutput()); 

  ImageWriterType::Pointer RightMedianwriter = ImageWriterType::New();
  RightMedianwriter->SetFileName( FILENAME("RightMedianVectorImage.tif"));
  RightMedianwriter->SetInput( m_RightMedianFilter->GetOutput() );
  RightMedianwriter->Update();
 
  ConcatenateVectorImageFilterType::Pointer m_ConcatenateDispEndInRightImage = ConcatenateVectorImageFilterType::New();
  m_ConcatenateDispEndInRightImage->SetInput1(m_CastRightDisparity->GetOutput());
  m_ConcatenateDispEndInRightImage->SetInput2(m_RightMedianFilter->GetOutput());

  WeightMedianType::Pointer m_RightDispMedian = WeightMedianType::New();
  m_RightDispMedian-> SetInput(m_ConcatenateDispEndInRightImage->GetOutput());    
  m_RightDispMedian->SetRadius(radiusM) ;

  ImageWriterType::Pointer writer_RightDispMedian = ImageWriterType::New();
  writer_RightDispMedian->SetFileName( FILENAME("RightMedianDisparity.tif"));
  writer_RightDispMedian->SetInput(m_RightDispMedian->GetOutput());
  writer_RightDispMedian->Update(); 


  return EXIT_SUCCESS;

  }
