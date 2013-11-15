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
  this->SetNumberOfRequiredInputs(6);
  this->SetNumberOfRequiredInputs(2);

  // Set the outputs
  this->SetNumberOfRequiredOutputs(3);
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

  // Default step
  m_Step = 1;

  // Default grid index
  m_GridIndex[0] = 0;
  m_GridIndex[1] = 0;
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
::GenerateOutputInformation()
{
  // Call superclass implementation
  Superclass::GenerateOutputInformation();

  // Sanity check
  if (this->m_Step == 0) this->m_Step = 1;
  this->m_GridIndex[0] = this->m_GridIndex[0] % this->m_Step;
  this->m_GridIndex[1] = this->m_GridIndex[1] % this->m_Step;

  // Modify output size and index depending on the step
  const TInputImage * inLeftPtr  = this->GetLeftInput();

  RegionType outputLargest = this->ConvertFullToSubsampledRegion(inLeftPtr->GetLargestPossibleRegion(), this->m_Step, this->m_GridIndex);

  TOutputMetricImage    * outMetricPtr = const_cast<TOutputMetricImage * >(this->GetMetricOutput());
  TOutputDisparityImage * outHDispPtr = const_cast<TOutputDisparityImage * >(this->GetHorizontalDisparityOutput());
  TOutputDisparityImage * outVDispPtr = const_cast<TOutputDisparityImage * >(this->GetVerticalDisparityOutput());

  outMetricPtr->SetLargestPossibleRegion(outputLargest);
  outHDispPtr->SetLargestPossibleRegion(outputLargest);
  outVDispPtr->SetLargestPossibleRegion(outputLargest);

  // Adapt spacing
  SpacingType outSpacing = inLeftPtr->GetSpacing();
  outSpacing[0] *= static_cast<double>(this->m_Step);
  outSpacing[1] *= static_cast<double>(this->m_Step);

  outMetricPtr->SetSpacing(outSpacing);
  outHDispPtr->SetSpacing(outSpacing);
  outVDispPtr->SetSpacing(outSpacing);

  // Adapt origin
  PointType outOrigin = inLeftPtr->GetOrigin();
  SpacingType inSpacing = inLeftPtr->GetSpacing();
  outOrigin[0] += inSpacing[0] * static_cast<double>(this->m_GridIndex[0]);
  outOrigin[1] += inSpacing[1] * static_cast<double>(this->m_GridIndex[1]);

  outMetricPtr->SetOrigin(outOrigin);
  outHDispPtr->SetOrigin(outOrigin);
  outVDispPtr->SetOrigin(outOrigin);
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

  // Sanity check
  if (this->m_Step == 0) this->m_Step = 1;
  this->m_GridIndex[0] = this->m_GridIndex[0] % this->m_Step;
  this->m_GridIndex[1] = this->m_GridIndex[1] % this->m_Step;

  // Retrieve requested region (TODO: check if we need to handle
  // region for outHDispPtr)
  RegionType inputLeftRegion = this->ConvertSubsampledToFullRegion(outMetricPtr->GetRequestedRegion(), this->m_Step, this->m_GridIndex);

  // Pad by the appropriate radius
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

  // Sanity check
  if (this->m_Step == 0) this->m_Step = 1;
  this->m_GridIndex[0] = this->m_GridIndex[0] % this->m_Step;
  this->m_GridIndex[1] = this->m_GridIndex[1] % this->m_Step;

  // Fill buffers with default values
  outMetricPtr->FillBuffer(0.);
  outHDispPtr->FillBuffer(static_cast<DisparityPixelType>(m_MaximumHorizontalDisparity) /
                          static_cast<DisparityPixelType>(m_Step));
  outVDispPtr->FillBuffer(static_cast<DisparityPixelType>(m_MinimumVerticalDisparity) /
                          static_cast<DisparityPixelType>(m_Step));
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
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

  // Compute region for thread at full resolution
  RegionType fullRegionForThread = this->ConvertSubsampledToFullRegion(outputRegionForThread, this->m_Step, this->m_GridIndex);

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

  // step value as disparityType
  DisparityPixelType stepDisparityInv = 1. / static_cast<DisparityPixelType>(this->m_Step);

  // We loop on disparities
  for(int vdisparity = m_MinimumVerticalDisparity; vdisparity <= m_MaximumVerticalDisparity; ++vdisparity)
    {
  for(int hdisparity = m_MinimumHorizontalDisparity; hdisparity <= m_MaximumHorizontalDisparity; ++hdisparity)
    {
    // First, we cast output region to the right image
    IndexType rightRequestedRegionIndex = fullRegionForThread.GetIndex();
    rightRequestedRegionIndex[0]+=hdisparity;
    rightRequestedRegionIndex[1]+=vdisparity;

    // We crop
    RegionType inputRightRegion;
    inputRightRegion.SetIndex(rightRequestedRegionIndex);
    inputRightRegion.SetSize(fullRegionForThread.GetSize());
    inputRightRegion.Crop(inRightPtr->GetLargestPossibleRegion());

    // And then cast back
    IndexType leftRequestedRegionIndex = inputRightRegion.GetIndex();
    leftRequestedRegionIndex[0]-=hdisparity;
    leftRequestedRegionIndex[1]-=vdisparity;

    RegionType inputLeftRegion;
    inputLeftRegion.SetIndex(leftRequestedRegionIndex);
    inputLeftRegion.SetSize(inputRightRegion.GetSize());

    // Compute the equivalent region in subsampled grid
    RegionType outputRegion = this->ConvertFullToSubsampledRegion(inputLeftRegion, this->m_Step, this->m_GridIndex);

    // Define iterators
    itk::ConstNeighborhoodIterator<TInputImage>     leftIt(m_Radius,inLeftPtr,inputLeftRegion);
    itk::ConstNeighborhoodIterator<TInputImage>     rightIt(m_Radius,inRightPtr,inputRightRegion);
    itk::ImageRegionIterator<TOutputMetricImage>    outMetricIt(outMetricPtr,outputRegion);
    itk::ImageRegionIterator<TOutputDisparityImage> outHDispIt(outHDispPtr,outputRegion);
    itk::ImageRegionIterator<TOutputDisparityImage> outVDispIt(outVDispPtr,outputRegion);
    itk::ImageRegionConstIterator<TMaskImage>       inLeftMaskIt;
    itk::ImageRegionConstIterator<TMaskImage>       inRightMaskIt;
    itk::ImageRegionConstIterator<TOutputDisparityImage>       inHDispIt;
    itk::ImageRegionConstIterator<TOutputDisparityImage>       inVDispIt;
    itk::ImageRegionIterator<TMaskImage>            initIt(initMaskPtr,outputRegion);

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
      // If the pixel location is on the subsampled grid
      IndexType tmpIndex = leftIt.GetIndex(leftIt.GetCenterNeighborhoodIndex());
      if (((tmpIndex[0] - this->m_GridIndex[0] + this->m_Step) % this->m_Step == 0) &&
          ((tmpIndex[1] - this->m_GridIndex[1] + this->m_Step) % this->m_Step == 0))
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
              // We adapt the disparity value to keep consistant with disparity map index space
            if(initIt.Get()==0)
                {
                outHDispIt.Set(static_cast<DisparityPixelType>(hdisparity) * stepDisparityInv);
                outVDispIt.Set(static_cast<DisparityPixelType>(vdisparity) * stepDisparityInv);
                outMetricIt.Set(metric);
                initIt.Set(1);
                }
              else if(m_Minimize && metric < outMetricIt.Get())
                {
                outHDispIt.Set(static_cast<DisparityPixelType>(hdisparity) * stepDisparityInv);
                outVDispIt.Set(static_cast<DisparityPixelType>(vdisparity) * stepDisparityInv);
                outMetricIt.Set(metric);
                }
              else if(!m_Minimize && metric > outMetricIt.Get())
                {
                outHDispIt.Set(static_cast<DisparityPixelType>(hdisparity) * stepDisparityInv);
                outVDispIt.Set(static_cast<DisparityPixelType>(vdisparity) * stepDisparityInv);
                outMetricIt.Set(metric);
                }
              }
            }
          }
        ++outMetricIt;
        ++outHDispIt;
        ++outVDispIt;
        ++initIt;
        progress.CompletedPixel();
        }
      ++leftIt;
      ++rightIt;

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
      }

    }
    }
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
typename PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>::RegionType
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::ConvertFullToSubsampledRegion(RegionType full, unsigned int step, IndexType index)
{
  IndexType shiftedFull = full.GetIndex();
  shiftedFull[0] -= index[0];
  shiftedFull[1] -= index[1];

  IndexType subIndex;
  subIndex[0] = (shiftedFull[0]) / step;
  subIndex[1] = (shiftedFull[1]) / step;
  if (shiftedFull[0] % step) ++subIndex[0];
  if (shiftedFull[1] % step) ++subIndex[1];

  if (shiftedFull[0]<0) subIndex[0] = 0;
  if (shiftedFull[1]<0) subIndex[1] = 0;

  SizeType subSize;
  subSize[0] = (full.GetSize(0) - (subIndex[0] * step) + shiftedFull[0]) / step;
  subSize[1] = (full.GetSize(1) - (subIndex[1] * step) + shiftedFull[1]) / step;

  if ((full.GetSize(0) - (subIndex[0] * step) + shiftedFull[0]) % step)
    ++subSize[0];
  if ((full.GetSize(1) - (subIndex[1] * step) + shiftedFull[1]) % step)
    ++subSize[1];

  RegionType subRegion;
  subRegion.SetIndex(subIndex);
  subRegion.SetSize(subSize);
  return subRegion;
}

template <class TInputImage, class TOutputMetricImage,
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
typename PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>::RegionType
PixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::ConvertSubsampledToFullRegion(RegionType sub, unsigned int step, IndexType index)
{
  IndexType fullIndex;
  fullIndex[0] = sub.GetIndex(0) * step + index[0];
  fullIndex[1] = sub.GetIndex(1) * step + index[1];

  SizeType fullSize;
  fullSize[0] = sub.GetSize(0) * step;
  fullSize[1] = sub.GetSize(1) * step;
  if (fullSize[0] > 0) fullSize[0] -= (step - 1);
  if (fullSize[1] > 0) fullSize[1] -= (step - 1);

  RegionType fullRegion;
  fullRegion.SetIndex(fullIndex);
  fullRegion.SetSize(fullSize);
  return fullRegion;
}

} // End namespace otb

#endif
