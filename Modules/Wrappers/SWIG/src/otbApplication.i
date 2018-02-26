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

%include "std_map.i"

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

}

}

namespace std {
%template(keywordlist) map<string,string>;

#if SWIGPYTHON
%extend map<string,string>
{
  %pythoncode
  {
    def __str__(self):
      ret = "{"
      for key in self:
        ret += str(key)+":"+str(self[key])+", "
      if len(ret) == 1:
        ret += ", "
      return ret[:-2]+"}"
  }
};
#endif
} // end of namespace std

// BASIC ITK TYPES WRAPPING
%include "itkFloatTypes.h"
%include "itkIntTypes.h"

namespace itk
{

template <unsigned int VDim = 2>
class Size
{
public:
  Size();
  virtual ~Size();
  void Fill(unsigned long val);
  SizeValueType GetElement(unsigned long element) const;
  void SetElement(unsigned long element, SizeValueType val);
  static unsigned int GetSizeDimension();
};

template <unsigned int VDim = 2>
class Index
{
public:
  Index();
  virtual ~Index();
  void Fill(signed long val);
  IndexValueType GetElement(unsigned long element) const;
  void SetElement(unsigned long element, IndexValueType val);
  static unsigned int GetIndexDimension();
};

template <unsigned int VDim>
class ImageRegion
{
public:
  ImageRegion();
  ImageRegion(const Index<VDim> &index, const Size<VDim> &size);
  virtual ~ImageRegion();
  void SetIndex(const Index<VDim> &index);
  void SetSize(const Size<VDim> &size);
  void SetUpperIndex(const Index<VDim> &idx);
  Index<VDim> GetUpperIndex() const;
  const Index<VDim> & GetIndex() const;
  const Size<VDim> & GetSize() const;
  bool IsInside(const Index<VDim> & index) const;
  void SetSize(unsigned int i, SizeValueType val);
  SizeValueType GetSize(unsigned int i) const;
  void SetIndex(unsigned int i, IndexValueType val);
  IndexValueType GetIndex(unsigned int i) const;
};

template <typename TValue, unsigned int VLength = 3>
class FixedArray
{
public:
  FixedArray();
  virtual ~FixedArray();
  unsigned int Size();
  void SetElement(unsigned short idx, const TValue &val);
  const TValue & GetElement(unsigned short idx);
};

template <typename TValue, unsigned int NDim = 3>
class Vector: public FixedArray<TValue,NDim>
{
public: 
  Vector();
  virtual ~Vector();
  typedef NumericTraits<TValue>::RealType RealValueType;
  RealValueType GetNorm() const;
  RealValueType GetSquaredNorm() const;
  RealValueType Normalize();
};

template <typename TCoord, unsigned int NDim = 3>
class Point: public FixedArray<TCoord,NDim>
{
public:
  Point();
  virtual ~Point();
};


// Instanciate the needed templates
%template(itkSize) Size<2>;
%template(itkIndex) Index<2>;
%template(itkRegion) ImageRegion<2>;
%template(itkFixedArray) FixedArray<SpacePrecisionType,2>;
%template(itkVector) Vector<SpacePrecisionType,2>;
%template(itkPoint) Point<SpacePrecisionType,2>;

} // end of namespace itk

#if SWIGPYTHON

%define WRAP_AS_LIST(N, T...)
%extend T
  {
  %pythoncode
    {
    def __str__(self):
      ret = "["
      for index in range(N):
        ret += str(self.GetElement(index))+","
      ret = ret[:-1] + "]"
      return ret
    def __len__(self):
      return N
    def __getitem__(self,idx):
      if idx >= N or idx < 0:
        raise IndexError('Index outside [0,'+str(N-1)+']')
      return self.GetElement(idx)
    def __setitem__(self,idx,val):
      if idx >= N or idx < 0:
        raise IndexError('Index outside [0,'+str(N-1)+']')
      return self.SetElement(idx,val)
    }
  };
%enddef

namespace itk
{
WRAP_AS_LIST(2, Size<2>)
WRAP_AS_LIST(2, Index<2>)
WRAP_AS_LIST(2, FixedArray<SpacePrecisionType,2>)
WRAP_AS_LIST(2, Vector<SpacePrecisionType,2>)
WRAP_AS_LIST(2, Point<SpacePrecisionType,2>)

%extend ImageRegion<2>
{
  %pythoncode
    {
    def __str__(self):
      return "{index:"+str(self.GetIndex())+", size:"+str(self.GetSize())+"}"
    def __len__(self):
      return 2
    def __getitem__(self,key):
      if key == 'index':
        return self.GetIndex()
      elif key == 'size':
        return self.GetSize()
      else:
        raise IndexError('Key not in ["index","size"]')
    def __setitem__(self,key,val):
      if key == 'index':
        self.SetIndex(val)
      elif key == 'size':
        self.SetSize(val)
      else:
        raise IndexError('Key not in ["index","size"]')
    }
};
} // end of namespace itk
#endif

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

  ImageBaseType * GetParameterOutputImage(std::string parameter);
  void SetParameterInputImage(std::string parameter, ImageBaseType * inputImage);
  ImageBaseType * GetParameterComplexOutputImage(std::string parameter);
  void SetParameterComplexInputImage(std::string parameter, ImageBaseType * inputImage);
  void AddImageToParameterInputImageList(std::string parameter,ImageBaseType * img);
  void AddParameterStringList(std::string parameter,const std::string & str);
  void SetNthParameterInputImageList(std::string parameter, const unsigned int &id, ImageBaseType * img);
  void SetNthParameterStringList(std::string parameter, const unsigned int &id, const std::string& str);
  void ClearParameterInputImageList(std::string parameter);
  unsigned int GetNumberOfElementsInParameterInputImageList(std::string parameter);

  itk::Point<SpacePrecisionType,2> GetImageOrigin(const std::string & key, unsigned int idx = 0);
  itk::Vector<SpacePrecisionType,2> GetImageSpacing(const std::string & key, unsigned int idx = 0);
  itk::Size<2> GetImageSize(const std::string & key, unsigned int idx = 0);
  unsigned int GetImageNbBands(const std::string & key, unsigned int idx = 0);
  std::string GetImageProjection(const std::string & key, unsigned int idx = 0);
  std::map<std::string,std::string> GetImageKeywordlist(const std::string & key, unsigned int idx = 0);
  unsigned long PropagateRequestedRegion(const std::string & key, itk::ImageRegion<2> region, unsigned int idx = 0);
  itk::ImageRegion<2> GetImageRequestedRegion(const std::string & key, unsigned int idx = 0);

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
%pythoncode {
import sys

class ApplicationProxy(object):
  def __init__(self, application, groupkey, value = None):
    self.__dict__["application"] = application
    self.__dict__["groupkey"] = groupkey
    if value is not None:
      self.__dict__["application"].SetParameterString(groupkey, value)

  def __str__(self):
    return self.__dict__["application"].GetParameterAsString(self.groupkey)

  def __eq__(self, other):
    if not type(other) == type(self):
      return (self.__str__() == other)
    else:
      return (isinstance(other, self.__class__) and self.__dict__ == other.__dict__)

  def __ne__(self, other):
    return not self.__eq__(other)

  def __getattr__(self,attr):
    return self.__dict__["application"].GetParameterValue( self.groupkey + "." + attr.lower() )

  def __setattr__(self,attr,value):
    if attr not in self.__dict__:
      return self.__dict__["application"].SetParameterValue( self.groupkey + "." + attr.lower(), value )
    else:
      return dict.__setattr__(self, attr, value)

}
#endif

#if SWIGPYTHON
/*Maybe TODO: change use a dict to  GetParameterTypeAsString */

%extend Application
{
  %pythoncode
    {

		def __str__(self):
			s  = self.GetDocName()

		def GetParameterTypeAsString(self, parameter_type):
			return {
				ParameterType_InputProcessXML : 'ParameterType_InputProcessXML',
				ParameterType_String : 'ParameterType_String',
				ParameterType_InputFilename : 'ParameterType_InputFilename',
				ParameterType_OutputImage : 'ParameterType_OutputImage',
				ParameterType_OutputVectorData : 'ParameterType_OutputVectorData',
				ParameterType_OutputProcessXML : 'ParameterType_OutputProcessXML',
				ParameterType_OutputFilename : 'ParameterType_OutputFilename',
				ParameterType_Directory : 'ParameterType_Directory',
				ParameterType_InputImage : 'ParameterType_InputImage',
				ParameterType_ComplexInputImage : 'ParameterType_ComplexInputImage',
				ParameterType_InputVectorData : 'ParameterType_InputVectorData',
				ParameterType_InputImageList : 'ParameterType_InputImageList',
				ParameterType_InputVectorDataList : 'ParameterType_InputImageList',
				ParameterType_InputFilenameList : 'ParameterType_InputFilenameList',
				ParameterType_StringList : 'ParameterType_StringList',
				ParameterType_ListView : 'ParameterType_ListView',
				ParameterType_Int : 'ParameterType_Int',
				ParameterType_Radius : 'ParameterType_Radius',
				ParameterType_RAM : 'ParameterType_RAM',
				ParameterType_Float : 'ParameterType_Float',
				ParameterType_Empty : 'ParameterType_Empty',
				ParameterType_Choice : 'ParameterType_Choice',
				ParameterType_Group : 'ParameterType_Group',
			}.get(parameter_type, 'ParameterType_UNKNOWN')

		def __str__(self):
			s  = self.GetDocName()
			s += '\n'
			s += self.GetDocLongDescription()
			return s

		def SetParameterValue(self, paramKey, value):
			paramType = self.GetParameterType(paramKey)
			if paramType in [ParameterType_InputProcessXML, ParameterType_RAM,
											 ParameterType_String, ParameterType_InputFilename,
											 ParameterType_OutputImage, ParameterType_OutputVectorData,
											 ParameterType_OutputProcessXML, ParameterType_OutputFilename,
											 ParameterType_Directory, ParameterType_InputImage,
											 ParameterType_ComplexInputImage, ParameterType_InputVectorData]:
			  return self.SetParameterString(paramKey, value)
			elif paramType in [ParameterType_InputImageList, ParameterType_InputVectorDataList,
												 ParameterType_InputFilenameList, ParameterType_StringList,
												 ParameterType_ListView]:
			  return self.setParameterStringList(paramKey, value)
			elif paramType in [ParameterType_Int, ParameterType_Radius]:
			  return self.SetParameterInt(paramKey, value)
			elif paramType in [ParameterType_Float]:
			  return self.SetParameterFloat(paramKey, value)
			elif paramType in [ParameterType_Empty]:
			  return self.EnableParameter(paramKey)
			elif paramType in [ParameterType_Group]:
			  return ApplicationProxy(self, paramKey)
			elif paramType in [ParameterType_Choice]:
			  return ApplicationProxy(self, paramKey, value)
			else:
			  print ("Unsupported parameter type '%s' with key '%s'" %(self.GetParameterTypeAsString(paramType) ,paramKey))
			return

		def GetParameterValue(self, paramKey):
			paramType = self.GetParameterType(paramKey)
			if paramType in [ParameterType_InputProcessXML,
											 ParameterType_String, ParameterType_InputFilename,
											 ParameterType_OutputImage, ParameterType_OutputVectorData,
											 ParameterType_OutputProcessXML, ParameterType_OutputFilename,
											 ParameterType_Directory, ParameterType_InputImage,
											 ParameterType_ComplexInputImage, ParameterType_InputVectorData]:
			  return self.GetParameterString(paramKey)
			elif paramType in [ParameterType_InputImageList, ParameterType_InputVectorDataList,
												 ParameterType_InputFilenameList, ParameterType_StringList,
												 ParameterType_ListView]:
			  return self.GetParameterStringList(paramKey)
			elif paramType in [ParameterType_Int, ParameterType_Radius, ParameterType_RAM]:
			  return self.GetParameterInt(paramKey)
			elif paramType in [ParameterType_Float]:
			  return self.GetParameterFloat(paramKey)
			elif paramType in [ParameterType_Empty]:
			  return self.IsParameterEnabled(paramKey)
			elif paramType in [ParameterType_Group, ParameterType_Choice]:
			  return ApplicationProxy(self, paramKey)
			else:
			  print ("Unsupported parameter type '%s' with key '%s'" %(self.GetParameterTypeAsString(paramType) ,paramKey))
			return None

		def __getattr__(self,name):
			"""
			__get_attribute__ is called whenever an instance request an attribute.
			eg: App.SetParameterString(), App.GetName() ..
			__getattr__ is only called if the attribute is not found by __get_attribute__ call
			So we keep hide the GetParameter** calls within this method so that it seems like
			an obivous call for users. App.IN , App.OUT , where 'in' and 'out' are
			parameters in the 'otb application' with instance App
			Since SWIG also uses this function, we have to copy their code before
			using custom OTB behaviour
			"""
			if (name == "thisown"):
				return self.this.own()
			method = Application.__swig_getmethods__.get(name, None)
			if method:
				return method(self)
			key_list = [k.upper() for k in self.GetParametersKeys(True)]
			if name in key_list:
				return self.GetParameterValue(name.lower())
			raise AttributeError("'%s' object has no attribute '%s'" % (Application.__name__, name))

		def __setattr__(self, name, value):
			"""
			__setattr__ is called if the attribute requested is not found in the attribute list.
			So these attributes are supposed to be 'key' of parameters used. Here we
			keep hide the SetParameter** calls within this method so that it seems like
			an obivous call for users. App.IN='my-input-file-name' , App.OUT='my-output-file-name'w
			here 'in' and 'out' are    parameters in the 'otb application' with instance App
			Ofcourse, we don't blindly accept any attributes as python, we check them against
			list of existing parameters for application with 'self.GetParametersKeys(True)'
			Since SWIG also uses this function, we have to copy their code before
			using custom OTB behaviour
			"""
			if (name == "thisown"):
				return self.this.own(value)
			if (name == "this"):
				if type(value).__name__ == 'SwigPyObject':
					self.__dict__[name] = value
					return
			method = Application.__swig_setmethods__.get(name, None)
			if method:
				return method(self, value)
			key_list = [k.upper() for k in self.GetParametersKeys(True)]
			if name in key_list:
				self.SetParameterValue(name.lower(), value)
			else:
				raise AttributeError("You cannot add attributes to %s" % self)

    }
}

#endif


#if OTB_SWIGNUMPY

%extend Application
{
  %pythoncode
    {
    def SetImageFromNumpyArray(self, paramKey, npArray):
      """
      This method takes a numpy array and set ImageIOBase of
      InputImageParameter by creating an otbImage with
      same pixel type as numpyarray.dtype
      """
      if len(npArray.shape) == 3:
         raise ValueError( "(len(npArray.shape) == 3)\n"
                           "Input array given is of 3 dimension.\n"
                           "SetImageFromNumpyArray create ImageIO from otbImage and thus demands a 2d array.\n"
                           "you can either provide an 2d numpy array or use SetVectorImageFromNumpyArray depending on your application.\n")

      dt = npArray.dtype.name
      if dt == 'int8':
        self.SetImageFromInt8NumpyArray_(paramKey, npArray)
      elif dt == 'int16':
        self.SetImageFromInt16NumpyArray_(paramKey, npArray)
      elif dt == 'int32':
        self.SetImageFromInt32NumpyArray_(paramKey, npArray)
      elif dt == 'uint8':
        self.SetImageFromUInt8NumpyArray_(paramKey, npArray)
      elif dt == 'uint16':
        self.SetImageFromUInt16NumpyArray_(paramKey, npArray)
      elif dt == 'uint32':
        self.SetImageFromUInt32NumpyArray_(paramKey, npArray)
      elif dt == 'float':
        self.SetImageFromFloatNumpyArray_(paramKey, npArray)
      elif dt == 'double':
        self.SetImageFromDoubleNumpyArray_(paramKey, npArray)
      else:
        self.SetImageFromFloatNumpyArray_(paramKey, npArray)
      return

    def SetVectorImageFromNumpyArray(self, paramKey, npArray):
      """
      This method takes a numpy array and set ImageIOBase of
      InputImageParameter by creating an otbVectorImage with
      same pixel type as numpyarray.dtype.
      NOTE: Input (npArray) must be an ndarray with 3 dimension,
      len(npArray.shape) must be > 2
      """
      if len(npArray.shape) < 3:
        raise ValueError( "(len(npArray.shape) < 3)\n"
                        "Input array given is not of 3 dimension.\n"
                        "SetVectorImageFromNumpyArray create ImageIO from otbVectorImage and thus demands an array of shape 3.\n"
                        "you can either provide an 3d numpy array or use SetImageFromNumpyArray depending on your application.\n")

      dt = npArray.dtype.name
      if dt == 'int8':
        self.SetVectorImageFromInt8NumpyArray_(paramKey, npArray)
      elif dt == 'int16':
        self.SetVectorImageFromInt16NumpyArray_(paramKey, npArray)
      elif dt == 'int32':
        self.SetVectorImageFromInt32NumpyArray_(paramKey, npArray)
      elif dt == 'uint8':
        self.SetVectorImageFromUInt8NumpyArray_(paramKey, npArray)
      elif dt == 'uint16':
        self.SetVectorImageFromUInt16NumpyArray_(paramKey, npArray)
      elif dt == 'uint32':
        self.SetVectorImageFromUInt32NumpyArray_(paramKey, npArray)
      elif dt == 'float':
        self.SetVectorImageFromFloatNumpyArray_(paramKey, npArray)
      elif dt == 'double':
        self.SetVectorImageFromDoubleNumpyArray_(paramKey, npArray)
      else:
        self.SetVectorImageFromFloatNumpyArray_(paramKey, npArray)
      return

    def GetVectorImageAsNumpyArray(self, paramKey, dt='float'):
      """
      If datatype is unknown this method assumes to numpy.float32
      Valid datatypes are:
      int8, int16, int32, uint8, uint16, uint32, float, double.
      NOTE: This method always return an numpy array with dimension 3
      """
      if dt == 'int8':
        return self.GetVectorImageAsInt8NumpyArray_(paramKey)
      elif dt == 'int16':
        return self.GetVectorImageAsInt16NumpyArray_(paramKey)
      elif dt == 'int32':
        return self.GetVectorImageAsInt32NumpyArray_(paramKey)
      elif dt == 'uint8':
        return self.GetVectorImageAsUInt8NumpyArray_(paramKey)
      elif dt == 'uint16':
        return self.GetVectorImageAsUInt16NumpyArray_(paramKey)
      elif dt == 'uint32':
        return self.GetVectorImageAsUInt32NumpyArray_(paramKey)
      elif dt == 'float':
        return self.GetVectorImageAsFloatNumpyArray_(paramKey)
      elif dt == 'double':
        return self.GetVectorImageAsDoubleNumpyArray_(paramKey)
      else:
        print ("Unknown datatype '" + dt + "'. Using float instead. Available types are:")
        print ("int8, int16, int32, uint8, uint16, uint32, float, double")
        return self.GetVectorImageAsFloatNumpyArray_(paramKey)

    def GetImageAsNumpyArray(self, paramKey, dt='float'):
      """
      If datatype is unknown this method assumes to numpy.float32
      Valid datatypes are:
      int8, int16, int32, uint8, uint16, uint32, float, double.
      NOTE: This method always return an numpy array with dimension 3
      """
      if dt == 'int8':
        numpy_vector_image = self.GetVectorImageAsInt8NumpyArray_(paramKey)
      elif dt == 'int16':
        numpy_vector_image = self.GetVectorImageAsInt16NumpyArray_(paramKey)
      elif dt == 'int32':
        numpy_vector_image = self.GetVectorImageAsInt32NumpyArray_(paramKey)
      elif dt == 'uint8':
        numpy_vector_image = self.GetVectorImageAsUInt8NumpyArray_(paramKey)
      elif dt == 'uint16':
        numpy_vector_image = self.GetVectorImageAsUInt16NumpyArray_(paramKey)
      elif dt == 'uint32':
        numpy_vector_image = self.GetVectorImageAsUInt32NumpyArray_(paramKey)
      elif dt == 'float':
        numpy_vector_image = self.GetVectorImageAsFloatNumpyArray_(paramKey)
      elif dt == 'double':
        numpy_vector_image = self.GetVectorImageAsDoubleNumpyArray_(paramKey)

      else:
        print ("Unknown datatype '" + dt + "'. Using float instead. Available types are:")
        print ("int8, int16, int32, uint8, uint16, uint32, float, double")
        numpy_vector_image = self.GetVectorImageAsFloatNumpyArray_(paramKey)

      if numpy_vector_image.shape[2] > 1:
        raise ValueError("numpy_vector_image.shape[2] > 1\n"
                         "Output image from application has more than 1 band\n"
                         "GetImageFromNumpyArray only returns the first band, which will result in a loss of data.\n"
                         "In this case you must use GetVectorImageFromNumpyArray which is capable of return a 3 dimension image.\n")

      numpy_vector_image = numpy_vector_image[:,:,0]
      return numpy_vector_image


    }
}

#endif /* OTB_SWIGNUMPY */

class Registry : public itkObject
{
public:

  static std::vector<std::string> GetAvailableApplications();
  static Application_Pointer CreateApplication(const std::string& name);
  static void AddApplicationPath(std::string newpath);
  static void SetApplicationPath(std::string newpath);

protected:
  Registry();
  virtual ~Registry();
};

class AddProcessToWatchEvent : public itkEventObject
{
public:
  AddProcessToWatchEvent();
  ~AddProcessToWatchEvent();
  virtual char const * GetEventName() const;
  virtual bool CheckEvent(itkEventObject const * e) const;
  virtual itkEventObject * MakeObject() const;
  AddProcessToWatchEvent(AddProcessToWatchEvent const & s);

  itkProcessObject * GetProcess() const;
  void SetProcess(itkProcessObject * process);
  void SetProcessDescription(const std::string desc);
  std::string GetProcessDescription() const;
};

%include "PyCommand.i"
