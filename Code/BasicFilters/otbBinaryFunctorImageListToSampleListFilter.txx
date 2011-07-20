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
#ifndef __otbBinaryFunctorImageListToSampleListFilter_txx
#define __otbBinaryFunctorImageListToSampleListFilter_txx
#include "otbBinaryFunctorImageListToSampleListFilter.h"

#include "itkProgressReporter.h"

namespace otb {

template < class TInputImageList, class TOutputSampleList, class TFunction >
BinaryFunctorImageListToSampleListFilter< TInputImageList, TOutputSampleList, TFunction >
::BinaryFunctorImageListToSampleListFilter()
{
  this->SetNumberOfRequiredInputs(2);

  // Generate the output sample list
  typename OutputSampleListObjectType::Pointer outputPtr =
    static_cast< OutputSampleListObjectType * >(this->MakeOutput(0).GetPointer());
  this->ProcessObject::SetNthOutput(0, outputPtr.GetPointer());

}

template < class TInputImageList, class TOutputSampleList, class TFunction >
typename BinaryFunctorImageListToSampleListFilter< TInputImageList, TOutputSampleList, TFunction >
::DataObjectPointer
BinaryFunctorImageListToSampleListFilter< TInputImageList, TOutputSampleList, TFunction >
::MakeOutput(unsigned int itkNotUsed(idx))
{
  typename OutputSampleListObjectType::Pointer outputPtr = OutputSampleListObjectType::New();
  OutputSampleListPointer outputSampleList = OutputSampleListType::New();
  outputPtr->Set(outputSampleList);
  return static_cast<DataObjectPointer>(outputPtr);
}

template < class TInputImageList, class TOutputSampleList, class TFunction >
typename BinaryFunctorImageListToSampleListFilter< TInputImageList, TOutputSampleList, TFunction >
::OutputSampleListType *
BinaryFunctorImageListToSampleListFilter< TInputImageList, TOutputSampleList, TFunction >
::GetOutputSampleList()
{
  typename OutputSampleListObjectType::Pointer dataObjectPointer
    = static_cast<OutputSampleListObjectType * > (this->ProcessObject::GetOutput(0) );
  return const_cast<OutputSampleListType *>(dataObjectPointer->Get());
}

template < class TInputImageList, class TOutputSampleList, class TFunction >
typename BinaryFunctorImageListToSampleListFilter< TInputImageList, TOutputSampleList, TFunction >
::OutputSampleListObjectType *
BinaryFunctorImageListToSampleListFilter< TInputImageList, TOutputSampleList, TFunction >
::GetOutput()
{
  return static_cast<OutputSampleListObjectType * >
    (this->ProcessObject::GetOutput(0) );
}

template < class TInputImageList, class TOutputSampleList, class TFunction >
void
BinaryFunctorImageListToSampleListFilter< TInputImageList, TOutputSampleList, TFunction >
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os, indent);
}

template < class TInputImageList, class TOutputSampleList, class TFunction >
void
BinaryFunctorImageListToSampleListFilter< TInputImageList, TOutputSampleList, TFunction >
::SetInput1( const InputImageListType * inputPtr )
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(0,
    const_cast< InputImageListType * >( inputPtr ) );
}

template < class TInputImageList, class TOutputSampleList, class TFunction >
void
BinaryFunctorImageListToSampleListFilter< TInputImageList, TOutputSampleList, TFunction >
::SetInput2( const InputImageListType * inputPtr )
{
  // Process object is not const-correct so the const_cast is required here
  this->itk::ProcessObject::SetNthInput(1,
    const_cast< InputImageListType * >( inputPtr ) );
}

template < class TInputImageList, class TOutputSampleList, class TFunction >
const TInputImageList *
BinaryFunctorImageListToSampleListFilter< TInputImageList, TOutputSampleList, TFunction >
::GetInput1() const
{
  if ( this->GetNumberOfInputs() < 1 )
  {
    return 0;
  }

  return static_cast<const InputImageListType * >
    (this->itk::ProcessObject::GetInput(0) );
}

template < class TInputImageList, class TOutputSampleList, class TFunction >
const TInputImageList *
BinaryFunctorImageListToSampleListFilter< TInputImageList, TOutputSampleList, TFunction >
::GetInput2() const
{
  if ( this->GetNumberOfInputs() < 2 )
  {
    return 0;
  }

  return static_cast<const InputImageListType * >
    (this->itk::ProcessObject::GetInput(1) );
}

template < class TInputImageList, class TOutputSampleList, class TFunction >
void
BinaryFunctorImageListToSampleListFilter< TInputImageList, TOutputSampleList, TFunction >
::GenerateData ()
{
  const InputImageListType * list1 = this->GetInput1();
  InputImageListConstIteratorType listIterator1 = list1->Begin();

  const InputImageListType * list2 = this->GetInput2();
  InputImageListConstIteratorType listIterator2 = list2->Begin();

  OutputSampleListType * outputList = this->GetOutputSampleList();

  // Set-up progress reporting
  itk::ProgressReporter progress(this, 0, list1->Size());

  while ( listIterator1 != list1->End()
          && listIterator2 != list2->End() )
  {
    InputImageType * img1 = listIterator1.Get();
    ImageConstIteratorType it1 ( img1, img1->GetLargestPossibleRegion() );
    it1.GoToBegin();

    InputImageType * img2 = listIterator2.Get();
    ImageConstIteratorType it2 ( img2, img2->GetLargestPossibleRegion() );
    it2.GoToBegin();

    while ( !it1.IsAtEnd() && !it2.IsAtEnd() )
    {
      if ( m_Functor.IsToGenerate( it1.Get(), it2.Get() ) )
      {
        outputList->PushBack( m_Functor( it1.Get(), it2.Get() ) );
      }

      ++it1;
      ++it2;
    }

    ++listIterator1;
    ++listIterator2;

    progress.CompletedPixel();
  }
}

} // end of namespace otb

#endif



