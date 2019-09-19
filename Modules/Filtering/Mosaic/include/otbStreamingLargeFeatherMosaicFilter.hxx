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
#ifndef __StreamingLargeFeatherMosaicFilter_hxx
#define __StreamingLargeFeatherMosaicFilter_hxx

#include "otbStreamingLargeFeatherMosaicFilter.h"

namespace otb {

/**
 * Processing
 */
template <class TInputImage, class TOutputImage, class TDistanceImage, class TInternalValueType>
void
StreamingLargeFeatherMosaicFilter<TInputImage, TOutputImage, TDistanceImage, TInternalValueType>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{

  // Debug info
  itkDebugMacro(<<"Actually executing thread " << threadId << " in region " << outputRegionForThread);

  // Support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels() );

  // Get output pointer
  OutputImageType * mosaicImage = this->GetOutput();

  // Get number of used inputs
  const unsigned int nbOfUsedInputImages = Superclass::GetNumberOfUsedInputImages();

  // Get number of bands
  const unsigned int nBands = Superclass::GetNumberOfBands();

  // Iterate through the thread region
  IteratorType outputIt(mosaicImage, outputRegionForThread);

  // Prepare input pointers, interpolators, and valid regions (input images)
  typename std::vector<InputImageType *>        currentImage;
  typename std::vector<InterpolatorPointerType> interp;
  Superclass::PrepareImageAccessors(currentImage, interp);

  // Prepare input pointers, interpolators, and valid regions (distances images)
  typename std::vector<DistanceImageType *>              currentDistanceImage;
  typename std::vector<DistanceImageInterpolatorPointer> distanceInterpolator;
  Superclass::PrepareDistanceImageAccessors(currentDistanceImage, distanceInterpolator);

  // Temporary thread region (from input)
  InputImageRegionType threadRegionInCurrentImage;

  // Temporary pixels
  InternalPixelType interpolatedMathPixel, tempOutputPixel;
  interpolatedMathPixel.SetSize(nBands);
  tempOutputPixel.SetSize(nBands);

  InputImagePixelType interpolatedPixel;
  InternalValueType   pixelValue, distanceImagePixel, sumDistances;
  bool                isDataInCurrentOutputPixel;

  // Temporary coordinates
  OutputImagePointType geoPoint;

  unsigned int band, i;

  for ( outputIt.GoToBegin(); !outputIt.IsAtEnd(); ++outputIt )
    {
    // Current pixel --> Geographical point
    mosaicImage->TransformIndexToPhysicalPoint (outputIt.GetIndex(), geoPoint) ;

    // Presence of at least one non-null pixel of the used input images
    isDataInCurrentOutputPixel = false;
    sumDistances = 0.0;

    // Transition pixels
    tempOutputPixel.Fill(0.0);

    // Loop on used input images
    for (i = 0 ; i < nbOfUsedInputImages ; i++)
      {

      // Check if the point is inside the transformed thread region
      // (i.e. the region in the current input image which match the thread
      // region)
      if (interp[i]->IsInsideBuffer(geoPoint) )
        {
        // Compute the interpolated pixel value
        interpolatedPixel = interp[i]->Evaluate(geoPoint);

        // Check that interpolated pixel is not empty
        if (Superclass::IsPixelNotEmpty(interpolatedPixel) )
          {
          // Get the alpha channel pixel value for this channel
          if (distanceInterpolator[i]->IsInsideBuffer(geoPoint) )
            {
            distanceImagePixel = distanceInterpolator[i]->Evaluate(geoPoint);
            distanceImagePixel -= Superclass::GetDistanceOffset();

            if (distanceImagePixel>0 )
              {
              // Update the presence of data for this pixel
              isDataInCurrentOutputPixel = true;

              // sum coef
              sumDistances += distanceImagePixel;

              /*
               * 1. Cast the interpolated pixel into math pixel type
               * 2. Multiply by feather coef
               * 3. Compute sum
               */
              const unsigned int inputImageIndex = Superclass::GetUsedInputImageIndice(i);
              for (band = 0 ; band < nBands ; band++)
                {
                // Cast the interpolated pixel to a internal pixel type
                interpolatedMathPixel[band] = static_cast<InternalValueType>(interpolatedPixel[band]);

                // Shift-scale the value
                if (this->GetShiftScaleInputImages() )
                  {
                  this->ShiftScaleValue(interpolatedMathPixel[band], inputImageIndex, band);
                  }

                // Multiply by Feather coef
                interpolatedMathPixel[band] *= distanceImagePixel;

                // Summing
                tempOutputPixel[band] += interpolatedMathPixel[band];

                } // loop on pixel bands

              } // distance > 0
            }   // Interpolated distance pixel not empty
          else
            {
            itkWarningMacro(<<"Unable to evaluate distance at point " << geoPoint);
            }
          } // Interpolated pixel is not empty
        }   // point inside buffer
      }     // next image

    // Prepare output pixel
    OutputImagePixelType outputPixel(Superclass::GetNoDataOutputPixel() );

    if (isDataInCurrentOutputPixel)
      {

      // Compute output pixel
      for (band = 0 ; band < nBands ; band++)
        {

        // Normalize & cast
        pixelValue = tempOutputPixel[band] / sumDistances;
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
