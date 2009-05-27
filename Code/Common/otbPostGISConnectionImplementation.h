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
#ifndef __otbPostGISConnectionImplementation_h
#define __otbPostGISConnectionImplementation_h

#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include <pqxx/pqxx>

namespace otb
{
/** \class PostGISConnectionImplementation
 * \brief this class represents a connection to a geospatial database (ie. PostGIS).
 *
 * 
 * \sa PostGISConnectionImplementationFileReader
 * \sa PostGISConnectionImplementationFileWriter
 *
 */

class ITK_EXPORT PostGISConnectionImplementation : public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef PostGISConnectionImplementation Self;
  typedef itk::DataObject Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;



  /** Standard macros */
  itkNewMacro(Self);
  itkTypeMacro(PostGISConnectionImplementation,itk::DataObject);


  /** Typedefs */
  typedef pqxx::basic_connection<pqxx::connect_direct> BasicConnectionType;
  typedef pqxx::transactor<pqxx::nontransaction> TransactorType;

  /** Acessors */
  itkGetMacro(Host, std::string);
  itkSetMacro(Host, std::string);

  itkGetMacro(DBName, std::string);
  itkSetMacro(DBName, std::string);

  itkGetMacro(User, std::string);
  itkSetMacro(User, std::string);

  itkGetMacro(Password, std::string);
  itkSetMacro(Password, std::string);

  itkGetMacro(Port, std::string);
  itkSetMacro(Port, std::string);

  itkGetMacro(Options, std::string);
  itkSetMacro(Options, std::string);

  /** Using the connection */

  void ConnectToDB();

  void PerformTransaction(const TransactorType& theTransaction);



protected:
  /** Constructor */
  PostGISConnectionImplementation();
  /** Destructor */
  virtual ~PostGISConnectionImplementation();
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  PostGISConnectionImplementation(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


  std::string m_Host;
  std::string m_DBName;
  std::string m_User;
  std::string m_Password;
  std::string m_Port;
  std::string m_Options;

  BasicConnectionType* m_PostGISConnection;


};
}// end namespace otb



#endif

