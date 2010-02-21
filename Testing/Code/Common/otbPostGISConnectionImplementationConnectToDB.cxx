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
#include "itkExceptionObject.h"
#include "otbMacro.h"

#include "otbPostGISConnectionImplementation.h"

int otbPostGISConnectionImplementationConnectToDB(int argc, char * argv[])
{

  const std::string hostName = argv[1];
  const std::string dbName = argv[2];
  const std::string userName = argv[3];
  const std::string userPassword = argv[4];

  typedef otb::PostGISConnectionImplementation GISConnectionType;

  //Instantiation
  GISConnectionType::Pointer connection = GISConnectionType::New();

  connection->SetHost( hostName );
  connection->SetDBName( dbName );
  connection->SetUser( userName );
  connection->SetPassword( userPassword );

  connection->ConnectToDB();


  return EXIT_SUCCESS;
}
