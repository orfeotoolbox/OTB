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

#include "otbBijectionCoherencyFilter.h"

  
int testOcclusionsDetectionFilter(int argc, char *argv[])
  {

  if(argc < 7) 
    {
    std::cerr << "Usage: " << argv[0] << " LeftDisparityMap RightDisparityMap dmin dmax tolerance sense outputPathFolder" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int Dimension = 2;
  typedef otb::VectorImage<float> FloatVectorImageType;
  typedef otb::Image< int, Dimension > IntImageType;
  typedef otb::ImageFileReader<IntImageType> IntImageReaderType; 
  typedef otb::ImageFileWriter< IntImageType > IntImageWriterType;  

  IntImageReaderType::Pointer m_LeftDisparity = IntImageReaderType::New();
  m_LeftDisparity->SetFileName(argv[1]); //LeftImage  
  m_LeftDisparity->UpdateOutputInformation();

  IntImageReaderType::Pointer m_RightDisparity = IntImageReaderType::New();
  m_RightDisparity->SetFileName(argv[2]);//RightImage
  m_RightDisparity->UpdateOutputInformation();

  int dispMin = atoi(argv[3]);
  int dispMax  = atoi(argv[4]);

  unsigned int tol = atoi(argv[5]);

  bool sense = atoi(argv[6]);

  std::string argv7 = std::string(argv[7]);
  #define FILENAME(n) std::string( argv7 + std::string(n)).c_str()

  typedef otb::BijectionCoherencyFilter< IntImageType, IntImageType > OcclusionType;
  OcclusionType::Pointer m_OcclusionFilter = OcclusionType::New();  

  if(sense==0)
    {
    m_OcclusionFilter->SetDirectHorizontalDisparityMapInput(m_LeftDisparity->GetOutput()); 
    m_OcclusionFilter->SetReverseHorizontalDisparityMapInput(m_RightDisparity->GetOutput());
    }
  else
    {
    m_OcclusionFilter->SetDirectHorizontalDisparityMapInput(m_RightDisparity->GetOutput()); 
    m_OcclusionFilter->SetReverseHorizontalDisparityMapInput(m_LeftDisparity->GetOutput());
    }
  
  m_OcclusionFilter->SetMaxHDisp(dispMax);
  m_OcclusionFilter->SetMinHDisp(dispMin); 
  m_OcclusionFilter->SetMinVDisp(0);
  m_OcclusionFilter->SetMaxVDisp(0);
  m_OcclusionFilter->SetTolerance(tol);

  IntImageWriterType::Pointer writer_OcclusionMask = IntImageWriterType::New();
  writer_OcclusionMask->SetFileName( FILENAME("OcclusionsMask.tif"));
  writer_OcclusionMask->SetInput(m_OcclusionFilter->GetOutput());
  writer_OcclusionMask->Update();

  return EXIT_SUCCESS;

  }
