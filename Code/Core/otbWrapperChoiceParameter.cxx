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
ChoiceParameter::AddChoice( std::string choicekey, std::string choiceName )
{
  Choice choice;
  choice.m_Key = choicekey;
  choice.m_Name = choiceName;
  choice.m_AssociatedParameter = ParameterGroup::New();
  m_ChoiceList.push_back(choice);
}

void
ChoiceParameter::AddParameterToChoice( std::string choicekey, std::string choiceName, Parameter* param )
{

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
ChoiceParameter::GetChoiceParameterGroupByIndex( int i )
{
  return m_ChoiceList[i].m_AssociatedParameter;
}

ParameterGroup::Pointer
ChoiceParameter::GetChoiceParameterGroupByKey( std::string choiceKey )
{
  ChoiceList::iterator it = m_ChoiceList.begin();

  for (it = m_ChoiceList.begin(); it != m_ChoiceList.end(); ++it)
    {
    if ( it->m_Key == choiceKey )
      {
      return it->m_AssociatedParameter;
      }
    }

  itkExceptionMacro(<< "Cannot find " << choiceKey);
}

unsigned int
ChoiceParameter::GetNbChoices( void )
{
  return m_ChoiceList.size();
}

void
ChoiceParameter::SetValue(unsigned int v)
{
  // Perform any cast
  m_CurrentChoice = v;
  // Call Modified();
  this->Modified();
}

void
ChoiceParameter::SetValue(std::string choiceKey)
{
  ChoiceList::iterator it = m_ChoiceList.begin();

  unsigned int i = 0;
  for (it = m_ChoiceList.begin(); it != m_ChoiceList.end(); ++it)
    {
    if ( it->m_Key == choiceKey )
      {
      m_CurrentChoice = i;
      this->Modified();
      return;
      }
    ++i;
    }

  itkExceptionMacro(<< "Cannot find " << choiceKey);
}

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

