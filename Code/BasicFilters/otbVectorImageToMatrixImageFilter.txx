/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANT2ABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbVectorImageToMatrixFilter_txx
#define __otbVectorImageToMatrixFilter_txx
#include "otbVectorImageToMatrixImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"

namespace otb
{

template<class TInputImage>
PersistentVectorImageToMatrixFilter<TInputImage>
::PersistentVectorImageToMatrixFilter()
{
  // first output is a copy of the image, DataObject created by
  // superclass
  //
  // allocate the data objects for the outputs which are
  // just decorators around vector/matric types
  this->itk::ProcessObject::SetNthOutput(1, this->MakeOutput(1).GetPointer());
}

template<class TInputImage>
itk::DataObject::Pointer
PersistentVectorImageToMatrixFilter<TInputImage>
::MakeOutput(unsigned int output)
{
  switch (output)
    {
    case 0:
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    case 1:
      return static_cast<itk::DataObject*>(MatrixObjectType::New().GetPointer());
      break;
    default:
      // might as well make an image
      return static_cast<itk::DataObject*>(TInputImage::New().GetPointer());
      break;
    }
}

template<class TInputImage>
typename PersistentVectorImageToMatrixFilter<TInputImage>::MatrixObjectType*
PersistentVectorImageToMatrixFilter<TInputImage>
::GetMatrixOutput()
{
  return static_cast<MatrixObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
const typename PersistentVectorImageToMatrixFilter<TInputImage>::MatrixObjectType*
PersistentVectorImageToMatrixFilter<TInputImage>
::GetMatrixOutput() const
{
  return static_cast<const MatrixObjectType*>(this->itk::ProcessObject::GetOutput(1));
}

template<class TInputImage>
void
PersistentVectorImageToMatrixFilter<TInputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  if (this->GetInput())
    {
    this->GetOutput()->CopyInformation(this->GetInput());
    this->GetOutput()->SetLargestPossibleRegion(this->GetInput()->GetLargestPossibleRegion());

    if (this->GetOutput()->GetRequestedRegion().GetNumberOfPixels() == 0)
      {
      this->GetOutput()->SetRequestedRegion(this->GetOutput()->GetLargestPossibleRegion());
      }
    }
}

template<class TInputImage>
void
PersistentVectorImageToMatrixFilter<TInputImage>
::AllocateOutputs()
{
  // This is commented to prevent the streaming of the whole image for the first stream strip
  // It shall not cause any problem because the output image of this filter is not intended to be used.
  //InputImagePointer image = const_cast< TInputImage * >( this->GetInput() );
  //this->GraftOutput( image );
  // Nothing that needs to be allocated for the remaining outputs
}

template<class TInputImage>
void
PersistentVectorImageToMatrixFilter<TInputImage>
::Reset()
{
  TInputImage * inputPtr = const_cast<TInputImage *>(this->GetInput());
  inputPtr->UpdateOutputInformation();

  this->GetMatrixOutput()->Get().set_size(
    inputPtr->GetNumberOfComponentsPerPixel(),
    inputPtr->GetLargestPossibleRegion().GetNumberOfPixels());
}

template<class TInputImage>
void
PersistentVectorImageToMatrixFilter<TInputImage>
::Synthetize()
{
}

template<class TInputImage>
void
PersistentVectorImageToMatrixFilter<TInputImage>
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Grab the input
  InputImagePointer inputPtr = const_cast<TInputImage *>(this->GetInput());
  const unsigned int width = this->GetInput()->GetLargestPossibleRegion().GetSize()[0];
  itk::ImageRegionConstIteratorWithIndex<TInputImage> it(inputPtr, outputRegionForThread);

  for (it.GoToBegin(); !it.IsAtEnd(); ++it, progress.CompletedPixel())
    {
    const IndexType& idx = it.GetIndex();

    unsigned int col = idx[0] + idx[1] * width;
    const PixelType& vectorValue = it.Get();
    MatrixType& m = this->GetMatrixOutput()->Get();
    for (unsigned int r = 0; r < vectorValue.Size(); ++r)
      {
      m(r, col) = vectorValue[r];
      }
    }
}

template <class TImage>
void
PersistentVectorImageToMatrixFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb
#endif
