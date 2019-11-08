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


#include "otbMacro.h"

#include "otbVectorData.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorDataFileReader.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"

int otbVectorDataFileGeoReaderWriter(int itkNotUsed(argc), char* argv[])
{

  typedef otb::VectorData<double, 2> VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType> VectorDataWriterType;

  // Instantiation
  VectorDataReaderType::Pointer vectorDataReader = VectorDataReaderType::New();
  VectorDataWriterType::Pointer vectorDataWriter = VectorDataWriterType::New();
  VectorDataType::Pointer       data             = VectorDataType::New();

  vectorDataReader->SetFileName(argv[1]);

  //      vectorData->SetOrigin(m_Reader->GetOutput()->GetOrigin());
  //       vectorData->SetSignedSpacing(m_Reader->GetOutput()->GetSignedSpacing());

  std::string projectionRef;
  itk::ExposeMetaData<std::string>(vectorDataReader->GetOutput()->GetMetaDataDictionary(), otb::MetaDataKey::ProjectionRefKey, projectionRef);
  //    vectorData->SetProjectionRef(projectionRef);
  std::cout << projectionRef << std::endl;

  vectorDataWriter->SetFileName(argv[2]);
  vectorDataWriter->SetInput(vectorDataReader->GetOutput());
  vectorDataWriter->Update();

  return EXIT_SUCCESS;
}
