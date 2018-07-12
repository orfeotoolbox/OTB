
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
#include "otbCostVolumeImageFilter.h"
#include "otbMinimumNBandsImageFilter.h"
#include "otbWeightsGuidedFilter.h"
#include "otbMeanVectorImageFilter.h"
#include "otbConcatenateVectorImageFilter.h"
#include "otbWeightedMedianImageFilter.h"
#include "otbBijectionCoherencyFilter.h"

#include "otbImageToVectorImageCastFilter.h"

#include "otbConvertValueFrom0To255.h"

#include "otbFillOcclusionPixel.h"


  
int testOcclusionsFilter(int argc, char *argv[])
  {
  if(argc < 3) {
    std::cerr << "Usage: " << argv[0] << " LeftDisparityMap RigthDisparityMap inMedianLeftImage minDisp maxDisp rFillOcc rwmf outputPathFolder" << std::endl;
    return EXIT_FAILURE;
    }

  std::cout << "OCCLUSIONS FILTER TEST"  << std::endl ; 


  const unsigned int Dimension = 2;
  typedef otb::VectorImage<float> FloatVectorImageType;
  // typedef otb::VectorImage<int> IntVectorImageType;
  // typedef otb::Image< double, Dimension > FloatImageType;
  typedef otb::Image< int, Dimension > IntImageType;

  typedef otb::ImageFileReader<FloatVectorImageType> FloatVectorImageReaderType;
  // typedef otb::ImageFileWriter<FloatVectorImageType> FloatVectorImageWriterType;


  // typedef otb::ImageFileReader<IntVectorImageType> IntVectorImageReaderType;
  // typedef otb::ImageFileWriter<IntVectorImageType> IntVectorImageWriterType;


  typedef otb::ImageFileReader<IntImageType> IntReaderType;
  typedef otb::ImageFileWriter<IntImageType> IntWriterType;

  //   typedef otb::ImageFileReader<FloatImageType> FloatReaderType;
  // typedef otb::ImageFileWriter<FloatImageType> FloatWriterType;



  IntReaderType::Pointer m_LeftDisparity = IntReaderType::New();
  m_LeftDisparity->SetFileName(argv[1]); //LeftDisparity 
  m_LeftDisparity->UpdateOutputInformation();

  IntReaderType::Pointer m_RightDisparity = IntReaderType::New();
  m_RightDisparity->SetFileName(argv[2]); //RightDisparity
  m_RightDisparity->UpdateOutputInformation();



  FloatVectorImageReaderType::Pointer m_inLeftMedian = FloatVectorImageReaderType::New();
  m_inLeftMedian->SetFileName(argv[3]); 
  m_inLeftMedian->UpdateOutputInformation();
  int dispMin = atoi(argv[4]);
  int dispMax = atoi(argv[5]);

  int rwmf = atoi(argv[6]) ;



  std::string argv7 = std::string(argv[7]);
  #define FILENAME(n) std::string( argv7 + std::string(n)).c_str()




  // OCCLUSION DETECTION
  typedef otb::BijectionCoherencyFilter< IntImageType, IntImageType > OcclusionType;
  OcclusionType::Pointer m_OcclusionFilter = OcclusionType::New();  
  m_OcclusionFilter->SetDirectHorizontalDisparityMapInput(m_LeftDisparity->GetOutput()); 
  m_OcclusionFilter->SetReverseHorizontalDisparityMapInput(m_RightDisparity->GetOutput()); 
   
  m_OcclusionFilter->SetMaxHDisp(dispMax);
  m_OcclusionFilter->SetMinHDisp(dispMin);
  m_OcclusionFilter->SetMinVDisp(0);
  m_OcclusionFilter->SetMaxVDisp(0);
  m_OcclusionFilter->SetTolerance(2);
  
   IntWriterType::Pointer OcclusionWriter = IntWriterType::New(); 
   OcclusionWriter->SetFileName( FILENAME("Occlusions.tif"));
   OcclusionWriter->SetInput( m_OcclusionFilter->GetOutput() );  
   OcclusionWriter->Update(); 



     typedef otb::ImageToVectorImageCastFilter<IntImageType,FloatVectorImageType> CastImageFilter;
  CastImageFilter::Pointer m_CastOccMap = CastImageFilter::New();
  m_CastOccMap-> SetInput( const_cast <IntImageType *>( m_OcclusionFilter->GetOutput() ));


  typedef otb::ImageToVectorImageCastFilter<IntImageType,FloatVectorImageType> CastImageFilter;
  CastImageFilter::Pointer m_CastLeftDisparity= CastImageFilter::New();
  m_CastLeftDisparity-> SetInput( const_cast <IntImageType *>( m_LeftDisparity->GetOutput() ));



  typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType> ConcatenateVectorImageFilterType;  
  ConcatenateVectorImageFilterType::Pointer m_ConcatenateLeftMedianIAndOccMap = ConcatenateVectorImageFilterType::New();
  m_ConcatenateLeftMedianIAndOccMap->SetInput1(m_inLeftMedian->GetOutput());
  m_ConcatenateLeftMedianIAndOccMap->SetInput2(m_CastOccMap->GetOutput());

  // FloatVectorImageWriterType::Pointer writer_ConcatenateLeftMedianIAndOccMap = FloatVectorImageWriterType::New();
  // writer_ConcatenateLeftMedianIAndOccMap->SetFileName( FILENAME("ConcatenateLeftMedianIAndOccMap.tif"));
  // writer_ConcatenateLeftMedianIAndOccMap->SetInput( m_ConcatenateLeftMedianIAndOccMap->GetOutput() ); 
  // writer_ConcatenateLeftMedianIAndOccMap->Update();


  typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType> ConcatenateVectorImageFilterType;  
  ConcatenateVectorImageFilterType::Pointer m_ConcatenateCastOccMapAndLeftImage = ConcatenateVectorImageFilterType::New();
  m_ConcatenateCastOccMapAndLeftImage->SetInput1(m_CastLeftDisparity->GetOutput());
  m_ConcatenateCastOccMapAndLeftImage->SetInput2(m_ConcatenateLeftMedianIAndOccMap->GetOutput());


  typedef otb::FillPixelFilter< FloatVectorImageType, IntImageType > FillOccFilter ;
  FillOccFilter::Pointer m_FillOcc = FillOccFilter::New();

    m_FillOcc->SetInput(m_ConcatenateCastOccMapAndLeftImage->GetOutput());

  FloatVectorImageType::SizeType radiusM;
  radiusM[0] = rwmf;
  radiusM[1] = rwmf;   
  m_FillOcc->SetRadius(radiusM);


  IntWriterType::Pointer writer_FillOccFilter = IntWriterType::New();
  writer_FillOccFilter->SetFileName( FILENAME("SmoothFillDisparity.tif"));
 writer_FillOccFilter->SetInput( m_FillOcc->GetOutput() );  
 writer_FillOccFilter->Update(); 

   CastImageFilter::Pointer m_CastFillOccFilter= CastImageFilter::New();
  m_CastFillOccFilter-> SetInput( m_FillOcc->GetOutput());

 typedef otb::ConvertValueFrom0To255<FloatVectorImageType, IntImageType > ConvertValue ;
 ConvertValue::Pointer m_convertSmoothDisparity = ConvertValue::New();
 m_convertSmoothDisparity->SetInput(m_CastFillOccFilter->GetOutput());
 m_convertSmoothDisparity->SetDispMin(dispMin);
 m_convertSmoothDisparity->SetDispMax(dispMax);

   IntWriterType::Pointer writer_convertSmoothFillDisparity = IntWriterType::New();
  writer_convertSmoothFillDisparity->SetFileName( FILENAME("ConvertSmoothFillDisparity.tif"));
 writer_convertSmoothFillDisparity->SetInput( m_convertSmoothDisparity->GetOutput() ); 
 writer_convertSmoothFillDisparity->Update();



  //  typedef otb::FillOcclusionDisparityImageFilter<IntImageType, IntImageType, IntImageType> FillOcclusionFilter ;
  //  FillOcclusionFilter::Pointer m_FillOccDisparityMap = FillOcclusionFilter::New();
  //  m_FillOccDisparityMap->SetInput1(m_OcclusionFilter->GetOutput() );
  //  m_FillOccDisparityMap->SetInput2(m_LeftDisparity->GetOutput() );
  // // m_FillOccDisparityMap->SetRadius(0,rFillOcc);


  //  // IntWriterType::Pointer writer_FillOcclusions = IntWriterType::New(); 
  //  // writer_FillOcclusions->SetFileName( FILENAME("FillOcclusions.tif"));
  //  // writer_FillOcclusions->SetInput( m_FillOccDisparityMap->GetOutput() );  
  //  // writer_FillOcclusions->Update(); 



  // CastImageFilter::Pointer m_CastOccMap = CastImageFilter::New();
  // m_CastOccMap-> SetInput( const_cast <IntImageType *>( m_FillOccDisparityMap->GetOutput() ));


 //  typedef otb::ConcatenateVectorImageFilter< FloatVectorImageType, FloatVectorImageType, FloatVectorImageType> ConcatenateVectorImageFilterType;  
 //  ConcatenateVectorImageFilterType::Pointer m_ConcatenateCastOccMapAndLeftImage = ConcatenateVectorImageFilterType::New();
 //  m_ConcatenateCastOccMapAndLeftImage->SetInput1(m_CastOccMap->GetOutput());
 //  m_ConcatenateCastOccMapAndLeftImage->SetInput2(m_inLeftMedian->GetOutput());


 //  typedef  otb::WeightMedianImageFilter< FloatVectorImageType, FloatVectorImageType > WeightMedianFilter;
 //  WeightMedianFilter::Pointer m_WeightOccMapAndLeftImageFilter = WeightMedianFilter::New();
 //  m_WeightOccMapAndLeftImageFilter->SetInput(m_ConcatenateCastOccMapAndLeftImage->GetOutput());

 //  FloatVectorImageType::SizeType radiusM;
 //  radiusM[0] = rwmf;
 //  radiusM[1] = rwmf;   
 //  m_WeightOccMapAndLeftImageFilter->SetRadius(radiusM);



 //  FloatVectorImageWriterType::Pointer writer_smoothFillDisparity = FloatVectorImageWriterType::New();
 //  writer_smoothFillDisparity->SetFileName( FILENAME("SmoothFillDisparity.tif"));
 // writer_smoothFillDisparity->SetInput( m_WeightOccMapAndLeftImageFilter->GetOutput() );  
 // writer_smoothFillDisparity->Update(); 





  return EXIT_SUCCESS;

}
