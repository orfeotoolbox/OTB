/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbWrapperParameterList_h
#define otbWrapperParameterList_h


#include "otbWrapperAbstractParameterList.h"
#include "otbWrapperStringListInterface.h"


namespace otb
{

namespace Wrapper
{


/** \class ParameterList
 *  \brief This class represents a InputImage parameter
 *
 * \ingroup OTBApplicationEngine
 */
template <typename T>
class ParameterList : public AbstractParameterList
{
  //
  // Public types.
public:
  /** Standard class typedef */
  typedef ParameterList         Self;
  typedef AbstractParameterList Superclass;

  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Custom types */
  typedef std::vector<typename T::Pointer> ParameterVector;

  //
  // Public methods.
public:
  /** RTTI support */
  itkTypeMacro(ParameterList, AbstractParameterList);

  /** */
  typename ParameterVector::const_iterator begin() const;

  /** */
  typename ParameterVector::const_iterator end() const;

  /** */
  void ClearValue() override;

  /** */
  bool HasValue() const override;

  /** Set file form a list of filenames */
  void SetListFromFileName(const StringVector&) override;

  /** */
  void InsertNullElement(std::size_t = -1) override;

  /** Add a filename from a filename */
  void AddFromFileName(const std::string&) override;

  /** */
  void Insert(const std::string&, std::size_t = -1) override;

  void InsertElement(typename T::Pointer, std::size_t = -1);

  /** Set one specific stored filename. */
  void SetNthFileName(std::size_t, const std::string&) override;

  /** */
  std::size_t SetStrings(const StringVector&) override;

  /** */
  std::size_t GetStrings(StringVector&) const override;

  /** Get the stored image filename list */
  StringVector GetFileNameList() const override;

  /** Get one specific stored image filename. */
  std::string GetNthFileName(std::size_t) const override;

  /** */
  const std::string& GetToolTip(std::size_t) const override;

  /** */
  using StringListInterface::Erase;
  void Erase(std::size_t start, std::size_t count) override;

  /** Retrieve number of elements */
  std::size_t Size() const override;

  /** */
  bool IsActive(size_t) const override;

  /** */
  void Swap(std::size_t, std::size_t) override;

  typename T::Pointer GetNthElement(std::size_t);

  std::vector<std::string> ToStringList() const override;
  void FromStringList(const std::vector<std::string>& value) override;
  std::string ToString() const override;

protected:
  /** Constructor */
  ParameterList();

  /** Destructor */
  ~ParameterList() override;

  // Protected methods.
protected:
  /** Utility method to factorize some code */
  template <typename L, typename From, typename Get>
  void SetObjectList(L&, const L&, From, Get);

  /** Utility method to factorize some code */
  template <typename L, typename Get>
  typename L::ObjectType* GetObjectList(L&, Get);

  /** */
  template <typename L, typename Get>
  const typename L::ObjectType* GetObjectList(L&, Get) const;

  /** */
  template <typename D, typename From>
  void AddData(D*, From);

  /** */
  template <typename D, typename Set>
  typename T::Pointer FromData(D*, Set, const std::string& description = std::string());

  /** */
  template <typename D, typename Set>
  typename T::Pointer& FromData(typename T::Pointer&, D*, Set, const std::string& description = std::string());

  //
  // Protected attributes.
protected:
  /** */
  ParameterVector m_Parameters;

}; // End class InputImage Parameter

} // End namespace Wrapper

} // End namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWrapperParameterList.hxx"
#endif

#endif
