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


#include "otbGISConnection.hxx"

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
template <unsigned int VDimension =2>
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
  itkStaticConstMacro(Dimension, unsigned int, VDimension);

  /** Typedefs */
  typedef otb::GISConnection ConnectionType;

  /** Acessors */

  itkGetMacro(TableName, std::string);
  itkSetMacro(TableName, std::string);

  itkGetObjectMacro(Connection, ConnectionType);
  itkSetObjectMacro(Connection, ConnectionType);


  
  /** Clear the table contents */
  virtual bool Clear();


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
  ConnectionType m_Connection;

  
};
}// end namespace otb


#ifndef OTB_MANUAL_INSTANTIATION
#include "otbGISTable.txx"
#endif

#endif

