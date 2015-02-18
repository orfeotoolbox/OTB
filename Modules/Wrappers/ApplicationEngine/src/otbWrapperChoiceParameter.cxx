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
  choice.m_AssociatedParameter->SetName(choiceName);
  choice.m_AssociatedParameter->SetRoot(this);

  m_ChoiceList.push_back(choice);

  // check if the new choice matches the m_CurrentChoice : if so the group should be active.
  if (m_CurrentChoice == (m_ChoiceList.size() - 1))
    {
    m_ChoiceList[m_CurrentChoice].m_AssociatedParameter->SetActive(true);
    }

  // Add the associated parameter as a child
  // in order to not have a gap in the children hierarchy
  this->AddChild(choice.m_AssociatedParameter.GetPointer());
}

std::string
ChoiceParameter::GetChoiceKey( int i )
{
  return m_ChoiceList[i].m_Key;
}


std::vector<std::string>
ChoiceParameter::GetChoiceKeys( )
{
  std::vector<std::string> ret;
  ChoiceList::iterator it = m_ChoiceList.begin();

  for (it = m_ChoiceList.begin(); it != m_ChoiceList.end(); ++it)
    {
    ret.push_back(it->m_Key);
    }

  return ret;
}


std::string
ChoiceParameter::GetChoiceName( int i )
{
  return m_ChoiceList[i].m_Name;
}


std::vector<std::string>
ChoiceParameter::GetChoiceNames( )
{
  std::vector<std::string> ret;
  ChoiceList::iterator it = m_ChoiceList.begin();

  for (it = m_ChoiceList.begin(); it != m_ChoiceList.end(); ++it)
    {
    ret.push_back(it->m_Name);
    }
  return ret;
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
  m_CurrentChoice = v;
  SetActive(true);
  // update the active flag in sub parameters
  for (unsigned int i=0; i< m_ChoiceList.size(); i++)
    {
    if (m_ChoiceList[i].m_AssociatedParameter)
      {
      if (i==m_CurrentChoice)
        {
        if (!m_ChoiceList[i].m_AssociatedParameter->GetActive())
          {
          m_ChoiceList[i].m_AssociatedParameter->SetActive(true);
          }
        }
      else
        {
        if (m_ChoiceList[i].m_AssociatedParameter->GetActive())
          {
          m_ChoiceList[i].m_AssociatedParameter->SetActive(false);
          }
        }
      }
    }

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
      SetValue(i);
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

/** Return any value */
std::vector<std::string>
ChoiceParameter::GetParametersKeys()
{
  std::vector<std::string> parameters;

  ChoiceList::iterator cit = m_ChoiceList.begin();

  for (cit = m_ChoiceList.begin(); cit != m_ChoiceList.end(); ++cit)
    {
    if (cit->m_AssociatedParameter)
      {
      std::vector<std::string> subparams = cit->m_AssociatedParameter->GetParametersKeys();
      for (std::vector<std::string>::const_iterator it = subparams.begin();
           it != subparams.end(); ++it)
        {
        parameters.push_back( cit->m_Key + "."  + *it );
        }
      }
    }
  return parameters;
}

}
}
