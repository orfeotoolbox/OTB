/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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


#include "otbVectorDataFileReader.h"
#include "otbVectorData.h"
#include <fstream>


int otbVectorDataFileReader(int itkNotUsed(argc), char* argv[])
{
  typedef otb::VectorData<>                         VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  VectorDataFileReaderType::Pointer                 reader = VectorDataFileReaderType::New();

  itk::Indent indent;

  reader->SetFileName(argv[1]);
  reader->Update();

  VectorDataType::Pointer data     = reader->GetOutput();

  std::ofstream fout(argv[2]);

  data->Print(fout,indent);
  /*added PrintSelf*/

  fout.close();
  return EXIT_SUCCESS;
}
