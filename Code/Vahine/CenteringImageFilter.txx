/**
 * CenteringImageFilter.txx
 *
 * Vahine Framework
 * Copyright (C) 2008, 2009 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */
#ifndef __VAHINECENTERING_TXX
#define __VAHINECENTERING_TXX

#include "CenteringImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkStatisticsImageFilter.h"
#include "itkShiftScaleImageFilter.h"
#include "itkCommand.h"
#include "itkProgressAccumulator.h"

namespace itk {

template <class TInputImage, class TOutputImage>
CenteringImageFilter<TInputImage, TOutputImage>::CenteringImageFilter() {
  m_StatisticsFilter = 0;
  m_StatisticsFilter = StatisticsImageFilter<TInputImage>::New();
  m_ShiftScaleFilter = ShiftScaleImageFilter<TInputImage,TOutputImage>::New();
}

template <class TInputImage, class TOutputImage>
void CenteringImageFilter<TInputImage, TOutputImage>::GenerateInputRequestedRegion() {
  Superclass::GenerateInputRequestedRegion();
  if ( this->GetInput() ) {
    InputImagePointer image = const_cast< typename Superclass::InputImageType * >( this->GetInput() );
    image->SetRequestedRegionToLargestPossibleRegion();
    }
}

template <class TInputImage, class TOutputImage>
void CenteringImageFilter<TInputImage, TOutputImage>::Modified() const {
  Superclass::Modified();
  m_StatisticsFilter->Modified();
  m_ShiftScaleFilter->Modified();
}

template <class TInputImage, class TOutputImage>
void CenteringImageFilter<TInputImage, TOutputImage>::GenerateData() {

  /*
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  progress->RegisterInternalFilter(m_StatisticsFilter,.5f);
  progress->RegisterInternalFilter(m_ShiftScaleFilter,.5f);
   */

  // Gather statistics
  m_StatisticsFilter->SetInput(this->GetInput() );
  //m_StatisticsFilter->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
  m_StatisticsFilter->Update();

  // Set the parameters for Shift
  vahineDebug("CenteringImageFilter : mean = "<< m_StatisticsFilter->GetMean() );
  m_ShiftScaleFilter->SetShift(-m_StatisticsFilter->GetMean() );
  m_ShiftScaleFilter->SetInput(this->GetInput() );

  m_ShiftScaleFilter->GraftOutput(this->GetOutput() );
  //m_ShiftScaleFilter->GetOutput()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
  m_ShiftScaleFilter->Update();

  //std::cout<<"shiftscale = "<<m_ShiftScaleFilter->GetOutput()<<std::endl;

  // Graft the mini pipeline output to this filters output
  this->GraftOutput(m_ShiftScaleFilter->GetOutput() );
}

} // end namespace itk

#endif