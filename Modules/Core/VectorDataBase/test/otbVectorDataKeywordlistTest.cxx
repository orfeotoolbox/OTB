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


#include <fstream>

#include "otbVectorDataKeywordlist.h"
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbMetaDataKey.h"


int otbVectorDataKeywordlist(int itkNotUsed(argc), char* argv[])
{

  typedef otb::VectorData<>                         VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  VectorDataFileReaderType::Pointer                 reader = VectorDataFileReaderType::New();
  itk::Indent indent;

  reader->SetFileName(argv[1]);
  reader->Update();

  VectorDataType::Pointer data     = reader->GetOutput();

  std::ofstream fout(argv[2]);

  auto itPair = data->GetIteratorPair();
  auto it = itPair.first;

  while (it != itPair.second)
  {
    if (data->Get(it)->GetMetaDataDictionary().HasKey(otb::MetaDataKey::VectorDataKeywordlistKey))
    {
      otb::VectorDataKeywordlist kwl;
      kwl.GetNameOfClass();
      itk::ExposeMetaData<otb::VectorDataKeywordlist>(data->Get(it)->GetMetaDataDictionary(), otb::MetaDataKey::VectorDataKeywordlistKey, kwl);
      fout << "New node: " << kwl.GetNumberOfFields() << " fields" << std::endl;
      fout << "- HasField(\"name\"): " << kwl.HasField("name") << std::endl;
      if (kwl.HasField("name"))
      {
        fout << "- name: " << kwl.GetFieldAsString("name") << std::endl;
      }
      fout << std::endl;
    }

    it++;
  }
  /*added PrintSelf*/

  fout.close();
  return EXIT_SUCCESS;
}
