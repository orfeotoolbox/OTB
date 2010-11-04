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
ListSampleToListSampleFilter<TInputSampleList,TOutputSampleList>
::ListSampleToListSampleFilter()
{
  this->SetNumberOfRequiredInputs(1);
  this->SetNumberOfRequiredOutputs(1);

  // Generate the output sample list
  typename OutputSampleListObjectType::Pointer outputPtr =
    static_cast< OutputSampleListObjectType * >(this->MakeOutput(0).GetPointer());
  this->ProcessObject::SetNthOutput(0, outputPtr.GetPointer());
}

template < class TInputSampleList, class TOutputSampleList >
typename ListSampleToListSampleFilter<TInputSampleList,TOutputSampleList>
::DataObjectPointer
ListSampleToListSampleFilter<TInputSampleList,TOutputSampleList>
::MakeOutput(unsigned int itkNotUsed(idx))
{
  typename OutputSampleListObjectType::Pointer outputPtr = OutputSampleListObjectType::New();
  OutputSampleListPointer outputSampleList = OutputSampleListType::New();
  outputPtr->Set(outputSampleList);
  return static_cast<DataObjectPointer>(outputPtr);
}

template < class TInputSampleList, class TOutputSampleList >
void
ListSampleToListSampleFilter<TInputSampleList,TOutputSampleList>
::SetInput( const InputSampleListType * input )
{
  typename InputSampleListObjectType::Pointer inputPtr = InputSampleListObjectType::New();
  inputPtr->Set(input);
  this->SetInput(inputPtr);
}

template < class TInputSampleList, class TOutputSampleList >
void
ListSampleToListSampleFilter<TInputSampleList,TOutputSampleList>
::SetInput( const InputSampleListObjectType * inputPtr )
{
  // Process object is not const-correct so the const_cast is required here
  this->ProcessObject::SetNthInput(0,
                                   const_cast< InputSampleListObjectType* >( inputPtr ) );
}

template < class TInputSampleList, class TOutputSampleList >
const typename ListSampleToListSampleFilter<TInputSampleList,TOutputSampleList>
::InputSampleListObjectType *
ListSampleToListSampleFilter<TInputSampleList,TOutputSampleList>
::GetInput() const
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const InputSampleListObjectType * >
    (this->ProcessObject::GetInput(0) );
}

template < class TInputSampleList, class TOutputSampleList >
const typename ListSampleToListSampleFilter<TInputSampleList,TOutputSampleList>
::InputSampleListType *
ListSampleToListSampleFilter<TInputSampleList,TOutputSampleList>
::GetInputSampleList() const
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  typename InputSampleListObjectType::Pointer dataObjectPointer = static_cast<const InputSampleListObjectType * >
    (this->ProcessObject::GetInput(0) );
  return dataObjectPointer->Get();
}

template < class TInputSampleList, class TOutputSampleList >
typename ListSampleToListSampleFilter<TInputSampleList,TOutputSampleList>
::OutputSampleListType *
ListSampleToListSampleFilter<TInputSampleList,TOutputSampleList>
::GetOutputSampleList()
{
  typename OutputSampleListObjectType::Pointer dataObjectPointer = static_cast<OutputSampleListObjectType * >
    (this->ProcessObject::GetOutput(0) );
  return const_cast<OutputSampleListType *>(dataObjectPointer->Get());
}

template < class TInputSampleList, class TOutputSampleList >
typename ListSampleToListSampleFilter<TInputSampleList,TOutputSampleList>
::OutputSampleListObjectType *
ListSampleToListSampleFilter<TInputSampleList,TOutputSampleList>
::GetOutput()
{
  return static_cast<OutputSampleListObjectType * >
    (this->ProcessObject::GetOutput(0) );
}


template < class TInputSampleList, class TOutputSampleList >
void
ListSampleToListSampleFilter<TInputSampleList,TOutputSampleList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}


} // End namespace Statistics
} // End namespace otb

#endif
