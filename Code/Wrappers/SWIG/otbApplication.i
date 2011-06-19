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
    ParameterType_Filename,
    ParameterType_Directory,
    ParameterType_Choice,
    ParameterType_InputImage,
    ParameterType_InputComplexImage,
    ParameterType_InputVectorData,
    ParameterType_OutputImage,
    ParameterType_OutputVectorData,
    ParameterType_Radius,
    ParameterType_Group,
  } ParameterType;
  
  typedef enum
  {
    UserLevel_Basic,
    UserLevel_Advanced
  } UserLevel;
}
}



/*
class Parameter : public itkObject
{
public:
  itkNewMacro(Parameter)

  itkSetStringMacro(Name)
  itkGetStringMacro(Name)
  
  itkSetStringMacro(Description)
  itkGetStringMacro(Description)
  
  itkSetStringMacro(Key)
  itkGetStringMacro(Key)

  itkSetMacro(Mandatory,bool)
  itkGetMacro(Mandatory,bool)
  itkBooleanMacro(Mandatory)

  itkSetEnumMacro(DefaultValueMode, otb::Wrapper::DefaultValueMode)
  itkGetEnumMacro(DefaultValueMode, otb::Wrapper::DefaultValueMode)

  virtual void Reset();
  
protected:
  Parameter();
  //~Parameter();
  
private:
    Parameter(const Parameter &);
    void operator =(const Parameter&);

};
DECLARE_REF_COUNT_CLASS( Parameter )

class ParameterGroup : public Parameter
{
public:
  itkNewMacro(ParameterGroup);
  
  void AddParameter(Parameter_Pointer p);
  Parameter_Pointer GetParameterByIndex(unsigned int i);
  Parameter_Pointer GetParameterByKey(std::string name);
  unsigned int GetNumberOfParameters();

protected:
  ParameterGroup();
  virtual ~ParameterGroup();

private:
  ParameterGroup(const ParameterGroup &);
  void operator =(const ParameterGroup&);
};
DECLARE_REF_COUNT_CLASS( ParameterGroup )
*/

class Application: public itkObject
{
public:
  itkSetStringMacro(Name);
  itkGetStringMacro(Name);

  itkSetStringMacro(Description);
  itkGetStringMacro(Description);

  void Init();
  void UpdateParameters();
  void Execute();
  void ExecuteAndWriteOutput();
  
//  ParameterGroup* GetParameterList();
//  Parameter* GetParameterByKey(std::string parameter);
//  const Parameter* GetParameterByKey(std::string parameter) const;
  std::vector<std::string> GetParametersKeys(bool recursive = true);
  std::string GetParameterName(std::string);
  std::string GetParameterDescription(std::string);
  
  void EnableParameter(std::string paramKey);
  void DisableParameter(std::string paramKey);
  bool IsParameterEnabled(std::string paramKey) const;
  bool IsMandatory(std::string paramKey) const;
  bool HasUserValue(std::string paramKey) const;
  void ClearValue(std::string paramKey);
  bool HasValue(std::string paramKey) const;
  otb::Wrapper::UserLevel GetParameterUserLevel(std::string paramKey) const;
  otb::Wrapper::ParameterType GetParameterType(std::string paramKey) const;
  
  void SetParameterInt(std::string parameter, int value);
  void SetParameterFloat(std::string parameter, float value);
  void SetParameterString(std::string parameter, std::string value);
  
  int GetParameterInt(std::string parameter);
  float GetParameterFloat(std::string parameter);
  std::string GetParameterString(std::string parameter);
  
protected:
  Application();
  //virtual ~Application();
  
  void AddChoice(std::string paramKey, std::string paramName);
  void AddParameter(otb::Wrapper::ParameterType type, std::string paramKey, std::string paramName);
  void MandatoryOn(std::string paramKey);
  void MandatoryOff(std::string paramKey);
  void SetParameterUserLevel(std::string paramKey, otb::Wrapper::UserLevel level);
  
private:
  virtual void DoCreateParameters() = 0;
  virtual void DoUpdateParameters() = 0;
  virtual void DoExecute() = 0;

  Application(const Application &);
  void operator =(const Application&);
};
DECLARE_REF_COUNT_CLASS( Application )


class Registry : public itkObject
{
public:
  static std::vector<std::string> GetAvailableApplications();
  static Application_Pointer CreateApplication(const std::string& name);

protected:
  Registry();
  virtual ~Registry();

private:
  Registry(const Self&);
  void operator=(const Self&);
};

