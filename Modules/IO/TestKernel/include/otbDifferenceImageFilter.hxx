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

#ifndef otbDifferenceImageFilter_hxx
#define otbDifferenceImageFilter_hxx

#include "otbDifferenceImageFilter.h"

#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNeighborhoodAlgorithm.h"
#include "itkProgressReporter.h"
#include "itkDefaultConvertPixelTraits.h"

namespace otb
{

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputImage>
DifferenceImageFilter<TInputImage, TOutputImage>
::DifferenceImageFilter()
{
  // We require two inputs to execute.
  this->SetNumberOfRequiredInputs(2);

  // Set the default DifferenceThreshold.
  m_DifferenceThreshold = itk::NumericTraits<ScalarRealType>::Zero;

  // Set the default ToleranceRadius.
  m_ToleranceRadius = 0;

  // Initialize statistics about difference image.
  itk::NumericTraits<RealType>::SetLength(
    m_MeanDifference,
    itk::DefaultConvertPixelTraits<RealType>::GetNumberOfComponents());
  itk::NumericTraits<AccumulateType>::SetLength(
    m_TotalDifference,
    itk::DefaultConvertPixelTraits<RealType>::GetNumberOfComponents());
  m_MeanDifference  = itk::NumericTraits<RealType>::ZeroValue(m_MeanDifference);
  m_TotalDifference = itk::NumericTraits<AccumulateType>::ZeroValue(m_TotalDifference);
  m_NumberOfPixelsWithDifferences = 0;
}

//----------------------------------------------------------------------------
template<class TInputImage, class TOutputImage>
void
DifferenceImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "ToleranceRadius: " << m_ToleranceRadius << "\n";
  os << indent << "DifferenceThreshold: " << m_DifferenceThreshold << "\n";
  os << indent << "MeanDifference: " << m_MeanDifference << "\n";
  os << indent << "TotalDifference: " << m_TotalDifference << "\n";
  os << indent << "NumberOfPixelsWithDifferences: "
     << m_NumberOfPixelsWithDifferences << "\n";
}

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputImage>
void
DifferenceImageFilter<TInputImage, TOutputImage>
::SetValidInput(const InputImageType* validImage)
{
  // The valid image should be input 0.
  this->SetInput(0, validImage);
}

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputImage>
void
DifferenceImageFilter<TInputImage, TOutputImage>
::SetTestInput(const InputImageType* testImage)
{
  // The test image should be input 1.
  this->SetInput(1, testImage);
}
template<class TInputImage, class TOutputImage>
void
DifferenceImageFilter<TInputImage, TOutputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  if (this->GetInput(0)->GetNumberOfComponentsPerPixel() != this->GetInput(1)->GetNumberOfComponentsPerPixel())
    {
    itkExceptionMacro(<< "Image 1 has " << this->GetInput(
                        0)->GetNumberOfComponentsPerPixel() << " bands, whereas image 2 has " << this->GetInput(
                        1)->GetNumberOfComponentsPerPixel());
    }
  this->GetOutput()->SetNumberOfComponentsPerPixel(this->GetInput(0)->GetNumberOfComponentsPerPixel());
}
//----------------------------------------------------------------------------
template<class TInputImage, class TOutputImage>
void
DifferenceImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  int numberOfThreads = this->GetNumberOfThreads();

  itk::NumericTraits<RealType>::SetLength(
    m_MeanDifference,
    this->GetInput(0)->GetNumberOfComponentsPerPixel());
  itk::NumericTraits<AccumulateType>::SetLength(
    m_TotalDifference,
    this->GetInput(0)->GetNumberOfComponentsPerPixel());

  // Initialize statistics about difference image.
  m_MeanDifference  = itk::NumericTraits<RealType>::ZeroValue(m_MeanDifference);
  m_TotalDifference = itk::NumericTraits<AccumulateType>::ZeroValue(m_TotalDifference);
  m_NumberOfPixelsWithDifferences = 0;

  // Resize the thread temporaries
  m_ThreadDifferenceSum.reserve(numberOfThreads);
  m_ThreadNumberOfPixels.SetSize(numberOfThreads);

  // Initialize the temporaries
  for (int i = 0; i < numberOfThreads; ++i)
    {
    m_ThreadDifferenceSum.push_back(m_TotalDifference);
    }
  m_ThreadNumberOfPixels.Fill(0);
}

//----------------------------------------------------------------------------
template <class TInputImage, class TOutputImage>
void
DifferenceImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& threadRegion, itk::ThreadIdType threadId)
{
  typedef itk::ConstNeighborhoodIterator<InputImageType>                           SmartIterator;
  typedef itk::ImageRegionConstIterator<InputImageType>                            InputIterator;
  typedef itk::ImageRegionIterator<OutputImageType>                                OutputIterator;
  typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<InputImageType> FacesCalculator;
  typedef typename FacesCalculator::RadiusType                                     RadiusType;
  typedef typename FacesCalculator::FaceListType                                   FaceListType;
  typedef typename FaceListType::iterator                                          FaceListIterator;
  typedef typename InputImageType::PixelType                                       InputPixelType;

  // Prepare standard boundary condition.
  itk::ZeroFluxNeumannBoundaryCondition<InputImageType> nbc;

  // Get a pointer to each image.
  const InputImageType* validImage = this->GetInput(0);
  const InputImageType* testImage = this->GetInput(1);
  OutputImageType*      outputPtr = this->GetOutput();

  // Create a radius of pixels.
  RadiusType radius;
  if (m_ToleranceRadius > 0)
    {
    radius.Fill(m_ToleranceRadius);
    }
  else
    {
    radius.Fill(0);
    }

  // Find the data-set boundary faces.
  FacesCalculator boundaryCalculator;
  FaceListType    faceList = boundaryCalculator(testImage, threadRegion, radius);

  // Support progress methods/callbacks.
  itk::ProgressReporter progress(this, threadId, threadRegion.GetNumberOfPixels());

  // Process the internal face and each of the boundary faces.
  for (FaceListIterator face = faceList.begin(); face != faceList.end(); ++face)
    {
    SmartIterator test(radius, testImage, *face); // Iterate over test image.
    InputIterator valid(validImage, *face);       // Iterate over valid image.
    OutputIterator out(outputPtr, *face);         // Iterate over output image.
    test.OverrideBoundaryCondition(&nbc);

    for (valid.GoToBegin(), test.GoToBegin(), out.GoToBegin();
         !valid.IsAtEnd();
         ++valid, ++test, ++out)
      {
      // Get the current valid pixel.
      InputPixelType t = valid.Get();

      // Find the closest-valued pixel in the neighborhood of the test
      // image.
      OutputPixelType minimumDifference = itk::NumericTraits<OutputPixelType>::max(t);
      unsigned int    neighborhoodSize = test.Size();
      for (unsigned int i = 0; i < neighborhoodSize; ++i)
        {
        // Use the RealType for the difference to make sure we get the
        // sign.
        RealType difference = static_cast<RealType>(t) - static_cast<RealType>(test.GetPixel(i));

        for (unsigned int j = 0; j < itk::NumericTraits<RealType>::GetLength(difference) ; ++j)
          {
          ScalarRealType d = static_cast<ScalarRealType>(
            itk::DefaultConvertPixelTraits<RealType>::GetNthComponent(j,difference));
          if (d < 0)
            {
            d *= -1;
            }
          ScalarRealType m = static_cast<ScalarRealType>(
            itk::DefaultConvertPixelTraits<OutputPixelType>::GetNthComponent(j,minimumDifference));
          if (d < m)
            {
            itk::DefaultConvertPixelTraits<OutputPixelType>::SetNthComponent(
              j,
              minimumDifference,
              d);
//             std::cout << std::setprecision(16) << minimumDifference[j] << std::endl;
//             std::cout << std::setprecision(16) << t << std::endl;
//             std::cout << std::setprecision(16) << test.GetPixel(i) << std::endl;
//             std::cout << "----------------------" << std::endl;
            }
          }
        }

      //for complex and vector type. FIXME: module might be better
//        ScalarRealType tMax=vcl_abs(t[0]);
      ScalarRealType tMax = 0.01; //Avoiding the 0 case for neighborhood computing
      // NB: still more restrictive than before for small values.
      for (unsigned int j = 0; j < itk::NumericTraits<InputPixelType>::GetLength(t); ++j)
        {
        ScalarRealType tc = static_cast<ScalarRealType>(
          itk::DefaultConvertPixelTraits<InputPixelType>::GetNthComponent(j,t));
        if (vcl_abs(tc) > tMax) tMax = vcl_abs(tc);
        }

      // Check if difference is above threshold
      // the threshold is interpreted as relative to the value
      bool isDifferent = false;

      for (unsigned int j = 0; j < itk::NumericTraits<OutputPixelType>::GetLength(minimumDifference); ++j)
        {
        ScalarRealType m = static_cast<ScalarRealType>(
          itk::DefaultConvertPixelTraits<OutputPixelType>::GetNthComponent(j,minimumDifference));
        if (m > m_DifferenceThreshold * tMax)
          {
//           std::cout << std::setprecision(16) << minimumDifference[j] << std::endl;
          isDifferent = true;
          }
        }

      if (isDifferent)
        {
        // Store the minimum difference value in the output image.
        out.Set(minimumDifference);

        // Update difference image statistics.
        m_ThreadDifferenceSum[threadId] += minimumDifference;
        m_ThreadNumberOfPixels[threadId]++;
        }
      else
        {
        // Difference is below threshold.
        out.Set(itk::NumericTraits<OutputPixelType>::ZeroValue(minimumDifference));
        }

      // Update progress.
      progress.CompletedPixel();
      }
    }
}
//----------------------------------------------------------------------------
template <class TInputImage, class TOutputImage>
void
DifferenceImageFilter<TInputImage, TOutputImage>
::AfterThreadedGenerateData()
{
  // Set statistics about difference image.
  int numberOfThreads = this->GetNumberOfThreads();
  for (int i = 0; i < numberOfThreads; ++i)
    {
    m_TotalDifference += m_ThreadDifferenceSum[i];
    m_NumberOfPixelsWithDifferences += m_ThreadNumberOfPixels[i];
    }

  // Get the total number of pixels processed in the region.
  // This is different from the m_TotalNumberOfPixels which
  // is the number of pixels that actually have differences
  // above the intensity threshold.
  OutputImageRegionType region = this->GetOutput()->GetRequestedRegion();
  unsigned int          numberOfPixels = region.GetNumberOfPixels();

  // Calculate the mean difference.

  m_MeanDifference = m_TotalDifference / numberOfPixels;
}

} // end namespace otb

#endif
