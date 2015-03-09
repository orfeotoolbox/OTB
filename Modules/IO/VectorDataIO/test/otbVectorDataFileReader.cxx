/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbVectorDataFileReader.h"
#include "otbVectorData.h"
#include <fstream>


int otbVectorDataFileReader(int itkNotUsed(argc), char * argv[])
{
  typedef otb::VectorData<>                         VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;
  VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();

  typedef otb::DataNode<double, 2, double>        DataNodeType;
  typedef DataNodeType::Pointer                   DataNodePointerType;
  typedef itk::TreeContainer<DataNodePointerType> DataTreeType;

  itk::Indent indent;

  reader->SetFileName(argv[1]);
  reader->Update();

  VectorDataType::Pointer data = reader->GetOutput();
  DataTreeType::Pointer   dataTree = DataTreeType::New();
  dataTree = data->GetDataTree();

  std::ofstream fout (argv[2]);

  itk::PreOrderTreeIterator<DataTreeType> it(dataTree);
  it.GoToBegin();

  while (!it.IsAtEnd())
    {
    itk::PreOrderTreeIterator<DataTreeType> itParent = it;
    bool                                    goesOn = true;
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
