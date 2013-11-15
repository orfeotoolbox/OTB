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
#ifndef __otbMultiDisparityMapTo3DFilter_txx
#define __otbMultiDisparityMapTo3DFilter_txx

#include "otbMultiDisparityMapTo3DFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"

namespace otb
{

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::MultiDisparityMapTo3DFilter()
{
  // Set the number of inputs (1 moving image by default -> 3 inputs)
  this->SetNumberOfRequiredInputs(3);
  this->SetNumberOfRequiredInputs(1);
  this->m_MovingKeywordLists.resize(1);

  // Set the outputs
  this->SetNumberOfRequiredOutputs(2);
  this->SetNthOutput(0,TOutputImage::New());
  this->SetNthOutput(1,TResidueImage::New());
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::~MultiDisparityMapTo3DFilter()
{}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
void
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::SetNumberOfMovingImages(unsigned int nb)
{
  if (nb > 0)
    {
    this->SetNumberOfRequiredInputs(3 * nb);
    this->m_MovingKeywordLists.resize(nb);
    }
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
unsigned int
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::GetNumberOfMovingImages()
{
  return this->m_MovingKeywordLists.size();
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
void
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::SetHorizontalDisparityMapInput(unsigned int index, const TDisparityImage * hmap)
{
  if ((3 * (index + 1)) > this->GetNumberOfRequiredInputs())
    {
    itkExceptionMacro(<<"Index is greater than the number of moving images");
    }
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(3 * index, const_cast<TDisparityImage *>( hmap ));
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
void
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::SetVerticalDisparityMapInput(unsigned int index, const TDisparityImage * vmap)
{
  if ((3 * (index + 1)) > this->GetNumberOfRequiredInputs())
    {
    itkExceptionMacro(<<"Index is greater than the number of moving images");
    }
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(3 * index + 1, const_cast<TDisparityImage *>( vmap ));
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
void
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::SetDisparityMaskInput(unsigned int index, const TMaskImage * mask)
{
  if ((3 * (index + 1)) > this->GetNumberOfRequiredInputs())
    {
    itkExceptionMacro(<<"Index is greater than the number of moving images");
    }
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(3 * index + 2, const_cast<TMaskImage *>( mask ));
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
const TDisparityImage *
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::GetHorizontalDisparityMapInput(unsigned int index) const
{
  if ((3 * (index + 1)) > this->GetNumberOfRequiredInputs())
    {
    return 0;
    }
  return static_cast<const TDisparityImage *>(this->itk::ProcessObject::GetInput(3 * index));
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
const TDisparityImage *
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::GetVerticalDisparityMapInput(unsigned int index) const
{
  if ((3 * (index + 1)) > this->GetNumberOfRequiredInputs())
    {
    return 0;
    }
  return static_cast<const TDisparityImage *>(this->itk::ProcessObject::GetInput(3 * index + 1));
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
const TMaskImage *
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::GetDisparityMaskInput(unsigned int index) const
{
  if ((3 * (index + 1)) > this->GetNumberOfRequiredInputs())
    {
    return 0;
    }
  return static_cast<const TMaskImage *>(this->itk::ProcessObject::GetInput(3 * index + 2));
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
const TResidueImage *
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::GetResidueOutput() const
{
  return static_cast<const TResidueImage *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
TResidueImage *
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::GetResidueOutput()
{
  return static_cast<TResidueImage *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
void
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::SetMovingKeywordList(unsigned int index, const ImageKeywordListType kwl)
{
  if (this->m_MovingKeywordLists.size() > index)
    {
    this->m_MovingKeywordLists[index] = kwl;
    }
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
const typename MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>::ImageKeywordListType &
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::GetMovingKeywordList(unsigned int index) const
{
  if (this->m_MovingKeywordLists.size() <= index)
  {
    itkExceptionMacro(<<"Keywordlist index is outside the container");
  }
  return this->m_MovingKeywordLists[index];
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
void
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::GenerateOutputInformation()
{
  const TDisparityImage * horizDisp = this->GetHorizontalDisparityMapInput(0);
  TOutputImage * outputPtr = this->GetOutput();
  TResidueImage * residuePtr = this->GetResidueOutput();
  
  if (horizDisp)
    {
    outputPtr->SetLargestPossibleRegion(horizDisp->GetLargestPossibleRegion());
    outputPtr->SetNumberOfComponentsPerPixel(3);

    residuePtr->SetLargestPossibleRegion(horizDisp->GetLargestPossibleRegion());
    residuePtr->SetNumberOfComponentsPerPixel(1);
    
    // copy also origin and spacing
    outputPtr->SetOrigin(horizDisp->GetOrigin());
    outputPtr->SetSpacing(horizDisp->GetSpacing());
  
    residuePtr->SetOrigin(horizDisp->GetOrigin());
    residuePtr->SetSpacing(horizDisp->GetSpacing());
    
    if (this->m_ReferenceKeywordList.GetSize() > 0)
      {
      itk::EncapsulateMetaData<ImageKeywordListType>
        (outputPtr->GetMetaDataDictionary(),
        MetaDataKey::OSSIMKeywordlistKey,
        this->m_ReferenceKeywordList);
      itk::EncapsulateMetaData<ImageKeywordListType>
        (residuePtr->GetMetaDataDictionary(),
        MetaDataKey::OSSIMKeywordlistKey,
        this->m_ReferenceKeywordList);
      }
    else
      {
      itkExceptionMacro(<<"Reference keywordlist is missing");
      }
    }
  else
    {
    itkExceptionMacro(<< "First horizontal disparity map is missing");
    }
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
void
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::GenerateInputRequestedRegion()
{
  RegionType requested = this->GetOutput()->GetRequestedRegion();
  RegionType largest = this->GetHorizontalDisparityMapInput(0)->GetLargestPossibleRegion();
  
  for (unsigned int i=0; i<this->GetNumberOfRequiredInputs(); ++i)
    {
    unsigned int index  = i % 3;
    unsigned int pos    = i / 3;
    switch (index)
      {
      case 0:
        {
        // check horizontal disparity maps
        TDisparityImage * horizDisp = const_cast<TDisparityImage*>(this->GetHorizontalDisparityMapInput(pos));
        if (horizDisp)
          {
          if (horizDisp->GetLargestPossibleRegion() != largest)
            {
            itkExceptionMacro(<<"Horizontal disparity map at position "<<pos<<" has a different largest region");
            }
          horizDisp->SetRequestedRegion(requested);
          }
        else
          {
          itkExceptionMacro(<<"Horizontal disparity map at position "<<pos<<" is missing");
          }
        }
      break;
      case 1:
        {
        // check vertical disparity maps
        TDisparityImage * vertiDisp = const_cast<TDisparityImage*>(this->GetVerticalDisparityMapInput(pos));
        if (vertiDisp)
          {
          if (vertiDisp->GetLargestPossibleRegion() != largest)
            {
            itkExceptionMacro(<<"Vertical disparity map at position "<<pos<<" has a different largest region");
            }
          vertiDisp->SetRequestedRegion(requested);
          }
        }
      break;
      case 2:
        {
        // check disparity masks
        TMaskImage * maskDisp = const_cast<TMaskImage*>(this->GetDisparityMaskInput(pos));
        if (maskDisp)
          {
          if (maskDisp->GetLargestPossibleRegion() != largest)
            {
            itkExceptionMacro(<<"Disparity mask at position "<<pos<<" has a different largest region");
            }
          maskDisp->SetRequestedRegion(requested);
          }
        }
      break;
      default:
        itkExceptionMacro(<< "Unexpected value for (i%3) ");
      }
    }
  
  // Check moving keywordlist
  for (unsigned int k=0; k<this->m_MovingKeywordLists.size(); ++k)
  {
    if (this->m_MovingKeywordLists[k].GetSize() == 0)
      {
      itkExceptionMacro(<< "Keywordlist of moving image at position "<<k<<" is empty");
      }
  }

}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
void
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::BeforeThreadedGenerateData()
{
  // Instanciate all transforms
  this->m_ReferenceToGroundTransform = RSTransformType::New();
  this->m_ReferenceToGroundTransform->SetInputKeywordList(this->m_ReferenceKeywordList);
  this->m_ReferenceToGroundTransform->InstanciateTransform();
  
  this->m_MovingToGroundTransform.clear();
  for (unsigned int k=0; k<this->m_MovingKeywordLists.size(); ++k)
    {
    RSTransformType::Pointer transfo = RSTransformType::New();
    transfo->SetInputKeywordList(this->m_MovingKeywordLists[k]);
    transfo->InstanciateTransform();
    this->m_MovingToGroundTransform.push_back(transfo);
    }
}

template <class TDisparityImage, class TOutputImage,
class TMaskImage, class TResidueImage>
void
MultiDisparityMapTo3DFilter<TDisparityImage,TOutputImage,TMaskImage,TResidueImage>
::ThreadedGenerateData(const RegionType & outputRegionForThread, itk::ThreadIdType threadId)
{
  TOutputImage * outputPtr = this->GetOutput();
  TResidueImage * residuePtr = this->GetResidueOutput();
  
  itk::ImageRegionIteratorWithIndex<OutputImageType>   outIt(outputPtr,outputRegionForThread);
  itk::ImageRegionIterator<ResidueImageType>  resIt(residuePtr,outputRegionForThread);
  
  typename OptimizerType::Pointer optimizer = OptimizerType::New();
  
  DispMapIteratorList hDispIts;
  DispMapIteratorList vDispIts;
  MaskIteratorList maskIts;
  
  for (unsigned int k=0; k<this->m_MovingKeywordLists.size(); ++k)
    {
    // Iterators over horizontal disparity maps
    hDispIts[k] = itk::ImageRegionConstIterator<DisparityMapType>(this->GetHorizontalDisparityMapInput(k),outputRegionForThread);
    hDispIts[k].GoToBegin();
    // Iterators over vertical disparity maps
    if (this->GetVerticalDisparityMapInput(k))
      {
      vDispIts[k] = itk::ImageRegionConstIterator<DisparityMapType>(this->GetVerticalDisparityMapInput(k),outputRegionForThread);
      vDispIts[k].GoToBegin();
      }
    // Iterators over disparity masks
    if (this->GetDisparityMaskInput(k))
      {
      maskIts[k] = itk::ImageRegionConstIterator<MaskImageType>(this->GetDisparityMaskInput(k),outputRegionForThread);
      maskIts[k].GoToBegin();
      }
    }
  
  outIt.GoToBegin();
  resIt.GoToBegin();
  
  PrecisionType altiMin = 0;
  PrecisionType altiMax = 500;
  
  typename OutputImageType::PointType pointRef;
  TDPointType currentPoint;
  
  typename OutputImageType::PixelType outPixel(3);
  PrecisionType globalResidue;

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
    pointA = this->m_ReferenceToGroundTransform->TransformPoint(currentPoint);
    
    currentPoint[2] = altiMin;
    pointB = this->m_ReferenceToGroundTransform->TransformPoint(currentPoint);
    
    pointSetA->SetPoint(0, pointA);
    pointSetB->SetPoint(0, pointB);
    pointSetA->SetPointData(0,0);
    pointSetB->SetPointData(0,0);
    
    unsigned int nbPoints = 1;
    
    for (unsigned int k=0; k<this->m_MovingKeywordLists.size(); ++k)
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
      pointAi = this->m_MovingToGroundTransform[k]->TransformPoint(currentPoint);
      
      currentPoint[2] = altiMin;
      pointBi = this->m_MovingToGroundTransform[k]->TransformPoint(currentPoint);
      
      pointSetA->SetPoint(nbPoints, pointAi);
      pointSetB->SetPoint(nbPoints, pointBi);
      pointSetA->SetPointData(nbPoints,k+1);
      pointSetB->SetPointData(nbPoints,k+1);
      ++nbPoints;
      }
    
    // Check if there are at least 2 lines of sight, then compute intersection
    if (nbPoints >= 2)
      {
      TDPointType intersection = optimizer->Compute(pointSetA,pointSetB);
      outPixel[0] = intersection[0];
      outPixel[1] = intersection[1];
      outPixel[2] = intersection[2];
      globalResidue = optimizer->GetGlobalResidue();
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
