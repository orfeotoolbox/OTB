

/* * Copyright (C) Damia Belazouz - 2017
 *
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
#include "otbCostVolumeVectorImageFilter.h"
#include "otbCostVolumeImageFilter.h"
#include "otbMinimumNBandsImageFilter.h"
#include "otbMaximumNBandsImageFilter.h"
#include "otbWeightsGuidedFilter.h"
#include "otbMeanVectorImageFilter.h"
#include "otbConcatenateVectorImageFilter.h"
#include "otbWeightedMedianImageFilter.h"
#include "otbVectorImageToImageListFilter.h"
#include "otbBijectionCoherencyFilter.h"
#include "otbFillOcclusionDisparityImageFilter.h"
#include "otbImageToVectorImageCastFilter.h"

#include "itkMedianImageFilter.h"
#include "itkMeanImageFilter.h"
#include "otbPerBandVectorImageFilter.h"
#include "otbCovarianceMatrix.h"
#include "itkCastImageFilter.h"







	
int testCVF(int argc, char *argv[])
  {


  if(argc < 9) {
  	std::cerr << "Usage: " << argv[0] << " leftImage rightImage minDisp maxDisp radius radiusToMedianImageFilter radiusToWeightFilter outputPathFolder" << std::endl;
  	return EXIT_FAILURE;
  	}

  static const unsigned int Dimension = 2;
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

  int dispMin = atoi(argv[3]);
	int dispMax  = atoi(argv[4]);
  long unsigned int r = atoi(argv[5]);	
  int rmf = atoi(argv[6]) ;
  int rwmf = atoi(argv[7]) ;


  std::string argv8 = std::string(argv[8]);
  #define FILENAME(n) std::string( argv8 + std::string(n)).c_str()


  // CALCUL GRADIENT
  typedef otb::LocalGradientVectorImageFilter<FloatVectorImageType, FloatVectorImageType> GradientType;
  GradientType::Pointer reader_XL = GradientType::New();
  GradientType::Pointer reader_XR = GradientType::New();
  reader_XL->SetInput(inLeft->GetOutput());
  reader_XR->SetInput(inRight->GetOutput());

  
  ImageWriterType::Pointer writer_gradX = ImageWriterType::New();
  writer_gradX->SetFileName( FILENAME("LeftGradient.tif"));
  writer_gradX->SetInput(reader_XL->GetOutput());
  writer_gradX->Update();

  
  ImageWriterType::Pointer writer_gradY = ImageWriterType::New();
  writer_gradY->SetFileName( FILENAME("RightGradient.tif"));
  writer_gradY->SetInput(reader_XR->GetOutput());
  writer_gradY->Update();



  // ReaderType::Pointer reader_XL = ReaderType::New();
  // reader_XL->SetFileName("/home/julie/Documents/PROJETS/CVF/results/scene/testCostVolumeFilter/GradientXLeft.tif"); //LeftImage  
  // reader_XL->UpdateOutputInformation();

  // ReaderType::Pointer reader_XR = ReaderType::New();
  // reader_XR->SetFileName("/home/julie/Documents/PROJETS/CVF/results/scene/testCostVolumeFilter/GradientXRight.tif"); //LeftImage  
  // reader_XR->UpdateOutputInformation();


 


  // COST VOLUME  
  typedef otb::CostVolumeImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > CostVolumeType; 
    // --- LEFT
  CostVolumeType::Pointer m_LeftCost = CostVolumeType::New();
  m_LeftCost->SetLeftInputImage(inLeft->GetOutput() );
  m_LeftCost->SetRightInputImage(inRight->GetOutput() );  
  m_LeftCost->SetLeftGradientXInput(reader_XL->GetOutput() ); 
  m_LeftCost->SetRightGradientXInput(reader_XR->GetOutput() );      
  m_LeftCost->SetMinDisp(dispMin);
  m_LeftCost->SetMaxDisp(dispMax);
  m_LeftCost->Update();


  
  // // ImageWriterType::Pointer writer_LeftCost = ImageWriterType::New();
  // // writer_LeftCost->SetFileName( FILENAME("LeftCost.tif"));
  // // writer_LeftCost->SetInput(m_LeftCost->GetOutput());
  // // writer_LeftCost->Update();  




  typedef otb::CostVolumeVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > CostVolumeVectorType; 
    // --- RIGHT
  CostVolumeVectorType::Pointer m_RightCost = CostVolumeVectorType::New();
  m_RightCost->SetLeftInputImage(inRight->GetOutput() );
  m_RightCost->SetRightInputImage(inLeft->GetOutput() );  
  m_RightCost->SetLeftGradientXInput(reader_XR->GetOutput() ); 
  m_RightCost->SetRightGradientXInput(reader_XL->GetOutput() );      
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


 

  //MEDIAN INPUT IMAGES
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
  LeftMedianwriter->SetFileName( FILENAME("LeftMedian.tif"));
  LeftMedianwriter->SetInput( m_LeftMedianFilter->GetOutput() );
  LeftMedianwriter->Update();

          // ---RIGHT
  PerBand::Pointer m_RightMedianFilter = PerBand::New();
  m_RightMedianFilter->SetFilter(LMedian);
  m_RightMedianFilter->SetInput(inRight->GetOutput()); 

  ImageWriterType::Pointer RightMedianwriter = ImageWriterType::New();
  RightMedianwriter->SetFileName( FILENAME("RightMedian.tif"));
  RightMedianwriter->SetInput( m_RightMedianFilter->GetOutput() );
  RightMedianwriter->Update();



// // OCCLUSION DETECTION
//   typedef otb::BijectionCoherencyFilter< IntImageType, IntImageType > OcclusionType;
//   OcclusionType::Pointer m_OcclusionFilter = OcclusionType::New();  
//   m_OcclusionFilter->SetDirectHorizontalDisparityMapInput(m_LeftDisparity->GetOutput()); 
//   m_OcclusionFilter->SetReverseHorizontalDisparityMapInput(m_RightDisparity->GetOutput()); 
   
//   m_OcclusionFilter->SetMaxHDisp(dispMax);
//   m_OcclusionFilter->SetMinHDisp(dispMin);
//   m_OcclusionFilter->SetMinVDisp(0);
//   m_OcclusionFilter->SetMaxVDisp(0);
//   m_OcclusionFilter->SetTolerance(1);
  

//   IntImageWriterType::Pointer OcclusionWriter = IntImageWriterType::New(); 
//   OcclusionWriter->SetFileName( FILENAME("Occlusions.tif"));
//   OcclusionWriter->SetInput( m_OcclusionFilter->GetOutput() );  
//   OcclusionWriter->Update(); 


//  typedef otb::FillOcclusionDisparityImageFilter<IntImageType, IntImageType, IntImageType> FillOcclusionFilter ;
//  FillOcclusionFilter::Pointer m_FillOccDisparityMap = FillOcclusionFilter::New();
//  m_FillOccDisparityMap->SetInput1(m_OcclusionFilter->GetOutput() );
//  m_FillOccDisparityMap->SetInput2(m_LeftDisparity->GetOutput() );
//  m_FillOccDisparityMap->SetRadius(0,4);


//  IntImageWriterType::Pointer writer_FillOcclusions = IntImageWriterType::New(); 
//  writer_FillOcclusions->SetFileName( FILENAME("FillOcclusions.tif"));
//  writer_FillOcclusions->SetInput( m_FillOccDisparityMap->GetOutput() );  
//  writer_FillOcclusions->Update(); 


//   typedef otb::ImageToVectorImageCastFilter<IntImageType,FloatVectorImageType> CastImageFilter;
//   CastImageFilter::Pointer m_CastOccMap = CastImageFilter::New();
//   m_CastOccMap-> SetInput( const_cast <IntImageType *>( m_FillOccDisparityMap->GetOutput() ));



//   typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType> ConcatenateVectorImageFilterType;  
//   ConcatenateVectorImageFilterType::Pointer m_ConcatenateCastOccMapAndLeftImage = ConcatenateVectorImageFilterType::New();
//   m_ConcatenateCastOccMapAndLeftImage->SetInput1(m_CastOccMap->GetOutput());
//   m_ConcatenateCastOccMapAndLeftImage->SetInput2(m_LeftMedianFilter->GetOutput());


//   typedef  otb::WeightMedianImageFilter< FloatVectorImageType, FloatVectorImageType > WeightMedianFilter;
//   WeightMedianFilter::Pointer m_WeightOccMapAndLeftImageFilter = WeightMedianFilter::New();
//   m_WeightOccMapAndLeftImageFilter->SetInput(m_ConcatenateCastOccMapAndLeftImage->GetOutput());

//   FloatVectorImageType::SizeType radiusM;
//   radiusM[0] = r;
//   radiusM[1] = r;   
//   m_WeightOccMapAndLeftImageFilter->SetRadius(radiusM);



//   ImageWriterType::Pointer writer_smoothFillDisparity = ImageWriterType::New();
//   writer_smoothFillDisparity->SetFileName( FILENAME("SmoothFillDisparity.tif"));
//  writer_smoothFillDisparity->SetInput( m_WeightOccMapAndLeftImageFilter->GetOutput() );  
//  writer_smoothFillDisparity->Update(); 






  // typedef otb::ImageToVectorImageCastFilter<IntImageType,FloatVectorImageType> CastIntImageFilter;
  // CastIntImageFilter::Pointer m_CastLeftDisparity = CastIntImageFilter::New();
  // m_CastLeftDisparity-> SetInput( const_cast <IntImageType *>( m_LeftDisparity->GetOutput() ));

  // typedef otb::ImageToVectorImageCastFilter<IntImageType,FloatVectorImageType> CastFloatImageFilter;
  // CastFloatImageFilter::Pointer m_CastLeftMedianFilter = CastFloatImageFilter::New();
  // m_CastLeftMedianFilter-> SetInput( const_cast <FloatImageType *>( m_LeftDisparity->GetOutput() ));





  // typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType> ConcatenateVectorImageFilterType;  
  // ConcatenateVectorImageFilterType::Pointer m_ConcatenateDispEndInLeftImage = ConcatenateVectorImageFilterType::New();
  // m_ConcatenateDispEndInLeftImage->SetInput1(m_CastLeftDisparity->GetOutput());
  // m_ConcatenateDispEndInLeftImage->SetInput2(m_CastLeftMedianFilter->GetOutput());





  // typedef otb::ConcatenateVectorImageFilter< IntVectorImageType, IntVectorImageType, IntVectorImageType> ConcatenateVectorImageFilterType;  
  // ConcatenateVectorImageFilterType::Pointer m_ConcatenateDispEndInLeftImage = ConcatenateVectorImageFilterType::New();
  // m_ConcatenateDispEndInLeftImage->SetInput1(m_CastLeftDisparity->GetOutput());
  // m_ConcatenateDispEndInLeftImage->SetInput2(m_CastLeftDisparity->GetOutput());





//         // ---RIGHT
//   PerBand::Pointer m_RightMedianFilter = PerBand::New();
//   m_RightMedianFilter->SetFilter(LMedian);
//   m_RightMedianFilter->SetInput(inRight->GetOutput()); 
//   ImageWriterType::Pointer RightMedianwriter = ImageWriterType::New();
//   RightMedianwriter->SetFileName( FILENAME("RightMedian.tif"));
//   RightMedianwriter->SetInput( m_RightMedianFilter->GetOutput() );
//   RightMedianwriter->Update();



//   /////////////
//      // --- LEFT
//   MeanVectorImage::Pointer m_meanMedianLeftWeights = MeanVectorImage::New();
//   m_meanMedianLeftWeights->SetInput1(m_meanLeftCost->GetOutput());
//   m_meanMedianLeftWeights->SetInput2(m_LeftMedianFilter->GetOutput());
//   m_meanMedianLeftWeights->SetRadius(0,r);
  
//     // --- RIGHT
//   MeanVectorImage::Pointer m_meanMedianRightWeights = MeanVectorImage::New();
//   m_meanMedianRightWeights->SetInput1(m_meanRightCost->GetOutput());
//   m_meanMedianRightWeights->SetInput2(m_RightMedianFilter->GetOutput());
//   m_meanMedianRightWeights->SetRadius(0,r);


//   //DISPARITY MAP
//   typedef otb::MinimumNBandsImageFilter< FloatVectorImageType, IntImageType > MinCostVolume;  
//     // --- LEFT
//   MinCostVolume::Pointer m_MedianLeftDisparity = MinCostVolume::New();
//   m_MedianLeftDisparity->SetInput(m_meanMedianLeftWeights->GetOutput());

//   IntImageWriterType::Pointer writer_MedianLeftDisparity = IntImageWriterType::New();
//   writer_MedianLeftDisparity->SetFileName( FILENAME("LeftMedianDisparity.tif"));
//   writer_MedianLeftDisparity->SetInput(m_MedianLeftDisparity->GetOutput());
//   writer_MedianLeftDisparity->Update(); 
  
//       // --- RIGHT

//   typedef otb::MaximumNBandsImageFilter< FloatVectorImageType, IntImageType > MaxCostVolume;  
//   MaxCostVolume::Pointer m_MedianRightDisparity = MaxCostVolume::New();
//   m_MedianRightDisparity->SetInput(m_meanMedianRightWeights->GetOutput());


  
//   IntImageWriterType::Pointer writer_MedianRightDisparity = IntImageWriterType::New();
//   writer_MedianRightDisparity->SetFileName( FILENAME("RightMedianDisparity.tif"));
//   writer_MedianRightDisparity->SetInput(m_MedianRightDisparity->GetOutput());
//   writer_MedianRightDisparity->Update(); 




  return EXIT_SUCCESS;

  }
