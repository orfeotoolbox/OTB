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


namespace otb
{

PostGISCreateTableTransactor::PostGISCreateTableTransactor() : PostGISCreateTableTransactor::Superclass("CreateTable") {};

PostGISCreateTableTransactor::PostGISCreateTableTransactor(const PostGISCreateTableTransactor& pgt)
  : PostGISCreateTableTransactor::Superclass("CreateTable")
{
  m_TableName = pgt.GetTableName();
  m_SRID = pgt.GetSRID();
  m_Dimension = pgt.GetDimension();
}

PostGISCreateTableTransactor& PostGISCreateTableTransactor::operator=(const PostGISCreateTableTransactor& pgt) throw() {
    m_TableName = pgt.GetTableName();
    m_SRID = pgt.GetSRID();
    m_Dimension = pgt.GetDimension();
  return *this;
} 
  
void PostGISCreateTableTransactor::operator()(pqxx::nontransaction &T)
{
  
  std::stringstream createCommand;
  
  createCommand << "CREATE TABLE "<< m_TableName
		<<" (id serial PRIMARY KEY,genre text);";
  
  otbGenericMsgDebugMacro(<<"Create Command " << createCommand.str());
  m_Result = T.exec(createCommand.str());
  
  std::stringstream addGeometryCommand;
  
  addGeometryCommand << "SELECT AddGeometryColumn( '"<< m_TableName <<
    "', 'geom', "<< m_SRID <<", 'GEOMETRY',"<< m_Dimension <<" );";
  
  m_Result = T.exec(addGeometryCommand.str());
  
  
}

void PostGISCreateTableTransactor::on_commit()
{
  std::cout << "\t Table is created \t"  << std::endl;
  
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

PostGISCreateTableTransactor::ResultType PostGISCreateTableTransactor::GetResult() const
{
  return m_Result;
}


} // end namespace otb


