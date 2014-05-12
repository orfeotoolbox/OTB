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
#ifndef __otbSubPixelDisparityImageFilter_txx
#define __otbSubPixelDisparityImageFilter_txx


namespace otb
{
template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::SubPixelDisparityImageFilter()
{
// Set the number of required inputs
  this->SetNumberOfRequiredInputs(3);

  // Set the outputs
  this->SetNumberOfRequiredOutputs(3);
  this->SetNthOutput(0,TDisparityImage::New());
  this->SetNthOutput(1,TDisparityImage::New());
  this->SetNthOutput(2,TOutputMetricImage::New());

  // Default parameters
  m_Radius.Fill(2);

  // Minimize by default
  m_Minimize = true;

  // Default disparity range
  m_MinimumHorizontalDisparity = -10;
  m_MaximumHorizontalDisparity =  10;
  m_MinimumVerticalDisparity = 0;
  m_MaximumVerticalDisparity = 0;

  // Default refinement method : parabolic
  m_RefineMethod = 0;

  // Default step value
  m_Step = 1;

  // Default grid index
  m_GridIndex[0] = 0;
  m_GridIndex[1] = 0;
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::~SubPixelDisparityImageFilter()
{}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::SetLeftInput(const TInputImage * image)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<TInputImage *>( image ));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::SetRightInput(const TInputImage * image)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<TInputImage *>( image ));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::SetHorizontalDisparityInput( const TDisparityImage * hfield)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(2, const_cast<TDisparityImage *>( hfield ));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::SetVerticalDisparityInput( const TDisparityImage * vfield)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(3, const_cast<TDisparityImage *>( vfield ));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::SetLeftMaskInput(const TMaskImage * image)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(4, const_cast<TMaskImage *>( image ));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::SetRightMaskInput(const TMaskImage * image)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(5, const_cast<TMaskImage *>( image ));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::SetMetricInput(const TOutputMetricImage * image)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(6, const_cast<TOutputMetricImage *>( image ));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TInputImage *
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetLeftInput() const
{
  if (this->GetNumberOfIndexedInputs()<1)
    {
    return 0;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(0));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TInputImage *
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetRightInput() const
{
  if(this->GetNumberOfIndexedInputs()<2)
    {
    return 0;
    }
  return static_cast<const TInputImage *>(this->itk::ProcessObject::GetInput(1));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TDisparityImage *
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetHorizontalDisparityInput() const
{
  if(this->GetNumberOfIndexedInputs()<3)
    {
    return 0;
    }
  return static_cast<const TDisparityImage *>(this->itk::ProcessObject::GetInput(2));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TDisparityImage *
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetVerticalDisparityInput() const
{
  if(this->GetNumberOfIndexedInputs()<4)
    {
    return 0;
    }
  return static_cast<const TDisparityImage *>(this->itk::ProcessObject::GetInput(3));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TMaskImage *
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetLeftMaskInput() const
{
  if(this->GetNumberOfIndexedInputs()<5)
    {
    return 0;
    }
  return static_cast<const TMaskImage *>(this->itk::ProcessObject::GetInput(4));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TMaskImage *
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetRightMaskInput() const
{
  if(this->GetNumberOfIndexedInputs()<6)
    {
    return 0;
    }
  return static_cast<const TMaskImage *>(this->itk::ProcessObject::GetInput(5));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TDisparityImage *
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetHorizontalDisparityOutput() const
{
  if (this->GetNumberOfOutputs()<1)
    {
    return 0;
    }
  return static_cast<const TDisparityImage *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
TDisparityImage *
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetHorizontalDisparityOutput()
{
if (this->GetNumberOfOutputs()<1)
    {
    return 0;
    }
  return static_cast<TDisparityImage *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TDisparityImage *
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetVerticalDisparityOutput() const
{
  if (this->GetNumberOfOutputs()<2)
    {
    return 0;
    }
  return static_cast<const TDisparityImage *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
TDisparityImage *
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetVerticalDisparityOutput()
{
if (this->GetNumberOfOutputs()<2)
    {
    return 0;
    }
  return static_cast<TDisparityImage *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
const TOutputMetricImage *
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetMetricOutput() const
{
  if (this->GetNumberOfOutputs()<3)
    {
    return 0;
    }
  return static_cast<const TOutputMetricImage *>(this->itk::ProcessObject::GetOutput(2));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
TOutputMetricImage *
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GetMetricOutput()
{
  if (this->GetNumberOfOutputs()<3)
    {
    return 0;
    }
  return static_cast<TOutputMetricImage *>(this->itk::ProcessObject::GetOutput(2));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::SetInputsFromBlockMatchingFilter(const BlockMatchingFilterType * filter)
{
  this->SetLeftInput(filter->GetLeftInput());
  this->SetRightInput(filter->GetRightInput());

  this->SetRadius(filter->GetRadius());

  this->SetMinimumHorizontalDisparity(filter->GetMinimumHorizontalDisparity());
  this->SetMaximumHorizontalDisparity(filter->GetMaximumHorizontalDisparity());

  this->SetMinimumVerticalDisparity(filter->GetMinimumVerticalDisparity());
  this->SetMaximumVerticalDisparity(filter->GetMaximumVerticalDisparity());

  this->SetMinimize(filter->GetMinimize());

  this->SetMetricInput(filter->GetMetricOutput());

  if (filter->GetHorizontalDisparityOutput())
    {
    this->SetHorizontalDisparityInput(filter->GetHorizontalDisparityOutput());
    }
  if (filter->GetVerticalDisparityOutput())
    {
    this->SetVerticalDisparityInput(filter->GetVerticalDisparityOutput());
    }
  if (filter->GetLeftMaskInput())
    {
    this->SetLeftMaskInput(filter->GetLeftMaskInput());
    }
  if (filter->GetRightMaskInput())
    {
    this->SetRightMaskInput(filter->GetRightMaskInput());
    }

}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::VerifyInputInformation()
{
  // Retrieve input pointers
  const TInputImage *     inLeftPtr       = this->GetLeftInput();
  const TInputImage *     inRightPtr      = this->GetRightInput();
  const TMaskImage *      inLeftMaskPtr   = this->GetLeftMaskInput();
  const TMaskImage *      inRightMaskPtr  = this->GetRightMaskInput();
  const TDisparityImage * inHDispPtr      = this->GetHorizontalDisparityInput();
  const TDisparityImage * inVDispPtr      = this->GetVerticalDisparityInput();

  const TOutputMetricImage *  outMetricPtr  = this->GetMetricOutput();
  const TDisparityImage *     outHDispPtr   = this->GetHorizontalDisparityOutput();
  const TDisparityImage *     outVDispPtr   = this->GetVerticalDisparityOutput();

  // Check pointers before using them
  if(!inLeftPtr || !inRightPtr)
    {
    itkExceptionMacro(<<"Missing input, need left and right input images.");
    }
  
  if (!inHDispPtr)
    {
    itkExceptionMacro(<<"Input horizontal disparity map is missing");
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
  if (inHDispPtr && inVDispPtr && inHDispPtr->GetLargestPossibleRegion() != inVDispPtr->GetLargestPossibleRegion())
    {
    itkExceptionMacro(<<"Initial horizontal and vertical disparity maps don't have the same size ! Horizontal disparity largest region: "<<inHDispPtr->GetLargestPossibleRegion()<<", vertical disparity largest region: "<<inVDispPtr->GetLargestPossibleRegion());
    }
  
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GenerateOutputInformation()
{
  // Retrieve input pointers
  const TInputImage * inLeftPtr  = this->GetLeftInput();
  const TDisparityImage * inHDispPtr = this->GetHorizontalDisparityInput();

  TOutputMetricImage    * outMetricPtr = this->GetMetricOutput();
  TDisparityImage * outHDispPtr = this->GetHorizontalDisparityOutput();
  TDisparityImage * outVDispPtr = this->GetVerticalDisparityOutput();
  
  outMetricPtr->CopyInformation(inHDispPtr);
  outHDispPtr->CopyInformation(inHDispPtr);
  outVDispPtr->CopyInformation(inHDispPtr);

  // Check the size of the input disparity maps (possible subsampled grid)
  SpacingType leftSpacing = inLeftPtr->GetSpacing();
  SpacingType dispSpacing = inHDispPtr->GetSpacing();
  PointType   leftOrigin  = inLeftPtr->GetOrigin();
  PointType   dispOrigin  = inHDispPtr->GetOrigin();

  double ratioX = dispSpacing[0] / leftSpacing[0];
  double ratioY = dispSpacing[1] / leftSpacing[1];
  int stepX = static_cast<int>(vcl_floor(ratioX + 0.5));
  int stepY = static_cast<int>(vcl_floor(ratioY + 0.5));
  if (stepX < 1 || stepY < 1 || stepX != stepY)
    {
    itkExceptionMacro(<<"Incompatible spacing values between disparity map and input image. Left spacing: "<<leftSpacing<<", disparity spacing: "<< dispSpacing);
    }
  this->m_Step = static_cast<unsigned int>(stepX);

  double shiftX = (dispOrigin[0] - leftOrigin[0]) / leftSpacing[0];
  double shiftY = (dispOrigin[1] - leftOrigin[1]) / leftSpacing[1];
  this->m_GridIndex[0] = static_cast<typename IndexType::IndexValueType>(vcl_floor(shiftX + 0.5));
  this->m_GridIndex[1] = static_cast<typename IndexType::IndexValueType>(vcl_floor(shiftY + 0.5));
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::GenerateInputRequestedRegion()
{
  // Call superclass implementation
  Superclass::GenerateInputRequestedRegion();

  // Retrieve input pointers
  TInputImage * inLeftPtr  = const_cast<TInputImage *>(this->GetLeftInput());
  TInputImage * inRightPtr = const_cast<TInputImage *>(this->GetRightInput());
  TMaskImage *  inLeftMaskPtr  = const_cast<TMaskImage * >(this->GetLeftMaskInput());
  TMaskImage *  inRightMaskPtr  = const_cast<TMaskImage * >(this->GetRightMaskInput());
  TDisparityImage * inHDispPtr = const_cast<TDisparityImage * >(this->GetHorizontalDisparityInput());
  TDisparityImage * inVDispPtr = const_cast<TDisparityImage * >(this->GetVerticalDisparityInput());

  TOutputMetricImage    * outMetricPtr = this->GetMetricOutput();
  TDisparityImage * outHDispPtr = this->GetHorizontalDisparityOutput();
  TDisparityImage * outVDispPtr = this->GetVerticalDisparityOutput();
  
  // Retrieve requested region (TODO: check if we need to handle
  // region for outHDispPtr)
  RegionType outputRequestedRegion = outHDispPtr->GetRequestedRegion();
  RegionType fullRequestedRegion = BlockMatchingFilterType::ConvertSubsampledToFullRegion(outputRequestedRegion, this->m_Step, this->m_GridIndex);

  // Pad by the appropriate radius
  RegionType inputLeftRegion  = fullRequestedRegion;
  inputLeftRegion.PadByRadius(m_Radius);

  // Now, we must find the corresponding region in moving image
  IndexType rightRequestedRegionIndex = fullRequestedRegion.GetIndex();
  rightRequestedRegionIndex[0]+=m_MinimumHorizontalDisparity;
  rightRequestedRegionIndex[1]+=m_MinimumVerticalDisparity;

  SizeType rightRequestedRegionSize = fullRequestedRegion.GetSize();
  rightRequestedRegionSize[0]+= m_MaximumHorizontalDisparity - m_MinimumHorizontalDisparity;
  rightRequestedRegionSize[1]+= m_MaximumVerticalDisparity - m_MinimumVerticalDisparity;

  RegionType inputRightRegion;
  inputRightRegion.SetIndex(rightRequestedRegionIndex);
  inputRightRegion.SetSize(rightRequestedRegionSize);

  RegionType inputRightMaskRegion = inputRightRegion;

  inputRightRegion.PadByRadius(m_Radius);

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
    inputRightMaskRegion.Crop(inRightPtr->GetLargestPossibleRegion());
    }
  else
    {
    // Depending on the minimum and maximum disparities, the right side patch can
    // be outside the image : in this case, just request an empty region
    inputRightRegion.SetIndex(inRightPtr->GetLargestPossibleRegion().GetIndex());
    inputRightRegion.SetSize(0,0);
    inputRightRegion.SetSize(1,0);
    inRightPtr->SetRequestedRegion( inputRightRegion );
    inputRightMaskRegion = inputRightRegion;


//     // Couldn't crop the region (requested region is outside the largest
//     // possible region).  Throw an exception.
//     // store what we tried to request (prior to trying to crop)
//     inRightPtr->SetRequestedRegion( inputRightRegion );
//
//     // build an exception
//     itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
//     std::ostringstream msg;
//     msg << this->GetNameOfClass()
//                 << "::GenerateInputRequestedRegion()";
//     e.SetLocation(msg.str().c_str());
//     e.SetDescription("Requested region is (at least partially) outside the largest possible region of right image.");
//     e.SetDataObject(inRightPtr);
//     throw e;
    }

  if(inLeftMaskPtr)
    {
    // no need to crop the mask region : left mask and left image have same largest possible region
    inLeftMaskPtr->SetRequestedRegion( fullRequestedRegion );
    }

  if(inRightMaskPtr)
    {
    inRightMaskPtr->SetRequestedRegion( inputRightMaskRegion );
    }

  if (inHDispPtr)
    {
    inHDispPtr->SetRequestedRegion( outputRequestedRegion );
    }

  if (inVDispPtr)
    {
    inVDispPtr->SetRequestedRegion( outputRequestedRegion );
    }
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::BeforeThreadedGenerateData()
{
  TOutputMetricImage    * outMetricPtr = this->GetMetricOutput();
  TDisparityImage * outHDispPtr   = this->GetHorizontalDisparityOutput();
  TDisparityImage * outVDispPtr   = this->GetVerticalDisparityOutput();

  // Fill buffers with default values
  outMetricPtr->FillBuffer(0.);
  outHDispPtr->FillBuffer(static_cast<DisparityPixelType>(m_MinimumHorizontalDisparity) /
                          static_cast<DisparityPixelType>(this->m_Step));
  outVDispPtr->FillBuffer(static_cast<DisparityPixelType>(m_MinimumVerticalDisparity) /
                          static_cast<DisparityPixelType>(this->m_Step));

  m_WrongExtrema.resize(this->GetNumberOfThreads());
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::ThreadedGenerateData(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // choose the refinement method to use
  switch(m_RefineMethod)
    {
    // 0 = Parabolic fit
    case 0 : ParabolicRefinement(outputRegionForThread, threadId);
      break;

    // 1 = triangular fit
    case 1 : TriangularRefinement(outputRegionForThread, threadId);
      break;

    // 2 = dichotomy search
    case 2 : DichotomyRefinement(outputRegionForThread, threadId);
      break;
    default : break;
    }
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::ParabolicRefinement(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Retrieve pointers
  const TInputImage *     inLeftPtr    = this->GetLeftInput();
  const TInputImage *     inRightPtr   = this->GetRightInput();
  const TMaskImage  *     inLeftMaskPtr    = this->GetLeftMaskInput();
  const TMaskImage  *     inRightMaskPtr    = this->GetRightMaskInput();
  const TDisparityImage * inHDispPtr = this->GetHorizontalDisparityInput();
  const TDisparityImage * inVDispPtr = this->GetVerticalDisparityInput();
  TOutputMetricImage    * outMetricPtr = this->GetMetricOutput();
  TDisparityImage * outHDispPtr   = this->GetHorizontalDisparityOutput();
  TDisparityImage * outVDispPtr   = this->GetVerticalDisparityOutput();

  unsigned int nb_WrongExtrema = 0;

  // Set-up progress reporting (this is not exact, since we do not
  // account for pixels that won't be refined)
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels(),100);

  RegionType fullRegionForThread = BlockMatchingFilterType::ConvertSubsampledToFullRegion(outputRegionForThread, this->m_Step, this->m_GridIndex);

  itk::ConstNeighborhoodIterator<TInputImage>     leftIt(m_Radius,inLeftPtr,fullRegionForThread);
  itk::ImageRegionIterator<TDisparityImage>       outHDispIt(outHDispPtr,outputRegionForThread);
  itk::ImageRegionIterator<TDisparityImage>       outVDispIt(outVDispPtr,outputRegionForThread);
  itk::ImageRegionIterator<TOutputMetricImage>    outMetricIt(outMetricPtr,outputRegionForThread);
  itk::ImageRegionConstIterator<TDisparityImage>  inHDispIt;
  itk::ImageRegionConstIterator<TDisparityImage>  inVDispIt;
  itk::ImageRegionConstIterator<TMaskImage>       inLeftMaskIt;
  itk::ImageRegionConstIterator<TMaskImage>       inRightMaskIt;

  bool useHorizontalDisparity = false;
  bool useVerticalDisparity = false;

  if (inHDispPtr)
    {
    useHorizontalDisparity = true;
    inHDispIt = itk::ImageRegionConstIterator<TDisparityImage>(inHDispPtr,outputRegionForThread);
    inHDispIt.GoToBegin();
    }
  if (inVDispPtr)
    {
    useVerticalDisparity = true;
    inVDispIt = itk::ImageRegionConstIterator<TDisparityImage>(inVDispPtr,outputRegionForThread);
    inVDispIt.GoToBegin();
    }

  if(inLeftMaskPtr)
    {
    inLeftMaskIt = itk::ImageRegionConstIterator<TMaskImage>(inLeftMaskPtr,fullRegionForThread);
    inLeftMaskIt.GoToBegin();
    }
  RegionType rightBufferedRegion = inRightPtr->GetBufferedRegion();
  if(inRightMaskPtr)
    {
    RegionType inRightMaskRegion = inRightMaskPtr->GetBufferedRegion();
    inRightMaskIt = itk::ImageRegionConstIterator<TMaskImage>(inRightMaskPtr,inRightMaskRegion);
    }

  itk::ConstantBoundaryCondition<TInputImage> nbc1;
  leftIt.OverrideBoundaryCondition(&nbc1);

  leftIt.GoToBegin();
  outHDispIt.GoToBegin();
  outVDispIt.GoToBegin();
  outMetricIt.GoToBegin();

  /* Specific variables */
  IndexType curLeftPos;
  IndexType curRightPos;

  float hDisp_f;
  float vDisp_f;
  int hDisp_i;
  int vDisp_i;

  // compute metric around current right position
  bool horizontalInterpolation = false;
  bool verticalInterpolation = false;

  typename ResamplerFilterType::Pointer resampler;

  // step value as disparityType
  DisparityPixelType stepDisparity = static_cast<DisparityPixelType>(this->m_Step);
  DisparityPixelType stepDisparityInv = 1. / stepDisparity;

  // metrics for neighbors positions : first index is x, second is y
  double neighborsMetric[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

  while (!leftIt.IsAtEnd()
        || !outHDispIt.IsAtEnd()
        || !outVDispIt.IsAtEnd()
        || !outMetricIt.IsAtEnd())
    {
    // If the pixel location is on the subsampled grid
    curLeftPos = leftIt.GetIndex();
    if (((curLeftPos[0] - this->m_GridIndex[0] + this->m_Step) % this->m_Step == 0) &&
        ((curLeftPos[1] - this->m_GridIndex[1] + this->m_Step) % this->m_Step == 0))
      {
      horizontalInterpolation = false;
      verticalInterpolation = false;

      /* compute estimated right position */
      if (useHorizontalDisparity)
        {
        hDisp_f = static_cast<float>(inHDispIt.Get()) * stepDisparity;
        hDisp_i = static_cast<int>(vcl_floor(hDisp_f + 0.5));
        curRightPos[0] = curLeftPos[0] + hDisp_i;
        }
      else
        {
        hDisp_i = 0;
        curRightPos[0] = curLeftPos[0];
        }


      if (useVerticalDisparity)
        {
        vDisp_f = static_cast<float>(inVDispIt.Get()) * stepDisparity;
        vDisp_i = static_cast<int>(vcl_floor(vDisp_f + 0.5));
        curRightPos[1] = curLeftPos[1] + vDisp_i;
        }
      else
        {
        vDisp_i = 0;
        curRightPos[1] = curLeftPos[1];
        }

      // check if the current right position is inside the right image
      if (rightBufferedRegion.IsInside(curRightPos))
        {
        if (inRightMaskPtr)
          {
          // Set right mask iterator position
          inRightMaskIt.SetIndex(curRightPos);
          }
        // check that the current positions are not masked
        if(!inLeftMaskPtr || (inLeftMaskPtr && inLeftMaskIt.Get() > 0) )
          {
          if(!inRightMaskPtr || (inRightMaskPtr && inRightMaskIt.Get() > 0) )
            {
            RegionType smallRightRegion;
            smallRightRegion.SetIndex(0,curRightPos[0]-1);
            smallRightRegion.SetIndex(1,curRightPos[1]-1);
            smallRightRegion.SetSize(0,3);
            smallRightRegion.SetSize(1,3);

            itk::ConstNeighborhoodIterator<TInputImage>     rightIt(m_Radius,inRightPtr,smallRightRegion);
            itk::ConstantBoundaryCondition<TInputImage>     nbc2;
            rightIt.OverrideBoundaryCondition(&nbc2);

            // compute metric at centre position
            rightIt.SetLocation(curRightPos);
            neighborsMetric[1][1] = m_Functor(leftIt,rightIt);

            if (m_MinimumVerticalDisparity < vDisp_i && vDisp_i < m_MaximumVerticalDisparity)
              {
              IndexType upIndex(curRightPos);
              upIndex[1]+= (-1);
              IndexType downIndex(curRightPos);
              downIndex[1]+= 1;
              if ( rightBufferedRegion.IsInside(upIndex) && rightBufferedRegion.IsInside(downIndex) )
                {
                rightIt.SetLocation(upIndex);
                neighborsMetric[1][0] = m_Functor(leftIt,rightIt);

                rightIt.SetLocation(downIndex);
                neighborsMetric[1][2] = m_Functor(leftIt,rightIt);

                // check that current position is an extrema
                if (m_Minimize)
                  {
                  if (neighborsMetric[1][1] < neighborsMetric[1][0] && neighborsMetric[1][1] < neighborsMetric[1][2])
                    {
                    verticalInterpolation = true;
                    }
                  }
                else
                  {
                  if (neighborsMetric[1][1] > neighborsMetric[1][0] && neighborsMetric[1][1] > neighborsMetric[1][2])
                    {
                    verticalInterpolation = true;
                    }
                  }
                }
              }

            if (m_MinimumHorizontalDisparity < hDisp_i && hDisp_i < m_MaximumHorizontalDisparity)
              {
              IndexType leftIndex(curRightPos);
              leftIndex[0]+= (-1);
              IndexType rightIndex(curRightPos);
              rightIndex[0]+= 1;
              if ( rightBufferedRegion.IsInside(leftIndex) && rightBufferedRegion.IsInside(rightIndex) )
                {
                rightIt.SetLocation(rightIndex);
                neighborsMetric[2][1] = m_Functor(leftIt,rightIt);

                rightIt.SetLocation(leftIndex);
                neighborsMetric[0][1] = m_Functor(leftIt,rightIt);

                // check that current position is an extrema
                if (m_Minimize)
                  {
                  if (neighborsMetric[1][1] < neighborsMetric[0][1] && neighborsMetric[1][1] < neighborsMetric[2][1])
                    {
                    horizontalInterpolation = true;
                    }
                  }
                else
                  {
                  if (neighborsMetric[1][1] > neighborsMetric[0][1] && neighborsMetric[1][1] > neighborsMetric[2][1])
                    {
                    horizontalInterpolation = true;
                    }
                  }
                }
              }

            // if both vertical and horizontal interpolation, compute metrics on corners
            if (verticalInterpolation && horizontalInterpolation)
              {
              IndexType uprightIndex(curRightPos);
              uprightIndex[0]+= 1;
              uprightIndex[1]+= (-1);
              rightIt.SetLocation(uprightIndex);
              neighborsMetric[2][0] = m_Functor(leftIt,rightIt);

              IndexType downrightIndex(curRightPos);
              downrightIndex[0]+= 1;
              downrightIndex[1]+= 1;
              rightIt.SetLocation(downrightIndex);
              neighborsMetric[2][2] = m_Functor(leftIt,rightIt);

              IndexType downleftIndex(curRightPos);
              downleftIndex[0]+= (-1);
              downleftIndex[1]+= 1;
              rightIt.SetLocation(downleftIndex);
              neighborsMetric[0][2] = m_Functor(leftIt,rightIt);

              IndexType upleftIndex(curRightPos);
              upleftIndex[0]+= (-1);
              upleftIndex[1]+= (-1);
              rightIt.SetLocation(upleftIndex);
              neighborsMetric[0][0] = m_Functor(leftIt,rightIt);

              //check that it is an extrema
              if (m_Minimize)
                {
                if (neighborsMetric[1][1] > neighborsMetric[2][0] ||
                    neighborsMetric[1][1] > neighborsMetric[2][2] ||
                    neighborsMetric[1][1] > neighborsMetric[0][2] ||
                    neighborsMetric[1][1] > neighborsMetric[0][0])
                  {
                  verticalInterpolation = false;
                  horizontalInterpolation = false;
                  }
                }
              else
                {
                if (neighborsMetric[1][1] < neighborsMetric[2][0] ||
                    neighborsMetric[1][1] < neighborsMetric[2][2] ||
                    neighborsMetric[1][1] < neighborsMetric[0][2] ||
                    neighborsMetric[1][1] < neighborsMetric[0][0])
                  {
                  verticalInterpolation = false;
                  horizontalInterpolation = false;
                  }
                }
              }
            }
          }
        }

      // Interpolate position : parabola fit
      if (verticalInterpolation && !horizontalInterpolation)
        {
        //vertical only
        double deltaV = 0.5 - (1.0 /
          (1.0 + (neighborsMetric[1][0]-neighborsMetric[1][1]) / (neighborsMetric[1][2]-neighborsMetric[1][1])));
        if (deltaV > (-0.5) && deltaV < 0.5)
          {
          outVDispIt.Set( (static_cast<double>(vDisp_i) + deltaV) * stepDisparityInv);
          }
        else
          {
          verticalInterpolation = false;
          }
        }
      else if (!verticalInterpolation && horizontalInterpolation)
        {
        // horizontal only
        double deltaH = 0.5 - (1.0 /
          (1.0 + (neighborsMetric[0][1]-neighborsMetric[1][1]) / (neighborsMetric[2][1]-neighborsMetric[1][1])));
        if (deltaH > (-0.5) && deltaH < 0.5)
          {
          outHDispIt.Set( (static_cast<double>(hDisp_i) + deltaH) * stepDisparityInv);
          }
        else
          {
          horizontalInterpolation = false;
          }
        }
      else if (verticalInterpolation && horizontalInterpolation)
        {
        //both horizontal and vertical
        double dx = 0.5 * (neighborsMetric[2][1] - neighborsMetric[0][1]);
        double dy = 0.5 * (neighborsMetric[1][2] - neighborsMetric[1][0]);
        double dxx = neighborsMetric[2][1] + neighborsMetric[0][1] - 2.0 * neighborsMetric[1][1];
        double dyy = neighborsMetric[1][2] + neighborsMetric[1][0] - 2.0 * neighborsMetric[1][1];
        double dxy = 0.25*(neighborsMetric[2][2] + neighborsMetric[0][0] - neighborsMetric[0][2] - neighborsMetric[2][0]);
        double det = dxx*dyy - dxy*dxy;
        if (vcl_abs(det) < (1e-10))
          {
          verticalInterpolation = false;
          horizontalInterpolation = false;
          }
        else
          {
          double deltaH = (-dx * dyy + dy * dxy )/det;
          double deltaV = ( dx * dxy - dy * dxx )/det;
          if (deltaH > (-1.0) && deltaH < 1.0 && deltaV > (-1.0) && deltaV < 1.0)
            {
            outHDispIt.Set( (static_cast<double>(hDisp_i) + deltaH) * stepDisparityInv);
            outVDispIt.Set( (static_cast<double>(vDisp_i) + deltaV) * stepDisparityInv);
            }
          else
            {
            verticalInterpolation = false;
            horizontalInterpolation = false;
            }
          }
        }

      if (!verticalInterpolation)
        {
        // No vertical interpolation done : simply copy the integer vertical disparity
        outVDispIt.Set( static_cast<double>(vDisp_i) * stepDisparityInv);
        }

      if (!horizontalInterpolation)
        {
        // No horizontal interpolation done : simply copy the integer horizontal disparity
        outHDispIt.Set( static_cast<double>(hDisp_i) * stepDisparityInv);
        }

      if (!verticalInterpolation && !horizontalInterpolation)
        {
        // no interpolation done : keep current score
        outMetricIt.Set(static_cast<double>(neighborsMetric[1][1]));
        }
      else
        {
        // interpolation done, use a resampler to compute new score
        typename TInputImage::Pointer fakeRightPtr = TInputImage::New();
        fakeRightPtr->SetRegions(rightBufferedRegion);
        fakeRightPtr->SetPixelContainer((const_cast<TInputImage*>(inRightPtr))->GetPixelContainer());

        itk::ConstNeighborhoodIterator<TInputImage>     shiftedIt;
        itk::ConstantBoundaryCondition<TInputImage>     nbc3;
        shiftedIt.OverrideBoundaryCondition(&nbc3);

        SizeType windowSize;
        windowSize[0] = 2 * m_Radius[0] + 1;
        windowSize[1] = 2 * m_Radius[1] + 1;

        IndexType upleftCorner;
        upleftCorner[0] = curRightPos[0] - m_Radius[0];
        upleftCorner[1] = curRightPos[1] - m_Radius[1];

        TransformationType::Pointer transfo = TransformationType::New();
        TransformationType::OutputVectorType offsetTransfo(2);

        RegionType tinyShiftedRegion;
        tinyShiftedRegion.SetSize(0, 1);
        tinyShiftedRegion.SetSize(1, 1);
        tinyShiftedRegion.SetIndex(0, curRightPos[0]);
        tinyShiftedRegion.SetIndex(1, curRightPos[1]);

        resampler = ResamplerFilterType::New();
        resampler->SetInput(fakeRightPtr);
        resampler->SetSize(windowSize);
        resampler->SetNumberOfThreads(1);
        resampler->SetTransform(transfo);
        resampler->SetOutputStartIndex(upleftCorner);

        offsetTransfo[0] = outHDispIt.Get() * stepDisparity - static_cast<double>(hDisp_i);
        offsetTransfo[1] = outVDispIt.Get() * stepDisparity - static_cast<double>(vDisp_i);
        transfo->SetOffset(offsetTransfo);
        resampler->Modified();
        resampler->Update();
        shiftedIt.Initialize(m_Radius,resampler->GetOutput(),tinyShiftedRegion);
        outMetricIt.Set(m_Functor(leftIt,shiftedIt));

        if ((outMetricIt.Get() > neighborsMetric[1][1] && m_Minimize) ||
            (outMetricIt.Get() < neighborsMetric[1][1] && !m_Minimize))
          {
          nb_WrongExtrema++;
          }
        }

      progress.CompletedPixel();
      ++outHDispIt;
      ++outVDispIt;
      ++outMetricIt;
      if (useHorizontalDisparity)
        {
        ++inHDispIt;
        }
      if (useVerticalDisparity)
        {
        ++inVDispIt;
        }
      }

    ++leftIt;

    if(inLeftMaskPtr)
      {
      ++inLeftMaskIt;
      }
    }

  m_WrongExtrema[threadId] = static_cast<double>(nb_WrongExtrema) /
      static_cast<double>(outputRegionForThread.GetNumberOfPixels());

}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::TriangularRefinement(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Retrieve pointers
  const TInputImage *     inLeftPtr    = this->GetLeftInput();
  const TInputImage *     inRightPtr   = this->GetRightInput();
  const TMaskImage  *     inLeftMaskPtr    = this->GetLeftMaskInput();
  const TMaskImage  *     inRightMaskPtr    = this->GetRightMaskInput();
  const TDisparityImage * inHDispPtr = this->GetHorizontalDisparityInput();
  const TDisparityImage * inVDispPtr = this->GetVerticalDisparityInput();
  TOutputMetricImage    * outMetricPtr = this->GetMetricOutput();
  TDisparityImage * outHDispPtr   = this->GetHorizontalDisparityOutput();
  TDisparityImage * outVDispPtr   = this->GetVerticalDisparityOutput();

  unsigned int nb_WrongExtrema = 0;

  // Set-up progress reporting (this is not exact, since we do not
  // account for pixels that won't be refined)
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels(),100);

  RegionType fullRegionForThread = BlockMatchingFilterType::ConvertSubsampledToFullRegion(outputRegionForThread, this->m_Step, this->m_GridIndex);

  itk::ConstNeighborhoodIterator<TInputImage>     leftIt(m_Radius,inLeftPtr,fullRegionForThread);
  itk::ImageRegionIterator<TDisparityImage>       outHDispIt(outHDispPtr,outputRegionForThread);
  itk::ImageRegionIterator<TDisparityImage>       outVDispIt(outVDispPtr,outputRegionForThread);
  itk::ImageRegionIterator<TOutputMetricImage>    outMetricIt(outMetricPtr,outputRegionForThread);
  itk::ImageRegionConstIterator<TDisparityImage>  inHDispIt;
  itk::ImageRegionConstIterator<TDisparityImage>  inVDispIt;
  itk::ImageRegionConstIterator<TMaskImage>       inLeftMaskIt;
  itk::ImageRegionConstIterator<TMaskImage>       inRightMaskIt;

  bool useHorizontalDisparity = false;
  bool useVerticalDisparity = false;

  if (inHDispPtr)
    {
    useHorizontalDisparity = true;
    inHDispIt = itk::ImageRegionConstIterator<TDisparityImage>(inHDispPtr,outputRegionForThread);
    inHDispIt.GoToBegin();
    }
  if (inVDispPtr)
    {
    useVerticalDisparity = true;
    inVDispIt = itk::ImageRegionConstIterator<TDisparityImage>(inVDispPtr,outputRegionForThread);
    inVDispIt.GoToBegin();
    }

  if(inLeftMaskPtr)
    {
    inLeftMaskIt = itk::ImageRegionConstIterator<TMaskImage>(inLeftMaskPtr,fullRegionForThread);
    inLeftMaskIt.GoToBegin();
    }
  RegionType rightBufferedRegion = inRightPtr->GetBufferedRegion();
  if(inRightMaskPtr)
    {
    RegionType inRightMaskRegion = inRightMaskPtr->GetBufferedRegion();
    inRightMaskIt = itk::ImageRegionConstIterator<TMaskImage>(inRightMaskPtr,inRightMaskRegion);
    }

  itk::ConstantBoundaryCondition<TInputImage> nbc1;
  leftIt.OverrideBoundaryCondition(&nbc1);

  leftIt.GoToBegin();
  outHDispIt.GoToBegin();
  outVDispIt.GoToBegin();
  outMetricIt.GoToBegin();

  /* Specific variables */
  IndexType curLeftPos;
  IndexType curRightPos;

  float hDisp_f;
  float vDisp_f;
  int hDisp_i;
  int vDisp_i;

  // compute metric around current right position
  bool horizontalInterpolation = false;
  bool verticalInterpolation = false;

  typename ResamplerFilterType::Pointer resampler;

  // step value as disparityType
  DisparityPixelType stepDisparity = static_cast<DisparityPixelType>(this->m_Step);
  DisparityPixelType stepDisparityInv = 1. / stepDisparity;


  // metrics for neighbors positions : first index is x, second is y
  double neighborsMetric[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

  while (!leftIt.IsAtEnd()
        || !outHDispIt.IsAtEnd()
        || !outVDispIt.IsAtEnd()
        || !outMetricIt.IsAtEnd())
    {
    // If the pixel location is on the subsampled grid
    curLeftPos = leftIt.GetIndex();
    if (((curLeftPos[0] - this->m_GridIndex[0] + this->m_Step) % this->m_Step == 0) &&
        ((curLeftPos[1] - this->m_GridIndex[1] + this->m_Step) % this->m_Step == 0))
      {
      horizontalInterpolation = false;
      verticalInterpolation = false;

      /* compute estimated right position */
      if (useHorizontalDisparity)
        {
        hDisp_f = static_cast<float>(inHDispIt.Get()) * stepDisparity;
        hDisp_i = static_cast<int>(vcl_floor(hDisp_f + 0.5));
        curRightPos[0] = curLeftPos[0] + hDisp_i;
        }
      else
        {
        hDisp_i = 0;
        curRightPos[0] = curLeftPos[0];
        }


      if (useVerticalDisparity)
        {
        vDisp_f = static_cast<float>(inVDispIt.Get()) * stepDisparity;
        vDisp_i = static_cast<int>(vcl_floor(vDisp_f + 0.5));
        curRightPos[1] = curLeftPos[1] + vDisp_i;
        }
      else
        {
        vDisp_i = 0;
        curRightPos[1] = curLeftPos[1];
        }

      // check if the current right position is inside the right image
      if (rightBufferedRegion.IsInside(curRightPos))
        {
        if (inRightMaskPtr)
          {
          // Set right mask iterator position
          inRightMaskIt.SetIndex(curRightPos);
          }
        // check that the current positions are not masked
        if(!inLeftMaskPtr || (inLeftMaskPtr && inLeftMaskIt.Get() > 0) )
          {
          if(!inRightMaskPtr || (inRightMaskPtr && inRightMaskIt.Get() > 0) )
            {
            RegionType smallRightRegion;
            smallRightRegion.SetIndex(0,curRightPos[0]-1);
            smallRightRegion.SetIndex(1,curRightPos[1]-1);
            smallRightRegion.SetSize(0,3);
            smallRightRegion.SetSize(1,3);

            itk::ConstNeighborhoodIterator<TInputImage>     rightIt(m_Radius,inRightPtr,smallRightRegion);
            itk::ConstantBoundaryCondition<TInputImage>     nbc2;
            rightIt.OverrideBoundaryCondition(&nbc2);

            // compute metric at centre position
            rightIt.SetLocation(curRightPos);
            neighborsMetric[1][1] = m_Functor(leftIt,rightIt);

            if (m_MinimumVerticalDisparity < vDisp_i && vDisp_i < m_MaximumVerticalDisparity)
              {
              IndexType upIndex(curRightPos);
              upIndex[1]+= (-1);
              IndexType downIndex(curRightPos);
              downIndex[1]+= 1;
              if ( rightBufferedRegion.IsInside(upIndex) && rightBufferedRegion.IsInside(downIndex) )
                {
                rightIt.SetLocation(upIndex);
                neighborsMetric[1][0] = m_Functor(leftIt,rightIt);

                rightIt.SetLocation(downIndex);
                neighborsMetric[1][2] = m_Functor(leftIt,rightIt);

                // check that current position is an extrema
                if (m_Minimize)
                  {
                  if (neighborsMetric[1][1] < neighborsMetric[1][0] && neighborsMetric[1][1] < neighborsMetric[1][2])
                    {
                    verticalInterpolation = true;
                    }
                  }
                else
                  {
                  if (neighborsMetric[1][1] > neighborsMetric[1][0] && neighborsMetric[1][1] > neighborsMetric[1][2])
                    {
                    verticalInterpolation = true;
                    }
                  }
                }
              }

            if (m_MinimumHorizontalDisparity < hDisp_i && hDisp_i < m_MaximumHorizontalDisparity)
              {
              IndexType leftIndex(curRightPos);
              leftIndex[0]+= (-1);
              IndexType rightIndex(curRightPos);
              rightIndex[0]+= 1;
              if ( rightBufferedRegion.IsInside(leftIndex) && rightBufferedRegion.IsInside(rightIndex) )
                {
                rightIt.SetLocation(rightIndex);
                neighborsMetric[2][1] = m_Functor(leftIt,rightIt);

                rightIt.SetLocation(leftIndex);
                neighborsMetric[0][1] = m_Functor(leftIt,rightIt);

                // check that current position is an extrema
                if (m_Minimize)
                  {
                  if (neighborsMetric[1][1] < neighborsMetric[0][1] && neighborsMetric[1][1] < neighborsMetric[2][1])
                    {
                    horizontalInterpolation = true;
                    }
                  }
                else
                  {
                  if (neighborsMetric[1][1] > neighborsMetric[0][1] && neighborsMetric[1][1] > neighborsMetric[2][1])
                    {
                    horizontalInterpolation = true;
                    }
                  }
                }
              }

            // if both vertical and horizontal interpolation, compute metrics on corners
            if (verticalInterpolation && horizontalInterpolation)
              {
              IndexType uprightIndex(curRightPos);
              uprightIndex[0]+= 1;
              uprightIndex[1]+= (-1);
              rightIt.SetLocation(uprightIndex);
              neighborsMetric[2][0] = m_Functor(leftIt,rightIt);

              IndexType downrightIndex(curRightPos);
              downrightIndex[0]+= 1;
              downrightIndex[1]+= 1;
              rightIt.SetLocation(downrightIndex);
              neighborsMetric[2][2] = m_Functor(leftIt,rightIt);

              IndexType downleftIndex(curRightPos);
              downleftIndex[0]+= (-1);
              downleftIndex[1]+= 1;
              rightIt.SetLocation(downleftIndex);
              neighborsMetric[0][2] = m_Functor(leftIt,rightIt);

              IndexType upleftIndex(curRightPos);
              upleftIndex[0]+= (-1);
              upleftIndex[1]+= (-1);
              rightIt.SetLocation(upleftIndex);
              neighborsMetric[0][0] = m_Functor(leftIt,rightIt);

              //check that it is an extrema
              if (m_Minimize)
                {
                if (neighborsMetric[1][1] > neighborsMetric[2][0] ||
                    neighborsMetric[1][1] > neighborsMetric[2][2] ||
                    neighborsMetric[1][1] > neighborsMetric[0][2] ||
                    neighborsMetric[1][1] > neighborsMetric[0][0])
                  {
                  verticalInterpolation = false;
                  horizontalInterpolation = false;
                  }
                }
              else
                {
                if (neighborsMetric[1][1] < neighborsMetric[2][0] ||
                    neighborsMetric[1][1] < neighborsMetric[2][2] ||
                    neighborsMetric[1][1] < neighborsMetric[0][2] ||
                    neighborsMetric[1][1] < neighborsMetric[0][0])
                  {
                  verticalInterpolation = false;
                  horizontalInterpolation = false;
                  }
                }
              }
            }
          }
        }

      // Interpolate position : triangular fit
      if (verticalInterpolation && !horizontalInterpolation)
        {
        //vertical only
        double deltaV;
        if ((neighborsMetric[1][0] < neighborsMetric[1][2] && m_Minimize) ||
            (neighborsMetric[1][0] > neighborsMetric[1][2] && !m_Minimize))
          {
          deltaV = 0.5 * ((neighborsMetric[1][0]-neighborsMetric[1][2])/(neighborsMetric[1][2]-neighborsMetric[1][1]));
          }
        else
          {
          deltaV = 0.5 * ((neighborsMetric[1][0]-neighborsMetric[1][2])/(neighborsMetric[1][0]-neighborsMetric[1][1]));
          }
        if (deltaV > (-0.5) && deltaV < 0.5)
          {
          outVDispIt.Set( (static_cast<double>(vDisp_i) + deltaV) * stepDisparityInv);
          }
        else
          {
          verticalInterpolation = false;
          }
        }
      else if (!verticalInterpolation && horizontalInterpolation)
        {
        // horizontal only
        double deltaH;
        if ((neighborsMetric[0][1] < neighborsMetric[2][1] && m_Minimize) ||
            (neighborsMetric[0][1] > neighborsMetric[2][1] && !m_Minimize))
          {
          deltaH = 0.5 * ((neighborsMetric[0][1]-neighborsMetric[2][1])/(neighborsMetric[2][1]-neighborsMetric[1][1]));
          }
        else
          {
          deltaH = 0.5 * ((neighborsMetric[0][1]-neighborsMetric[2][1])/(neighborsMetric[0][1]-neighborsMetric[1][1]));
          }
        if (deltaH > (-0.5) && deltaH < 0.5)
          {
          outHDispIt.Set( (static_cast<double>(hDisp_i) + deltaH) * stepDisparityInv);
          }
        else
          {
          horizontalInterpolation = false;
          }
        }
      else if (verticalInterpolation && horizontalInterpolation)
        {
        //both horizontal and vertical
        double deltaH;
        double deltaV;
        if ((neighborsMetric[1][0] < neighborsMetric[1][2] && m_Minimize) ||
            (neighborsMetric[1][0] > neighborsMetric[1][2] && !m_Minimize))
          {
          deltaV = 0.5 * ((neighborsMetric[1][0]-neighborsMetric[1][2])/(neighborsMetric[1][2]-neighborsMetric[1][1]));
          }
        else
          {
          deltaV = 0.5 * ((neighborsMetric[1][0]-neighborsMetric[1][2])/(neighborsMetric[1][0]-neighborsMetric[1][1]));
          }

        if ((neighborsMetric[0][1] < neighborsMetric[2][1] && m_Minimize) ||
            (neighborsMetric[0][1] > neighborsMetric[2][1] && !m_Minimize))
          {
          deltaH = 0.5 * ((neighborsMetric[0][1]-neighborsMetric[2][1])/(neighborsMetric[2][1]-neighborsMetric[1][1]));
          }
        else
          {
          deltaH = 0.5 * ((neighborsMetric[0][1]-neighborsMetric[2][1])/(neighborsMetric[0][1]-neighborsMetric[1][1]));
          }

        if (deltaV > (-1.0) && deltaV < 1.0 && deltaH > (-1.0) && deltaH < 1.0)
          {
          outVDispIt.Set( (static_cast<double>(vDisp_i) + deltaV) * stepDisparityInv);
          outHDispIt.Set( (static_cast<double>(hDisp_i) + deltaH) * stepDisparityInv);
          }
        else
          {
          verticalInterpolation = false;
          horizontalInterpolation = false;
          }
        }

      if (!verticalInterpolation)
        {
        // No vertical interpolation done : simply copy the integer vertical disparity
        outVDispIt.Set( static_cast<double>(vDisp_i) * stepDisparityInv);
        }

      if (!horizontalInterpolation)
        {
        // No horizontal interpolation done : simply copy the integer horizontal disparity
        outHDispIt.Set( static_cast<double>(hDisp_i) * stepDisparityInv);
        }

      if (!verticalInterpolation && !horizontalInterpolation)
        {
        // no interpolation done : keep current score
        outMetricIt.Set(static_cast<double>(neighborsMetric[1][1]));
        }
      else
        {
        // interpolation done, use a resampler to compute new score
        typename TInputImage::Pointer fakeRightPtr = TInputImage::New();
        fakeRightPtr->SetRegions(rightBufferedRegion);
        fakeRightPtr->SetPixelContainer((const_cast<TInputImage*>(inRightPtr))->GetPixelContainer());

        itk::ConstNeighborhoodIterator<TInputImage>     shiftedIt;
        itk::ConstantBoundaryCondition<TInputImage>     nbc3;
        shiftedIt.OverrideBoundaryCondition(&nbc3);

        SizeType windowSize;
        windowSize[0] = 2 * m_Radius[0] + 1;
        windowSize[1] = 2 * m_Radius[1] + 1;

        IndexType upleftCorner;
        upleftCorner[0] = curRightPos[0] - m_Radius[0];
        upleftCorner[1] = curRightPos[1] - m_Radius[1];

        TransformationType::Pointer transfo = TransformationType::New();
        TransformationType::OutputVectorType offsetTransfo(2);

        RegionType tinyShiftedRegion;
        tinyShiftedRegion.SetSize(0, 1);
        tinyShiftedRegion.SetSize(1, 1);
        tinyShiftedRegion.SetIndex(0, curRightPos[0]);
        tinyShiftedRegion.SetIndex(1, curRightPos[1]);

        resampler = ResamplerFilterType::New();
        resampler->SetInput(fakeRightPtr);
        resampler->SetSize(windowSize);
        resampler->SetNumberOfThreads(1);
        resampler->SetTransform(transfo);
        resampler->SetOutputStartIndex(upleftCorner);

        offsetTransfo[0] = outHDispIt.Get() * stepDisparity - static_cast<double>(hDisp_i);
        offsetTransfo[1] = outVDispIt.Get() * stepDisparity - static_cast<double>(vDisp_i);
        transfo->SetOffset(offsetTransfo);
        resampler->Modified();
        resampler->Update();
        shiftedIt.Initialize(m_Radius,resampler->GetOutput(),tinyShiftedRegion);
        outMetricIt.Set(m_Functor(leftIt,shiftedIt));

        if ((outMetricIt.Get() > neighborsMetric[1][1] && m_Minimize) ||
            (outMetricIt.Get() < neighborsMetric[1][1] && !m_Minimize))
          {
          nb_WrongExtrema++;
          }
        }

      progress.CompletedPixel();

      ++outHDispIt;
      ++outVDispIt;
      ++outMetricIt;

      if (useHorizontalDisparity)
        {
        ++inHDispIt;
        }
      if (useVerticalDisparity)
        {
        ++inVDispIt;
        }
      }
    ++leftIt;

    if(inLeftMaskPtr)
      {
      ++inLeftMaskIt;
      }
    }

  m_WrongExtrema[threadId] = static_cast<double>(nb_WrongExtrema) /
      static_cast<double>(outputRegionForThread.GetNumberOfPixels());
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::DichotomyRefinement(const RegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  // Retrieve pointers
  const TInputImage *     inLeftPtr    = this->GetLeftInput();
  const TInputImage *     inRightPtr   = this->GetRightInput();
  const TMaskImage  *     inLeftMaskPtr    = this->GetLeftMaskInput();
  const TMaskImage  *     inRightMaskPtr    = this->GetRightMaskInput();
  const TDisparityImage * inHDispPtr = this->GetHorizontalDisparityInput();
  const TDisparityImage * inVDispPtr = this->GetVerticalDisparityInput();
  TOutputMetricImage    * outMetricPtr = this->GetMetricOutput();
  TDisparityImage * outHDispPtr   = this->GetHorizontalDisparityOutput();
  TDisparityImage * outVDispPtr   = this->GetVerticalDisparityOutput();

  unsigned int nb_WrongExtrema = 0;

  // Set-up progress reporting (this is not exact, since we do not
  // account for pixels that won't be refined)
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels(),100);

  RegionType fullRegionForThread = BlockMatchingFilterType::ConvertSubsampledToFullRegion(outputRegionForThread, this->m_Step, this->m_GridIndex);

  itk::ConstNeighborhoodIterator<TInputImage>     leftIt(m_Radius,inLeftPtr,fullRegionForThread);
  itk::ImageRegionIterator<TDisparityImage>       outHDispIt(outHDispPtr,outputRegionForThread);
  itk::ImageRegionIterator<TDisparityImage>       outVDispIt(outVDispPtr,outputRegionForThread);
  itk::ImageRegionIterator<TOutputMetricImage>    outMetricIt(outMetricPtr,outputRegionForThread);
  itk::ImageRegionConstIterator<TDisparityImage>  inHDispIt;
  itk::ImageRegionConstIterator<TDisparityImage>  inVDispIt;
  itk::ImageRegionConstIterator<TMaskImage>       inLeftMaskIt;
  itk::ImageRegionConstIterator<TMaskImage>       inRightMaskIt;

  bool useHorizontalDisparity = false;
  bool useVerticalDisparity = false;

  if (inHDispPtr)
    {
    useHorizontalDisparity = true;
    inHDispIt = itk::ImageRegionConstIterator<TDisparityImage>(inHDispPtr,outputRegionForThread);
    inHDispIt.GoToBegin();
    }
  if (inVDispPtr)
    {
    useVerticalDisparity = true;
    inVDispIt = itk::ImageRegionConstIterator<TDisparityImage>(inVDispPtr,outputRegionForThread);
    inVDispIt.GoToBegin();
    }

  if(inLeftMaskPtr)
    {
    inLeftMaskIt = itk::ImageRegionConstIterator<TMaskImage>(inLeftMaskPtr,fullRegionForThread);
    inLeftMaskIt.GoToBegin();
    }
  RegionType rightBufferedRegion = inRightPtr->GetBufferedRegion();
  if(inRightMaskPtr)
    {
    RegionType inRightMaskRegion = inRightMaskPtr->GetBufferedRegion();
    inRightMaskIt = itk::ImageRegionConstIterator<TMaskImage>(inRightMaskPtr,inRightMaskRegion);
    }

  itk::ConstantBoundaryCondition<TInputImage> nbc1;
  leftIt.OverrideBoundaryCondition(&nbc1);

  leftIt.GoToBegin();
  outHDispIt.GoToBegin();
  outVDispIt.GoToBegin();
  outMetricIt.GoToBegin();

  /* Specific variables */
  IndexType curLeftPos;
  IndexType curRightPos;

  float hDisp_f;
  float vDisp_f;
  int hDisp_i;
  int vDisp_i;

  typename TInputImage::Pointer fakeRightPtr = TInputImage::New();
  fakeRightPtr->SetRegions(rightBufferedRegion);
  fakeRightPtr->SetPixelContainer((const_cast<TInputImage*>(inRightPtr))->GetPixelContainer());

  // compute metric around current right position
  bool horizontalInterpolation = false;
  bool verticalInterpolation = false;

  // metrics for neighbors positions : first index is x, second is y
  double neighborsMetric[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
  unsigned int nbIterMax = 10;

  // set the output size and start index to the center position
  // then set the smaller shift in the transform
  IndexType upleftCorner;
  SizeType windowSize;
  windowSize[0] = 2 * m_Radius[0] + 1;
  windowSize[1] = 2 * m_Radius[1] + 1;

  TransformationType::Pointer transfo = TransformationType::New();
  TransformationType::OutputVectorType offsetTransfo(2);
  offsetTransfo[0] = 0.0;
  offsetTransfo[1] = 0.0;

  typename ResamplerFilterType::Pointer resampler;

  // step value as disparityType
  DisparityPixelType stepDisparity = static_cast<DisparityPixelType>(this->m_Step);
  DisparityPixelType stepDisparityInv = 1. / stepDisparity;

  RegionType tinyShiftedRegion;
  tinyShiftedRegion.SetIndex(0, m_Radius[0]);
  tinyShiftedRegion.SetIndex(1, m_Radius[1]);
  tinyShiftedRegion.SetSize(0, 1);
  tinyShiftedRegion.SetSize(1, 1);

  // iterators on right image
  itk::ConstNeighborhoodIterator<TInputImage>     rightIt;
  itk::ConstNeighborhoodIterator<TInputImage>     shiftedIt;
  itk::ConstantBoundaryCondition<TInputImage>     nbc2;
  itk::ConstantBoundaryCondition<TInputImage>     nbc3;
  rightIt.OverrideBoundaryCondition(&nbc2);
  shiftedIt.OverrideBoundaryCondition(&nbc3);
  bool centreHasMoved;

  while (!leftIt.IsAtEnd()
        || !outHDispIt.IsAtEnd()
        || !outVDispIt.IsAtEnd()
        || !outMetricIt.IsAtEnd())
    {
    // If the pixel location is on the subsampled grid
    curLeftPos = leftIt.GetIndex();
    if (((curLeftPos[0] - this->m_GridIndex[0] + this->m_Step) % this->m_Step == 0) &&
        ((curLeftPos[1] - this->m_GridIndex[1] + this->m_Step) % this->m_Step == 0))
      {
      horizontalInterpolation = false;
      verticalInterpolation = false;

      /* compute estimated right position */
      if (useHorizontalDisparity)
        {
        hDisp_f = static_cast<float>(inHDispIt.Get()) * stepDisparity;
        hDisp_i = static_cast<int>(vcl_floor(hDisp_f + 0.5));
        curRightPos[0] = curLeftPos[0] + hDisp_i;
        }
      else
        {
        hDisp_i = 0;
        curRightPos[0] = curLeftPos[0];
        }


      if (useVerticalDisparity)
        {
        vDisp_f = static_cast<float>(inVDispIt.Get()) * stepDisparity;
        vDisp_i = static_cast<int>(vcl_floor(vDisp_f + 0.5));
        curRightPos[1] = curLeftPos[1] + vDisp_i;
        }
      else
        {
        vDisp_i = 0;
        curRightPos[1] = curLeftPos[1];
        }

      // update resampler input position
      upleftCorner[0] = curRightPos[0] - m_Radius[0];
      upleftCorner[1] = curRightPos[1] - m_Radius[1];

      // resampler
      resampler = ResamplerFilterType::New();
      resampler->SetInput(fakeRightPtr);
      resampler->SetSize(windowSize);
      resampler->SetNumberOfThreads(1);
      resampler->SetTransform(transfo);
      resampler->SetOutputStartIndex(upleftCorner);

      tinyShiftedRegion.SetIndex(0, curRightPos[0]);
      tinyShiftedRegion.SetIndex(1, curRightPos[1]);

      // check if the current right position is inside the right image
      if (rightBufferedRegion.IsInside(curRightPos))
        {
        if (inRightMaskPtr)
          {
          // Set right mask iterator position
          inRightMaskIt.SetIndex(curRightPos);
          }
        // check that the current positions are not masked
        if(!inLeftMaskPtr || (inLeftMaskPtr && inLeftMaskIt.Get() > 0) )
          {
          if(!inRightMaskPtr || (inRightMaskPtr && inRightMaskIt.Get() > 0) )
            {
            RegionType smallRightRegion;
            smallRightRegion.SetIndex(0,curRightPos[0]-1);
            smallRightRegion.SetIndex(1,curRightPos[1]-1);
            smallRightRegion.SetSize(0,3);
            smallRightRegion.SetSize(1,3);

            rightIt.Initialize(m_Radius,inRightPtr,smallRightRegion);

            // compute metric at centre position
            rightIt.SetLocation(curRightPos);
            neighborsMetric[1][1] = m_Functor(leftIt,rightIt);

            if (m_MinimumVerticalDisparity < vDisp_i && vDisp_i < m_MaximumVerticalDisparity)
              {
              IndexType upIndex(curRightPos);
              upIndex[1]+= (-1);
              IndexType downIndex(curRightPos);
              downIndex[1]+= 1;
              if ( rightBufferedRegion.IsInside(upIndex) && rightBufferedRegion.IsInside(downIndex) )
                {
                rightIt.SetLocation(upIndex);
                neighborsMetric[1][0] = m_Functor(leftIt,rightIt);

                rightIt.SetLocation(downIndex);
                neighborsMetric[1][2] = m_Functor(leftIt,rightIt);

                // check that current position is an extrema
                if (m_Minimize)
                  {
                  if (neighborsMetric[1][1] < neighborsMetric[1][0] && neighborsMetric[1][1] < neighborsMetric[1][2])
                    {
                    verticalInterpolation = true;
                    }
                  }
                else
                  {
                  if (neighborsMetric[1][1] > neighborsMetric[1][0] && neighborsMetric[1][1] > neighborsMetric[1][2])
                    {
                    verticalInterpolation = true;
                    }
                  }
                }
              }

            if (m_MinimumHorizontalDisparity < hDisp_i && hDisp_i < m_MaximumHorizontalDisparity)
              {
              IndexType leftIndex(curRightPos);
              leftIndex[0]+= (-1);
              IndexType rightIndex(curRightPos);
              rightIndex[0]+= 1;
              if ( rightBufferedRegion.IsInside(leftIndex) && rightBufferedRegion.IsInside(rightIndex) )
                {
                rightIt.SetLocation(rightIndex);
                neighborsMetric[2][1] = m_Functor(leftIt,rightIt);

                rightIt.SetLocation(leftIndex);
                neighborsMetric[0][1] = m_Functor(leftIt,rightIt);

                // check that current position is an extrema
                if (m_Minimize)
                  {
                  if (neighborsMetric[1][1] < neighborsMetric[0][1] && neighborsMetric[1][1] < neighborsMetric[2][1])
                    {
                    horizontalInterpolation = true;
                    }
                  }
                else
                  {
                  if (neighborsMetric[1][1] > neighborsMetric[0][1] && neighborsMetric[1][1] > neighborsMetric[2][1])
                    {
                    horizontalInterpolation = true;
                    }
                  }
                }
              }

            // if both vertical and horizontal interpolation, compute metrics on corners
            if (verticalInterpolation && horizontalInterpolation)
              {
              IndexType uprightIndex(curRightPos);
              uprightIndex[0]+= 1;
              uprightIndex[1]+= (-1);
              rightIt.SetLocation(uprightIndex);
              neighborsMetric[2][0] = m_Functor(leftIt,rightIt);

              IndexType downrightIndex(curRightPos);
              downrightIndex[0]+= 1;
              downrightIndex[1]+= 1;
              rightIt.SetLocation(downrightIndex);
              neighborsMetric[2][2] = m_Functor(leftIt,rightIt);

              IndexType downleftIndex(curRightPos);
              downleftIndex[0]+= (-1);
              downleftIndex[1]+= 1;
              rightIt.SetLocation(downleftIndex);
              neighborsMetric[0][2] = m_Functor(leftIt,rightIt);

              IndexType upleftIndex(curRightPos);
              upleftIndex[0]+= (-1);
              upleftIndex[1]+= (-1);
              rightIt.SetLocation(upleftIndex);
              neighborsMetric[0][0] = m_Functor(leftIt,rightIt);

              //check that it is an extrema
              if (m_Minimize)
                {
                if (neighborsMetric[1][1] > neighborsMetric[2][0] ||
                    neighborsMetric[1][1] > neighborsMetric[2][2] ||
                    neighborsMetric[1][1] > neighborsMetric[0][2] ||
                    neighborsMetric[1][1] > neighborsMetric[0][0])
                  {
                  verticalInterpolation = false;
                  horizontalInterpolation = false;
                  }
                }
              else
                {
                if (neighborsMetric[1][1] < neighborsMetric[2][0] ||
                    neighborsMetric[1][1] < neighborsMetric[2][2] ||
                    neighborsMetric[1][1] < neighborsMetric[0][2] ||
                    neighborsMetric[1][1] < neighborsMetric[0][0])
                  {
                  verticalInterpolation = false;
                  horizontalInterpolation = false;
                  }
                }
              }
            }
          }
        }

      // Interpolate position : dichotomy
      if (verticalInterpolation && !horizontalInterpolation)
        {
        double ya = static_cast<double>(vDisp_i -1);
        double yb = static_cast<double>(vDisp_i);
        double yc = static_cast<double>(vDisp_i +1);
        double s_yb = neighborsMetric[1][1];

        double yd;
        double s_yd;

        offsetTransfo[0] = 0.0;

        for (unsigned int k=0; k<nbIterMax; k++)
          {
          if ( (yb-ya) < (yc-yb) )
            {
            yd = 0.5 * (yc+yb);
            offsetTransfo[1] = yd - static_cast<double>(vDisp_i);
            transfo->SetOffset(offsetTransfo);
            resampler->Modified();
            resampler->Update();
            shiftedIt.Initialize(m_Radius,resampler->GetOutput(),tinyShiftedRegion);
            s_yd = m_Functor(leftIt,shiftedIt);

            if ((s_yd<s_yb && m_Minimize) || (s_yd>s_yb && !m_Minimize))
              {
              ya = yb;

              yb = yd;
              s_yb = s_yd;
              }
            else
              {
              yc = yd;
              }
            }
          else
            {
            yd = 0.5 * (ya+yb);
            offsetTransfo[1] = yd - static_cast<double>(vDisp_i);
            transfo->SetOffset(offsetTransfo);
            resampler->Modified();
            resampler->Update();
            shiftedIt.Initialize(m_Radius,resampler->GetOutput(),tinyShiftedRegion);
            s_yd = m_Functor(leftIt,shiftedIt);

            if ((s_yd<s_yb && m_Minimize) || (s_yd>s_yb && !m_Minimize))
              {
              yc = yb;

              yb = yd;
              s_yb = s_yd;
              }
            else
              {
              ya = yd;
              }
            }
          }

        outVDispIt.Set( yb * stepDisparityInv );
        outMetricIt.Set( s_yb );
        }
      else if (!verticalInterpolation && horizontalInterpolation)
        {
        double xa = static_cast<double>(hDisp_i -1);
        double xb = static_cast<double>(hDisp_i);
        double xc = static_cast<double>(hDisp_i +1);
        double s_xb = neighborsMetric[1][1];

        double xd;
        double s_xd;

        offsetTransfo[1] = 0.0;

        for (unsigned int k=0; k<nbIterMax; k++)
          {
          if ( (xb-xa) < (xc-xb) )
            {
            xd = 0.5 * (xc+xb);
            offsetTransfo[0] = xd - static_cast<double>(hDisp_i);
            transfo->SetOffset(offsetTransfo);
            resampler->Modified();
            resampler->Update();
            shiftedIt.Initialize(m_Radius,resampler->GetOutput(),tinyShiftedRegion);
            s_xd = m_Functor(leftIt,shiftedIt);

            if ((s_xd<s_xb && m_Minimize) || (s_xd>s_xb && !m_Minimize))
              {
              xa = xb;

              xb = xd;
              s_xb = s_xd;
              }
            else
              {
              xc = xd;
              }
            }
          else
            {
            xd = 0.5 * (xa+xb);
            offsetTransfo[0] = xd - static_cast<double>(hDisp_i);
            transfo->SetOffset(offsetTransfo);
            resampler->Modified();
            resampler->Update();
            shiftedIt.Initialize(m_Radius,resampler->GetOutput(),tinyShiftedRegion);
            s_xd = m_Functor(leftIt,shiftedIt);

            if ((s_xd<s_xb && m_Minimize) || (s_xd>s_xb && !m_Minimize))
              {
              xc = xb;

              xb = xd;
              s_xb = s_xd;
              }
            else
              {
              xa = xd;
              }
            }
          }

        outHDispIt.Set( xb * stepDisparityInv );
        outMetricIt.Set( s_xb );
        }
      else if (verticalInterpolation && horizontalInterpolation)
        {
        double xa = static_cast<double>(hDisp_i);
        double ya = static_cast<double>(vDisp_i - 1);
        double xb = static_cast<double>(hDisp_i);
        double yb = static_cast<double>(vDisp_i);
        double yc = static_cast<double>(vDisp_i + 1);
        double xe = static_cast<double>(hDisp_i - 1);
        double ye = static_cast<double>(vDisp_i);
        double xf = static_cast<double>(hDisp_i + 1);

        double s_b = neighborsMetric[1][1];

        double xd = xb;
        double yd = yb;
        double s_d;

        for (unsigned int k=0; k<nbIterMax; k++)
          {
          // Vertical step
          centreHasMoved = false;
          if ( (yb-ya) < (yc-yb) )
            {
            yd = 0.5 * (yc+yb);
            offsetTransfo[0] = xd - static_cast<double>(hDisp_i);
            offsetTransfo[1] = yd - static_cast<double>(vDisp_i);
            transfo->SetOffset(offsetTransfo);
            resampler->Modified();
            resampler->Update();
            shiftedIt.Initialize(m_Radius,resampler->GetOutput(),tinyShiftedRegion);
            s_d = m_Functor(leftIt,shiftedIt);

            if ((s_d<s_b && m_Minimize) || (s_d>s_b && !m_Minimize))
              {
              centreHasMoved = true;

              ya = yb;

              yb = yd;
              s_b = s_d;
              }
            else
              {
              yc = yd;
              }
            }
          else
            {
            yd = 0.5 * (ya+yb);
            offsetTransfo[0] = xd - static_cast<double>(hDisp_i);
            offsetTransfo[1] = yd - static_cast<double>(vDisp_i);
            transfo->SetOffset(offsetTransfo);
            resampler->Modified();
            resampler->Update();
            shiftedIt.Initialize(m_Radius,resampler->GetOutput(),tinyShiftedRegion);
            s_d = m_Functor(leftIt,shiftedIt);

            if ((s_d<s_b && m_Minimize) || (s_d>s_b && !m_Minimize))
              {
              centreHasMoved = true;

              yc = yb;

              yb = yd;
              s_b = s_d;
              }
            else
              {
              ya = yd;
              }
            }
          if (centreHasMoved)
            {
            // update points e and f
            ye = yb;

            offsetTransfo[0] = xe - static_cast<double>(hDisp_i);
            offsetTransfo[1] = ye - static_cast<double>(vDisp_i);
            transfo->SetOffset(offsetTransfo);
            resampler->Modified();
            resampler->Update();
            shiftedIt.Initialize(m_Radius,resampler->GetOutput(),tinyShiftedRegion);


            offsetTransfo[0] = xf - static_cast<double>(hDisp_i);
            transfo->SetOffset(offsetTransfo);
            resampler->Modified();
            resampler->Update();
            shiftedIt.Initialize(m_Radius,resampler->GetOutput(),tinyShiftedRegion);
            }

          // Horizontal step
          centreHasMoved = false;
          if ( (xb-xe) < (xf-xb) )
            {
            xd = 0.5 * (xf+xb);
            offsetTransfo[0] = xd - static_cast<double>(hDisp_i);
            offsetTransfo[1] = yd - static_cast<double>(vDisp_i);
            transfo->SetOffset(offsetTransfo);
            resampler->Modified();
            resampler->Update();
            shiftedIt.Initialize(m_Radius,resampler->GetOutput(),tinyShiftedRegion);
            s_d = m_Functor(leftIt,shiftedIt);

            if ((s_d<s_b && m_Minimize) || (s_d>s_b && !m_Minimize))
              {
              centreHasMoved = true;

              xe = xb;

              xb = xd;
              s_b = s_d;
              }
            else
              {
              xf = xd;
              }
            }
          else
            {
            xd = 0.5 * (xe+xb);
            offsetTransfo[0] = xd - static_cast<double>(hDisp_i);
            offsetTransfo[1] = yd - static_cast<double>(vDisp_i);
            transfo->SetOffset(offsetTransfo);
            resampler->Modified();
            resampler->Update();
            shiftedIt.Initialize(m_Radius,resampler->GetOutput(),tinyShiftedRegion);
            s_d = m_Functor(leftIt,shiftedIt);

            if ((s_d<s_b && m_Minimize) || (s_d>s_b && !m_Minimize))
              {
              centreHasMoved = true;

              xf = xb;

              xb = xd;
              s_b = s_d;
              }
            else
              {
              xe = xd;
              }
            }
          if (centreHasMoved)
            {
            // update a and c
            xa = xb;

            offsetTransfo[0] = xa - static_cast<double>(hDisp_i);
            offsetTransfo[1] = ya - static_cast<double>(vDisp_i);
            transfo->SetOffset(offsetTransfo);
            resampler->Modified();
            resampler->Update();
            shiftedIt.Initialize(m_Radius,resampler->GetOutput(),tinyShiftedRegion);

            offsetTransfo[1] = yc - static_cast<double>(vDisp_i);
            transfo->SetOffset(offsetTransfo);
            resampler->Modified();
            resampler->Update();
            shiftedIt.Initialize(m_Radius,resampler->GetOutput(),tinyShiftedRegion);
            }

          }

        outHDispIt.Set( xb * stepDisparityInv);
        outVDispIt.Set( yb * stepDisparityInv);
        outMetricIt.Set( s_b );
        }

      if (!verticalInterpolation)
        {
        // No vertical interpolation done : simply copy the integer vertical disparity
        outVDispIt.Set( static_cast<double>(vDisp_i) * stepDisparityInv);
        }

      if (!horizontalInterpolation)
        {
        // No horizontal interpolation done : simply copy the integer horizontal disparity
        outHDispIt.Set( static_cast<double>(hDisp_i) * stepDisparityInv);
        }

      if (!verticalInterpolation && !horizontalInterpolation)
        {
        outMetricIt.Set(static_cast<double>(neighborsMetric[1][1]));
        }
      else
        {
        if ((outMetricIt.Get() > neighborsMetric[1][1] && m_Minimize) ||
            (outMetricIt.Get() < neighborsMetric[1][1] && !m_Minimize))
          {
          nb_WrongExtrema++;

          }
        }

      progress.CompletedPixel();

      ++outHDispIt;
      ++outVDispIt;
      ++outMetricIt;

      if (useHorizontalDisparity)
        {
        ++inHDispIt;
        }
      if (useVerticalDisparity)
        {
        ++inVDispIt;
        }
      }
    ++leftIt;

    if(inLeftMaskPtr)
      {
      ++inLeftMaskIt;
      }
    }

  m_WrongExtrema[threadId] = static_cast<double>(nb_WrongExtrema) /
      static_cast<double>(outputRegionForThread.GetNumberOfPixels());
}

template <class TInputImage, class TOutputMetricImage,
class TDisparityImage, class TMaskImage, class TBlockMatchingFunctor>
void
SubPixelDisparityImageFilter<TInputImage,TOutputMetricImage,
TDisparityImage,TMaskImage,TBlockMatchingFunctor>
::AfterThreadedGenerateData()
{
  double wrongExtremaPercent = 0;
  for (unsigned int i=0; i<m_WrongExtrema.size(); i++)
    {
    wrongExtremaPercent += m_WrongExtrema[i];
    }
  wrongExtremaPercent /= m_WrongExtrema.size();

  //std::cout << "Wrong extrema percentage = "<< wrongExtremaPercent << std::endl;
}

} // End namespace otb

#endif
