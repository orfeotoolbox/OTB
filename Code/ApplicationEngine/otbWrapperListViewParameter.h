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
#ifndef __otbWrapperListViewParameter_h
#define __otbWrapperListViewParameter_h

#include "otbWrapperParameter.h"
#include "otbWrapperParameterGroup.h"

namespace otb
{
namespace Wrapper
{

/** \class ListViewParameter
 *  \brief This class represent a ListWidget parameter for the wrapper framework
 *
 * The row selected in the ListWidget are stored in a std::vector. The
 * method GetSelectedItems() allow the user to access to this method.
 *
 */
class ITK_ABI_EXPORT ListViewParameter
  : public Parameter
{
public:
  /** Standard class typedef */
  typedef ListViewParameter             Self;
  typedef Parameter                     Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Defining ::New() static method */
  itkNewMacro(Self);

  /** RTTI support */
  itkTypeMacro(ListViewParameter, Parameter);

  /** Add a value to the choice */
  void AddChoice( std::string choicekey, std::string choiceName );

  /** Get the key of a specific choice value */
  std::string GetChoiceKey( int i );

  /** Get the list of the different choice keys */
  std::vector<std::string> GetChoiceKeys();

  /** Get the long name of a specific choice value */
  std::string GetChoiceName( int i );

  /** Get the list of the different choice keys */
  std::vector<std::string> GetChoiceNames();

  /** Get the number of available choice */
  unsigned int GetNbChoices( void );

  /** Set choice value */
  virtual void SetValue(unsigned int v);

  /** Set choice value by its key */
  virtual void SetValue(std::string choiceKey);

  /** Return any value */
  virtual unsigned int GetValue();

  bool HasValue() const
  {
    // a choice parameter always has a value
    return true;
  }

  void ClearValue()
  {
    // nothing to do : a choice parameter always has a value
  }

  void ClearChoices();

  std::vector<int> GetSelectedItems()
  {
    if( m_SelectedNames.size() != 0 )
      {
        this->SetSelectedItemsByNames();
      }
    else if( m_SelectedKeys.size() != 0 )
      {
        this->SetSelectedItemsByKeys();
      }

    return m_SelectedItems;
  }

  void SetSelectedNames(std::vector<std::string> selectedNames)
  {
    m_SelectedNames = selectedNames;
    m_SelectedItems.clear();
    m_SelectedKeys.clear();
  }

  std::vector<std::string> GetSelectedNames()
    {
      return m_SelectedNames;
    }


  void SetSelectedKeys(std::vector<std::string> selectedKeys)
  {
    m_SelectedKeys = selectedKeys;
    m_SelectedItems.clear();
    m_SelectedNames.clear();
  }

  std::vector<std::string> GetSelectedKeys()
    {
      return m_SelectedKeys;
    }

  /** Set selected items using a lit of selected keys. */
  void SetSelectedItemsByKeys();
  /** Set selected items using a lit of selected names. */
  void SetSelectedItemsByNames();
  void SetSelectedItems(std::vector<std::string> selectedItems)
  {
    std::vector<int> items;
    for( unsigned int i=0; i<selectedItems.size(); i++ )
      {
        items.push_back( atoi( selectedItems[i].c_str() ) );
      }
    m_SelectedItems = items;
    m_SelectedNames.clear();
    m_SelectedKeys.clear();
  }

  void SetSelectedItems(std::vector<int> selectedItems)
  {
    m_SelectedItems = selectedItems;
  }

protected:
  /** Constructor */
  ListViewParameter();

  /** Destructor */
  virtual ~ListViewParameter();

  struct ListViewChoice
  {
    ListViewChoice() {}

    std::string             m_Key;
    std::string             m_Name;
  };

  typedef std::vector<ListViewChoice> ChoiceList;
  ChoiceList                          m_ChoiceList;
  unsigned int                        m_CurrentChoice;
  std::vector<int>                    m_SelectedItems;
  std::vector<std::string>            m_SelectedKeys;
  std::vector<std::string>            m_SelectedNames;

private:
  ListViewParameter(const ListViewParameter &); //purposely not implemented
  void operator =(const ListViewParameter&); //purposely not implemented

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
