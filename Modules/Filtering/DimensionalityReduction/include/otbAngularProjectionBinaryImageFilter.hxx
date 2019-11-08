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

#ifndef otbAngularProjectionBinaryImageFilter_hxx
#define otbAngularProjectionBinaryImageFilter_hxx
#include "otbAngularProjectionBinaryImageFilter.h"

#include <vnl/vnl_math.h>

#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{

template <class TInputImage, class TOutputImage, class TPrecision>
AngularProjectionBinaryImageFilter<TInputImage, TOutputImage, TPrecision>::AngularProjectionBinaryImageFilter()
{
  this->SetNumberOfRequiredInputs(2);
}

template <class TInputImage, class TOutputImage, class TPrecision>
void AngularProjectionBinaryImageFilter<TInputImage, TOutputImage, TPrecision>::SetInput1(const InputImageType* inputPtr)
{
  this->SetNthInput(0, const_cast<InputImageType*>(inputPtr));
}

template <class TInputImage, class TOutputImage, class TPrecision>
void AngularProjectionBinaryImageFilter<TInputImage, TOutputImage, TPrecision>::SetInput2(const InputImageType* inputPtr)
{
  this->SetNthInput(1, const_cast<InputImageType*>(inputPtr));
}

template <class TInputImage, class TOutputImage, class TPrecision>
const TInputImage* AngularProjectionBinaryImageFilter<TInputImage, TOutputImage, TPrecision>::GetInput1() const
{
  if (this->GetNumberOfInputs() < 1)
  {
    return nullptr;
  }

  return static_cast<const TInputImage*>(this->itk::ProcessObject::GetInput(0));
}

template <class TInputImage, class TOutputImage, class TPrecision>
const TInputImage* AngularProjectionBinaryImageFilter<TInputImage, TOutputImage, TPrecision>::GetInput2() const
{
  if (this->GetNumberOfInputs() < 2)
  {
    return nullptr;
  }

  return static_cast<const TInputImage*>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage, class TOutputImage, class TPrecision>
void AngularProjectionBinaryImageFilter<TInputImage, TOutputImage, TPrecision>::SetAngleSet(std::vector<PrecisionType>& angle)
{
  m_AngleSet = angle;
  this->SetNumberOfRequiredOutputs(angle.size());
  for (unsigned int i = 0; i < this->GetNumberOfRequiredOutputs(); ++i)
  {
    this->SetNthOutput(i, OutputImageType::New());
  }
  this->Modified();
}

template <class TInputImage, class TOutputImage, class TPrecision>
void AngularProjectionBinaryImageFilter<TInputImage, TOutputImage, TPrecision>::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  for (unsigned int i = 0; i < this->GetNumberOfOutputs(); ++i)
  {
    this->GetOutput(i)->SetRegions(this->GetInput()->GetRequestedRegion());
  }
}

template <class TInputImage, class TOutputImage, class TPrecision>
void AngularProjectionBinaryImageFilter<TInputImage, TOutputImage, TPrecision>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                                                                                                     itk::ThreadIdType threadId)
{
  itk::ProgressReporter reporter(this, threadId, outputRegionForThread.GetNumberOfPixels());

  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  itk::ImageRegionConstIterator<InputImageType> iter1(this->GetInput1(), inputRegionForThread);
  iter1.GoToBegin();

  itk::ImageRegionConstIterator<InputImageType> iter2(this->GetInput2(), inputRegionForThread);
  iter2.GoToBegin();

  std::vector<itk::ImageRegionIterator<OutputImageType>> outIter(this->GetNumberOfOutputs());
  for (unsigned int i = 0; i < outIter.size(); ++i)
  {
    outIter[i] = itk::ImageRegionIterator<OutputImageType>(this->GetOutput(i), outputRegionForThread);
    outIter[i].GoToBegin();
  }

  while (!iter1.IsAtEnd() && !iter2.IsAtEnd())
  {
    for (unsigned int i = 0; i < outIter.size(); ++i)
    {
      outIter[i].Set(std::cos(m_AngleSet[i]) * iter1.Get() + std::sin(m_AngleSet[i]) * iter2.Get());
      ++outIter[i];
    }

    ++iter1;
    ++iter2;

    reporter.CompletedPixel();
  }
}

} // end of namespace otb

#endif
