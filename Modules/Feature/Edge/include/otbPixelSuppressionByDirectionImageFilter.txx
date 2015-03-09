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
#ifndef __otbPixelSuppressionByDirectionImageFilter_txx
#define __otbPixelSuppressionByDirectionImageFilter_txx

#include "otbPixelSuppressionByDirectionImageFilter.h"

#include "itkDataObject.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkConstantBoundaryCondition.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"
#include "otbMath.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
PixelSuppressionByDirectionImageFilter<TInputImage, TOutputImage>::PixelSuppressionByDirectionImageFilter()
{

  m_Radius.Fill(1);
  m_AngularBeam = static_cast<double>(0.);

}

template <class TInputImage, class TOutputImage>
void
PixelSuppressionByDirectionImageFilter<TInputImage, TOutputImage>
::SetInputImage(const InputImageType *input)
{
  this->SetInput(0, input);
}

template <class TInputImage, class TOutputImage>
void
PixelSuppressionByDirectionImageFilter<TInputImage, TOutputImage>
::SetInputImageDirection(const InputImageType *input)
{
  this->SetInput(1, input);
}

template <class TInputImage, class TOutputImage>
const
typename PixelSuppressionByDirectionImageFilter<TInputImage, TOutputImage>::InputImageType *
PixelSuppressionByDirectionImageFilter<TInputImage, TOutputImage>
::GetInputImage(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TInputImage *>
           (this->GetInput(0));
}

template <class TInputImage, class TOutputImage>
const
typename PixelSuppressionByDirectionImageFilter<TInputImage, TOutputImage>::InputImageType *
PixelSuppressionByDirectionImageFilter<TInputImage, TOutputImage>
::GetInputImageDirection(void)
{
  if (this->GetNumberOfInputs() < 1)
    {
    return 0;
    }

  return static_cast<const TInputImage *>
           (this->GetInput(1));
}

template <class TInputImage, class TOutputImage>
void PixelSuppressionByDirectionImageFilter<TInputImage, TOutputImage>::GenerateInputRequestedRegion() throw (
  itk::InvalidRequestedRegionError)
  {
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  typename Superclass::InputImagePointer  inputPtr   =  const_cast<TInputImage *>(this->GetInputImageDirection());
  typename Superclass::OutputImagePointer outputPtr = this->GetOutput();

  if (!inputPtr || !outputPtr)
    {
    return;
    }

  // get a copy of the input requested region (should equal the output
  // requested region)
  typename TInputImage::RegionType inputRequestedRegion;
  inputRequestedRegion = inputPtr->GetRequestedRegion();

  // pad the input requested region by the operator radius
  inputRequestedRegion.PadByRadius(m_Radius);

  // crop the input requested region at the input's largest possible region
  if (inputRequestedRegion.Crop(inputPtr->GetLargestPossibleRegion()))
    {
    inputPtr->SetRequestedRegion(inputRequestedRegion);
    return;
    }
  else
    {
    // Couldn't crop the region (requested region is outside the largest
    // possible region).  Throw an exception.

    // store what we tried to request (prior to trying to crop)
    inputPtr->SetRequestedRegion(inputRequestedRegion);

    // build an exception
    itk::InvalidRequestedRegionError e(__FILE__, __LINE__);
    std::ostringstream msg;
    msg << static_cast<const char *>(this->GetNameOfClass())
        << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str().c_str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
    }
  }

template<class TInputImage, class TOutputImage>
void PixelSuppressionByDirectionImageFilter<TInputImage, TOutputImage>::ThreadedGenerateData(
  const OutputImageRegionType&     outputRegionForThread,
  itk::ThreadIdType threadId
  )
{

  itk::ConstantBoundaryCondition<InputImageType> cbc;
  const InputPixelType                           cvalue = 255;
  cbc.SetConstant(cvalue);

  itk::ConstNeighborhoodIterator<InputImageType> bit;
  itk::ImageRegionConstIterator<InputImageType>  itin;
  itk::ImageRegionIterator<OutputImageType>      itout;

  // Allocate output
  typename OutputImageType::Pointer     output = this->GetOutput();
  typename InputImageType::ConstPointer input  = this->GetInputImage();
  typename InputImageType::ConstPointer inputDirection  = this->GetInputImageDirection();

  // Find the data-set boundary "faces"
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType           faceList;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType::iterator fit;

  itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> bC;
  faceList = bC(inputDirection, outputRegionForThread, m_Radius);

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  //typename TInputImage::IndexType     bitIndex;

  //---------------------------------------------------------------------------

  InputPixelType PixelValue;

  // Location of the central pixel in the input image
//  int Xc, Yc;

  // Pixel location in the system axis of the region
  int x, y;

  // Pixel location in the system axis of the region after rotation of theta
  // where theta is the direction of the cantral pixel

  // Pixel Direction
  double ThetaXcYc, Thetaxtyt;

  //---------------------------------------------------------------------------

  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  for (fit = faceList.begin(); fit != faceList.end(); ++fit)
    {
    bit = itk::ConstNeighborhoodIterator<InputImageType>(m_Radius, inputDirection, *fit);

    itin = itk::ImageRegionConstIterator<InputImageType>(input, *fit);
    itout = itk::ImageRegionIterator<OutputImageType>(output, *fit);

    bit.OverrideBoundaryCondition(&cbc);
    bit.GoToBegin();

    while (!bit.IsAtEnd())
      {

      /*// Location of the central pixel of the region in the input image
      bitIndex = bit.GetIndex();

      Xc = bitIndex[0];
      Yc = bitIndex[1];
      */
      // Get Pixel Direction from the image of directions
      ThetaXcYc = static_cast<double>(bit.GetCenterPixel());

      // Pixel intensity in the input image
      PixelValue = itin.Get();

      bool IsLine = false;

      typename itk::ConstNeighborhoodIterator<InputImageType>::OffsetType off;
      // Loop on the region
      for (unsigned int i = 0; i < 2 * m_Radius[0] + 1; ++i)
        for (unsigned int j = 0; j < 2 * m_Radius[1] + 1; ++j)
          {

          off[0] = i - m_Radius[0];
          off[1] = j - m_Radius[1];

          x = off[0];
          y = off[1];

          // No calculation on the central pixel
          if ((x == 0) && (y == 0)) continue;

          Thetaxtyt = vcl_atan2(static_cast<double>(y), static_cast<double>(x));   //result is [-PI, PI]
          while (Thetaxtyt < 0)
            Thetaxtyt = CONST_PI + Thetaxtyt;  // Theta is now [0, PI] as is
          // the result of detectors
          while (Thetaxtyt > CONST_PI_2)
            Thetaxtyt = Thetaxtyt - CONST_PI;  // Theta is now [-PI/2, PI/2]

          if ((vcl_abs(vcl_cos(Thetaxtyt - ThetaXcYc)) >= vcl_cos(m_AngularBeam)) // this
              // pixel
              // is
              // in
              // the
              // angular beam
              && (vcl_abs(vcl_cos(bit.GetPixel(off) - ThetaXcYc)) >= vcl_cos(m_AngularBeam))) //and
          //its
          //direction
          //is
          //also
          //in
          //the beam
            {
            IsLine = true;
            continue;
            }

          }

      // end of the loop on the pixels of the region

      // Assignment of this value to the output pixel
      if (IsLine == true)
        {
        itout.Set(static_cast<OutputPixelType>(PixelValue));
        }
      else
        {
        itout.Set(static_cast<OutputPixelType>(0.));
        }

      ++bit;
      ++itin;
      ++itout;
      progress.CompletedPixel();

      }

    }
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void
PixelSuppressionByDirectionImageFilter<TInputImage, TOutput>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Radius: " << m_Radius << std::endl;
}

} // end namespace otb

#endif
