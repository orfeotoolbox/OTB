/*
 * Copyright (C) 2005-2019 CS Systemes d'Information (CS SI)
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
%apply (std::complex<float>** ARGOUTVIEW_ARRAY3, int *DIM1, int *DIM2, int *DIM3) {(std::complex<float>** buffer, int *dim1, int *dim2, int *dim3)};
%apply (std::complex<double>** ARGOUTVIEW_ARRAY3, int *DIM1, int *DIM2, int *DIM3) {(std::complex<double>** buffer, int *dim1, int *dim2, int *dim3)};

#endif /* OTB_SWIGNUMPY */

namespace otb
{
namespace Wrapper
{

  typedef enum
  {
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
    ParameterType_RAM,
    ParameterType_Bool
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
    ImagePixelType_cint16,
    ImagePixelType_cint32,
    ImagePixelType_cfloat,
    ImagePixelType_cdouble,
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

} // end of namespace Wrapper

class ImageKeywordlist
{
public:
  typedef std::map<std::string, std::string> KeywordlistMap;
  typedef KeywordlistMap::size_type KeywordlistMapSizeType;
  ImageKeywordlist();
  virtual ~ImageKeywordlist();
  const KeywordlistMap& GetKeywordlist() const;
  void Clear(void);
  KeywordlistMapSizeType Empty() const;
  KeywordlistMapSizeType GetSize(void) const;
  const std::string& GetMetadataByKey(const std::string& key) const;
  bool HasKey(const std::string& key) const;
  virtual void ClearMetadataByKey(const std::string& key);
  virtual void AddKey(const std::string& key, const std::string& value);
  virtual void Print(std::ostream& os, itkIndent indent = 0) const;
};

class VectorDataKeywordlist
{
public:
  VectorDataKeywordlist();
  ~VectorDataKeywordlist();
  // VectorDataKeywordlist(const Self& other);
  // TODO : finish wrapping
};

class OTB_GCP
{
public:
  std::string m_Id;
  std::string m_Info;
  double m_GCPCol;
  double m_GCPRow;
  double m_GCPX;
  double m_GCPY;
  double m_GCPZ;
  OTB_GCP();
  ~OTB_GCP();
  void Print(std::ostream& os) const;
};

#if SWIGPYTHON
%extend ImageKeywordlist
  {
  %pythoncode
    {
    def __str__(self):
      return str(self.GetKeywordlist())
    def __len__(self):
      return self.GetSize()
    def __getitem__(self,key):
      return self.GetKeywordlist()[key]
    def __setitem__(self,key,val):
      self.GetKeywordlist()[key] = val
    def keys(self):
      return self.GetKeywordlist().keys()
    }
  }
#endif

} // end of namespace otb

#if SWIGPYTHON


%include "otbPythonLogOutput.i"
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

  void LoadParametersFromXML(const std::string& filename);
  void SaveParametersToXML(const std::string& filename);

#if SWIGPYTHON
  Logger* GetLogger();
#endif
  unsigned long itk::Object::AddObserver(const EventObject & event, 
                                          Command * command);


#if SWIGPYTHON
  %extend 
    {
    /** SetupLogger : Add the PythonLogOutput and setup the progress 
     * reporting for the application */
    %pythoncode
      {
      def SetupLogger(self):
          logger = self.GetLogger()
          logger.AddLogOutput(_libraryLogOutput.GetPointer())
          
          self.AddObserver(AddProcessToWatchEvent(),
                           _libraryProgressReportManager.GetAddProcessCommand()
                          )
      }
    }
#endif // SWIGPYTHON

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

  void SetParameterOutputImagePixelType(std::string parameter, otb::Wrapper::ImagePixelType pixelType);

  otb::Wrapper::ImagePixelType GetParameterOutputImagePixelType(std::string parameter);

  int GetParameterInt(std::string parameter);
  float GetParameterFloat(std::string parameter);
  std::string GetParameterString(std::string parameter);
  std::vector<std::string> GetParameterStringList(std::string parameter);
  std::string GetParameterAsString(std::string paramKey);

  bool GetListViewSingleSelectionMode(const std::string& paramKey);

  ImageBaseType * GetParameterOutputImage(std::string parameter);
  void SetParameterInputImage(std::string parameter, ImageBaseType * inputImage);
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
  otb::ImageKeywordlist GetImageKeywordlist(const std::string & key, unsigned int idx = 0);
  unsigned long PropagateRequestedRegion(const std::string & key, itk::ImageRegion<2> region, unsigned int idx = 0);
  itk::ImageRegion<2> GetImageRequestedRegion(const std::string & key, unsigned int idx = 0);
  itkMetaDataDictionary GetImageMetaData(const std::string & key, unsigned int idx = 0);
  otb::Wrapper::ImagePixelType GetImageBasePixelType(const std::string & key, unsigned int idx = 0);

  itkProcessObject* GetProgressSource() const;

  std::string GetProgressDescription() const;

  void FreeRessources();

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
  ImageBaseType* Set##ImageClass##From##prefix##NumpyArray_(std::string pkey, int idx, ##PixelDataType##* buffer, int dim1, int dim2, int dim3) \
  {                                                                 \
    typedef otb::##ImageClass##<##PixelDataType##>   ImageType;     \
    ImageType::Pointer output = ImageType::New();                   \
    unsigned int numberOfPixels = dim1 * dim2 * dim3;               \
    ImageType::RegionType region;                                   \
    region.SetIndex(0, 0);                                          \
    region.SetIndex(1, 0);                                          \
    region.SetSize( 0, dim2);                                       \
    region.SetSize( 1, dim1);                                       \
    output->SetNumberOfComponentsPerPixel(dim3);                    \
    output->SetRegions(region);                                     \
    output->GetPixelContainer()->SetImportPointer(buffer, numberOfPixels, false); \
    $self->SetParameterImageBase(pkey,output.GetPointer(),idx);     \
    return output.GetPointer();                                     \
  }

  SetFromNumpyArrayMacro(UInt8, unsigned char, VectorImage)
  SetFromNumpyArrayMacro(Int16, signed short, VectorImage)
  SetFromNumpyArrayMacro(UInt16, unsigned short, VectorImage)
  SetFromNumpyArrayMacro(Int32, signed int, VectorImage)
  SetFromNumpyArrayMacro(UInt32, unsigned int, VectorImage)
  // SetFromNumpyArrayMacro(Int64, signed long, VectorImage)
  // SetFromNumpyArrayMacro(UInt64, unsigned long, VectorImage)
  SetFromNumpyArrayMacro(Float, float, VectorImage)
  SetFromNumpyArrayMacro(Double, double, VectorImage)
  SetFromNumpyArrayMacro(CFloat, std::complex<float>, VectorImage)
  SetFromNumpyArrayMacro(CDouble, std::complex<double>, VectorImage)

  SetFromNumpyArrayMacro(UInt8, unsigned char, Image)
  SetFromNumpyArrayMacro(Int16, signed short, Image)
  SetFromNumpyArrayMacro(UInt16, unsigned short, Image)
  SetFromNumpyArrayMacro(Int32, signed int, Image)
  SetFromNumpyArrayMacro(UInt32, unsigned int, Image)
  // SetFromNumpyArrayMacro(Int64, signed long, Image)
  // SetFromNumpyArrayMacro(UInt64, unsigned long, Image)
  SetFromNumpyArrayMacro(Float, float, Image)
  SetFromNumpyArrayMacro(Double, double, Image)
  SetFromNumpyArrayMacro(CFloat, std::complex<float>, Image)
  SetFromNumpyArrayMacro(CDouble, std::complex<double>, Image)
#undef SetFromNumpyArrayMacro

#define GetVectorImageAsNumpyArrayMacro(suffix, TPixel)                     \
  void GetVectorImageAs##suffix##NumpyArray_                                  \
    (std::string pkey, ##TPixel##** buffer, int *dim1, int *dim2, int *dim3)  \
    {                                                                         \
    ImageBaseType *img = $self->GetParameterOutputImage(pkey);                \
    img->Update();                                                            \
    unsigned int nbComp = img->GetNumberOfComponentsPerPixel();               \
    ImageBaseType::RegionType region = img->GetBufferedRegion();              \
    ImageBaseType::SizeType size = region.GetSize();                          \
    *dim1 = region.GetSize(1);                                                \
    *dim2 = region.GetSize(0);                                                \
    *dim3 = nbComp;                                                           \
    std::string className(img->GetNameOfClass());                             \
    if (className == "VectorImage")                                           \
      {                                                                       \
      typedef otb::VectorImage<##TPixel##,2> LocalVectorImageType;            \
      LocalVectorImageType* imgDown = dynamic_cast<LocalVectorImageType*>(img); \
      if (imgDown)                                                            \
        *buffer  =  reinterpret_cast<##TPixel##*>(imgDown->GetBufferPointer());  \
      else                                                                    \
        std::cerr << "VectorImage type doesn't match" << std::endl;           \
      }                                                                       \
    else                                                                      \
      {                                                                       \
      if (nbComp == 1)                                                        \
        {                                                                     \
        otb::Image<##TPixel##,2>* imgDown = dynamic_cast< otb::Image<##TPixel##,2>* >(img); \
        if (imgDown)                                                          \
          *buffer  =  reinterpret_cast<##TPixel##*>(imgDown->GetBufferPointer()); \
        else                                                                  \
          std::cerr << "Image type doesn't match" << std::endl;               \
        }                                                                     \
      else                                                                    \
        {                                                                     \
        std::cerr << "Unhandled number of components in otb::Image (RGB<T> "  \
            "and RGBA<T> not supported yet)" << std::endl;                    \
        }                                                                     \
      }                                                                       \
    }

  GetVectorImageAsNumpyArrayMacro(UInt8, unsigned char)
  GetVectorImageAsNumpyArrayMacro(Int16,signed short);
  GetVectorImageAsNumpyArrayMacro(UInt16,unsigned short);
  GetVectorImageAsNumpyArrayMacro(Int32,signed int);
  GetVectorImageAsNumpyArrayMacro(UInt32,unsigned int);
  GetVectorImageAsNumpyArrayMacro(Float,float);
  GetVectorImageAsNumpyArrayMacro(Double,double);
  GetVectorImageAsNumpyArrayMacro(CFloat,std::complex<float> );
  GetVectorImageAsNumpyArrayMacro(CDouble,std::complex<double> );
  // CInt16 and CInt32 are not supported in Numpy
#undef GetVectorImageAsNumpyArrayMacro

  std::string ConvertPixelTypeToNumpy(otb::Wrapper::ImagePixelType pixType)
    {
    std::ostringstream oss;
    switch (pixType)
      {
      case otb::Wrapper::ImagePixelType_uint8 :
        oss << "uint" << (sizeof(unsigned char) * 8);
        break;
      case otb::Wrapper::ImagePixelType_int16 :
        oss << "int" << (sizeof(signed short) * 8);
        break;
      case otb::Wrapper::ImagePixelType_uint16 :
        oss << "uint" << (sizeof(unsigned short) * 8);
        break;
      case otb::Wrapper::ImagePixelType_int32 :
        oss << "int" << (sizeof(signed int) * 8);
        break;
      case otb::Wrapper::ImagePixelType_uint32 :
        oss << "uint" << (sizeof(unsigned int) * 8);
        break;
      case otb::Wrapper::ImagePixelType_float :
        oss << "float" << (sizeof(float) * 8);
        break;
      case otb::Wrapper::ImagePixelType_double :
        oss << "float" << (sizeof(double) * 8);
        break;
      case otb::Wrapper::ImagePixelType_cfloat :
        oss << "complex" << (sizeof(std::complex<float>) * 8);
        break;
      case otb::Wrapper::ImagePixelType_cdouble :
        oss << "complex" << (sizeof(std::complex<double>) * 8);
        break;
      default:
        std::cerr << "Pixel type not handled" << std::endl;
        break;
      }
    return oss.str();
    }

  void SetupImageInformation(
    ImageBaseType* img,
    itk::Point<SpacePrecisionType,2> origin,
    itk::Vector<SpacePrecisionType,2> spacing,
    itk::Size<2> size,
    itk::ImageRegion<2> bufferRegion,
    itkMetaDataDictionary metadata)
    {
    img->SetOrigin(origin);
    otb::internal::SetSignedSpacing(img, spacing);
    itk::ImageRegion<2> largest;
    largest.SetSize(size);
    img->SetLargestPossibleRegion(largest);
    if (bufferRegion.GetSize() != img->GetBufferedRegion().GetSize())
      {
      std::cerr << "Given buffered size doesn't match actual buffer size" << std::endl;
      return;
      }
    img->SetRequestedRegion(bufferRegion);
    img->SetBufferedRegion(bufferRegion);
    img->SetMetaDataDictionary(metadata);
    }
} /* end of %extend */
#endif /* OTB_SWIGNUMPY */

protected:
  Application();
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
				ParameterType_String : 'ParameterType_String',
				ParameterType_InputFilename : 'ParameterType_InputFilename',
				ParameterType_OutputImage : 'ParameterType_OutputImage',
				ParameterType_OutputVectorData : 'ParameterType_OutputVectorData',
				ParameterType_OutputFilename : 'ParameterType_OutputFilename',
				ParameterType_Directory : 'ParameterType_Directory',
				ParameterType_InputImage : 'ParameterType_InputImage',
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
				ParameterType_Choice : 'ParameterType_Choice',
				ParameterType_Group : 'ParameterType_Group',
				ParameterType_Bool : 'ParameterType_Bool'
			}.get(parameter_type, 'ParameterType_UNKNOWN')

		def __str__(self):
			s  = self.GetDocName()
			s += '\n'
			s += self.GetDocLongDescription()
			return s

		def SetParameters(self, dict_params):
			for param_key, param_value in dict_params.items():
				self.SetParameterValue(param_key, param_value)

		def SetParameterValue(self, paramKey, value):
			paramType = self.GetParameterType(paramKey)
			if paramType in [ParameterType_RAM,
											 ParameterType_String, ParameterType_InputFilename,
											 ParameterType_OutputImage, ParameterType_OutputVectorData,
											 ParameterType_OutputFilename,
											 ParameterType_Directory, ParameterType_InputImage,
											 ParameterType_InputVectorData]:
			  return self.SetParameterString(paramKey, value)
			elif paramType in [ParameterType_InputImageList, ParameterType_InputVectorDataList,
												 ParameterType_InputFilenameList, ParameterType_StringList,
												 ParameterType_ListView]:
			  return self.SetParameterStringList(paramKey, value)
			elif paramType in [ParameterType_Int, ParameterType_Radius]:
			  return self.SetParameterInt(paramKey, value)
			elif paramType in [ParameterType_Float]:
			  return self.SetParameterFloat(paramKey, value)
			elif paramType in [ParameterType_Bool]:
			  return self.SetParameterString(paramKey, str(value) )
			elif paramType in [ParameterType_Group]:
			  return ApplicationProxy(self, paramKey)
			elif paramType in [ParameterType_Choice]:
			  return ApplicationProxy(self, paramKey, value)
			else:
			  print ("Unsupported parameter type '%s' with key '%s'" %(self.GetParameterTypeAsString(paramType) ,paramKey))
			return

		def GetParameters(self):
			ret = {}
			for key in self.GetParametersKeys():
				if self.HasValue(key) and self.IsParameterEnabled(key) and self.GetParameterRole(key) == 0:
					ret[key] = self.GetParameterValue(key)
			return ret

		def GetParameterValue(self, paramKey):
			paramType = self.GetParameterType(paramKey)
			if paramType in [
											 ParameterType_String, ParameterType_InputFilename,
											 ParameterType_OutputImage, ParameterType_OutputVectorData,
											 ParameterType_OutputFilename,
											 ParameterType_Directory, ParameterType_InputImage,
											 ParameterType_InputVectorData]:
			  return self.GetParameterString(paramKey)
			elif paramType in [ParameterType_InputImageList, ParameterType_InputVectorDataList,
												 ParameterType_InputFilenameList, ParameterType_StringList,
												 ParameterType_ListView]:
			  return self.GetParameterStringList(paramKey)
			elif paramType in [ParameterType_Int, ParameterType_Radius, ParameterType_RAM]:
			  return self.GetParameterInt(paramKey)
			elif paramType in [ParameterType_Float]:
			  return self.GetParameterFloat(paramKey)
			elif paramType in [ParameterType_Bool]:
			  return bool(self.GetParameterInt(paramKey))
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
    NumpyExporterMap = {
      ImagePixelType_uint8 : GetVectorImageAsUInt8NumpyArray_,
      ImagePixelType_int16 : GetVectorImageAsInt16NumpyArray_,
      ImagePixelType_uint16 : GetVectorImageAsUInt16NumpyArray_,
      ImagePixelType_int32 : GetVectorImageAsInt32NumpyArray_,
      ImagePixelType_uint32 : GetVectorImageAsUInt32NumpyArray_,
      ImagePixelType_float : GetVectorImageAsFloatNumpyArray_,
      ImagePixelType_double : GetVectorImageAsDoubleNumpyArray_,
      ImagePixelType_cfloat : GetVectorImageAsCFloatNumpyArray_,
      ImagePixelType_cdouble : GetVectorImageAsCDoubleNumpyArray_,
      }
    ImageImporterMap = {
      ImagePixelType_uint8 : SetImageFromUInt8NumpyArray_,
      ImagePixelType_int16 : SetImageFromInt16NumpyArray_,
      ImagePixelType_uint16 : SetImageFromUInt16NumpyArray_,
      ImagePixelType_int32 : SetImageFromInt32NumpyArray_,
      ImagePixelType_uint32 : SetImageFromUInt32NumpyArray_,
      ImagePixelType_float : SetImageFromFloatNumpyArray_,
      ImagePixelType_double : SetImageFromDoubleNumpyArray_,
      ImagePixelType_cfloat : SetImageFromCFloatNumpyArray_,
      ImagePixelType_cdouble : SetImageFromCDoubleNumpyArray_,
      }
    VectorImageImporterMap = {
      ImagePixelType_uint8 : SetVectorImageFromUInt8NumpyArray_,
      ImagePixelType_int16 : SetVectorImageFromInt16NumpyArray_,
      ImagePixelType_uint16 : SetVectorImageFromUInt16NumpyArray_,
      ImagePixelType_int32 : SetVectorImageFromInt32NumpyArray_,
      ImagePixelType_uint32 : SetVectorImageFromUInt32NumpyArray_,
      ImagePixelType_float : SetVectorImageFromFloatNumpyArray_,
      ImagePixelType_double : SetVectorImageFromDoubleNumpyArray_,
      ImagePixelType_cfloat : SetVectorImageFromCFloatNumpyArray_,
      ImagePixelType_cdouble : SetVectorImageFromCDoubleNumpyArray_,
      }
    
    def SetImageFromNumpyArray(self, paramKey, npArray, index=0):
      """
      This method takes a numpy array and set ImageIOBase of
      InputImageParameter by creating an otbImage with
      same pixel type as numpyarray.dtype
      """
      shp = npArray.shape
      if len(shp) == 2:
        npArray = npArray.reshape((shp[0],shp[1],1))
      elif len(shp) == 3:
        if shp[2] > 1:
          raise ValueError( "More than 1 band in numpy array\n"
                           "Cannot convert to Image, use SetVectorImageFromNumpyArray instead\n")
      else:
        raise ValueError( "Expected 2 or 3 dimensions for numpyarray\n")
      dt = npArray.dtype.name
      isFound = False
      for pixT in self.ImageImporterMap:
        if dt == self.ConvertPixelTypeToNumpy(pixT):
          isFound = True
          img = self.ImageImporterMap[pixT](self,paramKey, index, npArray)
          break
      if len(shp) == 2:
        npArray = npArray.reshape(shp)
      if not isFound:
        raise ValueError("Can't convert Numpy array of dtype "+dt)
      return img

    def SetVectorImageFromNumpyArray(self, paramKey, npArray, index=0):
      """
      This method takes a numpy array and set
      InputImageParameter by creating an otbVectorImage with
      same pixel type as numpyarray.dtype.
      NOTE: Input (npArray) must be an ndarray with 2 or 3 dimensions,
      """
      shp = npArray.shape
      if len(shp) == 2:
        npArray = npArray.reshape((shp[0],shp[1],1))
      elif len(npArray.shape) != 3:
        raise ValueError( "Expected 2 or 3 dimensions for numpyarray")
      dt = npArray.dtype.name
      isFound = False
      for pixT in self.VectorImageImporterMap:
        if dt == self.ConvertPixelTypeToNumpy(pixT):
          isFound = True
          img = self.VectorImageImporterMap[pixT](self,paramKey, index, npArray)
          break
      if len(shp) == 2:
        npArray = npArray.reshape(shp)
      if not isFound:
        raise ValueError("Can't convert Numpy array of dtype "+dt)
      return img

    def GetVectorImageAsNumpyArray(self, paramKey, dt='float'):
      """
      This function retrieves an output image parameter as a Numpy array.
      The array datatype is guessed automatically from the underlying
      otb::VectorImage<T,2> type (should also work with otb::Image<T,2>). The
      optional parameter dt is deprecated and should not be used anymore. The
      possible output datatypes are:
      int8, int16, int32, uint8, uint16, uint32, float, double, cint16, cint32,
      cfloat, cdouble.
      NOTE: This method always return an numpy array with 3 dimensions
      NOTE: cint16 and cint32 are not supported yet
      """
      pixT = self.GetImageBasePixelType(paramKey)
      return self.NumpyExporterMap[pixT](self,paramKey)

    def GetImageAsNumpyArray(self, paramKey, dt='float'):
      """
      This function retrieves an output image parameter as a Numpy array.
      The array datatype is guessed automatically from the underlying
      otb::VectorImage<T,2> type (should also work with otb::Image<T,2>). The
      optional parameter dt is deprecated and should not be used anymore. The
      possible output datatypes are:
      int8, int16, int32, uint8, uint16, uint32, float, double, cint16, cint32,
      cfloat, cdouble.
      NOTE: This method always return an numpy array with 2 dimensions
      NOTE: cint16 and cint32 are not supported yet
      """
      pixT = self.GetImageBasePixelType(paramKey)
      array = self.NumpyExporterMap[pixT](self,paramKey)
      if array.shape[2] > 1:
        raise ValueError("array.shape[2] > 1\n"
                         "Output image from application has more than 1 band.\n"
                         "GetImageAsNumpyArray only returns the first band, which will result in a loss of data.\n"
                         "In this case you must use GetVectorImageAsNumpyArray which can return a 3 dimension image.\n")
      array = array[:,:,0]
      return array

    def ImportImage(self, paramKey, pyImg, index = 0):
      """
      Import an image into a parameter, from a Python dict. with the following
      keys: array, origin, spacing, size, region, metadata
      """
      img = self.SetImageFromNumpyArray(paramKey, pyImg["array"], index)
      self.SetupImageInformation(img, pyImg["origin"], pyImg["spacing"], pyImg["size"], pyImg["region"], pyImg["metadata"])

    def ImportVectorImage(self, paramKey, pyImg, index = 0):
      """
      Import a vector image into a parameter, from a Python dict. with the following
      keys: array, origin, spacing, size, region, metadata
      """
      img = self.SetVectorImageFromNumpyArray(paramKey, pyImg["array"], index)
      self.SetupImageInformation(img, pyImg["origin"], pyImg["spacing"], pyImg["size"], pyImg["region"], pyImg["metadata"])

    def ExportImage(self, paramKey):
      """
      Export an output image from an otbApplication into a python dictionary with the
      following fields: array, origin, spacing, size, region, metadata
      """
      output = {}
      output["array"] = self.GetVectorImageAsNumpyArray(paramKey)
      output["origin"] = self.GetImageOrigin(paramKey)
      output["spacing"] = self.GetImageSpacing(paramKey)
      output["size"] = self.GetImageSize(paramKey)
      output["region"] = self.GetImageRequestedRegion(paramKey)
      output["metadata"] = self.GetImageMetaData(paramKey)
      return output

    }
}

#endif /* OTB_SWIGNUMPY */

class Registry : public itkObject
{
public:

  static std::vector<std::string> GetAvailableApplications();
  #if SWIGPYTHON
  %rename("CreateApplicationWithoutLogger") CreateApplication;
  static Application_Pointer CreateApplication(const std::string& name);
  %pythoncode
  {
    @staticmethod
    def CreateApplication(name):
        application = _otbApplication.Registry_CreateApplicationWithoutLogger(name)
        if application is not None:
            application.SetupLogger()
        return application
  }
  #else
  static Application_Pointer CreateApplication(const std::string& name);
  #endif
  static void AddApplicationPath(std::string newpath);
  static void SetApplicationPath(std::string newpath);
  static void CleanRegistry();

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

%extend itkMetaDataDictionary
{
  int GetType(const std::string &key)
    {
    return (int) otb::Wrapper::MetaDataHelper::GetType(key);
    }

  std::string GetString(const std::string &key)
    {
    return otb::Wrapper::MetaDataHelper::GetString(* $self,key);
    }
  void SetString(const std::string &key, const std::string &val)
    {
    otb::Wrapper::MetaDataHelper::SetString(* $self,key,val);
    }

  unsigned int GetInt(const std::string &key)
    {
    return otb::Wrapper::MetaDataHelper::GetInt(* $self,key);
    }
  void SetInt(const std::string &key, unsigned int val)
    {
    otb::Wrapper::MetaDataHelper::SetInt(* $self,key,val);
    }

  double GetDouble(const std::string &key)
    {
    return otb::Wrapper::MetaDataHelper::GetDouble(* $self,key);
    }
  void SetDouble(const std::string &key, double val)
    {
    otb::Wrapper::MetaDataHelper::SetDouble(* $self,key,val);
    }

  otb::OTB_GCP GetGCP(const std::string &key)
    {
    return otb::Wrapper::MetaDataHelper::GetGCP(* $self,key);
    }
  void SetGCP(const std::string &key, const otb::OTB_GCP &val)
    {
    otb::Wrapper::MetaDataHelper::SetGCP(* $self,key,val);
    }

  std::vector<double> GetVector(const std::string &key)
    {
    return otb::Wrapper::MetaDataHelper::GetVector(* $self,key);
    }
  void SetVector(const std::string &key, const std::vector<double> &val)
    {
    otb::Wrapper::MetaDataHelper::SetVector(* $self,key,val);
    }

  otb::ImageKeywordlist GetImageKWL(const std::string &key)
    {
    return otb::Wrapper::MetaDataHelper::GetImageKWL(* $self,key);
    }
  void SetImageKWL(const std::string &key, const otb::ImageKeywordlist &val)
    {
    otb::Wrapper::MetaDataHelper::SetImageKWL(* $self,key,val);
    }

  otb::VectorDataKeywordlist GetVectorDataKWL(const std::string &key)
    {
    return otb::Wrapper::MetaDataHelper::GetVectorDataKWL(* $self,key);
    }
  void SetVectorDataKWL(const std::string &key, const otb::VectorDataKeywordlist &val)
    {
    otb::Wrapper::MetaDataHelper::SetVectorDataKWL(* $self,key,val);
    }

  std::vector<bool> GetBoolVector(const std::string &key)
    {
    return otb::Wrapper::MetaDataHelper::GetBoolVector(* $self,key);
    }
  void SetBoolVector(const std::string &key, const std::vector<bool> &val)
    {
    otb::Wrapper::MetaDataHelper::SetBoolVector(* $self,key,val);
    }

#if SWIGPYTHON
// enhance the MetaDataDictionary class for Python
  %pythoncode
  {
    GetterMap = {
      0 : GetString,
      1 : GetInt,
      2 : GetDouble,
      3 : GetGCP,
      4 : GetVector,
      5 : GetImageKWL,
      6 : GetVectorDataKWL,
      7 : GetBoolVector,
      }

    SetterMap = {
      0 : SetString,
      1 : SetInt,
      2 : SetDouble,
      3 : SetGCP,
      4 : SetVector,
      5 : SetImageKWL,
      6 : SetVectorDataKWL,
      7 : SetBoolVector,
      }

    def __str__(self):
      ret = ''
      for k in self.GetKeys():
        ret += k + ':<...>, '
      if len(ret) == 0:
        ret += ', '
      return "{"+ret[:-2] +"}"
    def __len__(self):
      return len(self.GetKeys())
    def keys(self):
      return self.GetKeys()
    def __getitem__(self,key):
      if key in self.GetKeys():
        return self.GetterMap[self.GetType(key)](self,key)
      else:
        raise IndexError('Key not recognized')
    def __setitem__(self,key,val):
      if key in self.GetKeys():
        self.SetterMap[self.GetType(key)](self,key,val)
      else:
        raise IndexError('Key not recognized')
  }
#endif
};
