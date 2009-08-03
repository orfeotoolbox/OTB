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

template <class TConnectionImplementation, class TPrecision, unsigned int SpatialDimension>
 PostGISTable<TConnectionImplementation, TPrecision, SpatialDimension>
  ::PostGISTable() 
{
  //super();
  //std::cout << "create postgistable" << std::endl;
  //m_TableName="VectorDataToPostGIS";
  
  m_Connection = ConnectionType::New();
  /*
  m_Connection->SetHost( "localhost" );
  m_Connection->SetDBName( "test" );
  m_Connection->SetUser( "postgres" );
  m_Connection->SetPassword( "postgres" );
  std::cout << "this: " << this->GetTableName()<< std::endl;
  */
}

  

template <class TConnectionImplementation, class TPrecision, unsigned int SpatialDimension>
void
    PostGISTable<TConnectionImplementation, TPrecision, SpatialDimension>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os<<std::endl;


  os<<"Table name: "<< m_TableName<<std::endl;

}

template <class TConnectionImplementation, class TPrecision, unsigned int SpatialDimension>
bool
    PostGISTable<TConnectionImplementation, TPrecision, SpatialDimension>
  ::Clear()
{ //TODO implementation
  return true;
}

template <class TConnectionImplementation, class TPrecision, unsigned int SpatialDimension>
void
PostGISTable<TConnectionImplementation, TPrecision, SpatialDimension>
::InsertBegin( std::stringstream & sqlCmd )
{
  sqlCmd.str("");
  sqlCmd << "INSERT INTO \"" << this->GetTableName() << "\" ( \"the_geom\" ) VALUES (";
  sqlCmd << "GeometryFromText('";
}
template <class TConnectionImplementation, class TPrecision, unsigned int SpatialDimension>
void
PostGISTable<TConnectionImplementation, TPrecision, SpatialDimension>
  ::InsertPoint( const PointType &pt )
{
  std::stringstream sqlCmd;
  this->InsertBegin( sqlCmd );
  sqlCmd << "POINT( ";
  for (uint i=0 ; i < SpatialDimension ; i++)
  {
    sqlCmd << pt[i] << " ";      
  }
  
  int srid=-1;
  sqlCmd << ")'," << srid << ") );" << std::endl;
  
  
  //Execute the query
  //this->ExecuteQuery(sqlCmd)
}

template <class TConnectionImplementation, class TPrecision, unsigned int SpatialDimension>
    void
        PostGISTable<TConnectionImplementation, TPrecision, SpatialDimension>
  ::InsertLineString( LinePointerType l )
{
  std::stringstream sqlCmd;
  this->InsertBegin ( sqlCmd );
  sqlCmd << "LINESTRING( ";
  
  typedef typename LineType::VertexListConstIteratorType VertexIterator;
  VertexIterator itVertex = l->GetVertexList()->Begin();
  while (itVertex != l->GetVertexList()->End())
  {
    for (uint i=0 ; i < SpatialDimension ; i++)
    {
      sqlCmd << itVertex.Value()[i] << " "; 
      ++itVertex;
    } 
    sqlCmd << ",";
  }
  //Erase the last ','
  EraseLastChar ( sqlCmd );
  
  int srid=-1;
  
  sqlCmd << ")'," << srid << ") );" << std::endl;
  //Execute the query
  //this->ExecuteQuery(sqlCmd)
  
  /* Invalid (MULTI)Linestring */

}

template <class TConnectionImplementation, class TPrecision, unsigned int SpatialDimension>
void
PostGISTable<TConnectionImplementation, TPrecision, SpatialDimension>
::InsertPolygons(PolygonConstPointerType polygonExtRing, PolygonListConstPointerType polygonListInteriorRing)
{
  std::stringstream sqlCmd;
  this->InsertBegin ( sqlCmd );
  sqlCmd << "POLYGON( ( ";
  //std::cout << "sqlcmd: " << sqlCmd.str() << std::endl;
  typedef typename PolygonType::VertexListConstIteratorType VertexIterator;
  VertexIterator itVertex = polygonExtRing->GetVertexList()->Begin();
  while (itVertex != polygonExtRing->GetVertexList()->End())
  {
    //polygon->line_to(itVertex.Value()[0],m_SensorModelFlip*itVertex.Value()[1]);
    for (uint i=0 ; i < SpatialDimension ; i++)
    {
      sqlCmd << itVertex.Value()[i] << " "; 
      ++itVertex;
    } 
    sqlCmd << ",";
    ++itVertex;
  }
  //Erase the last ','
  EraseLastChar ( sqlCmd );
  
  sqlCmd << "),";
  //Add interior polygons 
  if ( !polygonListInteriorRing.IsNull() )
  {
    
    //get holes
    //sqlCmd << ",";
    typedef typename PolygonListType::ConstIterator 	PolygonListConstIteratorType;
    for (PolygonListConstIteratorType itPolygonList = polygonListInteriorRing->Begin();itPolygonList!=polygonListInteriorRing->End();++itPolygonList)
    {
      sqlCmd << "(";
      itVertex = itPolygonList.Get()->GetVertexList()->Begin();
      while (itVertex != itPolygonList.Get()->GetVertexList()->End())
      {
    //polygon->line_to(itVertex.Value()[0],m_SensorModelFlip*itVertex.Value()[1]);
        for (uint i=0 ; i < SpatialDimension ; i++)
        {
          sqlCmd << itVertex.Value()[i] << " ";
          ++itVertex;
        } 
        sqlCmd << ",";
        ++itVertex;
      }
      //Erase the last ','
      EraseLastChar ( sqlCmd );
       
      sqlCmd << "),";
    }
    //std::cout << "sqlcmd: " << sqlCmd.str() << std::endl;
    
  }
  //Erase the last ','
  EraseLastChar ( sqlCmd );
  
  int srid=-1;
  
  sqlCmd << ")'," << srid << ") );" << std::endl;
  std::cout << "sqlcmd: " << sqlCmd.str() << std::endl;
  //Insert the geometry
  this->InsertGeometries(sqlCmd.str());
  std::cout << "geometries!!"  << std::endl;
}


template <class TConnectionImplementation, class TPrecision, unsigned int SpatialDimension>
void
PostGISTable<TConnectionImplementation, TPrecision, SpatialDimension>
::InsertGeometries (const std::string &sqlCmd )
{
  typedef otb::PostGISFromStringTransactor TransactorType;
  TransactorType myStringTransactor; 
  //std::cout << "transactor string"  << sqlCmd<< std::endl;
  myStringTransactor.SetTransactionString( sqlCmd );
  //this->GetConnection()->ConnectToDB();
  this->GetConnection()->GetConnection()->perform( myStringTransactor ); 
}

template <class TConnectionImplementation, class TPrecision, unsigned int SpatialDimension>
void
PostGISTable<TConnectionImplementation, TPrecision, SpatialDimension>
::EraseLastChar (std::stringstream &sqlCmd )
{
  std::string EraseCmd = sqlCmd.str();
  EraseCmd.erase(EraseCmd.length() - 1);
  sqlCmd.str("");
  sqlCmd << EraseCmd;

}

template <class TConnectionImplementation, class TPrecision, unsigned int SpatialDimension>
    void
        PostGISTable<TConnectionImplementation, TPrecision, SpatialDimension>
  ::CreateTable (bool dropExistingGISTable)
{
  typedef otb::PostGISCreateTableTransactor TransactorType;

  //Instantiation
  TransactorType myTransactor;
  
  myTransactor.SetDimension( SpatialDimension );

  //std::string name = "mytable";
  myTransactor.SetTableName( this->GetTableName() );

  int srid = -1;
  myTransactor.SetSRID( srid );

  myTransactor.SetRemoveExistingTable( dropExistingGISTable );

  this->GetConnection()->GetConnection()->perform( myTransactor );

}

template <class TConnectionImplementation, class TPrecision, unsigned int SpatialDimension>
void
PostGISTable<TConnectionImplementation, TPrecision, SpatialDimension>
::getGeometryType()
{
  //the geomtric column name is the_geom
  std::string query="SELECT DISTINCT geometrytype(\"the_geom\") "
      "FROM \""+ this->GetTableName() + "\" WHERE NOT geometrytype(\"the_geom\") IS NULL";
  
  typedef otb::PostGISFromStringTransactor TransactorType;
  TransactorType myStringTransactor; 
  //std::cout << "transactor string"  << sqlCmd<< std::endl;
  myStringTransactor.SetTransactionString( query );
  //this->GetConnection()->ConnectToDB();
  
  //typedef pqxx::result ResultType;
  //ResultType R = this->GetConnection()->GetConnection()->perform( myStringTransactor ); 
  
}      
} // end namespace otb

#endif
