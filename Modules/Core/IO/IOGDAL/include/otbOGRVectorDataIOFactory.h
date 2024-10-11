/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbOGRVectorDataIOFactory_h
#define otbOGRVectorDataIOFactory_h

#include "itkObjectFactoryBase.h"
#include "OTBIOGDALExport.h"

namespace otb
{
/** \class OGRVectorDataIOFactory
 * \brief Create an instance of an OGRVectorDataIOFactory.
 *
 * \ingroup OTBIOGDAL
 */
class OTBIOGDAL_EXPORT OGRVectorDataIOFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef OGRVectorDataIOFactory        Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  const char* GetITKSourceVersion(void) const override;
  const char* GetDescription(void) const override;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OGRVectorDataIOFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    OGRVectorDataIOFactory::Pointer OGRFactory = OGRVectorDataIOFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(OGRFactory);
  }

protected:
  OGRVectorDataIOFactory();
  ~OGRVectorDataIOFactory() override;

private:
  OGRVectorDataIOFactory(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
