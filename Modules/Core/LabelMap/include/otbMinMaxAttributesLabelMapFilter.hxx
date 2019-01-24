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

#ifndef otbMinMaxAttributesLabelMapFilter_hxx
#define otbMinMaxAttributesLabelMapFilter_hxx

#include "otbMinMaxAttributesLabelMapFilter.h"
#include "itkNumericTraits.h"

namespace otb {

template <class TInputImage>
MinMaxAttributesLabelMapFilter<TInputImage>
::MinMaxAttributesLabelMapFilter()
{
  this->Superclass::SetNumberOfRequiredOutputs(3);
  this->itk::ProcessObject::SetNthOutput(1, this->MakeOutput(1).GetPointer());
  this->itk::ProcessObject::SetNthOutput(2, this->MakeOutput(2).GetPointer());
}

template<class TInputImage>
typename MinMaxAttributesLabelMapFilter<TInputImage>::DataObjectPointerType
MinMaxAttributesLabelMapFilter<TInputImage>
::MakeOutput(DataObjectPointerArraySizeType i)
{
  DataObjectPointerType ret;
  switch (i)
  {
    case 0:
      ret = InputImageType::New();
      break;
    case 1:
    case 2:
      ret = AttributesMapObjectType::New();
      break;
  }
  return ret;
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


  for(unsigned int i = 0; i < this->GetLabelMap()->GetNumberOfLabelObjects(); ++i)
    {
     LabelObjectType* labelObject =this->GetLabelMap()->GetNthLabelObject(i);
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
        ++it;
    }


}

}// end namespace otb
#endif
