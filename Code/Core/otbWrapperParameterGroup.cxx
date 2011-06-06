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
  // Split the parameter name
  std::vector<std::string> splittedName;
  boost::algorithm::split(splittedName, name, boost::is_any_of("."), boost::token_compress_on);

  // Get the first parameter key
  std::string parentName = splittedName[0];

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
          childNameOss << "." << std::endl;
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
      unsigned int nbChoices = parentAsChoice->GetNbChoices();
      for (int i = 0; i < nbChoices; ++i)
        {
        if ( parentAsChoice->GetChoiceKey(i) == splittedName[1] )
          {
          associatedParam = parentAsChoice->GetChoiceAssociatedParameter(i);
          break;
          }
        }

      if (splittedName.size() > 2)
        {
        if (associatedParam.IsNull())
          {
          itkExceptionMacro(<< "Parameter " << splittedName[1] << "has no child named " << splittedName[2])
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
            childNameOss << "." << std::endl;
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
  m_ParameterList.size();
}

}
}

