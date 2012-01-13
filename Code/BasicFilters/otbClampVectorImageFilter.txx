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

#ifndef __otbClampVectorImageFilter_txx
#define __otbClampVectorImageFilter_txx

#include "otbClampVectorImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
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
                       int threadId)
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
