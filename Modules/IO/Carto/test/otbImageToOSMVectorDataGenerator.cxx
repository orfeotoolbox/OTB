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

#include "otbImageToOSMVectorDataGenerator.h"

#include "otbVectorDataFileWriter.h"
#include "otbVectorImage.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"

typedef otb::VectorImage<unsigned int, 2> ImageType;

typedef otb::ImageToOSMVectorDataGenerator<ImageType> FilterType;
typedef FilterType::VectorDataType                    VectorDataType;

typedef otb::ImageFileReader<ImageType>           ReaderType;
typedef otb::VectorDataFileWriter<VectorDataType> VectorDataFileWriterType;

int otbImageToOSMVectorDataGenerator(int argc, char* argv[])
{
  if (argc != 5)
  {
    std::cerr << "Usage: otbImageToOSMVectorDataGenerator input osm_xml_file "
                 "output key_name\n";
    return EXIT_FAILURE;
  }

  // convenient typedefs to store keys and their value
  typedef std::pair<std::string, std::string> KeyValueType;
  typedef std::vector<KeyValueType> KeyValueListType;

  // Instantiate the image reader
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  reader->UpdateOutputInformation();

  // VectorData generator instantiation
  FilterType::Pointer vdgenerator = FilterType::New();
  vdgenerator->SetInput(reader->GetOutput());
  vdgenerator->SetUseUrl(false);
  vdgenerator->SetFileName(argv[2]);
  vdgenerator->Update();

  // Split the classes to get classes and values
  KeyValueListType keyvalueList;
  for (unsigned int idClass = 0; idClass < 1; idClass++)
  {
    std::string  key;
    KeyValueType currentkeyvalue;
    std::string  str = argv[4];

    // find the position of the separator ,
    size_t pos = str.find(",");

    // split the string
    currentkeyvalue.first = str.substr(0, pos);
    if (pos != std::string::npos)
      currentkeyvalue.second = str.substr(pos + 1);

    keyvalueList.push_back(currentkeyvalue);
  }

  std::cout << "Searching for class " << keyvalueList[0].first << " and subclass " << keyvalueList[0].second << std::endl;


  // Write the generated vector data
  VectorDataFileWriterType::Pointer writer = VectorDataFileWriterType::New();
  writer->SetFileName(argv[3]);

  const VectorDataType* vd = vdgenerator->GetVectorDataByName(keyvalueList[0].first, keyvalueList[0].second);
  writer->SetInput(vd);

  // trigger the execution
  writer->Update();

  return EXIT_SUCCESS;
}
