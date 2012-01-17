/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#ifndef __otbWrapperTypes_h
#define __otbWrapperTypes_h

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

typedef enum
{
  ParameterType_Empty,
  ParameterType_Int,
  ParameterType_Float,
  ParameterType_String,
  ParameterType_StringList,
  ParameterType_Filename,
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
  ParameterType_ComplexInputImage,
  ParameterType_ComplexOutputImage,
  ParameterType_RAM
} ParameterType;

typedef enum
{
  UserLevel_Basic,
  UserLevel_Advanced
} UserLevel;

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
  Role_Input,
  Role_Output
} Role;

// enum used for the map projection choices
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

#endif // __otbWrapperApplication_h_
