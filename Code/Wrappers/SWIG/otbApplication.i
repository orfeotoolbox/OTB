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

%module otbApplication


 %{
#include "itkBase.includes"
#include "otbWrapperSWIGIncludes.h"
 %}

%rename("SetParameterStringList_") Application::SetParameterStringList(std::string parameter, std::vector<std::string> value);
%rename("SetParameterInt_") Application::SetParameterInt(std::string parameter, int value);
%rename("SetParameterFloat_") Application::SetParameterFloat(std::string parameter, float value);
%rename("SetParameterString_") Application::SetParameterString(std::string parameter, std::string value);


// Langage specific extension
%include "Python.i"
%include "Java.i"
%include "Ruby.i"
%include "Lua.i"

%include "itkMacro.i"
%include "itkBase.i"

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
   Role_Input,
   Role_Output
 } Role;

 typedef enum
 {
   ComplexImagePixelType_float,
   ComplexImagePixelType_double,
 } ComplexImagePixelType;

}
}

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

  void SetParameterInt(std::string parameter, int value);
  void SetParameterFloat(std::string parameter, float value);
  void SetParameterString(std::string parameter, std::string value);
  void SetParameterStringList(std::string parameter, std::vector<std::string> value);

  void SetParameterOutputImagePixelType(std::string parameter, otb::Wrapper::ImagePixelType pixelType);
  void SetParameterComplexOutputImagePixelType(std::string parameter, otb::Wrapper::ComplexImagePixelType cpixelType);

  otb::Wrapper::ImagePixelType GetParameterOutputImagePixelType(std::string parameter);
  otb::Wrapper::ComplexImagePixelType GetParameterComplexOutputImagePixelType(std::string parameter);

  int GetParameterInt(std::string parameter);
  float GetParameterFloat(std::string parameter);
  std::string GetParameterString(std::string parameter);
  std::vector<std::string> GetParameterStringList(std::string parameter);
  std::string GetParameterAsString(std::string paramKey);

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

#if SWIGPYTHON
%extend Application {
  %pythoncode {
    def GetParameterValue(self, paramKey):
       paramType = self.GetParameterType(paramKey)
       if paramType in [ParameterType_InputProcessXML, ParameterType_Choice, ParameterType_ListView,
                        ParameterType_String, ParameterType_InputFilename,
                        ParameterType_OutputImage, ParameterType_OutputVectorData,
                        ParameterType_OutputProcessXML, ParameterType_OutputFilename,
                        ParameterType_Directory, ParameterType_InputImage,
                        ParameterType_ComplexInputImage, ParameterType_InputVectorData]:
          return self.GetParameterString(paramKey)

       elif paramType in [ ParameterType_InputImageList, ParameterType_InputVectorDataList,
                        ParameterType_InputFilenameList, ParameterType_StringList ]:
          return self.GetParameterStringList(paramKey)

       elif paramType in [ParameterType_Int, ParameterType_Radius, ParameterType_RAM]:
          return self.GetParameterInt(paramKey)

       elif paramType in [ParameterType_Float]:
          return self.GetParameterFloat(paramKey)

       else:
          print "Cant get value for parameter '" + paramKey  + " ' of type '" + paramType + "' or ParameterType Emtpy"
          return None

    def SetParameterStringList(self, paramKey, paramValue, overwriteFlag=False):
          assert not isinstance(paramValue, basestring) #provide a better error message
          self.SetParameterStringList_(paramKey, paramValue)
          self.SetParameterUserValue(paramKey, overwriteFlag)

    def SetParameterString(self, paramKey, paramValue, overwriteFlag=False):
          self.SetParameterString_(paramKey, paramValue)
          self.SetParameterUserValue(paramKey, overwriteFlag)

    def SetParameterInt(self, paramKey, paramValue, overwriteFlag=False):
          self.SetParameterInt_(paramKey, paramValue)
          self.SetParameterUserValue(paramKey, overwriteFlag)

    def SetParameterFloat(self, paramKey, paramValue, overwriteFlag=False):
          self.SetParameterFloat_(paramKey, paramValue)
          self.SetParameterUserValue(paramKey, overwriteFlag)

      }
}
#endif

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
