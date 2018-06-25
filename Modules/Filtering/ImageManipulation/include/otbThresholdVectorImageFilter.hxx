/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbThresholdVectorImageFilter_hxx
#define otbThresholdVectorImageFilter_hxx

#include "otbThresholdVectorImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"
#include "itkObjectFactory.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
ThresholdVectorImageFilter<TInputImage, TOutputImage>
::ThresholdVectorImageFilter()
{
  m_OutsideValue = itk::NumericTraits<OutputImageInternalPixelType>::Zero;
  m_Lower = itk::NumericTraits<InputImageInternalPixelType>::NonpositiveMin();
  m_Upper = itk::NumericTraits<InputImageInternalPixelType>::max();
}


/**
 *
 */
template <class TInputImage, class TOutputImage>
void
ThresholdVectorImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "OutsideValue: "
     << static_cast<typename itk::NumericTraits<InputImageInternalPixelType>::PrintType>(m_OutsideValue)
     << std::endl;
  os << indent << "Lower: "
     << static_cast<typename itk::NumericTraits<InputImageInternalPixelType>::PrintType>(m_Lower)
     << std::endl;
  os << indent << "Upper: "
     << static_cast<typename itk::NumericTraits<InputImageInternalPixelType>::PrintType>(m_Upper)
     << std::endl;
}

/**
 * The values greater than or equal to the value are set to OutsideValue
 */
template <class TInputImage, class TOutputImage>
void
ThresholdVectorImageFilter<TInputImage, TOutputImage>
::ThresholdAbove(const InputImageInternalPixelType &thresh)
{
  if (m_Upper != thresh
      || m_Lower > itk::NumericTraits<InputImageInternalPixelType>::NonpositiveMin())
    {
    m_Lower = itk::NumericTraits<InputImageInternalPixelType>::NonpositiveMin();
    m_Upper = thresh;
    this->Modified();
    }
}

/**
 * The values less than or equal to the value are set to OutsideValue
 */
template <class TInputImage, class TOutputImage>
void
ThresholdVectorImageFilter<TInputImage, TOutputImage>
::ThresholdBelow(const InputImageInternalPixelType &thresh)
{
  if (m_Lower != thresh || m_Upper < itk::NumericTraits<InputImageInternalPixelType>::max())
    {
    m_Lower = thresh;
    m_Upper = itk::NumericTraits<InputImageInternalPixelType>::max();
    this->Modified();
    }
}


/**
 * The values outside the range are set to OutsideValue
 */
template <class TInputImage, class TOutputImage>
void
ThresholdVectorImageFilter<TInputImage, TOutputImage>
::ThresholdOutside(const InputImageInternalPixelType &lower, const InputImageInternalPixelType &upper)
{
  if (lower > upper)
    {
    itkExceptionMacro(<<"Lower threshold cannot be greater than upper threshold.");
    return;
    }

  if (m_Lower != lower || m_Upper != upper)
    {
    m_Lower = lower;
    m_Upper = upper;
    this->Modified();
    }
}


/**
 *
 */
template <class TInputImage, class TOutputImage>
void
ThresholdVectorImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       itk::ThreadIdType threadId)
{
  itkDebugMacro(<<"Actually executing");

  // Get the input and output pointers
  InputImagePointer  inputPtr  = this->GetInput();
  OutputImagePointer outputPtr = this->GetOutput(0);

  // Define/declare an iterator that will walk the output region for this
  // thread.
  typedef itk::ImageRegionConstIterator<TInputImage> InputIterator;
  typedef itk::ImageRegionIterator<TOutputImage>      OutputIterator;

  InputIterator  inIt(inputPtr, outputRegionForThread);
  OutputIterator outIt(outputPtr, outputRegionForThread);

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // walk the regions, threshold each pixel
  while( !outIt.IsAtEnd() && !inIt.IsAtEnd()  )
    {
    const InputImagePixelType inPix = inIt.Get();
    unsigned int l_size = inPix.Size();
    OutputImagePixelType outPix;
    outPix.SetSize(l_size);
    for(unsigned int i=0; i<l_size; i++)
      {
      const InputImageInternalPixelType value = inPix[i];

      if (m_Lower <= value && value <= m_Upper)
        {
        // pixel passes to output unchanged and is replaced by m_OutsideValue in
        // the inverse output image
        outPix[i] = static_cast<OutputImageInternalPixelType>(value);
        }
      else
        {
        outPix[i] = static_cast<OutputImageInternalPixelType>(m_OutsideValue);
        }
      }

    outIt.Set( outPix );

    ++inIt;
    ++outIt;
    progress.CompletedPixel();
    }

}

} // end namespace itk

#endif
