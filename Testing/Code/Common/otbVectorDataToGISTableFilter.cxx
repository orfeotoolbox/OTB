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
#include <iostream>

#include "otbVectorDataToGISTableFilter.h"
#include "otbPostGISConnectionImplementation.h"
#include "otbPostGISTable.h"

int otbVectorDataToGISTableFilter(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  if (argc != 5)
    {
    std::cerr << "Usage: " << argv[0];
    std::cerr << " inputFile(shp) dbName userName userPassword" << std::endl;
    return EXIT_FAILURE;
    }

  const unsigned int dim = 2;
  typedef unsigned char PType;

  //typedef otb::VectorData<> VectorDataType;

  typedef otb::PostGISConnectionImplementation          BdConnection;
  typedef otb::PostGISConnectionImplementation::Pointer BdConnectionPointer;

  BdConnectionPointer myConnection = BdConnection::New();

  //myConnection->PostGISConnectionImplementation();
  //const std::string hostName = argv[1];
  const std::string dbName = argv[2];
  const std::string userName = argv[3];
  const std::string userPassword = argv[4];

  myConnection->SetHost("localhost");
  myConnection->SetDBName(dbName);
  myConnection->SetUser(userName);
  myConnection->SetPassword(userPassword);

  //myConnection->ConnectToDB();

  typedef otb::PostGISTable<BdConnection, double, 2> PostGISTableType;

  //Instantiation
  //std::cout << "YES" << std::endl;
  //PostGISTableType::Pointer data = PostGISTableType::New();
  //std::string myTableName = "VectorDataToPostGIS";
  //data->SetTableName(myTableName);
  //std::cout << "YES" << std::endl;
  //input : the vectordata

  typedef unsigned short int PixelType;

//  Software Guide : BeginLatex
//
//  We define the types for the vector data structure and the
//  corresponding file reader.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
  typedef otb::VectorData<PixelType, dim> VectorDataType;

  typedef otb::VectorDataFileReader<VectorDataType> VectorDataFileReaderType;

// Software Guide : EndCodeSnippet
//  Software Guide : BeginLatex
//
//  We can now instantiate the reader and read the data.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet

  VectorDataFileReaderType::Pointer reader = VectorDataFileReaderType::New();
  reader->SetFileName(argv[1]);
  reader->Update();
  // std::cout << "YES" << std::endl;
  typedef otb::VectorDataToGISTableFilter<VectorDataType, PostGISTableType> VectorDataToGISTableFilterType;
  VectorDataToGISTableFilterType::Pointer myFilter = VectorDataToGISTableFilterType::New();
  myFilter->SetInput(reader->GetOutput());
  //std::cout << "after set input filter" << std::endl;

  myFilter->SetInputGISConnection(myConnection);
  myFilter->SetDropExistingGISTable (true);
  //myFilter->SetGISTableName ("test_filter");
  myFilter->Update();

  return EXIT_SUCCESS;
}
