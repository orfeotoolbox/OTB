/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAttributeRelabelImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2006/08/01 19:16:18 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkAttributeRelabelImageFilter_txx
#define __itkAttributeRelabelImageFilter_txx

#include "itkAttributeRelabelImageFilter.h"
#include "itkProgressAccumulator.h"


namespace itk {

template<class TInputImage, class TLabelObject, class TLabelObjectValuator, class TAttributeAccessor>
AttributeRelabelImageFilter<TInputImage, TLabelObject, TLabelObjectValuator, TAttributeAccessor>
::AttributeRelabelImageFilter()
{
  m_BackgroundValue = NumericTraits<OutputImagePixelType>::NonpositiveMin();
  m_ReverseOrdering = false;
}

template<class TInputImage, class TLabelObject, class TLabelObjectValuator, class TAttributeAccessor>
void 
AttributeRelabelImageFilter<TInputImage, TLabelObject, TLabelObjectValuator, TAttributeAccessor>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  if( input )
    {
    input->SetRequestedRegion( input->GetLargestPossibleRegion() );
    }
}


template<class TInputImage, class TLabelObject, class TLabelObjectValuator, class TAttributeAccessor>
void 
AttributeRelabelImageFilter<TInputImage, TLabelObject, TLabelObjectValuator, TAttributeAccessor>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TLabelObject, class TLabelObjectValuator, class TAttributeAccessor>
void
AttributeRelabelImageFilter<TInputImage, TLabelObject, TLabelObjectValuator, TAttributeAccessor>
::GenerateData()
{
  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // Allocate the output
  this->AllocateOutputs();
  
  typename LabelizerType::Pointer labelizer = LabelizerType::New();
  labelizer->SetInput( this->GetInput() );
  labelizer->SetBackgroundValue( m_BackgroundValue );
  labelizer->SetNumberOfThreads( this->GetNumberOfThreads() );
  progress->RegisterInternalFilter(labelizer, .3f);
  
  typename LabelObjectValuatorType::Pointer valuator = LabelObjectValuatorType::New();
  valuator->SetInput( labelizer->GetOutput() );
  valuator->SetNumberOfThreads( this->GetNumberOfThreads() );
  progress->RegisterInternalFilter(valuator, .3f);
  
  typename RelabelType::Pointer opening = RelabelType::New();
  opening->SetInput( valuator->GetOutput() );
  opening->SetReverseOrdering( m_ReverseOrdering );
  opening->SetNumberOfThreads( this->GetNumberOfThreads() );
  progress->RegisterInternalFilter(opening, .2f);
  
  typename BinarizerType::Pointer binarizer = BinarizerType::New();
  binarizer->SetInput( opening->GetOutput() );
  binarizer->SetNumberOfThreads( this->GetNumberOfThreads() );
  progress->RegisterInternalFilter(binarizer, .2f);

  this->CustomizeInternalFilters( labelizer, valuator, opening, binarizer );

  binarizer->GraftOutput( this->GetOutput() );
  binarizer->Update();
  this->GraftOutput( binarizer->GetOutput() );
}


template<class TInputImage, class TLabelObject, class TLabelObjectValuator, class TAttributeAccessor>
void
AttributeRelabelImageFilter<TInputImage, TLabelObject, TLabelObjectValuator, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "ReverseOrdering: "  << m_ReverseOrdering << std::endl;
  os << indent << "BackgroundValue: "  << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_BackgroundValue) << std::endl;
}
  
}// end namespace itk
#endif
