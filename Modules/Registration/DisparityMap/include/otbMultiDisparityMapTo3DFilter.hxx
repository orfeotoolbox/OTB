/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbMultiDisparityMapTo3DFilter_hxx
#define otbMultiDisparityMapTo3DFilter_hxx

#include "otbMultiDisparityMapTo3DFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"

namespace otb
{

template <class TDisparityImage, class TOutputImage, class TMaskImage, class TResidueImage>
MultiDisparityMapTo3DFilter<TDisparityImage, TOutputImage, TMaskImage, TResidueImage>::MultiDisparityMapTo3DFilter()
{
  // Set the number of inputs (1 moving image by default -> 3 inputs)
  this->SetNumberOfRequiredInputs(3);
  this->SetNumberOfRequiredInputs(1);
  this->m_MovingImageMetadatas.resize(1);

  // Set the outputs
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(0, TOutputImage::New());
  this->SetNthOutput(1, TResidueImage::New());
}

template <class TDisparityImage, class TOutputImage, class TMaskImage, class TResidueImage>
MultiDisparityMapTo3DFilter<TDisparityImage, TOutputImage, TMaskImage, TResidueImage>::~MultiDisparityMapTo3DFilter()
{
}

template <class TDisparityImage, class TOutputImage, class TMaskImage, class TResidueImage>
void MultiDisparityMapTo3DFilter<TDisparityImage, TOutputImage, TMaskImage, TResidueImage>::SetNumberOfMovingImages(unsigned int nb)
{
  if (nb > 0)
  {
    this->SetNumberOfRequiredInputs(3 * nb);
    this->m_MovingImageMetadatas.resize(nb);
  }
}

template <class TDisparityImage, class TOutputImage, class TMaskImage, class TResidueImage>
unsigned int MultiDisparityMapTo3DFilter<TDisparityImage, TOutputImage, TMaskImage, TResidueImage>::GetNumberOfMovingImages()
{
  return this->m_MovingImageMetadatas.size();
}

template <class TDisparityImage, class TOutputImage, class TMaskImage, class TResidueImage>
void MultiDisparityMapTo3DFilter<TDisparityImage, TOutputImage, TMaskImage, TResidueImage>::SetHorizontalDisparityMapInput(unsigned int index,
                                                                                                                           const TDisparityImage* hmap)
{
  if ((3 * (index + 1)) > this->GetNumberOfRequiredInputs())
  {
    itkExceptionMacro(<< "Index is greater than the number of moving images");
  }
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(3 * index, const_cast<TDisparityImage*>(hmap));
}

template <class TDisparityImage, class TOutputImage, class TMaskImage, class TResidueImage>
void MultiDisparityMapTo3DFilter<TDisparityImage, TOutputImage, TMaskImage, TResidueImage>::SetVerticalDisparityMapInput(unsigned int index,
                                                                                                                         const TDisparityImage* vmap)
{
  if ((3 * (index + 1)) > this->GetNumberOfRequiredInputs())
  {
    itkExceptionMacro(<< "Index is greater than the number of moving images");
  }
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(3 * index + 1, const_cast<TDisparityImage*>(vmap));
}

template <class TDisparityImage, class TOutputImage, class TMaskImage, class TResidueImage>
void MultiDisparityMapTo3DFilter<TDisparityImage, TOutputImage, TMaskImage, TResidueImage>::SetDisparityMaskInput(unsigned int index, const TMaskImage* mask)
{
  if ((3 * (index + 1)) > this->GetNumberOfRequiredInputs())
  {
    itkExceptionMacro(<< "Index is greater than the number of moving images");
  }
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(3 * index + 2, const_cast<TMaskImage*>(mask));
}

template <class TDisparityImage, class TOutputImage, class TMaskImage, class TResidueImage>
const TDisparityImage*
MultiDisparityMapTo3DFilter<TDisparityImage, TOutputImage, TMaskImage, TResidueImage>::GetHorizontalDisparityMapInput(unsigned int index) const
{
  if ((3 * (index + 1)) > this->GetNumberOfRequiredInputs())
  {
    return nullptr;
  }
  return static_cast<const TDisparityImage*>(this->itk::ProcessObject::GetInput(3 * index));
}

template <class TDisparityImage, class TOutputImage, class TMaskImage, class TResidueImage>
const TDisparityImage*
MultiDisparityMapTo3DFilter<TDisparityImage, TOutputImage, TMaskImage, TResidueImage>::GetVerticalDisparityMapInput(unsigned int index) const
{
  if ((3 * (index + 1)) > this->GetNumberOfRequiredInputs())
  {
    return nullptr;
  }
  return static_cast<const TDisparityImage*>(this->itk::ProcessObject::GetInput(3 * index + 1));
}

template <class TDisparityImage, class TOutputImage, class TMaskImage, class TResidueImage>
const TMaskImage* MultiDisparityMapTo3DFilter<TDisparityImage, TOutputImage, TMaskImage, TResidueImage>::GetDisparityMaskInput(unsigned int index) const
{
  if ((3 * (index + 1)) > this->GetNumberOfRequiredInputs())
  {
    return nullptr;
  }
  return static_cast<const TMaskImage*>(this->itk::ProcessObject::GetInput(3 * index + 2));
}

template <class TDisparityImage, class TOutputImage, class TMaskImage, class TResidueImage>
const TResidueImage* MultiDisparityMapTo3DFilter<TDisparityImage, TOutputImage, TMaskImage, TResidueImage>::GetResidueOutput() const
{
  return static_cast<const TResidueImage*>(this->itk::ProcessObject::GetOutput(1));
}

template <class TDisparityImage, class TOutputImage, class TMaskImage, class TResidueImage>
TResidueImage* MultiDisparityMapTo3DFilter<TDisparityImage, TOutputImage, TMaskImage, TResidueImage>::GetResidueOutput()
{
  return static_cast<TResidueImage*>(this->itk::ProcessObject::GetOutput(1));
}

template <class TDisparityImage, class TOutputImage, class TMaskImage, class TResidueImage>
void MultiDisparityMapTo3DFilter<TDisparityImage, TOutputImage, TMaskImage, TResidueImage>::SetMovingImageMetadata(unsigned int index,
                                                                                                                 const ImageMetadata* imd)
{
  if (this->m_MovingImageMetadatas.size() > index)
  {
    this->m_MovingImageMetadatas[index] = imd;
  }
}

template <class TDisparityImage, class TOutputImage, class TMaskImage, class TResidueImage>
const ImageMetadata*
MultiDisparityMapTo3DFilter<TDisparityImage, TOutputImage, TMaskImage, TResidueImage>::GetMovingImageMetadata(unsigned int index) const
{
  if (this->m_MovingImageMetadatas.size() <= index)
  {
    itkExceptionMacro(<< "ImageMetadata index is outside the container");
  }
  return this->m_MovingImageMetadatas[index];
}

template <class TDisparityImage, class TOutputImage, class TMaskImage, class TResidueImage>
void MultiDisparityMapTo3DFilter<TDisparityImage, TOutputImage, TMaskImage, TResidueImage>::GenerateOutputInformation()
{
  const TDisparityImage* horizDisp  = this->GetHorizontalDisparityMapInput(0);
  TOutputImage*          outputPtr  = this->GetOutput();
  TResidueImage*         residuePtr = this->GetResidueOutput();

  if (horizDisp)
  {
    outputPtr->SetLargestPossibleRegion(horizDisp->GetLargestPossibleRegion());
    outputPtr->SetNumberOfComponentsPerPixel(3);

    residuePtr->SetLargestPossibleRegion(horizDisp->GetLargestPossibleRegion());
    residuePtr->SetNumberOfComponentsPerPixel(1);

    // copy also origin and spacing
    outputPtr->SetOrigin(horizDisp->GetOrigin());
    outputPtr->SetSignedSpacing(horizDisp->GetSignedSpacing());

    residuePtr->SetOrigin(horizDisp->GetOrigin());
    residuePtr->SetSignedSpacing(horizDisp->GetSignedSpacing());

    if (this->m_ReferenceImageMetadata)
    {
      auto outputmetadata = *m_ReferenceImageMetadata;
      // Don't copy band metadata, as output bands are not related to input bands.
      outputmetadata.Bands.clear();
      outputPtr->SetImageMetadata(outputmetadata);
      residuePtr->SetImageMetadata(outputmetadata);
    }
    else
    {
      itkExceptionMacro(<< "Reference ImageMetadata is missing");
    }
  }
  else
  {
    itkExceptionMacro(<< "First horizontal disparity map is missing");
  }
}

template <class TDisparityImage, class TOutputImage, class TMaskImage, class TResidueImage>
void MultiDisparityMapTo3DFilter<TDisparityImage, TOutputImage, TMaskImage, TResidueImage>::GenerateInputRequestedRegion()
{
  RegionType requested = this->GetOutput()->GetRequestedRegion();
  RegionType largest   = this->GetHorizontalDisparityMapInput(0)->GetLargestPossibleRegion();

  for (unsigned int i = 0; i < this->GetNumberOfRequiredInputs(); ++i)
  {
    unsigned int index = i % 3;
    unsigned int pos   = i / 3;
    switch (index)
    {
    case 0:
    {
      // check horizontal disparity maps
      TDisparityImage* horizDisp = const_cast<TDisparityImage*>(this->GetHorizontalDisparityMapInput(pos));
      if (horizDisp)
      {
        if (horizDisp->GetLargestPossibleRegion() != largest)
        {
          itkExceptionMacro(<< "Horizontal disparity map at position " << pos << " has a different largest region");
        }
        horizDisp->SetRequestedRegion(requested);
      }
      else
      {
        itkExceptionMacro(<< "Horizontal disparity map at position " << pos << " is missing");
      }
    }
    break;
    case 1:
    {
      // check vertical disparity maps
      TDisparityImage* vertiDisp = const_cast<TDisparityImage*>(this->GetVerticalDisparityMapInput(pos));
      if (vertiDisp)
      {
        if (vertiDisp->GetLargestPossibleRegion() != largest)
        {
          itkExceptionMacro(<< "Vertical disparity map at position " << pos << " has a different largest region");
        }
        vertiDisp->SetRequestedRegion(requested);
      }
    }
    break;
    case 2:
    {
      // check disparity masks
      TMaskImage* maskDisp = const_cast<TMaskImage*>(this->GetDisparityMaskInput(pos));
      if (maskDisp)
      {
        if (maskDisp->GetLargestPossibleRegion() != largest)
        {
          itkExceptionMacro(<< "Disparity mask at position " << pos << " has a different largest region");
        }
        maskDisp->SetRequestedRegion(requested);
      }
    }
    break;
    default:
      itkExceptionMacro(<< "Unexpected value for (i%3) ");
    }
  }

  // Check moving ImageMetadata
  for (unsigned int k = 0; k < this->m_MovingImageMetadatas.size(); ++k)
  {
    if (!this->m_MovingImageMetadatas[k])
    {
      itkExceptionMacro(<< "ImageMetadata of moving image at position " << k << " is empty");
    }
  }
}

template <class TDisparityImage, class TOutputImage, class TMaskImage, class TResidueImage>
void MultiDisparityMapTo3DFilter<TDisparityImage, TOutputImage, TMaskImage, TResidueImage>::ThreadedGenerateData(const RegionType& outputRegionForThread,
                                                                                                                 itk::ThreadIdType itkNotUsed(threadId))
{  // Instantiate all transforms
  auto referenceToGroundTransform = RSTransformType::New();

  referenceToGroundTransform->SetInputImageMetadata(this->m_ReferenceImageMetadata);
  referenceToGroundTransform->InstantiateTransform();

  /** Moving sensor image transforms */
  std::vector<RSTransformType::Pointer> movingToGroundTransform;

  for (unsigned int k = 0; k < this->m_MovingImageMetadatas.size(); ++k)
  {
    RSTransformType::Pointer transfo = RSTransformType::New();
    transfo->SetInputImageMetadata(this->m_MovingImageMetadatas[k]);
    transfo->InstantiateTransform();
    movingToGroundTransform.push_back(transfo);
  }


  TOutputImage*  outputPtr  = this->GetOutput();
  TResidueImage* residuePtr = this->GetResidueOutput();

  itk::ImageRegionIteratorWithIndex<OutputImageType> outIt(outputPtr, outputRegionForThread);
  itk::ImageRegionIterator<ResidueImageType>         resIt(residuePtr, outputRegionForThread);

  typename OptimizerType::Pointer optimizer = OptimizerType::New();

  DispMapIteratorList hDispIts;
  DispMapIteratorList vDispIts;
  MaskIteratorList    maskIts;

  for (unsigned int k = 0; k < this->m_MovingImageMetadatas.size(); ++k)
  {
    // Iterators over horizontal disparity maps
    hDispIts[k] = itk::ImageRegionConstIterator<DisparityMapType>(this->GetHorizontalDisparityMapInput(k), outputRegionForThread);
    hDispIts[k].GoToBegin();
    // Iterators over vertical disparity maps
    if (this->GetVerticalDisparityMapInput(k))
    {
      vDispIts[k] = itk::ImageRegionConstIterator<DisparityMapType>(this->GetVerticalDisparityMapInput(k), outputRegionForThread);
      vDispIts[k].GoToBegin();
    }
    // Iterators over disparity masks
    if (this->GetDisparityMaskInput(k))
    {
      maskIts[k] = itk::ImageRegionConstIterator<MaskImageType>(this->GetDisparityMaskInput(k), outputRegionForThread);
      maskIts[k].GoToBegin();
    }
  }

  outIt.GoToBegin();
  resIt.GoToBegin();

  PrecisionType altiMin = 0;
  PrecisionType altiMax = 500;

  typename OutputImageType::PointType pointRef;
  TDPointType                         currentPoint;

  typename OutputImageType::PixelType outPixel(3);
  PrecisionType                       globalResidue;

  typename PointSetType::Pointer pointSetA = PointSetType::New();
  typename PointSetType::Pointer pointSetB = PointSetType::New();

  while (!outIt.IsAtEnd())
  {
    pointSetA->Initialize();
    pointSetB->Initialize();

    // Compute reference line of sight
    TDPointType pointA, pointB;

    outputPtr->TransformIndexToPhysicalPoint(outIt.GetIndex(), pointRef);

    currentPoint[0] = pointRef[0];
    currentPoint[1] = pointRef[1];
    currentPoint[2] = altiMax;

    pointA          = referenceToGroundTransform->TransformPoint(currentPoint);

    currentPoint[2] = altiMin;
    pointB          = referenceToGroundTransform->TransformPoint(currentPoint);

    pointSetA->SetPoint(0, pointA);
    pointSetB->SetPoint(0, pointB);
    pointSetA->SetPointData(0, 0);
    pointSetB->SetPointData(0, 0);

    unsigned int nbPoints = 1;

    for (unsigned int k = 0; k < this->m_MovingImageMetadatas.size(); ++k)
    {
      // Compute the N moving lines of sight
      TDPointType pointAi, pointBi;

      if (maskIts.count(k) && !(maskIts[k].Get() > 0))
      {
        continue;
      }

      currentPoint[0] = pointRef[0] + hDispIts[k].Get();
      currentPoint[1] = pointRef[1];
      if (vDispIts.count(k))
      {
        currentPoint[1] += vDispIts[k].Get();
      }

      currentPoint[2] = altiMax;
      pointAi         = movingToGroundTransform[k]->TransformPoint(currentPoint);
      currentPoint[2] = altiMin;
      pointBi         = movingToGroundTransform[k]->TransformPoint(currentPoint);
      pointSetA->SetPoint(nbPoints, pointAi);
      pointSetB->SetPoint(nbPoints, pointBi);
      pointSetA->SetPointData(nbPoints, k + 1);
      pointSetB->SetPointData(nbPoints, k + 1);
      ++nbPoints;
    }

    // Check if there are at least 2 lines of sight, then compute intersection
    if (nbPoints >= 2)
    {
      TDPointType intersection = optimizer->Compute(pointSetA, pointSetB);
      outPixel[0]              = intersection[0];
      outPixel[1]              = intersection[1];
      outPixel[2]              = intersection[2];
      globalResidue            = optimizer->GetGlobalResidue();
    }
    else
    {
      outPixel.Fill(0);
      globalResidue = 0;
    }

    outIt.Set(outPixel);
    resIt.Set(globalResidue);

    // Increment all iterators
    for (typename DispMapIteratorList::iterator hIt = hDispIts.begin(); hIt != hDispIts.end(); ++hIt)
    {
      ++hIt->second;
    }
    for (typename DispMapIteratorList::iterator vIt = vDispIts.begin(); vIt != vDispIts.end(); ++vIt)
    {
      ++vIt->second;
    }
    for (typename MaskIteratorList::iterator mIt = maskIts.begin(); mIt != maskIts.end(); ++mIt)
    {
      ++mIt->second;
    }

    ++outIt;
    ++resIt;
  }
}
}

#endif
