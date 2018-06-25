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


#ifndef otbClampVectorImageFilter_hxx
#define otbClampVectorImageFilter_hxx

#include "otbClampVectorImageFilter.h"
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
ClampVectorImageFilter<TInputImage, TOutputImage>
::ClampVectorImageFilter()
{
  m_Lower = itk::NumericTraits<OutputImageInternalPixelType>::NonpositiveMin();
  m_Upper = itk::NumericTraits<OutputImageInternalPixelType>::max();

  m_DLower = static_cast<double>(m_Lower);
  m_DUpper = static_cast<double>(m_Upper);
}


/**
 *
 */
template <class TInputImage, class TOutputImage>
void
ClampVectorImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Lower: "
     << static_cast<typename itk::NumericTraits<OutputImageInternalPixelType>::PrintType>(m_Lower)
     << std::endl;
  os << indent << "Upper: "
     << static_cast<typename itk::NumericTraits<OutputImageInternalPixelType>::PrintType>(m_Upper)
     << std::endl;
}

/**
 * The values greater than or equal to the value are set to UpperValue
 */
template <class TInputImage, class TOutputImage>
void
ClampVectorImageFilter<TInputImage, TOutputImage>
::ClampAbove(const OutputImageInternalPixelType &thresh)
{
  if (m_Upper != thresh
      || m_Lower > itk::NumericTraits<OutputImageInternalPixelType>::NonpositiveMin())
    {
    m_Lower = itk::NumericTraits<OutputImageInternalPixelType>::NonpositiveMin();
    m_Upper = thresh;
    m_DUpper = static_cast<double>(m_Upper);
    this->Modified();
    }
}

/**
 * The values less than or equal to the value are set to OutsideValue
 */
template <class TInputImage, class TOutputImage>
void
ClampVectorImageFilter<TInputImage, TOutputImage>
::ClampBelow(const OutputImageInternalPixelType &thresh)
{
  if (m_Lower != thresh || m_Upper < itk::NumericTraits<OutputImageInternalPixelType>::max())
    {
    m_Lower = thresh;
    m_DLower = m_Lower;
    m_Upper = itk::NumericTraits<OutputImageInternalPixelType>::max();
    this->Modified();
    }
}


/**
 * The values outside the range are set to OutsideValue
 */
template <class TInputImage, class TOutputImage>
void
ClampVectorImageFilter<TInputImage, TOutputImage>
::ClampOutside(const OutputImageInternalPixelType &lower, const OutputImageInternalPixelType &upper)
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
    m_DLower = m_Lower;
    m_DUpper = m_Upper;
    this->Modified();
    }
}


/**
 *
 */
template <class TInputImage, class TOutputImage>
void
ClampVectorImageFilter<TInputImage, TOutputImage>
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
      // Cast the value of the pixel to double in order to compare
      // with the double version of the upper and the lower bounds of
      // output image
      const double value = static_cast<double>(inPix[i]);

      if ( m_DLower <= value && value <= m_DUpper )
        {
        // pixel passes to output unchanged and is replaced by m_OutsideValue in
        // the inverse output image
        outPix[i] = static_cast<OutputImageInternalPixelType>(value);
        }
      else if ( value < m_DLower )
        {
        outPix[i] = m_Lower;
        }
      else if ( value > m_DUpper )
        {
        outPix[i] = m_Upper;
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
