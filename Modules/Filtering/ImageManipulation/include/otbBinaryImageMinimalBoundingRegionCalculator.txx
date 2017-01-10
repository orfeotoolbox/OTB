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
#ifndef otbBinaryImageMinimalBoundingRegionCalculator_txx
#define otbBinaryImageMinimalBoundingRegionCalculator_txx

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
