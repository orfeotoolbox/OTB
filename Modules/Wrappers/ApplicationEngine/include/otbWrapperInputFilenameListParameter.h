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

#ifndef otbWrapperInputFilenameListParameter_h
#define otbWrapperInputFilenameListParameter_h


#include "otbWrapperParameterList.h"
#include "otbWrapperStringParameter.h"


namespace otb
{

namespace Wrapper
{

/** \class InputFilenameListParameter
 *  \brief This class represents a list of InputFilename parameter
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT InputFilenameListParameter : public ParameterList<StringParameter>
{
  //
  // Public types.
public:
  /** Standard class typedef */
  typedef InputFilenameListParameter     Self;
  typedef ParameterList<StringParameter> Superclass;

  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  //
  // Public methods.
public:
  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(InputFilenameListParameter, Superclass);

  /** */
  Role GetDirection(std::size_t) const override;

  /** */
  Role GetDirection() const override;

  /** */
  const std::string& GetFilenameFilter(std::size_t) const override;

  /** */
  const std::string& GetFilenameFilter() const override;

  ParameterType GetType() const override;


  //
  // Protected methods.
protected:
  /** Constructor */
  InputFilenameListParameter();

  /** Destructor */
  ~InputFilenameListParameter() override;

  //
  // Private methods.
private:
  InputFilenameListParameter(const Parameter&) = delete;
  void operator=(const Parameter&) = delete;

}; // End class InputFilenameList Parameter

} // End namespace Wrapper

} // End namespace otb

#endif
