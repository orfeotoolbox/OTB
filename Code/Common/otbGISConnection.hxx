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
#ifndef __otbGISConnection_h
#define __otbGISConnection_h


#include <pqxx/pqxx>

namespace otb
{
/** \class GISConnection
 * \brief this class represents a connection to a geospatial database (ie. PostGIS).
 *
 * 
 * \sa GISConnectionFileReader
 * \sa GISConnectionFileWriter
 *
 */
class GISConnection : public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef GISConnection Self;
  typedef itk::DataObject Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;



  /** Standard macros */
  itkNewMacro(Self);
  itkTypeMacro(GISConnection,DataObject);

  /** Typedefs */
  typedef pqxx::basic_connection<pqxx::connect_direct> BasicConnectionType;

  /** Acessors */




protected:
  /** Constructor */
  GISConnection();
  /** Destructor */
  virtual ~GISConnection() {};
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  GISConnection(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


  std::string m_TableName;

  
};
}// end namespace otb




#endif

