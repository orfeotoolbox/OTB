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
#ifndef __otbRationalQuotientResampleImageFilter_txx
#define __otbRationalQuotientResampleImageFilter_txx

#include "otbRationalQuotientResampleImageFilter.h"

namespace otb
{

  /**
   * Constructor
   */
  template <class TInputImage, class TOutputImage>
  RationalQuotientResampleImageFilter<TInputImage, TOutputImage>::RationalQuotientResampleImageFilter()
  {
    // Default rational quotient = 1:1
    unsigned int dimension = 0;
    for (dimension=0; dimension != ImageDimension; dimension++)
      {
  m_Numerators[dimension]=1;
  m_Denominators[dimension]=1;
      }

    // Internal filter
    m_ExpandFilter = ExpandFilterType::New();
    m_ShrinkFilter = ShrinkFilterType::New();

    m_ShrinkFilter->SetInput(m_ExpandFilter->GetOutput());

  }


  template <class TInputImage, class TOutputImage>
  void
  RationalQuotientResampleImageFilter<TInputImage,TOutputImage>
  ::GenerateData()
  {
    m_ExpandFilter->SetInput(this->GetInput());

    // Set expand factors as the numerator
    m_ExpandFilter->SetExpandFactors(m_Numerators);

    // Set shrink factors as the denominator
    m_ShrinkFilter->SetShrinkFactors(m_Denominators);

    m_ShrinkFilter->GraftOutput(this->GetOutput());

    m_ShrinkFilter->Update();
    this->GraftOutput(m_ShrinkFilter->GetOutput());
  }

  template <class TInputImage, class TOutputImage>
  void
  RationalQuotientResampleImageFilter<TInputImage, TOutputImage>::
  PrintSelf( std::ostream& os, itk::Indent indent ) const
  {
    Superclass::PrintSelf(os, indent);
    os << indent << "Rational quotient factor (numerator:denominator) : ";
    unsigned int dimension = 0;
    for(dimension=0; dimension != ImageDimension; dimension++)
      {
  os << m_Numerators[dimension] << ":" << m_Denominators[dimension] << " ";
      }
    os << std::endl;
  }

  template <class TInputImage, class TOutputImage>
  void
  RationalQuotientResampleImageFilter<TInputImage, TOutputImage>::
  SetDenominatorFactors( const unsigned int denominator)
  {
    unsigned int dimension = 0;
    for (dimension=0; dimension!=ImageDimension; dimension++)
      {
  if (denominator < 1)
    {
      m_Denominators[dimension]=1;
    }
  else
    {
      m_Denominators[dimension] = denominator;
    }
      }
  }

  template <class TInputImage, class TOutputImage>
  void
  RationalQuotientResampleImageFilter<TInputImage, TOutputImage>::
  SetDenominatorFactors( const unsigned int* denominators)
  {
    unsigned int dimension = 0;
    for (dimension=0; dimension!=ImageDimension; dimension++)
      {
  if (denominators[dimension] < 1)
    {
      m_Denominators[dimension] = 1;
    }
  else
    {
      m_Denominators[dimension] = denominators[dimension];
    }
      }
  }

  template <class TInputImage, class TOutputImage>
  void
  RationalQuotientResampleImageFilter<TInputImage, TOutputImage>::
  SetDenominatorFactors( const unsigned int i, const unsigned int denominator)
  {
    if (denominator < 1)
      {
  m_Denominators[i]=1;
      }
    else
      {
  m_Denominators[i] = denominator;
      }
  }

  template <class TInputImage, class TOutputImage>
  void
  RationalQuotientResampleImageFilter<TInputImage, TOutputImage>::
  SetNumeratorFactors( const unsigned int numerator)
  {
    unsigned int dimension = 0;
    for (dimension=0; dimension!=ImageDimension; dimension++)
      {
  if (numerator < 1)
    {
      m_Numerators[dimension]=1;
    }
  else
    {
      m_Numerators[dimension] = numerator;
    }
      }
  }

  template <class TInputImage, class TOutputImage>
  void
  RationalQuotientResampleImageFilter<TInputImage, TOutputImage>::
  SetNumeratorFactors( const unsigned int* numerators)
  {
    unsigned int dimension = 0;
    for (dimension=0; dimension!=ImageDimension; dimension++)
      {
  if (numerators[dimension] < 1)
    {
      m_Numerators[dimension] = 1;
    }
  else
    {
      m_Numerators[dimension] = numerators[dimension];
    }
      }
  }

  template <class TInputImage, class TOutputImage>
  void
  RationalQuotientResampleImageFilter<TInputImage, TOutputImage>::
  SetNumeratorFactors( const unsigned int i, const unsigned int numerator)
  {
    if (numerator < 1)
      {
  m_Numerators[i]=1;
      }
    else
      {
  m_Numerators[i] = numerator;
      }
  }

} // End namespace otb

#endif

