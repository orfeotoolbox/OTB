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
#ifndef __otbPixelWiseBlockMatchingImageFilter_txx
#define __otbPixelWiseBlockMatchingImageFilter_txx

#include "otbPixelWiseBlockMatchingImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"
#include "itkConstantBoundaryCondition.h"

namespace otb
{
template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::PixelWiseBlockMatchingImageFilter()
{
  // Set the number of inputs
  this->SetNumberOfInputs(6);
  this->SetNumberOfRequiredInputs(2);

  // Set the outputs
  this->SetNumberOfOutputs(3);
  this->SetNthOutput(0,TOutputMetricImage::New());
  this->SetNthOutput(1,TOutputDisparityImage::New());
  this->SetNthOutput(2,TOutputDisparityImage::New());

  // Default parameters
  m_Radius.Fill(2);

  // Minimize by default
  m_Minimize = true;

  // Default disparity range
  m_MinimumHorizontalDisparity = -10;
  m_MaximumHorizontalDisparity =  10;
  m_MinimumVerticalDisparity = 0;
  m_MaximumVerticalDisparity = 0;
  
  // Default initial disparity
  m_InitHorizontalDisparity = 0;
  m_InitVerticalDisparity = 0;
  
  // Default exploration radius : 0 (not used)
  m_ExplorationRadius.Fill(0);
}


template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::~PixelWiseBlockMatchingImageFilter()
{}


template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::SetLeftInput(const TInputImage * image)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<TInputImage *>( image ));
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::SetRightInput(const TInputImage * image)
{
// Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<TInputImage *>( image ));
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::SetLeftMaskInput(const TMaskImage * image)
{
// Process object is not const-correct so the const casting is required.
  this->SetNthInput(2, const_cast<TMaskImage *>( image ));
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::SetRightMaskInput(const TMaskImage * image)
{
// Process object is not const-correct so the const casting is required.
  this->SetNthInput(3, const_cast<TMaskImage *>( image ));
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TInputImage *
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetLeftInput() const
{
  if (this->GetNumberOfInputs()<1)
    {
    return 0;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(0));
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TInputImage *
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetRightInput() const
{
  if(this->GetNumberOfInputs()<2)
    {
    return 0;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TMaskImage *
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetLeftMaskInput() const
{
  if(this->GetNumberOfInputs()<3)
    {
    return 0;
    }
  return static_cast<const TMaskImage *>(this->itk::ProcessObject::GetInput(2));
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TMaskImage *
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetRightMaskInput() const
{
  if(this->GetNumberOfInputs()<4)
    {
    return 0;
    }
  return static_cast<const TMaskImage *>(this->itk::ProcessObject::GetInput(3));
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TOutputMetricImage *
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetMetricOutput() const
{
  if (this->GetNumberOfOutputs()<1)
    {
    return 0;
    }
  return static_cast<const TOutputMetricImage *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
TOutputMetricImage *
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetMetricOutput()
{
  if (this->GetNumberOfOutputs()<1)
    {
    return 0;
    }
  return static_cast<TOutputMetricImage *>(this->itk::ProcessObject::GetOutput(0));
}


template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TOutputDisparityImage *
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetHorizontalDisparityOutput() const
{
  if (this->GetNumberOfOutputs()<2)
    {
    return 0;
    }
  return static_cast<const TOutputDisparityImage *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
TOutputDisparityImage *
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetHorizontalDisparityOutput()
{
if (this->GetNumberOfOutputs()<2)
    {
    return 0;
    }
  return static_cast<TOutputDisparityImage *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TOutputDisparityImage *
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetVerticalDisparityOutput() const
{
  if (this->GetNumberOfOutputs()<3)
    {
    return 0;
    }
  return static_cast<const TOutputDisparityImage *>(this->itk::ProcessObject::GetOutput(2));
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
TOutputDisparityImage *
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetVerticalDisparityOutput()
{
if (this->GetNumberOfOutputs()<3)
    {
    return 0;
    }
  return static_cast<TOutputDisparityImage *>(this->itk::ProcessObject::GetOutput(2));
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::SetHorizontalDisparityInput( const TOutputDisparityImage * hfield)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(4, const_cast<TOutputDisparityImage *>( hfield ));
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::SetVerticalDisparityInput( const TOutputDisparityImage * vfield)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(5, const_cast<TOutputDisparityImage *>( vfield ));
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TOutputDisparityImage *
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetHorizontalDisparityInput() const
{
  if(this->GetNumberOfInputs()<5)
    {
    return 0;
    }
  return static_cast<const TOutputDisparityImage *>(this->itk::ProcessObject::GetInput(4));
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TOutputDisparityImage *
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetVerticalDisparityInput() const
{
  if(this->GetNumberOfInputs()<6)
    {
    return 0;
    }
  return static_cast<const TOutputDisparityImage *>(this->itk::ProcessObject::GetInput(5));
}


template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GenerateInputRequestedRegion()
{
  // Call superclass implementation
  Superclass::GenerateInputRequestedRegion();

  // Retrieve input pointers
  TInputImage * inLeftPtr  = const_cast<TInputImage *>(this->GetLeftInput());
  TInputImage * inRightPtr = const_cast<TInputImage *>(this->GetRightInput());
  TMaskImage *  inLeftMaskPtr  = const_cast<TMaskImage * >(this->GetLeftMaskInput());
  TMaskImage *  inRightMaskPtr  = const_cast<TMaskImage * >(this->GetRightMaskInput());
  TOutputDisparityImage * inHDispPtr = const_cast<TOutputDisparityImage * >(this->GetHorizontalDisparityInput());
  TOutputDisparityImage * inVDispPtr = const_cast<TOutputDisparityImage * >(this->GetVerticalDisparityInput());

  TOutputMetricImage    * outMetricPtr = this->GetMetricOutput();
  TOutputDisparityImage * outHDispPtr = this->GetHorizontalDisparityOutput();
  TOutputDisparityImage * outVDispPtr = this->GetVerticalDisparityOutput();

  // Check pointers before using them
  if(!inLeftPtr || !inRightPtr || !outMetricPtr || !outHDispPtr || !outVDispPtr)
    {
    return;
    }

  // Now, we impose that both inputs have the same size
  if(inLeftPtr->GetLargestPossibleRegion()
     != inRightPtr->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<<"Left and right images do not have the same size ! Left largest region: "<<inLeftPtr->GetLargestPossibleRegion()<<", right largest region: "<<inRightPtr->GetLargestPossibleRegion());
    }

  // We also check that left mask image has same size if present
  if(inLeftMaskPtr && inLeftPtr->GetLargestPossibleRegion() != inLeftMaskPtr->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<<"Left and mask images do not have the same size ! Left largest region: "<<inLeftPtr->GetLargestPossibleRegion()<<", mask largest region: "<<inLeftMaskPtr->GetLargestPossibleRegion());
    }
  
  // We also check that right mask image has same size if present
  if(inRightMaskPtr && inRightPtr->GetLargestPossibleRegion() != inRightMaskPtr->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<<"Right and mask images do not have the same size ! Right largest region: "<<inRightPtr->GetLargestPossibleRegion()<<", mask largest region: "<<inRightMaskPtr->GetLargestPossibleRegion());
    }
  
  // We check that the input initial disparity maps have the same size if present
  if (inHDispPtr && inLeftPtr->GetLargestPossibleRegion() != inHDispPtr->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<<"Left image and initial horizontal disparity map do not have the same size ! Left largest region: "<<inLeftPtr->GetLargestPossibleRegion()<<", horizontal disparity largest region: "<<inHDispPtr->GetLargestPossibleRegion());
    }
  if (inVDispPtr && inLeftPtr->GetLargestPossibleRegion() != inVDispPtr->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<<"Left image and initial vertical disparity map do not have the same size ! Left largest region: "<<inLeftPtr->GetLargestPossibleRegion()<<", vertical disparity largest region: "<<inVDispPtr->GetLargestPossibleRegion());
    }

  // Retrieve requested region (TODO: check if we need to handle
  // region for outHDispPtr)
  RegionType outputRequestedRegion = outMetricPtr->GetRequestedRegion();
  
  // Pad by the appropriate radius
  RegionType inputLeftRegion  = outputRequestedRegion;
  inputLeftRegion.PadByRadius(m_Radius);

  // Now, we must find the corresponding region in moving image
  IndexType rightRequestedRegionIndex = inputLeftRegion.GetIndex();
  rightRequestedRegionIndex[0]+=m_MinimumHorizontalDisparity;
  rightRequestedRegionIndex[1]+=m_MinimumVerticalDisparity;

  SizeType rightRequestedRegionSize = inputLeftRegion.GetSize();
  rightRequestedRegionSize[0]+= m_MaximumHorizontalDisparity - m_MinimumHorizontalDisparity;
  rightRequestedRegionSize[1]+= m_MaximumVerticalDisparity - m_MinimumVerticalDisparity;

  RegionType inputRightRegion;
  inputRightRegion.SetIndex(rightRequestedRegionIndex);
  inputRightRegion.SetSize(rightRequestedRegionSize);

  // crop the left region at the left's largest possible region
  if ( inputLeftRegion.Crop(inLeftPtr->GetLargestPossibleRegion()))
    {
    inLeftPtr->SetRequestedRegion( inputLeftRegion );
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.
    // store what we tried to request (prior to trying to crop)
    inLeftPtr->SetRequestedRegion( inputLeftRegion );

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    std::ostringstream msg;
    msg << this->GetNameOfClass()
                << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region of left image.");
    e.SetDataObject(inLeftPtr);
    throw e;
    }


  // crop the right region at the right's largest possible region
  if ( inputRightRegion.Crop(inRightPtr->GetLargestPossibleRegion()))
    {
    inRightPtr->SetRequestedRegion( inputRightRegion );
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.
    // store what we tried to request (prior to trying to crop)
    inRightPtr->SetRequestedRegion( inputRightRegion );

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    std::ostringstream msg;
    msg << this->GetNameOfClass()
                << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region of right image.");
    e.SetDataObject(inRightPtr);
    throw e;
    }

  if(inLeftMaskPtr)
    {
    // no need to crop the mask region : left mask and left image have same largest possible region
    inLeftMaskPtr->SetRequestedRegion( inputLeftRegion );
    }
  
  if(inRightMaskPtr)
    {
    // no need to crop the mask region : right mask and right image have same largest possible region
    inRightMaskPtr->SetRequestedRegion( inputRightRegion );
    }
    
  if (inHDispPtr && inVDispPtr)
    {
    inHDispPtr->SetRequestedRegion( inputLeftRegion );
    inVDispPtr->SetRequestedRegion( inputLeftRegion );
    }
    
}
template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::BeforeThreadedGenerateData()
{
  TOutputMetricImage    * outMetricPtr = this->GetMetricOutput();
  TOutputDisparityImage * outHDispPtr   = this->GetHorizontalDisparityOutput();
  TOutputDisparityImage * outVDispPtr   = this->GetVerticalDisparityOutput();

  // Fill buffers with default values
  outMetricPtr->FillBuffer(0.);
  outHDispPtr->FillBuffer(m_MaximumHorizontalDisparity);
  outVDispPtr->FillBuffer(m_MinimumVerticalDisparity);
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::ThreadedGenerateData(const RegionType& outputRegionForThread, int threadId)
{
  // Retrieve pointers
  const TInputImage *     inLeftPtr    = this->GetLeftInput();
  const TInputImage *     inRightPtr   = this->GetRightInput();
  const TMaskImage  *     inLeftMaskPtr    = this->GetLeftMaskInput();
  const TMaskImage  *     inRightMaskPtr    = this->GetRightMaskInput();
  const TOutputDisparityImage * inHDispPtr = this->GetHorizontalDisparityInput();
  const TOutputDisparityImage * inVDispPtr = this->GetVerticalDisparityInput();
  TOutputMetricImage    * outMetricPtr = this->GetMetricOutput();
  TOutputDisparityImage * outHDispPtr   = this->GetHorizontalDisparityOutput();
  TOutputDisparityImage * outVDispPtr   = this->GetVerticalDisparityOutput();

  // Set-up progress reporting (this is not exact, since we do not
  // account for pixels that are out of range for a given disparity
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels()*(m_MaximumHorizontalDisparity - m_MinimumHorizontalDisparity + 1)*(m_MaximumVerticalDisparity - m_MinimumVerticalDisparity + 1),100);

  
  // Handle initialisation properly
  typename InputMaskImageType::Pointer initMaskPtr = InputMaskImageType::New();
  initMaskPtr->SetRegions(outputRegionForThread);
  initMaskPtr->Allocate();
  initMaskPtr->FillBuffer(0);

  // Check if we use initial disparities and exploration radius
  bool useExplorationRadius = false;
  bool useInitDispMaps = false;
  if (m_ExplorationRadius[0] >= 1 || m_ExplorationRadius[1] >= 1)
    {
    useExplorationRadius = true;
    if (inHDispPtr && inVDispPtr)
      {
      useInitDispMaps = true;
      }
    }
  // We loop on disparities
  for(int vdisparity = m_MinimumVerticalDisparity; vdisparity <= m_MaximumVerticalDisparity; ++vdisparity)
    {
  for(int hdisparity = m_MinimumHorizontalDisparity; hdisparity <= m_MaximumHorizontalDisparity; ++hdisparity)
    {
    // First, we cast output region to the right image
    IndexType rightRequestedRegionIndex = outputRegionForThread.GetIndex();
    rightRequestedRegionIndex[0]+=hdisparity;
    rightRequestedRegionIndex[1]+=vdisparity;

    // We crop
    RegionType inputRightRegion;
    inputRightRegion.SetIndex(rightRequestedRegionIndex);
    inputRightRegion.SetSize(outputRegionForThread.GetSize());
    inputRightRegion.Crop(inRightPtr->GetLargestPossibleRegion());

    // And then cast back
    IndexType leftRequestedRegionIndex = inputRightRegion.GetIndex();
    leftRequestedRegionIndex[0]-=hdisparity;
    leftRequestedRegionIndex[1]-=vdisparity;

    RegionType inputLeftRegion;
    inputLeftRegion.SetIndex(leftRequestedRegionIndex);
    inputLeftRegion.SetSize(inputRightRegion.GetSize());

    // Define iterators
    itk::ConstNeighborhoodIterator<TInputImage>     leftIt(m_Radius,inLeftPtr,inputLeftRegion);
    itk::ConstNeighborhoodIterator<TInputImage>     rightIt(m_Radius,inRightPtr,inputRightRegion);
    itk::ImageRegionIterator<TOutputMetricImage>    outMetricIt(outMetricPtr,inputLeftRegion);
    itk::ImageRegionIterator<TOutputDisparityImage> outHDispIt(outHDispPtr,inputLeftRegion);
    itk::ImageRegionIterator<TOutputDisparityImage> outVDispIt(outVDispPtr,inputLeftRegion);
    itk::ImageRegionConstIterator<TMaskImage>       inLeftMaskIt;
    itk::ImageRegionConstIterator<TMaskImage>       inRightMaskIt;
    itk::ImageRegionConstIterator<TOutputDisparityImage>       inHDispIt;
    itk::ImageRegionConstIterator<TOutputDisparityImage>       inVDispIt;
    itk::ImageRegionIterator<TMaskImage>            initIt(initMaskPtr,inputLeftRegion);

    itk::ConstantBoundaryCondition<TInputImage> nbc1;
    itk::ConstantBoundaryCondition<TInputImage> nbc2;
    
    leftIt.OverrideBoundaryCondition(&nbc1);
    rightIt.OverrideBoundaryCondition(&nbc2);

    // If we have a mask, define the iterator
    if(inLeftMaskPtr)
      {
      inLeftMaskIt = itk::ImageRegionConstIterator<TMaskImage>(inLeftMaskPtr,inputLeftRegion);
      inLeftMaskIt.GoToBegin();
      }
    if(inRightMaskPtr)
      {
      inRightMaskIt = itk::ImageRegionConstIterator<TMaskImage>(inRightMaskPtr,inputRightRegion);
      inRightMaskIt.GoToBegin();
      }
    
    // If we use initial disparity maps, define the iterators
    if (useInitDispMaps)
      {
      inHDispIt = itk::ImageRegionConstIterator<TOutputDisparityImage>(inHDispPtr,inputLeftRegion);
      inVDispIt = itk::ImageRegionConstIterator<TOutputDisparityImage>(inVDispPtr,inputLeftRegion);
      inHDispIt.GoToBegin();
      inVDispIt.GoToBegin();
      }
    
    // Initialize iterators
    leftIt.GoToBegin();
    rightIt.GoToBegin();
    outMetricIt.GoToBegin();
    outHDispIt.GoToBegin();
    outVDispIt.GoToBegin();
    initIt.GoToBegin();

    // Loop on pixels
    while(!leftIt.IsAtEnd()
          || !rightIt.IsAtEnd()
          || !outMetricIt.IsAtEnd()
          || !outHDispIt.IsAtEnd()
          || !outVDispIt.IsAtEnd()
          || !initIt.IsAtEnd())
      {
      
      // If the mask is present and valid
      if(!inLeftMaskPtr || (inLeftMaskPtr && inLeftMaskIt.Get() > 0) )
        {
        if(!inRightMaskPtr || (inRightMaskPtr && inRightMaskIt.Get() > 0) )
          {
          int estimatedMinHDisp = m_MinimumHorizontalDisparity;
          int estimatedMinVDisp = m_MinimumVerticalDisparity;
          int estimatedMaxHDisp = m_MaximumHorizontalDisparity;
          int estimatedMaxVDisp = m_MaximumVerticalDisparity;
          if (useExplorationRadius)
            {
            // compute disparity bounds from initial position and exploration radius
            if (useInitDispMaps)
              {
              estimatedMinHDisp = inHDispIt.Get() - m_ExplorationRadius[0];
              estimatedMinVDisp = inVDispIt.Get() - m_ExplorationRadius[1];
              estimatedMaxHDisp = inHDispIt.Get() + m_ExplorationRadius[0];
              estimatedMaxVDisp = inVDispIt.Get() + m_ExplorationRadius[1];
              }
            else
              {
              estimatedMinHDisp = m_InitHorizontalDisparity - m_ExplorationRadius[0];
              estimatedMinVDisp = m_InitVerticalDisparity - m_ExplorationRadius[1];
              estimatedMaxHDisp = m_InitHorizontalDisparity + m_ExplorationRadius[0];
              estimatedMaxVDisp = m_InitVerticalDisparity + m_ExplorationRadius[1];
              }
            // clamp to the minimum disparities
            if (estimatedMinHDisp < m_MinimumHorizontalDisparity)
              {
              estimatedMinHDisp = m_MinimumHorizontalDisparity;
              }
            if (estimatedMinVDisp < m_MinimumVerticalDisparity)
              {
              estimatedMinVDisp = m_MinimumVerticalDisparity;
              }
            }
          
          if (vdisparity >= estimatedMinVDisp && vdisparity <= estimatedMaxVDisp &&
              hdisparity >= estimatedMinHDisp && hdisparity <= estimatedMaxHDisp)
            {
            // Compute the block matching value
          double metric = m_Functor(leftIt,rightIt);
    
            // If we are at first loop, fill both outputs
          if(initIt.Get()==0)
              {
              outHDispIt.Set(hdisparity);
              outVDispIt.Set(vdisparity);
              outMetricIt.Set(metric);
              initIt.Set(1);
              }
            else if(m_Minimize && metric < outMetricIt.Get())
              {
              outHDispIt.Set(hdisparity);
              outVDispIt.Set(vdisparity);
              outMetricIt.Set(metric);
              }
            else if(!m_Minimize && metric > outMetricIt.Get())
              {
              outHDispIt.Set(hdisparity);
              outVDispIt.Set(vdisparity);
              outMetricIt.Set(metric);
              }
            }
          }
        }
      ++leftIt;
      ++rightIt;
      ++outMetricIt;
      ++outHDispIt;
      ++outVDispIt;
      ++initIt;

      if(inLeftMaskPtr)
        {
        ++inLeftMaskIt;
        }
      
      if(inRightMaskPtr)
        {
        ++inRightMaskIt;
        }
      
      if(useInitDispMaps)
        {
        ++inHDispIt;
        ++inVDispIt;
        }
        progress.CompletedPixel();
      }
      
    }
    }
}

} // End namespace otb

#endif
