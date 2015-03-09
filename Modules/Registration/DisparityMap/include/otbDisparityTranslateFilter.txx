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
#ifndef __otbDisparityTranslateFilter_txx
#define __otbDisparityTranslateFilter_txx

#include "otbDisparityTranslateFilter.h"

#include "itkImageRegionIteratorWithIndex.h"
#include "itkImageRegionIterator.h"

namespace otb
{

template <class TDisparityImage, class TGridImage, class TSensorImage, class TMaskImage>
DisparityTranslateFilter<TDisparityImage,TGridImage,TSensorImage,TMaskImage>
::DisparityTranslateFilter()
{
  m_NoDataValue = -32768;
  // Set the number of inputs (1 moving image by default -> 3 inputs)
  this->SetNumberOfRequiredInputs(6);
  this->SetNumberOfRequiredInputs(1);

  // Set the outputs
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(0,TDisparityImage::New());
  this->SetNthOutput(1,TDisparityImage::New());
}

template <class TDisparityImage, class TGridImage, class TSensorImage, class TMaskImage>
void
DisparityTranslateFilter<TDisparityImage,TGridImage,TSensorImage,TMaskImage>
::SetHorizontalDisparityMapInput( const TDisparityImage * hmap )
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<TDisparityImage *>( hmap ));
}

template <class TDisparityImage, class TGridImage, class TSensorImage, class TMaskImage>
void
DisparityTranslateFilter<TDisparityImage,TGridImage,TSensorImage,TMaskImage>
::SetVerticalDisparityMapInput( const TDisparityImage * vmap )
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<TDisparityImage *>( vmap ));
}

template <class TDisparityImage, class TGridImage, class TSensorImage, class TMaskImage>
void
DisparityTranslateFilter<TDisparityImage,TGridImage,TSensorImage,TMaskImage>
::SetInverseEpipolarLeftGrid( const TGridImage * lgrid )
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(2, const_cast<TGridImage *>( lgrid ));
}

template <class TDisparityImage, class TGridImage, class TSensorImage, class TMaskImage>
void
DisparityTranslateFilter<TDisparityImage,TGridImage,TSensorImage,TMaskImage>
::SetDirectEpipolarRightGrid( const TGridImage * rgrid )
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(3, const_cast<TGridImage *>( rgrid ));
}

template <class TDisparityImage, class TGridImage, class TSensorImage, class TMaskImage>
void
DisparityTranslateFilter<TDisparityImage,TGridImage,TSensorImage,TMaskImage>
::SetDisparityMaskInput( const TMaskImage * mask )
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(4, const_cast<TMaskImage *>( mask ));
}

template <class TDisparityImage, class TGridImage, class TSensorImage, class TMaskImage>
void
DisparityTranslateFilter<TDisparityImage,TGridImage,TSensorImage,TMaskImage>
::SetLeftSensorImageInput(const TSensorImage * left)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(5, const_cast<TSensorImage *>( left ));
}

template <class TDisparityImage, class TGridImage, class TSensorImage, class TMaskImage>
const TDisparityImage *
DisparityTranslateFilter<TDisparityImage,TGridImage,TSensorImage,TMaskImage>
::GetHorizontalDisparityMapInput() const
{
  if (this->GetNumberOfInputs()<1)
    {
    return 0;
    }
  return static_cast<const TDisparityImage *>(this->itk::ProcessObject::GetInput(0));
}

template <class TDisparityImage, class TGridImage, class TSensorImage, class TMaskImage>
const TDisparityImage *
DisparityTranslateFilter<TDisparityImage,TGridImage,TSensorImage,TMaskImage>
::GetVerticalDisparityMapInput() const
{
  if (this->GetNumberOfInputs()<2)
    {
    return 0;
    }
  return static_cast<const TDisparityImage *>(this->itk::ProcessObject::GetInput(1));
}

template <class TDisparityImage, class TGridImage, class TSensorImage, class TMaskImage>
const TGridImage *
DisparityTranslateFilter<TDisparityImage,TGridImage,TSensorImage,TMaskImage>
::GetInverseEpipolarLeftGrid() const
{
  if (this->GetNumberOfInputs()<3)
    {
    return 0;
    }
  return static_cast<const TGridImage *>(this->itk::ProcessObject::GetInput(2));
}

template <class TDisparityImage, class TGridImage, class TSensorImage, class TMaskImage>
const TGridImage *
DisparityTranslateFilter<TDisparityImage,TGridImage,TSensorImage,TMaskImage>
::GetDirectEpipolarRightGrid() const
{
  if (this->GetNumberOfInputs()<4)
    {
    return 0;
    }
  return static_cast<const TGridImage *>(this->itk::ProcessObject::GetInput(3));
}

template <class TDisparityImage, class TGridImage, class TSensorImage, class TMaskImage>
const TMaskImage *
DisparityTranslateFilter<TDisparityImage,TGridImage,TSensorImage,TMaskImage>
::GetDisparityMaskInput() const
{
  if (this->GetNumberOfInputs()<5)
    {
    return 0;
    }
  return static_cast<const TMaskImage *>(this->itk::ProcessObject::GetInput(4));
}

template <class TDisparityImage, class TGridImage, class TSensorImage, class TMaskImage>
const TSensorImage *
DisparityTranslateFilter<TDisparityImage,TGridImage,TSensorImage,TMaskImage>
::GetLeftSensorImageInput() const
{
  if (this->GetNumberOfInputs()<6)
    {
    return 0;
    }
  return static_cast<const TSensorImage *>(this->itk::ProcessObject::GetInput(5));
}

template <class TDisparityImage, class TGridImage, class TSensorImage, class TMaskImage>
TDisparityImage *
DisparityTranslateFilter<TDisparityImage,TGridImage,TSensorImage,TMaskImage>
::GetHorizontalDisparityMapOutput()
{
  if (this->GetNumberOfOutputs()<1)
    {
    return 0;
    }
  return static_cast<TDisparityImage *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TDisparityImage, class TGridImage, class TSensorImage, class TMaskImage>
TDisparityImage *
DisparityTranslateFilter<TDisparityImage,TGridImage,TSensorImage,TMaskImage>
::GetVerticalDisparityMapOutput()
{
  if (this->GetNumberOfOutputs()<2)
    {
    return 0;
    }
  return static_cast<TDisparityImage *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TDisparityImage, class TGridImage, class TSensorImage, class TMaskImage>
void
DisparityTranslateFilter<TDisparityImage,TGridImage,TSensorImage,TMaskImage>
::GenerateOutputInformation()
{
//   this->Superclass::GenerateOutputInformation();

  TDisparityImage * horizOut = this->GetHorizontalDisparityMapOutput();
  TDisparityImage * vertiOut = this->GetVerticalDisparityMapOutput();

  const TSensorImage * leftIn = this->GetLeftSensorImageInput();

  horizOut->CopyInformation(leftIn);
  vertiOut->CopyInformation(leftIn);
}

template <class TDisparityImage, class TGridImage, class TSensorImage, class TMaskImage>
void
DisparityTranslateFilter<TDisparityImage,TGridImage,TSensorImage,TMaskImage>
::GenerateInputRequestedRegion()
{
  this->Superclass::GenerateInputRequestedRegion();

  TGridImage * leftGrid = const_cast<TGridImage*>(this->GetInverseEpipolarLeftGrid());
  TGridImage * rightGrid = const_cast<TGridImage*>(this->GetDirectEpipolarRightGrid());

  leftGrid->SetRequestedRegionToLargestPossibleRegion();
  rightGrid->SetRequestedRegionToLargestPossibleRegion();

  leftGrid->UpdateOutputData();
  rightGrid->UpdateOutputData();

  TSensorImage * leftIn = const_cast<TSensorImage*>(this->GetLeftSensorImageInput());
  RegionType emptyRegion = leftIn->GetLargestPossibleRegion();
  emptyRegion.SetSize(0,0);
  emptyRegion.SetSize(1,0);
  leftIn->SetRequestedRegion(emptyRegion);

  TDisparityImage * horizOut = this->GetHorizontalDisparityMapOutput();

  TDisparityImage * horizIn = const_cast<TDisparityImage*>(this->GetHorizontalDisparityMapInput());
  TDisparityImage * vertiIn = const_cast<TDisparityImage*>(this->GetVerticalDisparityMapInput());

  TMaskImage * maskIn = const_cast<TMaskImage*>(this->GetDisparityMaskInput());

  RegionType requested    = this->GetHorizontalDisparityMapOutput()->GetRequestedRegion();
  RegionType inputlargest = this->GetHorizontalDisparityMapInput()->GetLargestPossibleRegion();
  RegionType inputRequested;
  GridRegionType gridLargest = leftGrid->GetLargestPossibleRegion();

  IndexType minIndex,maxIndex;
  // -Wmaybe-uninitialized
  typedef typename IndexType::IndexValueType IndexValueType;
  minIndex.Fill(itk::NumericTraits<IndexValueType>::Zero);
  maxIndex.Fill(itk::NumericTraits<IndexValueType>::Zero);

  IndexType corners[4];
  for(int i = 0; i < 4; i++)
    corners[i].Fill(itk::NumericTraits<IndexValueType>::Zero);

  corners[0] = requested.GetIndex();
  corners[1] = requested.GetIndex();
  corners[1][0] += static_cast<IndexValueType>(requested.GetSize()[0]) - 1;
  corners[2] = requested.GetIndex();
  corners[2][1] += static_cast<IndexValueType>(requested.GetSize()[1]) - 1;
  corners[3] = requested.GetIndex();
  corners[3][0] += static_cast<IndexValueType>(requested.GetSize()[0]) - 1;
  corners[3][1] += static_cast<IndexValueType>(requested.GetSize()[1]) - 1;
  for (unsigned int k=0; k<4; ++k)
    {
    PointType pointSensor;
    horizOut->TransformIndexToPhysicalPoint(corners[k],pointSensor);
    itk::ContinuousIndex<double,2> indexGrid;
    leftGrid->TransformPhysicalPointToContinuousIndex(pointSensor,indexGrid);
    IndexType ul;
    ul[0] = static_cast<long>(vcl_floor(indexGrid[0]));
    ul[1] = static_cast<long>(vcl_floor(indexGrid[1]));
    if (ul[0] < gridLargest.GetIndex()[0]) ul[0] = gridLargest.GetIndex()[0];
    if (ul[1] < gridLargest.GetIndex()[1]) ul[1] = gridLargest.GetIndex()[1];
    if (ul[0] > static_cast<IndexValueType>(gridLargest.GetIndex()[0] + gridLargest.GetSize()[0]-2))
      ul[0] = (gridLargest.GetIndex()[0] + gridLargest.GetSize()[0]-2);
    if (ul[1] > static_cast<IndexValueType>(gridLargest.GetIndex()[1] + gridLargest.GetSize()[1]-2))
      ul[1] = (gridLargest.GetIndex()[1] + gridLargest.GetSize()[1]-2);

    IndexType ur = ul;
    ur[0] += 1;
    IndexType ll = ul;
    ll[1] += 1;
    IndexType lr = ul;
    lr[0] += 1;
    lr[1] += 1;

    double rx = indexGrid[0] - static_cast<double>(ul[0]);
    double ry = indexGrid[1] - static_cast<double>(ul[1]);
    PointType pointEpi = pointSensor;

    pointEpi[0] += (1. - ry) * ((1. - rx) * leftGrid->GetPixel(ul)[0] + rx * leftGrid->GetPixel(ur)[0]) +
                           ry * ((1. - rx) * leftGrid->GetPixel(ll)[0] + rx * leftGrid->GetPixel(lr)[0]);
    pointEpi[1] += (1. - ry) * ((1. - rx) * leftGrid->GetPixel(ul)[1] + rx * leftGrid->GetPixel(ur)[1]) +
                           ry * ((1. - rx) * leftGrid->GetPixel(ll)[1] + rx * leftGrid->GetPixel(lr)[1]);
    itk::ContinuousIndex<double,2> indexEpi;

    horizIn->TransformPhysicalPointToContinuousIndex(pointEpi,indexEpi);
    if (k == 0)
      {
      minIndex[0] = static_cast<long>(vcl_floor(indexEpi[0]));
      minIndex[1] = static_cast<long>(vcl_floor(indexEpi[1]));
      maxIndex[0] = static_cast<long>(vcl_ceil(indexEpi[0]));
      maxIndex[1] = static_cast<long>(vcl_ceil(indexEpi[1]));
      }
    else
      {
      if (minIndex[0]>static_cast<long>(vcl_floor(indexEpi[0])))  minIndex[0]=static_cast<long>(vcl_floor(indexEpi[0]));
      if (minIndex[1]>static_cast<long>(vcl_floor(indexEpi[1])))  minIndex[1]=static_cast<long>(vcl_floor(indexEpi[1]));
      if (maxIndex[0]<static_cast<long>(vcl_ceil(indexEpi[0])))   maxIndex[0]=static_cast<long>(vcl_ceil(indexEpi[0]));
      if (maxIndex[1]<static_cast<long>(vcl_ceil(indexEpi[1])))   maxIndex[1]=static_cast<long>(vcl_ceil(indexEpi[1]));
      }
    }

  inputRequested.SetIndex(minIndex);
  inputRequested.SetSize(0,static_cast<unsigned long>(maxIndex[0]-minIndex[0]));
  inputRequested.SetSize(1,static_cast<unsigned long>(maxIndex[1]-minIndex[1]));

  if (!inputRequested.Crop(inputlargest))
    {
    inputRequested.SetSize(0,0);
    inputRequested.SetSize(1,0);
    inputRequested.SetIndex(inputlargest.GetIndex());
    }

  horizIn->SetRequestedRegion(inputRequested);
  if (vertiIn) vertiIn->SetRequestedRegion(inputRequested);
  if (maskIn) maskIn->SetRequestedRegion(inputRequested);
}

template <class TDisparityImage, class TGridImage, class TSensorImage, class TMaskImage>
void
DisparityTranslateFilter<TDisparityImage,TGridImage,TSensorImage,TMaskImage>
::ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType itkNotUsed(threadId))
{
  const TGridImage * leftGrid = this->GetInverseEpipolarLeftGrid();
  const TGridImage * rightGrid = this->GetDirectEpipolarRightGrid();

  TDisparityImage * horizOut = this->GetHorizontalDisparityMapOutput();
  TDisparityImage * vertiOut = this->GetVerticalDisparityMapOutput();

  const TDisparityImage * horizIn = this->GetHorizontalDisparityMapInput();
  const TDisparityImage * vertiIn = this->GetVerticalDisparityMapInput();

  const TMaskImage * maskIn = this->GetDisparityMaskInput();

  GridRegionType leftLargest = leftGrid->GetLargestPossibleRegion();
  GridRegionType rightLargest = rightGrid->GetLargestPossibleRegion();
  RegionType buffered = horizIn->GetBufferedRegion();

  const bool emptyInputRegion=buffered.GetNumberOfPixels() == 0;

    typedef itk::ImageRegionIteratorWithIndex<TDisparityImage> DispIterator;
    DispIterator horizIter(horizOut, outputRegionForThread);
    DispIterator vertiIter(vertiOut, outputRegionForThread);

    horizIter.GoToBegin();
    vertiIter.GoToBegin();

    while (!horizIter.IsAtEnd() && !vertiIter.IsAtEnd())
      {

      if(!emptyInputRegion)
        {
      PointType pointSensor;
      horizOut->TransformIndexToPhysicalPoint(horizIter.GetIndex(), pointSensor);

      itk::ContinuousIndex<double, 2> indexGrid;
      leftGrid->TransformPhysicalPointToContinuousIndex(pointSensor, indexGrid);

      // Interpolate in left grid
      typedef typename IndexType::IndexValueType IndexValueType;
      IndexType ul;
      ul[0] = static_cast<long> (vcl_floor(indexGrid[0]));
      ul[1] = static_cast<long> (vcl_floor(indexGrid[1]));
      if (ul[0] < leftLargest.GetIndex()[0]) ul[0] = leftLargest.GetIndex()[0];
      if (ul[1] < leftLargest.GetIndex()[1]) ul[1] = leftLargest.GetIndex()[1];
      if (ul[0] > static_cast<IndexValueType>(leftLargest.GetIndex()[0] + leftLargest.GetSize()[0] - 2))
        ul[0] = (leftLargest.GetIndex()[0] + leftLargest.GetSize()[0] - 2);
      if (ul[1] > static_cast<IndexValueType>(leftLargest.GetIndex()[1] + leftLargest.GetSize()[1] - 2))
        ul[1] = (leftLargest.GetIndex()[1] + leftLargest.GetSize()[1] - 2);

      IndexType ur = ul;
      ur[0] += 1;
      IndexType ll = ul;
      ll[1] += 1;
      IndexType lr = ul;
      lr[0] += 1;
      lr[1] += 1;

      double rx = indexGrid[0] - static_cast<double> (ul[0]);
      double ry = indexGrid[1] - static_cast<double> (ul[1]);
      PointType pointEpi = pointSensor;

      pointEpi[0] += (1. - ry) * ((1. - rx) * leftGrid->GetPixel(ul)[0] + rx * leftGrid->GetPixel(ur)[0]) + ry * ((1.
          - rx) * leftGrid->GetPixel(ll)[0] + rx * leftGrid->GetPixel(lr)[0]);
      pointEpi[1] += (1. - ry) * ((1. - rx) * leftGrid->GetPixel(ul)[1] + rx * leftGrid->GetPixel(ur)[1]) + ry * ((1.
          - rx) * leftGrid->GetPixel(ll)[1] + rx * leftGrid->GetPixel(lr)[1]);

      itk::ContinuousIndex<double, 2> indexEpi;
      horizIn->TransformPhysicalPointToContinuousIndex(pointEpi, indexEpi);

      // Interpolate in disparity map
      ul[0] = static_cast<long> (vcl_floor(indexEpi[0]));
      ul[1] = static_cast<long> (vcl_floor(indexEpi[1]));
      if (ul[0] < buffered.GetIndex()[0]) ul[0] = buffered.GetIndex()[0];
      if (ul[1] < buffered.GetIndex()[1]) ul[1] = buffered.GetIndex()[1];
      if (ul[0] > static_cast<IndexValueType>(buffered.GetIndex()[0] + buffered.GetSize()[0] - 2))
        ul[0] = (buffered.GetIndex()[0] + buffered.GetSize()[0] - 2);
      if (ul[1] > static_cast<IndexValueType>(buffered.GetIndex()[1] + buffered.GetSize()[1] - 2))
        ul[1] = (buffered.GetIndex()[1] + buffered.GetSize()[1] - 2);

      ur = ul;
      ur[0] += 1;
      ll = ul;
      ll[1] += 1;
      lr = ul;
      lr[0] += 1;
      lr[1] += 1;

      // check if all corners are valid
      if (!maskIn || (maskIn && maskIn->GetPixel(ul) > 0 && maskIn->GetPixel(ur) > 0 && maskIn->GetPixel(ll) > 0 &&
          maskIn->GetPixel(lr) > 0))
        {
        rx = indexEpi[0] - static_cast<double> (ul[0]);
        ry = indexEpi[1] - static_cast<double> (ul[1]);

        itk::ContinuousIndex<double, 2> indexRight(indexEpi);

        indexRight[0] += (1. - ry) * ((1. - rx) * horizIn->GetPixel(ul) + rx * horizIn->GetPixel(ur)) + ry * ((1. - rx)
            * horizIn->GetPixel(ll) + rx * horizIn->GetPixel(lr));
        if (vertiIn)
          {
          indexRight[1] += (1. - ry) * ((1. - rx) * vertiIn->GetPixel(ul) + rx * vertiIn->GetPixel(ur)) + ry * ((1.
              - rx) * vertiIn->GetPixel(ll) + rx * vertiIn->GetPixel(lr));
          }

        PointType pointRight;
        horizIn->TransformContinuousIndexToPhysicalPoint(indexRight, pointRight);

        itk::ContinuousIndex<double, 2> indexGridRight;
        rightGrid->TransformPhysicalPointToContinuousIndex(pointRight, indexGridRight);

        // Interpolate in right grid
        ul[0] = static_cast<long> (vcl_floor(indexGridRight[0]));
        ul[1] = static_cast<long> (vcl_floor(indexGridRight[1]));
        if (ul[0] < rightLargest.GetIndex()[0]) ul[0] = rightLargest.GetIndex()[0];
        if (ul[1] < rightLargest.GetIndex()[1]) ul[1] = rightLargest.GetIndex()[1];
        if (ul[0] > static_cast<IndexValueType>(rightLargest.GetIndex()[0] + rightLargest.GetSize()[0] - 2))
          ul[0] = (rightLargest.GetIndex()[0] + rightLargest.GetSize()[0] - 2);
        if (ul[1] > static_cast<IndexValueType>(rightLargest.GetIndex()[1] + rightLargest.GetSize()[1] - 2))
          ul[1] = (rightLargest.GetIndex()[1] + rightLargest.GetSize()[1] - 2);

        ur = ul;
        ur[0] += 1;
        ll = ul;
        ll[1] += 1;
        lr = ul;
        lr[0] += 1;
        lr[1] += 1;

        rx = indexGridRight[0] - static_cast<double> (ul[0]);
        ry = indexGridRight[1] - static_cast<double> (ul[1]);

        PointType pointSensorRight = pointRight;

        pointSensorRight[0] += (1. - ry) * ((1. - rx) * rightGrid->GetPixel(ul)[0] + rx * rightGrid->GetPixel(ur)[0]) +
            ry * ((1. - rx) * rightGrid->GetPixel(ll)[0] + rx * rightGrid->GetPixel(lr)[0]);
        pointSensorRight[1] += (1. - ry) * ((1. - rx) * rightGrid->GetPixel(ul)[1] + rx * rightGrid->GetPixel(ur)[1]) +
            ry * ((1. - rx) * rightGrid->GetPixel(ll)[1] + rx * rightGrid->GetPixel(lr)[1]);

        horizIter.Set(pointSensorRight[0] - pointSensor[0]);
        vertiIter.Set(pointSensorRight[1] - pointSensor[1]);
        }
      else
        {
        horizIter.Set(m_NoDataValue);
        vertiIter.Set(m_NoDataValue);
        }
        }
      else
        {
        horizIter.Set(m_NoDataValue);
        vertiIter.Set(m_NoDataValue);
        }
      ++horizIter;
      ++vertiIter;
      }


}


}

#endif
