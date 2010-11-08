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
#ifndef __otbListSampleSource_txx
#define __otbListSampleSource_txx

#include "otbListSampleSource.h"

namespace otb {
namespace Statistics {

template < class TInputSampleList, class TOutputSampleList >
ListSampleSource<TInputSampleList,TOutputSampleList>
::ListSampleSource()
{
  this->SetNumberOfRequiredOutputs(1);

  // Generate the output sample list
  typename OutputSampleListObjectType::Pointer outputPtr =
    static_cast< OutputSampleListObjectType * >(this->MakeOutput(0).GetPointer());
  this->ProcessObject::SetNthOutput(0, outputPtr.GetPointer());
}

template < class TInputSampleList, class TOutputSampleList >
typename ListSampleSource<TInputSampleList,TOutputSampleList>
::DataObjectPointer
ListSampleSource<TInputSampleList,TOutputSampleList>
::MakeOutput(unsigned int itkNotUsed(idx))
{
  typename OutputSampleListObjectType::Pointer outputPtr = OutputSampleListObjectType::New();
  OutputSampleListPointer outputSampleList = OutputSampleListType::New();
  outputPtr->Set(outputSampleList);
  return static_cast<DataObjectPointer>(outputPtr);
}

template < class TInputSampleList, class TOutputSampleList >
typename ListSampleSource<TInputSampleList,TOutputSampleList>
::OutputSampleListType *
ListSampleSource<TInputSampleList,TOutputSampleList>
::GetOutputSampleList()
{
  typename OutputSampleListObjectType::Pointer dataObjectPointer = static_cast<OutputSampleListObjectType * >
    (this->ProcessObject::GetOutput(0) );
  return const_cast<OutputSampleListType *>(dataObjectPointer->Get());
}

template < class TInputSampleList, class TOutputSampleList >
typename ListSampleSource<TInputSampleList,TOutputSampleList>
::OutputSampleListObjectType *
ListSampleSource<TInputSampleList,TOutputSampleList>
::GetOutput()
{
  return static_cast<OutputSampleListObjectType * >
    (this->ProcessObject::GetOutput(0) );
}

template < class TInputSampleList, class TOutputSampleList >
void
ListSampleSource<TInputSampleList,TOutputSampleList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} // End namespace Statistics
} // End namespace otb

#endif
