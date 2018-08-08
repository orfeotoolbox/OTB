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

#ifndef otbSOMImageClassificationFilter_hxx
#define otbSOMImageClassificationFilter_hxx

#include "otbSOMImageClassificationFilter.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TSOMMap, class TMaskImage>
SOMImageClassificationFilter<TInputImage, TOutputImage, TSOMMap, TMaskImage>
::SOMImageClassificationFilter()
{
  OTB_DISABLE_DYNAMIC_MT;
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredInputs(1);
  m_DefaultLabel = itk::NumericTraits<LabelType>::ZeroValue();
}

template <class TInputImage, class TOutputImage, class TSOMMap, class TMaskImage>
void
SOMImageClassificationFilter<TInputImage, TOutputImage, TSOMMap, TMaskImage>
::SetInputMask(const MaskImageType * mask)
{
  this->itk::ProcessObject::SetNthInput(1, const_cast<MaskImageType *>(mask));
}

template <class TInputImage, class TOutputImage, class TSOMMap, class TMaskImage>
const typename SOMImageClassificationFilter<TInputImage, TOutputImage, TSOMMap, TMaskImage>
::MaskImageType *
SOMImageClassificationFilter<TInputImage, TOutputImage, TSOMMap, TMaskImage>
::GetInputMask()
{
  if (this->GetNumberOfInputs() < 2)
    {
    return nullptr;
    }
  return static_cast<const MaskImageType *>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage, class TOutputImage, class TSOMMap, class TMaskImage>
void
SOMImageClassificationFilter<TInputImage, TOutputImage, TSOMMap, TMaskImage>
::BeforeThreadedGenerateData()
{
  if (!m_Map)
    {
    itkGenericExceptionMacro(<< "No model for classification");
    }
}

template <class TInputImage, class TOutputImage, class TSOMMap, class TMaskImage>
void
SOMImageClassificationFilter<TInputImage, TOutputImage, TSOMMap, TMaskImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType itkNotUsed(threadId))
{
  InputImageConstPointerType inputPtr     = this->GetInput();
  MaskImageConstPointerType  inputMaskPtr  = this->GetInputMask();
  OutputImagePointerType     outputPtr    = this->GetOutput();

  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef itk::ImageRegionConstIterator<MaskImageType>  MaskIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType>     OutputIteratorType;

  ListSamplePointerType listSample = ListSampleType::New();
  listSample->SetMeasurementVectorSize(inputPtr->GetNumberOfComponentsPerPixel());

  InputIteratorType inIt(inputPtr, outputRegionForThread);

  MaskIteratorType maskIt;
  if (inputMaskPtr)
    {
    maskIt = MaskIteratorType(inputMaskPtr, outputRegionForThread);
    maskIt.GoToBegin();
    }
  unsigned int maxDimension = m_Map->GetNumberOfComponentsPerPixel();
  unsigned int sampleSize = std::min(inputPtr->GetNumberOfComponentsPerPixel(),
                                     maxDimension);
  bool validPoint = true;

  for (inIt.GoToBegin(); !inIt.IsAtEnd(); ++inIt)
    {
    if (inputMaskPtr)
      {
      validPoint = maskIt.Get() > 0;
      ++maskIt;
      }
    if (validPoint)
      {
      SampleType sample;
      sample.SetSize(sampleSize);
      sample.Fill(itk::NumericTraits<ValueType>::ZeroValue());
      for (unsigned int i = 0; i < sampleSize; ++i)
        {
        sample[i] = inIt.Get()[i];
        }
      listSample->PushBack(sample);
      }
    }
  ClassifierPointerType classifier = ClassifierType::New();
  classifier->SetMap(m_Map);
  classifier->SetSample(listSample);
  classifier->Update();

  typename ClassifierType::OutputType::Pointer       membershipSample = classifier->GetOutput();
  typename ClassifierType::OutputType::ConstIterator sampleIter = membershipSample->Begin();
  typename ClassifierType::OutputType::ConstIterator sampleLast = membershipSample->End();

  OutputIteratorType outIt(outputPtr, outputRegionForThread);

  outIt.GoToBegin();

  while (!outIt.IsAtEnd() && (sampleIter != sampleLast))
    {
    outIt.Set(m_DefaultLabel);
    ++outIt;
    }

  outIt.GoToBegin();

  if (inputMaskPtr)
    {
    maskIt.GoToBegin();
    }
  validPoint = true;

  while (!outIt.IsAtEnd() && (sampleIter != sampleLast))
    {
    if (inputMaskPtr)
      {
      validPoint = maskIt.Get() > 0;
      ++maskIt;
      }
    if (validPoint)
      {
      outIt.Set(sampleIter.GetClassLabel());
      ++sampleIter;
      }
    ++outIt;
    }
}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage, class TSOMMap, class TMaskImage>
void
SOMImageClassificationFilter<TInputImage, TOutputImage, TSOMMap, TMaskImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
