/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2019 Institut de Recherche en Sciences et
 *  Technologies pour l'Environnement et l'Agriculture (IRSTEA)
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
#ifndef __StreamingFeatherMosaicFilter_hxx
#define __StreamingFeatherMosaicFilter_hxx

#include "otbStreamingFeatherMosaicFilter.h"

namespace otb
{

template <class TInputImage, class TOutputImage, class TDistanceImage, class TInternalValueType>
StreamingFeatherMosaicFilter<TInputImage, TOutputImage, TDistanceImage, TInternalValueType>::StreamingFeatherMosaicFilter()
{
  m_FeatheringTransitionDistance = 500;
  m_FeatheringSmoothness         = 1.5;
}

/**
 * Processing
 */
template <class TInputImage, class TOutputImage, class TDistanceImage, class TInternalValueType>
void StreamingFeatherMosaicFilter<TInputImage, TOutputImage, TDistanceImage, TInternalValueType>::ThreadedGenerateData(
    const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{

  // Debug info
  itkDebugMacro(<< "Actually executing thread " << threadId << " in region " << outputRegionForThread);

  // Support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Get output pointer
  OutputImageType* mosaicImage = this->GetOutput();

  // Get number of used inputs
  const unsigned int nbOfUsedInputImages = Superclass::GetNumberOfUsedInputImages();

  // Get number of bands
  const unsigned int nBands = Superclass::GetNumberOfBands();

  // Iterate through the thread region
  IteratorType outputIt(mosaicImage, outputRegionForThread);

  // Prepare input pointers, interpolators, and valid regions (input images)
  typename std::vector<InputImageType*>         currentImage;
  typename std::vector<InterpolatorPointerType> interp;
  Superclass::PrepareImageAccessors(currentImage, interp);

  // Prepare input pointers, interpolators, and valid regions (distances images)
  typename std::vector<DistanceImageType*>               currentDistanceImage;
  typename std::vector<DistanceImageInterpolatorPointer> distanceInterpolator;
  Superclass::PrepareDistanceImageAccessors(currentDistanceImage, distanceInterpolator);

  // Temporary pixels
  InternalPixelType interpolatedMathPixel, tempOutputPixel;
  interpolatedMathPixel.SetSize(nBands);
  tempOutputPixel.SetSize(nBands);

  InputImagePixelType interpolatedPixel;
  InternalValueType   pixelValue, distanceImagePixel;
  bool                isDataInCurrentOutputPixel;

  // Temporary coordinates
  OutputImagePointType geoPoint;

  unsigned int band, i;

  for (outputIt.GoToBegin(); !outputIt.IsAtEnd(); ++outputIt)
  {
    // Current pixel --> Geographical point
    mosaicImage->TransformIndexToPhysicalPoint(outputIt.GetIndex(), geoPoint);

    // Presence of at least one non-null pixel of the used input images
    isDataInCurrentOutputPixel = false;

    // Transition pixels
    tempOutputPixel.Fill(0.0);

    // Loop on used input images
    for (i = 0; i < nbOfUsedInputImages; i++)
    {

      // Check if the point is inside the transformed thread region
      // (i.e. the region in the current input image which match the thread
      // region)
      if (interp[i]->IsInsideBuffer(geoPoint))
      {

        // Compute the interpolated pixel value
        interpolatedPixel = interp[i]->Evaluate(geoPoint);

        // Check that interpolated pixel is not empty
        if (Superclass::IsPixelNotEmpty(interpolatedPixel))
        {

          // Geographical point (current pixel) --> Continuous index (from
          // current input image)
          if (distanceInterpolator[i]->IsInsideBuffer(geoPoint))
          {
            distanceImagePixel = distanceInterpolator[i]->Evaluate(geoPoint);
            distanceImagePixel -= Superclass::GetDistanceOffset();

            // Check that the distance is positive (i.e. we are inside the valid
            // area of the image)
            if (distanceImagePixel > 0 || !isDataInCurrentOutputPixel)
            {

              // Two possibility: distance < buffer or not
              InternalValueType coef1, coef2;
              if (distanceImagePixel < m_FeatheringTransitionDistance && isDataInCurrentOutputPixel)
              {
                // Feather the image
                coef1 = distanceImagePixel / m_FeatheringTransitionDistance;
                coef1 = vcl_pow(coef1, m_FeatheringSmoothness);
                coef2 = 1.0 - coef1;
              }
              else
              {
                // Do not feather, put the image foreground
                coef1 = 1.0;
                coef2 = 0.0;
              }

              // Average-Weight pixels (current pixel, and temporary pixel)
              const unsigned int inputImageIndex = Superclass::GetUsedInputImageIndice(i);
              for (band = 0; band < nBands; band++)
              {
                // Cast the interpolated pixel to a internal pixel type
                interpolatedMathPixel[band] = static_cast<InternalValueType>(interpolatedPixel[band]);

                // Shift-scale the input pixel value
                if (this->GetShiftScaleInputImages())
                {
                  this->ShiftScaleValue(interpolatedMathPixel[band], inputImageIndex, band);
                }

                // Average-Weight
                tempOutputPixel[band] = coef1 * interpolatedMathPixel[band] + coef2 * tempOutputPixel[band];

              } // loop on pixels band

              // Update the presence of data for this pixel
              isDataInCurrentOutputPixel = true;

            } // Distance value is positive

          } // Interpolated distance pixel not empty

          else
          {
            itkWarningMacro(<< "Unable to evaluate distance at point " << geoPoint);
            distanceImagePixel = 0;
          }

        } // Interpolated pixel is not empty
      }   // point inside buffer
    }     // next image

    // Prepare output pixel
    OutputImagePixelType outputPixel(Superclass::GetNoDataOutputPixel());

    if (isDataInCurrentOutputPixel)
    {

      // Compute output pixel
      for (band = 0; band < nBands; band++)
      {

        // Normalize & cast
        pixelValue = tempOutputPixel[band];
        Superclass::NormalizePixelValue(pixelValue);
        outputPixel[band] = static_cast<OutputImageInternalPixelType>(pixelValue);
      }

    } // if data presence

    // Update output pixel value
    outputIt.Set(outputPixel);

    // Update progress
    progress.CompletedPixel();

  } // next output pixel
}

} // end namespace gtb

#endif
