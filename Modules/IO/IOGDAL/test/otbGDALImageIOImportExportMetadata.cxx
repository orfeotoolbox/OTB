/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#include "itkMacro.h"
#include <iostream>

#include "otbGDALImageIO.h"

//struct sGDALImageIO : public GDALImageIO
//{
//
//};

int otbGDALImageIOImportExportMetadata(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char* inputFilename  = argv[1];
  const char* outputFilename = argv[2];
  const char* outputTextFilename = argv[3];

  std::ofstream outfile(outputTextFilename);
  otb::GDALImageIO::Pointer readerGDAL = otb::GDALImageIO::New();
  readerGDAL->SetFileName(inputFilename);
  if (readerGDAL->CanReadFile(inputFilename))
  	{
  	std::cout << "Read file OK" << std::endl;
  	readerGDAL->ReadImageInformation();
  	outfile << readerGDAL->GetImageMetadata();	
    }
    
  else
  {
    std::cout << "Read file K0" << std::endl;
    return EXIT_FAILURE;
  }

//  otb::sGDALImageIO::Pointer io = otb::sGDALImageIO::New();
//  io->CanReadFile(inputFilename);
//  io->ImportMetadata();
//  outfile << io->m_Imd;
//
//  otb::sGDALImageIO::Pointer io2 = otb::sGDALImageIO::New();
//  io2.CarWriteFile(outputFilename);
//  io2->m_Imd = io->m_Imd;
//  io2.ExportMetadata();

  return EXIT_SUCCESS;
}
