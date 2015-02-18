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
#ifndef __otbVectorDataToGISTableFilter_h
#define __otbVectorDataToGISTableFilter_h

//#include "itkImageToImageFilter.h"
//#include "itkAttributeLabelObject.h"
#include "otbGISTableSource.h"
#include "otbGISConnectionImplementation.h"
#include <iostream>
#include <sstream>
#include <string>
#include "otbOGRVectorDataIO.h"

namespace otb {

/** \class VectorDataToGISTableFilter
 * \brief Convert a LabelMap to a VectorData
 *
 * VectorDataToGISTableFilter converts  a
 * VectorData to GIS Table (PostGIS for example).
 * Create first a PostGIS table (by default name=vector_data_to_gis_sample)
 * The table is not drop if it already exist
 * Structure of the PostGIS table:
 * vector_data_to_postgis (id serial, the_geom geometry)
 * with srid = -1
 * multi geometries are not handle yet
 *
 * \sa GISTableSource
 * \ingroup Common, GeospatialAnalysis
 */
template<class TVectorData, class TGISTable>
class ITK_EXPORT VectorDataToGISTableFilter :
  public GISTableSource<TGISTable>
{
public:
  /** Standard class typedefs. */
  typedef VectorDataToGISTableFilter Self;
  typedef GISTableSource<TGISTable>
  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Some convenient typedefs. */
  typedef TVectorData                                InputVectorDataType;
  typedef TGISTable                                  OutputGISTableType;
  typedef typename InputVectorDataType::Pointer      InputVectorDataPointer;
  typedef typename InputVectorDataType::ConstPointer InputVectorDataConstPointer;
  typedef typename OutputGISTableType::Pointer       OutputGISTablePointer;
  typedef typename OutputGISTableType::ConstPointer  OutputGISTableConstPointer;

  typedef typename InputVectorDataType::DataTreeType::TreeNodeType InternalTreeNodeType;
  typedef typename InternalTreeNodeType::ChildrenListType          ChildrenListType;
  typedef typename InputVectorDataType::DataNodeType               DataNodeType;
  typedef typename DataNodeType::Pointer                           DataNodePointerType;

  typedef typename OutputGISTableType::ConnectionType InputGISConnectionType;
  typedef typename InputGISConnectionType::Pointer    InputGISConnectionPointerType;

  typedef OGRVectorDataIO                       OGRVectorDataIOType;
  typedef typename OGRVectorDataIOType::Pointer OGRVectorDataIOPointerType;

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(VectorDataToGISTableFilter,
               GISTableSource);

  /**
   * Set/Get the boolean value if you do not want to create the GIS table
   */
  itkSetMacro(DropExistingGISTable, bool);
  itkGetConstMacro(DropExistingGISTable, bool);

  itkSetMacro(GISTableName, std::string);
  itkGetConstMacro(GISTableName, std::string);

  itkGetObjectMacro(InputGISConnection, InputGISConnectionType);
  itkSetObjectMacro(InputGISConnection, InputGISConnectionType);

  //void setConnection

  /** Set/Get the VectorData input of this process object.  */
  virtual void SetInput(const InputVectorDataType *input);
  virtual void SetInput(unsigned int idx, const InputVectorDataType *input);
  const InputVectorDataType * GetInput(void);
  const InputVectorDataType * GetInput(unsigned int idx);

protected:
  VectorDataToGISTableFilter();
  virtual ~VectorDataToGISTableFilter() {}
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** VectorDataToGISTAbleFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion();

  /** Single-threaded version of GenerateData.  */
  void GenerateData();

private:
  VectorDataToGISTableFilter(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  /** Remove table before insertion if true  (Not use for now, need access to specific options of OGR connection*/
  bool m_DropExistingGISTable;

  /** Connection parameters to the db*/
  InputGISConnectionPointerType m_InputGISConnection;
  /** GIS table name */
  std::string m_GISTableName;
}; // end of class

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbVectorDataToGISTableFilter.txx"
#endif

#endif
