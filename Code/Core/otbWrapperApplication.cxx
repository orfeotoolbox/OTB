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
#include "otbWrapperDirectoryParameter.h"
#include "otbWrapperEmptyParameter.h"
#include "otbWrapperFilenameParameter.h"
#include "otbWrapperInputComplexImageParameter.h"
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperInputVectorDataParameter.h"
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperRadiusParameter.h"
#include "otbWrapperStringParameter.h"

#include "otbWrapperParameterGroup.h"

namespace otb
{
namespace Wrapper
{

Application::Application()
 : m_Name(""),
   m_Description(""),
   m_ThreadId1(0),
   m_ThreadId2(0),
   m_ThreadId3(0),
   m_ThreadId4(0),
   m_Thread1HasFinished(true),
   m_Thread2HasFinished(true),
   m_Thread3HasFinished(true),
   m_Thread4HasFinished(true)
{
  // Don't call Init from the constructor, since it calls a virtual method !
  m_Threader = ThreaderType::New();
  m_ProgressSourceList = ObjectListType::New();
}

Application::~Application()
{
}

void
Application
::StartProcess1()
{
  m_ThreadId1 = m_Threader->SpawnThread(RunProcess1_static, this);
}

void
Application
::StopProcess1()
{
  m_Threader->TerminateThread(m_ThreadId1);
}

void
Application
::StartProcess2()
{
  m_ThreadId2 = m_Threader->SpawnThread(RunProcess2_static, this);
}

void
Application
::StopProcess2()
{
  m_Threader->TerminateThread(m_ThreadId2);
}

void
Application
::StartProcess3()
{
  m_ThreadId3 = m_Threader->SpawnThread(RunProcess3_static, this);
}

void
Application
::StopProcess3()
{
  m_Threader->TerminateThread(m_ThreadId3);
}

void
Application
::StartProcess4()
{
  m_ThreadId4 = m_Threader->SpawnThread(RunProcess4_static, this);
}

void
Application
::StopProcess4()
{
  m_Threader->TerminateThread(m_ThreadId4);
}

std::vector<std::string>
Application::GetParametersKeys(bool recursive)
{
  return GetParameterList()->GetParametersKeys(recursive);
}

ParameterGroup* Application::GetParameterList()
{
  if (!m_ParameterList)
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
  m_ParameterList = ParameterGroup::New();
  this->DoCreateParameters();
}

void Application::UpdateParameters()
{
  this->DoUpdateParameters();
}

void Application::Execute()
{
  //this->DoExecute();
  std::cout<<"Application::Execute starts"<<std::endl;
  this->StartProcess1();

  if( m_ProgressSourceList->Size() == 0 )
    {
      this->StartProcess2();
    }
  std::cout<<"Application::Execute done"<<std::endl;
}

void Application::ExecuteAndWriteOutput()
{
  this->Execute();

  while( m_Thread2HasFinished == false || m_Thread2HasFinished == false)
    {
      this->Sleep(500);
    }
 
  this->StartProcess3();
  this->StartProcess4();
}


void Application::DoWriteOutput()
{
  std::vector<std::string> paramList = GetParametersKeys(true);
  for (std::vector<std::string>::const_iterator it = paramList.begin();
      it != paramList.end();
      ++it)
    {
    if (GetParameterType(*it) == ParameterType_OutputImage)
      {
      Parameter* param = GetParameterByKey(*it);
      OutputImageParameter* outputParam = dynamic_cast<OutputImageParameter*>(param);
      m_CurWriter = outputParam->GetWriter();
      outputParam->Write();
      }
    }
}

void Application::DoWatchExecute()
{
  std::cout<<"Application::DoWatchExecute starts"<<std::endl;
  /*
  double last = 0;
  double updateThres = 0.01;
  double current = -1;
  */
  unsigned int nbProcessDone = 0;  
  unsigned int nbProcess( m_ProgressSourceList->Size() );
  unsigned int nbCurProcess( nbProcess );

  while ( nbProcessDone < nbProcess &&  m_Thread1HasFinished == false)
    {
      itk::OStringStream oss;
      for( unsigned int i=0; i<nbProcess; i++ )
        {
          unsigned int progressPercent = static_cast<unsigned int>(m_ProgressSourceList->GetNthElement( i )->GetProgress()*100);
          std::string stars(progressPercent, '*');
          std::string blanks(100-progressPercent, ' ');
          oss<< "\rProcessing progress " << i <<": " <<  progressPercent << "% [" << stars << blanks << "]\n" ;
          //std::cout << m_ProgressSourceList->GetNthElement( i )->GetProgress() << std:endl;
          //std::cout << "\rProcessing progress: " << progressPercent << "% [" << stars << blanks << "]" << std::flush;
          if( m_ProgressSourceList->GetNthElement( i )->GetProgress()==1 )
            {
              nbProcessDone++;
            }
        }
      std::cout << oss.str() << std::endl;//flush;
      Sleep(500);
      
      /*
        current = m_ProcessObject->GetProgress();
        if (current - last > updateThres)
        {
        // Make the main fltk loop update progress fields
        m_View->AwakeProgressFields(m_ProcessObject->GetProgress());
        last = current;
        }
        }
        // Sleep for a while
        Sleep(500);
        m_ProcessObject = m_Model->GetProcessObjectModel();
      */
    }  
  std::cout<<"Application::DoWatchExecute done"<<std::endl;
}

void Application::DoWatchWrite()
{
  while( m_CurWriter.IsNotNull() && m_Thread3HasFinished==false )
    {
      unsigned int progressPercent = static_cast<unsigned int>(m_CurWriter->GetProgress()*100);
      std::string stars(progressPercent, '*');
      std::string blanks(100-progressPercent, ' ');
      std::cout << "\rProcessing progress: " << progressPercent << "% [" << stars << blanks << "]" << std::endl;//flush;

      Sleep(500);
    }  
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

/* Get the parameter type from its name */
ParameterType Application::GetParameterType(std::string paramKey) const
{
  const Parameter* param = GetParameterByKey(paramKey);
  ParameterType type;

  if (dynamic_cast<const ChoiceParameter*>(param))
    {
    type = ParameterType_Choice;
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
  else if (dynamic_cast<const FilenameParameter*>(param))
    {
    type = ParameterType_Filename;
    }
  else if (dynamic_cast<const DirectoryParameter*>(param))
    {
    type = ParameterType_Directory;
    }
  else if (dynamic_cast<const InputImageParameter*>(param))
    {
    type = ParameterType_InputImage;
    }
  else if (dynamic_cast<const InputComplexImageParameter*>(param))
    {
    type = ParameterType_InputComplexImage;
    }
  else if (dynamic_cast<const InputVectorDataParameter*>(param))
    {
    type = ParameterType_InputVectorData;
    }
  else if (dynamic_cast<const OutputImageParameter*>(param))
    {
    type = ParameterType_OutputImage;
    }
  else if (dynamic_cast<const OutputVectorDataParameter*>(param))
    {
    type = ParameterType_OutputVectorData;
    }
  else if (dynamic_cast<const StringParameter*>(param))
    {
    type = ParameterType_String;
    }
  else if (dynamic_cast<const ParameterGroup*>(param))
    {
    type = ParameterType_Group;
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
    paramRadius->SetValue(value);
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

void Application::SetParameterString(std::string parameter, std::string value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<ChoiceParameter*>(param))
    {
    ChoiceParameter* paramDown = dynamic_cast<ChoiceParameter*>(param);
    paramDown->SetValue(value);
    }
  else if (dynamic_cast<StringParameter*>(param))
    {
    StringParameter* paramDown = dynamic_cast<StringParameter*>(param);
    paramDown->SetValue(value);
    }
  else if (dynamic_cast<FilenameParameter*>(param))
    {
    FilenameParameter* paramDown = dynamic_cast<FilenameParameter*>(param);
    paramDown->SetValue(value);
    }
  else if (dynamic_cast<DirectoryParameter*>(param))
    {
    DirectoryParameter* paramDown = dynamic_cast<DirectoryParameter*>(param);
    paramDown->SetValue(value);
    }
  else if (dynamic_cast<InputImageParameter*>(param))
    {
    InputImageParameter* paramDown = dynamic_cast<InputImageParameter*>(param);
    paramDown->SetFromFileName(value);
    }
  else if (dynamic_cast<InputComplexImageParameter*>(param))
    {
    InputComplexImageParameter* paramDown = dynamic_cast<InputComplexImageParameter*>(param);
    paramDown->SetFromFileName(value);
    }
  else if (dynamic_cast<InputVectorDataParameter*>(param))
    {
    InputVectorDataParameter* paramDown = dynamic_cast<InputVectorDataParameter*>(param);
    paramDown->SetFromFileName(value);
    }
  else if (dynamic_cast<OutputImageParameter*>(param))
    {
    OutputImageParameter* paramDown = dynamic_cast<OutputImageParameter*>(param);
    paramDown->SetFileName(value);
    }
  else if (dynamic_cast<OutputVectorDataParameter*>(param))
    {
    OutputVectorDataParameter* paramDown = dynamic_cast<OutputVectorDataParameter*>(param);
    paramDown->SetFileName(value);
    }
}


void Application::SetParameterOutputImage(std::string parameter, VectorImageType* value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<OutputImageParameter*>(param))
    {
    OutputImageParameter* paramDown = dynamic_cast<OutputImageParameter*>(param);
    paramDown->SetValue(value);
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
  else if (dynamic_cast<ChoiceParameter*>(param))
    {
    ChoiceParameter* paramChoice = dynamic_cast<ChoiceParameter*>(param);
    ret = paramChoice->GetValue();
    }

  //TODO: exception if not found ?
  return ret;
}

float Application::GetParameterFloat(std::string parameter)
{
  float ret = 0;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<FloatParameter*>(param))
    {
    FloatParameter* paramFloat = dynamic_cast<FloatParameter*>(param);
    ret = paramFloat->GetValue();
    }

  //TODO: exception if not found ?
  return ret;
}

std::string Application::GetParameterString(std::string parameter)
{
  std::string ret;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<FilenameParameter*>(param))
    {
    FilenameParameter* paramDown = dynamic_cast<FilenameParameter*>(param);
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
  else if (dynamic_cast<InputComplexImageParameter*>(param))
    {
    InputComplexImageParameter* paramDown = dynamic_cast<InputComplexImageParameter*>(param);
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

  //TODO: exception if not found ?
  return ret;
}

VectorImageType* Application::GetParameterImage(std::string parameter)
{
  VectorImageType::Pointer ret;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<InputImageParameter*>(param))
    {
    InputImageParameter* paramDown = dynamic_cast<InputImageParameter*>(param);
    ret = paramDown->GetImage();
    }

  //TODO: exception if not found ?
  return ret;
}

VectorImageType* Application::GetParameterComplexImage(std::string parameter)
{
  VectorImageType::Pointer ret;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<InputComplexImageParameter*>(param))
    {
    InputComplexImageParameter* paramDown = dynamic_cast<InputComplexImageParameter*>(param);
    ret = paramDown->GetImage();
    }

  //TODO: exception if not found ?
  return ret;

}

VectorDataType* Application::GetParameterVectorData(std::string parameter)
{
  VectorDataType::Pointer ret;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<InputVectorDataParameter*>(param))
    {
    InputVectorDataParameter* paramDown = dynamic_cast<InputVectorDataParameter*>(param);
    ret = paramDown->GetVectorData();
    }

  //TODO: exception if not found ?
  return ret;
}


void
Application::AddChoice(std::string paramKey, std::string paramName)
{
  GetParameterList()->AddChoice(paramKey, paramName);
}


void
Application::AddParameter(ParameterType type, std::string paramKey, std::string paramName)
{
  GetParameterList()->AddParameter(type, paramKey, paramName);
}


}
}

