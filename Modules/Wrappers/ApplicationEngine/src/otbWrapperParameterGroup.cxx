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

#include "otbWrapperTypes.h"
#include "otbWrapperChoiceParameter.h"
#include "otbWrapperListViewParameter.h"
#include "otbWrapperDirectoryParameter.h"
#include "otbWrapperInputFilenameParameter.h"
#include "otbWrapperInputFilenameListParameter.h"
#include "otbWrapperOutputFilenameParameter.h"
#include "otbWrapperOutputProcessXMLParameter.h"
#include "otbWrapperComplexInputImageParameter.h"
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperInputVectorDataParameter.h"
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperComplexOutputImageParameter.h"
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperStringListParameter.h"
#include "otbWrapperInputImageListParameter.h"
#include "otbWrapperInputVectorDataListParameter.h"
#include "otbWrapperInputProcessXMLParameter.h"
#include "otbWrapperParameterKey.h"
#include "otbWrapperProxyParameter.h"
#include "otbWrapperBoolParameter.h"

#include "otb_boost_string_header.h"

namespace otb
{
namespace Wrapper
{

ParameterGroup::ParameterGroup()
{
}

ParameterGroup::~ParameterGroup()
{
}

std::vector<std::string>
ParameterGroup::GetParametersKeys(bool recursive)
{
  std::vector<std::string> parameters;

  ParameterListType::iterator pit;
  for (pit = m_ParameterList.begin(); pit != m_ParameterList.end(); ++pit)
    {
    Parameter* param = *pit;
    std::string currentKey(param->GetKey());
    parameters.push_back( currentKey );

    // follow proxy parameters
    param = this->ResolveParameter(param);

    if (recursive && dynamic_cast<ParameterGroup*>(param))
      {
      ParameterGroup* paramAsGroup = dynamic_cast<ParameterGroup*>(param);
      std::vector<std::string> subparams = paramAsGroup->GetParametersKeys();
      for (std::vector<std::string>::const_iterator it = subparams.begin();
           it != subparams.end(); ++it)
        {
        parameters.push_back( currentKey + "."  + *it );
        }
      }
    else if (recursive && dynamic_cast<ChoiceParameter*>(param))
      {
      ChoiceParameter* paramAsChoice = dynamic_cast<ChoiceParameter*>(param);

      std::vector<std::string> subparams = paramAsChoice->GetParametersKeys();
      for (std::vector<std::string>::const_iterator it = subparams.begin();
           it != subparams.end(); ++it)
        {
        parameters.push_back( currentKey + "."  + *it );
        }
      }
    }
  return parameters;
}


/** Add a new choice value to the parameter group */
void
ParameterGroup::AddChoice(std::string paramKey, std::string paramName)
{
  ParameterKey pKey( paramKey );
  // Split the parameter name
  std::vector<std::string> splitKey = pKey.Split();

  if( splitKey.size() > 1 )
    {
      // Get the last subkey
      std::string lastkey = pKey.GetLastElement();

      std::string parentkey = pKey.GetRoot();
      Parameter::Pointer parentParam = GetParameterByKey(parentkey);

      // parentParam must be a choice, a listBox or this is an error
      ChoiceParameter* comboboxParentAsChoice = dynamic_cast<ChoiceParameter*>(parentParam.GetPointer());
      ListViewParameter* listBoxParentAsChoice = dynamic_cast<ListViewParameter*>(parentParam.GetPointer());

      if (comboboxParentAsChoice)
        {
        comboboxParentAsChoice->AddChoice(lastkey, paramName);
        }
      else if (listBoxParentAsChoice)
        {
        listBoxParentAsChoice->AddChoice(lastkey, paramName);
        }
      else
        {
          itkExceptionMacro(<<parentkey << " is not a choice");
		  
        }
    }
  else
    {
		itkExceptionMacro(
					 << "No choice parameter key given: paramKey = '"
					 << paramKey
					 << "'");
    }
}

/** Remove the choices made for the ViewList parameter */
void
ParameterGroup::ClearChoices(std::string paramKey)
{

  Parameter * param  = GetParameterByKey(paramKey);

  if(!param)
    {
    itkExceptionMacro("Parameter "<<paramKey<<" not found");
    }
  
   // param must be a choice, a listBox or this is an error
  ListViewParameter* paramAsChoice = dynamic_cast<ListViewParameter*>(param);

  if (paramAsChoice)
    {
    paramAsChoice->ClearChoices();
    }
  else
    {
    itkExceptionMacro(<<paramKey << " is not a ListView");
    }
}

void ParameterGroup::AddOutXMLParameter()
{
  Parameter::Pointer tmpParam;
  tmpParam = OutputProcessXMLParameter::New();
//  const std::string key =   tmpParam->GetKey();
//  const std::string descr = tmpParam->GetDescription();
const std::string defaultXMLFileName = std::string(GetName())  + ".xml";
  tmpParam->SetActive(false);
  AddParameter(tmpParam);

  tmpParam = nullptr;
  /*
  AddParameter(ParameterType_OutputProcessXML,  key,   descr);
  SetParameterDescription(key, descr);
  MandatoryOff(key);
  //SetParameterString(key, defaultXMLFileName);
  DisableParameter(key);
  */
}

void ParameterGroup::AddInXMLParameter()
{
  Parameter::Pointer tmpParam;
  tmpParam = InputProcessXMLParameter::New();
//  const std::string key =   tmpParam->GetKey();
//  const std::string descr = tmpParam->GetDescription();
  const std::string defaultXMLFileName = std::string(GetName())  + ".xml";
  tmpParam->SetActive(false);
  AddParameter(tmpParam);

  tmpParam = nullptr;
  /*
  AddParameter(ParameterType_InputProcessXML,  key,   descr);
  SetParameterDescription(key, descr);
  MandatoryOff(key);
  //SetParameterString(key, defaultXMLFileName);
  DisableParameter(key);
*/
}

/** Get the choices made in the QListWidget */
std::vector<int>
ParameterGroup::GetSelectedItems(std::string paramKey)
{
  Parameter * param  = GetParameterByKey(paramKey);

  if(!param)
    {
    itkExceptionMacro("Parameter "<<paramKey<<" not found");
    }
  
   // param must be a choice, a listBox or this is an error
  ListViewParameter* paramAsChoice = dynamic_cast<ListViewParameter*>(param);
  
  std::vector<int> selectedItems;

  if (paramAsChoice)
    {
    selectedItems = paramAsChoice->GetSelectedItems();
    }
  else
    {
    itkExceptionMacro(<<paramKey << " is not a ListView");
    }

  return selectedItems;
}

/* Get the parameter type from its string version of ParameterType enum */
ParameterType ParameterGroup::GetParameterTypeFromString(const std::string& str)
{
  return ParameterStringToType(str);
}

/* Get the parameter type as string from its ParameterType enum */
std::string ParameterGroup::GetParameterTypeAsString(ParameterType type)
{
  return ParameterTypeToString(type);
}

/** Add a new parameter to the parameter group */
void
ParameterGroup::AddParameter(ParameterType type, std::string paramKey, std::string paramName)
{
  ParameterKey pKey(paramKey);
  // Split the parameter name
  std::vector<std::string> splitKey = pKey.Split();

  // Get the last subkey
  std::string lastkey = pKey.GetLastElement();


  std::string parentkey;
  Parameter::Pointer parentParam;

  if (splitKey.size() > 1)
    {
    parentkey = pKey.GetRoot();
    parentParam = GetParameterByKey(parentkey);
    }
  else
    {
    parentParam = this;
    }

  ParameterGroup* parentAsGroup = dynamic_cast<ParameterGroup*> (parentParam.GetPointer());
  if (parentAsGroup)
    {
    Parameter::Pointer newParam;
    switch (type)
      {
      case ParameterType_Int:
        {
        newParam = IntParameter::New();
        }
        break;
      case ParameterType_Float:
        {
        newParam = FloatParameter::New();
        }
        break;
      case ParameterType_String:
        {
        newParam = StringParameter::New();
        }
        break;
      case ParameterType_InputFilename:
        {
        newParam = InputFilenameParameter::New();
        }
        break;
      case ParameterType_OutputFilename:
        {
        newParam = OutputFilenameParameter::New();
        }
        break;
      case ParameterType_Directory:
        {
        newParam = DirectoryParameter::New();
        }
        break;
      case ParameterType_InputImage:
        {
        newParam = InputImageParameter::New();
        }
        break;
      case ParameterType_InputVectorData:
        {
        newParam = InputVectorDataParameter::New();
        }
        break;
      case ParameterType_OutputImage:
        {
        newParam = OutputImageParameter::New();
        }
        break;
      case ParameterType_OutputVectorData:
        {
        newParam = OutputVectorDataParameter::New();
        }
        break;
      case ParameterType_Radius:
        {
        newParam = RadiusParameter::New();
        }
        break;
      case ParameterType_Choice:
        {
        newParam = ChoiceParameter::New();
        }
        break;
      case ParameterType_Group:
        {
        newParam = ParameterGroup::New();
        }
        break;
      case ParameterType_StringList:
        {
        newParam = StringListParameter::New();
        }
        break;
      case ParameterType_InputFilenameList:
        {
        newParam = InputFilenameListParameter::New();
        }
        break;
      case ParameterType_InputImageList:
        {
        newParam = InputImageListParameter::New();
        }
        break;
      case ParameterType_InputVectorDataList:
        {
        newParam = InputVectorDataListParameter::New();
        }
        break;
      case ParameterType_ListView:
        {
        newParam = ListViewParameter::New();
        }
        break;
      case ParameterType_ComplexInputImage:
        {
        newParam = ComplexInputImageParameter::New();
        }
        break;
      case ParameterType_ComplexOutputImage:
        {
        newParam = ComplexOutputImageParameter::New();
        }
        break;
      case ParameterType_RAM:
        {
        newParam = RAMParameter::New();
        }
        break;
      case ParameterType_OutputProcessXML:
        {
       newParam = OutputProcessXMLParameter::New();
        }
        break;
      case ParameterType_InputProcessXML:
        {
       newParam = InputProcessXMLParameter::New();
        }
        break;
      case ParameterType_Bool:
        {
       newParam = BoolParameter::New();
        }
        break;
      default:
        break;
      }

    if (newParam.IsNull())
      {
      itkExceptionMacro(<< "Parameter type not supported for " << paramKey);
      }

    newParam->SetKey(lastkey);
    newParam->SetName(paramName);

    // If splitKey is greater than 1, that means that the parameter
    // is not a root, and have a parent(s):
    // - Add the parent as root of this param
    // - Add the param as a child of its parents
    if (splitKey.size() > 1)
      {
      newParam->SetRoot(parentParam);
      parentParam->AddChild(newParam);
      }

    parentAsGroup->AddParameter(newParam);
    }
  else
    {
    itkExceptionMacro(<< "Cannot add " << lastkey << " to parameter " << parentkey);
    }
}

void
ParameterGroup::AddParameter(Parameter::Pointer p)
{
  m_ParameterList.push_back(p);
}

bool
ParameterGroup::ReplaceParameter(std::string &key, Parameter::Pointer p)
{
  bool ret = true;
  ParameterKey pName(key);
  std::vector<std::string> splitName = pName.Split();
  std::string lastkey = pName.GetLastElement();
  std::string parentkey = pName.GetRoot();
  ParameterGroup* parentGroup = this;
  if( splitName.size() > 1 )
    {
    Parameter* parentParam = GetParameterByKey(parentkey);
    parentGroup = dynamic_cast<ParameterGroup*>(parentParam);
    if (parentGroup)
      {
      ret = parentGroup->ReplaceParameter(lastkey, p);
      }
    else
      {
      ret = false;
      }
    }
  else
    {
    // find current parameter in the current group
    Parameter::Pointer oldParam;
    ParameterListType::iterator vit;
    for (vit = m_ParameterList.begin(); vit != m_ParameterList.end(); ++vit)
      {
      if (lastkey.compare((*vit)->GetKey()) == 0)
        {
        oldParam = *vit;
        break;
        }
      }
    if (oldParam.IsNull())
      {
      // parameter to replace not found : return false
      ret = false;
      }
    else
      {
      // parameter already exists : replace it
      *vit = p;
      p->SetKey(lastkey);
      }
    }
  if (ret)
    {
    if( splitName.size() > 1 )
      {
      p->SetRoot(parentGroup);
      parentGroup->AddChild(p);
      }
    }
  // don't check type compatibility here, we may want to handle special cases
  // at higher level
  return ret;
}

Parameter::Pointer
ParameterGroup::GetParameterByIndex(unsigned int i, bool follow)
{
  Parameter *param = m_ParameterList[i];
  if (follow)
    {
    param = this->ResolveParameter(param);
    }
  return Parameter::Pointer(param);
}

Parameter::Pointer
ParameterGroup::GetParameterByKey(std::string name, bool follow)
{
  ParameterKey pName(name);

 // Split the parameter name
  std::vector<std::string> splitName = pName.Split();

  // Get the first parameter key
  std::string parentName = pName.GetFirstElement();

  // Look for parentName in the current group
  Parameter::Pointer parentParam;
  ParameterListType::iterator vit;
  for (vit = m_ParameterList.begin(); vit != m_ParameterList.end(); ++vit)
    {
    Parameter::Pointer param = *vit;

    if (param->GetKey() == parentName)
      {
      parentParam = param;
      break;
      }
    }
  if (parentParam.IsNull())
    {
    itkExceptionMacro(<< "Could not find parameter " << name)
    }

  // follow proxy parameters (resolve intermediate group parameter because
  // we need to get to the requested parameter)
  if (follow || splitName.size() > 1)
    {
    Parameter *rawParam = this->ResolveParameter(parentParam.GetPointer());
    parentParam = rawParam;
    }

  // If the name contains a child, make a recursive call
  if (splitName.size() > 1)
    {
    // Handle ParameterGroup case
    ParameterGroup* parentAsGroup = dynamic_cast<ParameterGroup*>(parentParam.GetPointer());
    if (parentAsGroup)
      {
      // Remove the parent from the param name
      std::ostringstream childNameOss;
      std::vector<std::string>::const_iterator vvit = splitName.begin() + 1;
      while(vvit != splitName.end())
        {
        childNameOss << *vvit;
        ++vvit;
        if (vvit != splitName.end())
          {
          childNameOss << ".";
          }
        }
      std::string childName = childNameOss.str();

      return parentAsGroup->GetParameterByKey(childName,follow);
      }

    // Handle ChoiceParameter case
    ChoiceParameter* parentAsChoice = dynamic_cast<ChoiceParameter*>(parentParam.GetPointer());
    if (parentAsChoice)
      {
      // Check that splitName[1] is one of the choice
      ParameterGroup::Pointer associatedParam;

      // will throw if splitName[1] is not a choice key
      associatedParam = parentAsChoice->GetChoiceParameterGroupByKey(splitName[1]);

      if (splitName.size() > 2)
        {
        if (associatedParam.IsNull())
          {
          itkExceptionMacro(<< "Choice " << splitName[1] << "in "
                            << splitName[0] << "  has no key named "
                            << splitName[2]);
          }

        // Remove the parent and the choice value from the param name
        std::ostringstream childNameOss;
        std::vector<std::string>::const_iterator vvvit = splitName.begin() + 2;
        while(vvvit != splitName.end())
          {
          childNameOss << *vvvit;
          ++vvvit;
          if (vvvit != splitName.end())
            {
            childNameOss << ".";
            }
          }
        std::string childName = childNameOss.str();
        return associatedParam->GetParameterByKey(childName,follow);
        }
      return associatedParam.GetPointer();
      }
    // Neither ParameterGroup, neither ChoiceParameter
    itkExceptionMacro(<< "No parameter with key " << name);
    }

  return parentParam.GetPointer();
}

unsigned int
ParameterGroup::GetNumberOfParameters()
{
  return m_ParameterList.size();
}

Parameter* ParameterGroup::ResolveParameter(Parameter *param)
{
  Parameter* ret = param;
  if (ret == nullptr)
    {
    itkGenericExceptionMacro("Can't resolve NULL parameter!");
    }
  while (dynamic_cast<ProxyParameter*>(ret))
    {
    ProxyParameter* castParam = dynamic_cast<ProxyParameter*>(ret);
    ProxyParameter::ProxyTargetType target = castParam->GetTarget();
    ParameterGroup* targetGroup = dynamic_cast<ParameterGroup*>(target.first.GetPointer());
    if (targetGroup)
      {
      ret = targetGroup->GetParameterByKey(target.second);
      }
    else
      {
      itkGenericExceptionMacro("Target group of a proxy parameter is not of type ParameterGroup");
      }
    if (ret == param)
      {
      itkGenericExceptionMacro("Cycle detected with proxy parameters!");
      }
    }
  return ret;
}

}
}
