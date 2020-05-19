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

#ifndef otbWrapperAbstractParameterList_h
#define otbWrapperAbstractParameterList_h


#include "otbWrapperParameter.h"
#include "otbWrapperStringListInterface.h"


namespace otb
{

namespace Wrapper
{


/** \class AbstractParameterList
 *  \brief This class is a base class for list-type parameters
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT AbstractParameterList : public Parameter, public StringListInterface
{
  //
  // Public types.
public:
  /** Standard class typedef */
  typedef AbstractParameterList Self;
  typedef Parameter             Superclass;

  //
  // Public methods.
public:
  /** RTTI support */
  itkTypeMacro(AbstractParameterList, Parameter);

  //
  // Protected methods.
protected:
  /** Constructor */
  AbstractParameterList();

  /** Destructor */
  ~AbstractParameterList() override;

  //
  // Private methods.
private:
  AbstractParameterList(const Parameter&) = delete;
  void operator=(const Parameter&) = delete;

  //
  // Protected methods.
protected:
  //
  // Private attributes.
private:
}; // End class InputImage Parameter

} // End namespace Wrapper

} // End namespace otb

#endif
