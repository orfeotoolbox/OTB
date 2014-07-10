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

#include <fstream>

#include "otbVectorDataKeywordlist.h"
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbMetaDataKey.h"

int otbVectorDataKeywordlistNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  otb::VectorDataKeywordlist kwl;
  std::cout << kwl << std::endl;
  return EXIT_SUCCESS;
}

int otbVectorDataKeywordlist(int itkNotUsed(argc), char * argv[])
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
    if (it.Get()->GetMetaDataDictionary().HasKey(otb::MetaDataKey::VectorDataKeywordlistKey))
      {
      otb::VectorDataKeywordlist kwl;
      kwl.GetNameOfClass();
      itk::ExposeMetaData<otb::VectorDataKeywordlist>(it.Get()->GetMetaDataDictionary(),
                                                      otb::MetaDataKey::VectorDataKeywordlistKey,
                                                      kwl);
      fout << "New node: " << kwl.GetNumberOfFields() << " fields" << std::endl;
      fout << "- HasField(\"name\"): " << kwl.HasField("name") << std::endl;
      if (kwl.HasField("name"))
        {
        fout << "- name: " << kwl.GetFieldAsString("name") << std::endl;
        }
      fout << std::endl;
      }

    ++it;
    }
  /*added PrintSelf*/

  fout.close();
  return EXIT_SUCCESS;

}
