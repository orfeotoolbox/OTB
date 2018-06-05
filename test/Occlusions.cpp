

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
#include "otbCostVolumeImageFilter.h"
#include "otbMinimumNBandsImageFilter.h"
#include "otbWeightsGuidedFilter.h"
#include "otbMeanVectorImageFilter.h"
#include "otbConcatenateVectorImageFilter.h"
#include "otbWeightedMedianImageFilter.h"
#include "otbBijectionCoherencyFilter.h"
#include "otbFillOcclusionDisparityImageFilter.h"






	
int testOcclusionsFilter(int argc, char *argv[])
  {
  if(argc < 3) {
  	std::cerr << "Usage: " << argv[0] << " LeftDisparityMap InvRigthDisparityMap outputPathFolder" << std::endl;
  	return EXIT_FAILURE;
  	}

  std::cout << "OCCLUSIONS FILTER TEST"  << std::endl ; 


  const unsigned int Dimension = 2;
  typedef otb::VectorImage<float> FloatVectorImageType;
  typedef otb::VectorImage<int> IntVectorImageType;
  typedef otb::Image< double, Dimension > FloatImageType;
  typedef otb::Image< int, Dimension > IntImageType;

  typedef otb::ImageFileReader<FloatVectorImageType> ReaderType;
  typedef otb::ImageFileWriter<FloatVectorImageType> ImageWriterType;
  typedef otb::ImageFileReader<IntImageType> IntReaderType;
  typedef otb::ImageFileWriter<IntImageType> IntWriterType;



  IntReaderType::Pointer m_LeftDisparity = IntReaderType::New();
  m_LeftDisparity->SetFileName(argv[1]); //LeftDisparity 
  m_LeftDisparity->UpdateOutputInformation();

  IntReaderType::Pointer m_RightDisparityInv = IntReaderType::New();
  m_RightDisparityInv->SetFileName(argv[2]); //RightDisparity
  m_RightDisparityInv->UpdateOutputInformation();


  std::string argv3 = std::string(argv[3]);
  #define FILENAME(n) std::string( argv3 + std::string(n)).c_str()

  unsigned int dispMax = 15;
  unsigned int dispMin = 0; 



  // OCCLUSION DETECTION
  typedef otb::BijectionCoherencyFilter< IntImageType, IntImageType > OcclusionType;
  OcclusionType::Pointer m_OcclusionFilter = OcclusionType::New();  
  m_OcclusionFilter->SetDirectHorizontalDisparityMapInput(m_LeftDisparity->GetOutput()); 
  m_OcclusionFilter->SetReverseHorizontalDisparityMapInput(m_RightDisparityInv->GetOutput()); 
   
  m_OcclusionFilter->SetMaxHDisp(dispMax);
  m_OcclusionFilter->SetMinHDisp(dispMin);
  m_OcclusionFilter->SetMinVDisp(0);
  m_OcclusionFilter->SetMaxVDisp(0);
  m_OcclusionFilter->SetTolerance(1);
  

 IntWriterType::Pointer OcclusionWriter = IntWriterType::New(); 
 OcclusionWriter->SetFileName( FILENAME("Occlusions.tif"));
 OcclusionWriter->SetInput( m_OcclusionFilter->GetOutput() );  
 OcclusionWriter->Update(); 

 typedef otb::FillOcclusionDisparityImageFilter<IntImageType, IntImageType, IntImageType> FillOcclusionFilter ;
 FillOcclusionFilter::Pointer m_FillOccDisparityMap = FillOcclusionFilter::New();
 m_FillOccDisparityMap->SetInput1(m_OcclusionFilter->GetOutput() );
 m_FillOccDisparityMap->SetInput2(m_LeftDisparity->GetOutput() );
 m_FillOccDisparityMap->SetRadius(0,1);


 IntWriterType::Pointer writer_FillOcclusions = IntWriterType::New(); 
 writer_FillOcclusions->SetFileName( FILENAME("FillOcclusions.tif"));
 writer_FillOcclusions->SetInput( m_FillOccDisparityMap->GetOutput() );  
 writer_FillOcclusions->Update(); 


 




  return EXIT_SUCCESS;

}
