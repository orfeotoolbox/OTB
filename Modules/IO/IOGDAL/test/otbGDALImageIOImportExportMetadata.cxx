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
  const char* outputReaderFilename = argv[3];


  std::ofstream outfileR(outputReaderFilename);
  otb::GDALImageIO::Pointer readerGDAL = otb::GDALImageIO::New();
  readerGDAL->SetFileName(inputFilename);
  if (readerGDAL->CanReadFile(inputFilename))
  	{
  	std::cout << "Read file OK" << std::endl;
  	readerGDAL->ReadImageInformation();
  	outfileR << readerGDAL->GetImageMetadata();	
    }    
  else
  	{
    std::cout << "Read file K0" << std::endl;
    return EXIT_FAILURE;
  	}

  otb::GDALImageIO::Pointer writerGDAL = otb::GDALImageIO::New();
  if(writerGDAL->CanWriteFile(outputFilename))
    {
   	writerGDAL->WriteImageInformation();
    }  	
  else
    {
  	return EXIT_FAILURE ;
    }  	


  return EXIT_SUCCESS;
}
