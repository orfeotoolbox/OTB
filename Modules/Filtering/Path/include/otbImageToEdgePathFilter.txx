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
#ifndef __otbImageToEdgePathFilter_txx
#define __otbImageToEdgePathFilter_txx

#include "otbImageToEdgePathFilter.h"
#include "otbMacro.h"
#include "itkConstantPadImageFilter.h"
#include "itkConstShapedNeighborhoodIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace otb
{
/*
 * Constructor.
 */
template <class TInputImage, class TOutputPath>
ImageToEdgePathFilter<TInputImage, TOutputPath>
::ImageToEdgePathFilter()
{
  m_ForegroundValue = PixelType(255);
}
/**
 * Main computation method.
 */
template <class TInputImage, class TOutputPath>
void
ImageToEdgePathFilter<TInputImage, TOutputPath>
::GenerateData(void)
{

  const InputImageType * inputImage = this->GetInput();
  OutputPathType *       outputPath       = this->GetOutput();

  otbMsgDebugMacro(<< "Foreground value : " << m_ForegroundValue);

  PixelType initPadConstant(0);
  if (initPadConstant == m_ForegroundValue)
    {
    initPadConstant = 1;
    }

  typedef itk::ConstantPadImageFilter<InputImageType, InputImageType> PadFilterType;
  typedef itk::ConstShapedNeighborhoodIterator<InputImageType>        IteratorType;
  typedef itk::ImageRegionConstIteratorWithIndex<InputImageType>      LinearIteratorType;

  // Padding to deal with near the border objects.
  typename PadFilterType::Pointer pad = PadFilterType::New();
  pad->SetInput(inputImage);
  pad->SetConstant(initPadConstant);
  long unsigned int padSize[2] = {1, 1};
  pad->SetPadUpperBound(padSize);
  pad->SetPadLowerBound(padSize);
  pad->Update();
  // Iterate on the image to get a starting point
  LinearIteratorType linIter(pad->GetOutput(), pad->GetOutput()->GetLargestPossibleRegion());
  linIter.GoToBegin();
  bool flag = true;
  while (flag && !linIter.IsAtEnd())
    {
    if (linIter.Get() == m_ForegroundValue)
      {
      flag = false;
      }
    else
      {
      ++linIter;
      }
    }
  typename InputImageType::IndexType start = linIter.GetIndex();
//  outputPath->AddVertex(start);

  // Neighborhood definition
  typename IteratorType::RadiusType radius;
  radius.Fill(1);
  IteratorType it(radius, pad->GetOutput(), pad->GetOutput()->GetLargestPossibleRegion());
  const typename IteratorType::OffsetType LEFT   = {{-1, 0}};
  const typename IteratorType::OffsetType RIGHT  = {{1, 0}};
  const typename IteratorType::OffsetType UP     = {{0, -1}};
  const typename IteratorType::OffsetType DOWN   = {{0, 1}};
  const typename IteratorType::OffsetType LEFTUP   = {{-1, -1}};
  const typename IteratorType::OffsetType RIGHTDOWN = {{1, 1}};
  const typename IteratorType::OffsetType RIGHTUP  = {{1, -1}};
  const typename IteratorType::OffsetType LEFTDOWN = {{-1, 1}};
  const typename IteratorType::OffsetType CENTER = {{0, 0}};
  it.ClearActiveList();
  it.ActivateOffset(LEFT);
  it.ActivateOffset(RIGHT);
  it.ActivateOffset(UP);
  it.ActivateOffset(DOWN);
  it.ActivateOffset(CENTER);
  it.ActivateOffset(RIGHTUP);
  it.ActivateOffset(RIGHTDOWN);
  it.ActivateOffset(LEFTUP);
  it.ActivateOffset(LEFTDOWN);
  // The rotation vector allows getting the clock-wise next pixel
  std::vector<typename IteratorType::OffsetType> rotation;
  rotation.push_back(RIGHT);
  rotation.push_back(RIGHTDOWN);
  rotation.push_back(DOWN);
  rotation.push_back(LEFTDOWN);
  rotation.push_back(LEFT);
  rotation.push_back(LEFTUP);
  rotation.push_back(UP);
  rotation.push_back(RIGHTUP);
  // Set up the iterator
  it.SetLocation(start);

  ContinuousIndexType newVertex = it.GetIndex(CENTER);
  if (it.GetPixel(RIGHT) == m_ForegroundValue) newVertex[0] -= 0.5;
  if (it.GetPixel(LEFT) == m_ForegroundValue) newVertex[0] += 0.5;
  if (it.GetPixel(UP) == m_ForegroundValue) newVertex[1] += 0.5;
  if (it.GetPixel(DOWN) == m_ForegroundValue) newVertex[1] -= 0.5;
  outputPath->AddVertex(newVertex);

  otbMsgDebugMacro(<< "START: " << start);
  // stopping flag
  flag = true;
  int nbMove = 0;
  // nexstart gives a clue of where to begin searching in next step of the search
  int nextStart = 0;
  // While the search has not eended
  while (flag)
    {
    // move is used to walk the neighnorhood clock-wise
    int move = nextStart;
    // edgeFound indicate that the edge has been found.
    bool EdgeFound = false;
    // LastWasPositive indicate wether the previous pixel belong to the object or not
    bool LastWasPositive(false);
    // While unexplored pixels remain and no edge was found
    while ((move < nextStart + 8) && (!EdgeFound))
      {
      //otbMsgDevMacro(<<"SEARCH: "<<move%8<<" "<<it.GetPixel(rotation[move%8])<<" LAST: "<<LastWasPositive);
      // If last pixel was not in the object and the current is, we have found the edge
      if ((!LastWasPositive) && (it.GetPixel(rotation[move % 8]) == m_ForegroundValue))
        {
        EdgeFound = true;
        }
      else
        {
        //  Else goes on
        LastWasPositive = (it.GetPixel(rotation[move % 8]) == m_ForegroundValue);
        move++;
        }
      }
    // Once the search has been completed, if an edge pixel was found
    if (EdgeFound)
      {
      // Update the output path
      it += rotation[move % 8];
      nextStart = (move + 5) % 8;
      newVertex = it.GetIndex(CENTER);
      if (it.GetPixel(RIGHT) == m_ForegroundValue) newVertex[0] -= 0.5;
      if (it.GetPixel(LEFT) == m_ForegroundValue) newVertex[0] += 0.5;
      if (it.GetPixel(UP) == m_ForegroundValue) newVertex[1] += 0.5;
      if (it.GetPixel(DOWN) == m_ForegroundValue) newVertex[1] -= 0.5;
      outputPath->AddVertex(newVertex);
      otbMsgDebugMacro(<< newVertex);
      // If we came back to our start point after a sufficient number of moves
      if ((it.GetIndex(CENTER) == start) && (nbMove >= 2))
        {
        // search end
        flag = false;
        }
      else
        {
        // else
        for (int i = 0; i < 8; ++i)
          {
          // If we came back near our starting pointer after a sufficient number of moves
          if ((it.GetIndex(rotation[i]) == start) && (nbMove >= 2))
            {
            // search end
            flag = false;
            }
          }
        }
      }
    // else
    else
      {
      // search ended, no pixel can be added to the edge path.
      flag = false;
      }
    nbMove++;
    }
}
template <class TInputImage, class TOutputPath>
void
ImageToEdgePathFilter<TInputImage, TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << "Foreground value : " << m_ForegroundValue << std::endl;
}
} // end namespace otb

#endif
