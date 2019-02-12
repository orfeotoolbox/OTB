/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbGISConnectionImplementation_h
#define otbGISConnectionImplementation_h

#include "itkDataObject.h"
#include "itkObjectFactory.h"
#include <string>

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

  virtual void PerformTransaction(const TransactorType& itkNotUsed(theTransaction)) const {}

protected:
  /** Constructor */
  GISConnectionImplementation(){};
  /** Destructor */
  virtual ~GISConnectionImplementation(){};

private:
  GISConnectionImplementation(const Self &) = delete;
  void operator =(const Self&) = delete;

  std::string m_Host;
  std::string m_DBName;
  std::string m_User;
  std::string m_Password;
  std::string m_Port;
  std::string m_Options;

};
} // end namespace otb

#endif
