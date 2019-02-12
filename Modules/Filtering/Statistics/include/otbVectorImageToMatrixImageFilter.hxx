/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbVectorImageToMatrixFilter_hxx
#define otbVectorImageToMatrixFilter_hxx
#include "otbVectorImageToMatrixImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
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
::MakeOutput(DataObjectPointerArraySizeType output)
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
