/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbFrostImageFilter_hxx
#define otbFrostImageFilter_hxx

#include "otbFrostImageFilter.h"

#include "itkDataObject.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkNeighborhoodInnerProduct.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkOffset.h"
#include "itkProgressReporter.h"

namespace otb
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
FrostImageFilter<TInputImage, TOutputImage>::FrostImageFilter()
{
  m_Radius.Fill(1);
  m_Deramp = 2;
}

template <class TInputImage, class TOutputImage>
void FrostImageFilter<TInputImage, TOutputImage>::GenerateInputRequestedRegion() throw(itk::InvalidRequestedRegionError)
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the input and output
  typename Superclass::InputImagePointer  inputPtr  = const_cast<TInputImage*>(this->GetInput());
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
    std::ostringstream               msg;
    msg << static_cast<const char*>(this->GetNameOfClass()) << "::GenerateInputRequestedRegion()";
    e.SetLocation(msg.str());
    e.SetDescription("Requested region is (at least partially) outside the largest possible region.");
    e.SetDataObject(inputPtr);
    throw e;
  }
}

template <class TInputImage, class TOutputImage>
void FrostImageFilter<TInputImage, TOutputImage>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  unsigned int                                                        i;
  itk::ZeroFluxNeumannBoundaryCondition<InputImageType>               nbc;
  itk::ConstNeighborhoodIterator<InputImageType>                      bit;
  typename itk::ConstNeighborhoodIterator<InputImageType>::OffsetType off;
  itk::ImageRegionIterator<OutputImageType>                           it;

  // Allocate output
  typename OutputImageType::Pointer     output = this->GetOutput();
  typename InputImageType::ConstPointer input  = this->GetInput();

  // Find the data-set boundary "faces"
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType           faceList;
  typename itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType>::FaceListType::iterator fit;

  itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> bC;
  faceList = bC(input, outputRegionForThread, m_Radius);

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  InputRealType sum;
  InputRealType sum2;

  double Mean, Variance;
  double Alpha;
  double NormFilter;
  double FrostFilter;
  double CoefFilter;
  double dPixel;

  // Process each of the boundary faces.  These are N-d regions which border
  // the edge of the buffer.
  for (fit = faceList.begin(); fit != faceList.end(); ++fit)
  {
    bit                           = itk::ConstNeighborhoodIterator<InputImageType>(m_Radius, input, *fit);
    unsigned int neighborhoodSize = bit.Size();
    it                            = itk::ImageRegionIterator<OutputImageType>(output, *fit);
    bit.OverrideBoundaryCondition(&nbc);

    bit.GoToBegin();
    it.GoToBegin();

    while (!bit.IsAtEnd())
    {
      sum  = itk::NumericTraits<InputRealType>::Zero;
      sum2 = itk::NumericTraits<InputRealType>::Zero;

      for (i = 0; i < neighborhoodSize; ++i)
      {
        dPixel = static_cast<double>(bit.GetPixel(i));
        sum += dPixel;
      }
      Mean = sum / static_cast<double>(neighborhoodSize);

      for (i = 0; i < neighborhoodSize; ++i)
      {
        dPixel = static_cast<double>(bit.GetPixel(i));
        sum2 += (dPixel - Mean) * (dPixel - Mean);
      }
      Variance = sum2 / double(neighborhoodSize - 1);

      const double epsilon = 0.0000000001;
      if (std::abs(Mean) < epsilon)
      {
        dPixel = itk::NumericTraits<OutputPixelType>::Zero;
      }
      else if (std::abs(Variance) < epsilon)
      {
        dPixel = Mean;
      }
      else
      {
        Alpha = m_Deramp * Variance / (Mean * Mean);

        NormFilter  = 0.0;
        FrostFilter = 0.0;

        const int rad_x = m_Radius[0];
        const int rad_y = m_Radius[1];

        for (int x = -rad_x; x <= rad_x; ++x)
        {
          for (int y = -rad_y; y <= rad_y; ++y)
          {
            double Dist = std::sqrt(static_cast<double>(x * x + y * y));
            off[0]      = x;
            off[1]      = y;

            dPixel = static_cast<double>(bit.GetPixel(off));

            CoefFilter = std::exp(-Alpha * Dist);
            NormFilter += CoefFilter;
            FrostFilter += (CoefFilter * dPixel);
          }
        }

        dPixel = FrostFilter / NormFilter;
      }

      it.Set(static_cast<OutputPixelType>(dPixel));

      ++bit;
      ++it;
      progress.CompletedPixel();
    }
  }
}

/**
 * Standard "PrintSelf" method
 */
template <class TInputImage, class TOutput>
void FrostImageFilter<TInputImage, TOutput>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Radius: " << m_Radius << std::endl;
}

} // end namespace otb

#endif
