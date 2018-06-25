/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbRemoveCarvingPathFilter_hxx
#define otbRemoveCarvingPathFilter_hxx

#include "otbRemoveCarvingPathFilter.h"
#include "itkMacro.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TInputPath, class TOutputImage>
RemoveCarvingPathFilter<TInputImage, TInputPath, TOutputImage>
::RemoveCarvingPathFilter()
{
  m_Value = static_cast<OutputImagePixelType>(255.0);
  m_Direction = 0;
}

template <class TInputImage, class TInputPath, class TOutputImage>
inline void
RemoveCarvingPathFilter<TInputImage, TInputPath, TOutputImage>
::SetInputPath(const TInputPath * path)
{
  this->SetPathInput(path);
}

template <class TInputImage, class TInputPath, class TOutputImage>
const TInputPath *
RemoveCarvingPathFilter<TInputImage, TInputPath, TOutputImage>
::GetInputPath(void)
{
  return this->GetPathInput();
}

/**
 * Main computation method.
 */
template <class TInputImage, class TInputPath, class TOutputImage>
void
RemoveCarvingPathFilter<TInputImage, TInputPath, TOutputImage>
::GenerateData(void)
{

  const InputImageType * inputImage = this->GetImageInput();
  const InputPathType *  inputPath = this->GetPathInput();
  OutputImageType *      outputImage = this->GetOutput();

  outputImage->SetBufferedRegion(outputImage->GetRequestedRegion());
  outputImage->Allocate();

  InputIteratorType inputIterator(inputImage,
                                  inputImage->GetLargestPossibleRegion());
  OutputIteratorType outputIterator(outputImage,
                                    outputImage->GetLargestPossibleRegion());

  unsigned int dir0;
  unsigned int dir1;

  if (m_Direction == 0)
    {
    dir0 = 0;
    dir1 = 1;
    }
  else
    {
    dir0 = 1;
    dir1 = 0;
    }

  inputIterator.SetFirstDirection(dir0);
  inputIterator.SetSecondDirection(dir1);
  inputIterator.GoToBegin();
  outputIterator.SetFirstDirection(dir0);
  outputIterator.SetSecondDirection(dir1);
  outputIterator.GoToBegin();

  int                                                   line = -1;
  typename InputImageType::IndexType                    index;
  typename InputPathType::VertexListType::ConstPointer  vertexList = inputPath->GetVertexList();
  typename InputPathType::VertexListType::ConstIterator pathIterator;
  typename InputImageType::IndexType                    indexToRemove;

  //go to the end of the path
  pathIterator = vertexList->Begin();
  while (pathIterator != vertexList->End())
    {
    ++pathIterator;
    }
  --pathIterator;

  while (!inputIterator.IsAtEnd())
    {
    while (!inputIterator.IsAtEndOfSlice())
      {
      while (!inputIterator.IsAtEndOfLine())
        {
        index = inputIterator.GetIndex();

        if (index[dir1] != line)
          {
          line = index[dir1];
          typename InputImageType::PointType tmpIndex;
          inputImage->TransformContinuousIndexToPhysicalPoint(pathIterator.Value(), tmpIndex);
          inputImage->TransformPhysicalPointToIndex(tmpIndex, indexToRemove);

          if (pathIterator != vertexList->Begin())
            {
            --pathIterator;
            }
          if (index[dir1] != indexToRemove[dir1])
            {
            itkExceptionMacro(<< "Error!!!");
            }
          }
        if (index[dir0] != indexToRemove[dir0])
          {
          outputIterator.Set(inputIterator.Get());
          ++outputIterator;
          }
        ++inputIterator;
        }
      inputIterator.NextLine();
      outputIterator.NextLine();
      }
    inputIterator.NextSlice();
    outputIterator.NextSlice();
    }

}

/**
 * Printself Method
 */
template <class TInputImage, class TInputPath, class TOutputImage>
void
RemoveCarvingPathFilter<TInputImage, TInputPath, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Path Value: " << m_Value << std::endl;
}

template <class TInputImage, class TInputPath, class TOutputImage>
void
RemoveCarvingPathFilter<TInputImage, TInputPath, TOutputImage>
::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  typename Superclass::InputImageConstPointer inputPtr = this->GetInput();
  typename Superclass::OutputImagePointer     outputPtr = this->GetOutput();

  unsigned int dir0;
  //unsigned int dir1;

  if (m_Direction == 0)
    {
    dir0 = 0;
    //dir1 = 1;
    }
  else
    {
    dir0 = 1;
    //dir1 = 0;
    }

  // we need to compute the output spacing, the output image size, and the
  // output image start index
  const typename TInputImage::SpacingType&
                                        inputSpacing = inputPtr->GetSignedSpacing();
  const typename TInputImage::SizeType& inputSize
    = inputPtr->GetLargestPossibleRegion().GetSize();
  const typename TInputImage::IndexType& inputStartIndex
    = inputPtr->GetLargestPossibleRegion().GetIndex();

  typename TOutputImage::SpacingType outputSpacing;
  typename TOutputImage::SizeType    outputSize;
  typename TOutputImage::IndexType   outputStartIndex;

  for (unsigned int i = 0; i < TOutputImage::ImageDimension; ++i)
    {
    outputSpacing[i] = inputSpacing[i];
    outputSize[i] = (unsigned int) (inputSize[i]);
    outputStartIndex[i] = inputStartIndex[i];
    }
  // we remove one column of the image
  outputSize[dir0] = inputSize[dir0] - 1;

  outputPtr->SetSignedSpacing(outputSpacing);

  typename TOutputImage::RegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize(outputSize);
  outputLargestPossibleRegion.SetIndex(outputStartIndex);

  outputPtr->SetLargestPossibleRegion(outputLargestPossibleRegion);
}

} // end namespace otb

#endif
