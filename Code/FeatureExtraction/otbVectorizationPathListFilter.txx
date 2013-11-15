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
#ifndef __otbVectorizationPathListFilter_txx
#define __otbVectorizationPathListFilter_txx

#include "otbVectorizationPathListFilter.h"
#include "otbMacro.h"
#include "otbMath.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputModulus, class TInputDirection, class TOutputPath>
VectorizationPathListFilter<TInputModulus, TInputDirection, TOutputPath>
::VectorizationPathListFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredInputs(2);
  m_AmplitudeThreshold = 1.0;
}

template <class TInputModulus, class TInputDirection, class TOutputPath>
void
VectorizationPathListFilter<TInputModulus, TInputDirection, TOutputPath>
::SetInput(InputModulusType * inputModulus)
{
  this->itk::ProcessObject::SetNthInput(0, const_cast<InputModulusType *>(inputModulus));
}

template <class TInputModulus, class TInputDirection, class TOutputPath>
const typename VectorizationPathListFilter<TInputModulus, TInputDirection, TOutputPath>
::InputModulusType *
VectorizationPathListFilter<TInputModulus, TInputDirection, TOutputPath>
::GetInput(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  return static_cast<const TInputModulus*>(this->itk::ProcessObject::GetInput(0));
}

template <class TInputModulus, class TInputDirection, class TOutputPath>
void
VectorizationPathListFilter<TInputModulus, TInputDirection, TOutputPath>
::SetInputDirection(InputDirectionType * inputDirection)
{
  this->itk::ProcessObject::SetNthInput(1, const_cast<InputDirectionType *>(inputDirection));
}

template <class TInputModulus, class TInputDirection, class TOutputPath>
const typename VectorizationPathListFilter<TInputModulus, TInputDirection, TOutputPath>
::InputDirectionType *
VectorizationPathListFilter<TInputModulus, TInputDirection, TOutputPath>
::GetInputDirection(void)
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }
  return static_cast<const TInputDirection *>(this->itk::ProcessObject::GetInput(1));
}

/**
 * Main computation method
 */
template <class TInputModulus, class TInputDirection, class TOutputPath>
void
VectorizationPathListFilter<TInputModulus, TInputDirection, TOutputPath>
::GenerateData(void)
{
  InputModulusConstPointerType   modPtr = this->GetInput();
  InputDirectionConstPointerType dirPtr = this->GetInputDirection();
  OutputPathListPointerType      outPtr = this->GetOutput();

  typedef typename OffsetVectorType::iterator OffsetIteratorType;

  RadiusType radius;
  radius.Fill(2);
  OffsetVectorType offsetVector;

  // Creation of the flag image
  FlagImagePointerType flagImage = FlagImageType::New();
  flagImage->SetRegions(modPtr->GetLargestPossibleRegion());
  flagImage->Allocate();
  flagImage->FillBuffer(false);

  // Iterators instantiation
  ModRegionIteratorType modIt(modPtr, modPtr->GetLargestPossibleRegion());
  DirRegionIteratorType dirIt(dirPtr, dirPtr->GetLargestPossibleRegion());
  FlagRegionIteratorType flagIt(flagImage, flagImage->GetLargestPossibleRegion());

  for (modIt.GoToBegin(), dirIt.GoToBegin(), flagIt.GoToBegin();
       (!modIt.IsAtEnd()) && (!dirIt.IsAtEnd()) && (!flagIt.IsAtEnd());
       ++modIt, ++dirIt, ++flagIt)
    {
    if ((modIt.Get() > m_AmplitudeThreshold) && (!flagIt.Get()))
      {
      //this is a beginning, to follow in two directions
      OutputPathPointerType pathTempDirect = OutputPathType::New();
      OutputPathPointerType pathTempReverse = OutputPathType::New();
      OutputPathPointerType path = OutputPathType::New();

      bool   flagFinish;
      int    flagReverse = 0;
      double totalAmplitude = 0;

      ModNeighborhoodIteratorType nModIt(radius, modPtr, modPtr->GetLargestPossibleRegion());
      DirNeighborhoodIteratorType nDirIt(radius, dirPtr, dirPtr->GetLargestPossibleRegion());
      FlagNeighborhoodIteratorType nFlagIt(radius, flagImage, flagImage->GetLargestPossibleRegion());

      for (flagReverse = 0; flagReverse < 2; ++flagReverse)
        {
        nModIt.SetLocation(modIt.GetIndex());
        nDirIt.SetLocation(dirIt.GetIndex());
        nFlagIt.SetLocation(flagIt.GetIndex());
        // temporary point
        PointType  point;
        VertexType vertex;
        modPtr->TransformIndexToPhysicalPoint(nModIt.GetIndex(), point);
        modPtr->TransformPhysicalPointToContinuousIndex(point, vertex);
        if (flagReverse == 0)
          {
          flagIt.Set(true);

          //  otbMsgDebugMacro(<<"Adding new vertex: "<<vertex);

          pathTempDirect->AddVertex(vertex);
          }
        flagFinish = false;
        while (!flagFinish)
          {
          offsetVector = GetThreeNeighborOffsetFromDirection(nDirIt.GetCenterPixel(), flagReverse);
          OffsetIteratorType vecIt = offsetVector.begin();
          bool               flagFound = false;
          while (vecIt != offsetVector.end() && !flagFound)
            {
            flagFound = nModIt.GetPixel(*vecIt) > 0
                        && !nFlagIt.GetPixel(*vecIt);
            ++vecIt;
            }
          if (flagFound)
            {
            point.Fill(0);
            PointType tmpPoint;
            totalAmplitude = 0;
            for (vecIt = offsetVector.begin(); vecIt != offsetVector.end(); ++vecIt)
              {
              double currentAmplitude = nModIt.GetPixel(*vecIt);
              modPtr->TransformIndexToPhysicalPoint(nModIt.GetIndex(*vecIt), tmpPoint);
              point[0] += currentAmplitude * tmpPoint[0];
              point[1] += currentAmplitude * tmpPoint[1];
              totalAmplitude += currentAmplitude;
              }
            point[0] = point[0] / totalAmplitude;
            point[1] = point[1] / totalAmplitude;
            modPtr->TransformPhysicalPointToContinuousIndex(point, vertex);
            if (flagReverse == 0)
              {
//               otbMsgDevMacro(<<"Adding new vertex (direct): "<<vertex);

              pathTempDirect->AddVertex(vertex);
              }
            else
              {

//               otbMsgDevMacro(<<"Adding new vertex (reverse): "<<vertex);

              pathTempReverse->AddVertex(vertex);
              }
            // flag the pixel use
            nFlagIt.SetCenterPixel(true);
            //update the neighbor iterators so they are centered on the nearest pixel to the barycenter
            IndexType newIndex;
            if (modPtr->TransformPhysicalPointToIndex(point, newIndex))
              {
//              otbMsgDevMacro(<<"Moving to new center: " << newIndex);
              nModIt.SetLocation(newIndex);
              nDirIt.SetLocation(newIndex);
              nFlagIt.SetLocation(newIndex);

              if (nModIt.GetCenterPixel() == 0)
                {
                //we need to check that in case the barycenter is out...
                flagFinish = true;
                }
              if (nFlagIt.GetCenterPixel())
                {
                //we don't want to go back to the same pixels
                flagFinish = true;
                }
              }
            else
              {
              //new point outside image
              flagFinish = true;
              }
            }
          else
            {
            flagFinish = true;
            }
          }
        }
      VertexListPointerType vertexDirect = pathTempDirect->GetVertexList();
      VertexListPointerType vertexReverse = pathTempReverse->GetVertexList();

      unsigned int numberVertex = 0;

      VertexIteratorType vertexReverseIt = vertexReverse->End();
      if (vertexReverseIt != vertexReverse->Begin())
        {
        --vertexReverseIt;
        while (vertexReverseIt != vertexReverse->Begin())
          {
          path->AddVertex(vertexReverseIt.Value());
          ++numberVertex;
          --vertexReverseIt;
          }
        path->AddVertex(vertexReverseIt.Value());
        }

      VertexIteratorType vertexDirectIt = vertexDirect->Begin();
      while (vertexDirectIt != vertexDirect->End())
        {
        path->AddVertex(vertexDirectIt.Value());
        ++vertexDirectIt;
        ++numberVertex;
        }

      // otbMsgDebugMacro(<<"Path number of vertices: "<<numberVertex);

      if (numberVertex > 3)
        {
        outPtr->PushBack(path);
        }
      }
    }
}

/**
 * Compute the 8 neighbors to explore from the direction and the type of search (forward or backward).
 * \param direction The direction
 * \param flagReverse The type of search
 * \return The neighborhood
 */
template <class TInputModulus, class TInputDirection, class TOutputPath>
typename VectorizationPathListFilter<TInputModulus, TInputDirection, TOutputPath>
::OffsetVectorType
VectorizationPathListFilter<TInputModulus, TInputDirection, TOutputPath>
::GetEightNeighborOffsetFromDirection(double direction, unsigned int flagReverse)
{
  int              neighborhoodNumber = 0;
  OffsetVectorType offset;
  offset.reserve(8);
  if (direction > 0)
    {
    //find the direction in terms of 0, 1, 2, 3
    neighborhoodNumber = (int) (direction / (CONST_PI_4) -1);
    }
  else
    {
    neighborhoodNumber = (int) ((direction + CONST_PI) / (CONST_PI_4) -1);
    neighborhoodNumber = (neighborhoodNumber + 4);
    //if the direction was <0 need to convert to 4, 5, 6, 7
    }
  if (flagReverse)
    {
    //if the reverse flag is activated we need to look on the other side
    neighborhoodNumber = (neighborhoodNumber + 4) % 8;
    }
  OffsetType tmpOffset;
  switch (neighborhoodNumber)
    {
    case 0:
      tmpOffset[0] = 1;
      tmpOffset[1] = 0;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 1;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 0;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);

      tmpOffset[0] = 2;
      tmpOffset[1] = 0;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 2;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 2;
      tmpOffset[1] = 2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 1;
      tmpOffset[1] = 2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 0;
      tmpOffset[1] = 2;
      offset.push_back(tmpOffset);

      break;

    case 1:
      tmpOffset[0] = 1;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 0;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -1;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);

      tmpOffset[0] = 2;
      tmpOffset[1] = 2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 1;
      tmpOffset[1] = 2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 0;
      tmpOffset[1] = 2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -1;
      tmpOffset[1] = 2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -2;
      tmpOffset[1] = 2;
      offset.push_back(tmpOffset);
      break;

    case 2:
      tmpOffset[0] = 0;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -1;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -1;
      tmpOffset[1] = 0;
      offset.push_back(tmpOffset);

      tmpOffset[0] = 0;
      tmpOffset[1] = 2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -1;
      tmpOffset[1] = 2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -2;
      tmpOffset[1] = 2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -2;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -2;
      tmpOffset[1] = 0;
      offset.push_back(tmpOffset);
      break;

    case 3:
      tmpOffset[0] = -1;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -1;
      tmpOffset[1] = 0;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -1;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);

      tmpOffset[0] = -2;
      tmpOffset[1] = 2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -2;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -2;
      tmpOffset[1] = 0;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -2;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -2;
      tmpOffset[1] = -2;
      offset.push_back(tmpOffset);
      break;

    case 4:
      tmpOffset[0] = -1;
      tmpOffset[1] = 0;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -1;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 0;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);

      tmpOffset[0] = -2;
      tmpOffset[1] = 0;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -2;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -2;
      tmpOffset[1] = -2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -1;
      tmpOffset[1] = -2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 0;
      tmpOffset[1] = -2;
      offset.push_back(tmpOffset);
      break;

    case 5:
      tmpOffset[0] = -1;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 0;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 1;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);

      tmpOffset[0] = -2;
      tmpOffset[1] = -2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -1;
      tmpOffset[1] = -2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 0;
      tmpOffset[1] = -2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 1;
      tmpOffset[1] = -2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 2;
      tmpOffset[1] = -2;
      offset.push_back(tmpOffset);
      break;

    case 6:
      tmpOffset[0] = 0;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 1;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 1;
      tmpOffset[1] = 0;
      offset.push_back(tmpOffset);

      tmpOffset[0] = 0;
      tmpOffset[1] = -2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 1;
      tmpOffset[1] = -2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 2;
      tmpOffset[1] = -2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 2;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 2;
      tmpOffset[1] = 0;
      offset.push_back(tmpOffset);
      break;

    case 7:
      tmpOffset[0] = 1;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 1;
      tmpOffset[1] = 0;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 1;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);

      tmpOffset[0] = 2;
      tmpOffset[1] = -2;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 2;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 2;
      tmpOffset[1] = 0;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 2;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 2;
      tmpOffset[1] = 2;
      offset.push_back(tmpOffset);
      break;
    }
  return offset;
}

/**
 * Compute the 3 neighbors to explore from the direction and the type of search (forward or backward).
 * \param direction The direction
 * \param flagReverse The type of search
 * \return The neighborhood
 */
template <class TInputModulus, class TInputDirection, class TOutputPath>
typename VectorizationPathListFilter<TInputModulus, TInputDirection, TOutputPath>
::OffsetVectorType
VectorizationPathListFilter<TInputModulus, TInputDirection, TOutputPath>
::GetThreeNeighborOffsetFromDirection(double direction, unsigned int flagReverse)
{
  int              neighborhoodNumber = 0;
  OffsetVectorType offset;
  offset.reserve(3);
  if (direction > 0)
    {
    //find the direction in terms of 0, 1, 2, 3
    neighborhoodNumber = (int) (direction / (CONST_PI_4) -1);
    }
  else
    {
    neighborhoodNumber = (int) ((direction + CONST_PI) / (CONST_PI_4) -1);
    neighborhoodNumber = (neighborhoodNumber + 4);
    //if the direction was <0 need to convert to 4, 5, 6, 7
    }
  if (flagReverse)
    {
    //if the reverse flag is activated we need to look on the other side
    neighborhoodNumber = (neighborhoodNumber + 4) % 8;
    }
  OffsetType tmpOffset;
//  otbMsgDevMacro(<<"Direction: " << neighborhoodNumber)
  switch (neighborhoodNumber)
    {
    case 0:
      tmpOffset[0] = 1;
      tmpOffset[1] = 0;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 1;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 0;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);

      break;

    case 1:
      tmpOffset[0] = 1;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 0;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -1;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);

      break;

    case 2:
      tmpOffset[0] = 0;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -1;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -1;
      tmpOffset[1] = 0;
      offset.push_back(tmpOffset);

      break;

    case 3:
      tmpOffset[0] = -1;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -1;
      tmpOffset[1] = 0;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -1;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);

      break;

    case 4:
      tmpOffset[0] = -1;
      tmpOffset[1] = 0;
      offset.push_back(tmpOffset);
      tmpOffset[0] = -1;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 0;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);

      break;

    case 5:
      tmpOffset[0] = -1;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 0;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 1;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);

      break;

    case 6:
      tmpOffset[0] = 0;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 1;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 1;
      tmpOffset[1] = 0;
      offset.push_back(tmpOffset);

      break;

    case 7:
      tmpOffset[0] = 1;
      tmpOffset[1] = -1;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 1;
      tmpOffset[1] = 0;
      offset.push_back(tmpOffset);
      tmpOffset[0] = 1;
      tmpOffset[1] = 1;
      offset.push_back(tmpOffset);

      break;
    }
  return offset;
}

/**
 * PrintSelf Method
 */
template <class TInputModulus, class TInputDirection, class TOutputPath>
void
VectorizationPathListFilter<TInputModulus, TInputDirection, TOutputPath>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb
#endif
