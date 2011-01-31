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
#ifndef __otbImageToCarvingPathFilter_txx
#define __otbImageToCarvingPathFilter_txx

#include "otbImageToCarvingPathFilter.h"
#include "otbMacro.h"
#include "itkConstantPadImageFilter.h"
#include "itkConstShapedNeighborhoodIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkImageLinearConstIteratorWithIndex.h"

namespace otb
{
/*
 * Constructor.
 */
template <class TInputImage, class TOutputPath>
ImageToCarvingPathFilter<TInputImage, TOutputPath>
::ImageToCarvingPathFilter()
{
  m_ForegroundValue = PixelType(255);
  m_Direction = 0;
  m_EnergyPerPix = 0.0;
}

/**
 * Main computation method.
 */
template <class TInputImage, class TOutputPath>
void
ImageToCarvingPathFilter<TInputImage, TOutputPath>
::GenerateData(void)
{

  const InputImageType * inputImage = this->GetInput();
  OutputPathType *       outputPath       = this->GetOutput();

  typedef itk::ImageSliceConstIteratorWithIndex
  <InputImageType> IteratorType;
  typedef itk::NeighborhoodIterator<InputImageType>
  NeighborhoodIteratorType;

  IteratorType it(inputImage,
                  inputImage->GetLargestPossibleRegion());
  it.GoToBegin();

  PixelType maxValue = itk::NumericTraits<PixelType>::max();

  typename InputImageType::Pointer energyImage = InputImageType::New();
  energyImage->SetRegions(inputImage->GetLargestPossibleRegion());
  energyImage->Allocate();

  typename NeighborhoodIteratorType::RadiusType radius;
  radius.Fill(1);
  NeighborhoodIteratorType neighIt(radius, energyImage,
                                   energyImage->GetLargestPossibleRegion());
  neighIt.GoToBegin();

  unsigned int dir0;
  unsigned int dir1;

  typename IteratorType::OffsetType UP0;
  typename IteratorType::OffsetType UP1;
  typename IteratorType::OffsetType UP2;
  typename IteratorType::OffsetType NEXT;

  const typename IteratorType::OffsetType LEFT   = {{-1, 0}};
  const typename IteratorType::OffsetType RIGHT  = {{1, 0}};
  const typename IteratorType::OffsetType UP     = {{0, -1}};
  const typename IteratorType::OffsetType DOWN   = {{0, 1}};
  const typename IteratorType::OffsetType LEFTUP   = {{-1, -1}};
//    const typename IteratorType::OffsetType RIGHTDOWN ={{1, 1}};
  const typename IteratorType::OffsetType RIGHTUP  = {{1, -1}};
  const typename IteratorType::OffsetType LEFTDOWN = {{-1, 1}};
//    const typename IteratorType::OffsetType CENTER ={{0, 0}};

  if (m_Direction == 0)
    {
    dir0 = 0;
    dir1 = 1;
    UP0 = LEFTUP;
    UP1 = UP;
    UP2 = RIGHTUP;
    NEXT = DOWN;
    }
  else
    {
    dir0 = 1;
    dir1 = 0;
    UP0 = LEFTUP;
    UP1 = LEFT;
    UP2 = LEFTDOWN;
    NEXT = RIGHT;
    }

  /** Compute the energy image top-down or left-right
   * with cumulative energy (dynamic programming first step) */
  it.SetFirstDirection(dir0);
  it.SetSecondDirection(dir1);
  it.GoToBegin();
  neighIt.GoToBegin();
  while (!it.IsAtEnd())
    {
    while (!it.IsAtEndOfSlice())
      {
      while (!it.IsAtEndOfLine())
        {
        neighIt.SetLocation(it.GetIndex()); //TODO bad for performances... find a better option
        // this is really about 20% of total processing time !!!

        PixelType min = maxValue;
        bool      isInside = false;
        bool      flag = false;
        if (neighIt.GetPixel(UP0, isInside) < min)
          {
          if (isInside)
            {
            min = neighIt.GetPixel(UP0);
            flag = true;
            }
          }
        if (neighIt.GetPixel(UP1, isInside) < min)
          {
          if (isInside)
            {
            min = neighIt.GetPixel(UP1);
            flag = true;
            }
          }
        if (neighIt.GetPixel(UP2, isInside) < min)
          {
          if (isInside)
            {
            min = neighIt.GetPixel(UP2);
            flag = true;
            }
          }

        if (!flag) //no previous value
          {
          min = 0;
          }

        PixelType newValue = it.Get() + min;
        neighIt.SetCenterPixel(newValue);

        ++it;

        }
      it.NextLine();
      }
    it.NextSlice();
    }

  /** Follow the minima bottom-up or right-left
   * (dynamic programming second step) */

  //find the starting point to follow on the last line
  typedef itk::ImageLinearConstIteratorWithIndex<InputImageType>
  LinearIteratorType;

  LinearIteratorType LinIt(energyImage,
                           energyImage->GetLargestPossibleRegion());
  LinIt.SetDirection(dir0);
  LinIt.GoToReverseBegin();
  typedef typename InputImageType::IndexType IndexType;

  PixelType min = maxValue;
  IndexType indexToAdd;
  while (!LinIt.IsAtReverseEndOfLine())
    {
    if (LinIt.Get() < min)
      {
      min = LinIt.Get();
      indexToAdd = LinIt.GetIndex();
      }
    --LinIt;
    }
  outputPath->AddVertex(indexToAdd);
  m_EnergyPerPix = ((double) min) / (inputImage->GetLargestPossibleRegion().GetSize())[dir1];

  //follow the min
  neighIt.SetLocation(indexToAdd);
  bool flag = true;
  while (flag)
    {
    flag = false;
    PixelType min = maxValue;
    IndexType indexToAdd;
    bool      isInside = false;

    if (neighIt.GetPixel(UP0, isInside) < min)
      {
      if (isInside)
        {
        min = neighIt.GetPixel(UP0);
        indexToAdd = neighIt.GetIndex(UP0);
        flag = true;
        }
      }

    if (neighIt.GetPixel(UP1, isInside) < min)
      {
      if (isInside)
        {
        min = neighIt.GetPixel(UP1);
        indexToAdd = neighIt.GetIndex(UP1);
        flag = true;
        }
      }

    if (neighIt.GetPixel(UP2, isInside) < min)
      {
      if (isInside)
        {
        min = neighIt.GetPixel(UP2);
        indexToAdd = neighIt.GetIndex(UP2);
        flag = true;
        }
      }

    if (flag)
      {
      outputPath->AddVertex(indexToAdd);
      neighIt.SetLocation(indexToAdd);
      }

    }

}

template <class TInputImage, class TOutputPath>
void
ImageToCarvingPathFilter<TInputImage, TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << "Foreground value : " << m_ForegroundValue << std::endl;
}
} // end namespace otb

#endif
