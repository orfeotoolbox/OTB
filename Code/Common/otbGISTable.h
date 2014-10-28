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
#ifndef __otbGISTable_h
#define __otbGISTable_h

#include "itkObjectFactory.h"
#include "itkPoint.h"
#include "otbPolygon.h"
#include "otbObjectList.h"

namespace otb
{
/** \class GISTable
 * \brief this class represents a table of a geospatial database (ie. PostGIS).
 *
 *
 * \sa GISTableFileReader
 * \sa GISTableFileWriter
 *
 */
template <class TConnectionImplementation, class TPrecision = double, unsigned int TSpatialDimension = 2>
class GISTable
  : public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef GISTable                      Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macros */
  itkNewMacro(Self);
  itkTypeMacro(GISTable, DataObject);
  itkStaticConstMacro(Dimension, unsigned int, TSpatialDimension);

  /** Typedefs */
  typedef TPrecision PrecisionType;

  typedef TConnectionImplementation        ConnectionType;
  typedef typename ConnectionType::Pointer ConnectionPointerType;

  //typedef TConnectionImplementation::TransactionType TransactionType;

  typedef itk::Point<TPrecision, TSpatialDimension>                       PointType;
  typedef PolyLineParametricPathWithValue <TPrecision, TSpatialDimension> LineType;
  typedef typename LineType::Pointer                                      LinePointerType;

  typedef Polygon <TPrecision>                   PolygonType;
  typedef typename PolygonType::Pointer          PolygonPointerType;
  typedef typename PolygonType::ConstPointer     PolygonConstPointerType;
  typedef ObjectList<PolygonType>                PolygonListType;
  typedef typename PolygonListType::Pointer      PolygonListPointerType;
  typedef typename PolygonListType::ConstPointer PolygonListConstPointerType;
  /** Acessors */

  //itkStaticConstMacro(GISDimension, unsigned int, TSpatialDimension);

  itkGetConstMacro(TableName, std::string);
  itkSetMacro(TableName, std::string);

  itkGetConstMacro(Srid, int);
  itkSetMacro(Srid, int);

  itkGetObjectMacro(Connection, ConnectionType);
  itkSetObjectMacro(Connection, ConnectionType);

  /** Clear the vector data  not implemented yet*/
  virtual bool Clear(){return true; }

  /** Get attributes of the Table*/ //TODO implement

  /** Get srid of the geometric column*/ //TODO implement
  //virtual
  /** Add Point content to the GIS Table*/ //TODO implement
  virtual void InsertPoint(const PointType& pt, const std::string& attribute = 0){}
  virtual void InsertMultiPoint(){}
  virtual void InsertPolygons(PolygonConstPointerType polygonExtRing,
                              PolygonListConstPointerType polygonListInteriorRing = 0,
                              const std::string& attribute = 0){}
  virtual void InsertLineString(LinePointerType l, const std::string& attribute = 0){}

  virtual void CreateTable(bool dropExistingGISTable){}

  virtual void getGeometryType(){}

  virtual void SetProjectionRef(const std::string& projectionRef);
  virtual std::string GetProjectionRef() const;

  /** Get string connection usable by OGR library*/
  virtual std::string GetOGRStrConnection() const {return 0; }

  /** Add an alpha numeric column to the table */
  virtual const std::string AddVarCharColumn(unsigned int size) {return 0; }

  /** Insert Alpha Numeric Data in the Car char column */
  virtual void AddStrDataToVarCharColumn(const std::string& data) {}
protected:
  /** Constructor */
  GISTable();
  /** Destructor */
  virtual ~GISTable() {}
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  int m_Srid;

private:
  GISTable(const Self&); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  std::string           m_TableName;
  ConnectionPointerType m_Connection;

};
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGISTable.txx"
#endif

#endif
