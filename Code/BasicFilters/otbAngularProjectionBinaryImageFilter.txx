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
#ifndef __otbAngularProjectionBinaryImageFilter_txx
#define __otbAngularProjectionBinaryImageFilter_txx
#include "otbAngularProjectionBinaryImageFilter.h"

#include <vnl/vnl_math.h>

#include <itkImageRegionIterator.h>

namespace otb {

template < class TInputImage, class TOutputImage, class TPrecision >
AngularProjectionBinaryImageFilter< TInputImage, TOutputImage, TPrecision >
::AngularProjectionBinaryImageFilter ()
{
  this->SetNumberOfRequiredInputs(2);
}

template < class TInputImage, class TOutputImage, class TPrecision >
void
AngularProjectionBinaryImageFilter< TInputImage, TOutputImage, TPrecision >
::SetInput1 ( const InputImageType * inputPtr )
{
  this->SetNthInput(0, const_cast<InputImageType*>( inputPtr ));
}

template < class TInputImage, class TOutputImage, class TPrecision >
void
AngularProjectionBinaryImageFilter< TInputImage, TOutputImage, TPrecision >
::SetInput2 ( const InputImageType * inputPtr )
{
  this->SetNthInput(1, const_cast<InputImageType*>( inputPtr ));
}

template < class TInputImage, class TOutputImage, class TPrecision >
const TInputImage *
AngularProjectionBinaryImageFilter< TInputImage, TOutputImage, TPrecision >
::GetInput1() const
{
  if ( this->GetNumberOfInputs() < 1 )
  {
    return 0;
  }

  return static_cast<const TInputImage * > (this->itk::ProcessObject::GetInput(0) );
}

template < class TInputImage, class TOutputImage, class TPrecision >
const TInputImage *
AngularProjectionBinaryImageFilter< TInputImage, TOutputImage, TPrecision >
::GetInput2() const
{
  if ( this->GetNumberOfInputs() < 2 )
  {
    return 0;
  }

  return static_cast<const TInputImage * > (this->itk::ProcessObject::GetInput(1));
}

template < class TInputImage, class TOutputImage, class TPrecision >
void
AngularProjectionBinaryImageFilter< TInputImage, TOutputImage, TPrecision >
::SetAngleSet ( std::vector<PrecisionType> & angle )
{
  m_AngleSet = angle;
  this->SetNumberOfRequiredOutputs( angle.size() );
  for ( unsigned int i = 0; i < this->GetNumberOfOutputs(); ++i )
  {
    this->SetNthOutput(i, OutputImageType::New());
  }
  this->Modified();
}

template < class TInputImage, class TOutputImage, class TPrecision >
void
AngularProjectionBinaryImageFilter< TInputImage, TOutputImage, TPrecision >
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  for (unsigned int i = 0; i < this->GetNumberOfOutputs(); ++i)
  {
    this->GetOutput(i)->SetRegions(
      this->GetInput()->GetRequestedRegion());
  }
}

template < class TInputImage, class TOutputImage, class TPrecision >
void
AngularProjectionBinaryImageFilter< TInputImage, TOutputImage, TPrecision >
::ThreadedGenerateData
  ( const OutputImageRegionType & outputRegionForThread, itk::ThreadIdType threadId )
{
  itk::ProgressReporter reporter(this, threadId,
                                 outputRegionForThread.GetNumberOfPixels() );

  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion( inputRegionForThread, outputRegionForThread );

  itk::ImageRegionConstIterator<InputImageType> iter1
    ( this->GetInput1(), inputRegionForThread );
  iter1.GoToBegin();

  itk::ImageRegionConstIterator<InputImageType> iter2
    ( this->GetInput2(), inputRegionForThread );
  iter2.GoToBegin();

  std::vector< itk::ImageRegionIterator<OutputImageType> > outIter
    ( this->GetNumberOfOutputs() );
  for ( unsigned int i = 0; i < outIter.size(); ++i )
  {
    outIter[i] = itk::ImageRegionIterator<OutputImageType>
                  ( this->GetOutput(i), outputRegionForThread );
    outIter[i].GoToBegin();
  }

  while ( !iter1.IsAtEnd() && !iter2.IsAtEnd() )
  {
    for ( unsigned int i = 0; i < outIter.size(); ++i )
    {
      outIter[i].Set( vcl_cos( m_AngleSet[i] ) * iter1.Get()
                      + vcl_sin( m_AngleSet[i] ) * iter2.Get() );
      ++outIter[i];
    }

    ++iter1;
    ++iter2;

    reporter.CompletedPixel();
  }
}

} // end of namespace otb

#endif


