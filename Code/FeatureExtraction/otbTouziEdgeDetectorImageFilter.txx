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
#ifndef __otbTouziEdgeDetectorImageFilter_txx
#define __otbTouziEdgeDetectorImageFilter_txx

#include "otbTouziEdgeDetectorImageFilter.h"

#include "itkDataObject.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkZeroFluxNeumannBoundaryCondition.h"
#include "itkProgressReporter.h"
#include "otbMacro.h"
#include "otbMath.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
TouziEdgeDetectorImageFilter<TInputImage, TOutputImage, TOutputImageDirection>::TouziEdgeDetectorImageFilter()
{
  m_Radius.Fill(1);
}

template <class TInputImage, class TOutputImage, class TOutputImageDirection>
void TouziEdgeDetectorImageFilter<TInputImage, TOutputImage,
    TOutputImageDirection>::GenerateInputRequestedRegion() throw (
  itk::InvalidRequestedRegionError)
  {
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  typename Superclass::InputImagePointer  inputPtr   =  const_cast<TInputImage *>(this->GetInput());
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

/**
 * Set up state of filter before multi-threading.
 * InterpolatorType::SetInputImage is not thread-safe and hence
 * has to be set up before ThreadedGenerateData
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
void
TouziEdgeDetectorImageFilter<TInputImage, TOutputImage, TOutputImageDirection>
::BeforeThreadedGenerateData()
{

  typename OutputImageDirectionType::RegionType region;
  typename OutputImageType::Pointer             output = this->GetOutput();

  OutputImageDirectionType * direction = this->GetOutputDirection();

  region.SetSize(output->GetRequestedRegion().GetSize());
  region.SetIndex(output->GetRequestedRegion().GetIndex());
  direction->SetRegions(region);
  direction->SetOrigin(output->GetOrigin());
  direction->SetSpacing(output->GetSpacing());
  direction->Allocate();
}

template <class TInputImage, class TOutputImage, class TOutputImageDirection>
void
TouziEdgeDetectorImageFilter<TInputImage, TOutputImage, TOutputImageDirection>
::ThreadedGenerateData(
  const OutputImageRegionType&     outputRegionForThread,
  itk::ThreadIdType threadId
  )
{
  unsigned int                                          i;
  itk::ZeroFluxNeumannBoundaryCondition<InputImageType> nbc;
  itk::ConstNeighborhoodIterator<InputImageType>        bit;
  itk::ImageRegionIterator<OutputImageType>             it;
  itk::ImageRegionIterator<OutputImageType>             it_dir;

  // Allocate output
  typename OutputImageType::Pointer          output = this->GetOutput();
  typename InputImageType::ConstPointer      input  = this->GetInput();
  typename OutputImageDirectionType::Pointer outputDir = this->GetOutputDirection();

  // Find the data-set boundary "faces"
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType           faceList;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType::iterator fit;

  itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> bC;
  faceList = bC(input, outputRegionForThread, m_Radius);

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  typename TInputImage::IndexType bitIndex;

  // Initializations
  // ---------------
  // Number of direction
  const int NB_DIR = 4;
  // Number of region of the filter
  const int NB_REGION = 2;
  // Definition of the 4 directions
  double Theta[NB_DIR];

  Theta[0] = 0.;
  Theta[1] = CONST_PI_4;
  Theta[2] = CONST_PI_2;
  Theta[3] = 3 * CONST_PI / 4.;

  // contains for the 4 directions the sum of the pixels belonging to each region
  double Sum[NB_DIR][NB_REGION];
  // Mean of region 1
  double M1;
  // Mean of region 2
  double M2;
  // Result of the filter for each direction
  double R_theta[NB_DIR];
  double Sum_R_theta = 0.;
  // Intensity of the contour
  double R_contour;
  // Direction of the contour
  double Dir_contour = 0.;
  // sign of the contour
  int sign;

  // Pixel location in the input image
  int x;
  int y;
  // Location of the central pixel in the input image
  int xc;
  int yc;

  int cpt = 0;

  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  for (fit = faceList.begin(); fit != faceList.end(); ++fit)
    {

    cpt += 1;

    bit = itk::ConstNeighborhoodIterator<InputImageType>(m_Radius, input, *fit);
    unsigned int neighborhoodSize = bit.Size();

    it = itk::ImageRegionIterator<OutputImageType>(output, *fit);
    it_dir = itk::ImageRegionIterator<OutputImageDirectionType>(outputDir, *fit);

    bit.OverrideBoundaryCondition(&nbc);
    bit.GoToBegin();

    while (!bit.IsAtEnd())
      {

      // Location of the pixel central
      bitIndex = bit.GetIndex();

      xc = bitIndex[0];
      yc = bitIndex[1];

      // Initializations
      for (int dir = 0; dir < NB_DIR; ++dir)
        {
        for (int m = 0; m < NB_REGION; m++)
          Sum[dir][m] = 0.;
        }

      R_contour = -1;
      Dir_contour = 0.;
      Sum_R_theta = 0.;

      // Loop on pixels of the filter
      for (i = 0; i < neighborhoodSize; ++i)
        {

        bitIndex = bit.GetIndex(i);
        x = bitIndex[0];
        y = bitIndex[1];

        // We determine for each direction with which region the pixel belongs.

        // Horizontal direction
        if (y < yc) Sum[0][0] += static_cast<double>(bit.GetPixel(i));
        else if (y > yc) Sum[0][1] += static_cast<double>(bit.GetPixel(i));

        // Diagonal direction 1
        if ((y - yc) < (x - xc)) Sum[1][0] += static_cast<double>(bit.GetPixel(i));
        else if ((y - yc) > (x - xc)) Sum[1][1] += static_cast<double>(bit.GetPixel(i));

        // Vertical direction
        if (x > xc) Sum[2][0] += static_cast<double>(bit.GetPixel(i));
        else if (x < xc) Sum[2][1] += static_cast<double>(bit.GetPixel(i));

        // Diagonal direction 2
        if ((y - yc) > -(x - xc)) Sum[3][0] += static_cast<double>(bit.GetPixel(i));
        else if ((y - yc) < -(x - xc)) Sum[3][1] += static_cast<double>(bit.GetPixel(i));

        } // end of the loop on pixels of the filter

      // Loop on the 4 directions
      for (int dir = 0; dir < NB_DIR; ++dir)
        {
        // Calculation of the mean of the 2 regions
        M1 = Sum[dir][0] / static_cast<double>(m_Radius[0] * (2 * m_Radius[0] + 1));
        M2 = Sum[dir][1] / static_cast<double>(m_Radius[0] * (2 * m_Radius[0] + 1));

        // Calculation of the intensity of the contour
        if ((M1 != 0) && (M2 != 0)) R_theta[dir] = static_cast<double>(1 - std::min((M1 / M2), (M2 / M1)));
        else R_theta[dir] = 0.;

        // Determination of the maximum intensity of the contour
        R_contour = static_cast<double>(std::max(R_contour, R_theta[dir]));

        // Determination of the sign of contour
        if (M2 > M1) sign = +1;
        else sign = -1;

        Dir_contour += sign * Theta[dir] * R_theta[dir];
        Sum_R_theta += R_theta[dir];

        } // end of the loop on the directions

      // Assignment of this value to the output pixel
      it.Set(static_cast<OutputPixelType>(R_contour));

      // Determination of the direction of the contour
      if (Sum_R_theta != 0.) Dir_contour = Dir_contour / Sum_R_theta;

      // Assignment of this value to the "outputdir" pixel
      it_dir.Set(static_cast<OutputPixelDirectionType>(Dir_contour));

      ++bit;
      ++it;
      ++it_dir;
      progress.CompletedPixel();

      }

    }
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutputImage, class TOutputImageDirection>
void
TouziEdgeDetectorImageFilter<TInputImage, TOutputImage, TOutputImageDirection>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Radius: " << m_Radius << std::endl;
}

} // end namespace otb

#endif
