

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
#include "otbWeightsGuidedFilter.h"
#include "otbMeanVectorImageFilter.h"
#include "otbConcatenateVectorImageFilter.h"
#include "otbWeightedMedianImageFilter.h"
#include "otbVectorImageToImageListFilter.h"
#include "otbBijectionCoherencyFilter.h"
#include "otbStandardFilterWatcher.h"

#include "otbMaximumNBandsImageFilter.h"




	
int testCVF(int argc, char *argv[])
  {
  if(argc < 8) {
  	std::cerr << "Usage: " << argv[0] << " leftImage rightImage minDisp maxDisp radius radiusToMeanFilter outputPathFolder" << std::endl;
  	return EXIT_FAILURE;
  	}

  const unsigned int Dimension = 2;
  typedef otb::VectorImage<float> FloatVectorImageType;
  typedef otb::Image< double, Dimension > FloatImageType;
  typedef otb::Image< int, Dimension > IntImageType;
 // typedef otb::ImageList<FloatImageType>    ImageListType;


  typedef otb::ImageFileReader<FloatVectorImageType> ReaderType;
  typedef otb::ImageFileWriter<FloatVectorImageType> ImageWriterType;
  typedef otb::ImageFileWriter< FloatImageType > OtbImageWriterType; 
  typedef otb::ImageFileReader< FloatImageType > OtbImageReaderType; 




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

  std::string argv7 = std::string(argv[7]);
  #define FILENAME(n) std::string( argv7 + std::string(n)).c_str()



  // CALCUL GRADIENT
  typedef otb::LocalGradientVectorImageFilter<FloatVectorImageType, FloatVectorImageType> GradientType;
  GradientType::Pointer gradX = GradientType::New();
  GradientType::Pointer gradY = GradientType::New();
  gradX->SetInput(inLeft->GetOutput());
  gradY->SetInput(inRight->GetOutput());

  
  ImageWriterType::Pointer writer_gradX = ImageWriterType::New();
  writer_gradX->SetFileName( FILENAME("LeftGradient.tif"));
  writer_gradX->SetInput(gradX->GetOutput());
  writer_gradX->Update();

  
  ImageWriterType::Pointer writer_gradY = ImageWriterType::New();
  writer_gradY->SetFileName( FILENAME("RightGradient.tif"));
  writer_gradY->SetInput(gradY->GetOutput());
  writer_gradY->Update();


  // COST VOLUME  
  typedef otb::CostVolumeImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > CostVolumeType; 
    // --- LEFT
  CostVolumeType::Pointer m_LeftCost = CostVolumeType::New();
  m_LeftCost->SetLeftInputImage(inLeft->GetOutput() );
  m_LeftCost->SetRightInputImage(inRight->GetOutput() );  
  m_LeftCost->SetLeftGradientXInput(gradX->GetOutput() ); 
  m_LeftCost->SetRightGradientXInput(gradY->GetOutput() );      
  m_LeftCost->SetMinDisp(dispMin);
  m_LeftCost->SetMaxDisp(dispMax);
  m_LeftCost->Update();


  /*
  ImageWriterType::Pointer writer_LeftCost = ImageWriterType::New();
  writer_LeftCost->SetFileName( FILENAME("LeftCost.tif"));
  writer_LeftCost->SetInput(m_LeftCost->GetOutput());
  writer_LeftCost->Update();  
*/



  typedef otb::CostVolumeVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > CostVolumeVectorType; 
    // --- RIGHT
  CostVolumeVectorType::Pointer m_RightCost = CostVolumeVectorType::New();
  m_RightCost->SetLeftInputImage(inRight->GetOutput() );
  m_RightCost->SetRightInputImage(inLeft->GetOutput() );  
  m_RightCost->SetLeftGradientXInput(gradY->GetOutput() ); 
  m_RightCost->SetRightGradientXInput(gradX->GetOutput() );      
  m_RightCost->SetMinDisp(-dispMax);
  m_RightCost->SetMaxDisp(-dispMin);
  m_RightCost->Update();
      
  // ImageWriterType::Pointer writer_RightCost = ImageWriterType::New();
  // writer_RightCost->SetFileName( FILENAME("RightCost.tif"));
  // writer_RightCost->SetInput(m_RightCost->GetOutput());
  // writer_RightCost->Update();


/*

  typedef otb::MinimumNBandsImageFilter< FloatVectorImageType, FloatVectorImageType > MinCostVolume;  
  MinCostVolume::Pointer m_minCost = MinCostVolume::New();
  m_minCost->SetInput(m_LeftCost->GetOutput());


  ImageWriterType::Pointer writer_m_minCost = ImageWriterType::New();
  writer_m_minCost->SetFileName( FILENAME("MinLeftCost.tif"));
  writer_m_minCost->SetInput(m_minCost->GetOutput());
  writer_m_minCost->Update();
*/

  



  //WEIGHTS  
  typedef otb::WeightsGuidedFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType > Weights_ak_bk;
    // --- LEFT
  Weights_ak_bk::Pointer m_meanLeftCost = Weights_ak_bk::New(); 
  m_meanLeftCost->SetInput1(inLeft->GetOutput());
  m_meanLeftCost->SetInput2(m_LeftCost->GetOutput());  
  m_meanLeftCost->SetRadius(0,r);


  /*
   ImageWriterType::Pointer writer_meanLeftCost = ImageWriterType::New();
   writer_meanLeftCost->SetFileName( FILENAME("LeftWeights.tif"));
   writer_meanLeftCost->SetInput(m_meanLeftCost->GetOutput());
   writer_meanLeftCost->Update();
 */

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
  typedef otb::MinimumNBandsImageFilter< FloatVectorImageType, IntImageType > MinCostVolume;  
    // --- LEFT
  MinCostVolume::Pointer m_LeftDisparity = MinCostVolume::New();
  m_LeftDisparity->SetInput(m_meanLeftWeights->GetOutput());
  
  typedef otb::ImageFileWriter< IntImageType > OtbIntImageWriterType; 

  OtbIntImageWriterType::Pointer writer_LeftDisparity = OtbIntImageWriterType::New();
  writer_LeftDisparity->SetFileName( FILENAME("LeftDisparity.tif"));
  writer_LeftDisparity->SetInput(m_LeftDisparity->GetOutput());
  writer_LeftDisparity->Update(); 
  

      // --- RIGHT

  typedef otb::MaximumNBandsImageFilter< FloatVectorImageType, IntImageType > MaxCostVolume;  
  MaxCostVolume::Pointer m_RightDisparity = MaxCostVolume::New();
  m_RightDisparity->SetInput(m_meanRightWeights->GetOutput());


  
  OtbIntImageWriterType::Pointer writer_RightDisparity = OtbIntImageWriterType::New();
  writer_RightDisparity->SetFileName( FILENAME("RightDisparity.tif"));
  writer_RightDisparity->SetInput(m_RightDisparity->GetOutput());
  writer_RightDisparity->Update(); 




  return EXIT_SUCCESS;

  }
