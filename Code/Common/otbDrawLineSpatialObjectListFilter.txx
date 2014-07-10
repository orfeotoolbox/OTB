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
#ifndef __otbDrawLineSpatialObjectListFilter_txx
#define __otbDrawLineSpatialObjectListFilter_txx

#include "otbDrawLineSpatialObjectListFilter.h"

#include "itkLineIterator.h"
#include "itkDataObject.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>
::DrawLineSpatialObjectListFilter()
{
  this->SetNumberOfRequiredInputs(2);
  this->SetNumberOfRequiredOutputs(1);

  m_Value = static_cast<OutputPixelType>(255.);
}

template <class TInputImage, class TOutputImage>
void
DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>
::SetInputLineSpatialObjectList(const LinesListType * list)
{
  this->ProcessObjectType::SetNthInput(1,
                                       const_cast<LinesListType *>(list));

}

template <class TInputImage, class TOutputImage>
typename DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>::LinesListType *
DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>
::GetInputLineSpatialObjectList(void)
{
  //ROMAIN
  return static_cast</*const*/ LinesListType*>
           (this->ProcessObjectType::GetInput(1));
}

template <class TInputImage, class TOutputImage>
void
DrawLineSpatialObjectListFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType itkNotUsed(threadId))

{
  typename InputImageType::ConstPointer input  = this->GetInput();
  typename OutputImageType::Pointer     output  = this->GetOutput();
  typename LinesListType::Pointer       list = const_cast<LinesListType*>(this->GetInputLineSpatialObjectList());

  /** Copy the input requested region in the output requested region*/
  typedef itk::ImageRegionIterator<OutputImageType>     OutputIteratorType;
  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;

  OutputIteratorType    outputIt(output, outputRegionForThread);
  InputIteratorType     inputIt(input,  outputRegionForThread);

  outputIt.GoToBegin();
  inputIt.GoToBegin();

  for (outputIt.GoToBegin(); !outputIt.IsAtEnd(); ++outputIt, ++inputIt)
    outputIt.Set(static_cast<OutputPixelType>(inputIt.Get()));

  /** Draw the lines in the output image using lineIterator*/
  typedef itk::LineIterator<OutputImageType> LineIteratorFilter;
  OutputIndexType  indexBeginLine, indexEndLine;
  LineListIterator itList = list->begin();

  typename InputImageType::SizeType size = input->GetLargestPossibleRegion().GetSize();
  m_Length = size[1];
  m_Width  = size[0];

  while (itList != list->end())
    {
    PointListType&                         pointsList = (*itList)->GetPoints();
    typename PointListType::const_iterator itPoints = pointsList.begin();

    indexBeginLine[0] = static_cast<OutputIndexValueType>((*itPoints).GetPosition()[0]);
    indexBeginLine[1] = static_cast<OutputIndexValueType>((*itPoints).GetPosition()[1]);

    ++itPoints;    //Get the second extremity of the segment

    indexEndLine[0] = static_cast<OutputIndexValueType>((*itPoints).GetPosition()[0]);
    indexEndLine[1] = static_cast<OutputIndexValueType>((*itPoints).GetPosition()[1]);

    /** Crop the segment if it is outside the region in the left*/

    if (!(this->IsColumnOutsideOfTheRegion(&indexBeginLine, &indexEndLine,
                                           &outputRegionForThread) &&
          this->IsColumnOutsideOfTheRegion(&indexEndLine, &indexBeginLine, &outputRegionForThread)))
      {
      if (indexEndLine[0] >= static_cast<OutputIndexValueType>(size[0]))
        this->CropRightSegment(&indexEndLine,
                               &indexBeginLine,
                               &outputRegionForThread);

      if (indexBeginLine[0] >= static_cast<OutputIndexValueType>(size[0]))
        this->CropRightSegment(
          &indexBeginLine,
          &indexEndLine,
          &
          outputRegionForThread);
      }

    /**
     * If an extremity is under the region
     * Technically, the X component of the index is inside the image
     */
    if (this->IsDownsideTheImage(&indexBeginLine)  &&
        input->GetLargestPossibleRegion().IsInside(indexEndLine))
      this->CropSegment(&indexBeginLine,
                        &indexEndLine,
                        &outputRegionForThread);

    if (this->IsDownsideTheImage(&indexEndLine) &&
        input->GetLargestPossibleRegion().IsInside(indexBeginLine))
      this->CropSegment(&indexEndLine,
                        &indexBeginLine,
                        &outputRegionForThread);

    /** If the segments are not in the region (upside or downside the region)*/
    if (!(this->IsUpsideTheRegion(&indexBeginLine,
                                  &outputRegionForThread)   &&
          this->IsUpsideTheRegion(&indexEndLine, &outputRegionForThread)) &&
        !(this->IsDownsideTheRegion(&indexBeginLine,
                                    &outputRegionForThread) &&
          this->IsDownsideTheRegion(&indexEndLine, &outputRegionForThread)) &&
        !(this->IsColumnOutsideOfTheRegion(&indexBeginLine, &indexEndLine,
                                           &outputRegionForThread) &&
          this->IsColumnOutsideOfTheRegion(&indexEndLine, &indexBeginLine, &outputRegionForThread))
        )
      {

      /** Instantiation of the line iterator with begin and ending index*/
      LineIteratorFilter   itLine(output, indexBeginLine, indexEndLine);

      /** Iteration over the line and writing white lines */
      while (!itLine.IsAtEnd())
        {
        if (outputRegionForThread.IsInside(itLine.GetIndex())) itLine.Set(m_Value);
        ++itLine;
        }
      }

    ++itList;
    }
}

/**
 * Compute the intersection between the segment to draw and the region belonging to the current thread
 * It is used if the segment abcisse is greater than the width of the image
 *
 */
template <class TInputImage, class TOutput>
void
DrawLineSpatialObjectListFilter<TInputImage, TOutput>
::CropRightSegment(OutputIndexType *indexToCrop,
                   OutputIndexType *otherIndex,
                   const OutputImageRegionType *outputRegionForThread) const

{

  /** Dimensions of the buffered region*/
  typename OutputImageRegionType::SizeType  size  = outputRegionForThread->GetSize();
  //typename OutputImageRegionType::IndexType start = outputRegionForThread->GetIndex();

  /** Equation of the line (Begin, End)*/
  double lengthSegment = -(*otherIndex)[1] + (*indexToCrop)[1];
  double slope         =  lengthSegment / ((*indexToCrop)[0]  - (*otherIndex)[0]);
  double origin        =  (*otherIndex)[1] - (slope * (*otherIndex)[0]);

  (*indexToCrop)[0] = static_cast<OutputIndexValueType>(size[0] - 1);
  (*indexToCrop)[1] = static_cast<OutputIndexValueType>(slope * (*indexToCrop)[0] + origin + 0.5);
}

/**
 * Define if the point is upside the region or not
 *
 */
template <class TInputImage, class TOutput>
bool
DrawLineSpatialObjectListFilter<TInputImage, TOutput>
::IsUpsideTheRegion(OutputIndexType *indexToCrop, const OutputImageRegionType *outputRegionForThread) const

{
  /** Dimensions of the buffered region*/
  //typename OutputImageRegionType::SizeType  size  = outputRegionForThread->GetSize();
  typename OutputImageRegionType::IndexType start = outputRegionForThread->GetIndex();

  return (*indexToCrop)[1] < static_cast<OutputIndexValueType>(start[1]);
}

/**
 * Define if the point is upside the region or not
 *
 */
template <class TInputImage, class TOutput>
bool
DrawLineSpatialObjectListFilter<TInputImage, TOutput>
::IsDownsideTheRegion(OutputIndexType *indexToCrop, const OutputImageRegionType *outputRegionForThread) const

{
  /** Dimensions of the buffered region*/
  typename OutputImageRegionType::SizeType  size  = outputRegionForThread->GetSize();
  typename OutputImageRegionType::IndexType start = outputRegionForThread->GetIndex();

  return (*indexToCrop)[1] >= static_cast<OutputIndexValueType>(start[1] + size[1]); //The down limit of the region in the Y direction
}

/**
 * Define if the point is outside the hole image
 *
 */
template <class TInputImage, class TOutput>
bool
DrawLineSpatialObjectListFilter<TInputImage, TOutput>
::IsDownsideTheImage(OutputIndexType *indexToCrop) const

{
  return (*indexToCrop)[1] >= static_cast<OutputIndexValueType>(m_Length); //The down limit of the Image in the Y direction
}

/**
 *
 *
 */
template <class TInputImage, class TOutput>
bool
DrawLineSpatialObjectListFilter<TInputImage, TOutput>
::IsColumnOutsideOfTheRegion(OutputIndexType *indexToCheck,
                             OutputIndexType *otherToCheck,
                             const OutputImageRegionType *outputRegionForThread) const
{
  /** Dimensions of the buffered region*/
  typename OutputImageRegionType::SizeType size = outputRegionForThread->GetSize();
  bool                                     res = false, res1 = false, res2 = false;

  if (((*indexToCheck)[0] >= static_cast<OutputIndexValueType>(size[0])) &&
      ((*otherToCheck)[0] >= static_cast<OutputIndexValueType>(size[0]))) res  = true;

  if ((*indexToCheck)[0] >= static_cast<OutputIndexValueType>(size[0]) &&
      this->IsUpsideTheRegion(otherToCheck, outputRegionForThread)) res1 = true;

  if ((*indexToCheck)[0] >= static_cast<OutputIndexValueType>(size[0]) &&
      this->IsDownsideTheRegion(otherToCheck, outputRegionForThread)) res2 = true;

  return res || res1 || res2;

}

/**
 * Compute the intersection between the segment to draw and the region belonging to the current thread
 *
 */
template <class TInputImage, class TOutput>
void
DrawLineSpatialObjectListFilter<TInputImage, TOutput>
::CropSegment(OutputIndexType *indexToCrop,
              OutputIndexType *otherIndex,
              const OutputImageRegionType *itkNotUsed(outputRegionForThread)) const

{
  OutputIndexType tempIndex;
  tempIndex = *indexToCrop;

  /** Dimensions of the buffered region*/
  //typename OutputImageRegionType::SizeType  size = outputRegionForThread->GetSize();
  //typename OutputImageRegionType::IndexType start = outputRegionForThread->GetIndex();

  /** Equation of the line (Begin, End)*/
  double slope = 0.;
  double lengthSegment = 0.;
  double origin = 0.;
  double tempOtherIndexX = 0.;

  /** Equation of the first Line*/

  if (vcl_abs((*otherIndex)[0] - (*indexToCrop)[0]) < 1e-4) tempOtherIndexX = 0.000001;
  else tempOtherIndexX = static_cast<double>((*otherIndex)[0]);

  if ((*indexToCrop)[0] < (*otherIndex)[0]) lengthSegment = (*otherIndex)[1] - (*indexToCrop)[1];
  else lengthSegment = (*indexToCrop)[1] - (*otherIndex)[1];

  slope = lengthSegment / (tempOtherIndexX - static_cast<double>((*indexToCrop)[0]));
  origin = (*indexToCrop)[1] - (slope * static_cast<double>((*indexToCrop)[0]));

  if ((*indexToCrop)[1] < 0)
    {
    unsigned int Y = 0;
    tempIndex[1] = Y;
    tempIndex[0] = static_cast<unsigned int>((Y - origin) / slope);  // X = (Y-B)/A
    }

  if (this->IsDownsideTheImage(indexToCrop))
    {
    double Y = static_cast<double>(m_Length - 1) /*tstart[1]+size[1]-1*/;
    tempIndex[1] = static_cast<OutputIndexValueType>(Y);
    tempIndex[0] = static_cast<OutputIndexValueType>((Y - origin) / slope);  // X = (Y-B)/A
    }

  (*indexToCrop)[0] = tempIndex[0];
  (*indexToCrop)[1] = tempIndex[1];
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void
DrawLineSpatialObjectListFilter<TInputImage, TOutput>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
