/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelMapToBinaryImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelMapToBinaryImageFilter_txx
#define __itkLabelMapToBinaryImageFilter_txx

#include "itkLabelMapToBinaryImageFilter.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"

namespace itk {

template <class TInputImage, class TOutputImage>
LabelMapToBinaryImageFilter<TInputImage, TOutputImage>
::LabelMapToBinaryImageFilter()
{
  m_BackgroundValue = NumericTraits<OutputImagePixelType>::NonpositiveMin();
  m_ForegroundValue = NumericTraits<OutputImagePixelType>::max();
}

template <class TInputImage, class TOutputImage>
void 
LabelMapToBinaryImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  if ( !input )
    { return; }
  input->SetRequestedRegion( input->GetLargestPossibleRegion() );
}


template <class TInputImage, class TOutputImage>
void 
LabelMapToBinaryImageFilter<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TOutputImage>
void
LabelMapToBinaryImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  m_Barrier = Barrier::New();
  m_Barrier->Initialize( this->GetNumberOfThreads() );

  Superclass::BeforeThreadedGenerateData();

}


template<class TInputImage, class TOutputImage>
void
LabelMapToBinaryImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData( const OutputImageRegionType& outputRegionForThread, int threadId )
{
  OutputImageType * output = this->GetOutput();
//  const InputImageType * input = this->GetInput();

  // fill the output with background value - they will be overiden with the foreground value
  // later, if there is some objects
  if( this->GetNumberOfInputs() == 2 )
    {
    // fill the background with the background values from the background image
    ImageRegionConstIterator< OutputImageType > bgIt( this->GetBackgroundImage(), outputRegionForThread );
    ImageRegionIterator< OutputImageType > oIt( output, outputRegionForThread );
    for( oIt.GoToBegin(), bgIt.GoToBegin();
      !oIt.IsAtEnd();
      ++oIt, ++bgIt )
      {
      const OutputImagePixelType & bg = bgIt.Get();
      if( bg != m_ForegroundValue )
        {
        oIt.Set( bg );
        }
      else
        {
        oIt.Set( m_BackgroundValue );
        }
      }
    }
  else
    {
    // fill the background with the background value
    ImageRegionIterator< OutputImageType > oIt( output, outputRegionForThread );
    for( oIt.GoToBegin(); !oIt.IsAtEnd(); ++oIt )
      {
      oIt.Set( m_BackgroundValue );
      }
    }

  // wait for the other threads to complete that part
  m_Barrier->Wait();

  // and delegate to the superclass implementation to use the thread support for the label objects
  Superclass::ThreadedGenerateData( outputRegionForThread, threadId );

}


template<class TInputImage, class TOutputImage>
void
LabelMapToBinaryImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData( LabelObjectType * labelObject )
{
  OutputImageType * output = this->GetOutput();

  typename InputImageType::LabelObjectType::LineContainerType::const_iterator lit;
  typename InputImageType::LabelObjectType::LineContainerType & lineContainer = labelObject->GetLineContainer();

  for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
    {
    IndexType idx = lit->GetIndex();
    unsigned long length = lit->GetLength();
    for( unsigned int i=0; i<length; i++)
      {
      output->SetPixel( idx, m_ForegroundValue );
      idx[0]++;
      }
    }
}

}// end namespace itk
#endif
