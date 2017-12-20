/*
 * Copyright (C) 2005-2017 CS Systemes d'Information (CS SI)
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


%module otbApplication

 %{
#include "itkBase.includes"
#include "otbWrapperSWIGIncludes.h"
#include <string>         // std::string
#include <locale>
#define SWIG_FILE_WITH_INIT
%}

// Language specific extension
%include "Python.i"
%include "Java.i"
%include "Ruby.i"
%include "Lua.i"
%include "itkMacro.i"
%include "itkBase.i"

#if OTB_SWIGNUMPY
%include "numpy.i"

%init
%{
import_array();
%}

/*leave the mess to SWIG and let us not worry.*/
%apply (signed char* INPLACE_ARRAY3, int DIM1, int DIM2, int DIM3) {(signed char* buffer, int dim1, int dim2, int dim3)};
%apply (signed short* INPLACE_ARRAY3, int DIM1, int DIM2, int DIM3) {(signed short* buffer, int dim1, int dim2, int dim3)};
%apply (signed int* INPLACE_ARRAY3, int DIM1, int DIM2, int DIM3) {(signed int* buffer, int dim1, int dim2, int dim3)};
%apply (signed long* INPLACE_ARRAY3, int DIM1, int DIM2, int DIM3) {(signed long* buffer, int dim1, int dim2, int dim3)};
%apply (float* INPLACE_ARRAY3, int DIM1, int DIM2, int DIM3) {(float* buffer, int dim1, int dim2, int dim3)};
%apply (unsigned char* INPLACE_ARRAY3, int DIM1, int DIM2, int DIM3) {(unsigned char* buffer, int dim1, int dim2, int dim3)};
%apply (unsigned short* INPLACE_ARRAY3, int DIM1, int DIM2, int DIM3) {(unsigned short* buffer, int dim1, int dim2, int dim3)};
%apply (unsigned int* INPLACE_ARRAY3, int DIM1, int DIM2, int DIM3) {(unsigned int* buffer, int dim1, int dim2, int dim3)};
%apply (unsigned long* INPLACE_ARRAY3, int DIM1, int DIM2, int DIM3) {(unsigned long* buffer, int dim1, int dim2, int dim3)};
%apply (double* INPLACE_ARRAY3, int DIM1, int DIM2, int DIM3) {(double* buffer, int dim1, int dim2, int dim3)};

%apply (signed char** ARGOUTVIEW_ARRAY3, int *DIM1, int *DIM2, int *DIM3) {(signed char** buffer, int *dim1, int *dim2, int *dim3)};
%apply (signed short** ARGOUTVIEW_ARRAY3, int *DIM1, int *DIM2, int *DIM3) {(signed short** buffer, int *dim1, int *dim2, int *dim3)};
%apply (signed int** ARGOUTVIEW_ARRAY3, int *DIM1, int *DIM2, int *DIM3) {(signed int** buffer, int *dim1, int *dim2, int *dim3)};
%apply (signed long** ARGOUTVIEW_ARRAY3, int *DIM1, int *DIM2, int *DIM3) {(signed long** buffer, int *dim1, int *dim2, int *dim3)};
%apply (float** ARGOUTVIEW_ARRAY3, int *DIM1, int *DIM2, int *DIM3) {(float** buffer, int *dim1, int *dim2, int *dim3)};
%apply (unsigned char** ARGOUTVIEW_ARRAY3, int *DIM1, int *DIM2, int *DIM3) {(unsigned char** buffer, int *dim1, int *dim2, int *dim3)};
%apply (unsigned short** ARGOUTVIEW_ARRAY3, int *DIM1, int *DIM2, int *DIM3) {(unsigned short** buffer, int *dim1, int *dim2, int *dim3)};
%apply (unsigned int** ARGOUTVIEW_ARRAY3, int *DIM1, int *DIM2, int *DIM3) {(unsigned int** buffer, int *dim1, int *dim2, int *dim3)};
%apply (unsigned long** ARGOUTVIEW_ARRAY3, int *DIM1, int *DIM2, int *DIM3) {(unsigned long** buffer, int *dim1, int *dim2, int *dim3)};
%apply (double** ARGOUTVIEW_ARRAY3, int *DIM1, int *DIM2, int *DIM3) {(double** buffer, int *dim1, int *dim2, int *dim3)};

#endif /* OTB_SWIGNUMPY */

namespace otb
{
namespace Wrapper
{
  enum DefaultValueMode
  {
    DefaultValueMode_UNKNOWN,
    DefaultValueMode_RELATIVE,
    DefaultValueMode_ABSOLUTE
  };

  typedef enum
  {
    ParameterType_Empty,
    ParameterType_Int,
    ParameterType_Float,
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
    ParameterType_ComplexInputImage,
    ParameterType_ComplexOutputImage,
    ParameterType_RAM,
    ParameterType_OutputProcessXML,
    ParameterType_InputProcessXML
  } ParameterType;

  typedef enum
  {
    UserLevel_Basic,
    UserLevel_Advanced
  } UserLevel;

  typedef enum
  {
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
    Role_Input = 0,
    Role_Output
  } Role;

  typedef enum
  {
    ComplexImagePixelType_float,
    ComplexImagePixelType_double,
  } ComplexImagePixelType;

} /* end namespace wapper */

} /* end namespace otb */

%include "cDeclApplication.i"

    /* Int8 Int16 Int32 Int64 */
    /* UInt8 UInt16 UInt32 UInt64 */
    /* Float32 Double64 */
    /* Complex32 Complex64 */

    /* typedef signed char       Int8; */
    /* typedef signed short      Int16; */
    /* typedef signed int        Int32; */
    /* typedef signed long      Int64; */
    /* typedef unsigned char     UInt8; */
    /* typedef unsigned short    UInt16; */
    /* typedef unsigned int      UInt32; */
    /* typedef unsigned long     UInt64; */
    /* typedef float Float32; */
    /* typedef double Float64; */

#if SWIGPYTHON
%include "pyApplicationProxy.i"
#endif

#if SWIGPYTHON
/*Maybe TODO: change use a dict to  GetParameterTypeAsString */
%include "pyExtensionApplication.i"

#endif


#if OTB_SWIGNUMPY
%include "pyNumpyApplicationInterface.i"

#endif /* OTB_SWIGNUMPY */

%include "cDeclItkClass.i"

%include "PyCommand.i"
