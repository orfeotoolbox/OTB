

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
#include "otbVectorImageToImageListFilter.h"
#include "otbBijectionCoherencyFilter.h"
#include "otbStandardFilterWatcher.h"




	
int testOcclusionsFilter(int argc, char *argv[])
  {
  if(argc < 3) {
  	std::cerr << "Usage: " << argv[0] << " LeftDisparityMap RigthDisparityMap outputPathFolder" << std::endl;
  	return EXIT_FAILURE;
  	}

  std::cout << "OCCLUSIONS FILTER TEST"  << std::endl ; 


  const unsigned int Dimension = 2;
  typedef otb::VectorImage<float> FloatVectorImageType;
  typedef otb::Image< double, Dimension > FloatImageType;
  typedef otb::Image< int, Dimension > IntImageType;

  typedef otb::ImageFileReader<FloatVectorImageType> ReaderType;
  typedef otb::ImageFileWriter<FloatVectorImageType> ImageWriterType;
  typedef otb::ImageFileReader<IntImageType> IntReaderType;
  typedef otb::ImageFileWriter<IntImageType> IntWriterType;



  IntReaderType::Pointer m_LeftDisparity = IntReaderType::New();
  m_LeftDisparity->SetFileName(argv[1]); //LeftDisparity 
  m_LeftDisparity->UpdateOutputInformation();

  IntReaderType::Pointer m_RightDisparity = IntReaderType::New();
  m_RightDisparity->SetFileName(argv[2]); //RightDisparity
  m_RightDisparity->UpdateOutputInformation();


  std::string argv3 = std::string(argv[3]);
  #define FILENAME(n) std::string( argv3 + std::string(n)).c_str()












  return EXIT_SUCCESS;

}
