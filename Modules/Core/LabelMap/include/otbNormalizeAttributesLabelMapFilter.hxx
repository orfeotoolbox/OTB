/*
 * Copyright (C) 1999-2011 Insight Software Consortium
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

#ifndef otbNormalizeAttributesLabelMapFilter_hxx
#define otbNormalizeAttributesLabelMapFilter_hxx

#include "otbNormalizeAttributesLabelMapFilter.h"

namespace otb
{

namespace Functor
{

template <class TLabelObject>
NormalizeAttributesLabelObjectFunctor<TLabelObject>::NormalizeAttributesLabelObjectFunctor()
{
}

/** The comparator (!=) */
template <class TLabelObject>
bool NormalizeAttributesLabelObjectFunctor<TLabelObject>::operator!=(const Self& other)
{
  bool resp = (m_Min != other.m_Min) || (m_Max != other.m_Max);
  return resp;
}

/** The comparator (==)*/
template <class TLabelObject>
bool NormalizeAttributesLabelObjectFunctor<TLabelObject>::operator==(const Self& self)
{
  return !(this != self);
}

template <class TLabelObject>
void NormalizeAttributesLabelObjectFunctor<TLabelObject>::operator()(LabelObjectType* lo) const
{
  const std::vector<std::string>& attr = lo->GetAvailableAttributes();

  std::vector<std::string>::const_iterator it;
  for (it = attr.begin(); it != attr.end(); ++it)
  {
    const AttributesValueType&                 value = lo->GetAttribute((*it).c_str());
    typename AttributesMapType::const_iterator minIt = m_Min.find(*it);
    typename AttributesMapType::const_iterator maxIt = m_Max.find(*it);
    if (minIt != m_Min.end() && maxIt != m_Max.end())
    {
      lo->SetAttribute((*it).c_str(), (value - minIt->second) / (maxIt->second - minIt->second));
    }
  }
}

} // end namespace Functor

template <class TImage>
void NormalizeAttributesLabelMapFilter<TImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb
#endif
