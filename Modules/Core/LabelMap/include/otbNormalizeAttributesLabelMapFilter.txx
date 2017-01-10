/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbNormalizeAttributesLabelMapFilter_txx
#define otbNormalizeAttributesLabelMapFilter_txx

#include "otbNormalizeAttributesLabelMapFilter.h"

namespace otb {

namespace Functor {

template <class TLabelObject>
NormalizeAttributesLabelObjectFunctor<TLabelObject>
::NormalizeAttributesLabelObjectFunctor()
{}

/** The comparator (!=) */
template <class TLabelObject>
bool
NormalizeAttributesLabelObjectFunctor<TLabelObject>
::operator != (const Self &other)
  {
  bool resp = (m_Min != other.m_Min) || (m_Max != other.m_Max);
  return resp;
  }

/** The comparator (==)*/
template <class TLabelObject>
bool
NormalizeAttributesLabelObjectFunctor<TLabelObject>
::operator == (const Self &self)
  {
  return !(this != self);
  }

template <class TLabelObject>
void
NormalizeAttributesLabelObjectFunctor<TLabelObject>
::operator() (LabelObjectType * lo) const
{
  const std::vector<std::string>& attr = lo->GetAvailableAttributes();

  std::vector<std::string>::const_iterator it;
  for (it = attr.begin(); it != attr.end(); ++it)
    {
    const AttributesValueType& value = lo->GetAttribute( (*it).c_str() );
    typename AttributesMapType::const_iterator minIt = m_Min.find(*it);
    typename AttributesMapType::const_iterator maxIt = m_Max.find(*it);
    if (minIt != m_Min.end() && maxIt != m_Max.end())
      {
      lo->SetAttribute( (*it).c_str(), (value - minIt->second)/(maxIt->second - minIt->second) );
      }
    }
}

} // end namespace Functor

template<class TImage>
void
NormalizeAttributesLabelMapFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb
#endif
