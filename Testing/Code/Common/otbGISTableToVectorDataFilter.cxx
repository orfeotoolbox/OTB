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

#include "otbVectorDataFileWriter.h"
#include "otbVectorData.h"
#include <fstream>
#include <iostream>

#include "otbPostGISTable.h"
#include "otbGISTableToVectorDataFilter.h"
#include "otbPostGISConnectionImplementation.h"

int otbGISTableToVectorDataFilter(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  /** Read a PostGIS table and write it in a VectorData. The Filter is based on the OGR reader/writer factory*/
  if (argc != 6)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " outputFile(shp) dbName tableName userName userPassword" << std::endl;
    return EXIT_FAILURE;
    }

  const int dim = 2;

  const std::string dbName = argv[2];
  const std::string tableName = argv[3];
  const std::string userName = argv[4];
  const std::string userPassword = argv[5];

  typedef unsigned char                                     PType;
  typedef otb::VectorData<double, dim>                      VectorDataType;
  typedef otb::PostGISConnectionImplementation              GISConnectionType;
  typedef GISConnectionType::Pointer                        GISConnectionPointerType;
  typedef otb::PostGISTable<GISConnectionType, double, dim> GISTableType;

  //Instantiation
  GISTableType::Pointer data = GISTableType::New();

  GISConnectionPointerType myConnection = GISConnectionType::New();

  myConnection->SetHost("localhost");
  myConnection->SetDBName(dbName);
  myConnection->SetUser(userName);
  myConnection->SetPassword(userPassword);

  data->SetConnection(myConnection);
  data->SetTableName(tableName);

  typedef otb::GISTableToVectorDataFilter<GISTableType, VectorDataType> GISTableToVectorDataFilter;

  GISTableToVectorDataFilter::Pointer MyFilter = GISTableToVectorDataFilter::New();
  std::cout << MyFilter << std::endl;
  MyFilter->SetInput(data);
  MyFilter->Update();
  typedef otb::VectorDataFileWriter<VectorDataType> WriterType;

  WriterType::Pointer writer = WriterType::New();

  writer->SetFileName(argv[1]);
  writer->SetInput(MyFilter->GetOutput());
  writer->Update();
  //data->itkGetObjectMacro ( )

  return EXIT_SUCCESS;
}
