/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "otbImageIOFactory.h"
#include "itkMacro.h"
#include <iostream>

#include "otbImageFileWriter.h"
#include "otbImageFileReader.h"
#include "otbImage.h"
#include "otbVectorImage.h"


int negativespacing(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef float InputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image< InputPixelType ,  Dimension > OTBInputImageType;

  typedef otb::ImageFileReader< OTBInputImageType > OTBReaderType;
  typedef otb::ImageFileWriter< OTBInputImageType > OTBWriterType;

  OTBReaderType::Pointer otbReader ( OTBReaderType::New() );
  OTBWriterType::Pointer otbWriter ( OTBWriterType::New() );
  otbReader->SetFileName( "/home/antoine/dev/my_data/spacing/input_negat_spacing.tif" );
  otbWriter->SetFileName( "/home/antoine/dev/my_data/spacing/input_negat_spacing_result.tif" );
  otbReader->UpdateOutputInformation();

  otbReader->Update();
  OTBInputImageType::Pointer otbinput = otbReader->GetOutput();

  std::cout<<otbinput->GetSpacing()<<std::endl;
  auto spacing = otbinput->GetSpacing();
  // spacing[0] = -spacing[0];
  otbinput->SetSignedSpacing(spacing);
  std::cout<<otbinput->GetSpacing()<<std::endl;
  std::cout<<otbinput->GetSignedSpacing()<<std::endl;
  otbWriter->SetInput( otbReader->GetOutput() );
  otbWriter->Update();
  std::cout<<"OTBWriter"<<std::endl;

  return EXIT_SUCCESS;
}
