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
#ifndef __otbVectorImageTo3DScalarImageFilter_txx
#define __otbVectorImageTo3DScalarImageFilter_txx

#include "otbVectorImageTo3DScalarImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageSliceIteratorWithIndex.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage>
VectorImageTo3DScalarImageFilter<TInputImage, TOutputImage>
::VectorImageTo3DScalarImageFilter()
{}
/** Generate output information */
template <class TInputImage, class TOutputImage>
void
VectorImageTo3DScalarImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation()
{

  const InputImageType* inputPtr = this->GetInput();
  OutputImageType*      outputPtr = this->GetOutput();

  InputImageRegionType inputLargestRegion = this->GetInput()->GetLargestPossibleRegion();
  OutputImageSizeType  size;
  OutputImageIndexType index;
  for (unsigned int i = 0; i < InputImageType::ImageDimension; ++i)
    {
    size[i] = inputLargestRegion.GetSize()[i];
    index[i] = inputLargestRegion.GetIndex()[i];
    }
  size[OutputImageType::ImageDimension - 1] = inputPtr->GetNumberOfComponentsPerPixel();
  index[OutputImageType::ImageDimension - 1] = 0;

  OutputImageRegionType outputRegion;
  outputRegion.SetSize(size);
  outputRegion.SetIndex(index);
  outputPtr->SetLargestPossibleRegion(outputRegion);
}
/** Generate input requested region */
template <class TInputImage, class TOutputImage>
void
VectorImageTo3DScalarImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  InputImageType*  inputPtr = const_cast<InputImageType *>(this->GetInput());
  OutputImageType* outputPtr = this->GetOutput();

  OutputImageRegionType requestedRegion = outputPtr->GetRequestedRegion();
  InputImageRegionType  inputRequestedRegion;
  InputImageSizeType    size;
  InputImageIndexType   index;

  for (unsigned int i = 0; i < InputImageType::ImageDimension; ++i)
    {
    size[i] = requestedRegion.GetSize()[i];
    index[i] = requestedRegion.GetIndex()[i];
    }
  inputRequestedRegion.SetSize(size);
  inputRequestedRegion.SetIndex(index);
  inputPtr->SetRequestedRegion(inputRequestedRegion);
}
template <class TInputImage, class TOutputImage>
void
VectorImageTo3DScalarImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       itk::ThreadIdType threadId)
{
  const InputImageType* inputPtr = this->GetInput();
  OutputImageType*      outputPtr = this->GetOutput();

  typedef itk::ImageRegionConstIterator<InputImageType>     InputIteratorType;
  typedef itk::ImageSliceIteratorWithIndex<OutputImageType> OutputIteratorType;

  InputImageRegionType inputRegion;
  InputImageSizeType   size;
  InputImageIndexType  index;

  for (unsigned int i = 0; i < InputImageType::ImageDimension; ++i)
    {
    size[i] = outputRegionForThread.GetSize()[i];
    index[i] = outputRegionForThread.GetIndex()[i];
    }
  inputRegion.SetSize(size);
  inputRegion.SetIndex(index);

  InputIteratorType inIt(inputPtr, inputRegion);

  OutputIteratorType outIt(outputPtr, outputRegionForThread);
  outIt.SetFirstDirection(0);
  outIt.SetSecondDirection(1);

  outIt.GoToBegin();
  inIt.GoToBegin();

  while (!outIt.IsAtEnd())
    {
    outIt.Set(static_cast<OutputPixelType>(inIt.Get()[outIt.GetIndex()[InputImageType::ImageDimension]]));
    ++inIt;
    if (inIt.IsAtEnd())
      {
      inIt.GoToBegin();
      }
    ++outIt;
    if (outIt.IsAtEndOfLine())
      {
      outIt.NextLine();
      }
    if (outIt.IsAtEndOfSlice())
      {
      outIt.NextSlice();
      }
    }
}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage>
void
VectorImageTo3DScalarImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
