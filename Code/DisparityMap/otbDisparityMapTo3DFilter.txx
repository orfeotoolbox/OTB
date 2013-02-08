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
#ifndef __otbDisparityMapTo3DFilter_txx
#define __otbDisparityMapTo3DFilter_txx

#include "otbDisparityMapTo3DFilter.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkImageRegionIterator.h"

namespace otb
{

template <class TDisparityImage, class TOutputImage,
class TEpipolarGridImage, class TMaskImage>
DisparityMapTo3DFilter<TDisparityImage,TOutputImage,TEpipolarGridImage,TMaskImage>
::DisparityMapTo3DFilter()
{
  // Set the number of inputs
  this->SetNumberOfInputs(5);
  this->SetNumberOfRequiredInputs(1);

  // Set the outputs
  this->SetNumberOfOutputs(1);
  this->SetNthOutput(0,TOutputImage::New());
}

template <class TDisparityImage, class TOutputImage,
class TEpipolarGridImage, class TMaskImage>
DisparityMapTo3DFilter<TDisparityImage,TOutputImage,TEpipolarGridImage,TMaskImage>
::~DisparityMapTo3DFilter()
{}

template <class TDisparityImage, class TOutputImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapTo3DFilter<TDisparityImage,TOutputImage,TEpipolarGridImage,TMaskImage>
::SetHorizontalDisparityMapInput( const TDisparityImage * hmap)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<TDisparityImage *>( hmap ));
}

template <class TDisparityImage, class TOutputImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapTo3DFilter<TDisparityImage,TOutputImage,TEpipolarGridImage,TMaskImage>
::SetVerticalDisparityMapInput( const TDisparityImage * vmap)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<TDisparityImage *>( vmap ));
}

template <class TDisparityImage, class TOutputImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapTo3DFilter<TDisparityImage,TOutputImage,TEpipolarGridImage,TMaskImage>
::SetLeftEpipolarGridInput( const TEpipolarGridImage * grid)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(2, const_cast<TEpipolarGridImage *>( grid ));
}

template <class TDisparityImage, class TOutputImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapTo3DFilter<TDisparityImage,TOutputImage,TEpipolarGridImage,TMaskImage>
::SetRightEpipolarGridInput( const TEpipolarGridImage * grid)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(3, const_cast<TEpipolarGridImage *>( grid ));
}

template <class TDisparityImage, class TOutputImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapTo3DFilter<TDisparityImage,TOutputImage,TEpipolarGridImage,TMaskImage>
::SetDisparityMaskInput( const TMaskImage * mask)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(4, const_cast<TMaskImage *>( mask ));
}

template <class TDisparityImage, class TOutputImage,
class TEpipolarGridImage, class TMaskImage>
const TDisparityImage *
DisparityMapTo3DFilter<TDisparityImage,TOutputImage,TEpipolarGridImage,TMaskImage>
::GetHorizontalDisparityMapInput() const
{
  if(this->GetNumberOfInputs()<1)
    {
    return 0;
    }
  return static_cast<const TDisparityImage *>(this->itk::ProcessObject::GetInput(0));
}

template <class TDisparityImage, class TOutputImage,
class TEpipolarGridImage, class TMaskImage>
const TDisparityImage *
DisparityMapTo3DFilter<TDisparityImage,TOutputImage,TEpipolarGridImage,TMaskImage>
::GetVerticalDisparityMapInput() const
{
  if(this->GetNumberOfInputs()<2)
    {
    return 0;
    }
  return static_cast<const TDisparityImage *>(this->itk::ProcessObject::GetInput(1));
}

template <class TDisparityImage, class TOutputImage,
class TEpipolarGridImage, class TMaskImage>
const TEpipolarGridImage *
DisparityMapTo3DFilter<TDisparityImage,TOutputImage,TEpipolarGridImage,TMaskImage>
::GetLeftEpipolarGridInput() const
{
  if(this->GetNumberOfInputs()<3)
    {
    return 0;
    }
  return static_cast<const TEpipolarGridImage *>(this->itk::ProcessObject::GetInput(2));
}

template <class TDisparityImage, class TOutputImage,
class TEpipolarGridImage, class TMaskImage>
const TEpipolarGridImage *
DisparityMapTo3DFilter<TDisparityImage,TOutputImage,TEpipolarGridImage,TMaskImage>
::GetRightEpipolarGridInput() const
{
  if(this->GetNumberOfInputs()<4)
    {
    return 0;
    }
  return static_cast<const TEpipolarGridImage *>(this->itk::ProcessObject::GetInput(3));
}

template <class TDisparityImage, class TOutputImage,
class TEpipolarGridImage, class TMaskImage>
const TMaskImage *
DisparityMapTo3DFilter<TDisparityImage,TOutputImage,TEpipolarGridImage,TMaskImage>
::GetDisparityMaskInput() const
{
  if(this->GetNumberOfInputs()<5)
    {
    return 0;
    }
  return static_cast<const TMaskImage *>(this->itk::ProcessObject::GetInput(4));
}

template <class TDisparityImage, class TOutputImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapTo3DFilter<TDisparityImage,TOutputImage,TEpipolarGridImage,TMaskImage>
::GenerateOutputInformation()
{
  const TDisparityImage * horizDisp = this->GetHorizontalDisparityMapInput();
  TOutputImage * outputPtr = this->GetOutput();
  
  outputPtr->SetLargestPossibleRegion(horizDisp->GetLargestPossibleRegion());
  outputPtr->SetNumberOfComponentsPerPixel(3);
  
  // copy also origin and spacing
  outputPtr->SetOrigin(horizDisp->GetOrigin());
  outputPtr->SetSpacing(horizDisp->GetSpacing());
}

template <class TDisparityImage, class TOutputImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapTo3DFilter<TDisparityImage,TOutputImage,TEpipolarGridImage,TMaskImage>
::GenerateInputRequestedRegion()
{
  // For the epi grid : generate full buffer here !
  TEpipolarGridImage * leftGrid = const_cast<TEpipolarGridImage*>(this->GetLeftEpipolarGridInput());
  TEpipolarGridImage * rightGrid = const_cast<TEpipolarGridImage*>(this->GetRightEpipolarGridInput());
  
  leftGrid->SetRequestedRegionToLargestPossibleRegion();
  rightGrid->SetRequestedRegionToLargestPossibleRegion();
  
  TOutputImage * outputDEM = this->GetOutput();
  
  TDisparityImage * horizDisp = const_cast<TDisparityImage*>(this->GetHorizontalDisparityMapInput());
  TDisparityImage * vertiDisp = const_cast<TDisparityImage*>(this->GetVerticalDisparityMapInput());
  TMaskImage * maskDisp = const_cast<TMaskImage*>(this->GetDisparityMaskInput());
  
  // We impose that both disparity map inputs have the same size
  if(vertiDisp &&
     horizDisp->GetLargestPossibleRegion() != vertiDisp->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<<"Horizontal and vertical disparity maps do not have the same size ! Horizontal largest region: "
      <<horizDisp->GetLargestPossibleRegion()<<", vertical largest region: "<<vertiDisp->GetLargestPossibleRegion());
    }
    
  
  if (maskDisp && horizDisp->GetLargestPossibleRegion() != maskDisp->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<<"Disparity map and mask do not have the same size ! Map region : "
      <<horizDisp->GetLargestPossibleRegion()<<", mask region : "<<maskDisp->GetLargestPossibleRegion());
    }
  
  horizDisp->SetRequestedRegion( outputDEM->GetRequestedRegion() );
  
  if (vertiDisp)
    {
    vertiDisp->SetRequestedRegion( outputDEM->GetRequestedRegion() );
    }
    
  if (maskDisp)
    {
    maskDisp->SetRequestedRegion( outputDEM->GetRequestedRegion() );
    }
  
  // Check that the keywordlists are not empty
  if (m_LeftKeywordList.GetSize() == 0 || m_RightKeywordList.GetSize() == 0)
    {
    itkExceptionMacro(<<"At least one of the image keywordlist is empty : can't instanciate corresponding projection");
    }
}

template <class TDisparityImage, class TOutputImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapTo3DFilter<TDisparityImage,TOutputImage,TEpipolarGridImage,TMaskImage>
::BeforeThreadedGenerateData()
{
  // Instanciate transforms
  m_LeftToGroundTransform = RSTransformType::New();
  m_RightToGroundTransform = RSTransformType::New();
  
  m_LeftToGroundTransform->SetInputKeywordList(m_LeftKeywordList);
  m_RightToGroundTransform->SetInputKeywordList(m_RightKeywordList);
    
  m_LeftToGroundTransform->InstanciateTransform();
  m_RightToGroundTransform->InstanciateTransform();
}

template <class TDisparityImage, class TOutputImage,
class TEpipolarGridImage, class TMaskImage>
void
DisparityMapTo3DFilter<TDisparityImage,TOutputImage,TEpipolarGridImage,TMaskImage>
::ThreadedGenerateData(const RegionType & outputRegionForThread, int threadId)
{
  const TDisparityImage * horizDisp = this->GetHorizontalDisparityMapInput();
  const TDisparityImage * vertiDisp = this->GetVerticalDisparityMapInput();
  
  const TMaskImage * disparityMask = this->GetDisparityMaskInput();
  
  TOutputImage * outputDEM = this->GetOutput();

  // Get epipolar grids
  const TEpipolarGridImage * leftGrid = this->GetLeftEpipolarGridInput();
  const TEpipolarGridImage * rightGrid = this->GetRightEpipolarGridInput();
  
  typename TEpipolarGridImage::RegionType gridRegion = leftGrid->GetLargestPossibleRegion();
  
  typename TOutputImage::RegionType outputRequestedRegion = outputDEM->GetRequestedRegion();
  
  itk::ImageRegionIterator<OutputImageType> demIt(outputDEM,outputRequestedRegion);
  itk::ImageRegionConstIteratorWithIndex<DisparityMapType> horizIt(horizDisp,outputRequestedRegion);
  
  demIt.GoToBegin();
  horizIt.GoToBegin();

  bool useVerti = false;
  itk::ImageRegionConstIteratorWithIndex<DisparityMapType> vertiIt;
  if (vertiDisp)
  {
    useVerti = true;
    vertiIt = itk::ImageRegionConstIteratorWithIndex<DisparityMapType>(vertiDisp,outputRequestedRegion);
    vertiIt.GoToBegin();
  }
  
  bool useMask = false;
  itk::ImageRegionConstIterator<MaskImageType> maskIt;
  if (disparityMask)
    {
    useMask = true;
    maskIt = itk::ImageRegionConstIterator<MaskImageType>(disparityMask,outputRequestedRegion);
    maskIt.GoToBegin();
    }
  
  double elevationMin = 0.0;
  double elevationMax = 300.0;
  
  typename TDisparityImage::PointType epiPoint;
  itk::ContinuousIndex<double,2> gridIndexConti;
  double subPixIndex[2];
  typename GridImageType::IndexType ulIndex, urIndex, lrIndex, llIndex;
  typename GridImageType::PixelType ulPixel(2);
  typename GridImageType::PixelType urPixel(2);
  typename GridImageType::PixelType lrPixel(2);
  typename GridImageType::PixelType llPixel(2);
  typename GridImageType::PixelType cPixel(2);
  
  typename GridImageType::PointType ulPoint;
  typename GridImageType::PointType urPoint;
  typename GridImageType::PointType lrPoint;
  typename GridImageType::PointType llPoint;
  
  TDPointType sensorPoint;
  TDPointType leftGroundHmin;
  TDPointType leftGroundHmax;
  TDPointType rightGroundHmin;
  TDPointType rightGroundHmax;
  
  while (!demIt.IsAtEnd() && !horizIt.IsAtEnd())
    {
    // check mask value if any
    if (useMask)
      {
      if (!(maskIt.Get() > 0))
        {
        // TODO : what to do when masked ? put a no-data value ?
        
        ++demIt;
        ++horizIt;
        if (useVerti) ++vertiIt;
        ++maskIt;
        continue;
        }
      }
    
    // compute left ray
    horizDisp->TransformIndexToPhysicalPoint(horizIt.GetIndex(),epiPoint);
    leftGrid->TransformPhysicalPointToContinuousIndex(epiPoint,gridIndexConti);
    
    ulIndex[0] = static_cast<int>(vcl_floor(gridIndexConti[0]));
    ulIndex[1] = static_cast<int>(vcl_floor(gridIndexConti[1]));
    if (ulIndex[0] < gridRegion.GetIndex(0)) ulIndex[0] = gridRegion.GetIndex(0);
    if (ulIndex[1] < gridRegion.GetIndex(1)) ulIndex[1] = gridRegion.GetIndex(1);
    if (ulIndex[0] > (gridRegion.GetIndex(0) + static_cast<int>(gridRegion.GetSize(0)) - 2))
      {
      ulIndex[0] = gridRegion.GetIndex(0) + gridRegion.GetSize(0) - 2;
      }
    if (ulIndex[1] > (gridRegion.GetIndex(1) + static_cast<int>(gridRegion.GetSize(1)) - 2))
      {
      ulIndex[1] = gridRegion.GetIndex(1) + gridRegion.GetSize(1) - 2;
      }
    urIndex[0] = ulIndex[0] + 1;
    urIndex[1] = ulIndex[1];
    lrIndex[0] = ulIndex[0] + 1;
    lrIndex[1] = ulIndex[1] + 1;
    llIndex[0] = ulIndex[0];
    llIndex[1] = ulIndex[1] + 1;
    subPixIndex[0] = gridIndexConti[0] - static_cast<double>(ulIndex[0]);
    subPixIndex[1] = gridIndexConti[1] - static_cast<double>(ulIndex[1]);
    
    leftGrid->TransformIndexToPhysicalPoint(ulIndex, ulPoint);
    leftGrid->TransformIndexToPhysicalPoint(urIndex, urPoint);
    leftGrid->TransformIndexToPhysicalPoint(lrIndex, lrPoint);
    leftGrid->TransformIndexToPhysicalPoint(llIndex, llPoint);
    
    ulPixel[0] = (leftGrid->GetPixel(ulIndex))[0] + ulPoint[0];
    ulPixel[1] = (leftGrid->GetPixel(ulIndex))[1] + ulPoint[1];
    urPixel[0] = (leftGrid->GetPixel(urIndex))[0] + urPoint[0];
    urPixel[1] = (leftGrid->GetPixel(urIndex))[1] + urPoint[1];
    lrPixel[0] = (leftGrid->GetPixel(lrIndex))[0] + lrPoint[0];
    lrPixel[1] = (leftGrid->GetPixel(lrIndex))[1] + lrPoint[1];
    llPixel[0] = (leftGrid->GetPixel(llIndex))[0] + llPoint[0];
    llPixel[1] = (leftGrid->GetPixel(llIndex))[1] + llPoint[1];
    cPixel = (ulPixel * (1.0 - subPixIndex[0]) + urPixel * subPixIndex[0]) * (1.0 - subPixIndex[1]) +
             (llPixel * (1.0 - subPixIndex[0]) + lrPixel * subPixIndex[0]) * subPixIndex[1];
    
    sensorPoint[0] = cPixel[0];
    sensorPoint[1] = cPixel[1];
    sensorPoint[2] = elevationMin;
    leftGroundHmin = m_LeftToGroundTransform->TransformPoint(sensorPoint);
    
    sensorPoint[2] = elevationMax;
    leftGroundHmax = m_LeftToGroundTransform->TransformPoint(sensorPoint);
    
    // compute right ray
    itk::ContinuousIndex<double,2> rightIndexEstimate;
    rightIndexEstimate[0] = static_cast<double>((horizIt.GetIndex())[0]) + static_cast<double>(horizIt.Get());
    
    double verticalShift = 0;
    if (useVerti) verticalShift = static_cast<double>(vertiIt.Get());
    rightIndexEstimate[1] = static_cast<double>((horizIt.GetIndex())[1]) + verticalShift;
    
    horizDisp->TransformContinuousIndexToPhysicalPoint(rightIndexEstimate,epiPoint);
    rightGrid->TransformPhysicalPointToContinuousIndex(epiPoint,gridIndexConti);
    
    ulIndex[0] = static_cast<int>(vcl_floor(gridIndexConti[0]));
    ulIndex[1] = static_cast<int>(vcl_floor(gridIndexConti[1]));
    if (ulIndex[0] < gridRegion.GetIndex(0)) ulIndex[0] = gridRegion.GetIndex(0);
    if (ulIndex[1] < gridRegion.GetIndex(1)) ulIndex[1] = gridRegion.GetIndex(1);
    if (ulIndex[0] > (gridRegion.GetIndex(0) + static_cast<int>(gridRegion.GetSize(0)) - 2))
      {
      ulIndex[0] = gridRegion.GetIndex(0) + gridRegion.GetSize(0) - 2;
      }
    if (ulIndex[1] > (gridRegion.GetIndex(1) + static_cast<int>(gridRegion.GetSize(1)) - 2))
      {
      ulIndex[1] = gridRegion.GetIndex(1) + gridRegion.GetSize(1) - 2;
      }
    urIndex[0] = ulIndex[0] + 1;
    urIndex[1] = ulIndex[1];
    lrIndex[0] = ulIndex[0] + 1;
    lrIndex[1] = ulIndex[1] + 1;
    llIndex[0] = ulIndex[0];
    llIndex[1] = ulIndex[1] + 1;
    subPixIndex[0] = gridIndexConti[0] - static_cast<double>(ulIndex[0]);
    subPixIndex[1] = gridIndexConti[1] - static_cast<double>(ulIndex[1]);
    
    rightGrid->TransformIndexToPhysicalPoint(ulIndex, ulPoint);
    rightGrid->TransformIndexToPhysicalPoint(urIndex, urPoint);
    rightGrid->TransformIndexToPhysicalPoint(lrIndex, lrPoint);
    rightGrid->TransformIndexToPhysicalPoint(llIndex, llPoint);
    
    ulPixel[0] = (rightGrid->GetPixel(ulIndex))[0] + ulPoint[0];
    ulPixel[1] = (rightGrid->GetPixel(ulIndex))[1] + ulPoint[1];
    urPixel[0] = (rightGrid->GetPixel(urIndex))[0] + urPoint[0];
    urPixel[1] = (rightGrid->GetPixel(urIndex))[1] + urPoint[1];
    lrPixel[0] = (rightGrid->GetPixel(lrIndex))[0] + lrPoint[0];
    lrPixel[1] = (rightGrid->GetPixel(lrIndex))[1] + lrPoint[1];
    llPixel[0] = (rightGrid->GetPixel(llIndex))[0] + llPoint[0];
    llPixel[1] = (rightGrid->GetPixel(llIndex))[1] + llPoint[1];
    cPixel = (ulPixel * (1.0 - subPixIndex[0]) + urPixel * subPixIndex[0]) * (1.0 - subPixIndex[1]) +
             (llPixel * (1.0 - subPixIndex[0]) + lrPixel * subPixIndex[0]) * subPixIndex[1];
    
    sensorPoint[0] = cPixel[0];
    sensorPoint[1] = cPixel[1];
    sensorPoint[2] = elevationMin;
    rightGroundHmin = m_RightToGroundTransform->TransformPoint(sensorPoint);
    
    sensorPoint[2] = elevationMax;
    rightGroundHmax = m_RightToGroundTransform->TransformPoint(sensorPoint);
    
    // Compute ray intersection (mid-point method), TODO : implement non-iterative method from Hartley & Sturm
    double a = (leftGroundHmax[0] - leftGroundHmin[0]) * (leftGroundHmax[0] - leftGroundHmin[0]) +
               (leftGroundHmax[1] - leftGroundHmin[1]) * (leftGroundHmax[1] - leftGroundHmin[1]) +
               (leftGroundHmax[2] - leftGroundHmin[2]) * (leftGroundHmax[2] - leftGroundHmin[2]);
    double b = (rightGroundHmax[0] - rightGroundHmin[0]) * (rightGroundHmax[0] - rightGroundHmin[0]) +
               (rightGroundHmax[1] - rightGroundHmin[1]) * (rightGroundHmax[1] - rightGroundHmin[1]) +
               (rightGroundHmax[2] - rightGroundHmin[2]) * (rightGroundHmax[2] - rightGroundHmin[2]);
    double c = -(leftGroundHmax[0] - leftGroundHmin[0]) * (rightGroundHmax[0] - rightGroundHmin[0])
               -(leftGroundHmax[1] - leftGroundHmin[1]) * (rightGroundHmax[1] - rightGroundHmin[1])
               -(leftGroundHmax[2] - leftGroundHmin[2]) * (rightGroundHmax[2] - rightGroundHmin[2]);
    double g = (leftGroundHmax[0] - leftGroundHmin[0]) * (rightGroundHmin[0] - leftGroundHmin[0]) +
               (leftGroundHmax[1] - leftGroundHmin[1]) * (rightGroundHmin[1] - leftGroundHmin[1]) +
               (leftGroundHmax[2] - leftGroundHmin[2]) * (rightGroundHmin[2] - leftGroundHmin[2]);
    double h = -(rightGroundHmax[0] - rightGroundHmin[0]) * (rightGroundHmin[0] - leftGroundHmin[0])
               -(rightGroundHmax[1] - rightGroundHmin[1]) * (rightGroundHmin[1] - leftGroundHmin[1])
               -(rightGroundHmax[2] - rightGroundHmin[2]) * (rightGroundHmin[2] - leftGroundHmin[2]);
    
    double rLeft = (b * g - c * h) / (a * b - c * c);
    double rRight = (a * h - c * g) / (a * b - c * c);
    
    TDPointType leftFoot;
    leftFoot.SetToBarycentricCombination(leftGroundHmax,leftGroundHmin,rLeft );
    
    TDPointType rightFoot;
    rightFoot.SetToBarycentricCombination(rightGroundHmax,rightGroundHmin,rRight);
    
    TDPointType midPoint3D;
    midPoint3D.SetToMidPoint(leftFoot,rightFoot);
    
    // record 3D point
    typename OutputImageType::PixelType pixel3D(3);
    pixel3D[0] = midPoint3D[0];
    pixel3D[1] = midPoint3D[1];
    pixel3D[2] = midPoint3D[2];
    demIt.Set(pixel3D);
    
    ++demIt;
    ++horizIt;
    
    if (useVerti) ++vertiIt;
    if (useMask) ++maskIt;
    
    }
   
}

}

#endif
