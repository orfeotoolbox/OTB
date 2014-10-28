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
#ifndef __otbImageFittingPolygonListFilter_txx
#define __otbImageFittingPolygonListFilter_txx

#include "otbImageFittingPolygonListFilter.h"
#include "otbPolyLineImageConstIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace otb
{
/**
 * Constructor
 */
template <class TPath, class TImage>
ImageFittingPolygonListFilter<TPath, TImage>
::ImageFittingPolygonListFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredInputs(2);
  m_Radius = 1;
  m_NumberOfIterations = 1;
}

template <class TPath, class TImage>
void
ImageFittingPolygonListFilter<TPath, TImage>
::SetInputImage(const ImageType * image)
{
  this->itk::ProcessObject::SetNthInput(1, const_cast<ImageType *>(image));
}

template <class TPath, class TImage>
const typename ImageFittingPolygonListFilter<TPath, TImage>
::ImageType *
ImageFittingPolygonListFilter<TPath, TImage>
::GetInputImage(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }
  return static_cast<const ImageType *>(this->itk::ProcessObject::GetInput(1));
}

//FIXME
//There is an issue here with integer and continous indexes
//maybe we should use the itk::LineConstIterator
template <class TPath, class TImage>
void
ImageFittingPolygonListFilter<TPath, TImage>
::GenerateData()
{
  // I/O wiring
  ImageConstPointerType inputImagePtr = this->GetInputImage();
  const PathListType *  inputPtr  = this->GetInput();
  PathListType *        outputPtr = this->GetOutput();

  typename ImageType::RegionType regionLargest = inputImagePtr->GetLargestPossibleRegion();

  typedef itk::ImageRegionConstIteratorWithIndex<ImageType> NeighborhoodIteratorType;

  typename ImageType::SizeType size;
  size[0] = 2 * m_Radius + 1;
  size[1] = 2 * m_Radius + 1;
  typename ImageType::RegionType region;
  region.SetSize(size);
  typename ImageType::IndexType start;

  //go through all the polygons in the list
  IteratorType it = inputPtr->Begin();
  while (it != inputPtr->End())
    {
    PathPointerType polygon = it.Get();
    if (polygon->GetVertexList()->Size() > 2)
      {
      for (unsigned int iteration = 0; iteration < m_NumberOfIterations; ++iteration)
        {
        PathPointerType             newPolygon = PathType::New();
        VertexListConstIteratorType vertexIt = polygon->GetVertexList()->Begin();
        //We are now going to go through all the vertex, we won't start to process
        // first as we need to know the last one for that.
        VertexType firstPoint = vertexIt.Value();
        VertexType previousPoint = vertexIt.Value();
        ++vertexIt;
        VertexType currentPoint = vertexIt.Value();
        ++vertexIt;
        while (vertexIt != polygon->GetVertexList()->End())
          {
          VertexType nextPoint = vertexIt.Value();
          /** try all the possible neighbor for the current point
           * to factorize
           * */
            {

            start[0] = static_cast<long int>(currentPoint[0] - m_Radius);
            start[1] = static_cast<long int>(currentPoint[1] - m_Radius);
            region.SetIndex(start);
            region.Crop(inputImagePtr->GetLargestPossibleRegion());

            NeighborhoodIteratorType nIt(inputImagePtr, region);
            double     maxValue = 0.0;
            VertexType maxPoint = currentPoint;
            nIt.GoToBegin();
            while (!nIt.IsAtEnd())
              {
              if (regionLargest.IsInside(nIt.GetIndex()))
                {
                VertexType middlePoint = static_cast<VertexType>(nIt.GetIndex());
                double     currentValue = computeValue(inputImagePtr, middlePoint, previousPoint, nextPoint);
                if (currentValue > maxValue)
                  {
                  maxValue = currentValue;
                  maxPoint = middlePoint;
                  }
                }
              ++nIt;
              }
            currentPoint = maxPoint;
            newPolygon->AddVertex(maxPoint);
            }
          /** End 'to factorize' */

          ++vertexIt;
          previousPoint = currentPoint;
          currentPoint = nextPoint;

          }
        //We now need to process the last and the first point

        VertexType nextPoint = firstPoint;
        /** try all the possible neighbor for the current point
         * to factorize
         * */
          {
          start[0] = static_cast<long int>(currentPoint[0] - m_Radius);
          start[1] = static_cast<long int>(currentPoint[1] - m_Radius);
          region.SetIndex(start);

          NeighborhoodIteratorType nIt(inputImagePtr, region);
          double     maxValue = 0.0;
          VertexType maxPoint = currentPoint;
          nIt.GoToBegin();

          while (!nIt.IsAtEnd())
            {
            if (regionLargest.IsInside(nIt.GetIndex()))
              {
              VertexType middlePoint = static_cast<VertexType>(nIt.GetIndex());
              double     currentValue = computeValue(inputImagePtr, middlePoint, previousPoint, nextPoint);
              if (currentValue > maxValue)
                {
                maxValue = currentValue;
                maxPoint = middlePoint;
                }
              }
            ++nIt;
            }
          currentPoint = maxPoint;
          newPolygon->AddVertex(maxPoint);
          }
        /** End 'to factorize' */

        previousPoint = currentPoint;
        currentPoint = firstPoint;
        vertexIt = newPolygon->GetVertexList()->Begin();
        nextPoint = vertexIt.Value();

        /** try all the possible neighbor for the current point
        * to factorize
        * */
          {

          start[0] = static_cast<long int>(currentPoint[0] - m_Radius);
          start[1] = static_cast<long int>(currentPoint[1] - m_Radius);
          region.SetIndex(start);

          NeighborhoodIteratorType nIt(inputImagePtr, region);
          double     maxValue = 0.0;
          VertexType maxPoint = currentPoint;
          nIt.GoToBegin();
          while (!nIt.IsAtEnd())
            {
            if (regionLargest.IsInside(nIt.GetIndex()))
              {
              VertexType middlePoint = static_cast<VertexType>(nIt.GetIndex());
              double     currentValue = computeValue(inputImagePtr, middlePoint, previousPoint, nextPoint);
              if (currentValue > maxValue)
                {
                maxValue = currentValue;
                maxPoint = middlePoint;
                }
              }
            ++nIt;
            }
          currentPoint = maxPoint;
          newPolygon->AddVertex(maxPoint);
          }
        /** End 'to factorize' */

        polygon = newPolygon; //prepare the next iteration
        }
      }

    outputPtr->PushBack(polygon);

    ++it;
    } //going through the polygon list
}

template <class TPath, class TImage>
double
ImageFittingPolygonListFilter<TPath, TImage>
::computeValue(ImageConstPointerType image, VertexType middlePoint, VertexType previousPoint,
               VertexType nextPoint) const
{
  typedef typename ImageType::IndexType IndexType;
  IndexType middleIndex;
  IndexType previousIndex;
  IndexType nextIndex;
  middleIndex[0] = static_cast<long int>(middlePoint[0]);
  middleIndex[1] = static_cast<long int>(middlePoint[1]);
  previousIndex[0] = static_cast<long int>(previousPoint[0]);
  previousIndex[1] = static_cast<long int>(previousPoint[1]);
  nextIndex[0] = static_cast<long int>(nextPoint[0]);
  nextIndex[1] = static_cast<long int>(nextPoint[1]);
  double       currentValue = 0.0;
  unsigned int count = 0;
  //compute for first segment
  LineConstIteratorType itLineFirst(image, previousIndex, middleIndex);
  while (!itLineFirst.IsAtEnd())
    {
    currentValue += itLineFirst.Get();
    ++count;
    ++itLineFirst;
    }

  //compute for second segment
  LineConstIteratorType itLineSecond(image, nextIndex, middleIndex);
  while (!itLineSecond.IsAtEnd())
    {
    currentValue += itLineSecond.Get();
    ++count;
    ++itLineSecond;
    }

  return currentValue / count;
}

/**
 * PrintSelf Method
 */
template <class TPath, class TImage>
void
ImageFittingPolygonListFilter<TPath, TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // End namespace otb
#endif
