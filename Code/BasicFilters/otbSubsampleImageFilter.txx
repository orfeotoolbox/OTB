/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved. 
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbSubsampleImageFilter_txx
#define __otbSubsampleImageFilter_txx

#include "otbSubsampleImageFilter.h"

#include "otbSubsampledImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"


namespace otb {

template <class TInputImage, class TOutputImage>
void
SubsampleImageFilter< TInputImage, TOutputImage >
::PrintSelf( std::ostream & os, itk::Indent indent ) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "SubsampleFactor = " << m_SubsampleFactor << "\n";
}

template <class TInputImage, class TOutputImage>
void
SubsampleImageFilter< TInputImage, TOutputImage >
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  if ( GetSubsampleFactor() > 1 )
  {
    this->GetOutput()->CopyInformation( this->GetInput() );

    OutputImageRegionType newRegion;
    this->CallCopyInputRegionToOutputRegion( newRegion, this->GetInput()->GetLargestPossibleRegion() );
    this->GetOutput()->SetRegions( newRegion );
  }
}

template <class TInputImage, class TOutputImage>
void 
SubsampleImageFilter< TInputImage, TOutputImage >
::CallCopyOutputRegionToInputRegion 
( InputImageRegionType & destRegion, const OutputImageRegionType & srcRegion )
{
  Superclass::CallCopyOutputRegionToInputRegion( destRegion, srcRegion );

  typename OutputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
  typename OutputImageRegionType::SizeType srcSize = srcRegion.GetSize();

  typename InputImageRegionType::IndexType destIndex;
  typename InputImageRegionType::SizeType destSize;

  for ( unsigned int i = 0; i < InputImageDimension; i++ )
  {
    destIndex[i] = srcIndex[i] * GetSubsampleFactor();
    destSize[i] = ( srcSize[i] - 1 ) * GetSubsampleFactor() + 1;
  }

  destRegion.SetIndex( destIndex );
  destRegion.SetSize( destSize );
}

template <class TInputImage, class TOutputImage>
void 
SubsampleImageFilter< TInputImage, TOutputImage >
::CallCopyInputRegionToOutputRegion
( OutputImageRegionType & destRegion, const InputImageRegionType & srcRegion )	 
{
  Superclass::CallCopyInputRegionToOutputRegion( destRegion, srcRegion );

  typename InputImageRegionType::IndexType srcIndex = srcRegion.GetIndex();
  typename InputImageRegionType::SizeType srcSize = srcRegion.GetSize();

  typename OutputImageRegionType::IndexType destIndex;
  typename OutputImageRegionType::SizeType destSize;

  for ( unsigned int i = 0; i < InputImageDimension; i++ )
  {
    destIndex[i] = srcIndex[i] / GetSubsampleFactor();
    destSize[i] = ( srcSize[i] - 1 ) / GetSubsampleFactor() + 1;
  }

  destRegion.SetIndex( destIndex );
  destRegion.SetSize( destSize );
}

template <class TInputImage, class TOutputImage>
void
SubsampleImageFilter< TInputImage, TOutputImage >
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId)
{
  InputImageRegionType inputRegionForThread;
	this->CallCopyOutputRegionToInputRegion( inputRegionForThread, outputRegionForThread );

  SubsampledImageRegionConstIterator< InputImageType > subsamplingIterator 
    ( this->GetInput(), inputRegionForThread );
  subsamplingIterator.SetSubsampleFactor( GetSubsampleFactor() );
  subsamplingIterator.GoToBegin();

  itk::ImageRegionIterator< InputImageType > outputIter 
    ( this->GetOutput(), outputRegionForThread );
  outputIter.GoToBegin();

  while ( !subsamplingIterator.IsAtEnd() && !outputIter.IsAtEnd() )
  {
    outputIter.Set( subsamplingIterator.Get() );

    ++outputIter;
    ++subsamplingIterator;
  }
}

} // end of namespace otb

#endif



