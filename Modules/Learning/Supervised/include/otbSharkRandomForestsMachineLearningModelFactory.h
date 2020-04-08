/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbSharkRandomForestsMachineLearningModelFactory_h
#define otbSharkRandomForestsMachineLearningModelFactory_h

#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace otb
{
/** \class SharkRandomForestsMachineLearningModelFactory
 * \brief Creation of an instance of a SharkRandomForestsMachineLearningModel object using the object factory
 *
 * \ingroup OTBSupervised
 */
template <class TInputValue, class TTargetValue>
class ITK_EXPORT SharkRandomForestsMachineLearningModelFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef SharkRandomForestsMachineLearningModelFactory Self;
  typedef itk::ObjectFactoryBase                        Superclass;
  typedef itk::SmartPointer<Self>                       Pointer;
  typedef itk::SmartPointer<const Self>                 ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const override;
  virtual const char* GetDescription(void) const override;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SharkRandomForestsMachineLearningModelFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    Pointer RFFactory = SharkRandomForestsMachineLearningModelFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(RFFactory);
  }

protected:
  SharkRandomForestsMachineLearningModelFactory();
  virtual ~SharkRandomForestsMachineLearningModelFactory();

private:
  SharkRandomForestsMachineLearningModelFactory(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSharkRandomForestsMachineLearningModelFactory.hxx"
#endif

#endif
