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
%include docString.i
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

%include otbWrapperType.i

/*This code is the declaration of methods of the application class*/

class Application: public itkObject
{
public:
  virtual void SetName( const std::string & name );
  itkGetStringMacro(Name);

  itkSetStringMacro(Description);
  itkGetStringMacro(Description);

  void Init();
  void UpdateParameters();
  int Execute();
  int ExecuteAndWriteOutput();

  std::vector<std::string> GetParametersKeys(bool recursive = true);
  Parameter* Application::GetParameterByKey(std::string name);
  std::string GetParameterName(std::string);
  std::string GetParameterDescription(std::string);
  void SetParameterDescription(std::string paramKey, std::string dec);
  void SetParameterUserValue(std::string paramKey, bool value);


  void EnableParameter(std::string paramKey);
  void DisableParameter(std::string paramKey);
  bool IsParameterEnabled(std::string paramKey) const;
  bool IsMandatory(std::string paramKey) const;
  bool HasAutomaticValue(std::string paramKey) const;
  bool HasUserValue(std::string paramKey) const;
  void ClearValue(std::string paramKey);
  bool HasValue(std::string paramKey) const;
  otb::Wrapper::UserLevel GetParameterUserLevel(std::string paramKey) const;
  otb::Wrapper::ParameterType GetParameterType(std::string paramKey) const;
  otb::Wrapper::Role GetParameterRole(std::string paramKey) const;

  std::vector<std::string> GetChoiceKeys(std::string choiceKey);
  std::vector<std::string> GetChoiceNames(std::string choiceKey);

  bool IsApplicationReady();

  void SetParameterInt(std::string parameter, int value, bool hasUserValueFlag = true);
  void SetParameterFloat(std::string parameter, float value, bool hasUserValueFlag = true);
  void SetParameterString(std::string parameter, std::string value, bool hasUserValueFlag = true);
  void SetParameterStringList(std::string parameter, std::vector<std::string> values, bool hasUserValueFlag = true);
  void SetParameterEmpty(std::string parameter, bool value, bool hasUserValueFlag = true);

  void SetParameterOutputImagePixelType(std::string parameter, otb::Wrapper::ImagePixelType pixelType);
  void SetParameterComplexOutputImagePixelType(std::string parameter, otb::Wrapper::ComplexImagePixelType cpixelType);

  otb::Wrapper::ImagePixelType GetParameterOutputImagePixelType(std::string parameter);
  otb::Wrapper::ComplexImagePixelType GetParameterComplexOutputImagePixelType(std::string parameter);

  int GetParameterInt(std::string parameter);
  float GetParameterFloat(std::string parameter);
  std::string GetParameterString(std::string parameter);
  std::vector<std::string> GetParameterStringList(std::string parameter);
  std::string GetParameterAsString(std::string paramKey);

  InputImageParameter::ImageBaseType * GetParameterOutputImage(std::string parameter);
  void SetParameterInputImage(std::string parameter, InputImageParameter::ImageBaseType * inputImage);
  ComplexInputImageParameter::ImageBaseType * GetParameterComplexOutputImage(std::string parameter);
  void SetParameterComplexInputImage(std::string parameter, ComplexInputImageParameter::ImageBaseType * inputImage);
  void AddImageToParameterInputImageList(std::string parameter,InputImageParameter::ImageBaseType * img);
  void AddParameterStringList(std::string parameter,const std::string & str);
  void SetNthParameterInputImageList(std::string parameter, const unsigned int &id, InputImageParameter::ImageBaseType * img);
  void SetNthParameterStringList(std::string parameter, const unsigned int &id, const std::string& str);
  void ClearParameterInputImageList(std::string parameter);
  unsigned int GetNumberOfElementsInParameterInputImageList(std::string parameter);



  itkProcessObject* GetProgressSource() const;

  std::string GetProgressDescription() const;

  itkSetStringMacro(DocName);
  itkGetStringMacro(DocName);
  itkSetStringMacro(DocLongDescription);
  itkGetStringMacro(DocLongDescription);
  itkSetStringMacro(DocAuthors);
  itkGetStringMacro(DocAuthors);
  itkSetStringMacro(DocLimitations);
  itkGetStringMacro(DocLimitations);
  itkSetStringMacro(DocSeeAlso);
  itkGetStringMacro(DocSeeAlso);
  void SetDocTags( std::vector<std::string> val );
  void AddDocTag( const std::string & tag );
  std::vector<std::string> GetDocTags();

  otb::Wrapper::ParameterGroup* GetParameterList();

  unsigned int GetNumberOfExamples();
  std::string GetExampleComment(unsigned int id);
  unsigned int GetExampleNumberOfParameters(unsigned int id);
  std::string GetExampleParameterKey(unsigned int exId, unsigned int paramId);
  std::string GetExampleParameterValue(unsigned int exId, unsigned int paramId);

  void SetDocExampleParameterValue( const std::string key, const std::string value, unsigned int exId=0 );
  void SetExampleComment( const std::string & comm, unsigned int i );
  unsigned int AddExample( const std::string & comm="" );

  std::string GetCLExample();
  std::string GetHtmlExample();
  std::vector< std::pair<std::string, std::string> > GetOutputParametersSumUp();

#if OTB_SWIGNUMPY
%extend {

#define SetFromNumpyArrayMacro(prefix, PixelDataType, ImageClass)       \
      void Set##ImageClass##From##prefix##NumpyArray_(std::string pkey, ##PixelDataType##* buffer, int dim1, int dim2, int dim3) \
      {                                                                 \
        otb::Wrapper::Parameter *parameter = $self->GetParameterList()->GetParameterByKey(pkey); \
        InputImageParameter* inputImageParam = dynamic_cast<InputImageParameter*>(parameter); \
        typedef otb::##ImageClass##<##PixelDataType##>   ImageType;     \
        ImageType::Pointer output = ImageType::New();          \
        typedef ImageType::SizeType        SizeType;           \
        typedef ImageType::IndexType       IndexType;          \
        typedef ImageType::RegionType      RegionType;         \
        typedef ImageType::PointType       PointType;          \
        typedef ImageType::SpacingType     SpacingType;        \
        typedef ImageType::DirectionType   DirectionType;      \
        IndexType start;                                                \
        DirectionType direction;                                        \
        start.Fill( 0 );                                                \
        SizeType size;                                                  \
        size[0] = dim2; size[1] = dim1;                                 \
        SetVectorLengthMacro                                            \
        output->Allocate();                                             \
        unsigned int numberOfPixels = dim1 * dim2 * dim3;               \
        RegionType region;                                              \
        region.SetIndex( start );                                       \
        region.SetSize( size );                                         \
        PointType origin;                                               \
        origin.Fill( 0.0 );                                             \
        SpacingType spacing;                                            \
        spacing.Fill( 1.0 );                                            \
        direction.SetIdentity();                                        \
        output->SetOrigin( origin );                                    \
        output->SetSignedSpacing( spacing );                                  \
        output->SetDirection(direction);                                \
        output->SetLargestPossibleRegion(region);                       \
        output->SetRequestedRegion(output->GetLargestPossibleRegion()); \
        output->SetBufferedRegion(output->GetLargestPossibleRegion());  \
        output->GetPixelContainer()->SetImportPointer(buffer, numberOfPixels, false); \
        inputImageParam->SetImage<ImageType>(output);                   \
  }

#define SetVectorLengthMacro output->SetVectorLength(dim3);
       SetFromNumpyArrayMacro(Float, float, VectorImage)
       SetFromNumpyArrayMacro(Int8, signed char, VectorImage)
       SetFromNumpyArrayMacro(Int16, signed short, VectorImage)
       SetFromNumpyArrayMacro(Int32, signed int, VectorImage)
       SetFromNumpyArrayMacro(Int64, signed long, VectorImage)
       SetFromNumpyArrayMacro(UInt8, unsigned char, VectorImage)
       SetFromNumpyArrayMacro(UInt16, unsigned short, VectorImage)
       SetFromNumpyArrayMacro(UInt32, unsigned int, VectorImage)
       SetFromNumpyArrayMacro(UInt64, unsigned long, VectorImage)
       SetFromNumpyArrayMacro(Double, double, VectorImage)
#undef SetVectorLengthMacro

#define SetVectorLengthMacro dim3=1;
       SetFromNumpyArrayMacro(Float, float, Image)
       SetFromNumpyArrayMacro(Int8, signed char, Image)
       SetFromNumpyArrayMacro(Int16, signed short, Image)
       SetFromNumpyArrayMacro(Int32, signed int, Image)
       SetFromNumpyArrayMacro(Int64, signed long, Image)
       SetFromNumpyArrayMacro(UInt8, unsigned char, Image)
       SetFromNumpyArrayMacro(UInt16, unsigned short, Image)
       SetFromNumpyArrayMacro(UInt32, unsigned int, Image)
       SetFromNumpyArrayMacro(UInt64, unsigned long, Image)
       SetFromNumpyArrayMacro(Double, double, Image)
#undef SetVectorLengthMacro
#undef SetFromNumpyArrayMacro

#define GetVectorImageAsNumpyArrayMacro(prefix, PixelType)                    \
      void GetVectorImageAs##prefix##NumpyArray_(std::string pkey, ##PixelType##** buffer, int *dim1, int *dim2, int *dim3) \
        {                                                               \
        otb::Wrapper::Parameter *parameter = $self->GetParameterList()->GetParameterByKey(pkey); \
        OutputImageParameter* outputImageParam = dynamic_cast<OutputImageParameter*>(parameter); \
        typedef itk::ImageBase<2> ImageBaseType;                        \
        typedef ImageBaseType::RegionType RegionType;          \
        ImageBaseType::Pointer imageBase;                               \
        imageBase = outputImageParam->GetValue();                       \
        imageBase->Update();                                            \
        typedef ImageBaseType::SizeType        SizeType;       \
        typedef ImageBaseType::IndexType       IndexType;      \
        typedef ImageBaseType::PointType       PointType;      \
        typedef ImageBaseType::SpacingType     SpacingType;    \
        RegionType region = imageBase->GetBufferedRegion();             \
        SizeType size =  region.GetSize();                              \
        *dim1 = size[1];                                                \
        *dim2 = size[0];                                                \
        typedef otb::VectorImage<signed char> Int8ImageType;            \
        typedef otb::VectorImage<signed short> Int16ImageType;          \
        typedef otb::VectorImage<signed int> Int32ImageType;            \
        typedef otb::VectorImage<unsigned char> UInt8ImageType;         \
        typedef otb::VectorImage<unsigned short> UInt16ImageType;       \
        typedef otb::VectorImage<unsigned int> UInt32ImageType;         \
        typedef otb::VectorImage<float> FloatImageType;                 \
        typedef otb::VectorImage<double> DoubleImageType;               \
        if (dynamic_cast<UInt8ImageType*>(imageBase.GetPointer()))      \
          {                                                             \
            UInt8ImageType* output = dynamic_cast<UInt8ImageType*>(imageBase.GetPointer()); \
            *buffer  =  reinterpret_cast<##PixelType##*>(output->GetBufferPointer()); \
              *dim3 = output->GetNumberOfComponentsPerPixel();          \
          }                                                             \
        else if (dynamic_cast<Int16ImageType*>(imageBase.GetPointer())) \
          {                                                             \
            Int16ImageType* output = dynamic_cast<Int16ImageType*>(imageBase.GetPointer()); \
            *buffer  =  reinterpret_cast<##PixelType##*>(output->GetBufferPointer()); \
              *dim3 = output->GetNumberOfComponentsPerPixel();          \
          }                                                             \
        else if (dynamic_cast<UInt16ImageType*>(imageBase.GetPointer())) \
          {                                                             \
            UInt16ImageType* output = dynamic_cast<UInt16ImageType*>(imageBase.GetPointer()); \
            *buffer  =  reinterpret_cast<##PixelType##*>(output->GetBufferPointer()); \
              *dim3 = output->GetNumberOfComponentsPerPixel();          \
          }                                                             \
        else if (dynamic_cast<Int32ImageType*>(imageBase.GetPointer())) \
          {                                                             \
            Int32ImageType* output = dynamic_cast<Int32ImageType*>(imageBase.GetPointer()); \
            *buffer  =  reinterpret_cast<##PixelType##*>(output->GetBufferPointer()); \
              *dim3 = output->GetNumberOfComponentsPerPixel();          \
          }                                                             \
        else if (dynamic_cast<UInt32ImageType*>(imageBase.GetPointer())) \
          {                                                             \
            UInt32ImageType* output = dynamic_cast<UInt32ImageType*>(imageBase.GetPointer()); \
            *buffer = reinterpret_cast<##PixelType##*>(output->GetBufferPointer()); \
              *dim3 = output->GetNumberOfComponentsPerPixel();          \
          }                                                             \
        else if (dynamic_cast<FloatImageType*>(imageBase.GetPointer())) \
          {                                                             \
            FloatImageType* output = dynamic_cast<FloatImageType*>(imageBase.GetPointer()); \
            *buffer  =  reinterpret_cast<##PixelType##*>(output->GetBufferPointer()); \
              *dim3 = output->GetNumberOfComponentsPerPixel();          \
          }                                                             \
        else if (dynamic_cast<DoubleImageType*>(imageBase.GetPointer())) \
          {                                                             \
            DoubleImageType* output = dynamic_cast<DoubleImageType*>(imageBase.GetPointer()); \
            *buffer  =  reinterpret_cast<##PixelType##*>(output->GetBufferPointer()); \
              *dim3 = output->GetNumberOfComponentsPerPixel();          \
          }                                                             \
        else                                                            \
          {                                                             \
            std::cerr << "unknown image type. cannot make a numpy array" << std::endl; \
          }                                                             \
      }

       GetVectorImageAsNumpyArrayMacro(Float, float)
       GetVectorImageAsNumpyArrayMacro(Int16, signed short)
       GetVectorImageAsNumpyArrayMacro(Int32, signed int)
       GetVectorImageAsNumpyArrayMacro(UInt8, unsigned char)
       GetVectorImageAsNumpyArrayMacro(UInt16, unsigned short)
       GetVectorImageAsNumpyArrayMacro(UInt32, unsigned int)
       GetVectorImageAsNumpyArrayMacro(Double, double)
#undef GetVectorImageAsNumpyArrayMacro

} /* end of %extend */
#endif /* OTB_SWIGNUMPY */

protected:
  Application();
#if SWIGJAVA
  virtual ~Application();
#endif
private:
  Application(const Application &);
  void operator =(const Application&);
};

DECLARE_REF_COUNT_CLASS( Application )

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

%include "otbRegistry.i"
%include "otbAddProcessToWatchEvent.i"

%include "PyCommand.i"
