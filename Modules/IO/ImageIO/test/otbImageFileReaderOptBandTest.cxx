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

#include "otbImage.h"
#include "itkMacro.h"
#include <iostream>
#include <fstream>

#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbExtendedFilenameToReaderOptions.h"

int otbImageFileReaderOptBandTest(int itkNotUsed(argc), char* argv[])
{
  typedef otb::ExtendedFilenameToReaderOptions FilenameHelperType;
  FilenameHelperType::Pointer helper = FilenameHelperType::New();

  // Verify the number of parameters in the command line
  const char * inputFilename  = argv[1];
  const char * outputFilename = argv[2];



  helper->SetExtendedFileName(inputFilename);
  unsigned int nbBands = atoi(argv[3]);
  std::vector<unsigned int> bandList;
  bool ret = helper->ResolveBandRange(helper->GetBandRange(),nbBands,bandList);
  if (ret)
    {
    std::cout << "BandList = [";
    for (unsigned int k=0 ; k<bandList.size() ; k++)
      {
      if (k) std::cout << ",";
      std::cout << bandList[k];
      }
    std::cout << "]" << std::endl;
    }
  else
    {
    std::cout << "Invalid band range for a "<<nbBands<<" bands image"<< std::endl;
    }
  
  
  typedef unsigned int PixelType;
  const unsigned int Dimension = 2;
  
  typedef otb::VectorImage<PixelType, Dimension> ImageType;
  
  typedef otb::ImageFileReader<ImageType> ReaderType; 
  typedef otb::ImageFileWriter<ImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(inputFilename);
  writer->SetFileName(outputFilename);
 
  writer->SetInput(reader->GetOutput());
  writer->Update();

  return EXIT_SUCCESS;
}
