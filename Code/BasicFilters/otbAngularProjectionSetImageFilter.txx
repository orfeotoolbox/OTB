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
#ifndef __otbAngularProjectionSetImageFilter_txx
#define __otbAngularProjectionSetImageFilter_txx
#include "otbAngularProjectionSetImageFilter.h"

#include <vnl/vnl_math.h>

#include <itkImageRegionIterator.h>

namespace otb {

template < class TInputImage, class TOutputImage, class TAngleList, class TPrecision >
AngularProjectionSetImageFilter< TInputImage, TOutputImage, TAngleList, TPrecision >
::AngularProjectionSetImageFilter ()
{
  //this->SetNumberOfRequiredInputs( NumberOfInputImages );
  this->SetNumberOfRequiredOutputs(1);
  this->SetNthOutput(0, OutputImageListType::New());

  m_FilterList = FilterListType::New();
  m_AngleList = AngleListType::New();
}

template < class TInputImage, class TOutputImage, class TAngleList, class TPrecision >
void
AngularProjectionSetImageFilter< TInputImage, TOutputImage, TAngleList, TPrecision >
::SetInput ( unsigned int i, const InputImageType * img )
{
  this->itk::ProcessObject::SetNthInput(i,
    const_cast< InputImageType * >( img ) );
}

template < class TInputImage, class TOutputImage, class TAngleList, class TPrecision >
const TInputImage *
AngularProjectionSetImageFilter< TInputImage, TOutputImage, TAngleList, TPrecision >
::GetInput ( unsigned int i ) const
{
  if ( i >= this->GetNumberOfInputs() )
  {
    return 0;
  }

  return static_cast<const InputImageType * >
    (this->itk::ProcessObject::GetInput(i) );
}


template < class TInputImage, class TOutputImage, class TAngleList, class TPrecision >
void
AngularProjectionSetImageFilter< TInputImage, TOutputImage, TAngleList, TPrecision >
::GenerateData ()
{
  itk::ProgressAccumulator::Pointer progress = itk::ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  std::cerr << "Using " << GetAngleList()->Size() << " outputs...\n";
  GetFilterList()->Resize( GetAngleList()->Size() );
  this->GetOutput()->Resize( GetAngleList()->Size() );

  for ( unsigned int idx = 0; idx < GetFilterList()->Size(); ++idx )
  {
    GetFilterList()->SetNthElement(0, FilterType::New());
    FilterPointerType filter = GetFilterList()->GetNthElement(0);
    for ( unsigned int i = 0; i < this->GetNumberOfInputs(); ++i )
      filter->SetInput( i, this->GetInput(i) );
    filter->SetAngleArray( GetAngleList()->GetMeasurementVector(idx) );

    progress->RegisterInternalFilter(filter, 1.f/static_cast<float>( GetAngleList()->Size() ) );

    filter->Update();

    this->GetOutput()->SetNthElement( idx, filter->GetOutput() );
  }
}

} // end of namespace otb

#endif


