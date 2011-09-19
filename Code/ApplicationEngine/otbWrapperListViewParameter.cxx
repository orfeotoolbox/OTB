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
#include "otbWrapperListViewParameter.h"

namespace otb
{
namespace Wrapper
{

ListViewParameter::ListViewParameter()
  : m_CurrentChoice(0)
{
}

ListViewParameter::~ListViewParameter()
{
}


void
ListViewParameter::AddChoice( std::string choicekey, std::string choiceName )
{
  ListViewChoice choice;
  choice.m_Key = choicekey;
  choice.m_Name = choiceName;
  m_ChoiceList.push_back(choice);
}

std::string
ListViewParameter::GetChoiceKey( int i )
{
  return m_ChoiceList[i].m_Key;
}


std::vector<std::string>
ListViewParameter::GetChoiceKeys( )
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
ListViewParameter::GetChoiceName( int i )
{
  return m_ChoiceList[i].m_Name;
}


std::vector<std::string>
ListViewParameter::GetChoiceNames( )
{
  std::vector<std::string> ret;
  ChoiceList::iterator it = m_ChoiceList.begin();

  for (it = m_ChoiceList.begin(); it != m_ChoiceList.end(); ++it)
    {
    ret.push_back(it->m_Name);
    }
  return ret;
}

unsigned int
ListViewParameter::GetNbChoices( void )
{
  return m_ChoiceList.size();
}

void
ListViewParameter::SetValue(unsigned int v)
{
  m_CurrentChoice = v;
  SetActive(true);
  // Call Modified();
  this->Modified();
}

void
ListViewParameter::SetValue(std::string choiceKey)
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
ListViewParameter::GetValue()
{
  return m_CurrentChoice;
}

// TODO : not implemented yet
std::vector<std::string>
ListViewParameter::GetParametersKeys()
{
  std::vector<std::string> parameters;

  ChoiceList::iterator cit = m_ChoiceList.begin();

  // for (cit = m_ChoiceList.begin(); cit != m_ChoiceList.end(); ++cit)
  //   {
  //   if (cit->m_AssociatedParameter)
  //     {
  //     std::vector<std::string> subparams = cit->m_AssociatedParameter->GetParametersKeys();
  //     for (std::vector<std::string>::const_iterator it = subparams.begin();
  //          it != subparams.end(); ++it)
  //       {
  //       parameters.push_back( cit->m_Key + "."  + *it );
  //       }
  //     }
  //   }
  return parameters;
}


/** Clear choices */
void
ListViewParameter::ClearChoices()
{
  m_ChoiceList.clear();
}

}
}

