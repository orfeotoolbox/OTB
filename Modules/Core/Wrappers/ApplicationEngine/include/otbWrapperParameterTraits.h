/*
 * Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbWrapperParameterTraits_h
#define otbWrapperParameterTraits_h

#include "otbWrapperTypes.h"
#include "otbWrapperParameter.h"
#include <itkSmartPointer.h>

namespace otb::Wrapper {

/**
 * Type-traits meant to associate `ParameterType` enum value to a concrete `Parameter` child class.
 * This traits will be used by templated `Application::AddParameter` flavor.
 * \tparam type  Enum value of type `ParameterType`
 *
 * \internal
 * All traits definitions are centralized in a Translation Unit to avoid introducing a dependency
 * between public interface and all the header files where a child class of
 * `otb::Wrapper::Parameter` is defined. In otbWrapperParameterTraits.h header file,
 * `otb::Wrapper::Parameter`  child classes are only forward declred. Not defined.
 * Hence, in order to work around incomplete types and missing _deleter_ abstraction in
 * `itk::SmartPointer<>` class, the `New()` function defined in traits specializations will return a
 * raw pointer and not a RAII encapsulated one.
 */
template <ParameterType type>
struct ParameterTypeTraits
{
  // using Type = void;
};

/** Type alias shortcut to `ParameterTypeTraits<type>::Type`. */
template <ParameterType type>
using ParameterTypeTraits_t = typename ParameterTypeTraits<type>::Type;

/** Type alias shortcut to `ParameterTypeTraits<type>::Pointer`. */
template <ParameterType type>
using ParameterTypeTraits_p = typename ParameterTypeTraits<type>::Pointer;

#define OTB_DECLARE_PARAMETER_TRAITS(EnumValue, ParameterType)   \
  class ParameterType;                                           \
  /** `ParameterTypeTraits` specialisation for `EnumValue`. */   \
  template <>                                                    \
  struct ParameterTypeTraits<EnumValue>                          \
  {                                                              \
    using Type         = ParameterType;                          \
    using Pointer      = Type*;                                  \
    using SmartPointer = itk::SmartPointer<Type>;                \
    static Pointer New(Parameter::Info);                         \
    static itk::SmartPointer<Parameter> Upcast(Pointer & p);     \
  };


OTB_DECLARE_PARAMETER_TRAITS(ParameterType_Group,               ParameterGroup);

OTB_DECLARE_PARAMETER_TRAITS(ParameterType_Choice,              ChoiceParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_ListView,            ListViewParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_Directory,           DirectoryParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_InputFilename,       InputFilenameParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_InputFilenameList,   InputFilenameListParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_OutputFilename,      OutputFilenameParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_InputVectorData,     InputVectorDataParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_OutputImage,         OutputImageParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_OutputVectorData,    OutputVectorDataParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_Float,               FloatParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_Double,              DoubleParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_Int,                 IntParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_Radius,              RadiusParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_StringList,          StringListParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_InputImageList,      InputImageListParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_InputVectorDataList, InputVectorDataListParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_Bool,                BoolParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_Field,               FieldParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_Band,                BandParameter);
OTB_DECLARE_PARAMETER_TRAITS(ParameterType_String,              StringParameter);

#undef OTB_DECLARE_PARAMETER_TRAITS

// Then the cases where we cannot #define New for a single parameter
class InputImageParameter;
/** `ParameterTypeTraits` specialisation for `ParameterType_InputImage`. */
template <>
struct ParameterTypeTraits<ParameterType_InputImage>
{
  using Type         = InputImageParameter;
  using Pointer      = Type*;
  using SmartPointer = itk::SmartPointer<Type>;
  static Pointer New(
      Parameter::Info,
      bool mustReuseLoadedRegion = false,
      unsigned long streamHeight = 0
  );
  static Pointer New(
      Parameter::Info,
      unsigned long streamHeight
  );
  static itk::SmartPointer<Parameter> Upcast(Pointer & p);
};

class RAMParameter;
/** `ParameterTypeTraits` specialisation for `ParameterType_RAM`. */
template <>
struct ParameterTypeTraits<ParameterType_RAM>
{
  using Type         = InputImageParameter;
  using Pointer      = Type*;
  using SmartPointer = itk::SmartPointer<Type>;
  static Pointer New();
  static itk::SmartPointer<Parameter> Upcast(Pointer & p);
};

} // otb::Wrapper namespace

#endif // otbWrapperParameterTraits_h
