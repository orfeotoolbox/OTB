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
template <class TConnectionImplementation, unsigned int SpatialDimension =2>
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

  /** Acessors */

  itkGetMacro(TableName, std::string);
  itkSetMacro(TableName, std::string);

  itkGetObjectMacro(Connection, ConnectionType);
  itkSetObjectMacro(Connection, ConnectionType);


  

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

