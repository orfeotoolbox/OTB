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
#ifndef __otbGISConnectionImplementation_h
#define __otbGISConnectionImplementation_h

#include "itkDataObject.h"
#include "itkObjectFactory.h"

namespace otb
{
/** \class GISConnectionImplementation
 * \brief Base class for GIS DB connection implementations
 *
 *
 *
 * \ingroup OTBVectorDataBase
 */

template <class TransactorType>
class ITK_EXPORT GISConnectionImplementation
  : public itk::DataObject
{
public:
  /** Standard class typedefs */
  typedef GISConnectionImplementation   Self;
  typedef itk::DataObject               Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macros */
  itkNewMacro(Self);
  itkTypeMacro(GISConnectionImplementation, itk::DataObject);

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

  virtual void ConnectToDB(){}

  virtual void PerformTransaction(const TransactorType& theTransaction) const {}

protected:
  /** Constructor */
  GISConnectionImplementation(){};
  /** Destructor */
  virtual ~GISConnectionImplementation(){};

private:
  GISConnectionImplementation(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  std::string m_Host;
  std::string m_DBName;
  std::string m_User;
  std::string m_Password;
  std::string m_Port;
  std::string m_Options;

};
} // end namespace otb

#endif
