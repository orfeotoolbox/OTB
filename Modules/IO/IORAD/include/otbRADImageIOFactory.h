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

#ifndef otbRADImageIOFactory_h
#define otbRADImageIOFactory_h

#include "itkObjectFactoryBase.h"

namespace otb
{
/** \class RADImageIOFactory
 * \brief Creation d'un instance d'un objet RADImageIO utilisant les object factory.
 *
 * \ingroup OTBIORAD
 */
class ITK_EXPORT RADImageIOFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef RADImageIOFactory             Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  const char* GetITKSourceVersion(void) const override;
  const char* GetDescription(void) const override;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);
  static RADImageIOFactory* FactoryNew()
  {
    return new RADImageIOFactory;
  }

  /** Run-time type information (and related methods). */
  itkTypeMacro(RADImageIOFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    RADImageIOFactory::Pointer RADFactory = RADImageIOFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(RADFactory);
  }

protected:
  RADImageIOFactory();
  ~RADImageIOFactory() override;

private:
  RADImageIOFactory(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
