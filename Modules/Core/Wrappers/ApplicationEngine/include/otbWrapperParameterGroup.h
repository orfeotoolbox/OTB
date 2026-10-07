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

#ifndef otbWrapperParameterGroup_h
#define otbWrapperParameterGroup_h

#include "itkObject.h"
#include "otbWrapperParameter.h"
#include <vector>
#include <string>

namespace otb
{
namespace Wrapper
{

/**
 * \class Group
 *
 * \ingroup OTBApplicationEngine
 */
class OTBApplicationEngine_EXPORT ParameterGroup : public Parameter
{
public:
  using Self         = ParameterGroup;
  using Superclass   = Parameter;
  using Pointer      = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  itkNewMacro(Self);

  itkTypeMacro(ParameterList, Parameter);

  static constexpr auto Type = ParameterType_Group;

  void AddParameter(Parameter::Pointer p);

  /** Method to substitute a parameter in a group.
   *  The function returns true on success, false on failure */
  bool ReplaceParameter(std::string const& key, Parameter::Pointer p);

  /** Add a new choice value to an existing choice parameter */
  void AddChoice(std::string paramKey, std::string paramName);

  /** Remove choices made in ListViewParameter widget*/
  void ClearChoices(std::string paramKey);

  /** Get the choices made in a ListView Parameter widget*/
  std::vector<int> GetSelectedItems(std::string const& paramKey) const;

  /** Add a new parameter to the parameter group.
   * The parent key of paramKey can be the path to a parameter group
   * or the path to a choice value
   */
  void AddParameter(ParameterType type, std::string_view paramKey, std::string paramName);

  /**
   * Templated overload of `AddParameter`.
   * Unlike the non-templated version, this one is simplifier and more efficient as type association
   * is done through `ParameterTypeTraits<type>`.

   * @tparam type  Exact type of parameter to add (enum value of type `ParameterType`)
   * @param[in] paramKey   Parameter key -- can be a path in a parameter group
   * @param[in] paramName  Parameter name
   *
   * \return the new parameter created -- with no type degradation
   */
  template <ParameterType type>
  auto AddParameter(std::string paramKey, std::string paramName)
  -> ParameterTypeTraits_t<type>*
  {
    ParameterKey pKey(paramKey);
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
    if (parentAsGroup)
    {
      using ActualParameterType = ParameterTypeTraits_t<type>;
      Parameter::Pointer newParam = ActualParameterType::New();
      newParam->SetKey(std::move(paramKey));
      newParam->SetName(std::move(paramName));

      if (parentAsGroup != this)
      {
        newParam->SetRoot(parentAsGroup);
        parentAsGroup->AddChild(newParam);
      }
      parentAsGroup->AddParameter(newParam);
      return newParam;
    }
    else
    {
      itkExceptionMacro(<< "Cannot add " << lastkey << " to parameter " << parentkey);
    }
  }


  Parameter::Pointer GetParameterByIndex(unsigned int i, bool follow = true) const;

  Parameter::Pointer GetParameterByKey(std::string const& name, bool follow = true) const;

  void Clear()
  {
    m_ParameterList.clear();
  }

  /** Get the parameter type as string from its ParameterType enum
   * For example if type of parameter is ParameterType_InputImage this
   * function return the string InputImage */
  std::string GetParameterTypeAsString(ParameterType paramType) const;

  /* Get the parameter type from its string version of ParameterType enum */
  ParameterType GetParameterTypeFromString(std::string const& paramType) const;

  unsigned int GetNumberOfParameters() const;

  std::vector<std::string> GetParametersKeys(bool recursive = true) const;

  // Always has value
  bool HasValue() const override
  {
    return true;
  }

  /** Resolve potential proxy parameters by following their targets until
   *  a non-proxy parameter. It will detect cycles and report an error */
  static Parameter* ResolveParameter(Parameter* param);

  ParameterType GetType() const override
  {
    return Type;
  }

protected:
  using Parameter::Parameter;
  ~ParameterGroup() override = default;

  typedef std::vector<Parameter::Pointer> ParameterListType;
  ParameterListType                       m_ParameterList;

private:
  ParameterGroup(const ParameterGroup&) = delete;
  void operator=(const ParameterGroup&) = delete;
};

/** `ParameterTypeTraits` specialisation for `ParameterType_Group`. */
template <>
struct ParameterTypeTraits<ParameterType_Group>
{
  using Type = ParameterGroup;
};

static_assert(ParameterTypeTraits<ParameterType_Group>::Type::Type == ParameterType_Group);

}
}

#endif
