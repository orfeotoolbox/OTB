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
#ifndef __otbStereoSensorModelToElevationMapFilter_txx
#define __otbStereoSensorModelToElevationMapFilter_txx

#include "otbStereorectificationDeformationFieldSource.h"
#include "itkProgressReporter.h"
#include "otbMath.h"

// For partial specialization
#include "otbVectorImage.h"

#include "otbDEMHandler.h"

namespace otb
{

template <class TInputImage, class TOutputImage>
StereorectificationDeformationFieldSource<TInputImage, TOutputImage>
::StereorectificationDeformationFieldSource() :
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
  this->SetNumberOfOutputs(2);

  // Create the 2nd input (not created by default)
  this->SetNthOutput(0, OutputImageType::New());
  this->SetNthOutput(1, OutputImageType::New());

  // Build the RS Transforms
  m_LeftToRightTransform = RSTransformType::New();
  m_RightToLeftTransform = RSTransformType::New();
}

template <class TInputImage, class TOutputImage>
const typename StereorectificationDeformationFieldSource<TInputImage, TOutputImage>
::OutputImageType *
StereorectificationDeformationFieldSource<TInputImage, TOutputImage>
::GetLeftDeformationFieldOutput() const
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<const OutputImageType *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputImage, class TOutputImage>
typename StereorectificationDeformationFieldSource<TInputImage, TOutputImage>
::OutputImageType *
StereorectificationDeformationFieldSource<TInputImage, TOutputImage>
::GetLeftDeformationFieldOutput()
{
  if (this->GetNumberOfOutputs() < 1)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->itk::ProcessObject::GetOutput(0));
}

template <class TInputImage, class TOutputImage>
const typename StereorectificationDeformationFieldSource<TInputImage, TOutputImage>
::OutputImageType *
StereorectificationDeformationFieldSource<TInputImage, TOutputImage>
::GetRightDeformationFieldOutput() const
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<const OutputImageType *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputImage>
typename StereorectificationDeformationFieldSource<TInputImage, TOutputImage>
::OutputImageType *
StereorectificationDeformationFieldSource<TInputImage, TOutputImage>
::GetRightDeformationFieldOutput()
{
  if (this->GetNumberOfOutputs() < 2)
    {
    return 0;
    }
  return static_cast<OutputImageType *>(this->itk::ProcessObject::GetOutput(1));
}

template <class TInputImage, class TOutputImage>
void
StereorectificationDeformationFieldSource<TInputImage, TOutputImage>
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

  leftToGroundTransform->InstanciateTransform();

  // Retrieve the deformation field pointers
  OutputImageType * leftDFPtr = this->GetLeftDeformationFieldOutput();
  OutputImageType * rightDFPtr = this->GetRightDeformationFieldOutput();

  // Set up  the RS transforms
  m_LeftToRightTransform->SetInputKeywordList(m_LeftImage->GetImageKeywordlist());
  m_LeftToRightTransform->SetOutputKeywordList(m_RightImage->GetImageKeywordlist());
  m_LeftToRightTransform->InstanciateTransform();

  m_RightToLeftTransform->SetInputKeywordList(m_RightImage->GetImageKeywordlist());
  m_RightToLeftTransform->SetOutputKeywordList(m_LeftImage->GetImageKeywordlist());
  m_RightToLeftTransform->InstanciateTransform();

  // Now, we must determine the optimized size, spacing and origin of the
  // stereo-rectified images, as well as the position of the origin in
  // the left image

  // First, spacing
  SpacingType outputSpacing;
  outputSpacing.Fill(m_Scale * m_GridStep);
  outputSpacing[0]*=m_LeftImage->GetSpacing()[0];
  outputSpacing[1]*=m_LeftImage->GetSpacing()[1];

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

  // The begining of the epipolar line in the left image is the image
  // of rightEpiPoint at a lower elevation (using the offset)
  rightEpiPoint[2] = localElevation - m_ElevationOffset;
  leftEpiLineStart = m_RightToLeftTransform->TransformPoint(rightEpiPoint);

  // The ending of the epipolar line in the left image is the image
  // of rightEpiPoint at a higher elevation (using the offset)
  rightEpiPoint[2] = localElevation + m_ElevationOffset;
  leftEpiLineEnd = m_RightToLeftTransform->TransformPoint(rightEpiPoint);

  // Now, we can compute the equation of the epipolar line y = a*x+b
  // (do not forget that the y axis is flip in our case)
  // TODO: Add some division by zero check here (but this would only
  // happen in case the images are almost epipolar already)
  double a = (leftEpiLineEnd[1] - leftEpiLineStart[1])
           / (leftEpiLineEnd[0] - leftEpiLineStart[0]);
  double b = leftEpiLineStart[1] - a * leftEpiLineStart[0];

  // Now, we will change coordinate so that the epipolar line is
  // horizontal
  double alpha = otb::CONST_PI - vcl_atan(a);

  // And compute the unitary vectors of the new axis (equivalent to
  // the column of the rotation matrix)
  // TODO: Check if we need to use the input image spacing here
  double ux =   vcl_cos(alpha);
  double uy = - vcl_sin(alpha);
  double vx =   vcl_sin(alpha);
  double vy =   vcl_cos(alpha);

  // Now, we will compute the bounding box of the left input image in
  // this rotated geometry

  // First we compute coordinates of the 4 corners (we omit ulx which
  // coordinates are {0,0})
  double urx = ux * m_LeftImage->GetLargestPossibleRegion().GetSize()[0];
  double ury = vx * m_LeftImage->GetLargestPossibleRegion().GetSize()[0];
  double llx = uy * m_LeftImage->GetLargestPossibleRegion().GetSize()[1];
  double lly = vy * m_LeftImage->GetLargestPossibleRegion().GetSize()[1];
  double lrx = ux * m_LeftImage->GetLargestPossibleRegion().GetSize()[0]
             + uy * m_LeftImage->GetLargestPossibleRegion().GetSize()[1];
  double lry = vx * m_LeftImage->GetLargestPossibleRegion().GetSize()[0]
             + vy * m_LeftImage->GetLargestPossibleRegion().GetSize()[1];

  // Bounding box (this time we do not omit ulx)
  double minx = std::min(std::min(std::min(urx,llx),lrx),0.);
  double miny = std::min(std::min(std::min(ury,lly),lry),0.);
  double maxx = std::max(std::max(std::max(urx,llx),lrx),0.);
  double maxy = std::max(std::max(std::max(ury,lly),lry),0.);

  // We can now estimate the output image size, taking into account
  // the scale parameter
  m_RectifiedImageSize[0] = static_cast<unsigned int>((maxx-minx)/m_Scale);
  m_RectifiedImageSize[1] = static_cast<unsigned int>((maxy-miny)/m_Scale);

  // Now, we can compute the origin of the epipolar images in the left
  // input image geometry (we rotate back)
  m_OutputOriginInLeftImage[0] = leftInputOrigin[0] + m_LeftImage->GetSpacing()[0] * (ux * minx + vx * miny);
  m_OutputOriginInLeftImage[1] = leftInputOrigin[1] + m_LeftImage->GetSpacing()[0] * (uy * minx + vy * miny);
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

  leftDFPtr->SetSpacing(outputSpacing);
  rightDFPtr->SetSpacing(outputSpacing);

  leftDFPtr->SetNumberOfComponentsPerPixel(2);
  rightDFPtr->SetNumberOfComponentsPerPixel(2);
}

template <class TInputImage, class TOutputImage>
void
StereorectificationDeformationFieldSource<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(itk::DataObject * output)
{
  // Call superclass
  Superclass::EnlargeOutputRequestedRegion(output);

  // Retrieve the deformation field pointers
  OutputImageType * leftDFPtr = this->GetLeftDeformationFieldOutput();
  OutputImageType * rightDFPtr = this->GetRightDeformationFieldOutput();

  // Prevent from streaming
  leftDFPtr->SetRequestedRegionToLargestPossibleRegion();
  rightDFPtr->SetRequestedRegionToLargestPossibleRegion();
}

template <class TInputImage, class TOutputImage>
void
StereorectificationDeformationFieldSource<TInputImage, TOutputImage>
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

  leftToGroundTransform->InstanciateTransform();

  // Retrieve the output pointers
  OutputImageType * leftDFPtr = this->GetLeftDeformationFieldOutput();
  OutputImageType * rightDFPtr = this->GetRightDeformationFieldOutput();

  // Declare all the TDPoint variables we will need
  TDPointType currentPoint1, currentPoint2,nextLineStart1,nextLineStart2, startLine1, endLine1, startLine2, endLine2, epiPoint1, epiPoint2;

  // Then, we retrieve the origin of the left input image
  double localElevation = otb::DEMHandler::Instance()->GetDefaultHeightAboveEllipsoid();

  // Initialize
  currentPoint1 = m_OutputOriginInLeftImage;
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
    double a1,a2;

    // First, for image 1

    // This point is the image of the left input image origin at the
    // average elevation
    if(m_UseDEM)
      {
      RSTransform2DType::InputPointType tmpPoint;
      tmpPoint[0] = currentPoint1[0];
      tmpPoint[1] = currentPoint1[1];
      localElevation = demHandler->GetHeightAboveEllipsoid(leftToGroundTransform->TransformPoint(tmpPoint));
      }
    currentPoint1[2] = localElevation;
    epiPoint2 = m_LeftToRightTransform->TransformPoint(currentPoint1);

    // The begining of the epipolar line in the left image is the image
    // of epiPoint2 at a lower elevation (using the offset)
    epiPoint2[2] = localElevation - m_ElevationOffset;
    startLine1 = m_RightToLeftTransform->TransformPoint(epiPoint2);

    // The endning of the epipolar line in the left image is the image
    // of epiPoint2 at a higher elevation (using the offset)
    epiPoint2[2] = localElevation + m_ElevationOffset;
    endLine1 = m_RightToLeftTransform->TransformPoint(epiPoint2);

    // Estimate the local baseline ratio
    double localBaselineRatio = vcl_sqrt((endLine1[0] - startLine1[0])
                                *  (endLine1[0] - startLine1[0])
                                +  (endLine1[1] - startLine1[1])
                                *  (endLine1[1] - startLine1[1]))
                                / (2 * m_ElevationOffset);

    m_MeanBaselineRatio+=localBaselineRatio;

    // Now, we can compute the equation of the epipolar line y = a*x+b
    // (do not forget that the y axis is flip in our case)
    // TODO: Add some division by zero check here (but this would only
    // happen in case the images are almost epipolar already)
    a1 = (endLine1[1] - startLine1[1]) / (endLine1[0] - startLine1[0]);

    // We do the same for image 2
    currentPoint2[2] = localElevation;
    epiPoint1 = m_RightToLeftTransform->TransformPoint(currentPoint2);

    epiPoint1[2] = localElevation - m_ElevationOffset;
    startLine2 = m_LeftToRightTransform->TransformPoint(epiPoint1);

    epiPoint1[2] = localElevation + m_ElevationOffset;
    endLine2 = m_LeftToRightTransform->TransformPoint(epiPoint1);

    a2 = (endLine2[1] - startLine2[1]) / (endLine2[0] - startLine2[0]);

    // 4 - Determine position of next points

    // We want to move m_Scale pixels away in the epipolar line of the
    // first image
    // TODO: Take into account height direction ?
    double alpha1 = otb::CONST_PI - vcl_atan(a1);
    double deltax1 =  m_Scale * m_GridStep * m_LeftImage->GetSpacing()[0] * vcl_cos(alpha1);
    double deltay1 = -m_Scale * m_GridStep * m_LeftImage->GetSpacing()[1] * vcl_sin(alpha1);

    // Before moving currentPoint1, we will store its image by the
    // left to right transform at the m_AverageElevation, to compute
    // the equivalent displacement in right image

    currentPoint1[2] = localElevation;
    startLine2 = m_LeftToRightTransform->TransformPoint(currentPoint1);

    // Now we move currentPoint1
    currentPoint1[0]+=deltax1;
    currentPoint1[1]+=deltay1;
    currentPoint1[2] = localElevation;

    // And we compute the equivalent displacement in right image
    endLine2 = m_LeftToRightTransform->TransformPoint(currentPoint1);

    double iscale = vcl_sqrt((endLine2[0]-startLine2[0])*(endLine2[0]-startLine2[0])
                             +
                             (endLine2[1]-startLine2[1])*(endLine2[1]-startLine2[1]));

    // Now, we compute the displacement in right image. The iscale
    // factor already account for m_GridStep and scale, no need to use
    // them again
    double alpha2 = otb::CONST_PI - vcl_atan(a2);
    double deltax2 =   iscale * vcl_cos(alpha2);
    double deltay2 = - iscale * vcl_sin(alpha2);

    // We can now move currentPoint2
    currentPoint2[0] += deltax2;
    currentPoint2[1] += deltay2;
    currentPoint2[2] =  localElevation;

    // 5 - Finally, we have to handle a special case for beginning of
    // line, since at this position we are able to compute the
    // position of the beginning of next line
    if(it1.GetIndex()[0] == 0 || it2.GetIndex()[0] == 0)
      {
      // We want to move 1 pixel away in the direction orthogonal to
      // epipolar line
      double nextdeltax1 = m_Scale * m_LeftImage->GetSpacing()[0] * m_GridStep * vcl_sin(alpha1);
      double nextdeltay1 = m_Scale * m_LeftImage->GetSpacing()[1] *m_GridStep * vcl_cos(alpha1);

      // We can then update nextLineStart1
      nextLineStart1[0] = currentPoint1[0] - deltax1 + nextdeltax1;
      nextLineStart1[1] = currentPoint1[1] - deltay1 + nextdeltay1;
      nextLineStart1[2] = localElevation;

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
StereorectificationDeformationFieldSource<TInputImage, TOutputImage>
::PrintSelf( std::ostream& os, itk::Indent indent ) const
{
  // Call superclass implementation
  Superclass::PrintSelf(os,indent);
}

} // end namespace otb

#endif
