/*
 * Copyright (C) 2005-2026 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
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
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperInputVectorDataParameter.h"
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperStringListParameter.h"
#include "otbWrapperInputImageListParameter.h"
#include "otbWrapperInputVectorDataListParameter.h"
#include "otbWrapperBoolParameter.h"
#include "otbWrapperFieldParameter.h"
#include "otbWrapperBandParameter.h"

#include "otbWrapperParameterKey.h"
#include "otbWrapperProxyParameter.h"

#include "otb_boost_string_header.h"

// Actually most key error happenning here are programming errors in OTB Application writting
// => let's assert first, and still keep the exception if the applications are not tested in debug
// mode.

namespace otb
{
namespace Wrapper
{

std::vector<std::string> ParameterGroup::GetParametersKeys(bool recursive) const
{
  std::vector<std::string> parameters;
  parameters.reserve(m_ParameterList.size());

  for (Parameter* param : m_ParameterList)
  {
    std::string currentKey(param->GetKey());
    parameters.push_back(currentKey);

    // follow proxy parameters
    auto* resolved_param = this->ResolveParameter(param);
    if (!recursive)
      continue;

    if (auto* paramAsGroup = dynamic_cast<ParameterGroup*>(resolved_param)
        ; paramAsGroup)
    {
      // ParameterGroup*          paramAsGroup = dynamic_cast<ParameterGroup*>(resolved_param);
      std::vector<std::string> subparams    = paramAsGroup->GetParametersKeys();
      for (auto it = subparams.begin(); it != subparams.end(); ++it)
      {
        parameters.push_back(currentKey + '.' + *it);
      }
    }
    else if (auto* paramAsChoice = dynamic_cast<ChoiceParameter*>(resolved_param)
             ; paramAsChoice)
    {
      // ChoiceParameter* paramAsChoice = dynamic_cast<ChoiceParameter*>(resolved_param);
      std::vector<std::string> subparams = paramAsChoice->GetParametersKeys();
      for (auto it = subparams.begin(); it != subparams.end(); ++it)
      {
        parameters.push_back(currentKey + '.' + *it);
      }
    }
  }
  return parameters;
}


/** Add a new choice value to the parameter group */
void ParameterGroup::AddChoice(std::string paramKey, std::string paramName)
{
  ParameterKey pKey(paramKey);

  if (pKey.HasSeveralParts())
  {
    // Get the last subkey
    std::string_view lastkey = pKey.GetLastElement();

    std::string_view   parentkey   = pKey.GetRoot();
    Parameter::Pointer parentParam = GetParameterByKey(parentkey);

    // parentParam must be a choice, a listBox or this is an error
    ChoiceParameter*   comboboxParentAsChoice = dynamic_cast<ChoiceParameter*>(parentParam.GetPointer());
    ListViewParameter* listBoxParentAsChoice  = dynamic_cast<ListViewParameter*>(parentParam.GetPointer());

    if (comboboxParentAsChoice)
    {
      comboboxParentAsChoice->AddChoice(std::string{lastkey}, paramName);
    }
    else if (listBoxParentAsChoice)
    {
      listBoxParentAsChoice->AddChoice(std::string{lastkey}, paramName);
    }
    else
    {
      assert(!"parentkey is not a choice");
      itkExceptionMacro(<< parentkey << " is not a choice");
    }
  }
  else
  {
    assert(!"No choice parameter key given");
    itkExceptionMacro(<< "No choice parameter key given: paramKey = '" << paramKey << "'");
  }
}

/** Remove the choices made for the ViewList parameter */
void ParameterGroup::ClearChoices(std::string_view paramKey)
{
  Parameter* param = GetParameterByKey(paramKey);

  if (!param)
  {
    assert(!"Parameter not found");
    itkExceptionMacro("Parameter " << paramKey << " not found");
  }

  // param must be a choice, a listBox or this is an error
  ListViewParameter* paramAsChoice = dynamic_cast<ListViewParameter*>(param);

  if (paramAsChoice)
  {
    paramAsChoice->ClearChoices();
  }
  else
  {
    assert(!"paramKey is not a ListView");
    itkExceptionMacro(<< paramKey << " is not a ListView");
  }
}

/** Get the choices made in the QListWidget */
std::vector<int> ParameterGroup::GetSelectedItems(std::string_view paramKey) const
{
  Parameter* param = GetParameterByKey(paramKey);

  if (!param)
  {
    assert(!"Parameter not found");
    itkExceptionMacro("Parameter " << paramKey << " not found");
  }

  // param must be a choice, a listBox or this is an error
  ListViewParameter* paramAsChoice = dynamic_cast<ListViewParameter*>(param);

  if (paramAsChoice)
  {
    return paramAsChoice->GetSelectedItems();
  }
  else
  {
    assert(!"paramKey is not a ListView");
    itkExceptionMacro(<< paramKey << " is not a ListView");
  }
}

/* Get the parameter type from its string version of ParameterType enum */
ParameterType ParameterGroup::GetParameterTypeFromString(const std::string& str) const
{
  return ParameterStringToType(str);
}

/* Get the parameter type as string from its ParameterType enum */
std::string ParameterGroup::GetParameterTypeAsString(ParameterType type) const
{
  return ParameterTypeToString(type);
}

std::tuple<ParameterGroup*, std::string_view, std::string_view>
ParameterGroup::DecodeKey(std::string_view paramKey)
{
  // ParameterKey pKey(paramKey);
  // Split the parameter name
  auto [parentkey, lastkey] = ParameterKey::SplitKeyOnLastDot(paramKey);
  // std::cout << "Décodage: " << paramKey << " -> " << parentkey << " / " << lastkey << std::endl;

  Parameter::Pointer parentParam = parentkey.empty()
    ? this
    : GetParameterByKey(std::string(parentkey));

  ParameterGroup* parentAsGroup = dynamic_cast<ParameterGroup*>(parentParam.GetPointer());

  return {parentAsGroup, parentkey, lastkey};
}

/** Add a new parameter to the parameter group */
void ParameterGroup::AddParameter(ParameterType type, std::string_view paramKey, std::string paramName)
{
#if 0
  ParameterKey pKey(paramKey);
  // Split the parameter name
  std::vector<std::string> splitKey = pKey.Split();

  // Get the last subkey
  std::string lastkey = pKey.GetLastElement();


  std::string        parentkey;
  Parameter::Pointer parentParam;

  if (splitKey.size() > 1)
  {
    parentkey   = pKey.GetRoot();
    parentParam = GetParameterByKey(parentkey);
  }
  else
  {
    parentParam = this;
  }

  ParameterGroup* parentAsGroup = dynamic_cast<ParameterGroup*>(parentParam.GetPointer());
#else
  auto [parentAsGroup, parentkey, lastkey] = DecodeKey(paramKey);
  Parameter::Pointer parentParam = parentAsGroup;
#endif
  if (parentAsGroup)
  {
    Info info{
        .key         = std::string(lastkey),
        .name        = std::move(paramName),
        .description = "" // for now...
    };
    Parameter::Pointer newParam;
    switch (type)
    {
      case ParameterType_Int:
        {
          newParam = IntParameter::New(std::move(info));
        }
        break;
      case ParameterType_Float:
        {
          newParam = FloatParameter::New(std::move(info));
        }
        break;
      case ParameterType_Double:
        {
          newParam = DoubleParameter::New(std::move(info));
        }
        break;
      case ParameterType_String:
        {
          newParam = StringParameter::New(std::move(info));
        }
        break;
      case ParameterType_InputFilename:
        {
          newParam = InputFilenameParameter::New(std::move(info));
        }
        break;
      case ParameterType_OutputFilename:
        {
          newParam = OutputFilenameParameter::New(std::move(info));
        }
        break;
      case ParameterType_Directory:
        {
          newParam = DirectoryParameter::New(std::move(info));
        }
        break;
      case ParameterType_InputImage:
        {
          newParam = InputImageParameter::New(std::move(info));
        }
        break;
      case ParameterType_InputVectorData:
        {
          newParam = InputVectorDataParameter::New(std::move(info));
        }
        break;
      case ParameterType_OutputImage:
        {
          newParam = OutputImageParameter::New(std::move(info));
        }
        break;
      case ParameterType_OutputVectorData:
        {
          newParam = OutputVectorDataParameter::New(std::move(info));
        }
        break;
      case ParameterType_Radius:
        {
          newParam = RadiusParameter::New(std::move(info));
        }
        break;
      case ParameterType_Choice:
        {
          newParam = ChoiceParameter::New(std::move(info));
        }
        break;
      case ParameterType_Group:
        {
          newParam = ParameterGroup::New(std::move(info));
        }
        break;
      case ParameterType_StringList:
        {
          newParam = StringListParameter::New(std::move(info));
        }
        break;
      case ParameterType_InputFilenameList:
        {
          newParam = InputFilenameListParameter::New(std::move(info));
        }
        break;
      case ParameterType_InputImageList:
        {
          newParam = InputImageListParameter::New(std::move(info));
        }
        break;
      case ParameterType_InputVectorDataList:
        {
          newParam = InputVectorDataListParameter::New(std::move(info));
        }
        break;
      case ParameterType_ListView:
        {
          newParam = ListViewParameter::New(std::move(info));
        }
        break;
      case ParameterType_RAM:
        {
          newParam = RAMParameter::New();
        }
        break;
      case ParameterType_Bool:
        {
          newParam = BoolParameter::New(std::move(info));
        }
        break;
      case ParameterType_Field:
        {
          newParam = FieldParameter::New(std::move(info));
        }
        break;
      case ParameterType_Band:
        {
          newParam = BandParameter::New(std::move(info));
        }
        break;
      default:
        break;
    }

    if (newParam.IsNull())
    {
      assert(! "Parameter type not supported");
      itkExceptionMacro(<< "Parameter type not supported for " << paramKey);
    }


    // If splitKey is greater than 1, that means that the parameter
    // is not a root, and have a parent(s):
    // - Add the parent as root of this param
    // - Add the param as a child of its parents
    if (!parentkey.empty())
    {
      newParam->SetRoot(parentParam);
      parentParam->AddChild(newParam);
    }

    parentAsGroup->AddParameter(newParam);
  }
  else
  {
    assert(!"Cannot add lastkey to parameter");
    itkExceptionMacro(<< "Cannot add " << lastkey << " to parameter " << parentkey);
  }
}

void ParameterGroup::AddParameter(Parameter::Pointer p)
{
  m_ParameterList.push_back(p);
}

bool ParameterGroup::ReplaceParameter(std::string_view key, Parameter::Pointer p)
{
  ParameterKey             pName(std::string{key});
  std::string_view         lastkey     = pName.GetLastElement();
  std::string_view         parentkey   = pName.GetRoot();
  ParameterGroup*          parentGroup = this;
  if (pName.HasSeveralParts())
  {
    Parameter* parentParam = GetParameterByKey(parentkey);
    parentGroup            = dynamic_cast<ParameterGroup*>(parentParam);
    if (! parentGroup || ! parentGroup->ReplaceParameter(lastkey, p))
      return false;

    p->SetRoot(parentGroup);
    parentGroup->AddChild(p);
  }
  else
  {
    // find current parameter in the current group
    ParameterListType::iterator vit = std::find_if(
        std::begin(m_ParameterList), std::end(m_ParameterList),
        [&](auto const& param) { return param->GetKey() == lastkey; }
    );
    if (vit == std::end(m_ParameterList))
    { // parameter to replace not found : return false
      return false;
    }
    *vit = p;
    p->SetKey(std::string{lastkey});
  }

  // don't check type compatibility here, we may want to handle special cases
  // at higher level
  return true;
}

Parameter::Pointer ParameterGroup::GetParameterByIndex(unsigned int i, bool follow) const
{
  Parameter* param = m_ParameterList[i];
  if (follow)
  {
    param = this->ResolveParameter(param);
  }
  return Parameter::Pointer(param);
}

Parameter::Pointer ParameterGroup::GetParameterByKey(std::string_view name, bool follow) const
{
  auto const [parentName, tail] = ParameterKey::SplitKeyOnFirstDot(name);
  // std::cout << "GetParameterByKey(" << name << ") -> " << parentName << " / " << tail << std::endl;

  auto itParam = std::find_if(
      std::begin(m_ParameterList), std::end(m_ParameterList),
      [&](auto const& param) { return param->GetKey() == parentName;}
  );
  if (itParam == std::end(m_ParameterList)) {
    assert(!"Could not find parameter");
    itkExceptionMacro(<< "Could not find parameter " << name);
  }

  Parameter::Pointer parentParam = *itParam;
  // std::cout << "GetParameterByKey / param found -> " << parentParam->GetName() << std::endl;

  // follow proxy parameters (resolve intermediate group parameter because
  // we need to get to the requested parameter)
  if (follow || !tail.empty())
  {
    parentParam = this->ResolveParameter(parentParam.GetPointer());;
    assert(parentParam);
    // std::cout << "GetParameterByKey / Resolved (follow|tail) -> " << parentParam->GetName() << std::endl;
  }

  // If the name contains a child, make a recursive call
  if (!tail.empty())
  {
    // Handle ParameterGroup case
    ParameterGroup* parentAsGroup = dynamic_cast<ParameterGroup*>(parentParam.GetPointer());
    if (parentAsGroup)
    {
      return parentAsGroup->GetParameterByKey(tail, follow);
    } // ---< Group

    // Handle ChoiceParameter case
    ChoiceParameter* parentAsChoice = dynamic_cast<ChoiceParameter*>(parentParam.GetPointer());
    if (parentAsChoice)
    {
      // Check that splitName[1] is one of the choice
      auto const [choice, tail2] = ParameterKey::SplitKeyOnFirstDot(tail);
      assert(! choice.empty());
      // Will throw if not a choice key
      ParameterGroup::Pointer associatedParam = parentAsChoice->GetChoiceParameterGroupByKey(choice);
      if (!tail2.empty())
      {
        if (associatedParam.IsNull())
        {
          assert(!"Choice in parameter match no key");
          itkExceptionMacro(<< "Choice " << choice << "in " << parentName << "  has no key named " << tail2);
        }
        return associatedParam->GetParameterByKey(tail2, follow);
      }
      else
      {
        return associatedParam.GetPointer();
      }
    } // ---< Choice

    // Neither ParameterGroup, neither ChoiceParameter
    assert(!"No paramemeter with key");
    itkExceptionMacro(<< "No parameter with key " << name);
  }
  return parentParam.GetPointer();
}


#if 0
Parameter::Pointer ParameterGroup::GetParameterByKey(std::string const& name, bool follow) const
{
  // TODO: Simplify w/ SplitKeyOnFirstDot
  ParameterKey pName(name);

  // Split the parameter name
  std::vector<std::string> splitName = pName.Split();

  // Get the first parameter key
  std::string parentName = pName.GetFirstElement();

  // Look for parentName in the current group
  Parameter::Pointer          parentParam;
  for (Parameter::Pointer param : m_ParameterList)
  {
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
    Parameter* rawParam = this->ResolveParameter(parentParam.GetPointer());
    parentParam         = rawParam;
  }

  // If the name contains a child, make a recursive call
  if (splitName.size() > 1)
  {
    // Handle ParameterGroup case
    ParameterGroup* parentAsGroup = dynamic_cast<ParameterGroup*>(parentParam.GetPointer());
    if (parentAsGroup)
    {
      // Remove the parent from the param name
      std::ostringstream                       childNameOss;
      std::vector<std::string>::const_iterator vvit = splitName.begin() + 1;
      while (vvit != splitName.end())
      {
        childNameOss << *vvit;
        ++vvit;
        if (vvit != splitName.end())
        {
          childNameOss << ".";
        }
      }
      std::string childName = childNameOss.str();

      return parentAsGroup->GetParameterByKey(childName, follow);
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
          itkExceptionMacro(<< "Choice " << splitName[1] << "in " << splitName[0] << "  has no key named " << splitName[2]);
        }

        // Remove the parent and the choice value from the param name
        std::ostringstream                       childNameOss;
        std::vector<std::string>::const_iterator vvvit = splitName.begin() + 2;
        while (vvvit != splitName.end())
        {
          childNameOss << *vvvit;
          ++vvvit;
          if (vvvit != splitName.end())
          {
            childNameOss << ".";
          }
        }
        std::string childName = childNameOss.str();
        return associatedParam->GetParameterByKey(childName, follow);
      }
      return associatedParam.GetPointer();
    }
    // Neither ParameterGroup, neither ChoiceParameter
    itkExceptionMacro(<< "No parameter with key " << name);
  }

  return parentParam.GetPointer();
}
#endif

unsigned int ParameterGroup::GetNumberOfParameters() const
{
  return m_ParameterList.size();
}

Parameter* ParameterGroup::ResolveParameter(Parameter* param)
{
  Parameter* ret = param;
  if (ret == nullptr)
  {
    assert(!"Can't resolve NULL parameter!");
    itkGenericExceptionMacro("Can't resolve NULL parameter!");
  }
 for (
     ProxyParameter* castParam  = dynamic_cast<ProxyParameter*>(ret)
     ; castParam
     ; castParam  = dynamic_cast<ProxyParameter*>(ret)
     )
  {
    ProxyParameter::ProxyTargetType target      = castParam->GetTarget();
    ParameterGroup*                 targetGroup = dynamic_cast<ParameterGroup*>(target.first.GetPointer());
    if (targetGroup)
    {
      ret = targetGroup->GetParameterByKey(target.second);
    }
    else
    {
      assert(!"Target group of a proxy parameter is not of type ParameterGroup");
      itkGenericExceptionMacro("Target group of a proxy parameter is not of type ParameterGroup");
    }
    if (ret == param)
    {
      assert(!"Cycle detected with proxy parameters!");
      itkGenericExceptionMacro("Cycle detected with proxy parameters!");
    }
  }
  return ret;
}
}
}
