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

#ifndef otbMachineLearningModelFactory_h
#define otbMachineLearningModelFactory_h

#include "otbMachineLearningModel.h"
#include "otbMachineLearningModelFactoryBase.h"

namespace otb
{
/** \class MachineLearningModelFactory
 * \brief Creation of object instance using object factory.
 *
 * \ingroup OTBSupervised
 */
template <class TInputValue, class TOutputValue>
class MachineLearningModelFactory : public MachineLearningModelFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef MachineLearningModelFactory   Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Class Methods used to interface with the registered factories */

  /** Run-time type information (and related methods). */
  itkTypeMacro(MachineLearningModelFactory, itk::Object);

  /** Convenient typedefs. */
  typedef otb::MachineLearningModel<TInputValue, TOutputValue> MachineLearningModelType;
  typedef typename MachineLearningModelType::Pointer MachineLearningModelTypePointer;

  /** Mode in which the files is intended to be used */
  typedef enum { ReadMode, WriteMode } FileModeType;

  /** Create the appropriate MachineLearningModel depending on the particulars of the file. */
  static MachineLearningModelTypePointer CreateMachineLearningModel(const std::string& path, FileModeType mode);

  static void CleanFactories();

protected:
  MachineLearningModelFactory();
  ~MachineLearningModelFactory() override;

private:
  MachineLearningModelFactory(const Self&) = delete;
  void operator=(const Self&) = delete;

  /** Register Built-in factories */
  static void RegisterBuiltInFactories();

  /** Register a single factory, ensuring it has not been registered
    * twice */
  static void RegisterFactory(itk::ObjectFactoryBase* factory);
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMachineLearningModelFactory.hxx"
#endif

#endif
