/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryFillholeImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2006/12/15 21:41:24 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryFillholeImageFilter_txx
#define __itkBinaryFillholeImageFilter_txx

#include "itkBinaryFillholeImageFilter.h"
#include "itkBinaryReconstructionByErosionImageFilter.h"
#include "itkImageRegionExclusionIteratorWithIndex.h"
#include "itkProgressAccumulator.h"

namespace itk {

template <class TInputImage>
BinaryFillholeImageFilter<TInputImage>
::BinaryFillholeImageFilter()
{
  m_FullyConnected = false;
  m_ForegroundValue = NumericTraits<InputImagePixelType>::max();
}

template <class TInputImage>
void 
BinaryFillholeImageFilter<TInputImage>
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
BinaryFillholeImageFilter<TInputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage>
void
BinaryFillholeImageFilter<TInputImage>
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

  // let choose a background value. Background value should not be given by user
  // because closing is extensive so no background pixels will be added
  // it is just needed for internal erosion filter and constant padder
  InputImagePixelType backgroundValue = NumericTraits<InputImagePixelType>::Zero;
  if ( m_ForegroundValue == backgroundValue )
    {
    // current background value is already used for foreground value
    // choose another one
    backgroundValue = NumericTraits<InputImagePixelType>::max();
    }

  // allocate a marker image
  InputImagePointer markerPtr = InputImageType::New();
  markerPtr->SetRegions( this->GetInput()->GetRequestedRegion() );
  markerPtr->CopyInformation( this->GetInput() );
  markerPtr->Allocate();

  // fill the marker image with the maximum value from the input
  markerPtr->FillBuffer( m_ForegroundValue );

  // set the border of the marker to the background value
  //
  ImageRegionExclusionIteratorWithIndex<TInputImage>
    markerBoundaryIt( markerPtr, this->GetInput()->GetRequestedRegion() );
  markerBoundaryIt.SetExclusionRegionToInsetRegion();

  // copy the boundary pixels
  markerBoundaryIt.GoToBegin();
  while ( !markerBoundaryIt.IsAtEnd() )
    {
    markerBoundaryIt.Set( backgroundValue );
    ++markerBoundaryIt;
    }
  
  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // Delegate to a geodesic erosion filter.
  //
  //
  typename BinaryReconstructionByErosionImageFilter<TInputImage>::Pointer
    erode
    = BinaryReconstructionByErosionImageFilter<TInputImage>::New();
  erode->SetMarkerImage( markerPtr );
  erode->SetForegroundValue( m_ForegroundValue );
  erode->SetBackgroundValue( backgroundValue );
  erode->SetMaskImage( this->GetInput() );
  erode->SetFullyConnected( m_FullyConnected );
  erode->SetNumberOfThreads( this->GetNumberOfThreads() );
  progress->RegisterInternalFilter(erode,1.0f);

  erode->GraftOutput( this->GetOutput() );
  erode->Update();
  this->GraftOutput( erode->GetOutput() );
}


template<class TInputImage>
void
BinaryFillholeImageFilter<TInputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "ForegroundValue: " << static_cast<typename NumericTraits<InputImagePixelType>::PrintType>(m_ForegroundValue) << std::endl;
  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
}
  
}// end namespace itk
#endif
