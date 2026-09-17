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
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbWrapperTypes_h
#define otbWrapperTypes_h

#include <complex>
// #include <array>
#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbVectorData.h"

namespace otb
{
namespace Wrapper
{

enum ParameterType {
  ParameterType_Int,
  ParameterType_Float,
  ParameterType_Double,
  ParameterType_String,
  ParameterType_StringList,
  ParameterType_InputFilename,
  ParameterType_InputFilenameList,
  ParameterType_OutputFilename,
  ParameterType_Directory,
  ParameterType_Choice,
  ParameterType_InputImage,
  ParameterType_InputImageList,
  ParameterType_InputVectorData,
  ParameterType_InputVectorDataList,
  ParameterType_OutputImage,
  ParameterType_OutputVectorData,
  ParameterType_Radius,
  ParameterType_Group,
  ParameterType_ListView,
  ParameterType_RAM,
  ParameterType_OutputProcessXML,
  ParameterType_InputProcessXML,
  ParameterType_Bool,
  ParameterType_Field,
  ParameterType_Band,
  ParameterType_MAX__
};

namespace
{
constexpr char const* parameterTypesStrings[] = {"Int",
                                                 "Float",
                                                 "Double",
                                                 "String",
                                                 "StringList",
                                                 "InputFilename",
                                                 "InputFilenameList",
                                                 "OutputFilename",
                                                 "Directory",
                                                 "Choice",
                                                 "InputImage",
                                                 "InputImageList",
                                                 "InputVectorData",
                                                 "InputVectorDataList",
                                                 "OutputImage",
                                                 "OutputVectorData",
                                                 "Radius",
                                                 "Group",
                                                 "ListView",
                                                 "RAM",
                                                 "OutputProcessXML",
                                                 "InputProcessXML",
                                                 "Bool",
                                                 "Field",
                                                 "Band",

};
static_assert(std::extent<decltype(parameterTypesStrings)>::value == ParameterType_MAX__, "Wrong number of parameters in parameterTypesStrings");
}

// Free functions to convert from and to std::string
std::string ParameterTypeToString(ParameterType type);
ParameterType ParameterStringToType(const std::string& str);

enum UserLevel { UserLevel_Basic, UserLevel_Advanced };

enum ImagePixelType {
  // Not int8 because not handle by Gdal
  ImagePixelType_uint8,
  ImagePixelType_int16,
  ImagePixelType_uint16,
  ImagePixelType_int32,
  ImagePixelType_uint32,
  ImagePixelType_float,
  ImagePixelType_double,
  ImagePixelType_cint16,
  ImagePixelType_cint32,
  ImagePixelType_cfloat,
  ImagePixelType_cdouble,
};

enum ComplexImagePixelType {
  ComplexImagePixelType_int16,
  ComplexImagePixelType_int32,
  ComplexImagePixelType_float,
  ComplexImagePixelType_double,
};

enum Role { Role_Input = 0, Role_Output };

// enum used for the map projection choices
enum ElevationType {
  Elevation_DEM,
  Elevation_Average
  // Elevation_Tiff,
};


using UInt8ImageType                    = otb::Image<unsigned char>;
using Int16ImageType                    = otb::Image<short>;
using UInt16ImageType                   = otb::Image<unsigned short>;
using Int32ImageType                    = otb::Image<int>;
using UInt32ImageType                   = otb::Image<unsigned int>;
using FloatImageType                    = otb::Image<float>;
using DoubleImageType                   = otb::Image<double>;

using UInt8VectorImageType              = otb::VectorImage<unsigned char>;
using Int16VectorImageType              = otb::VectorImage<short>;
using UInt16VectorImageType             = otb::VectorImage<unsigned short>;
using Int32VectorImageType              = otb::VectorImage<int>;
using UInt32VectorImageType             = otb::VectorImage<unsigned int>;
using FloatVectorImageType              = otb::VectorImage<float>;
using DoubleVectorImageType             = otb::VectorImage<double>;

using UInt8RGBImageType                 = otb::Image<itk::RGBPixel<unsigned char>>;
using UInt8RGBAImageType                = otb::Image<itk::RGBAPixel<unsigned char>>;

using Int16ComplexPixelType             = std::complex<short>;
using Int32ComplexPixelType             = std::complex<int>;
using FloatComplexPixelType             = std::complex<float>;
using DoubleComplexPixelType            = std::complex<double>;

// Complex Image Type
using ComplexInt16ImageType             = otb::Image<Int16ComplexPixelType, 2>;
using ComplexInt32ImageType             = otb::Image<Int32ComplexPixelType, 2>;
using ComplexFloatImageType             = otb::Image<FloatComplexPixelType, 2>;
using ComplexDoubleImageType            = otb::Image<DoubleComplexPixelType, 2>;

using ComplexInt16VectorImageType       = otb::VectorImage<Int16ComplexPixelType, 2>;
using ComplexInt32VectorImageType       = otb::VectorImage<Int32ComplexPixelType, 2>;
using ComplexFloatVectorImageType       = otb::VectorImage<FloatComplexPixelType, 2>;
using ComplexDoubleVectorImageType      = otb::VectorImage<DoubleComplexPixelType, 2>;


using VectorDataCoordinatePrecisionType = double;
using VectorDataValuePrecisionType      = double;
using VectorDataType                    = otb::VectorData<VectorDataCoordinatePrecisionType, 2, VectorDataValuePrecisionType>;
using VectorDataListType                = otb::ObjectList<VectorDataType>;

using FloatVectorImageListType          = otb::ObjectList<FloatVectorImageType>;
using FloatImageListType                = otb::ObjectList<FloatImageType>;

using ImageBaseType                     = itk::ImageBase<2>;


} // end namespace Wrapper
} // end namespace otb

#endif // otbWrapperApplication_h_
