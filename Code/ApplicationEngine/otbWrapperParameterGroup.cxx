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
#include "otbWrapperParameterGroup.h"
#include "otbWrapperChoiceParameter.h"
#include "otbWrapperListViewParameter.h"
#include "otbWrapperDirectoryParameter.h"
#include "otbWrapperEmptyParameter.h"
#include "otbWrapperInputFilenameParameter.h"
#include "otbWrapperInputFilenameListParameter.h"
#include "otbWrapperOutputFilenameParameter.h"
#include "otbWrapperComplexInputImageParameter.h"
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperInputVectorDataParameter.h"
#include "otbWrapperNumericalParameter.h"
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperComplexOutputImageParameter.h"
#include "otbWrapperOutputVectorDataParameter.h"
#include "otbWrapperRadiusParameter.h"
#include "otbWrapperStringParameter.h"
#include "otbWrapperStringListParameter.h"
#include "otbWrapperInputImageListParameter.h"
#include "otbWrapperInputVectorDataListParameter.h"
#include "otbWrapperParameterKey.h"
#include "otbWrapperRAMParameter.h"

#include <boost/algorithm/string.hpp>

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
    parameters.push_back( param->GetKey() );

    if (recursive && dynamic_cast<ParameterGroup*>(param))
      {
      ParameterGroup* paramAsGroup = dynamic_cast<ParameterGroup*>(param);
      std::vector<std::string> subparams = paramAsGroup->GetParametersKeys();
      for (std::vector<std::string>::const_iterator it = subparams.begin();
           it != subparams.end(); ++it)
        {
        parameters.push_back( std::string(paramAsGroup->GetKey()) + "."  + *it );
        }
      }
    else if (recursive && dynamic_cast<ChoiceParameter*>(param))
      {
      ChoiceParameter* paramAsChoice = dynamic_cast<ChoiceParameter*>(param);

      std::vector<std::string> subparams = paramAsChoice->GetParametersKeys();
      for (std::vector<std::string>::const_iterator it = subparams.begin();
           it != subparams.end(); ++it)
        {
        parameters.push_back( std::string(paramAsChoice->GetKey()) + "."  + *it );
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
  std::vector<std::string> splittedKey = pKey.Split();

  if( splittedKey.size() > 1 )
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
      itkExceptionMacro(<<"No choice parameter key given");
    }
}

/** Remove the choices made for the ViewList parameter */
void
ParameterGroup::ClearChoices(std::string paramKey)
{
  ParameterKey pKey( paramKey );
  // Split the parameter name
  std::vector<std::string> splittedKey = pKey.Split();

  std::string parentkey;
  Parameter::Pointer parentParam;

  if (splittedKey.size() > 1)
    {
    parentkey = pKey.GetRoot();
    parentParam = GetParameterByKey(parentkey);
    }
  else
    {
    parentParam = GetParameterByKey(splittedKey[0]);
    }

   // parentParam must be a choice, a listBox or this is an error
  ListViewParameter* listBoxParentAsChoice = dynamic_cast<ListViewParameter*>(parentParam.GetPointer());

  if (listBoxParentAsChoice)
    {
    listBoxParentAsChoice->ClearChoices();
    }
  else
    {
    itkExceptionMacro(<<parentkey << " is not a ListView");
    }
}

/** Get the choices made in the QListWidget */
std::vector<int>
ParameterGroup::GetSelectedItems(std::string paramKey)
{
  std::vector<int> selectedItems;
  ParameterKey pKey( paramKey );
  // Split the parameter name
  std::vector<std::string> splittedKey = pKey.Split();

  std::string parentkey;
  Parameter::Pointer parentParam;

  if (splittedKey.size() > 1)
    {
    parentkey = pKey.GetRoot();
    parentParam = GetParameterByKey(parentkey);
    }
  else
    {
    parentParam = GetParameterByKey(splittedKey[0]);
    }

   // parentParam must be a choice, a listBox or this is an error
  ListViewParameter* listBoxParentAsChoice = dynamic_cast<ListViewParameter*>(parentParam.GetPointer());
  
  if (listBoxParentAsChoice)
    {
    selectedItems = listBoxParentAsChoice->GetSelectedItems();
    }
  else
    {
    itkExceptionMacro(<<parentkey << " is not a ListView");
    }

  return selectedItems;
}
  
  
/** Add a new parameter to the parameter group */
void
ParameterGroup::AddParameter(ParameterType type, std::string paramKey, std::string paramName)
{
  ParameterKey pKey(paramKey);
  // Split the parameter name
  std::vector<std::string> splittedKey = pKey.Split();

  // Get the last subkey
  std::string lastkey = pKey.GetLastElement();

  std::string parentkey;
  Parameter::Pointer parentParam;

  if (splittedKey.size() > 1)
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
      case ParameterType_Empty:
        {
        newParam = EmptyParameter::New();
        }
        break;
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
      }

    if (newParam.IsNull())
      {
      itkExceptionMacro(<< "Parameter type not supported for " << paramKey);
      }

    newParam->SetKey(lastkey);
    newParam->SetName(paramName);

    // If splittedKey is greater than 1, that means that the parameter
    // is not a root, and have a parent(s):
    // - Add the parent as root of this param
    // - Add the param as a child of its parents
    if (splittedKey.size() > 1)
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

Parameter::Pointer
ParameterGroup::GetParameterByIndex(unsigned int i)
{
  return m_ParameterList[i];
}

Parameter::Pointer
ParameterGroup::GetParameterByKey(std::string name)
{
  ParameterKey pName(name);
 // Split the parameter name
  std::vector<std::string> splittedName = pName.Split();

  // Get the first parameter key
  std::string parentName = pName.GetFirstElement();

  // Look for parentName in the current group
  Parameter::Pointer parentParam;
  ParameterListType::iterator it;
  for (it = m_ParameterList.begin(); it != m_ParameterList.end(); ++it)
    {
    Parameter::Pointer param = *it;
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

  // If the name contains a child, make a recursive call
  if (splittedName.size() > 1)
    {
    // Handle ParameterGroup case
    ParameterGroup* parentAsGroup = dynamic_cast<ParameterGroup*>(parentParam.GetPointer());
    if (parentAsGroup)
      {
      // Remove the parent from the param name
      std::ostringstream childNameOss;
      std::vector<std::string>::const_iterator it = splittedName.begin() + 1;
      while(it != splittedName.end())
        {
        childNameOss << *it;
        ++it;
        if (it != splittedName.end())
          {
          childNameOss << ".";
          }
        }
      std::string childName = childNameOss.str();

      return parentAsGroup->GetParameterByKey(childName);
      }

    // Handle ChoiceParameter case
    ChoiceParameter* parentAsChoice = dynamic_cast<ChoiceParameter*>(parentParam.GetPointer());
    if (parentAsChoice)
      {
      // Check that splittedName[1] is one of the choice
      ParameterGroup::Pointer associatedParam;

      // will throw if splittedName[1] is not a choice key
      associatedParam = parentAsChoice->GetChoiceParameterGroupByKey(splittedName[1]);

      if (splittedName.size() > 2)
        {
        if (associatedParam.IsNull())
          {
          itkExceptionMacro(<< "Choice " << splittedName[1] << "in "
                            << splittedName[0] << "  has no key named "
                            << splittedName[2]);
          }

        // Remove the parent and the choice value from the param name
        std::ostringstream childNameOss;
        std::vector<std::string>::const_iterator it = splittedName.begin() + 2;
        while(it != splittedName.end())
          {
          childNameOss << *it;
          ++it;
          if (it != splittedName.end())
            {
            childNameOss << ".";
            }
          }
        std::string childName = childNameOss.str();
        return associatedParam->GetParameterByKey(childName);
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

}
}
