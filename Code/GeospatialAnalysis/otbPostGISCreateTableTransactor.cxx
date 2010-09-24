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
#include "otbPostGISCreateTableTransactor.h"
#include <sstream>
#include "otbMacro.h"

#include <assert.h>

namespace otb
{

PostGISCreateTableTransactor::PostGISCreateTableTransactor() : PostGISCreateTableTransactor::Superclass("CreateTable")
{
  m_RemoveExistingTable = false;
}

PostGISCreateTableTransactor::PostGISCreateTableTransactor(const PostGISCreateTableTransactor& pgt)
  : PostGISCreateTableTransactor::Superclass("CreateTable")
{
  m_TableName = pgt.GetTableName();
  m_SRID = pgt.GetSRID();
  m_Dimension = pgt.GetDimension();
  m_RemoveExistingTable = pgt.GetRemoveExistingTable();
}

PostGISCreateTableTransactor & PostGISCreateTableTransactor::operator =(const PostGISCreateTableTransactor& pgt)
throw()
{
  m_TableName = pgt.GetTableName();
  m_SRID = pgt.GetSRID();
  m_Dimension = pgt.GetDimension();
  m_RemoveExistingTable = pgt.GetRemoveExistingTable();
  return *this;
}

void PostGISCreateTableTransactor::operator ()(pqxx::nontransaction& T)
{

  if (m_RemoveExistingTable)
    {
    std::stringstream dropCommand;

    //dropCommand << "DROP TABLE " << m_TableName;
    dropCommand << "DROP TABLE IF EXISTS " << m_TableName << " CASCADE";

    otbGenericMsgDebugMacro(<< "Drop Command " << dropCommand.str());

    m_Result = T.exec(dropCommand.str());
    }

  std::stringstream createCommand;

  createCommand << "CREATE TABLE " << m_TableName
                << " (id serial PRIMARY KEY, genre TEXT);";

  otbGenericMsgDebugMacro(<< "Create Command " << createCommand.str());
  m_Result = T.exec(createCommand.str());

  std::stringstream addGeometryCommand;

  addGeometryCommand << "SELECT AddGeometryColumn( '" << m_TableName <<
  "', 'the_geom', " << m_SRID << ", 'GEOMETRY'," << m_Dimension << " );";

  m_Result = T.exec(addGeometryCommand.str());


  /** creation index GIST */
  //FIXME not working yet
//  std::stringstream addGISTIndexCommand;
//
//  addGISTIndexCommand << "CREATE INDEX idx_" << m_TableName << "_the_geom ON " << m_TableName <<
//  " USING gist( the_geom );";
//
//  otbGenericMsgDebugMacro(<< "Create Command " << addGISTIndexCommand.str());
//
//  m_Result = T.exec(addGISTIndexCommand.str());
}

void PostGISCreateTableTransactor::on_commit()
{
  std::cout << "\t Table is created \t"  << std::endl;
  //T.exec
}

std::string PostGISCreateTableTransactor::GetTableName() const
{
  return m_TableName;
}

void PostGISCreateTableTransactor::SetTableName(const std::string& aName)
{
  m_TableName = aName;
}

int PostGISCreateTableTransactor::GetSRID() const
{
  return m_SRID;
}

void PostGISCreateTableTransactor::SetSRID(int aSRID)
{
  m_SRID = aSRID;
}

unsigned short PostGISCreateTableTransactor::GetDimension() const
{
  return m_Dimension;
}

void PostGISCreateTableTransactor::SetDimension(unsigned short aDim)
{
  m_Dimension = aDim;
}

void PostGISCreateTableTransactor::SetRemoveExistingTable(bool val)
{
  m_RemoveExistingTable = val;
}

bool PostGISCreateTableTransactor::GetRemoveExistingTable() const
{
  return m_RemoveExistingTable;
}

PostGISCreateTableTransactor::ResultType PostGISCreateTableTransactor::GetResult() const
{
  return m_Result;
}

void PostGISCreateTableTransactor::CreateGISTIndex(pqxx::nontransaction& T)
{
  /*
  std::stringstream addGISTIndexCmd;

  addGISTIndexCmd << "CREATE INDEX idx_" << m_TableName << "_the_geom ON " << m_TableName << " USING gist( the_geom );";

  otbGenericMsgDebugMacro(<<"Create Command " << addGISTIndexCmd.str());
  T.exec(addGISTIndexCmd.str());
  */
  assert(0);
}

} // end namespace otb
