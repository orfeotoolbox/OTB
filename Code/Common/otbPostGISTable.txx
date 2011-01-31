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
#ifndef __otbPostGISTable_txx
#define __otbPostGISTable_txx

#include "otbPostGISTable.h"
#include "itkPreOrderTreeIterator.h"
#include "otbMetaDataKey.h"

#include <iostream>
#include <sstream>
#include <string>

#include "otbPostGISFromStringTransactor.h"
#include "otbPostGISConnectionImplementation.h"
#include "otbPostGISCreateTableTransactor.h"

namespace otb
{

template <class TConnectionImplementation, class TPrecision, unsigned int TSpatialDimension>
PostGISTable<TConnectionImplementation, TPrecision, TSpatialDimension>
::PostGISTable()
{
  m_Connection = ConnectionType::New();
  this->SetSrid(-1);
}

template <class TConnectionImplementation, class TPrecision, unsigned int TSpatialDimension>
void
PostGISTable<TConnectionImplementation, TPrecision, TSpatialDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << std::endl;

  os << "Table name: " << m_TableName << std::endl;

}

template <class TConnectionImplementation, class TPrecision, unsigned int TSpatialDimension>
bool
PostGISTable<TConnectionImplementation, TPrecision, TSpatialDimension>
::Clear()
{ //TODO implementation
  return true;
}

template <class TConnectionImplementation, class TPrecision, unsigned int TSpatialDimension>
void
PostGISTable<TConnectionImplementation, TPrecision, TSpatialDimension>
::InsertBegin(std::stringstream& sqlCmd)
{
  sqlCmd.str("");
  sqlCmd << "INSERT INTO \"" << this->GetTableName() << "\" ( \"the_geom\" , \"genre\" ) VALUES (";
  sqlCmd << "GeometryFromText('";
}
template <class TConnectionImplementation, class TPrecision, unsigned int TSpatialDimension>
void
PostGISTable<TConnectionImplementation, TPrecision, TSpatialDimension>
::InsertPoint(const PointType& pt, const std::string& attribute)
{
  std::stringstream sqlCmd;
  this->InsertBegin(sqlCmd);
  sqlCmd << "POINT( ";
  for (unsigned int i = 0; i < TSpatialDimension; i++)
    {
    sqlCmd << pt[i] << " ";
    }

  //int this->GetSrid()=-1;
  sqlCmd << ")'," << this->GetSrid() << ") ,'" << attribute << "');" << std::endl;

  //Execute the query
  this->InsertGeometries(sqlCmd.str());
}

template <class TConnectionImplementation, class TPrecision, unsigned int TSpatialDimension>
void
PostGISTable<TConnectionImplementation, TPrecision, TSpatialDimension>
::InsertLineString(LinePointerType l, const std::string& attribute)
{
  std::stringstream sqlCmd;
  this->InsertBegin (sqlCmd);
  sqlCmd << "LINESTRING( ";

  typedef typename LineType::VertexListConstIteratorType VertexIterator;
  VertexIterator itVertex = l->GetVertexList()->Begin();
  while (itVertex != l->GetVertexList()->End())
    {
    for (unsigned int i = 0; i < TSpatialDimension; i++)
      {
      sqlCmd << itVertex.Value()[i] << " ";
      ++itVertex;
      }
    sqlCmd << ",";
    }
  //Erase the last ','
  EraseLastChar (sqlCmd);

  //int this->GetSrid()=-1;

  sqlCmd << ")'," << this->GetSrid() << ") ,'" << attribute << "');" << std::endl;
  //Execute the query
  this->InsertGeometries(sqlCmd.str());

  /* Invalid (MULTI)Linestring */

}

template <class TConnectionImplementation, class TPrecision, unsigned int TSpatialDimension>
void
PostGISTable<TConnectionImplementation, TPrecision, TSpatialDimension>
::InsertPolygons(PolygonConstPointerType polygonExtRing,
                 PolygonListConstPointerType polygonListInteriorRing,
                 const std::string& attribute)
{

  /** correct polygon exterior ring */
  CorrectFunctorType correct;
  PolygonPointerType correctPolygonExtRing = correct(polygonExtRing);

  std::stringstream sqlCmd;
  this->InsertBegin (sqlCmd);
  sqlCmd << "POLYGON( ( ";
  //std::cout << "sqlcmd: " << sqlCmd.str() << std::endl;
  typedef typename PolygonType::VertexListConstIteratorType VertexIterator;
  VertexIterator itVertex = correctPolygonExtRing->GetVertexList()->Begin();
  //std::cout << "sizeof ext poly: " << polygonExtRing->GetVertexList()->Size()<< std::endl;
  while (itVertex != correctPolygonExtRing->GetVertexList()->End())
    {
    //polygon->line_to(itVertex.Value()[0], m_SensorModelFlip*itVertex.Value()[1]);
    //std::cout << "vertex: " << itVertex.Value()<< std::endl;
    for (unsigned int i = 0; i < TSpatialDimension; ++i)
      {
      sqlCmd << itVertex.Value()[i] << " ";
      //++itVertex;
      }
    sqlCmd << ",";
    ++itVertex;
    }
  //Erase the last ','
  //std::cout << "avt erase :  " << sqlCmd.str() << std::endl;
  EraseLastChar (sqlCmd);
  //std::cout << "apres arase  " << sqlCmd.str() << std::endl;
  sqlCmd << "),";
  //std::cout << "ext polygon: " << sqlCmd.str() << std::endl;
  //Add interior polygons
  if (!polygonListInteriorRing.IsNull())
    {

    //get holes
    //sqlCmd << ",";
    typedef typename PolygonListType::ConstIterator PolygonListConstIteratorType;
    for (PolygonListConstIteratorType itPolygonList = polygonListInteriorRing->Begin();
         itPolygonList != polygonListInteriorRing->End();
         ++itPolygonList)
      {
      /** correct current polygon interior ring */
      CorrectFunctorType correctCurrentInt;
      PolygonPointerType correctCurrentPolygonIntRing = correctCurrentInt(itPolygonList.Get());

      sqlCmd << "(";

      itVertex = correctCurrentPolygonIntRing->GetVertexList()->Begin();
      while (itVertex != correctCurrentPolygonIntRing->GetVertexList()->End())
        {
        //polygon->line_to(itVertex.Value()[0], m_SensorModelFlip*itVertex.Value()[1]);
        for (unsigned int i = 0; i < TSpatialDimension; i++)
          {
          sqlCmd << itVertex.Value()[i] << " ";
          ++itVertex;
          }
        sqlCmd << ",";
        ++itVertex;
        }
      //Erase the last ','
      EraseLastChar (sqlCmd);

      sqlCmd << "),";
      }
    //std::cout << "int polygon: " << sqlCmd.str() << std::endl;

    }
  //Erase the last ','
  EraseLastChar (sqlCmd);

  //int this->GetSrid()=-1;

  sqlCmd << ")'," << this->GetSrid() << ") ,'" << attribute << "');" << std::endl;
  //std::cout << "sqlcmd polygon: " << sqlCmd.str() << std::endl;
  //Insert the geometry
  this->InsertGeometries(sqlCmd.str());
  //std::cout << "geometries!!"  << std::endl;
}

template <class TConnectionImplementation, class TPrecision, unsigned int TSpatialDimension>
void
PostGISTable<TConnectionImplementation, TPrecision, TSpatialDimension>
::InsertGeometries(const std::string& sqlCmd)
{
  typedef otb::PostGISFromStringTransactor TransactorType;
  TransactorType myStringTransactor;
  //std::cout << "transactor string"  << sqlCmd<< std::endl;
  myStringTransactor.SetTransactionString(sqlCmd);
  //this->GetConnection()->ConnectToDB();
  this->GetConnection()->GetConnection()->perform(myStringTransactor);
// this->GetConnection()->PerformTransaction( myStringTransactor );
//myStringTransactor ( *( this->GetConnection()->GetConnection() ) );
}

template <class TConnectionImplementation, class TPrecision, unsigned int TSpatialDimension>
void
PostGISTable<TConnectionImplementation, TPrecision, TSpatialDimension>
::EraseLastChar(std::stringstream& sqlCmd)
{
  std::string EraseCmd = sqlCmd.str();
  EraseCmd.erase(EraseCmd.length() - 1);
  sqlCmd.str("");
  sqlCmd << EraseCmd;

}

template <class TConnectionImplementation, class TPrecision, unsigned int TSpatialDimension>
void
PostGISTable<TConnectionImplementation, TPrecision, TSpatialDimension>
::CreateTable(bool dropExistingGISTable)
{
  typedef otb::PostGISCreateTableTransactor TransactorType;

  //Instantiation
  TransactorType myTransactor;

  myTransactor.SetDimension(TSpatialDimension);

  //std::string name = "mytable";
  myTransactor.SetTableName(this->GetTableName());

  //int this->GetSrid() = -1;
  myTransactor.SetSRID(this->GetSrid());

  myTransactor.SetRemoveExistingTable(dropExistingGISTable);

  this->GetConnection()->GetConnection()->perform(myTransactor);
//this->GetConnection()->PerformTransaction( myTransactor );
}

template <class TConnectionImplementation, class TPrecision, unsigned int TSpatialDimension>
void
PostGISTable<TConnectionImplementation, TPrecision, TSpatialDimension>
::getGeometryType()
{
  //the geomtric column name is the_geom
  std::string query = "SELECT DISTINCT geometrytype(\"the_geom\") "
                      "FROM \"" + this->GetTableName() + "\" WHERE NOT geometrytype(\"the_geom\") IS NULL";

  typedef otb::PostGISFromStringTransactor TransactorType;
  TransactorType myStringTransactor;
  //std::cout << "transactor string"  << sqlCmd<< std::endl;
  myStringTransactor.SetTransactionString(query);
  //this->GetConnection()->ConnectToDB();

  //typedef pqxx::result ResultType;
  //ResultType R = this->GetConnection()->GetConnection()->perform( myStringTransactor );

}

template <class TConnectionImplementation, class TPrecision, unsigned int TSpatialDimension>
std::string
PostGISTable<TConnectionImplementation, TPrecision, TSpatialDimension>
::GetOGRStrConnection() const
{
  //std::cout << "getOGR: " << m_Connection << std::endl;
  std::string connectionSTR = "";
  connectionSTR += "PG:";

  connectionSTR += "dbname='";
  connectionSTR += m_Connection->GetDBName();
  connectionSTR += "' ";
  connectionSTR += "host='";
  connectionSTR += m_Connection->GetHost();
  connectionSTR += "' ";
  connectionSTR += "port='";
  connectionSTR += m_Connection->GetPort();
  connectionSTR += "' ";
  connectionSTR += "user='";
  connectionSTR += m_Connection->GetUser();
  connectionSTR += "' ";
  connectionSTR += "password='";
  connectionSTR += m_Connection->GetPassword();
  connectionSTR += "' ";
  if (!m_TableName.empty())
    {
    connectionSTR += "tables=";
    connectionSTR += m_TableName;
//     connectionSTR += "' ";
    }

  otbGenericMsgDebugMacro(<< "OGR connection string " << connectionSTR);

  return connectionSTR;
}
} // end namespace otb

#endif
