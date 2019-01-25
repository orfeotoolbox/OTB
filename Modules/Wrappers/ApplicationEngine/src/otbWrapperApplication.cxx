/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#include "otbWrapperChoiceParameter.h"
#include "otbWrapperListViewParameter.h"
#include "otbWrapperDirectoryParameter.h"
#include "otbWrapperInputFilenameParameter.h"
#include "otbWrapperInputFilenameListParameter.h"
#include "otbWrapperOutputFilenameParameter.h"
#include "otbWrapperOutputProcessXMLParameter.h"
#include "otbWrapperInputVectorDataParameter.h"
#include "otbWrapperInputVectorDataListParameter.h"
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperRadiusParameter.h"
#include "otbWrapperStringListParameter.h"
#include "otbWrapperInputImageListParameter.h"
#include "otbWrapperInputProcessXMLParameter.h"
#include "otbWrapperRAMParameter.h"
#include "otbWrapperProxyParameter.h"
#include "otbWrapperParameterKey.h"
#include "otbWrapperBoolParameter.h"

#include "otbWrapperAddProcessToWatchEvent.h"

#include "otbCast.h"
#include "otbMacro.h"
#include "otbWrapperTypes.h"
#include <exception>
#include "itkMacro.h"
#include <stack>
#include <set>
#include <unordered_set>

namespace otb
{

ApplicationException::ApplicationException(const char *file, unsigned int line,
                   const char* message, const char* loc) :
ExceptionObject(file, line, message, loc)
{
}

/** Constructor. */
ApplicationException::ApplicationException(const std::string &file, unsigned int line,
                   const char* message, const char* loc) :
ExceptionObject(file, line, message, loc)
{
}

ApplicationException::ApplicationException(const std::string &file, unsigned int line,
                   const std::string& message, const std::string& loc) :
ExceptionObject(file, line, message, loc)
{
}

namespace Wrapper
{

void Application::SetName( const std::string & name )
{
  m_Name = name;
  GetDocExample()->SetApplicationName(name);
  this->Modified();
  m_Logger->SetName(name);
}

const char* Application::GetName() const
{
  return m_Name.c_str();
}

void Application::SetDescription(const std::string& description)
{
  m_Description = description;
  this->Modified();
}

const char* Application::GetDescription() const
{
  return m_Description.c_str();
}

void Application::SetHaveInXML(bool haveInXML)
{
  m_HaveInXML = haveInXML;
  this->Modified();
}

bool Application::GetHaveInXML() const
{
  return m_HaveInXML;
}

void Application::SetHaveOutXML(bool haveOutXML)
{
  m_HaveOutXML = haveOutXML;
  this->Modified();
}

bool Application::GetHaveOutXML() const
{
  return m_HaveOutXML;
}

void Application::SetDocName(const std::string& value)
{
  m_DocName = value;
  this->Modified();
}

const char* Application::GetDocName() const
{
  return m_DocName.c_str();
}

void Application::SetDocLongDescription(const std::string& value)
{
  m_DocLongDescription = value;
  this->Modified();
}

const char* Application::GetDocLongDescription() const
{
  return m_DocLongDescription.c_str();
}

void Application::SetDocAuthors(const std::string& value)
{
  m_DocAuthors = value;
  this->Modified();
}

const char* Application::GetDocAuthors() const
{
  return m_DocAuthors.c_str();
}

void Application::SetDocLimitations(const std::string& value)
{
  m_DocLimitations = value;
  this->Modified();
}

const char* Application::GetDocLimitations() const
{
  return m_DocLimitations.c_str();
}

void Application::SetDocSeeAlso(const std::string& value)
{
  m_DocSeeAlso = value;
  this->Modified();
}

const char* Application::GetDocSeeAlso() const
{
  return m_DocSeeAlso.c_str();
}

void Application::SetDocTags(std::vector<std::string> value)
{
  m_DocTags = value;
  this->Modified();
}

std::vector<std::string> Application::GetDocTags() const
{
  return m_DocTags;
}

void Application::AddDocTag(const std::string & tag)
{
  const auto wh = std::find(begin(m_DocTags), end(m_DocTags), tag);
  if (wh == end(m_DocTags))
    {
    m_DocTags.push_back(tag);
    this->Modified();
    }
}

DocExampleStructure::Pointer Application::GetDocExample()
{
  if (!IsInitialized())
  {
    Init();
  }

  return m_DocExample;
}

unsigned int Application::GetNumberOfExamples()
{
  return GetDocExample()->GetNbOfExamples();
}

std::string Application::GetExampleComment(unsigned int id)
{
  return GetDocExample()->GetExampleComment(id);
}

unsigned int Application::GetExampleNumberOfParameters(unsigned int id)
{
  return GetDocExample()->GetNumberOfParameters(id);
}

std::string Application::GetExampleParameterKey(unsigned int exId, unsigned int paramId)
{
  return GetDocExample()->GetParameterKey(paramId, exId);
}

std::string Application::GetExampleParameterValue(unsigned int exId, unsigned int paramId)
{
  return GetDocExample()->GetParameterValue(paramId, exId);
}

void Application::SetDocExampleParameterValue(const std::string key, const std::string value, unsigned int exId)
{
  GetDocExample()->AddParameter( key, value, exId );
  this->Modified();
}

void Application::SetExampleComment( const std::string & comm, unsigned int i )
{
  GetDocExample()->SetExampleComment( comm, i );
  this->Modified();
}

unsigned int Application::AddExample(const std::string & comm)
{
  unsigned int id = GetDocExample()->AddExample( comm );
  this->Modified();
  return id;
}

std::string Application::GetCLExample()
{
  return GetDocExample()->GenerateCLExample();
}

std::string Application::GetHtmlExample()
{
  return GetDocExample()->GenerateHtmlExample();
}

void Application::ForceInXMLParseFlag()
{
  m_IsInXMLParsed = false;
}

void Application::SetDocLink(const std::string & link)
{
  if (m_Doclink.compare(link) != 0)
  {
    m_Doclink = link;
    this->Modified();
  }
}

const std::string& Application::GetDocLink() const
{
  return m_Doclink;
}

void Application::SetOfficialDocLink()
{
  const std::string link = std::string("https://www.orfeo-toolbox.org/CookBook/Applications/app_") + this->GetName() + ".html";
  this->SetDocLink(link);
}

Application::Application()
  : m_Name(""),
    m_Description(""),
    m_Logger(otb::Logger::New()),
    m_ProgressSourceDescription(""),
    m_DocName(""),
    m_DocLongDescription(""),
    m_DocAuthors(""),
    m_DocLimitations(""),
    m_DocSeeAlso(""),
    m_DocTags(),
    m_Doclink(""),
    m_HaveInXML(true),
    m_HaveOutXML(true),
    m_IsInXMLParsed(false),
    m_IsInPrivateDo(false),
    m_ExecuteDone(false)
{
  // Don't call Init from the constructor, since it calls a virtual method !
  m_Logger->SetName("Application.logger");
}

Application::~Application()
{
}

otb::Logger* Application::GetLogger() const
{
  return m_Logger;
}

void Application::SetLogger(otb::Logger *logger)
{
  if (m_Logger != logger)
    {
    m_Logger = logger;
    }
  this->Modified();
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

Parameter* Application::GetParameterByKey(std::string name, bool follow)
{
  return GetParameterList()->GetParameterByKey(name, follow);
}

void Application::SetParameterInt(std::string parameter, int value, bool hasUserValueFlag)
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
  else if (dynamic_cast<BoolParameter*>(param))
    {
    BoolParameter* paramBool = dynamic_cast<BoolParameter*>(param);
    paramBool->SetValue(static_cast<bool>(value));
    }
  else
    {
    otbAppLogWARNING(<< "SetParameterInt on parameter " + parameter);
    }

  this->SetParameterUserValue(parameter, hasUserValueFlag);
}
void Application::SetParameterFloat(std::string parameter, float value, bool hasUserValueFlag)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<FloatParameter*>(param))
    {
    FloatParameter* paramFloat = dynamic_cast<FloatParameter*>(param);
    paramFloat->SetValue(value);
    }

  this->SetParameterUserValue(parameter, hasUserValueFlag);
}

void Application::SetParameterString(std::string parameter, std::string value, bool hasUserValueFlag)
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
    paramDown->SetFromFileName(value);
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
    if ( !paramDown->SetFileName(value) )
    otbAppLogCRITICAL( <<"Invalid XML parameter filename " << value <<".");
    }
  else if (dynamic_cast<BoolParameter*>(param))
    {
    BoolParameter* paramDown = dynamic_cast<BoolParameter*>(param);
    paramDown->SetValue(value);
    }
  else
    {
    otbAppLogWARNING( <<"This parameter can't be set using SetParameterString().");
    }

  this->SetParameterUserValue(parameter, hasUserValueFlag);
}

void Application::SetParameterStringList(std::string parameter, std::vector<std::string> values, bool hasUserValueFlag)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<InputImageListParameter*>(param))
    {
    InputImageListParameter* paramDown = dynamic_cast<InputImageListParameter*>(param);
    paramDown->SetListFromFileName( values );
    }
  else if (dynamic_cast<InputVectorDataListParameter*>(param))
    {
    InputVectorDataListParameter* paramDown = dynamic_cast<InputVectorDataListParameter*>(param);
    paramDown->SetListFromFileName( values );
    }
  else if (dynamic_cast<InputFilenameListParameter*>(param))
    {
    InputFilenameListParameter* paramDown = dynamic_cast<InputFilenameListParameter*>(param);
    paramDown->SetListFromFileName( values );
    }
  else if (dynamic_cast<StringListParameter*>(param))
    {
    StringListParameter* paramDown = dynamic_cast<StringListParameter*>(param);
    paramDown->SetValue(values);
    }
  else if(dynamic_cast<ListViewParameter *>(param))
    {
    ListViewParameter * paramDown = dynamic_cast<ListViewParameter *>(param);
    paramDown->SetSelectedNames(values);
    }

  this->SetParameterUserValue(parameter, hasUserValueFlag);
}

void Application::SetParameterUserValue(std::string paramKey, bool value)
{
  /** UserValue is set/unset parameter must be active.
  Can't set the m_Active flg in Parameter::SetUserValue() instead of
  using Application::EnableParameter();
  **/
  EnableParameter(paramKey);
  if (m_IsInPrivateDo)
    {
    GetParameterByKey(paramKey)->SetUserValue(false);
    }
  else
    {
    GetParameterByKey(paramKey)->SetUserValue(value);
    }
}

const Parameter* Application::GetParameterByKey(std::string name, bool follow) const
{
  // GetParameterList is non const...
  Application* _this = const_cast<Application*>(this);
  return _this->GetParameterByKey(name,follow);
}

void Application::Init()
{
  m_DocExample    = DocExampleStructure::New();
  m_ParameterList = ParameterGroup::New();
  //reset inXML parse checker in case if reinit-ing
  m_IsInXMLParsed = false;
  m_IsInPrivateDo = true;
  this->DoInit();
  m_IsInPrivateDo = false;

  //rashad: global parameters. now used only for inxml and outxml
  if(this->GetHaveInXML())
    {
    AddInXMLParameter();
    }
  if(this->GetHaveOutXML())
    {
    AddOutXMLParameter();
    }
}

void Application::UpdateParameters()
{
  //read application from xml only once m_IsInXMLParsed is in charge of it.
  std::string inXMLKey = "inxml";
  if (m_HaveInXML && !m_IsInXMLParsed)
    {
    if ( GetParameterType(inXMLKey) == ParameterType_InputProcessXML  &&
         IsParameterEnabled(inXMLKey) && HasValue(inXMLKey) )
      {
      Parameter* param = GetParameterByKey(inXMLKey);
      InputProcessXMLParameter* inXMLParam = dynamic_cast<InputProcessXMLParameter*>(param);
      if(inXMLParam!=nullptr)
        {
        // switch on 'm_IsInXMLParsed' before Read() to avoid cyclic calls
        m_IsInXMLParsed = true;
        inXMLParam->Read(this);
        }
      }
    }
  m_IsInPrivateDo = true;
  this->DoUpdateParameters();
  m_IsInPrivateDo = false;
  // reset the flag m_ExecuteDone
  m_ExecuteDone = false;
}

void Application::AfterExecuteAndWriteOutputs()
{}

void
Application::RegisterPipeline()
{
  std::stack< itk::DataObject * > dataStack;
  std::set< itk::DataObject * > inputData;
  std::vector<std::string> paramList = GetParametersKeys(true);
  // Get both end of the pipeline
  for ( auto const & key : paramList )
    {
    if ( GetParameterType(key) == ParameterType_OutputImage )
      {
      Parameter* param = GetParameterByKey(key);
      OutputImageParameter * outP = 
        dynamic_cast< OutputImageParameter * >( param );
      itk::ImageBase< 2 > * outData = outP->GetValue();
      if ( outData )
        dataStack.push(outData);
      }
    else if ( GetParameterType(key) == ParameterType_OutputVectorData )
      {
      Parameter* param = GetParameterByKey(key);
      OutputVectorDataParameter * outP = 
        dynamic_cast< OutputVectorDataParameter * >( param );
      VectorDataType * outData = outP->GetValue();
      if ( outData )
        dataStack.push(outData);
      }
    else if ( GetParameterType(key) == ParameterType_InputImage )
      {
      Parameter* param = GetParameterByKey(key);
      InputImageParameter * inP = 
        dynamic_cast< InputImageParameter * >( param );
      if ( !inP->HasValue() )
        continue;
      ImageBaseType * inData = inP->GetImage< ImageBaseType >();
      if ( inData && !inputData.count(inData) )
        inputData.insert(inData);
      }
    else if ( GetParameterType(key) == ParameterType_InputImageList )
      {
      Parameter * param = GetParameterByKey(key);
      InputImageListParameter * inP = 
        dynamic_cast< InputImageListParameter * > ( param );
      if ( !inP->HasValue() )
        continue;
      const FloatVectorImageListType * list = inP->GetImageList();
      for ( auto it = list->Begin() ; it != list->End() ; ++it ) 
        {
        FloatVectorImageType * inData = it.Get().GetPointer();
        if ( inData && !inputData.count(inData) )
          inputData.insert(inData);
        }
      }
    else if ( GetParameterType(key) == ParameterType_InputVectorData )
      {
      Parameter * param = GetParameterByKey(key);
      InputVectorDataParameter * inP =
        dynamic_cast< InputVectorDataParameter * > ( param );
      if ( !inP->HasValue() )
        continue;
      VectorDataType * inData = inP->GetVectorData();
      if ( inData && !inputData.count(inData) )
        inputData.insert(inData);
      }
    else if ( GetParameterType(key) == ParameterType_InputVectorDataList )
      {
      Parameter * param = GetParameterByKey(key);
      InputVectorDataListParameter * inP =
        dynamic_cast< InputVectorDataListParameter * > ( param );
      if ( !inP->HasValue() )
        continue;
      VectorDataListType * list = inP->GetVectorDataList();
      for ( auto it = list->Begin() ; it != list->End() ; ++it )
        {
        VectorDataType * inData = it.Get().GetPointer();
        if ( inData && !inputData.count(inData) )
          inputData.insert(inData);
        }
      }
    }

  // DFS
  while ( !dataStack.empty() )
    {
    itk::DataObject * current = dataStack.top();
    dataStack.pop();
    // whether current = null or is an input data it has no source
    if ( !current || inputData.count( current ) )
      continue;
    // if current is a list push every of its members in datastack
    if ( dynamic_cast< DataObjectListInterface *> (current) )
      {
      DataObjectListInterface * list = 
        dynamic_cast< DataObjectListInterface *> (current);
      int length = list->Size();
      for ( int i = 0 ; i < length ; i++ )
        {
        itk::DataObject * newData = list->GetNthDataObject(i);
        if ( !current || inputData.count( current ) )
          continue;
        dataStack.push( newData );
      continue;
        }
      }
    // Finally get the current's process object source
    itk::ProcessObject * process = (current->GetSource()).GetPointer();
    if ( !process || m_Filters.find( process ) != m_Filters.end() )
      continue;
    m_Filters.insert( process );
    std::vector< itk::DataObject::Pointer > inputs = process->GetInputs();
    // Push back all source's inputs in datastack
    for ( auto const & it : inputs )
      {
      if ( inputData.count( it.GetPointer() ) )
        continue;
      dataStack.push( it.GetPointer() );
      }
    }
}

void Application::FreeRessources()
{
  std::set< itk::DataObject * > dataSetToRelease; // do not release output
  std::set< itk::DataObject * > dataSet;
  std::vector<std::string> paramList = GetParametersKeys(true);
  // Get the end of the pipeline
  for ( const auto & key : paramList )
    {
    if ( GetParameterType(key) == ParameterType_OutputImage )
      {
      Parameter* param = GetParameterByKey(key);
      OutputImageParameter * outP = dynamic_cast<OutputImageParameter*>(param);
      itk::ImageBase<2> * outData = outP->GetValue();
      if ( outData )
        dataSet.insert(outData);
      }
    else if ( GetParameterType(key) == ParameterType_OutputVectorData )
      {
      Parameter* param = GetParameterByKey(key);
      OutputVectorDataParameter * outP = dynamic_cast<OutputVectorDataParameter*>(param);
      Wrapper::VectorDataType * outData = outP->GetValue();
      if ( outData )
        dataSet.insert(outData);
      }
    else
      continue;
    }
  // initialize DFS
  std::stack< itk::ProcessObject * > processStack;
  for ( auto data : dataSet )
    {
    auto process = (data->GetSource()).GetPointer();
    if ( process )
      processStack.push( process );
    }
  // DFS
  while ( !processStack.empty() )
    {
    itk::ProcessObject * current = processStack.top();
    processStack.pop();
    // if null continue
    if ( !current )
      continue;
    // Get all inputs
    auto inputVector = current->GetInputs();
    for ( auto data : inputVector )
      {
      // If input is null or already in the set continue
      if ( !data.GetPointer() || dataSet.count( data.GetPointer() ) )
        continue;
      // If input is a list
      if ( dynamic_cast< DataObjectListInterface *> (data.GetPointer()) )
        {
        DataObjectListInterface * list = 
          dynamic_cast< DataObjectListInterface *> (data.GetPointer());
        int length = list->Size();
        for ( int i = 0 ; i < length ; i++ )
          {
          itk::DataObject * newData = list->GetNthDataObject(i);
          if ( !newData || dataSet.count( newData ) )
            continue;
          dataSet.insert( newData );
          dataSetToRelease.insert( newData );
          itk::ProcessObject * process = newData->GetSource().GetPointer();
          if ( process )
            processStack.push( process );
          }
        }
      else
        {
        dataSet.insert( data.GetPointer() );
        dataSetToRelease.insert( data.GetPointer() );
        itk::ProcessObject * process = data->GetSource().GetPointer();
        if ( process )
          processStack.push( process );
        }
      }
    }
  // Release data
  for ( auto data : dataSetToRelease )
  {
    data->ReleaseData();
  }
  // Call override method
  DoFreeRessources();
}

int Application::Execute()
{
  //----------- Recursive part -------------------------------------------------
  std::vector<std::string> paramList = GetParametersKeys(true);
  int status=0;
  std::unordered_set<Application*> targetApps;
  for (std::vector<std::string>::const_iterator it = paramList.begin(); it != paramList.end(); ++it)
    {
    std::string key = *it;
    Parameter* param = GetParameterByKey(key);
    InputImageParameter* imgParam = dynamic_cast<InputImageParameter*>(param);
    
    if(imgParam)
      {
      Application::Pointer targetApp = otb::DynamicCast<Application>(imgParam->GetConnection().app);
      if(targetApp.IsNotNull() && !targetApp->IsExecuteDone())
        {
        targetApps.insert(targetApp);
        }
      }
    else
      {
      InputImageListParameter* imgListParam = dynamic_cast<InputImageListParameter*>(param);
      if (imgListParam)
        {
        for (unsigned int i=0 ; i<imgListParam->Size(); i++)
          {
          Application::Pointer targetApp = otb::DynamicCast<Application>(imgListParam->GetNthElement(i)->GetConnection().app);
          if(targetApp.IsNotNull() && !targetApp->IsExecuteDone())
            {
            targetApps.insert(targetApp);
            }
          }
        }
      } 
    }
  for (auto &app : targetApps)
    {
    // Call target Execute()
    status = status | app->Execute();
    }
  for (std::vector<std::string>::const_iterator it = paramList.begin(); it != paramList.end(); ++it)
    {
    std::string key = *it;
    Parameter* param = GetParameterByKey(key);
    InputImageParameter* imgParam = dynamic_cast<InputImageParameter*>(param);
    if(imgParam)
      {
      Application::Pointer targetApp = otb::DynamicCast<Application>(imgParam->GetConnection().app);
      if(targetApp.IsNotNull())
        {
        std::string outKey = imgParam->GetConnection().key;
        if(imgParam->GetConnection().isMem || !targetApp->HasValue(outKey))
          {
          // memory connection
          SetParameterInputImage(key,
            targetApp->GetParameterOutputImage(outKey));
          targetApp->DisableParameter(outKey);
          }
        else
          {
          // set input string based on out image
          SetParameterString(key, targetApp->GetParameterString(outKey));
          targetApp->EnableParameter(outKey);
          }
        }
      }
    else
      {
      InputImageListParameter* imgListParam = dynamic_cast<InputImageListParameter*>(param);
      if (imgListParam)
        {
        for (unsigned int i=0 ; i<imgListParam->Size() ; i++)
          {
          Application::Pointer targetApp = otb::DynamicCast<Application>(imgListParam->GetNthElement(i)->GetConnection().app);
          if(targetApp.IsNotNull())
            {
            std::string outKey = imgListParam->GetNthElement(i)->GetConnection().key;
            if(imgListParam->GetNthElement(i)->GetConnection().isMem ||
               !targetApp->HasValue(outKey))
              {
              // memory connection
              SetNthParameterInputImageList(key,i,
                targetApp->GetParameterOutputImage(outKey));
              targetApp->DisableParameter(outKey);
              }
            else
              {
              // set input string based on out image
              SetNthParameterStringList(key, i, targetApp->GetParameterString(outKey));
              targetApp->EnableParameter(outKey);
              }
            }
          }
        }
      }
    }
  if (status != 0)
    {
    return status;
    }
  for (auto &app : targetApps)
    {
    app->WriteOutput();
    }

  //------------------------------------------------------------
  this->UpdateParameters();

  // before execute we set the seed of mersenne twister
  
  bool UseSpecificSeed = false;

  for (std::vector<std::string>::const_iterator it = paramList.begin(); it != paramList.end(); ++it)
    {
      std::string key = *it;
      if ((key.compare(0, 4, "rand") == 0) && HasValue("rand"))
       {
         UseSpecificSeed = true;
      Parameter* param = GetParameterByKey(key);
      IntParameter* randParam = dynamic_cast<IntParameter*> (param);
      if(randParam!=nullptr)
        {
        int seed = randParam->GetValue();
        itk::Statistics::MersenneTwisterRandomVariateGenerator::GetInstance()->SetSeed(seed);
        }
      }
    }
  if (!UseSpecificSeed)
    {
      itk::Statistics::MersenneTwisterRandomVariateGenerator::GetInstance()->Initialize();
    }

  m_IsInPrivateDo = true;
  this->DoExecute();
  m_IsInPrivateDo = false;
  m_ExecuteDone = true;
  
  // Ensure that all output image parameter have called UpdateOutputInformation()
  for (auto it = paramList.begin(); it != paramList.end(); ++it)
    {
    OutputImageParameter * outImgParamPtr = dynamic_cast<OutputImageParameter *>(GetParameterByKey(*it));
    // If this is an OutputImageParameter
    if(outImgParamPtr != nullptr)
      {
      // If the parameter is enabled
      if(IsParameterEnabled(*it))
        {
        // Call UpdateOutputInformation()
        outImgParamPtr->GetValue()->UpdateOutputInformation();
        }
      }
    }

  return 0;
}

void Application::WriteOutput()
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
      if(ramParam!=nullptr)
        {
        ram = ramParam->GetValue();
        useRAM = true;
        }
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

      if(outputParam!=nullptr)
        {
        std::string checkReturn = outputParam->CheckFileName(true);
        if (!checkReturn.empty())
          {
          otbAppLogWARNING("Check filename: "<<checkReturn);
          }
        if (useRAM)
          {
          outputParam->SetRAMValue(ram);
          }
        outputParam->InitializeWriters();
        std::ostringstream progressId;
        progressId << "Writing " << outputParam->GetFileName() << "...";
        AddProcess(outputParam->GetWriter(), progressId.str());
        outputParam->Write();
        }
      }
    else if (GetParameterType(key) == ParameterType_OutputVectorData
             && IsParameterEnabled(key) && HasValue(key) )
      {
      Parameter* param = GetParameterByKey(key);
      OutputVectorDataParameter* outputParam = dynamic_cast<OutputVectorDataParameter*>(param);
      if(outputParam!=nullptr)
        {
        outputParam->InitializeWriters();
        std::ostringstream progressId;
        progressId << "Writing " << outputParam->GetFileName() << "...";
        AddProcess(outputParam->GetWriter(), progressId.str());
        outputParam->Write();
        }
      }
    else if (GetParameterType(key) == ParameterType_ComplexOutputImage
             && IsParameterEnabled(key) && HasValue(key) )
      {
      Parameter* param = GetParameterByKey(key);
      ComplexOutputImageParameter* outputParam = dynamic_cast<ComplexOutputImageParameter*>(param);

      if(outputParam!=nullptr)
        {
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
      }
    //xml writer parameter
    else if (m_HaveOutXML && GetParameterType(key) == ParameterType_OutputProcessXML
             && IsParameterEnabled(key) && HasValue(key) )
      {
      Parameter* param = GetParameterByKey(key);
      OutputProcessXMLParameter* outXMLParam = dynamic_cast<OutputProcessXMLParameter*>(param);
      if(outXMLParam!=nullptr)
        {
        outXMLParam->Write(this);
        }
      }
    }
}

int Application::ExecuteAndWriteOutput()
{
  m_Chrono.Restart();
  // reset the flag m_ExecuteDone
  m_ExecuteDone = false;

  m_Logger->LogSetupInformation();

  int status = this->Execute();

  if (status == 0)
    {
    this->WriteOutput();
    }

  this->AfterExecuteAndWriteOutputs();
  m_Chrono.Stop();
  
  FreeRessources();
  m_Filters.clear();
  return status;
}

void
Application::Stop()
{
  m_ProgressSource->SetAbortGenerateData(true);
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
bool Application::IsParameterEnabled(std::string paramKey, bool recurseParents) const
{
  return GetParameterByKey(paramKey)->GetActive(recurseParents);
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
  else if (dynamic_cast<const BoolParameter*>(param))
    {
    type = ParameterType_Bool;
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


void Application::SetDefaultParameterInt(std::string parameter, int value)
{
  Parameter* param = GetParameterByKey(parameter);
  bool hasUserValue = param->HasUserValue();

  if (dynamic_cast<RadiusParameter*>(param))
    {
    RadiusParameter* paramRadius = dynamic_cast<RadiusParameter*>(param);
    paramRadius->SetDefaultValue(value);
    if (!hasUserValue) paramRadius->SetValue(value);
    }
   else if (dynamic_cast<IntParameter*>(param))
    {
    IntParameter* paramInt = dynamic_cast<IntParameter*>(param);
    paramInt->SetDefaultValue(value);
    if (!hasUserValue) paramInt->SetValue(value);
    }
  else if (dynamic_cast<FloatParameter*>(param))
    {
    FloatParameter* paramFloat = dynamic_cast<FloatParameter*>(param);
    paramFloat->SetDefaultValue(static_cast<float>(value));
    if (!hasUserValue) paramFloat->SetValue(static_cast<float>(value));
    }
  else if (dynamic_cast<RAMParameter*>(param))
    {
    RAMParameter* paramRAM = dynamic_cast<RAMParameter*>(param);
    paramRAM->SetDefaultValue(static_cast<unsigned int>(value));
    if (!hasUserValue) paramRAM->SetValue(static_cast<unsigned int>(value));
    }
}

int Application::GetDefaultParameterInt(std::string parameter)
{
  Parameter* param = GetParameterByKey(parameter);
  int ret = 0 ;
  if (dynamic_cast<RadiusParameter*>(param))
    {
    RadiusParameter* paramRadius = dynamic_cast<RadiusParameter*>(param);
    ret = paramRadius->GetDefaultValue();
    }
   else if (dynamic_cast<IntParameter*>(param))
    {
    IntParameter* paramInt = dynamic_cast<IntParameter*>(param);
    ret = paramInt->GetDefaultValue();
    }
  else if (dynamic_cast<FloatParameter*>(param))
    {
    FloatParameter* paramFloat = dynamic_cast<FloatParameter*>(param);
    ret = paramFloat->GetDefaultValue();
    }
  else if (dynamic_cast<RAMParameter*>(param))
    {
    RAMParameter* paramRAM = dynamic_cast<RAMParameter*>(param);
    ret = paramRAM->GetDefaultValue();
    }
  else
    {
    // log
    }
  return ret;
}

void Application::SetDefaultParameterFloat(std::string parameter, float value)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<FloatParameter*>(param))
    {
    FloatParameter* paramFloat = dynamic_cast<FloatParameter*>(param);
    paramFloat->SetDefaultValue(value);
    if (!param->HasUserValue()) paramFloat->SetValue(value);
    }
}

float Application::GetDefaultParameterFloat(std::string parameter)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<FloatParameter*>(param))
    {
    FloatParameter* paramFloat = dynamic_cast<FloatParameter*>(param);
    return paramFloat->GetDefaultValue();
    }
  return 0;
}

void Application::SetDefaultOutputPixelType(std::string parameter, ImagePixelType type)
{
  Parameter* param = GetParameterByKey(parameter);
  OutputImageParameter* paramDown = dynamic_cast<OutputImageParameter*>(param);
  if (paramDown)
    {
    paramDown->SetDefaultPixelType(type);
    paramDown->SetPixelType(type);
    }
}

void
Application::SetDefaultOutputComplexPixelType(std::string parameter, ComplexImagePixelType type)
{
  Parameter* param = GetParameterByKey(parameter);
  ComplexOutputImageParameter* paramDown = dynamic_cast<ComplexOutputImageParameter*>(param);
  if (paramDown)
    {
    paramDown->SetDefaultComplexPixelType(type);
    paramDown->SetComplexPixelType(type);
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
    itkExceptionMacro(<<parameter << " parameter can't be casted to int");
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
    itkExceptionMacro(<<parameter << " parameter can't be casted to int");

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
    itkExceptionMacro(<<parameter << " parameter can't be casted to float");
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
    itkExceptionMacro(<<parameter << " parameter can't be casted to float");

}

void Application::SetListViewSingleSelectionMode(std::string parameter, bool status)
{
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<ListViewParameter*>(param))
    {
    ListViewParameter* paramListView = dynamic_cast<ListViewParameter*>(param);
    paramListView->SetSingleSelection(status);
    }
  else
    itkExceptionMacro(<<parameter << " parameter can't be casted to ListView");

}

bool Application::GetListViewSingleSelectionMode(const std::string& parameter)
{
  Parameter* param = GetParameterByKey(parameter);

  ListViewParameter* paramListView = dynamic_cast<ListViewParameter*>(param);
  if (paramListView)
    {
    return paramListView->GetSingleSelection();
    }
  else
    {
    itkExceptionMacro(<<parameter << " parameter can't be casted to ListView");
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

std::string Application::GetParameterName(std::string parameter)
{
  // get the actual parameter, even if it is a proxy
  Parameter* param = GetParameterByKey(parameter,false);
  return param->GetName();
}

std::string Application::GetParameterDescription(std::string parameter)
{
  // get the actual parameter, even if it is a proxy
  Parameter* param = GetParameterByKey(parameter,false);
  return param->GetDescription();
}

void Application::SetParameterDescription(std::string parameter, std::string desc)
{
  // get the actual parameter, even if it is a proxy
  Parameter* param = GetParameterByKey(parameter,false);
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
  else if (dynamic_cast<BoolParameter*>(param))
    {
    BoolParameter* paramBool = dynamic_cast<BoolParameter*>(param);
    ret = static_cast<int>(paramBool->GetValue());
    }
  else
    {
     itkExceptionMacro(<<parameter << " parameter can't be casted to int");
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
    itkExceptionMacro(<<parameter << " parameter can't be casted to float");
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
    if (paramDown->HasValue())
    {
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
    else
    {
        ret = "";
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
  else if (dynamic_cast<InputProcessXMLParameter*>(param))
    {
    InputProcessXMLParameter* paramDown = dynamic_cast<InputProcessXMLParameter*>(param);
    ret = paramDown->GetFileName();
    }
  else if (dynamic_cast<BoolParameter*>(param))
    {
    BoolParameter* paramDown = dynamic_cast<BoolParameter*>(param);
    ret = paramDown->GetValueAsString();
    }
  else
   {
    itkExceptionMacro(<<parameter << " : parameter can't be casted to string");
   }

  return ret;
}

std::vector< std::string >
Application
::GetParameterStringList( const std::string & parameter )
{
  std::vector<std::string> ret;
  Parameter * param = GetParameterByKey(parameter);

  if (dynamic_cast<InputImageListParameter*> (param))
    {
    InputImageListParameter * paramDown = dynamic_cast<InputImageListParameter*> (param);
    ret = paramDown->GetFileNameList();
    }
  else
    if (dynamic_cast<InputVectorDataListParameter*> (param))
      {
      InputVectorDataListParameter* paramDown = dynamic_cast<InputVectorDataListParameter*> (param);
      ret = paramDown->GetFileNameList();
      }
    else
      if (dynamic_cast<InputFilenameListParameter*> (param))
        {
	InputFilenameListParameter* paramDown = dynamic_cast<InputFilenameListParameter*> (param);
        ret = paramDown->GetFileNameList();
        }
      else
        if (dynamic_cast<StringListParameter*> (param))
          {
	  StringListParameter* paramDown = dynamic_cast<StringListParameter*> (param);
          ret = paramDown->GetValue();
          }
        else
          if (dynamic_cast<ListViewParameter*> (param))
            {
            ListViewParameter* paramDown = dynamic_cast<ListViewParameter*> (param);
            ret = paramDown->GetSelectedNames();
            }
          else
            {
            itkExceptionMacro(<<parameter << " parameter can't be casted to StringList");
            }

  return ret;
}

void Application::SetParameterInputImage(std::string parameter, ImageBaseType * inputImage)
{
  Parameter* param = GetParameterByKey(parameter);

  InputImageParameter* paramDown = dynamic_cast<InputImageParameter*> (param);

  if (paramDown)
    {
    paramDown->SetImage(inputImage);
    }
  else
    {
    itkExceptionMacro(<<parameter << " parameter can't be casted to InputImageParameter");
    }
}

ImageBaseType * Application::GetParameterOutputImage(std::string parameter)
{
  Parameter* param = GetParameterByKey(parameter);

  OutputImageParameter* paramDown = dynamic_cast<OutputImageParameter*> (param);

  if (paramDown)
    {
    return paramDown->GetValue();
    }
  else
    {
    itkExceptionMacro(<<parameter << " parameter can't be casted to OutputImageParameter");
    }
}


void Application::SetParameterComplexInputImage(std::string parameter, ImageBaseType * inputImage)
{
  Parameter* param = GetParameterByKey(parameter);

  ComplexInputImageParameter* paramDown = dynamic_cast<ComplexInputImageParameter*> (param);

  if (paramDown)
    {
    paramDown->SetImage(inputImage);
    }
  else
    {
    itkExceptionMacro(<<parameter << " parameter can't be casted to ComplexInputImageParameter");
    }
}

ImageBaseType * Application::GetParameterComplexOutputImage(std::string parameter)
{
  Parameter* param = GetParameterByKey(parameter);

  ComplexOutputImageParameter* paramDown = dynamic_cast<ComplexOutputImageParameter*> (param);

  if (paramDown)
    {
    return paramDown->GetValue();
    }
  else
    {
    itkExceptionMacro(<<parameter << " parameter can't be casted to ComplexOutputImageParameter");
    }
}

void Application::AddImageToParameterInputImageList(std::string parameter, ImageBaseType * img)
{
  Parameter* param = GetParameterByKey(parameter);

  InputImageListParameter * paramDown = dynamic_cast<InputImageListParameter *>(param);

  if(paramDown)
    {
    paramDown->AddImage(img);
    }
  else
    {
    itkExceptionMacro(<<parameter << " parameter can't be casted to InputImageListParameter");
    }

}

void Application::SetNthParameterInputImageList(std::string parameter, const unsigned int &id, ImageBaseType * img)
{
  Parameter* param = GetParameterByKey(parameter);

  InputImageListParameter * paramDown = dynamic_cast<InputImageListParameter *>(param);

  if(paramDown)
    {
    paramDown->SetNthImage(id,img);
    }
  else
    {
    itkExceptionMacro(<<parameter << " parameter can't be casted to InputImageListParameter");
    }

}

void Application::AddParameterStringList(std::string parameter, const std::string & str)
{
  Parameter* param = GetParameterByKey(parameter);

  InputImageListParameter * paramDown = dynamic_cast<InputImageListParameter *>(param);

  if(paramDown)
    {
    paramDown->AddFromFileName(str);
    }
  else
    {
    itkExceptionMacro(<<parameter << " parameter can't be casted to InputImageListParameter");
    }

}

void Application::SetNthParameterStringList(std::string parameter, const unsigned int &id, const std::string & str)
{
  Parameter* param = GetParameterByKey(parameter);

  InputImageListParameter * paramDown = dynamic_cast<InputImageListParameter *>(param);

  if(paramDown)
    {
    paramDown->SetNthFileName(id,str);
    }
  else
    {
    itkExceptionMacro(<<parameter << " parameter can't be casted to InputImageListParameter");
    }

}



void Application::ClearParameterInputImageList(std::string parameter)
{
  Parameter* param = GetParameterByKey(parameter);

  InputImageListParameter * paramDown = dynamic_cast<InputImageListParameter *>(param);

  if(paramDown)
    {
    paramDown->ClearValue();
    }
  else
    {
    itkExceptionMacro(<<parameter << " parameter can't be casted to InputImageListParameter");
    }

}

unsigned int Application::GetNumberOfElementsInParameterInputImageList(std::string parameter)
{
  Parameter* param = GetParameterByKey(parameter);

  InputImageListParameter * paramDown = dynamic_cast<InputImageListParameter *>(param);

  if(paramDown)
    {
    return paramDown->Size();
    }
  else
    {
    itkExceptionMacro(<<parameter << " parameter can't be casted to InputImageListParameter");
    }

}

FloatVectorImageType* Application::GetParameterImage(std::string parameter)
{
  return this->GetParameterImage<FloatVectorImageType>(parameter);
}

FloatVectorImageListType* Application::GetParameterImageList(std::string parameter)
{
  FloatVectorImageListType::Pointer ret=nullptr;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<InputImageListParameter*>(param))
    {
    InputImageListParameter* paramDown = dynamic_cast<InputImageListParameter*>(param);
    ret = paramDown->GetImageList();
    }
  else
    {
    itkExceptionMacro(<<parameter << " parameter can't be casted to ImageListType");
    }

  return ret;
}

ComplexFloatVectorImageType* Application::GetParameterComplexImage(std::string parameter)
{
  ComplexFloatVectorImageType::Pointer ret=nullptr;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<ComplexInputImageParameter*>(param))
    {
    ComplexInputImageParameter* paramDown = dynamic_cast<ComplexInputImageParameter*>(param);
    ret = paramDown->GetImage();
    }
  else
    {
    itkExceptionMacro(<<parameter << " parameter can't be casted to ComplexImageType");
    }

  return ret;
}

VectorDataType* Application::GetParameterVectorData(std::string parameter)
{
  VectorDataType::Pointer ret=nullptr;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<InputVectorDataParameter*>(param))
    {
    InputVectorDataParameter* paramDown = dynamic_cast<InputVectorDataParameter*>(param);
    ret = paramDown->GetVectorData();
    }
  else
    {
    itkExceptionMacro(<<parameter << " parameter can't be casted to Vector Data");
    }
  return ret;
}

VectorDataListType* Application::GetParameterVectorDataList(std::string parameter)
{
  VectorDataListType::Pointer ret=nullptr;
  Parameter* param = GetParameterByKey(parameter);

  if (dynamic_cast<InputVectorDataListParameter*>(param))
    {
    InputVectorDataListParameter* paramDown = dynamic_cast<InputVectorDataListParameter*>(param);
    ret = paramDown->GetVectorDataList();
    }
  else
   {
    itkExceptionMacro(<<parameter << " parameter can't be casted to Vector Data List");
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
      || type == ParameterType_OutputProcessXML || type == ParameterType_InputProcessXML
      || type == ParameterType_Bool)
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
  else if( type == ParameterType_StringList ||
    type == ParameterType_InputImageList ||
    type == ParameterType_InputVectorDataList ||
    type == ParameterType_InputFilenameList)
    {
      std::ostringstream oss;
      oss << std::setprecision(10);
      const std::vector<std::string> strList = this->GetParameterStringList(paramKey);
      for (size_t i = 0; i < strList.size(); i++)
      {
        if (i != 0)
        {
          oss << " ";
        }
        oss << strList[i];
      }
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
  // Get the  RAM Parameter from the configuration manager
  AddRAMParameter(paramKey, "Available RAM (MB)", otb::ConfigurationManager::GetMaxRAMHint());
  MandatoryOff(paramKey);
  SetParameterDescription(paramKey, "Available memory for processing (in MB).");
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

  GetParameterList()->AddParameter(ParameterType_Int, paramKey, "Random seed");
  MandatoryOff(paramKey);
  SetParameterDescription(paramKey, "Set a specific random seed with integer value.");

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
    // Check all parameters
    if (IsParameterMissing(*it))
      {
      ready = false;
      break;
      }
    }
  return ready;
}

bool
Application::IsParameterMissing(const std::string &key) const
{
  bool ret(false);
  const Parameter* param = GetParameterByKey(key);
  if (param->GetRole() == Role_Input &&
      GetParameterType(key) != ParameterType_Group &&
      param->GetMandatory() &&
      !param->HasValue())
    {
    ret = true;
    ParameterKey paramKey(key);
    std::vector<std::string> split = paramKey.Split();
    std::string currentRoot(key);
    unsigned int level = 1;
    while (level < split.size())
      {
      currentRoot.resize(currentRoot.find_last_of("."));
      param = GetParameterByKey(currentRoot);
      if (!param->GetActive() && !param->GetMandatory())
        {
        // the missing parameter is not on an active branch : we can ignore it
        ret = false;
        break;
        }
      level++;
      }
    if (ret)
      {
      // the missing parameter is on an active branch : we need it
      otbDebugMacro("MISSING : "<< key << " (Level "<< split.size()<<")");
      }
    }
  return ret;
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

double Application::GetLastExecutionTiming() const
{
  return m_Chrono.GetElapsedMilliseconds() / 1000.0;
}

ImageBaseType::PointType
Application::GetImageOrigin(const std::string & key, unsigned int idx)
{
  return this->GetParameterImageBase(key, idx)->GetOrigin();
}

ImageBaseType::SpacingType
Application::GetImageSpacing(const std::string & key, unsigned int idx)
{
  return otb::internal::GetSignedSpacing(this->GetParameterImageBase(key, idx));
}

ImageBaseType::SizeType
Application::GetImageSize(const std::string & key, unsigned int idx)
{
  return this->GetParameterImageBase(key, idx)->GetLargestPossibleRegion().GetSize();
}

unsigned int
Application::GetImageNbBands(const std::string & key, unsigned int idx)
{
  return this->GetParameterImageBase(key, idx)->GetNumberOfComponentsPerPixel();
}

std::string
Application::GetImageProjection(const std::string & key, unsigned int idx)
{
  std::string proj;
  const itk::MetaDataDictionary& dict =
    this->GetParameterImageBase(key, idx)->GetMetaDataDictionary();

  if (!dict.HasKey(MetaDataKey::ProjectionRefKey))
    return std::string("");

  itk::ExposeMetaData<std::string>(dict, MetaDataKey::ProjectionRefKey, proj);
  return proj;
}

otb::ImageKeywordlist
Application::GetImageKeywordlist(const std::string & key, unsigned int idx)
{
  ImageKeywordlist kwl;
  const itk::MetaDataDictionary& dict =
    this->GetParameterImageBase(key, idx)->GetMetaDataDictionary();

  if (dict.HasKey(MetaDataKey::OSSIMKeywordlistKey))
    itk::ExposeMetaData<ImageKeywordlist>(dict, MetaDataKey::OSSIMKeywordlistKey, kwl);

  return kwl;
}

unsigned long
Application::PropagateRequestedRegion(const std::string & key, ImageBaseType::RegionType region, unsigned int idx)
{
  ImageBaseType* image = this->GetParameterImageBase(key, idx);
  ImageBaseType::RegionType largest = image->GetLargestPossibleRegion();
  ImageBaseType::RegionType requested = region;
  requested.SetIndex(0, requested.GetIndex(0) + largest.GetIndex(0));
  requested.SetIndex(1, requested.GetIndex(1) + largest.GetIndex(1));
  image->SetRequestedRegion(requested);
  image->PropagateRequestedRegion();
  // estimate RAM usage
  otb::PipelineMemoryPrintCalculator::Pointer memoryPrintCalculator =
    otb::PipelineMemoryPrintCalculator::New();
  memoryPrintCalculator->SetDataToWrite(image);
  memoryPrintCalculator->SetBiasCorrectionFactor(1);
  memoryPrintCalculator->Compute(false);
  return memoryPrintCalculator->GetMemoryPrint();
}

ImageBaseType::RegionType
Application::GetImageRequestedRegion(const std::string & key, unsigned int idx)
{
  ImageBaseType* image = this->GetParameterImageBase(key, idx);
  ImageBaseType::RegionType largest = image->GetLargestPossibleRegion();
  ImageBaseType::RegionType requested = image->GetRequestedRegion();
  requested.SetIndex(0, requested.GetIndex(0) - largest.GetIndex(0));
  requested.SetIndex(1, requested.GetIndex(1) - largest.GetIndex(1));
  return requested;
}

itk::MetaDataDictionary
Application::GetImageMetaData(const std::string & key, unsigned int idx)
{
  ImageBaseType* image = this->GetParameterImageBase(key, idx);
  return image->GetMetaDataDictionary();
}

ImageBaseType*
Application::GetParameterImageBase(const std::string & key, unsigned int idx)
{
  Parameter* param = GetParameterByKey(key);
  if (dynamic_cast<InputImageParameter*>(param))
    {
    InputImageParameter* paramDown = dynamic_cast<InputImageParameter*>(param);
    return paramDown->GetImage<ImageBaseType>();
    }
  else if (dynamic_cast<InputImageListParameter*>(param))
    {
    InputImageListParameter* paramDown = dynamic_cast<InputImageListParameter*>(param);
    return paramDown->GetNthImage(idx);
    }
  else if (dynamic_cast<ComplexInputImageParameter*>(param))
    {
    ComplexInputImageParameter* paramDown = dynamic_cast<ComplexInputImageParameter*>(param);
    return paramDown->GetImage<ImageBaseType>();
    }
  else if (dynamic_cast<OutputImageParameter*>(param))
    {
    OutputImageParameter* paramDown = dynamic_cast<OutputImageParameter*>(param);
    return paramDown->GetValue();
    }
  else if (dynamic_cast<ComplexOutputImageParameter*>(param))
    {
    ComplexOutputImageParameter* paramDown = dynamic_cast<ComplexOutputImageParameter*>(param);
    return paramDown->GetValue();
    }
  else
    {
    itkExceptionMacro("Wrong parameter type, expect InputImageParameter, "
      "InputImageListParameter, ComplexInputImageParameter, "
      "OutputImageParameter, ComplexOutputImageParameter");
    }
  return nullptr;
}

void
Application::SetParameterImageBase(const std::string & key, ImageBaseType* img, unsigned int idx)
{
  Parameter* param = GetParameterByKey(key);
  if (dynamic_cast<InputImageParameter*>(param))
    {
    InputImageParameter* paramDown = dynamic_cast<InputImageParameter*>(param);
    paramDown->SetImage<ImageBaseType>(img);
    }
  else if (dynamic_cast<InputImageListParameter*>(param))
    {
    InputImageListParameter* paramDown = dynamic_cast<InputImageListParameter*>(param);
    if (idx >= paramDown->Size())
      {
      paramDown->AddImage(img);
      }
    else
      {
      paramDown->SetNthImage(idx, img);
      }
    }
  else if (dynamic_cast<ComplexInputImageParameter*>(param))
    {
    ComplexInputImageParameter* paramDown = dynamic_cast<ComplexInputImageParameter*>(param);
    paramDown->SetImage<ImageBaseType>(img);
    }
  else
    {
    itkExceptionMacro("Wrong parameter type, expect InputImageParameter, InputImageListParameter or ComplexInputImageParameter");
    }
}

ImagePixelType
Application::GetImageBasePixelType(const std::string & key, unsigned int idx)
{
  ImageBaseType* img = this->GetParameterImageBase(key, idx);
  if (! img)
    {
    itkExceptionMacro("No input image");
    }
  std::string className(img->GetNameOfClass());
  if (className == "VectorImage")
    {
#define FindVectorImagePixelTypeMacro(TImage, TPixel) \
    TImage##VectorImageType* img##TImage = dynamic_cast< TImage##VectorImageType* >(img); \
    if ( img##TImage ) return ImagePixelType_##TPixel ;

    FindVectorImagePixelTypeMacro(UInt8,  uint8)
    FindVectorImagePixelTypeMacro(Int16,  int16)
    FindVectorImagePixelTypeMacro(UInt16, uint16)
    FindVectorImagePixelTypeMacro(Int32,  int32)
    FindVectorImagePixelTypeMacro(UInt32, uint32)
    FindVectorImagePixelTypeMacro(Float,  float)
    FindVectorImagePixelTypeMacro(Double, double)
    FindVectorImagePixelTypeMacro(ComplexInt16,  cint16)
    FindVectorImagePixelTypeMacro(ComplexInt32,  cint32)
    FindVectorImagePixelTypeMacro(ComplexFloat,  cfloat)
    FindVectorImagePixelTypeMacro(ComplexDouble, cdouble)
#undef FindVectorImagePixelTypeMacro
    }
  else
    {
#define FindImagePixelTypeMacro(TImage, TPixel) \
    TImage##ImageType* img##TImage = dynamic_cast< TImage##ImageType* >(img); \
    if ( img##TImage ) return ImagePixelType_##TPixel ;

    FindImagePixelTypeMacro(UInt8,  uint8)
    FindImagePixelTypeMacro(Int16,  int16)
    FindImagePixelTypeMacro(UInt16, uint16)
    FindImagePixelTypeMacro(Int32,  int32)
    FindImagePixelTypeMacro(UInt32, uint32)
    FindImagePixelTypeMacro(Float,  float)
    FindImagePixelTypeMacro(Double, double)
    FindImagePixelTypeMacro(ComplexInt16,  cint16)
    FindImagePixelTypeMacro(ComplexInt32,  cint32)
    FindImagePixelTypeMacro(ComplexFloat,  cfloat)
    FindImagePixelTypeMacro(ComplexDouble, cdouble)
    FindImagePixelTypeMacro(UInt8RGB,  uint8)
    FindImagePixelTypeMacro(UInt8RGBA, uint8)
#undef FindImagePixelTypeMacro
    }
  itkWarningMacro("Unknown pixel type");
  // by default uint8
  return ImagePixelType_uint8;
}

#define otbGetParameterImageMacro( Image )                              \
  Image##Type * Application::GetParameter##Image(std::string parameter) \
    {                                                                   \
    Parameter* param = GetParameterByKey(parameter);                    \
    InputImageParameter* paramDown = dynamic_cast<InputImageParameter*>(param); \
    if ( paramDown )                                                    \
      {                                                                 \
      return paramDown->Get##Image();                                   \
      }                                                                 \
    ComplexInputImageParameter* paramDownC = dynamic_cast<ComplexInputImageParameter*>(param); \
    if ( paramDownC )                                                   \
      {                                                                 \
      return paramDownC->Get##Image();                                  \
      }                                                                 \
    return Image##Type::Pointer();                                      \
    }

otbGetParameterImageMacro(UInt8Image);
otbGetParameterImageMacro(UInt16Image);
otbGetParameterImageMacro(Int16Image);
otbGetParameterImageMacro(UInt32Image);
otbGetParameterImageMacro(Int32Image);
otbGetParameterImageMacro(FloatImage);
otbGetParameterImageMacro(DoubleImage);

otbGetParameterImageMacro(UInt8VectorImage);
otbGetParameterImageMacro(UInt16VectorImage);
otbGetParameterImageMacro(Int16VectorImage);
otbGetParameterImageMacro(UInt32VectorImage);
otbGetParameterImageMacro(Int32VectorImage);
otbGetParameterImageMacro(FloatVectorImage);
otbGetParameterImageMacro(DoubleVectorImage);

otbGetParameterImageMacro(UInt8RGBImage);
otbGetParameterImageMacro(UInt8RGBAImage);

// Complex image
otbGetParameterImageMacro(ComplexInt16Image);
otbGetParameterImageMacro(ComplexInt32Image);
otbGetParameterImageMacro(ComplexFloatImage);
otbGetParameterImageMacro(ComplexDoubleImage);
otbGetParameterImageMacro(ComplexInt16VectorImage);
otbGetParameterImageMacro(ComplexInt32VectorImage);
otbGetParameterImageMacro(ComplexFloatVectorImage);
otbGetParameterImageMacro(ComplexDoubleVectorImage);


bool
Application::ConnectImage(std::string in, Application* app, std::string out)
{
  if(app == nullptr)
    {
    // throw error ?
    return false;
    }

  Parameter* param = GetParameterByKey(in);
  InputImageParameter* inParam = dynamic_cast<InputImageParameter*>(param);
  if(inParam == nullptr)
    {
    InputImageListParameter* inListParam = dynamic_cast<InputImageListParameter*>(param);
    if (inListParam == nullptr)
      {
      return false;
      }
    inListParam->InsertElement(InputImageParameter::New(),inListParam->Size());
    inParam = (inListParam->GetNthElement(inListParam->Size() - 1)).GetPointer();
    }
  param = app->GetParameterByKey(out);
  OutputImageParameter* outParam = dynamic_cast<OutputImageParameter*>(param);
  if(outParam == nullptr)
    {
    return false;
    }
  InputImageParameter::Connector c;
  c.app = app;
  c.key = out;
  c.isMem = true;
  inParam->SetConnection(c);
  return true;
}

void
Application::PropagateConnectMode(bool isMem)
{
  // reset ExecuteDone flag
  m_ExecuteDone = false;
  std::vector<std::string> paramList = GetParametersKeys(true);
  std::unordered_set<Application*> targetApps;
  for (std::vector<std::string>::const_iterator it = paramList.begin(); it != paramList.end(); ++it)
    {
    std::string key = *it;
    Parameter* param = GetParameterByKey(key);
    InputImageParameter* imgParam = dynamic_cast<InputImageParameter*>(param);
    
    if(imgParam)
      {
      Application::Pointer targetApp = otb::DynamicCast<Application>(imgParam->GetConnection().app);
      if(targetApp.IsNotNull())
        {
        imgParam->SetConnectionMode(isMem);
        targetApps.insert(targetApp);
        }
      }
    else
      {
      InputImageListParameter* imgListParam = dynamic_cast<InputImageListParameter*>(param);
      if (imgListParam)
        {
        for (unsigned int i=0 ; i<imgListParam->Size(); i++)
          {
          Application::Pointer targetApp = otb::DynamicCast<Application>(imgListParam->GetNthElement(i)->GetConnection().app);
          if(targetApp.IsNotNull())
            {
            imgListParam->GetNthElement(i)->SetConnectionMode(isMem);
            targetApps.insert(targetApp);
            }
          }
        }
      }
    }
  for (auto &app : targetApps)
    {
    app->PropagateConnectMode(isMem);
    }
}

bool
Application::IsExecuteDone()
{
  return m_ExecuteDone;
}

}
}
