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

#ifndef otbMachineLearningModelFactoryBase_h
#define otbMachineLearningModelFactoryBase_h

#include "itkMutexLock.h"
#include "OTBLearningBaseExport.h"

namespace otb
{
/** \class MachineLearningModelFactoryBase
 * \brief Base class for the MachinelearningModelFactory
 *
 * This class intends to hold the static attributes that can not be
 * part of a template class (ld error).
 *
 * \ingroup OTBLearningBase
 */
class OTBLearningBase_EXPORT MachineLearningModelFactoryBase : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef MachineLearningModelFactoryBase Self;
  typedef itk::Object                     Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(MachineLearningModelFactoryBase, itk::Object);

protected:
  MachineLearningModelFactoryBase();
  ~MachineLearningModelFactoryBase() override;

  static itk::SimpleMutexLock mutex;

private:
  MachineLearningModelFactoryBase(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
