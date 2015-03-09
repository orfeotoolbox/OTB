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
#ifndef __otbListSampleToListSample_txx
#define __otbListSampleToListSample_txx

#include "otbListSampleToListSampleFilter.h"
#include "itkProgressReporter.h"

namespace otb {
namespace Statistics {

template < class TInputSampleList, class TOutputSampleList >
ListSampleToListSampleFilter<TInputSampleList, TOutputSampleList>
::ListSampleToListSampleFilter()
{
  this->SetNumberOfRequiredInputs(1);
}

template < class TInputSampleList, class TOutputSampleList >
void
ListSampleToListSampleFilter<TInputSampleList, TOutputSampleList>
::SetInput( const InputSampleListType * inputPtr )
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
                                        const_cast< InputSampleListType* >( inputPtr ) );
}

template < class TInputSampleList, class TOutputSampleList >
const typename ListSampleToListSampleFilter<TInputSampleList, TOutputSampleList>
::InputSampleListType *
ListSampleToListSampleFilter<TInputSampleList, TOutputSampleList>
::GetInput() const
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const InputSampleListType * >
    (this->itk::ProcessObject::GetInput(0) );
}

template < class TInputSampleList, class TOutputSampleList >
void
ListSampleToListSampleFilter<TInputSampleList, TOutputSampleList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
}

} // End namespace Statistics
} // End namespace otb

#endif
