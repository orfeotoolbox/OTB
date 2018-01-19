/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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
#include "itkRGBPixel.h"
#include "itkRGBAPixel.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbVectorData.h"

namespace otb
{
namespace Wrapper
{

/** Enumeration that list all of the parameter types that can be used
 * in an application. See Parameter for more information on the
 * parameter structure.
 */
typedef enum
{
  /** %Parameter holding a boolean. 
   * See EmptyParameter.
   */
  ParameterType_Empty,
  /** %Parameter holding an int. 
   * See NumericalParameter.
   */
  ParameterType_Int,
  /** %Parameter holding a float.
   * See NumericalParameter. 
   */
  ParameterType_Float,
  /** %Parameter holding a string.
   * See StringParameter. 
   */
  ParameterType_String,
  /** %Parameter holding a string list.
   * See StringListParameter. 
   */
  ParameterType_StringList,
  /** %Parameter holding a filename used for input. 
   * See InputFilenameParameter.
   */
  ParameterType_InputFilename,
  /** %Parameter holding a filename list used for input. 
   * See InputFilenameListParameter.
   */
  ParameterType_InputFilenameList,
  /** %Parameter holding a filename used for output. 
   * See OutputFilenameParameter.
   */
  ParameterType_OutputFilename,
  /** %Parameter holding a path. 
   * See DirectoryParameter.
   */
  ParameterType_Directory,
  /** %Parameter holding choice available for user. 
   * See ChoiceParameter.
   */
  ParameterType_Choice,
  /** %Parameter holding a image used for input. 
   * See InputImageParameter.
   */
  ParameterType_InputImage,
  /** %Parameter holding a image list used for input. 
   * See InputImageListParameter.
   */
  ParameterType_InputImageList,
  /** %Parameter holding a vector of data used for input. 
   * See InputVectorDataParameter.
   */
  ParameterType_InputVectorData,
  /** %Parameter holding a vector of data list used for input. 
   * See InputVectorDataListParameter.
   */
  ParameterType_InputVectorDataList,
  /** %Parameter holding a image used for output. 
   * See OutputImageParameter.
   */
  ParameterType_OutputImage,
  /** %Parameter holding a vector of data used for output. 
   * See OutputVectorDataParameter.
   */
  ParameterType_OutputVectorData,
  /** %Parameter holding a radius. 
   * See RadiusParameter.
   */
  ParameterType_Radius,
  /** %Parameter holding a group of parameter.
   * See GroupParameter. 
   */
  ParameterType_Group,
  /** %Parameter holding a list view.
   * See ListViewParameter.
   */
  ParameterType_ListView,
  /** %Parameter holding a complex image used for input.
   * See ComplexInputImageParameter. 
   */
  ParameterType_ComplexInputImage,
  /** %Parameter holding a complex image used for output.
   * See ComplexOutputImageParameter. 
   */
  ParameterType_ComplexOutputImage,
  /** %Parameter holding the ram parameter.
   * See RAMParameter.
   */
  ParameterType_RAM,
  /** %Parameter holding the xml used for output.
   * See OutputProcessXMLParameter.
   */
  ParameterType_OutputProcessXML,
  /** %Parameter holding the xml used for input.
   * See InputProcessXMLParameter.
   */
  ParameterType_InputProcessXML
} ParameterType;

typedef enum
{
  UserLevel_Basic,
  UserLevel_Advanced
} UserLevel;

/** Enumeration that define the numerical type of the pixel. */
typedef enum
{
  // Not int8 because not handle by Gdal
  ImagePixelType_uint8,
  ImagePixelType_int16,
  ImagePixelType_uint16,
  ImagePixelType_int32,
  ImagePixelType_uint32,
  ImagePixelType_float,
  ImagePixelType_double,
} ImagePixelType;

typedef enum
{
  ComplexImagePixelType_float,
  ComplexImagePixelType_double,
} ComplexImagePixelType;

typedef enum
{
  Role_Input = 0,
  Role_Output
} Role;

/** Enumeration used for the choice of elevation modes. 
 * See ElevationParametersHandler.
 */
typedef enum
{
  Elevation_DEM,
  Elevation_Average
  //Elevation_Tiff,
} ElevationType;


typedef otb::Image<unsigned char>              UInt8ImageType;
typedef otb::Image<short>                      Int16ImageType;
typedef otb::Image<unsigned short>             UInt16ImageType;
typedef otb::Image<int>                        Int32ImageType;
typedef otb::Image<unsigned int>               UInt32ImageType;
typedef otb::Image<float>                      FloatImageType;
typedef otb::Image<double>                     DoubleImageType;

typedef otb::VectorImage<unsigned char>  UInt8VectorImageType;
typedef otb::VectorImage<short>          Int16VectorImageType;
typedef otb::VectorImage<unsigned short> UInt16VectorImageType;
typedef otb::VectorImage<int>            Int32VectorImageType;
typedef otb::VectorImage<unsigned int>   UInt32VectorImageType;
typedef otb::VectorImage<float>          FloatVectorImageType;
typedef otb::VectorImage<double>         DoubleVectorImageType;

typedef otb::Image< itk::RGBPixel<unsigned char> >  UInt8RGBImageType;
typedef otb::Image< itk::RGBAPixel<unsigned char> > UInt8RGBAImageType;

typedef std::complex<float>  FloatComplexPixelType;
typedef std::complex<double> DoubleComplexPixelType;

// Complex Image Type (first : double and float)
typedef otb::Image< FloatComplexPixelType, 2 >  ComplexFloatImageType;
typedef otb::Image< DoubleComplexPixelType, 2 > ComplexDoubleImageType;

typedef otb::VectorImage<FloatComplexPixelType, 2 >   ComplexFloatVectorImageType;
typedef otb::VectorImage< DoubleComplexPixelType, 2 > ComplexDoubleVectorImageType;


typedef double VectorDataCoordinatePrecisionType;
typedef double VectorDataValuePrecisionType;
typedef otb::VectorData<VectorDataCoordinatePrecisionType,
                        2,
                        VectorDataValuePrecisionType>
           VectorDataType;
typedef otb::ObjectList<VectorDataType> VectorDataListType;

typedef otb::ObjectList<FloatVectorImageType> FloatVectorImageListType;
typedef otb::ObjectList<FloatImageType>       FloatImageListType;


} // end namespace Wrapper
} // end namespace otb

#endif // otbWrapperApplication_h_
