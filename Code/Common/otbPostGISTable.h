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

  itkGetObjectMacro(Connection, ConnectionType);
  itkSetObjectMacro(Connection, ConnectionType);

  /** Clear the vector data  not implemented yet*/
  bool Clear();
  
  /** Get attributes of the Table*/ //TODO implement
  
  /** Get srid of the geometric column*/ //TODO implement 
  //virtual 
  /** Add Point content to the GIS Table*/ //TODO implement
  void InsertBegin( std::stringstream & sqlCmd );
  void InsertPoint( const PointType &pt );
  //void InsertMultiPoint();
  void InsertPolygons(PolygonConstPointerType polygonExtRing, PolygonListConstPointerType polygonListInteriorRing);
  void InsertLineString(LinePointerType l);
  
  void InsertGeometries (const std::string &sqlCmd );
  
  void EraseLastChar (std::stringstream &sqlCmd );
  
  void CreateTable ();
  
  void getGeometryType();
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

