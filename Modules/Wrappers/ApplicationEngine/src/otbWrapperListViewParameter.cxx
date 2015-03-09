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
  if (m_ChoiceList.empty())
  {
    return "";
  }
  else
  {
    return m_ChoiceList[i].m_Key;
  }
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
  if (m_ChoiceList.empty())
  {
    return "";
  }
  else
  {
    return m_ChoiceList[i].m_Name;
  }
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


/** Clear choices */
void
ListViewParameter::ClearChoices()
{
  m_ChoiceList.clear();
}

void
ListViewParameter::SetSelectedItemsByNames()
{
  std::vector<int> selectedItems;
  std::vector<std::string> names = this->GetChoiceNames();
  for(unsigned int i=0; i<m_SelectedNames.size(); i++)
    {
    const std::string selectedName = m_SelectedNames[i];
    unsigned int j(0);
      for(; j<names.size(); j++)
      {
      if( names[j] == selectedName )
            {
        selectedItems.push_back(j);
        break;
        }
      }
    // If not found
    if( j==names.size() )
      {
      std::ostringstream oss;
      for( j=0; j<names.size(); j++)
        {
        oss<<names[j];
        if( j < names.size()-1 )
          {
          oss<<", ";
          }
        }
      itkExceptionMacro("Value "<<selectedName<<" not found in the choices possibilities"<<oss.str()<<".");
      }
    }
  this->SetSelectedItems(selectedItems);
}


void
ListViewParameter::SetSelectedItemsByKeys()
{
  std::vector<int> selectedItems;
  std::vector<std::string> keys = this->GetChoiceKeys();
  for(unsigned int i=0; i<m_SelectedKeys.size(); i++)
    {
    const std::string selectedKey = m_SelectedKeys[i];
    unsigned int j(0);
    std::ostringstream oss;
      for(; j<keys.size(); j++)
      {
      if( keys[j] == selectedKey )
            {
        selectedItems.push_back(j);
        break;
        }
      }
    // If not found
    if( j==keys.size() )
      {
      for( j=0; j<keys.size(); j++)
        {
        oss<<keys[j];
        if( j < keys.size()-1 )
          {
          oss<<", ";
          }
        }
      itkExceptionMacro("Value "<<selectedKey<<" not found in the choices possibilities"<<oss.str()<<".");
      }
    }
  this->SetSelectedItems(selectedItems);
}
}
}
