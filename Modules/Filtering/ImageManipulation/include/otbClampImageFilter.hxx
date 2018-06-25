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


#ifndef otbClampImageFilter_hxx
#define otbClampImageFilter_hxx

#include "otbClampImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"
#include <limits>
#include "itkObjectFactory.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
ClampImageFilter<TInputImage, TOutputImage>
::ClampImageFilter()
{
  m_Lower = std::numeric_limits < OutputPixelValueType >::lowest();
  m_Upper = std::numeric_limits < OutputPixelValueType >::max();
}

template <class TInputImage, class TOutputImage>
void
ClampImageFilter<TInputImage, TOutputImage>
::SetLower(OutputPixelValueType val)
{
  if ( m_Lower != val )
    {
    m_Lower = val;
    this->GetFunctor().SetLowest( m_Lower );
    this->Modified();  
    }
}

template <class TInputImage, class TOutputImage>
void
ClampImageFilter<TInputImage, TOutputImage>
::SetUpper(OutputPixelValueType val)
{
  if ( m_Upper != val )
    {
    m_Upper = val;
    this->GetFunctor().SetHighest( m_Upper );
    this->Modified();
    }
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
void
ClampImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Lower: "
     << static_cast<typename itk::NumericTraits<OutputImagePixelType>::PrintType>(m_Lower)
     << std::endl;
  os << indent << "Upper: "
     << static_cast<typename itk::NumericTraits<OutputImagePixelType>::PrintType>(m_Upper)
     << std::endl;
}

/**
 * The values greater than or equal to the value are set to UpperValue
 */
template <class TInputImage, class TOutputImage>
void
ClampImageFilter<TInputImage, TOutputImage>
::ClampAbove(const OutputPixelValueType &thresh)
{
  if (m_Upper != thresh
      || m_Lower > std::numeric_limits < OutputPixelValueType >::lowest())
    {
    m_Lower = std::numeric_limits < OutputPixelValueType >::lowest();
    m_Upper = thresh;
    this->GetFunctor().SetLowest( m_Lower );
    this->GetFunctor().SetHighest( m_Upper );
    this->Modified();
    }
}

/**
 * The values less than or equal to the value are set to OutsideValue
 */
template <class TInputImage, class TOutputImage>
void
ClampImageFilter<TInputImage, TOutputImage>
::ClampBelow(const OutputPixelValueType &thresh)
{
  if (m_Lower != thresh || m_Upper < std::numeric_limits < OutputPixelValueType >::max())
    {
    m_Upper = std::numeric_limits < OutputPixelValueType >::max();
    m_Lower = thresh;
    this->GetFunctor().SetLowest( m_Lower );
    this->GetFunctor().SetHighest( m_Upper );
    this->Modified();
    }
}


/**
 * The values outside the range are set to OutsideValue
 */
template <class TInputImage, class TOutputImage>
void
ClampImageFilter<TInputImage, TOutputImage>
::ClampOutside(const OutputPixelValueType &lower, const OutputPixelValueType &upper)
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
    this->GetFunctor().SetLowest( m_Lower );
    this->GetFunctor().SetHighest( m_Upper );
    this->Modified();
    }
}

} // end namespace itk

#endif
