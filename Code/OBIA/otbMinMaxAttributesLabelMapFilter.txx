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
#ifndef __otbMinMaxAttributesLabelMapFilter_txx
#define __otbMinMaxAttributesLabelMapFilter_txx

#include "otbMinMaxAttributesLabelMapFilter.h"
#include "itkNumericTraits.h"

namespace otb {

template <class TInputImage>
MinMaxAttributesLabelMapFilter<TInputImage>
::MinMaxAttributesLabelMapFilter()
{
  typename AttributesMapObjectType::Pointer min = AttributesMapObjectType::New();
  typename AttributesMapObjectType::Pointer max = AttributesMapObjectType::New();

  this->itk::ProcessObject::SetNthOutput(1, min.GetPointer());
  this->itk::ProcessObject::SetNthOutput(2, max.GetPointer());
}

template<class TInputImage>
typename MinMaxAttributesLabelMapFilter<TInputImage>::AttributesMapObjectType*
MinMaxAttributesLabelMapFilter<TInputImage>
::GetMinimumOutput()
{
  return static_cast<AttributesMapObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
const typename MinMaxAttributesLabelMapFilter<TInputImage>::AttributesMapObjectType*
MinMaxAttributesLabelMapFilter<TInputImage>
::GetMinimumOutput() const
{
  return static_cast<const AttributesMapObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
typename MinMaxAttributesLabelMapFilter<TInputImage>::AttributesMapObjectType*
MinMaxAttributesLabelMapFilter<TInputImage>
::GetMaximumOutput()
{
  return static_cast<AttributesMapObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage>
const typename MinMaxAttributesLabelMapFilter<TInputImage>::AttributesMapObjectType*
MinMaxAttributesLabelMapFilter<TInputImage>
::GetMaximumOutput() const
{
  return static_cast<const AttributesMapObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage>
void
MinMaxAttributesLabelMapFilter<TInputImage>
::GenerateData()
{
  unsigned int nbAttr = this->GetLabelMap()->GetLabelObject(0)->GetNumberOfAttributes();
  std::vector<std::string> attributes = this->GetLabelMap()->GetLabelObject(0)->GetAvailableAttributes();

  AttributesMapType& minAttr = this->GetMinimumOutput()->Get();
  AttributesMapType& maxAttr = this->GetMaximumOutput()->Get();

  // create an entry in the output maps for each attribute
  std::vector<std::string>::const_iterator valueIt;
  for (valueIt = attributes.begin(); valueIt != attributes.end(); ++valueIt)
    {
    minAttr[*valueIt] = itk::NumericTraits<AttributesValueType>::max();
    maxAttr[*valueIt] = itk::NumericTraits<AttributesValueType>::NonpositiveMin();
    }

  LabelObjectContainerConstIterator it  = this->GetLabelMap()->GetLabelObjectContainer().begin();
  LabelObjectContainerConstIterator end = this->GetLabelMap()->GetLabelObjectContainer().end();
  for (; it != end; ++it)
    {
    // get the label object
    LabelObjectType * labelObject = it->second;

    std::vector<std::string>::const_iterator it;
    for (it = attributes.begin(); it != attributes.end(); ++it)
      {
      AttributesValueType val = labelObject->GetAttribute((*it).c_str());
      // Update min
      if (val < minAttr[*it])
        minAttr[*it] = val;
      //Update max
      if (val > maxAttr[*it])
        maxAttr[*it] = val;
      }
    }
}

}// end namespace otb
#endif
