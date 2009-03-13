/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryGrindPeakImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2006/12/15 21:41:24 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryGrindPeakImageFilter_txx
#define __itkBinaryGrindPeakImageFilter_txx

#include "itkBinaryGrindPeakImageFilter.h"
#include "itkBinaryReconstructionByDilationImageFilter.h"
#include "itkImageRegionExclusionIteratorWithIndex.h"
#include "itkProgressAccumulator.h"

namespace itk {

template <class TInputImage>
BinaryGrindPeakImageFilter<TInputImage>
::BinaryGrindPeakImageFilter()
{
  m_FullyConnected = false;
  m_ForegroundValue = NumericTraits<InputImagePixelType>::max();
  m_BackgroundValue = NumericTraits<InputImagePixelType>::Zero;
}

template <class TInputImage>
void 
BinaryGrindPeakImageFilter<TInputImage>
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


template <class TInputImage>
void 
BinaryGrindPeakImageFilter<TInputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage>
void
BinaryGrindPeakImageFilter<TInputImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();
  
  // construct a marker image to manipulate using reconstruction by
  // erosion. the marker image will have the same pixel values as the
  // input image on the boundary of the image and will have the
  // maximum pixel value from the input image for all the pixels in
  // the interior
  //

  // allocate a marker image
  InputImagePointer markerPtr = InputImageType::New();
  markerPtr->SetRegions( this->GetInput()->GetRequestedRegion() );
  markerPtr->CopyInformation( this->GetInput() );
  markerPtr->Allocate();

  // fill the marker image with the maximum value from the input
  markerPtr->FillBuffer( m_BackgroundValue );

  // set the border of the marker to the background value
  //
  ImageRegionExclusionIteratorWithIndex<TInputImage>
    markerBoundaryIt( markerPtr, this->GetInput()->GetRequestedRegion() );
  markerBoundaryIt.SetExclusionRegionToInsetRegion();

  // copy the boundary pixels
  markerBoundaryIt.GoToBegin();
  while ( !markerBoundaryIt.IsAtEnd() )
    {
    markerBoundaryIt.Set( m_ForegroundValue );
    ++markerBoundaryIt;
    }
  
  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // Delegate to a geodesic erosion filter.
  //
  //
  typename BinaryReconstructionByDilationImageFilter<TInputImage>::Pointer
    dilate
    = BinaryReconstructionByDilationImageFilter<TInputImage>::New();
  dilate->SetMarkerImage( markerPtr );
  dilate->SetForegroundValue( m_ForegroundValue );
  dilate->SetBackgroundValue( m_BackgroundValue );
  dilate->SetMaskImage( this->GetInput() );
  dilate->SetFullyConnected( m_FullyConnected );
  dilate->SetNumberOfThreads( this->GetNumberOfThreads() );
  progress->RegisterInternalFilter(dilate,1.0f);

  dilate->GraftOutput( this->GetOutput() );
  dilate->Update();
  this->GraftOutput( dilate->GetOutput() );
}


template<class TInputImage>
void
BinaryGrindPeakImageFilter<TInputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "ForegroundValue: " << static_cast<typename NumericTraits<InputImagePixelType>::PrintType>(m_ForegroundValue) << std::endl;
  os << indent << "BackgroundValue: " << static_cast<typename NumericTraits<InputImagePixelType>::PrintType>(m_BackgroundValue) << std::endl;
  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
}
  
}// end namespace itk
#endif
