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
#include "otbWrapperChoiceParameter.h"

namespace otb
{
namespace Wrapper
{

ChoiceParameter::ChoiceParameter()
  : m_CurrentChoice(0)
{
}

ChoiceParameter::~ChoiceParameter()
{
}

void
ChoiceParameter::AddChoice( std::string key, std::string name, Parameter* param )
{
  ParameterGroup* paramAsGroup = dynamic_cast<ParameterGroup*>(param);
  if ( paramAsGroup != 0 )
    {
    Choice choice;
    choice.m_Key = key;
    choice.m_Name = name;
    choice.m_AssociatedParameter = paramAsGroup;
    m_ChoiceList.push_back(choice);
    }
  else
    {
    // wrap in  group first
    ParameterGroup::Pointer group = ParameterGroup::New();
    group->AddParameter(param);
    AddChoice(key, name, group.GetPointer());
    }
}

std::string
ChoiceParameter::GetChoiceKey( int i )
{
  return m_ChoiceList[i].m_Key;
}

std::string
ChoiceParameter::GetChoiceName( int i )
{
  return m_ChoiceList[i].m_Name;
}

ParameterGroup::Pointer
ChoiceParameter::GetChoiceAssociatedParameter( int i )
{
  return m_ChoiceList[i].m_AssociatedParameter;
}

unsigned int
ChoiceParameter::GetNbChoices( void )
{
  return m_ChoiceList.size();
}

/** Set any value */
void
ChoiceParameter::SetValue(unsigned int v)
{
  // Perform any cast
  m_CurrentChoice = v;
  // Call Modified();
  this->Modified();
}

/** Return any value */
unsigned int
ChoiceParameter::GetValue()
{
  return m_CurrentChoice;
}

/** Set any value */
void
ChoiceParameter::SetAnyValue(boost::any v)
{
  // Perform any cast
  unsigned int val = boost::any_cast<unsigned int>(v);

  if ( val >= GetNbChoices() )
    {
    itkExceptionMacro(<< "Invalid choice value : " << val)
    }

  m_CurrentChoice = val;

  // Call Modified();
  this->Modified();
}

/** Return any value */
boost::any
ChoiceParameter::GetAnyValue()
{
  return boost::any(m_CurrentChoice);
}


}
}

