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
#ifndef _otbPerBandVectorImageFilter_txx
#define _otbPerBandVectorImageFilter_txx

#include "otbPerBandVectorImageFilter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFilter>
PerBandVectorImageFilter<TInputImage,TOutputImage,TFilter>
::PerBandVectorImageFilter()
{
  m_DecompositionFilter = DecompositionFilterType::New();
  m_ProcessingFilter = ProcessingFilterType::New();
  m_RecompositionFilter = RecompositionFilterType::New();
}
/**
 * Set the processing filter. This method is provided to allow the setting up of the filter.
 * \param filter The filter to use.
 */
template <class TInputImage, class TOutputImage, class TFilter>
void
PerBandVectorImageFilter<TInputImage,TOutputImage,TFilter>
::SetFilter(FilterType * filter)
{
  m_ProcessingFilter->SetFilter(filter);
}
/**
 * Get the processing filter. This method is provided to allow the setting of the filter.
 * \return The filter used.
 */
template <class TInputImage, class TOutputImage, class TFilter>
typename PerBandVectorImageFilter<TInputImage,TOutputImage,TFilter>
::FilterType *
PerBandVectorImageFilter<TInputImage,TOutputImage,TFilter>
::GetFilter()
{
  return m_ProcessingFilter->GetFilter();
}
/**
 * Set the index of the output of the filter from which to recompose the vector image.
 * Default is 0.
 * \param index The index of the output of the filter to use.
 */
template <class TInputImage, class TOutputImage, class TFilter>
void
PerBandVectorImageFilter<TInputImage,TOutputImage,TFilter>
::SetOutputIndex(unsigned int index)
{
  m_ProcessingFilter->SetOutputIndex(index);
}
/**
 * Get the index of the output of the filter from which the vector image is recomposed.
 * Default is 0.
 * \return The index of the output of the filter used.
 */
template <class TInputImage, class TOutputImage, class TFilter>
unsigned int
PerBandVectorImageFilter<TInputImage,TOutputImage,TFilter>
::GetOutputIndex()
{
  return m_ProcessingFilter->GetOutputIndex();
}
/**
 * Main computation method
 */
template <class TInputImage, class TOutputImage, class TFilter>
void
PerBandVectorImageFilter<TInputImage,TOutputImage,TFilter>
::GenerateData()
{
  m_DecompositionFilter->SetInput(this->GetInput());
  m_ProcessingFilter->SetInput(m_DecompositionFilter->GetOutput());
  m_RecompositionFilter->SetInput(m_ProcessingFilter->GetOutput());

  m_RecompositionFilter->GraftOutput(this->GetOutput());
  m_RecompositionFilter->Update();
  this->GraftOutput(m_RecompositionFilter->GetOutput());
}

/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage, class TFilter>
void
PerBandVectorImageFilter<TInputImage,TOutputImage,TFilter>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
