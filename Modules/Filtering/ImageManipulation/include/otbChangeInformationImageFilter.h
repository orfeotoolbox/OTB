/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbChangeInformationImageFilter_h
#define otbChangeInformationImageFilter_h

#include "itkChangeInformationImageFilter.h"
#include "otbMetaDataKey.h"

namespace otb
{

/**
 * \class ChangeInformationImageFilter
 * \brief Filter to modify image metadata
 *
 * The base class is itk::ChangeInformationImageFilter that allows
 * modifying origin, spacing, direction and buffered region. This deriving
 * filter adds the support of MetaDataDictionary.
 *
 * \ingroup OTBImageManipulation
 */
template< typename TInputImage >
class ChangeInformationImageFilter:
  public itk::ChangeInformationImageFilter< TInputImage >
{
public:
  /** Standard class typedefs. */
  typedef ChangeInformationImageFilter                   Self;
  typedef itk::ChangeInformationImageFilter<TInputImage> Superclass;
  typedef itk::SmartPointer<Self>                        Pointer;
  typedef itk::SmartPointer<const Self>                  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(ChangeInformationImageFilter, itk::ChangeInformationImageFilter);

  /** Set key names to change */
  void SetChangeMetaData(const char *keyname, bool flag);

  /** Ask if a metadata will be changed */
  bool GetChangeMetaData(const char *keyname);

  /** Set output values for metadata, passing a NULL value will remove the
   *  metadata from output. If not set for a key name in the change list,
   *  the metadata will also be set.
   */
  template<typename T>
  void SetOutputMetaData(const char *keyname, const T * value);

protected:
  ChangeInformationImageFilter() {}
  ~ChangeInformationImageFilter() override {}

  /** Apply changes to the output image metadata. */
  void GenerateOutputInformation() override;

private:
  ChangeInformationImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** List of metadata keys to change */
  std::set<std::string> m_ChangedKeys;

};

} // End of namespace OTB

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbChangeInformationImageFilter.hxx"
#endif

#endif
