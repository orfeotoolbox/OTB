/*
 * Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
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
#ifndef otbStreamingBufferGenerator_hxx
#define otbStreamingBufferGenerator_hxx

#include "otbStreamingBufferGenerator.h"
#include "itkImageAlgorithm.h"

namespace otb
{

/**
 * Compute the output image
 */
template <class TInputImage>
void
StreamingBufferGenerator<TInputImage>::GenerateData()
{
  // Output pointer and requested region
  ImageType * inputPtr = static_cast<ImageType *>(Superclass::ProcessObject::GetInput(0));
  ImageType * outputPtr = this->GetOutput();
  const RegionType reqRegion = outputPtr->GetRequestedRegion();
  otbDebugMacro("Allocate a buffer of size " << reqRegion.GetSize(0) << "x" << reqRegion.GetSize(1));
  outputPtr->SetBufferedRegion(reqRegion);
  outputPtr->Allocate();
  otbDebugMacro("Allocation ok")

  m_StreamingManager->PrepareStreaming(inputPtr, reqRegion);
  int numberOfDivisions = m_StreamingManager->GetNumberOfSplits();

  const auto firstSplitSize = m_StreamingManager->GetSplit(0).GetSize();
  otbLogMacro(Info, << "Buffer will be written in " << numberOfDivisions << " blocks of " 
                    << firstSplitSize[0] << "x" << firstSplitSize[1] << " pixels");

  /**
   * Loop over the number of pieces, execute the upstream pipeline on each
   * piece, and copy the results into the output image.
   */
  itk::ProgressReporter progress(this, 0, numberOfDivisions);
  for (int currentDivision = 0; currentDivision < numberOfDivisions; currentDivision++)
  {
    otbDebugMacro("Processing region " << (currentDivision + 1) << " over " << numberOfDivisions);
    RegionType streamRegion = m_StreamingManager->GetSplit(currentDivision);
    otbDebugMacro("Region start: " << streamRegion.GetIndex() << " size: " << streamRegion.GetSize());

    // Propagate region
    inputPtr->SetRequestedRegion(streamRegion);
    inputPtr->PropagateRequestedRegion();
    inputPtr->UpdateOutputData();

    // Copy the subregion to output
    itk::ImageAlgorithm::Copy(inputPtr, outputPtr, streamRegion, streamRegion);
    progress.CompletedPixel();

    inputPtr->ReleaseData();
  }
}


} // end namespace otb


#endif