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

#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include "itkPoint.h"
#include "otbPolyLineParametricPathWithValue.h"
#include "otbPolygon.h"

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
template <class TConnectionImplementation, class TPrecision = double, unsigned int SpatialDimension =2>
class GISTable
      : public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef GISTable Self;
  typedef itk::DataObject Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;



  /** Standard macros */
  itkNewMacro(Self);
  itkTypeMacro(GISTable,DataObject);
  itkStaticConstMacro(Dimension, unsigned int, SpatialDimension);

  /** Typedefs */
  typedef TConnectionImplementation ConnectionType;
  typedef typename ConnectionType::Pointer ConnectionPointerType;
  
  //typedef TConnectionImplementation::TransactionType TransactionType;
  
  typedef itk::Point<TPrecision , SpatialDimension > PointType;
  typedef PolyLineParametricPathWithValue < TPrecision , SpatialDimension >  LineType;
  typedef typename LineType::Pointer 	LinePointerType;
  
  typedef Polygon < TPrecision > 	        PolygonType;
  typedef typename PolygonType::Pointer 	                PolygonPointerType;
  typedef typename PolygonType::ConstPointer 	        PolygonConstPointerType;
  typedef ObjectList< PolygonType > 	        PolygonListType;
  typedef typename PolygonListType::Pointer 	        PolygonListPointerType;
  typedef typename PolygonListType::ConstPointer 	PolygonListConstPointerType;
  /** Acessors */

  itkGetConstMacro(TableName, std::string);
  itkSetMacro(TableName, std::string);

  itkGetConstObjectMacro(Connection, ConnectionType);
  itkSetObjectMacro(Connection, ConnectionType);

  /** Clear the vector data  not implemented yet*/
  virtual bool Clear(){};
  
  /** Get attributes of the Table*/ //TODO implement
  
  /** Get srid of the geometric column*/ //TODO implement 
  //virtual 
  /** Add Point content to the GIS Table*/ //TODO implement
  virtual void InsertPoint( const PointType &pt ){};
  virtual void InsertMultiPoint(){};
  virtual void InsertPolygons(PolygonConstPointerType polygonExtRing, PolygonListConstPointerType polygonListInteriorRing){};
  virtual void InsertLineString(LinePointerType l){};
  
  virtual void CreateTable(bool dropExistingGISTable){};
  
  virtual void getGeometryType(){};
protected:
  /** Constructor */
  GISTable();
  /** Destructor */
  virtual ~GISTable() {};
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  GISTable(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


  std::string m_TableName;
  ConnectionPointerType m_Connection;

  
};
}// end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGISTable.txx"
#endif

#endif

