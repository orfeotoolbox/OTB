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
  ParameterType_InputComplexImage,
  ParameterType_InputVectorData,
  ParameterType_OutputImage,
  ParameterType_OutputImageList,
  ParameterType_OutputVectorData,
  ParameterType_Radius,
  ParameterType_Group,
} ParameterType;

typedef enum
{
  UserLevel_Basic,
  UserLevel_Advanced
} UserLevel;

typedef double                         PixelType;
typedef otb::VectorImage<PixelType, 2> VectorImageType;

typedef std::complex<float>            ComplexPixelType;
typedef otb::VectorImage<PixelType, 2> ComplexVectorImageType;

typedef double                                             VectorDataCoordinatePrecisionType;
typedef double                                             VectorDataValuePrecisionType;
typedef otb::VectorData <VectorDataCoordinatePrecisionType,
                         2, VectorDataValuePrecisionType>  VectorDataType;


} // end namespace Wrapper
} //end namespace otb

#endif // __otbWrapperApplication_h_
