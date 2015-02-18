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

#include "otbMacro.h"

#include "otbVectorData.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorDataFileReader.h"
#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"

int otbVectorDataFileGeoReaderWriter(int itkNotUsed(argc), char * argv[])
{

  typedef otb::VectorData<double, 2>                VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType> VectorDataWriterType;

  //Instantiation
  VectorDataReaderType::Pointer vectorDataReader = VectorDataReaderType::New();
  VectorDataWriterType::Pointer vectorDataWriter = VectorDataWriterType::New();
  VectorDataType::Pointer       data = VectorDataType::New();

  vectorDataReader->SetFileName(argv[1]);

//      vectorData->SetOrigin(m_Reader->GetOutput()->GetOrigin());
//       vectorData->SetSpacing(m_Reader->GetOutput()->GetSpacing());

  std::string projectionRef;
  itk::ExposeMetaData<std::string>(
    vectorDataReader->GetOutput()->GetMetaDataDictionary(), otb::MetaDataKey::ProjectionRefKey, projectionRef);
//    vectorData->SetProjectionRef(projectionRef);
  std::cout << projectionRef << std::endl;

  vectorDataWriter->SetFileName(argv[2]);
  vectorDataWriter->SetInput(vectorDataReader->GetOutput());
  vectorDataWriter->Update();

  return EXIT_SUCCESS;
}
