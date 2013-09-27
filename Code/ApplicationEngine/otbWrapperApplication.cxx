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
#include "otbWrapperApplication.h"

#include "otbWrapperChoiceParameter.h"
#include "otbWrapperListViewParameter.h"
#include "otbWrapperDirectoryParameter.h"
#include "otbWrapperEmptyParameter.h"
#include "otbWrapperInputFilenameParameter.h"
#include "otbWrapperInputFilenameListParameter.h"
#include "otbWrapperOutputFilenameParameter.h"
#include "otbWrapperOutputProcessXMLParameter.h"
#include "otbWrapperInputVectorDataParameter.h"
#include "otbWrapperInputVectorDataListParameter.h"
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperRadiusParameter.h"
#include "otbWrapperStringParameter.h"
#include "otbWrapperStringListParameter.h"
#include "otbWrapperInputImageListParameter.h"
#include "otbWrapperInputProcessXMLParameter.h"
#include "otbWrapperRAMParameter.h"

#include "otbWrapperParameterGroup.h"

#include "otbWrapperAddProcessToWatchEvent.h"


#include "otbWrapperTypes.h"
#include <exception>
#include "itkMacro.h"

namespace otb
{
namespace Wrapper
{

Application::Application()
  : m_Name(""),
    m_Description(""),
    m_Logger(itk::Logger::New()),
    m_ProgressSourceDescription(""),
    m_DocName(""),
    m_DocLongDescription(""),
    m_DocAuthors(""),
    m_DocLimitations(""),
    m_DocSeeAlso(""),
    m_DocTags(),
    m_HaveInXML(true),
    m_HaveOutXML(true)

{
  // Don't call Init from the constructor, since it calls a virtual method !

  m_Logger->SetName("Application.logger");
  m_Logger->SetPriorityLevel(itk::LoggerBase::DEBUG);
  m_Logger->SetLevelForFlushing(itk::LoggerBase::CRITICAL);
}

Application::~Application()
{
}

itk::Logger* Application::GetLogger()
{
  return m_Logger;
}

std::vector<std::string>
Application::GetParametersKeys(bool recursive)
{
  return GetParameterList()->GetParametersKeys(recursive);
}

bool Application::IsInitialized() const
{
  return m_ParameterList.IsNotNull() && m_DocExample.IsNotNull();
}

ParameterGroup* Application::GetParameterList()
{
  if (!IsInitialized())
    {
    Init();
    }

  return m_ParameterList;
}

Parameter* Application::GetParameterByKey(std::string name)
{
  return GetParameterList()->GetParameterByKey(name);
}

const Parameter* Application::GetParameterByKey(std::string name) const
{
  // GetParameterList is non const...
  Application* _this = const_cast<Application*>(this);
  return _this->GetParameterByKey(name);
}

void Application::Init()
{
  m_DocExample    = DocExampleStructure::New();
  m_ParameterList = ParameterGroup::New();
  this->DoInit();
  
  //rashad: global parameters. now used only for save xml
  if (this->GetHaveInXML())
    {
      AddInXMLParameter();
    }
  if (this->GetHaveOutXML())
    {
      AddOutXMLParameter();
    }
}

void Application::AddOutXMLParameter()
{
  Parameter::Pointer tmpParam;
  tmpParam = OutputProcessXMLParameter::New();
  const std::string key =   tmpParam->GetKey();
  const std::string descr = tmpParam->GetDescription();
  const std::string defaultXMLFileName = std::string(GetName())  + ".xml";
  tmpParam = NULL;

  AddParameter(ParameterType_OutputProcessXML,  key,   descr);
  SetParameterDescription(key, descr);
  MandatoryOff(key);
  //SetParameterString(key, defaultXMLFileName);
  DisableParameter(key);
}

void Application::AddInXMLParameter()
{
  Parameter::Pointer tmpParam;
  tmpParam = InputProcessXMLParameter::New();
  const std::string key =   tmpParam->GetKey();
  const std::string descr = tmpParam->GetDescription();
  const std::string defaultXMLFileName = std::string(GetName())  + ".xml";
  tmpParam = NULL;

  AddParameter(ParameterType_InputProcessXML,  key,   descr);
  SetParameterDescription(key, descr);
  MandatoryOff(key);
  //SetParameterString(key, defaultXMLFileName);
  DisableParameter(key);
}

void Application::UpdateParameters()
{
  this->DoUpdateParameters();
}

int Application::Execute()
{
  int ret = 0;

  //read application from xml
  std::string inXMLKey = "inxml";
  if ( m_HaveInXML && GetParameterType(inXMLKey) == ParameterType_InputProcessXML
      && IsParameterEnabled(inXMLKey)  )
    {
      Parameter* param = GetParameterByKey(inXMLKey);
      InputProcessXMLParameter* inXMLParam = dynamic_cast<InputProcessXMLParameter*>(param);
      int ret = inXMLParam->Read(this);
      if( ret != 0 )
        {
	  return -1;
        }

      this->UpdateParameters();
    }

  // before execute we set the seed of mersenne twister
  std::vector<std::string> paramList = GetParametersKeys(true);
  bool UseSpecificSeed = false;

  for (std::vector<std::string>::const_iterator it = paramList.begin(); it != paramList.end(); ++it)
    {
      std::string key = *it;
      if ((key.compare(0, 4, "rand") == 0) && HasValue("rand"))
	{
	  UseSpecificSeed = true;
      Parameter* param = GetParameterByKey(key);
      IntParameter* randParam = dynamic_cast<IntParameter*> (param);
      int seed = randParam->GetValue();
      itk::Statistics::MersenneTwisterRandomVariateGenerator::GetInstance()->SetSeed(seed);
      }
    }
  if (!UseSpecificSeed)
    {
      itk::Statistics::MersenneTwisterRandomVariateGenerator::GetInstance()->Initialize();
    }

  this->DoExecute();

  return ret;
}

int Application::ExecuteAndWriteOutput()
{
  int status = this->Execute();

  if (status == 0)
    {
      std::vector<std::string> paramList = GetParametersKeys(true);
      // First Get the value of the available memory to use with the
      // writer if a RAMParameter is set
      bool useRAM = false;
      unsigned int ram = 0;
      for (std::vector<std::string>::const_iterator it = paramList.begin();
	   it != paramList.end();
	   ++it)
      {
	std::string key = *it;

	if (GetParameterType(key) == ParameterType_RAM
	    && IsParameterEnabled(key))
	  {
	    Parameter* param = GetParameterByKey(key);
	    RAMParameter* ramParam = dynamic_cast<RAMParameter*>(param);
	    ram = ramParam->GetValue();
	    useRAM = true;
	  }
      }

      for (std::vector<std::string>::const_iterator it = paramList.begin();
	   it != paramList.end();
	   ++it)
	{
	  std::string key = *it;
	  if (GetParameterType(key) == ParameterType_OutputImage
	      && IsParameterEnabled(key) && HasValue(key) )
	    {
	      Parameter* param = GetParameterByKey(key);
	      OutputImageParameter* outputParam = dynamic_cast<OutputImageParameter*>(param);
	      outputParam->InitializeWriters();
	      if (useRAM)
		{
		  outputParam->SetRAMValue(ram);
		}
	      std::ostringstream progressId;
	      progressId << "Writing " << outputParam->GetFileName() << "...";
	      AddProcess(outputParam->GetWriter(), progressId.str());
	      outputParam->Write();
	    }
	  else if (GetParameterType(key) == ParameterType_OutputVectorData
		   && IsParameterEnabled(key) && HasValue(key) )
	    {
	      Parameter* param = GetParameterByKey(key);
	      OutputVectorDataParameter* outputParam = dynamic_cast<OutputVectorDataParameter*>(param);
	      outputParam->InitializeWriters();
	      std::ostringstream progressId;
	      progressId << "Writing " << outputParam->GetFileName() << "...";
	      AddProcess(outputParam->GetWriter(), progressId.str());
	      outputParam->Write();
	    }
	  else if (GetParameterType(key) == ParameterType_ComplexOutputImage
		   && IsParameterEnabled(key) && HasValue(key) )
	    {
	      Parameter* param = GetParameterByKey(key);
	      ComplexOutputImageParameter* outputParam = dynamic_cast<ComplexOutputImageParameter*>(param);
	      outputParam->InitializeWriters();
	      if (useRAM)
		{
		  outputParam->SetRAMValue(ram);
		}
	      std::ostringstream progressId;
	      progressId << "Writing " << outputParam->GetFileName() << "...";
	      AddProcess(outputParam->GetWriter(), progressId.str());
	      outputParam->Write();
	    }

	  //xml writer parameter
	  else if (m_HaveOutXML && GetParameterType(key) == ParameterType_OutputProcessXML
		   && IsParameterEnabled(key) && HasValue(key) )
	    {
	      Parameter* param = GetParameterByKey(key);
	      OutputProcessXMLParameter* outXMLParam = dynamic_cast<OutputProcessXMLParameter*>(param);
	      outXMLParam->Write(this);
	    }
	}
    }
  return status;
}

/* Enable the use of an optional parameter. Returns the previous state */
void Application::EnableParameter(std::string paramKey)
{
  Parameter* param = GetParameterByKey(paramKey);
  param->SetActive(true);
}

/* Disable the use of an optional parameter. Returns the previous state  */
void Application::DisableParameter(std::string paramKey)
{
  GetParameterByKey(paramKey)->SetActive(false);
}

/* Return the enable state of an optional parameter  */
bool Application::IsParameterEnabled(std::string paramKey) const
{
  return GetParameterByKey(paramKey)->GetActive();
}

/* Return true if the specified parameter is mandatory */
bool Application::IsMandatory(std::string paramKey) const
{
  return GetParameterByKey(paramKey)->GetMandatory();
}

void Application::MandatoryOn(std::string paramKey)
{
  GetParameterByKey(paramKey)->SetMandatory(true);
}

void Application::MandatoryOff(std::string paramKey)
{
  GetParameterByKey(paramKey)->SetMandatory(false);
}

/* Return true if the specified parameter was set automatically in the
 * application
 */
bool Application::HasAutomaticValue(std::string paramKey) const
{
  return GetParameterByKey(paramKey)->GetAutomaticValue();
}

void Application::AutomaticValueOn(std::string paramKey)
{
  GetParameterByKey(paramKey)->SetAutomaticValue(true);
}

void Application::AutomaticValueOff(std::string paramKey)
{
  GetParameterByKey(paramKey)->SetAutomaticValue(false);
}

/* Returns true if the parameter has an associated value provided externally
 *  (not automatically computed by the application) */
bool Application::HasUserValue(std::string paramKey) const
{
  return GetParameterByKey(paramKey)->HasUserValue();
}

/* If a user value was provided clear it and update the other parameters */
void Application::ClearValue(std::string paramKey)
{
  GetParameterByKey(paramKey)->ClearValue();
}

/* Returns true if the parameter has an associated value.
 * This value can be an automatically computed value or default value,
 * or a value set externally by user */
bool Application::HasValue(std::string paramKey) const
{
  return GetParameterByKey(paramKey)->HasValue();
}

/* Return the user level of access to a parameter */
UserLevel Application::GetParameterUserLevel(std::string paramKey) const
{
  return GetParameterByKey(paramKey)->GetUserLevel();
}


/* Return the role (input/output) of a parameter */
Role Application::GetParameterRole(std::string paramKey) const
{
  return GetParameterByKey(paramKey)->GetRole();
}

void Application::SetParameterEmpty(std::string paramKey, bool active)
{
  GetParameterByKey(paramKey)->SetActive(active);
}

bool Application::GetParameterEmpty(std::string paramKey)
{
  return GetParameterByKey(paramKey)->GetActive();
}

/* Return the role (input/output) of a parameter */
void Application::SetParameterRole(std::string paramKey, Role role)
{
  GetParameterByKey(paramKey)->SetRole(role);
}

/* Get the parameter type from its name */
ParameterType Application::GetParameterType(std::string paramKey) const
{
  const Parameter* param = GetParameterByKey(paramKey);

  ParameterType type;

  if (dynamic_cast<const ChoiceParameter*>(param))
    {
    type = ParameterType_Choice;
    }
  else if (dynamic_cast<const ListViewParameter*>(param))
    {
    type = ParameterType_ListView;
    }
  else if (dynamic_cast<const RadiusParameter*>(param))
    {
    type = ParameterType_Radius;
    }
  else if (dynamic_cast<const EmptyParameter*>(param))
    {
    type = ParameterType_Empty;
    }
 else if (dynamic_cast<const IntParameter*>(param))
    {
    type = ParameterType_Int;
    }
  else if (dynamic_cast<const FloatParameter*>(param))
    {
    type = ParameterType_Float;
    }
  else if (dynamic_cast<const InputFilenameParameter*>(param))
    {
    type = ParameterType_InputFilename;
    }
  else if (dynamic_cast<const InputFilenameListParameter*>(param))
    {
    type = ParameterType_InputFilenameList;
    }
  else if (dynamic_cast<const OutputFilenameParameter*>(param))
    {
    type = ParameterType_OutputFilename;
    }
  else if (dynamic_cast<const DirectoryParameter*>(param))
    {
    type = ParameterType_Directory;
    }
  else if (dynamic_cast<const InputImageParameter*>(param))
    {
    type = ParameterType_InputImage;
    }
 else if (dynamic_cast<const InputImageListParameter*>(param))
    {
    type = ParameterType_InputImageList;
    }
  else if (dynamic_cast<const ComplexInputImageParameter*>(param))
    {
    type = ParameterType_ComplexInputImage;
    }
  else if (dynamic_cast<const InputVectorDataParameter*>(param))
    {
    type = ParameterType_InputVectorData;
    }
  else if (dynamic_cast<const InputVectorDataListParameter*>(param))
    {
    type = ParameterType_InputVectorDataList;
    }
  else if (dynamic_cast<const OutputImageParameter*>(param))
    {
    type = ParameterType_OutputImage;
    }
    else if (dynamic_cast<const ComplexOutputImageParameter*>(param))
    {
    type = ParameterType_ComplexOutputImage;
    }
  else if (dynamic_cast<const OutputVectorDataParameter*>(param))
    {
    type = ParameterType_OutputVectorData;
    }
  else if (dynamic_cast<const StringParameter*>(param))
    {
    type = ParameterType_String;
    }
  else if (dynamic_cast<const StringListParameter*>(param))
    {
    type = ParameterType_StringList;
    }
  else if (dynamic_cast<const RAMParameter*>(param))
    {
    type = ParameterType_RAM;
    }
  else if (dynamic_cast<const ParameterGroup*>(param))
    {
    type = ParameterType_Group;
    }
  else if (dynamic_cast<const OutputProcessXMLParameter*>(param))
    {
    type = ParameterType_OutputProcessXML;
    }
  else if (dynamic_cast<const InputProcessXMLParameter*>(param))
    {
    type = ParameterType_InputProcessXML;
    }
  else
    {
    itkExceptionMacro(<< "Unknown parameter : " << paramKey);
    }

  return type;
}

std::vector<std::string> Application::GetChoiceKeys(std::string name)
{
  Parameter* param = GetParameterByKey(name);
  if (dynamic_cast<ChoiceParameter*>(param))
    {
    ChoiceParameter* paramChoice = dynamic_cast<ChoiceParameter*>(param);
    return paramChoice->GetChoiceKeys();
    }
  else if (dynamic_cast<ListViewParameter*>(param))
    {
    ListViewParameter* paramList = dynamic_cast<ListViewParameter*>(param);
    return paramList->GetChoiceKeys();
    }
  itkExceptionMacro(<< name << " is not a choice parameter");
}

std::vector<std::string> Application::GetChoiceNames(std::string name)
{
  Parameter* param = GetParameterByKey(name);
  if (dynamic_cast<ChoiceParameter*>(param))
    {
    ChoiceParameter* paramChoice = dynamic_cast<ChoiceParameter*>(param);
    return paramChoice->GetChoiceNames();
    }
  else if (dynamic_cast<ListViewParameter*>(param))
    {
    ListViewParameter* paramList = dynamic_cast<ListViewParameter*>(param);
    return paramList->GetChoiceNames();
    }
  itkExceptionMacro(<< name << " is not a choice parameter");
}

void Application::SetParameterInt(std::string parameter, int value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<IntParameter*>(param))
    {
    IntParameter* paramInt = dynamic_cast<IntParameter*>(param);
    paramInt->SetValue(value);
    }
  else if (dynamic_cast<FloatParameter*>(param))
    {
    FloatParameter* paramFloat = dynamic_cast<FloatParameter*>(param);
    paramFloat->SetValue(static_cast<float>(value));
    }
  else if (dynamic_cast<RadiusParameter*>(param))
    {
    RadiusParameter* paramRadius = dynamic_cast<RadiusParameter*>(param);
    paramRadius->SetValue(static_cast<unsigned int>(value));
    }
  else if (dynamic_cast<ChoiceParameter*>(param))
    {
    ChoiceParameter* paramChoice = dynamic_cast<ChoiceParameter*>(param);
    paramChoice->SetValue(value);
    }
}

void Application::SetParameterFloat(std::string parameter, float value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<FloatParameter*>(param))
    {
    FloatParameter* paramFloat = dynamic_cast<FloatParameter*>(param);
    paramFloat->SetValue(value);
    }
}

void Application::SetDefaultParameterInt(std::string parameter, int value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<RadiusParameter*>(param))
    {
    RadiusParameter* paramRadius = dynamic_cast<RadiusParameter*>(param);
    paramRadius->SetDefaultValue(value);
    paramRadius->SetValue(value);
    }
   else if (dynamic_cast<IntParameter*>(param))
    {
    IntParameter* paramInt = dynamic_cast<IntParameter*>(param);
    paramInt->SetDefaultValue(value);
    paramInt->SetValue(value);
    }
  else if (dynamic_cast<FloatParameter*>(param))
    {
    FloatParameter* paramFloat = dynamic_cast<FloatParameter*>(param);
    paramFloat->SetDefaultValue(static_cast<float>(value));
    paramFloat->SetValue(static_cast<float>(value));
    }
  else if (dynamic_cast<RAMParameter*>(param))
    {
    RAMParameter* paramRAM = dynamic_cast<RAMParameter*>(param);
    paramRAM->SetDefaultValue(static_cast<unsigned int>(value));
    paramRAM->SetValue(static_cast<unsigned int>(value));
    }
}

void Application::SetDefaultParameterFloat(std::string parameter, float value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<FloatParameter*>(param))
    {
    FloatParameter* paramFloat = dynamic_cast<FloatParameter*>(param);
    paramFloat->SetDefaultValue(value);
    paramFloat->SetValue(value);
    }
}

void Application::SetMinimumParameterIntValue(std::string parameter, int value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<IntParameter*>(param))
    {
    IntParameter* paramInt = dynamic_cast<IntParameter*>(param);
    paramInt->SetMinimumValue(value);
    }
 else
    itkExceptionMacro(<<parameter << "parameter can't be casted to int");
}

void Application::SetMaximumParameterIntValue(std::string parameter, int value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<IntParameter*>(param))
    {
    IntParameter* paramInt = dynamic_cast<IntParameter*>(param);
    paramInt->SetMaximumValue(value);
    }
  else
    itkExceptionMacro(<<parameter << "parameter can't be casted to int");

}

void Application::SetMinimumParameterFloatValue(std::string parameter, float value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<FloatParameter*>(param))
    {
    FloatParameter* paramFloat = dynamic_cast<FloatParameter*>(param);
    paramFloat->SetMinimumValue(value);
    }
 else
    itkExceptionMacro(<<parameter << "parameter can't be casted to float");
}

void Application::SetMaximumParameterFloatValue(std::string parameter, float value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<FloatParameter*>(param))
    {
    FloatParameter* paramFloat = dynamic_cast<FloatParameter*>(param);
    paramFloat->SetMaximumValue(value);
    }
  else
    itkExceptionMacro(<<parameter << "parameter can't be casted to float");

}


void Application::SetParameterString(std::string parameter, std::string value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<ChoiceParameter*>(param))
    {
    ChoiceParameter* paramDown = dynamic_cast<ChoiceParameter*>(param);
    paramDown->SetValue(value);
    }
  else if (dynamic_cast<ListViewParameter*>(param))
    {
    ListViewParameter* paramDown = dynamic_cast<ListViewParameter*>(param);
    paramDown->SetValue(value);
    }
  else if (dynamic_cast<StringParameter*>(param))
    {
    StringParameter* paramDown = dynamic_cast<StringParameter*>(param);
    paramDown->SetValue(value);
    }
  else if (dynamic_cast<InputFilenameParameter*>(param))
    {
    InputFilenameParameter* paramDown = dynamic_cast<InputFilenameParameter*>(param);
    paramDown->SetValue(value);
    }
  else if (dynamic_cast<OutputFilenameParameter*>(param))
    {
    OutputFilenameParameter* paramDown = dynamic_cast<OutputFilenameParameter*>(param);
    paramDown->SetValue(value);
    }
  else if (dynamic_cast<DirectoryParameter*>(param))
    {
    DirectoryParameter* paramDown = dynamic_cast<DirectoryParameter*>(param);
    paramDown->SetValue(value);
    }
 else if (dynamic_cast<FloatParameter*>(param))
    {
    FloatParameter* paramDown = dynamic_cast<FloatParameter*>(param);
    paramDown->SetValue(value);
    }
 else if (dynamic_cast<RadiusParameter*>(param))
    {
    RadiusParameter* paramDown = dynamic_cast<RadiusParameter*>(param);
    paramDown->SetValue(value);
    }
 else if (dynamic_cast<IntParameter*>(param))
    {
    IntParameter* paramDown = dynamic_cast<IntParameter*>(param);
    paramDown->SetValue(value);
    }
  else if (dynamic_cast<InputImageParameter*>(param))
    {
    InputImageParameter* paramDown = dynamic_cast<InputImageParameter*>(param);
    if ( !paramDown->SetFromFileName(value) )
      otbAppLogCRITICAL( <<"Invalid image filename " << value <<".");

    }
  else if (dynamic_cast<ComplexInputImageParameter*>(param))
    {
    ComplexInputImageParameter* paramDown = dynamic_cast<ComplexInputImageParameter*>(param);
    paramDown->SetFromFileName(value);
    }
  else if (dynamic_cast<InputVectorDataParameter*>(param))
    {
    InputVectorDataParameter* paramDown = dynamic_cast<InputVectorDataParameter*>(param);
    if ( !paramDown->SetFromFileName(value) )
      otbAppLogCRITICAL( <<"Invalid vector data filename " << value <<".");
    }
  else if (dynamic_cast<OutputImageParameter*>(param))
    {
    OutputImageParameter* paramDown = dynamic_cast<OutputImageParameter*>(param);
    paramDown->SetFileName(value);
    }
  else if (dynamic_cast<ComplexOutputImageParameter*>(param))
    {
    ComplexOutputImageParameter* paramDown = dynamic_cast<ComplexOutputImageParameter*>(param);
    paramDown->SetFileName(value);
    }
  else if (dynamic_cast<OutputVectorDataParameter*>(param))
    {
    OutputVectorDataParameter* paramDown = dynamic_cast<OutputVectorDataParameter*>(param);
    paramDown->SetFileName(value);
    }
  else if (dynamic_cast<RAMParameter*>(param))
    {
    RAMParameter* paramDown = dynamic_cast<RAMParameter*>(param);
    paramDown->SetValue(value);
    }
  else if (dynamic_cast<OutputProcessXMLParameter*>(param))
    {
    OutputProcessXMLParameter* paramDown = dynamic_cast<OutputProcessXMLParameter*>(param);
    paramDown->SetValue(value);
    }
  else if (dynamic_cast<InputProcessXMLParameter*>(param))
    {
    InputProcessXMLParameter* paramDown = dynamic_cast<InputProcessXMLParameter*>(param);
    paramDown->SetValue(value);
    }
}

void Application::SetParameterStringList(std::string parameter, std::vector<std::string> value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<InputImageListParameter*>(param))
    {
    InputImageListParameter* paramDown = dynamic_cast<InputImageListParameter*>(param);
    if( !paramDown->SetListFromFileName(value) )
    otbAppLogCRITICAL( <<"At least one image filename is invalid.");
    }
  else if (dynamic_cast<InputVectorDataListParameter*>(param))
     {
     InputVectorDataListParameter* paramDown = dynamic_cast<InputVectorDataListParameter*>(param);
     if( !paramDown->SetListFromFileName(value)  )
       otbAppLogCRITICAL( <<"At least one vector data filename is invalid..");
     }
  else if (dynamic_cast<InputFilenameListParameter*>(param))
     {
     InputFilenameListParameter* paramDown = dynamic_cast<InputFilenameListParameter*>(param);
     if( !paramDown->SetListFromFileName(value)  )
       otbAppLogCRITICAL( <<"At least one filename is invalid..");
     }
  else if (dynamic_cast<StringListParameter*>(param))
    {
    StringListParameter* paramDown = dynamic_cast<StringListParameter*>(param);
    paramDown->SetValue(value);
    }
  else if(dynamic_cast<ListViewParameter *>(param))
    {
    ListViewParameter * paramDown = dynamic_cast<ListViewParameter *>(param);
    paramDown->SetSelectedNames(value);
    }
}

void Application::SetParameterOutputImage(std::string parameter, FloatVectorImageType* value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<OutputImageParameter*>(param))
    {
    OutputImageParameter* paramDown = dynamic_cast<OutputImageParameter*>(param);
    paramDown->SetValue(value);
    }
}

void Application::SetParameterComplexOutputImage(std::string parameter, ComplexFloatVectorImageType* value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<ComplexOutputImageParameter*>(param))
    {
    ComplexOutputImageParameter* paramDown = dynamic_cast<ComplexOutputImageParameter*>(param);
    paramDown->SetValue(value);
    }
}

void Application::SetParameterOutputImagePixelType(std::string parameter, ImagePixelType pixelType)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<OutputImageParameter*>(param))
    {
    OutputImageParameter* paramDown = dynamic_cast<OutputImageParameter*>(param);
    paramDown->SetPixelType(pixelType);
    }
}

void Application::SetParameterComplexOutputImagePixelType(std::string parameter,
                                                          ComplexImagePixelType cpixelType)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<ComplexOutputImageParameter*>(param))
    {
    ComplexOutputImageParameter* paramDown = dynamic_cast<ComplexOutputImageParameter*>(param);
    paramDown->SetComplexPixelType(cpixelType);
    }
}
void Application::SetParameterOutputVectorData(std::string parameter, VectorDataType* value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<OutputVectorDataParameter*>(param))
    {
    OutputVectorDataParameter* paramDown = dynamic_cast<OutputVectorDataParameter*>(param);
    paramDown->SetValue(value);
    }
}

bool Application::IsUseXMLValue(std::string parameter)
{
  Parameter* param = GetParameterByKey(parameter);
  return param->IsUseXMLValue();
}
std::string Application::GetParameterName(std::string parameter)
{
  Parameter* param = GetParameterByKey(parameter);
  return param->GetName();
}

std::string Application::GetParameterDescription(std::string parameter)
{
  Parameter* param = GetParameterByKey(parameter);
  return param->GetDescription();
}

void Application::SetParameterDescription(std::string parameter, std::string desc)
{
  Parameter* param = GetParameterByKey(parameter);
  param->SetDescription(desc);
}

int Application::GetParameterInt(std::string parameter)
{
  int ret = 0;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<IntParameter*>(param))
    {
    IntParameter* paramInt = dynamic_cast<IntParameter*>(param);
    ret = paramInt->GetValue();
    }
  else if (dynamic_cast<FloatParameter*>(param))
    {
    FloatParameter* paramFloat = dynamic_cast<FloatParameter*>(param);
    ret = static_cast<int>(paramFloat->GetValue());
    }
  else if (dynamic_cast<RadiusParameter*>(param))
    {
    RadiusParameter* paramRadius = dynamic_cast<RadiusParameter*>(param);
    ret = paramRadius->GetValue();
    }
  else if (dynamic_cast<RAMParameter*>(param))
    {
    RAMParameter* paramRAM = dynamic_cast<RAMParameter*>(param);
    ret = paramRAM->GetValue();
    }
  else if (dynamic_cast<ChoiceParameter*>(param))
    {
    ChoiceParameter* paramChoice = dynamic_cast<ChoiceParameter*>(param);
    ret = paramChoice->GetValue();
    }
  else
    {
     itkExceptionMacro(<<parameter << "parameter can't be casted to int");
    }

  return ret;
}

float Application::GetParameterFloat(std::string parameter)
{
  float ret = 0.0;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<FloatParameter*> (param))
    {
    FloatParameter* paramFloat = dynamic_cast<FloatParameter*> (param);
    ret = paramFloat->GetValue();
    }
  else
    {
    itkExceptionMacro(<<parameter << "parameter can't be casted to float");
    }

  return ret;
}

std::string Application::GetParameterString(std::string parameter)
{
  std::string ret="";
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<ChoiceParameter*>(param))
    {
    ChoiceParameter* paramDown = dynamic_cast<ChoiceParameter*>(param);
    std::string choiceKey = paramDown->GetChoiceKey( paramDown->GetValue() );
    size_t lastPointPos = choiceKey.find_last_of('.');
    if(lastPointPos != std::string::npos)
      {
      ret = choiceKey.substr(lastPointPos);
        }
    else
      {
      ret = choiceKey;
      }
    }
  else if (dynamic_cast<ListViewParameter*>(param))
    {
    ListViewParameter* paramDown = dynamic_cast<ListViewParameter*>(param);
    std::string choiceKey = paramDown->GetChoiceKey( paramDown->GetValue() );
    size_t lastPointPos = choiceKey.find_last_of('.');
    if(lastPointPos != std::string::npos)
      {
      ret = choiceKey.substr(lastPointPos);
        }
    else
      {
      ret = choiceKey;
      }
    }
  else if (dynamic_cast<StringParameter*>(param))
    {
    StringParameter* paramDown = dynamic_cast<StringParameter*>(param);
    ret = paramDown->GetValue();
    }
  else if (dynamic_cast<InputFilenameParameter*>(param))
    {
    InputFilenameParameter* paramDown = dynamic_cast<InputFilenameParameter*>(param);
    ret = paramDown->GetValue();
    }
  else if (dynamic_cast<OutputFilenameParameter*>(param))
    {
    OutputFilenameParameter* paramDown = dynamic_cast<OutputFilenameParameter*>(param);
    ret = paramDown->GetValue();
    }
  else if (dynamic_cast<DirectoryParameter*>(param))
    {
    DirectoryParameter* paramDown = dynamic_cast<DirectoryParameter*>(param);
    ret = paramDown->GetValue();
    }
  else if (dynamic_cast<InputImageParameter*>(param))
    {
    InputImageParameter* paramDown = dynamic_cast<InputImageParameter*>(param);
    ret = paramDown->GetFileName();
    }
  else if (dynamic_cast<ComplexInputImageParameter*>(param))
    {
    ComplexInputImageParameter* paramDown = dynamic_cast<ComplexInputImageParameter*>(param);
    ret = paramDown->GetFileName();
    }
  else if (dynamic_cast<InputVectorDataParameter*>(param))
    {
    InputVectorDataParameter* paramDown = dynamic_cast<InputVectorDataParameter*>(param);
    ret = paramDown->GetFileName();
    }
  else if (dynamic_cast<OutputImageParameter*>(param))
    {
    OutputImageParameter* paramDown = dynamic_cast<OutputImageParameter*>(param);
    ret = paramDown->GetFileName();
    }
  else if (dynamic_cast<OutputVectorDataParameter*>(param))
    {
    OutputVectorDataParameter* paramDown = dynamic_cast<OutputVectorDataParameter*>(param);
    ret = paramDown->GetFileName();
    }
  else if (dynamic_cast<OutputProcessXMLParameter*>(param))
    {
    OutputProcessXMLParameter* paramDown = dynamic_cast<OutputProcessXMLParameter*>(param);
    ret = paramDown->GetFileName();
    }
  else
   {
    itkExceptionMacro(<<parameter << " : parameter can't be casted to string");
   }

  return ret;
}

std::vector<std::string> Application::GetParameterStringList(std::string parameter)
{
  std::vector<std::string> ret;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<InputImageListParameter*> (param))
    {
    InputImageListParameter* paramDown = dynamic_cast<InputImageListParameter*> (param);
    ret = paramDown->GetFileNameList();
    }
  else if (dynamic_cast<InputVectorDataListParameter*> (param))
      {
      InputVectorDataListParameter* paramDown = dynamic_cast<InputVectorDataListParameter*> (param);
      ret = paramDown->GetFileNameList();
      }
  else if (dynamic_cast<InputFilenameListParameter*> (param))
      {
      InputFilenameListParameter* paramDown = dynamic_cast<InputFilenameListParameter*> (param);
      ret = paramDown->GetFileNameList();
      }
  else if (dynamic_cast<StringListParameter*> (param))
      {
      StringListParameter* paramDown = dynamic_cast<StringListParameter*> (param);
      ret = paramDown->GetValue();
      }
  else
    {
    itkExceptionMacro(<<parameter << "parameter can't be casted to StringList");
    }

  return ret;
}


FloatVectorImageType* Application::GetParameterImage(std::string parameter)
{
  FloatVectorImageType::Pointer ret = NULL;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<InputImageParameter*> (param))
    {
    InputImageParameter* paramDown = dynamic_cast<InputImageParameter*> (param);
    ret = paramDown->GetImage();
    }
  else
    {
    itkExceptionMacro(<<parameter << "parameter can't be casted to ImageType");
    }

  return ret;
}

FloatVectorImageListType* Application::GetParameterImageList(std::string parameter)
{
  FloatVectorImageListType::Pointer ret=NULL;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<InputImageListParameter*>(param))
    {
    InputImageListParameter* paramDown = dynamic_cast<InputImageListParameter*>(param);
    ret = paramDown->GetImageList();
    }
  else
    {
    itkExceptionMacro(<<parameter << "parameter can't be casted to ImageListType");
    }

  return ret;
}

ComplexFloatVectorImageType* Application::GetParameterComplexImage(std::string parameter)
{
  ComplexFloatVectorImageType::Pointer ret=NULL;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<ComplexInputImageParameter*>(param))
    {
    ComplexInputImageParameter* paramDown = dynamic_cast<ComplexInputImageParameter*>(param);
    ret = paramDown->GetImage();
    }
  else
    {
    itkExceptionMacro(<<parameter << "parameter can't be casted to ComplexImageType");
    }

  return ret;
}

VectorDataType* Application::GetParameterVectorData(std::string parameter)
{
  VectorDataType::Pointer ret=NULL;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<InputVectorDataParameter*>(param))
    {
    InputVectorDataParameter* paramDown = dynamic_cast<InputVectorDataParameter*>(param);
    ret = paramDown->GetVectorData();
    }
  else
    {
    itkExceptionMacro(<<parameter << "parameter can't be casted to Vector Data");
    }
  return ret;
}

VectorDataListType* Application::GetParameterVectorDataList(std::string parameter)
{
  VectorDataListType::Pointer ret=NULL;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<InputVectorDataListParameter*>(param))
    {
    InputVectorDataListParameter* paramDown = dynamic_cast<InputVectorDataListParameter*>(param);
    ret = paramDown->GetVectorDataList();
    }
  else
   {
    itkExceptionMacro(<<parameter << "parameter can't be casted to Vector Data List");
   }
  return ret;
}


std::string Application::GetParameterAsString(std::string paramKey)
{
  std::string ret="";
  ParameterType type = this->GetParameterType( paramKey );

  if( type == ParameterType_String || type == ParameterType_InputFilename || type == ParameterType_OutputFilename
      || type == ParameterType_Directory || type == ParameterType_InputImage
      || type == ParameterType_ComplexInputImage || type == ParameterType_InputVectorData
      || type == ParameterType_OutputImage || type == ParameterType_OutputVectorData
      || type == ParameterType_ListView || type == ParameterType_Choice
      || type == ParameterType_OutputProcessXML )
    {
      ret = this->GetParameterString( paramKey );
    }
  else if ( type == ParameterType_Int || type == ParameterType_Radius
            || type == ParameterType_RAM)
    {
      std::ostringstream oss;
      oss << this->GetParameterInt( paramKey );
      ret = oss.str();
    }
  else if( type == ParameterType_Float )
    {
      std::ostringstream oss;
      //oss << std::setprecision(10);
      oss << this->GetParameterFloat( paramKey );
      ret = oss.str();
    }
  else if( type == ParameterType_StringList )
    {
      std::ostringstream oss;
      oss << std::setprecision(10);
      const std::vector<std::string> strList = this->GetParameterStringList( paramKey );
      for (unsigned int i=0; i<strList.size(); i++)
        oss << strList[i] << std::endl;
      ret = oss.str();
    }
  else
    {
      itkExceptionMacro(<<paramKey << " parameter can't be casted to string");
    }
   return ret;
}

ImagePixelType Application::GetParameterOutputImagePixelType(std::string parameter)
{
  Parameter* param = GetParameterByKey(parameter);
  ImagePixelType ret=ImagePixelType_uint8; //by default to avoid warning

  if (dynamic_cast<OutputImageParameter*>(param))
    {
    OutputImageParameter* paramDown = dynamic_cast<OutputImageParameter*>(param);
    ret = paramDown->GetPixelType();
    }
  else
    {
    itkExceptionMacro("Unable to find PixelType in parameter "<<parameter<<".");
    }

  return ret;
}

ComplexImagePixelType Application::GetParameterComplexOutputImagePixelType(std::string parameter)
{
  Parameter* param = GetParameterByKey(parameter);
  ComplexImagePixelType ret=ComplexImagePixelType_float;  //by default to avoid warning

  if (dynamic_cast<ComplexOutputImageParameter*>(param))
    {
    ComplexOutputImageParameter* paramDown = dynamic_cast<ComplexOutputImageParameter*>(param);
    ret = paramDown->GetComplexPixelType();
    }
  else
    {
    itkExceptionMacro("Unable to find PixelType in parameter "<<parameter<<".");
    }

  return ret;
}


void
Application::AddChoice(std::string paramKey, std::string paramName)
{
  GetParameterList()->AddChoice(paramKey, paramName);
}

void
Application::ClearChoices(std::string paramKey)
{
  GetParameterList()->ClearChoices(paramKey);
}

std::vector<int>
Application::GetSelectedItems(std::string param)
{
  return GetParameterList()->GetSelectedItems(param);
}

void
Application::AddParameter(ParameterType type, std::string paramKey, std::string paramName)
{
  GetParameterList()->AddParameter(type, paramKey, paramName);
}

void Application::AddRAMParameter(std::string paramKey, std::string paramName, unsigned int defaultValue)
{
  GetParameterList()->AddParameter(ParameterType_RAM, paramKey, paramName);
  SetDefaultParameterInt(paramKey, defaultValue);
  MandatoryOff(paramKey);
}

// paramKey default value = ram
void Application::AddRAMParameter(std::string paramKey)
{
  // Get the  RAM Parameter from the configuration file
  if (otb::ConfigurationFile::GetInstance()->IsValid() )
    {
    AddRAMParameter(paramKey,
                    "Available RAM (Mb)",
                    otb::ConfigurationFile::GetInstance()->GetAvailableRAMInMBytes());
    }
  else
    {
    // TODO check this
    AddRAMParameter(paramKey,
                    "Available RAM (Mb)",
                    128);
    }
  MandatoryOff(paramKey);
  SetParameterDescription(paramKey, "Available memory for processing (in MB)");
}

void Application::AddRANDParameter(std::string paramKey, std::string paramName, unsigned int defaultValue)
{
  GetParameterList()->AddParameter(ParameterType_Int, paramKey, paramName);
  SetDefaultParameterInt(paramKey, defaultValue);
  MandatoryOff(paramKey);
}

// paramKey default value = rand
void Application::AddRANDParameter(std::string paramKey)
{
  // Get the  RAND Parameter from the configuration file

  GetParameterList()->AddParameter(ParameterType_Int, paramKey, "set user defined seed");
  MandatoryOff(paramKey);
  SetParameterDescription(paramKey, "Set specific seed. with integer value.");

}


std::vector< std::pair<std::string, std::string> >
Application::GetOutputParametersSumUp()
{
  std::vector< std::pair<std::string, std::string> > res;
  std::vector<std::string> paramList = GetParametersKeys(true);
  for (std::vector<std::string>::const_iterator it = paramList.begin();
       it != paramList.end();
       ++it)
    {
    Parameter* param = GetParameterByKey(*it);
    ParameterType type = GetParameterType(*it);

    if ( type != ParameterType_Group )
      {
      if ( param->GetRole() == Role_Output && IsParameterEnabled(*it) )
        {
        std::pair<std::string, std::string> keyVal;
        keyVal.first = (*it);
        if (type == ParameterType_Float)
          {
          std::ostringstream oss;
          oss << std::setprecision(10);
          oss << GetParameterFloat(*it);
          keyVal.second = oss.str();
          }
        else
          {
          keyVal.second = GetParameterAsString(*it);
          }
        res.push_back( keyVal );
        }
      }
    }
  return res;
}

bool
Application::IsApplicationReady()
{
  // Check if all the mandatory parameters are set
  bool ready = true;

  std::vector<std::string> paramList = GetParametersKeys(true);
  for (std::vector<std::string>::const_iterator it = paramList.begin();
       it != paramList.end();
       ++it)
    {
    // Check all Input Parameters with Input Role
    if (GetParameterByKey(*it)->GetRole() == Role_Input)
      {
      // When a parameter is mandatory :
      // return false when does not have value and:
      //  - The param is root
      //  - The param is not root and belonging to a Mandatory Group
      //    wich is activated
      if ( !this->HasValue(*it)  && IsMandatory(*it) )
        {
        if( GetParameterByKey(*it)->IsRoot() )
          {
          otbDebugMacro("MISSING : "<< (*it).c_str() << " ( Is Root)");
          return false;
          }
        else
          {
          // check if the parameter is linked to a root parameter with a chain of active parameters
          Parameter* currentParam = GetParameterByKey(*it)->GetRoot();
          if (currentParam->IsRoot())
            {
            otbDebugMacro("MISSING : "<< (*it).c_str() << " ( Is Level 1)");
            return false;
            }

          int level = 1;

          while (!currentParam->IsRoot())
            {
            if (!currentParam->GetActive())
              {
              // the missing parameter is not on an active branch : we can ignore it
              break;
              }
            currentParam = currentParam->GetRoot();

            level++;

            if (currentParam->IsRoot())
              {
              // the missing parameter is on an active branch : we need it
              otbDebugMacro("MISSING : "<< (*it).c_str() << " ( Is Level "<< level<<")");
              return false;
              }
            }
          }
        }
      }
    }

  return ready;
}

void
Application::AddProcess(itk::ProcessObject* object, std::string description)
{
  m_ProgressSource = object;
  m_ProgressSourceDescription = description;

  AddProcessToWatchEvent event;
  event.SetProcess(object);
  event.SetProcessDescription(description);
  this->InvokeEvent(event);
}

itk::ProcessObject* Application::GetProgressSource() const
{
  return m_ProgressSource;
}

std::string Application::GetProgressDescription() const
{
  return m_ProgressSourceDescription;
}


}
}
