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
   m_Description("")
{
  // Don't call Init from the constructor, since it calls a virtual method !
}

Application::~Application()
{
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
  this->DoExecute();
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

  if (dynamic_cast<FilenameParameter*>(param))
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


void Application::SetParameterOutputImage(std::string parameter, OutputImageParameter::VectorImageType* value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<OutputImageParameter*>(param))
    {
    OutputImageParameter* paramDown = dynamic_cast<OutputImageParameter*>(param);
    paramDown->SetValue(value);
    }
}

void Application::SetParameterOutputVectorData(std::string parameter, OutputVectorDataParameter::VectorDataType* value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<OutputVectorDataParameter*>(param))
    {
    OutputVectorDataParameter* paramDown = dynamic_cast<OutputVectorDataParameter*>(param);
    paramDown->SetValue(value);
    }
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

InputImageParameter::VectorImageType* Application::GetParameterImage(std::string parameter)
{
  InputImageParameter::VectorImageType::Pointer ret;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<InputImageParameter*>(param))
    {
    InputImageParameter* paramDown = dynamic_cast<InputImageParameter*>(param);
    ret = paramDown->GetImage();
    }

  //TODO: exception if not found ?
  return ret;
}

InputComplexImageParameter::VectorImageType* Application::GetParameterComplexImage(std::string parameter)
{
  InputComplexImageParameter::VectorImageType::Pointer ret;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<InputComplexImageParameter*>(param))
    {
    InputComplexImageParameter* paramDown = dynamic_cast<InputComplexImageParameter*>(param);
    ret = paramDown->GetImage();
    }

  //TODO: exception if not found ?
  return ret;

}

InputVectorDataParameter::VectorDataType* Application::GetParameterVectorData(std::string parameter)
{
  InputVectorDataParameter::VectorDataType::Pointer ret;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<InputVectorDataParameter*>(param))
    {
    InputVectorDataParameter* paramDown = dynamic_cast<InputVectorDataParameter*>(param);
    ret = paramDown->GetVectorData();
    }

  //TODO: exception if not found ?
  return ret;
}




}
}

