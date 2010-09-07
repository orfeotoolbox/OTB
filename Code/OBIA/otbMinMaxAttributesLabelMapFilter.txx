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
#include "itkProgressReporter.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace otb {

template <class TInputImage, class TOutputImage>
MinMaxAttributesLabelMapFilter<TInputImage, TOutputImage>
::MinMaxAttributesLabelMapFilter()
{
  typename AttributesValueVectorObjectType::Pointer min = AttributesValueVectorObjectType::New();
  typename AttributesValueVectorObjectType::Pointer max = AttributesValueVectorObjectType::New();

  this->itk::ProcessObject::SetNthOutput(1, min.GetPointer());
  this->itk::ProcessObject::SetNthOutput(2, max.GetPointer());
}

template<class TInputImage, class TOutputImage>
typename MinMaxAttributesLabelMapFilter<TInputImage,TOutputImage>::AttributesValueVectorObjectType*
MinMaxAttributesLabelMapFilter<TInputImage,TOutputImage>
::GetMinimumOutput()
{
  return static_cast<AttributesValueVectorObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage, class TOutputImage>
const typename MinMaxAttributesLabelMapFilter<TInputImage,TOutputImage>::AttributesValueVectorObjectType*
MinMaxAttributesLabelMapFilter<TInputImage,TOutputImage>
::GetMinimumOutput() const
{
  return static_cast<const AttributesValueVectorObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage, class TOutputImage>
typename MinMaxAttributesLabelMapFilter<TInputImage,TOutputImage>::AttributesValueVectorObjectType*
MinMaxAttributesLabelMapFilter<TInputImage,TOutputImage>
::GetMaximumOutput()
{
  return static_cast<AttributesValueVectorObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage, class TOutputImage>
const typename MinMaxAttributesLabelMapFilter<TInputImage,TOutputImage>::AttributesValueVectorObjectType*
MinMaxAttributesLabelMapFilter<TInputImage,TOutputImage>
::GetMaximumOutput() const
{
  return static_cast<const AttributesValueVectorObjectType*>(this->itk::ProcessObject::GetOutput(2));
}

template<class TInputImage, class TOutputImage>
void
MinMaxAttributesLabelMapFilter<TInputImage, TOutputImage>
::GenerateData()
{
  LabelObjectContainerConstIterator it  = this->GetLabelMap()->GetLabelObjectContainer().begin();
  LabelObjectContainerConstIterator end = this->GetLabelMap()->GetLabelObjectContainer().end();

  unsigned int nbAttr = this->GetLabelMap()->GetLabelObject(0)->GetNumberOfAttributes();
  std::vector<std::string> attributes = this->GetLabelMap()->GetLabelObject(0)->GetAvailableAttributes();

  AttributesValueVectorType& minAttr = this->GetMinimumOutput()->Get();
  AttributesValueVectorType& maxAttr = this->GetMaximumOutput()->Get();

  minAttr.resize(nbAttr);
  maxAttr.resize(nbAttr);
  std::fill(minAttr.begin(), minAttr.end(), itk::NumericTraits<AttributesValueType>::max());
  std::fill(maxAttr.begin(), maxAttr.end(), itk::NumericTraits<AttributesValueType>::NonpositiveMin());

  for (; it != end; ++it)
    {
    // get the label object
    LabelObjectType * labelObject = it->second;

    for (unsigned int i = 0; i < attributes.size(); ++i)
      {
      AttributesValueType val = labelObject->GetAttribute(attributes[i].c_str());
      // Update min
      if (val < minAttr[i])
        minAttr[i] = val;
      //Update max
      if (val > maxAttr[i])
        maxAttr[i] = val;
      }
    }
}

}// end namespace otb
#endif
