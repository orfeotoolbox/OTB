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

#ifndef otbBSQImageIOFactory_h
#define otbBSQImageIOFactory_h

#include "itkObjectFactoryBase.h"
#include "OTBIOBSQExport.h"
namespace otb
{
/** \class BSQImageIOFactory
 * \brief Creation d'un instance d'un objet BSQImageIO utilisant les object factory.
 *
 * \ingroup OTBIOBSQ
 */
class OTBIOBSQ_EXPORT BSQImageIOFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef BSQImageIOFactory             Self;
  typedef itk::ObjectFactoryBase        Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  const char* GetITKSourceVersion(void) const override;
  const char* GetDescription(void) const override;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);
  static BSQImageIOFactory * FactoryNew() { return new BSQImageIOFactory; }

  /** Run-time type information (and related methods). */
  itkTypeMacro(BSQImageIOFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    BSQImageIOFactory::Pointer BSQFactory = BSQImageIOFactory::New();
    itk::ObjectFactoryBase::RegisterFactoryInternal(BSQFactory);
  }

protected:
  BSQImageIOFactory();
  ~BSQImageIOFactory() override;

private:
  BSQImageIOFactory(const Self &) = delete;
  void operator =(const Self&) = delete;

};

} // end namespace otb

#endif
