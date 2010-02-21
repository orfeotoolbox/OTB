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
#ifndef __otbPostGISTable_h
#define __otbPostGISTable_h

#include "otbGISTable.h"
#include "otbCorrectPolygonFunctor.h"

namespace otb
{
/** \class PostGISTable
 * \brief this class represents a table of a geospatial database (PostGIS).
 *
 *
 * \sa GISTableFileReader
 * \sa GISTableFileWriter
 *
 */
template <class TConnectionImplementation, class TPrecision =double, unsigned int SpatialDimension =2>
class ITK_EXPORT PostGISTable
  : public GISTable <TConnectionImplementation, TPrecision, SpatialDimension>
{
public:
  /** Standard class typedefs */
  typedef PostGISTable Self;
  typedef GISTable <TConnectionImplementation, TPrecision, SpatialDimension> Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;


  /** Standard macros */
  itkNewMacro(Self);
  itkTypeMacro(PostGISTable,GISTable);
  itkStaticConstMacro(Dimension, unsigned int, SpatialDimension);

  /** Some convenient typedefs */
  typedef TConnectionImplementation ConnectionType;
  typedef typename ConnectionType::Pointer ConnectionPointerType;
  typedef itk::Point<TPrecision , SpatialDimension > PointType;
  typedef PolyLineParametricPathWithValue < TPrecision , SpatialDimension >  LineType;
  typedef typename LineType::Pointer        LinePointerType;
  typedef Polygon < TPrecision >                PolygonType;
  typedef typename PolygonType::Pointer                        PolygonPointerType;
  typedef typename PolygonType::ConstPointer                PolygonConstPointerType;
  typedef ObjectList< PolygonType >                PolygonListType;
  typedef typename PolygonListType::Pointer                PolygonListPointerType;
  typedef typename PolygonListType::ConstPointer        PolygonListConstPointerType;
  
  /** typedefs for correct polygon */
  typedef otb::CorrectPolygonFunctor<PolygonType> CorrectFunctorType;
  
  
  /** Acessors */

  itkGetConstMacro(TableName, std::string);
  itkSetMacro(TableName, std::string);

  //itkGetConstObjectMacro(Connection, ConnectionType);
  itkGetObjectMacro(Connection, ConnectionType);
  itkSetObjectMacro(Connection, ConnectionType);

  /** Clear the table  not implemented yet*/
  bool Clear();
  
  /** Get attributes of the Table*/ //TODO implement
  
  /** Get srid of the geometric column*/ //TODO implement
  //virtual
  /** Init basic SQL command*/
  void InsertBegin( std::stringstream & sqlCmd );
  /** Add Point content to the GIS Table*/
  void InsertPoint( const PointType &pt , const std::string & attribute = 0);
  //void InsertMultiPoint();
  /** Add Polygons to the GIS Table (exterior and interior ring)*/
  void InsertPolygons(PolygonConstPointerType polygonExtRing, PolygonListConstPointerType polygonListInteriorRing=0, const std::string & attribute = 0);
  /** Add Line to the GIS Table*/
  void InsertLineString(LinePointerType l, const std::string & attribute = 0);
  
  /** Execute the sqlCmd which add geometric rows to the gis table*/
  void InsertGeometries (const std::string &sqlCmd );
  /** Clean up SQL command*/
  void EraseLastChar (std::stringstream &sqlCmd );
  /** Effective Creation of the table*/
  void CreateTable (bool dropExistingGISTable);
  /** Get geometry column type Not implemented yet in progress */
  void getGeometryType();
  
  /** Get string connection usable by OGR library*/
  std::string GetOGRStrConnection() const;
  
  /** Get const string connection methos temp*/
  //ConnectionPointerType GetConstConnection() const;
  
  /** Add an alpha numeric column to the table */
//   const std::string AddVarCharColumn(unsigned int size) ;
  
  /** Insert Alpha Numeric Data in the Car char column */
//   void AddStrDataToVarCharColumn(std::string data) ;
protected:

  /** Constructor */
  PostGISTable();
  /** Destructor */
  virtual ~PostGISTable() {};
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  PostGISTable(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  std::string m_TableName;
  ConnectionPointerType m_Connection;

};
}// end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbPostGISTable.txx"
#endif

#endif

