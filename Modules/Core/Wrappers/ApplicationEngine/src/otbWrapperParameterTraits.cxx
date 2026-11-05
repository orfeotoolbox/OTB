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

#include "otbWrapperParameterTraits.h"
#include "otbWrapperChoiceParameter.h"
#include "otbWrapperListViewParameter.h"
#include "otbWrapperDirectoryParameter.h"
#include "otbWrapperInputFilenameParameter.h"
#include "otbWrapperInputFilenameListParameter.h"
#include "otbWrapperOutputFilenameParameter.h"
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperInputVectorDataParameter.h"
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperParameterGroup.h"
#include "otbWrapperStringParameter.h"
#include "otbWrapperStringListParameter.h"
#include "otbWrapperInputImageListParameter.h"
#include "otbWrapperInputVectorDataListParameter.h"
#include "otbWrapperBoolParameter.h"
#include "otbWrapperFieldParameter.h"
#include "otbWrapperBandParameter.h"

namespace otb::Wrapper {

#define OTB_DEFINE_PARAMETER_TRAITS(EnumValue, ParameterType)                      \
                                                                                   \
  static_assert(ParameterTypeTraits<EnumValue>::Type::Type == EnumValue);          \
                                                                                   \
  auto ParameterTypeTraits<EnumValue>::New()                   \
  -> Pointer                                                                       \
  {                                                                                \
    auto res = Type::New();                                                    \
    res->Register();                                                               \
    return res;                                                                    \
  }                                                                                \
                                                                                   \
  itk::SmartPointer<Parameter> ParameterTypeTraits<EnumValue>::Upcast(Pointer & p) \
  {                                                                                \
    itk::SmartPointer<Parameter> res = p; \
    res->UnRegister(); \
    return res;                                                                    \
  }

OTB_DEFINE_PARAMETER_TRAITS(ParameterType_Group,               ParameterGroup);

OTB_DEFINE_PARAMETER_TRAITS(ParameterType_Choice,              ChoiceParameter);
OTB_DEFINE_PARAMETER_TRAITS(ParameterType_ListView,            ListViewParameter);
OTB_DEFINE_PARAMETER_TRAITS(ParameterType_Directory,           DirectoryParameter);
OTB_DEFINE_PARAMETER_TRAITS(ParameterType_InputFilename,       InputFilenameParameter);
OTB_DEFINE_PARAMETER_TRAITS(ParameterType_InputFilenameList,   InputFilenameListParameter);
OTB_DEFINE_PARAMETER_TRAITS(ParameterType_OutputFilename,      OutputFilenameParameter);
OTB_DEFINE_PARAMETER_TRAITS(ParameterType_InputVectorData,     InputVectorDataParameter);
OTB_DEFINE_PARAMETER_TRAITS(ParameterType_OutputImage,         OutputImageParameter);
OTB_DEFINE_PARAMETER_TRAITS(ParameterType_OutputVectorData,    OutputVectorDataParameter);
OTB_DEFINE_PARAMETER_TRAITS(ParameterType_Float,               FloatParameter);
OTB_DEFINE_PARAMETER_TRAITS(ParameterType_Double,              DoubleParameter);
OTB_DEFINE_PARAMETER_TRAITS(ParameterType_Int,                 IntParameter);
OTB_DEFINE_PARAMETER_TRAITS(ParameterType_Radius,              RadiusParameter);
OTB_DEFINE_PARAMETER_TRAITS(ParameterType_StringList,          StringListParameter);
OTB_DEFINE_PARAMETER_TRAITS(ParameterType_InputVectorDataList, InputVectorDataListParameter);
OTB_DEFINE_PARAMETER_TRAITS(ParameterType_Bool,                BoolParameter);
OTB_DEFINE_PARAMETER_TRAITS(ParameterType_Field,               FieldParameter);
OTB_DEFINE_PARAMETER_TRAITS(ParameterType_Band,                BandParameter);
OTB_DEFINE_PARAMETER_TRAITS(ParameterType_String,              StringParameter);

#undef OTB_DEFINE_PARAMETER_TRAITS

// ====[ Then the cases where we cannot #define New for a single parameter
// ----[ InputImage
static_assert(ParameterTypeTraits<ParameterType_InputImage>::Type::Type == ParameterType_InputImage);
auto ParameterTypeTraits<ParameterType_InputImage>::New(
    bool mustReuseLoadedRegion,
    unsigned long streamHeight
)
-> Pointer
{
  auto res = Type::New(mustReuseLoadedRegion, streamHeight);
  res->Register();
  return res;
}

itk::SmartPointer<Parameter> ParameterTypeTraits<ParameterType_InputImage>::Upcast(Pointer & p)
{
  itk::SmartPointer<Parameter> res = p;
  res->UnRegister();
  return res;
}

auto ParameterTypeTraits<ParameterType_InputImage>::New(
    unsigned long streamHeight
)
-> Pointer
{
  auto res = Type::New(streamHeight);
  res->Register();
  return res;
}

// ----[ RAM
auto ParameterTypeTraits<ParameterType_RAM>::New()
-> Pointer
{
  auto res = Type::New();
  res->Register();
  return res;
}

itk::SmartPointer<Parameter> ParameterTypeTraits<ParameterType_RAM>::Upcast(Pointer & p)
{
  itk::SmartPointer<Parameter> res = p;
  res->UnRegister();
  return res;
}

} // otb::Wrapper namespace
