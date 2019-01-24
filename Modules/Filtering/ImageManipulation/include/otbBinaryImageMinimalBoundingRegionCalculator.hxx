/*
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

#ifndef otbBinaryImageMinimalBoundingRegionCalculator_hxx
#define otbBinaryImageMinimalBoundingRegionCalculator_hxx

#include "otbBinaryImageMinimalBoundingRegionCalculator.h"
#include "itkImageSliceConstIteratorWithIndex.h"
#include "otbMacro.h"

namespace otb
{
/*
 * Constructor
 */
template <class TInputImage>
BinaryImageMinimalBoundingRegionCalculator<TInputImage>
::BinaryImageMinimalBoundingRegionCalculator()
{
  // The pad option is desactivated by default
  m_Pad = 0;
  // Set the default region
  typename InputImageType::SizeType  size;
  typename InputImageType::IndexType index;
  size[0] = 0;
  size[1] = 0;
  index[0] = 0;
  index[1] = 0;
  m_Region.SetSize(size);
  m_Region.SetIndex(index);
  m_InsideValue = static_cast<PixelType>(255);
}
/**
 * Main computation method
 */
template <class TInputImage>
void
BinaryImageMinimalBoundingRegionCalculator<TInputImage>
::GenerateData(void)
{
  // Input images pointers
  const InputImageType* image = this->GetInput();
  // Iterator definition
  typedef itk::ImageSliceConstIteratorWithIndex<InputImageType> SliceIteratorType;
  // Indexes containing upper-left and lower-right corner
  typename InputImageType::IndexType min;
  typename InputImageType::IndexType max;
  min[0] = 0;
  min[1] = 0;
  max[1] = 0;
  max[1] = 0;
  for (unsigned int axis = 0; axis < InputImageType::ImageDimension; ++axis)
    { // Create the forward iterator to find lower bound
    SliceIteratorType fit(image, image->GetLargestPossibleRegion());

    fit.SetFirstDirection(!axis);
    fit.SetSecondDirection(axis);
    fit.GoToBegin();

    // Walk through the two images line by line
    while (!fit.IsAtEnd())
      {
      while (!fit.IsAtEndOfSlice())
        {
        while (!fit.IsAtEndOfLine())
          {
          // If a common intersection is found
          if (fit.Get() == m_InsideValue)
            {
            // then the lower bound is found
            min[axis] = fit.GetIndex()[axis];
            fit.GoToReverseBegin(); // skip to the end
            break;
            }
          ++fit;
          }
        fit.NextLine();
        }
      fit.NextSlice();
      }
    // Create the reverse iterator to find upper bound
    SliceIteratorType rit(image, image->GetLargestPossibleRegion());
    rit.SetFirstDirection(!axis);
    rit.SetSecondDirection(axis);
    rit.GoToReverseBegin();
    // Walk through the two images line by line
    while (!rit.IsAtReverseEnd())
      {
      while (!rit.IsAtReverseEndOfSlice())
        {
        while (!rit.IsAtReverseEndOfLine())
          {
          // If a common intersection is found
          if (rit.Get() == m_InsideValue)
            {
            max[axis] = rit.GetIndex()[axis];
            rit.GoToBegin(); //Skip to reverse end
            break;
            }
          --rit;

          }
        rit.PreviousLine();

        }
      rit.PreviousSlice();
      }
    }

  typename InputImageType::SizeType  size;
  typename InputImageType::IndexType index;

  for (unsigned int i = 0; i < InputImageType::ImageDimension; ++i)
    {
    size[i] = max[i] - min[i] + 1;
    index[i] = min[i];
    }

  m_Region.SetIndex(index);
  m_Region.SetSize(size);

  if (m_Pad)
    {
    m_Region.PadByRadius(m_Pad);
    }

  m_Region.Crop(image->GetLargestPossibleRegion());
}
/**
 * PrintSelf method
 */
template <class TInputImage>
void
BinaryImageMinimalBoundingRegionCalculator<TInputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb

#endif
