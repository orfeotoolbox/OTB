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

class CreateTable : public pqxx::transactor<pqxx::nontransaction>
{
  pqxx::result m_Result;
public:
  CreateTable() : pqxx::transactor<pqxx::nontransaction>("CreateTable") {}

  void operator()(argument_type &T)
  {
    m_Result = T.exec("CREATE TABLE testtable (id serial PRIMARY KEY,genre text);");
    m_Result = T.exec("SELECT AddGeometryColumn( 'testtable', 'geom', -1, 'GEOMETRY', 2 );");

    
  }

  void on_commit()
  {
      std::cout << "\t Table is created \t"  << std::endl;

  }
};

int otbPostGISConnectionImplementationPerformTransaction(int argc, char * argv[])
{

  const std::string hostName = argv[1];
  const std::string dbName = argv[2];
  const std::string userName = argv[3];
  
  typedef otb::PostGISConnectionImplementation GISConnectionType;

  //Instantiation
  GISConnectionType::Pointer connection = GISConnectionType::New();

  connection->SetHost( hostName );
  connection->SetDBName( dbName );
  connection->SetUser( userName );

  connection->ConnectToDB();

  connection->PerformTransaction( CreateTable() );

  return EXIT_SUCCESS;
}
