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
#ifndef __otbHorizontalPixelWiseBlockMatchingImageFilter_txx
#define __otbHorizontalPixelWiseBlockMatchingImageFilter_txx

#include "otbHorizontalPixelWiseBlockMatchingImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"

#include "itkProgressReporter.h"

namespace otb
{
template <class TInputImage, class TOutputMetricImage, 
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
HorizontalPixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::HorizontalPixelWiseBlockMatchingImageFilter()
{
  // Set the number of inputs
  this->SetNumberOfInputs(3);
  this->SetNumberOfRequiredInputs(2);

  // Set the outputs
  this->SetNumberOfOutputs(2);
  this->SetNthOutput(0,TOutputMetricImage::New());
  this->SetNthOutput(1,TOutputDisparityImage::New());

  // Default parameters
  m_Radius.Fill(2);

  // Minimize by default
  m_Minimize = true;

  // Default disparity range
  m_MinimumDisparity = -10;
  m_MaximumDisparity =  10;
}


template <class TInputImage, class TOutputMetricImage, 
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
HorizontalPixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::~HorizontalPixelWiseBlockMatchingImageFilter()
{}


template <class TInputImage, class TOutputMetricImage, 
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
HorizontalPixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::SetLeftInput(const TInputImage * image)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<TInputImage *>( image ));
}

template <class TInputImage, class TOutputMetricImage, 
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
HorizontalPixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::SetRightInput(const TInputImage * image)
{
// Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<TInputImage *>( image ));
}

template <class TInputImage, class TOutputMetricImage, 
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
HorizontalPixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::SetMaskInput(const TMaskImage * image)
{
// Process object is not const-correct so the const casting is required.
  this->SetNthInput(2, const_cast<TMaskImage *>( image ));
}

template <class TInputImage, class TOutputMetricImage, 
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TInputImage * 
HorizontalPixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
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
HorizontalPixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
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
HorizontalPixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetMaskInput() const
{
  if(this->GetNumberOfInputs()<3)
    {
    return 0;
    }
  return static_cast<const TMaskImage *>(this->itk::ProcessObject::GetInput(2));
}

template <class TInputImage, class TOutputMetricImage, 
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TOutputMetricImage * 
HorizontalPixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
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
HorizontalPixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
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
HorizontalPixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetDisparityOutput() const
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
HorizontalPixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetDisparityOutput()
{
if (this->GetNumberOfOutputs()<2)
    {
    return 0;
    }
  return static_cast<TOutputDisparityImage *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputMetricImage, 
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
HorizontalPixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GenerateInputRequestedRegion()
{
  // Call superclass implementation
  Superclass::GenerateInputRequestedRegion();

  // Retrieve input pointers
  TInputImage * inLeftPtr  = const_cast<TInputImage *>(this->GetLeftInput());
  TInputImage * inRightPtr = const_cast<TInputImage *>(this->GetRightInput());
  TMaskImage *  inMaskPtr  = const_cast<TMaskImage * >(this->GetMaskInput());

  TOutputMetricImage    * outMetricPtr = this->GetMetricOutput();
  TOutputDisparityImage * outDispPtr = this->GetDisparityOutput();
  
  // Check pointers before using them
  if(!inLeftPtr || !inRightPtr || !outMetricPtr || !outDispPtr)
    {
    return;
    }

  // Now, we impose that both inputs have the same size
  if(inLeftPtr->GetLargestPossibleRegion()
     != inRightPtr->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<<"Left and right images do not have the same size ! Left largest region: "<<inLeftPtr->GetLargestPossibleRegion()<<", right largest region: "<<inRightPtr->GetLargestPossibleRegion());
    }

  // We also check that mask image has same size if present
  if(inMaskPtr && inLeftPtr->GetLargestPossibleRegion() != inMaskPtr->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<<"Left and mask images do not have the same size ! Left largest region: "<<inLeftPtr->GetLargestPossibleRegion()<<", mask largest region: "<<inMaskPtr->GetLargestPossibleRegion());
    }

  // Retrieve requested region (TODO: check if we need to handle
  // region for outDispPtr)
  RegionType outputRequestedRegion = outMetricPtr->GetRequestedRegion();
  
  // Pad by the appropriate radius
  RegionType inputLeftRegion  = outputRequestedRegion;
  inputLeftRegion.PadByRadius(m_Radius);

  // Now, we must find the corresponding region in moving image
  IndexType rightRequestedRegionIndex = inputLeftRegion.GetIndex();
  rightRequestedRegionIndex[0]+=m_MinimumDisparity;
  
  SizeType rightRequestedRegionSize = inputLeftRegion.GetSize();
  rightRequestedRegionSize[0]+= m_MaximumDisparity - m_MinimumDisparity;
  
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

  if(inMaskPtr)
    {
    // crop the mask region at the mask's largest possible region
    if ( inputLeftRegion.Crop(inMaskPtr->GetLargestPossibleRegion()))
      {
      inMaskPtr->SetRequestedRegion( inputLeftRegion );
      }
    else
      {
      // Couldn't crop the region (requested region is outside the largest
      // possible region).  Throw an exception.
      // store what we tried to request (prior to trying to crop)
      inMaskPtr->SetRequestedRegion( inputLeftRegion );
      
      // build an exception
      itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
      std::ostringstream msg;
      msg << this->GetNameOfClass()
          << "::GenerateInputRequestedRegion()";
      e.SetLocation(msg.str().c_str());
      e.SetDescription("Requested region is (at least partially) outside the largest possible region of mask image.");
      e.SetDataObject(inMaskPtr);
      throw e;
      }
    }
}
template <class TInputImage, class TOutputMetricImage, 
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
HorizontalPixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::BeforeThreadedGenerateData()
{
  TOutputMetricImage    * outMetricPtr = this->GetMetricOutput();
  TOutputDisparityImage * outDispPtr   = this->GetDisparityOutput();

  // Fill buffers with default values
  outMetricPtr->FillBuffer(0.);
  outDispPtr->FillBuffer(m_MinimumDisparity);
}

template <class TInputImage, class TOutputMetricImage, 
class TOutputDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
HorizontalPixelWiseBlockMatchingImageFilter<TInputImage,TOutputMetricImage,
TOutputDisparityImage,TMaskImage,TBlockMatchingFunctor>
::ThreadedGenerateData(const RegionType& outputRegionForThread, int threadId)
{
  // Retrieve pointers
  const TInputImage *     inLeftPtr    = this->GetLeftInput();
  const TInputImage *     inRightPtr   = this->GetRightInput();
  const TMaskImage  *     inMaskPtr    = this->GetMaskInput();
  TOutputMetricImage    * outMetricPtr = this->GetMetricOutput();
  TOutputDisparityImage * outDispPtr   = this->GetDisparityOutput();

  // Set-up progress reporting (this is not exact, since we do not
  // account for pixels that are out of range for a given disparity
  itk::ProgressReporter progress(this, 0, outputRegionForThread.GetNumberOfPixels()*(m_MaximumDisparity - m_MinimumDisparity));

  // Define the block matching functor
  TBlockMatchingFunctor bmFunctor;

    // std::cout<<"Output region: "<<outputRegionForThread<<std::endl;


  // We loop on disparities
  for(int disparity = m_MinimumDisparity; disparity <= m_MaximumDisparity; ++disparity)
    {
    // First, we cast output region to the right image
    IndexType rightRequestedRegionIndex = outputRegionForThread.GetIndex();
    rightRequestedRegionIndex[0]+=disparity;
    // We crop
    RegionType inputRightRegion;
    inputRightRegion.SetIndex(rightRequestedRegionIndex);
    inputRightRegion.SetSize(outputRegionForThread.GetSize());
    inputRightRegion.Crop(inRightPtr->GetLargestPossibleRegion());

    // And then cast back
    IndexType leftRequestedRegionIndex = inputRightRegion.GetIndex();
    leftRequestedRegionIndex[0]-=disparity;
  
    RegionType inputLeftRegion;
    inputLeftRegion.SetIndex(leftRequestedRegionIndex);
    inputLeftRegion.SetSize(inputRightRegion.GetSize());

    // std::cout<<"Disparity: "<<disparity<<std::endl;
    // std::cout<<"Left  region: "<<inputLeftRegion<<std::endl;
    // std::cout<<"Right region: "<<inputRightRegion<<std::endl;

    // Define iterators
    itk::ConstNeighborhoodIterator<TInputImage>     leftIt(m_Radius,inLeftPtr,inputLeftRegion);
    itk::ConstNeighborhoodIterator<TInputImage>     rightIt(m_Radius,inRightPtr,inputRightRegion);
    itk::ImageRegionIterator<TOutputMetricImage>    outMetricIt(outMetricPtr,inputLeftRegion);
    itk::ImageRegionIterator<TOutputDisparityImage> outDispIt(outDispPtr,inputLeftRegion);

    itk::ImageRegionConstIterator<TMaskImage>       inMaskIt;

    // If we have a mask, define the iterator
    if(inMaskPtr)
      {
      inMaskIt = itk::ImageRegionConstIterator<TMaskImage>(inMaskPtr,inputLeftRegion);
      inMaskIt.GoToBegin();
      }
    
    // Initialize iterators
    leftIt.GoToBegin();
    rightIt.GoToBegin();
    outMetricIt.GoToBegin();
    outDispIt.GoToBegin();

    // Loop on pixels
    while(!leftIt.IsAtEnd() 
          || !rightIt.IsAtEnd() 
          || !outMetricIt.IsAtEnd() 
          || !outDispIt.IsAtEnd())
      {
      
      // If the mask is present and valid
      if(!inMaskPtr || (inMaskPtr && inMaskIt.Get() > 0) )
        {
        // Compute the block matching value
        double metric = bmFunctor(leftIt,rightIt);

        // If we are at first loop, fill both outputs
        if(disparity == m_MinimumDisparity)
          {
          outDispIt.Set(disparity);
          outMetricIt.Set(metric);
          }
        else if(m_Minimize && metric < outMetricIt.Get())
          {
          outDispIt.Set(disparity);
          outMetricIt.Set(metric);
          }
        else if(!m_Minimize && metric > outMetricIt.Get())
          {
          outDispIt.Set(disparity);
          outMetricIt.Set(metric);
          }
        }

      progress.CompletedPixel();

      ++leftIt;
      ++rightIt;
      ++outMetricIt;
      ++outDispIt;

      if(inMaskPtr)
        {
        ++inMaskIt;
        }
      }
    }
}
} // End namespace otb

#endif
