/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSimpleRcsPanSharpeningFusionImageFilter_txx
#define __otbSimpleRcsPanSharpeningFusionImageFilter_txx

#include "otbSimpleRcsPanSharpeningFusionImageFilter.h"

namespace otb
{
template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
SimpleRcsPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::SimpleRcsPanSharpeningFusionImageFilter()
{
  // Fix number of required inputs
  this->SetNumberOfRequiredInputs(2);

  // Instantiate convolution filter
  m_ConvolutionFilter = ConvolutionFilterType::New();
  m_ConvolutionFilter->NormalizeFilterOn();

  // Set-up default parameters
  m_Radius.Fill(3);
  m_Filter.SetSize(7 * 7);
  m_Filter.Fill(1);

  // Instantiate fusion filter
  m_FusionFilter = FusionFilterType::New();
  m_FusionFilter->SetInput2(m_ConvolutionFilter->GetOutput());

  // Set-up progress reporting
  m_ProgressAccumulator = itk::ProgressAccumulator::New();
  m_ProgressAccumulator->SetMiniPipelineFilter(this);
  m_ProgressAccumulator->RegisterInternalFilter(m_ConvolutionFilter, 0.9);
  m_ProgressAccumulator->RegisterInternalFilter(m_FusionFilter, 0.1);
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
void
SimpleRcsPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::SetPanInput(const TPanImageType *image)
{
  // We have 2 inputs:  an image and a vector image

  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(1,
                                        const_cast<TPanImageType*>(image));
  this->Modified();
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
const TPanImageType *
SimpleRcsPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::GetPanInput(void) const
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }

  return static_cast<const TPanImageType *>
           (this->itk::ProcessObject::GetInput(1));
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
void
SimpleRcsPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::SetXsInput(const TXsImageType *image)
{
  // We have 2 inputs:  an image and a vector image

  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast<TXsImageType*>(image));
  this->Modified();
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
const TXsImageType *
SimpleRcsPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::GetXsInput(void) const
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TXsImageType *>
           (this->itk::ProcessObject::GetInput(0));
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
void
SimpleRcsPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::GenerateData()
{
  //Check if size is correct
  typename TPanImageType::SizeType       sizePan;
  typename TXsImageType::SizeType        sizeXs;
  sizePan = this->GetPanInput()->GetLargestPossibleRegion().GetSize();
  sizeXs = this->GetXsInput()->GetLargestPossibleRegion().GetSize();
  if ((sizePan[0] != sizeXs[0]) || (sizePan[1] != sizeXs[1]))
    {
    itkExceptionMacro(<< "SimpleRcsPanSharpeningFusionImageFilter: Wrong Pan/Xs size");
    }

  //Process the fusion
  m_ConvolutionFilter->SetInput(this->GetPanInput());
  m_ConvolutionFilter->SetRadius(this->m_Radius);
  m_ConvolutionFilter->SetFilter(this->m_Filter);

  m_FusionFilter->SetInput1(this->GetXsInput());
  m_FusionFilter->SetInput3(this->GetPanInput());

  // Wire composite filter
  m_FusionFilter->GraftOutput(this->GetOutput());
  m_FusionFilter->Update();
  this->GraftOutput(m_FusionFilter->GetOutput());
}

template <class TPanImageType, class TXsImageType, class TOutputImageType, class TInternalPrecision>
void
SimpleRcsPanSharpeningFusionImageFilter
<TPanImageType, TXsImageType, TOutputImageType, TInternalPrecision>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os
  << indent << "Radius:" << this->m_Radius
  << std::endl;
}

} // end namespace otb

#endif
