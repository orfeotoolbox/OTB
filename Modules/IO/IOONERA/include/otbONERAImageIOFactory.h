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

#ifndef otbONERAImageIOFactory_h
#define otbONERAImageIOFactory_h

#include "itkObjectFactoryBase.h"

namespace otb
{
/** \class ONERAImageIOFactory
 * \brief Creation d'un instance d'un objet ONERAImageIO utilisant les object factory.
 *
 * \ingroup OTBIOONERA
 */
class ITK_EXPORT ONERAImageIOFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef ONERAImageIOFactory           Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  const char* GetITKSourceVersion(void) const override;
  const char* GetDescription(void) const override;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);
  static ONERAImageIOFactory * FactoryNew() { return new ONERAImageIOFactory; }

  /** Run-time type information (and related methods). */
  itkTypeMacro(ONERAImageIOFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    ONERAImageIOFactory::Pointer ONERAFactory = ONERAImageIOFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(ONERAFactory);
  }

protected:
  ONERAImageIOFactory();
  ~ONERAImageIOFactory() override;

private:
  ONERAImageIOFactory(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#endif
