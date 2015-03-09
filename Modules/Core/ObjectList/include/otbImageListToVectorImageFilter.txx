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
#ifndef __otbImageListToVectorImageFilter_txx
#define __otbImageListToVectorImageFilter_txx

#include "otbImageListToVectorImageFilter.h"
#include "itkImageRegionIterator.h"
#include <vector>
#include "otbMacro.h"
#include "itkProgressReporter.h"

namespace otb
{
/**
 * GenerateOutputInformation
 */
template <class TImageList, class TVectorImage>
void
ImageListToVectorImageFilter<TImageList, TVectorImage>
::GenerateOutputInformation(void)
{
  if (this->GetOutput())
    {
    if (this->GetInput()->Size() > 0)
      {
      this->GetOutput()->CopyInformation(this->GetInput()->GetNthElement(0));
      this->GetOutput()->SetNumberOfComponentsPerPixel(this->GetInput()->Size());
      this->GetOutput()->SetLargestPossibleRegion(this->GetInput()->GetNthElement(0)->GetLargestPossibleRegion());
      }
    }
}
/**
 * GenerateInputRequestedRegion
 */
template <class TImageList, class TVectorImage>
void
ImageListToVectorImageFilter<TImageList, TVectorImage>
::GenerateInputRequestedRegion(void)
{
  InputImageListPointerType                  inputPtr = this->GetInput();
  typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();
  while (inputListIt != inputPtr->End())
    {
    inputListIt.Get()->SetRequestedRegion(this->GetOutput()->GetRequestedRegion());
    ++inputListIt;
    }
}
/**
 * Main computation method
 */
template <class TImageList, class TVectorImage>
void
ImageListToVectorImageFilter<TImageList, TVectorImage>
::GenerateData(void)
{

  InputImageListPointerType    inputPtr = this->GetInput();
  OutputVectorImagePointerType outputPtr = this->GetOutput();

  itk::ProgressReporter progress(this, 0, outputPtr->GetRequestedRegion().GetNumberOfPixels());

  // Output image initializations
  typename OutputVectorImageType::PixelType black;
  black.SetSize(inputPtr->Size());
  black.Fill(0);
  outputPtr->SetBufferedRegion(outputPtr->GetRequestedRegion());
  outputPtr->Allocate();
  outputPtr->FillBuffer(black);

  // defines input and output iterators
  typedef itk::ImageRegionConstIterator<InputImageType>   InputIteratorType;
  typedef itk::ImageRegionIterator<OutputVectorImageType> OutputIteratorType;

  typename InputImageListType::ConstIterator inputListIt = inputPtr->Begin();

  // defines a vector of input iterators
  typedef std::vector<InputIteratorType> InputIteratorListType;
  InputIteratorListType inputIteratorList;

  // fills the vector of input iterators
  for (; inputListIt != inputPtr->End(); ++inputListIt)
    {
    inputIteratorList.push_back(InputIteratorType(inputListIt.Get(), outputPtr->GetRequestedRegion()));
    inputIteratorList.back().GoToBegin();
    }

  // walk through the output image
  OutputIteratorType outputIt(outputPtr, outputPtr->GetRequestedRegion());
  outputIt.GoToBegin();

  while (!outputIt.IsAtEnd())
    {
    typename OutputVectorImageType::PixelType pixel = outputIt.Get();
    unsigned int                              counter = 0;
    // for each input iterator, fill the right component
    for (typename InputIteratorListType::iterator it = inputIteratorList.begin();
         it != inputIteratorList.end(); ++it)
      {
      if (!it->IsAtEnd())
        {
        pixel[counter] = static_cast<typename OutputVectorImageType::InternalPixelType>(it->Get());
        ++(*it);
        ++counter;
        }
      }
    outputIt.Set(pixel);
    progress.CompletedPixel();
    ++outputIt;
    }
}
/**
 * PrintSelf Method
 */
template <class TVectorImageType, class TImageList>
void
ImageListToVectorImageFilter<TVectorImageType, TImageList>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
