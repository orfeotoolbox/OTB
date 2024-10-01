/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperListViewParameter_h
#define otbWrapperListViewParameter_h

#include "otbWrapperParameterGroup.h"
#include <string>

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
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT ListViewParameter : public Parameter
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

  itkSetMacro(SingleSelection, bool);
  itkGetMacro(SingleSelection, bool);
  itkBooleanMacro(SingleSelection);

  /** Add a value to the choice */
  void AddChoice(std::string choicekey, std::string choiceName);

  /** Get the key of a specific choice value */
  std::string GetChoiceKey(int i) const;

  /** Get the list of the different choice keys */
  std::vector<std::string> GetChoiceKeys();

  /** Get the long name of a specific choice value */
  std::string GetChoiceName(int i);

  /** Get the list of the different choice keys */
  std::vector<std::string> GetChoiceNames();

  /** Get the number of available choice */
  unsigned int GetNbChoices(void);

  /** Set choice value */
  virtual void SetValue(unsigned int v);

  /** Set choice value by its key */
  virtual void SetValue(std::string choiceKey);

  /** Return any value */
  virtual unsigned int GetValue() const;

  bool HasValue() const override
  {
    // a choice parameter always has a value
    return true;
  }

  void ClearValue() override
  {
    // nothing to do : a choice parameter always has a value
  }

  void ClearChoices();

  std::vector<int> GetSelectedItems()
  {
    return m_SelectedItems;
  }

  void SetSelectedNames(std::vector<std::string> selectedNames);

  std::vector<std::string> GetSelectedNames() const
  {
    return m_SelectedNames;
  }


  void SetSelectedKeys(std::vector<std::string> selectedKeys);

  std::vector<std::string> GetSelectedKeys()
  {
    return m_SelectedKeys;
  }

  /** Set selected items using a lit of selected keys.
   *  OBSOLETE : this method is not needed anymore and does nothing. */
  void SetSelectedItemsByKeys()
  {
  }

  /** Set selected items using a lit of selected names.
   *  OBSOLETE : this method is not needed anymore and does nothing. */
  void SetSelectedItemsByNames()
  {
  }

  void SetSelectedItems(std::vector<std::string> selectedItems)
  {
    std::vector<int> items;
    for (unsigned int i = 0; i < selectedItems.size(); i++)
    {
      items.push_back(atoi(selectedItems[i].c_str()));
    }
    this->SetSelectedItems(items);
  }

  void SetSelectedItems(std::vector<int> selectedItems)
  {
    m_SelectedItems = selectedItems;
    m_SelectedNames.clear();
    m_SelectedKeys.clear();
    // update selected names and keys
    std::vector<std::string> names = this->GetChoiceNames();
    std::vector<std::string> keys  = this->GetChoiceKeys();
    for (unsigned int i = 0; i < m_SelectedItems.size(); i++)
    {
      m_SelectedNames.push_back(names[m_SelectedItems[i]]);
      m_SelectedKeys.push_back(keys[m_SelectedItems[i]]);
    }
  }

  ParameterType GetType() const override
  {
    return ParameterType_ListView;
  }

  std::string ToString() const override
  {
    std::string choiceKey    = GetChoiceKey(GetValue());
    size_t      lastPointPos = choiceKey.find_last_of('.');

    if (lastPointPos != std::string::npos)
    {
      return choiceKey.substr(lastPointPos);
    }
    return choiceKey;
  }

  void FromString(const std::string& value) override
  {
    SetSelectedNames({value});
  }

  std::vector<std::string> ToStringList() const override
  {
    return GetSelectedNames();
  }

  void FromStringList(const std::vector<std::string>& values) override
  {
    SetSelectedNames(values);
  }

protected:
  /** Constructor */
  ListViewParameter();

  /** Destructor */
  ~ListViewParameter() override;

  struct ListViewChoice
  {
    ListViewChoice()
    {
    }

    std::string m_Key;
    std::string m_Name;
  };

  typedef std::vector<ListViewChoice> ChoiceList;
  ChoiceList                          m_ChoiceList;
  unsigned int                        m_CurrentChoice;
  std::vector<int>                    m_SelectedItems;
  std::vector<std::string>            m_SelectedKeys;
  std::vector<std::string>            m_SelectedNames;
  bool                                m_SingleSelection;

private:
  ListViewParameter(const ListViewParameter&) = delete;
  void operator=(const ListViewParameter&) = delete;

}; // End class Parameter

} // End namespace Wrapper
} // End namespace otb

#endif
