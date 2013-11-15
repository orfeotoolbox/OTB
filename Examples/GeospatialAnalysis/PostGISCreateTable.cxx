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

// Software Guide : BeginLatex
//
// This example uses a transactor to create a table. The transactor
// needs to have a copy constructor, so the initialization is done
// correctly for the class variables which are set before the transaction.
// Let's look at the minimal code required to use this algorithm. First, the
// following header defining the \doxygen{otb}{PostGISCreateTableTransactor} class
// must be included.
// Software Guide : EndLatex

#include "otbMacro.h"

// Software Guide : BeginCodeSnippet
#include "otbPostGISCreateTableTransactor.h"
#include "otbPostGISConnectionImplementation.h"
// Software Guide : EndCodeSnippet

int main(int argc, char * argv[])
{

  // Software Guide : BeginLatex
  //
  // We use a PostGIS (PQXX-based) transactor
  // for creating PostGIS tables.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::PostGISCreateTableTransactor TransactorType;

  TransactorType myTransactor;
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // We set arguments of the transactor.

  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  unsigned short dimension = 2;
  myTransactor.SetDimension(dimension);

  std::string name = "mytable";
  myTransactor.SetTableName(name);

  int srid = -1;
  myTransactor.SetSRID(srid);

  myTransactor.SetRemoveExistingTable(true);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // After defining the transcator, we need to create the
  // connection interface to the PostgreSQL database.
  // This is done by the \doxygen{otb}{PostGISConnectionImplementation}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const std::string hostName = argv[1];
  const std::string dbName = argv[2];
  const std::string userName = argv[3];
  const std::string userPassword = argv[4];

  typedef otb::PostGISConnectionImplementation GISConnectionType;

  GISConnectionType::Pointer connection = GISConnectionType::New();

  connection->SetHost(hostName);
  connection->SetDBName(dbName);
  connection->SetUser(userName);
  connection->SetPassword(userPassword);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Let's now connect to the database and perform the transaction
  // with the method \code{PerformTransaction}.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  connection->ConnectToDB();

  connection->PerformTransaction(myTransactor);
  // Software Guide : EndCodeSnippet
  return EXIT_SUCCESS;
}
