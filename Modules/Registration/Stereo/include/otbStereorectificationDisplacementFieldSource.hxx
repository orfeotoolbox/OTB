/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbStereoSensorModelToElevationMapFilter_hxx
#define otbStereoSensorModelToElevationMapFilter_hxx

#include "otbStereorectificationDisplacementFieldSource.h"
#include "itkProgressReporter.h"
#include "otbMath.h"

// For partial specialization
#include "otbVectorImage.h"

#include "otbDEMHandler.h"

namespace otb
{

template <class TInputImage, class TOutputImage>
StereorectificationDisplacementFieldSource<TInputImage, TOutputImage>
::StereorectificationDisplacementFieldSource() :
  m_ElevationOffset(50),
  m_Scale(1),
  m_GridStep(1),
  m_LeftImage(),
  m_RightImage(),
  m_LeftToRightTransform(),
  m_RightToLeftTransform(),
  m_OutputOriginInLeftImage(),
  m_MeanBaselineRatio(0),
  m_UseDEM(false)
{
  // Set the number of outputs to 2 (one deformation field for each
  // image)
  this->SetNumberOfRequiredOutputs(2);

  // Create the 2nd input (not created by default)
  this->SetNthOutput(0, OutputImageType::New());
  this->SetNthOutput(1, OutputImageType::New());

  // Build the RS Transforms
  m_LeftToRightTransform = RSTransformType::New();
  m_RightToLeftTransform = RSTransformType::New();
}

template <class TInputImage, class TOutputImage>
const typename StereorectificationDisplacementFieldSource<TInputImage, TOutputImage>
::OutputImageType *
StereorectificationDisplacementFieldSource<TInputImage, TOutputImage>
::GetLeftDisplacementFieldOutput() const
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<const OutputImageType *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputImage, class TOutputImage>
typename StereorectificationDisplacementFieldSource<TInputImage, TOutputImage>
::OutputImageType *
StereorectificationDisplacementFieldSource<TInputImage, TOutputImage>
::GetLeftDisplacementFieldOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return nullptr;
    }
  return static_cast<OutputImageType *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputImage, class TOutputImage>
const typename StereorectificationDisplacementFieldSource<TInputImage, TOutputImage>
::OutputImageType *
StereorectificationDisplacementFieldSource<TInputImage, TOutputImage>
::GetRightDisplacementFieldOutput() const
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<const OutputImageType *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputImage>
typename StereorectificationDisplacementFieldSource<TInputImage, TOutputImage>
::OutputImageType *
StereorectificationDisplacementFieldSource<TInputImage, TOutputImage>
::GetRightDisplacementFieldOutput()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return nullptr;
    }
  return static_cast<OutputImageType *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputImage>
void
StereorectificationDisplacementFieldSource<TInputImage, TOutputImage>
::GenerateOutputInformation()
{
  //Superclass::GenerateOutputInformation();

  // Ensure that both left image and right image are available
  if(m_LeftImage.IsNull() || m_RightImage.IsNull())
    {
    itkExceptionMacro(<<"Either left image or right image pointer is null, can not perform stereo-rectification.");
    }

  // Ensure input images have up-to-date information
  m_LeftImage->UpdateOutputInformation();
  m_RightImage->UpdateOutputInformation();

  // Setup the DEM handler if needed
  typename DEMHandler::Pointer demHandler = DEMHandler::Instance();

  // Set-up a transform to use the DEMHandler
  typedef otb::GenericRSTransform<> RSTransform2DType;
  RSTransform2DType::Pointer leftToGroundTransform = RSTransform2DType::New();
  leftToGroundTransform->SetInputKeywordList(m_LeftImage->GetImageKeywordlist());

  leftToGroundTransform->InstantiateTransform();

  // Retrieve the deformation field pointers
  OutputImageType * leftDFPtr = this->GetLeftDisplacementFieldOutput();
  OutputImageType * rightDFPtr = this->GetRightDisplacementFieldOutput();

  // Set up  the RS transforms
  m_LeftToRightTransform->SetInputKeywordList(m_LeftImage->GetImageKeywordlist());
  m_LeftToRightTransform->SetOutputKeywordList(m_RightImage->GetImageKeywordlist());
  m_LeftToRightTransform->InstantiateTransform();

  m_RightToLeftTransform->SetInputKeywordList(m_RightImage->GetImageKeywordlist());
  m_RightToLeftTransform->SetOutputKeywordList(m_LeftImage->GetImageKeywordlist());
  m_RightToLeftTransform->InstantiateTransform();

  // Now, we must determine the optimized size, spacing and origin of the
  // stereo-rectified images, as well as the position of the origin in
  // the left image

  // First, spacing : choose a square spacing,
  SpacingType outputSpacing;
  outputSpacing.Fill(m_Scale * m_GridStep);
  double mean_spacing=0.5*(std::abs(m_LeftImage->GetSignedSpacing()[0])+std::abs(m_LeftImage->GetSignedSpacing()[1]));
  //double ratio_x = mean_spacing / std::abs(m_LeftImage->GetSignedSpacing()[0]);
  //double ratio_y = mean_spacing / std::abs(m_LeftImage->GetSignedSpacing()[1]);

  outputSpacing[0]*=mean_spacing;
  outputSpacing[1]*=mean_spacing;

  // Then, we retrieve the origin of the left input image
  double localElevation = otb::DEMHandler::Instance()->GetDefaultHeightAboveEllipsoid();

  if(m_UseDEM)
    {
    RSTransform2DType::InputPointType tmpPoint;
    localElevation = demHandler->GetHeightAboveEllipsoid(leftToGroundTransform->TransformPoint(m_LeftImage->GetOrigin()));
    }

  TDPointType leftInputOrigin;
  leftInputOrigin[0] = m_LeftImage->GetOrigin()[0];
  leftInputOrigin[1] = m_LeftImage->GetOrigin()[1];
  leftInputOrigin[2] = localElevation;

  // Next, we will compute the parameters of the local epipolar line
  // at the left image origin
  TDPointType rightEpiPoint, leftEpiLineStart, leftEpiLineEnd;

  // This point is the image of the left input image origin at the
  // average elevation
  rightEpiPoint = m_LeftToRightTransform->TransformPoint(leftInputOrigin);

  // The beginning of the epipolar line in the left image is the image
  // of rightEpiPoint at a lower elevation (using the offset)
  rightEpiPoint[2] = localElevation - m_ElevationOffset;
  leftEpiLineStart = m_RightToLeftTransform->TransformPoint(rightEpiPoint);

  // The ending of the epipolar line in the left image is the image
  // of rightEpiPoint at a higher elevation (using the offset)
  rightEpiPoint[2] = localElevation + m_ElevationOffset;
  leftEpiLineEnd = m_RightToLeftTransform->TransformPoint(rightEpiPoint);

  // Now, we can compute the equation of the epipolar line y = a*x+b
  // epipolar angle is computed in left image physical space
  double alpha = 0;
  if (leftEpiLineEnd[0] == leftEpiLineStart[0])
    {
    if (leftEpiLineEnd[1] > leftEpiLineStart[1])
      {
      alpha = 0.5*otb::CONST_PI;
      }
    else
      {
      alpha = -0.5*otb::CONST_PI;
      }
    }
  else
    {
    double a = (leftEpiLineEnd[1] - leftEpiLineStart[1])
             / (leftEpiLineEnd[0] - leftEpiLineStart[0]);
    if (leftEpiLineEnd[0] > leftEpiLineStart[0])
      {
      alpha = std::atan(a);
      }
    else
      {
      alpha = otb::CONST_PI + std::atan(a);
      }

    }

  // And compute the unitary vectors of the new axis (equivalent to
  // the column of the rotation matrix)
  // TODO: Check if we need to use the input image spacing here
  double ux =   std::cos(alpha);
  double uy =   std::sin(alpha);
  double vx = - std::sin(alpha);
  double vy =   std::cos(alpha);

  // Now, we will compute the bounding box of the left input image in
  // this rotated geometry

  // First we compute coordinates of the 4 corners (we omit ulx which
  // coordinates are {0,0})
  double urx = ux * m_LeftImage->GetLargestPossibleRegion().GetSize()[0] * m_LeftImage->GetSignedSpacing()[0];
  double ury = vx * m_LeftImage->GetLargestPossibleRegion().GetSize()[0] * m_LeftImage->GetSignedSpacing()[0];
  double llx = uy * m_LeftImage->GetLargestPossibleRegion().GetSize()[1] * m_LeftImage->GetSignedSpacing()[1];
  double lly = vy * m_LeftImage->GetLargestPossibleRegion().GetSize()[1] * m_LeftImage->GetSignedSpacing()[1];
  double lrx = ux * m_LeftImage->GetLargestPossibleRegion().GetSize()[0] * m_LeftImage->GetSignedSpacing()[0]
             + uy * m_LeftImage->GetLargestPossibleRegion().GetSize()[1] * m_LeftImage->GetSignedSpacing()[1];
  double lry = vx * m_LeftImage->GetLargestPossibleRegion().GetSize()[0] * m_LeftImage->GetSignedSpacing()[0]
             + vy * m_LeftImage->GetLargestPossibleRegion().GetSize()[1] * m_LeftImage->GetSignedSpacing()[1];

  // Bounding box (this time we do not omit ulx)
  double minx = std::min(std::min(std::min(urx,llx),lrx),0.);
  double miny = std::min(std::min(std::min(ury,lly),lry),0.);
  double maxx = std::max(std::max(std::max(urx,llx),lrx),0.);
  double maxy = std::max(std::max(std::max(ury,lly),lry),0.);

  // We can now estimate the output image size, taking into account
  // the scale parameter
  m_RectifiedImageSize[0] = static_cast<unsigned int>((maxx-minx)/(mean_spacing*m_Scale));
  m_RectifiedImageSize[1] = static_cast<unsigned int>((maxy-miny)/(mean_spacing*m_Scale));

  // Now, we can compute the origin of the epipolar images in the left
  // input image geometry (we rotate back)
  m_OutputOriginInLeftImage[0] = leftInputOrigin[0] + (ux * minx + vx * miny);
  m_OutputOriginInLeftImage[1] = leftInputOrigin[1] + (uy * minx + vy * miny);
  m_OutputOriginInLeftImage[2] = localElevation;

  // And also the size of the deformation field
  SizeType outputSize;
  outputSize[0] = (m_RectifiedImageSize[0] / m_GridStep + 2 );
  outputSize[1] = (m_RectifiedImageSize[1] / m_GridStep + 2);

  // Build the output largest region
  RegionType outputLargestRegion;
  outputLargestRegion.SetSize(outputSize);

  // Update the information
  leftDFPtr->SetLargestPossibleRegion(outputLargestRegion);
  rightDFPtr->SetLargestPossibleRegion(outputLargestRegion);

  leftDFPtr->SetSignedSpacing(outputSpacing);
  rightDFPtr->SetSignedSpacing(outputSpacing);

  leftDFPtr->SetNumberOfComponentsPerPixel(2);
  rightDFPtr->SetNumberOfComponentsPerPixel(2);
}

template <class TInputImage, class TOutputImage>
void
StereorectificationDisplacementFieldSource<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(itk::DataObject * output)
{
  // Call superclass
  Superclass::EnlargeOutputRequestedRegion(output);

  // Retrieve the deformation field pointers
  OutputImageType * leftDFPtr = this->GetLeftDisplacementFieldOutput();
  OutputImageType * rightDFPtr = this->GetRightDisplacementFieldOutput();

  // Prevent from streaming
  leftDFPtr->SetRequestedRegionToLargestPossibleRegion();
  rightDFPtr->SetRequestedRegionToLargestPossibleRegion();
}

template <class TInputImage, class TOutputImage>
void
StereorectificationDisplacementFieldSource<TInputImage, TOutputImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  // Setup the DEM handler if needed
  typename DEMHandler::Pointer demHandler = DEMHandler::Instance();

  // Set-up a transform to use the DEMHandler
  typedef otb::GenericRSTransform<> RSTransform2DType;
  RSTransform2DType::Pointer leftToGroundTransform = RSTransform2DType::New();

  leftToGroundTransform->SetInputKeywordList(m_LeftImage->GetImageKeywordlist());

  leftToGroundTransform->InstantiateTransform();

  // Retrieve the output pointers
  OutputImageType * leftDFPtr = this->GetLeftDisplacementFieldOutput();
  OutputImageType * rightDFPtr = this->GetRightDisplacementFieldOutput();

  // Declare all the TDPoint variables we will need
  TDPointType currentPoint1, currentPoint2,nextLineStart1,nextLineStart2, startLine1, endLine1, startLine2, endLine2, epiPoint1, epiPoint2;

  // Then, we retrieve the origin of the left input image
  double localElevation = otb::DEMHandler::Instance()->GetDefaultHeightAboveEllipsoid();

  // Use the mean spacing as before
  double mean_spacing=0.5*(std::abs(m_LeftImage->GetSignedSpacing()[0])+std::abs(m_LeftImage->GetSignedSpacing()[1]));

  // Initialize
  currentPoint1 = m_OutputOriginInLeftImage;
  if(m_UseDEM)
    {
    RSTransform2DType::InputPointType tmpPoint;
    tmpPoint[0] = currentPoint1[0];
    tmpPoint[1] = currentPoint1[1];
    localElevation = demHandler->GetHeightAboveEllipsoid(leftToGroundTransform->TransformPoint(tmpPoint));
    }
  currentPoint1[2] = localElevation;
  currentPoint2 = m_LeftToRightTransform->TransformPoint(currentPoint1);
  currentPoint2[2] = currentPoint1[2];

  // These are the points were the next stereo-rectified image line starts
  nextLineStart1 = currentPoint1;
  nextLineStart2 = currentPoint2;

  // We define the iterators we will use
  typedef itk::ImageRegionIteratorWithIndex<OutputImageType> IteratorType;

  IteratorType it1(leftDFPtr,leftDFPtr->GetLargestPossibleRegion());
  IteratorType it2(rightDFPtr,rightDFPtr->GetLargestPossibleRegion());

  it1.GoToBegin();
  it2.GoToBegin();

  // Reset the mean baseline ratio
  m_MeanBaselineRatio = 0;

  // Set-up progress reporting
  itk::ProgressReporter progress(this, 0, leftDFPtr->GetLargestPossibleRegion().GetNumberOfPixels());


  // We loop on the deformation fields
  while(!it1.IsAtEnd() && !it2.IsAtEnd())
    {
    // 1 - We start by handling the special case where we are at beginning
    // of a new line
    if(it1.GetIndex()[0] == 0 || it2.GetIndex()[0] == 0)
      {
      // In which case we reset the current points
      currentPoint1 = nextLineStart1;
      currentPoint2 = nextLineStart2;
      }

    // 2 - Next, we will fill the deformation fields
    typename OutputImageType::PixelType dFValue1 = it1.Get();
    typename OutputImageType::PixelType dFValue2 = it2.Get();

    // We must cast iterators position to physical space
    PointType currentDFPoint1, currentDFPoint2;
    leftDFPtr->TransformIndexToPhysicalPoint(it1.GetIndex(), currentDFPoint1);
    rightDFPtr->TransformIndexToPhysicalPoint(it2.GetIndex(), currentDFPoint2);

    // Now we can compute the shifts
    dFValue1[0] = currentPoint1[0] - currentDFPoint1[0];
    dFValue1[1] = currentPoint1[1] - currentDFPoint1[1];
    dFValue2[0] = currentPoint2[0] - currentDFPoint2[0];
    dFValue2[1] = currentPoint2[1] - currentDFPoint2[1];

    // And set the values
    it1.Set(dFValue1);
    it2.Set(dFValue2);

    // 3 - Next, we will compute epipolar lines direction in both
    // images
    double a1;

    // First, for image 1

    // This point is the image of the left input image origin at the
    // average elevation
    epiPoint2 = m_LeftToRightTransform->TransformPoint(currentPoint1);

    // The beginning of the epipolar line in the left image is the image
    // of epiPoint2 at a lower elevation (using the offset)
    epiPoint2[2] = localElevation - m_ElevationOffset;
    startLine1 = m_RightToLeftTransform->TransformPoint(epiPoint2);

    // The endning of the epipolar line in the left image is the image
    // of epiPoint2 at a higher elevation (using the offset)
    epiPoint2[2] = localElevation + m_ElevationOffset;
    endLine1 = m_RightToLeftTransform->TransformPoint(epiPoint2);

    // Estimate the local baseline ratio
    double localBaselineRatio = std::sqrt((endLine1[0] - startLine1[0])
                                *  (endLine1[0] - startLine1[0])
                                +  (endLine1[1] - startLine1[1])
                                *  (endLine1[1] - startLine1[1]))
                                / (2 * m_ElevationOffset);

    m_MeanBaselineRatio+=localBaselineRatio;

    // Now, we can compute the equation of the epipolar line y = a*x+b
    // (compute angle in physical space)
    double alpha1 = 0;
    if (endLine1[0] == startLine1[0])
      {
      if (endLine1[1] > startLine1[1])
        {
        alpha1 = 0.5*otb::CONST_PI;
        }
      else
        {
        alpha1 = -0.5*otb::CONST_PI;
        }
      }
    else
      {
      a1 = (endLine1[1] - startLine1[1]) / (endLine1[0] - startLine1[0]);
      if (endLine1[0] > startLine1[0])
        {
        alpha1 = std::atan(a1);
        }
      else
        {
        alpha1 = otb::CONST_PI + std::atan(a1);
        }
      }

    // We do the same for image 2
    currentPoint2[2] = localElevation;
    epiPoint1 = m_RightToLeftTransform->TransformPoint(currentPoint2);

    epiPoint1[2] = localElevation - m_ElevationOffset;
    startLine2 = m_LeftToRightTransform->TransformPoint(epiPoint1);

    epiPoint1[2] = localElevation + m_ElevationOffset;
    endLine2 = m_LeftToRightTransform->TransformPoint(epiPoint1);

    // 4 - Determine position of next points

    // We want to move m_Scale pixels away in the epipolar line of the
    // first image
    // Take into account height direction
    //double alpha1 = otb::CONST_PI - std::atan(a1);
    double deltax1 =  m_Scale * m_GridStep * mean_spacing * std::cos(alpha1);
    double deltay1 =  m_Scale * m_GridStep * mean_spacing * std::sin(alpha1);

    // Now we move currentPoint1
    currentPoint1[0]+=deltax1;
    currentPoint1[1]+=deltay1;
    if(m_UseDEM)
      {
      RSTransform2DType::InputPointType tmpPoint;
      tmpPoint[0] = currentPoint1[0];
      tmpPoint[1] = currentPoint1[1];
      localElevation = demHandler->GetHeightAboveEllipsoid(leftToGroundTransform->TransformPoint(tmpPoint));
      }
    currentPoint1[2] = localElevation;

    // And we compute the equivalent displacement in right image
    currentPoint2 = m_LeftToRightTransform->TransformPoint(currentPoint1);

    // 5 - Finally, we have to handle a special case for beginning of
    // line, since at this position we are able to compute the
    // position of the beginning of next line
    if(it1.GetIndex()[0] == 0 || it2.GetIndex()[0] == 0)
      {
      // We want to move 1 pixel away in the direction orthogonal to
      // epipolar line
      double nextdeltax1 = -m_Scale * mean_spacing * m_GridStep * std::sin(alpha1);
      double nextdeltay1 = m_Scale * mean_spacing * m_GridStep * std::cos(alpha1);

      // We can then update nextLineStart1
      nextLineStart1[0] = currentPoint1[0] - deltax1 + nextdeltax1;
      nextLineStart1[1] = currentPoint1[1] - deltay1 + nextdeltay1;
      nextLineStart1[2] = localElevation;
      if(m_UseDEM)
        {
        RSTransform2DType::InputPointType tmpPoint;
        tmpPoint[0] = nextLineStart1[0];
        tmpPoint[1] = nextLineStart1[1];
        nextLineStart1[2] = demHandler->GetHeightAboveEllipsoid(leftToGroundTransform->TransformPoint(tmpPoint));
        }


      // By construction, nextLineStart2 is always the image of
      // nextLineStart1 by the left to right transform at the m_AverageElevation
      nextLineStart2 = m_LeftToRightTransform->TransformPoint(nextLineStart1);
      }

    // Last, we move forward
    ++it1;
    ++it2;

    // Update progress
    progress.CompletedPixel();
    }

  // Compute the mean baseline ratio
  m_MeanBaselineRatio /= leftDFPtr->GetBufferedRegion().GetNumberOfPixels();
}

template <class TInputImage, class TOutputImage>
void
StereorectificationDisplacementFieldSource<TInputImage, TOutputImage>
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
