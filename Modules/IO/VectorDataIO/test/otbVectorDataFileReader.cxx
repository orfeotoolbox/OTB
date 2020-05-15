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


#include "otbVectorDataFileReader.h"
#include "otbVectorData.h"
#include <fstream>


int otbVectorDataFileReader(int itkNotUsed(argc), char* argv[])
{
  typedef otb::VectorData<>                         VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  VectorDataFileReaderType::Pointer                 reader = VectorDataFileReaderType::New();

  typedef otb::DataNode<double, 2, double> DataNodeType;
  typedef DataNodeType::Pointer                   DataNodePointerType;
  typedef itk::TreeContainer<DataNodePointerType> DataTreeType;

  itk::Indent indent;

  reader->SetFileName(argv[1]);
  reader->Update();

  VectorDataType::Pointer data     = reader->GetOutput();
  DataTreeType::Pointer   dataTree = DataTreeType::New();
  dataTree                         = data->GetDataTree();

  std::ofstream fout(argv[2]);

  itk::PreOrderTreeIterator<DataTreeType> it(dataTree);
  it.GoToBegin();

  while (!it.IsAtEnd())
  {
    itk::PreOrderTreeIterator<DataTreeType> itParent = it;
    bool                                    goesOn   = true;
    while (itParent.HasParent() && goesOn)
    {
      fout << indent;
      goesOn = itParent.GoToParent();
    }
    fout << "+" << it.Get()->GetNodeTypeAsString() << std::endl;
    ++it;
  }
  /*added PrintSelf*/

  fout.close();
  return EXIT_SUCCESS;
}
