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

#ifndef otbDEMToImageGenerator_hxx
#define otbDEMToImageGenerator_hxx

#include "otbDEMToImageGenerator.h"
#include "otbMacro.h"
#include "itkProgressReporter.h"

namespace otb
{

template <class TDEMImage>
DEMToImageGenerator<TDEMImage>::DEMToImageGenerator()
{
  m_DEMHandler          = DEMHandlerType::Instance();
  m_OutputSpacing[0]    = 0.0001;
  m_OutputSpacing[1]    = -0.0001;
  m_OutputSize[0]       = 1;
  m_OutputSize[1]       = 1;
  m_OutputOrigin[0]     = 0;
  m_OutputOrigin[1]     = 0;
  m_DefaultUnknownValue = itk::NumericTraits<PixelType>::ZeroValue();
  m_AboveEllipsoid      = false;

  m_Transform = GenericRSTransformType::New();
}

// GenerateOutputInformation method
template <class TDEMImage>
void DEMToImageGenerator<TDEMImage>::GenerateOutputInformation()
{
  DEMImageType* output;
  output = this->GetOutput(0);

  IndexType start;
  start[0] = 0;
  start[1] = 0;

  // Specify region parameters
  OutputImageRegionType largestPossibleRegion;
  largestPossibleRegion.SetSize(m_OutputSize);
  largestPossibleRegion.SetIndex(start);

  output->SetLargestPossibleRegion(largestPossibleRegion);
  output->SetSignedSpacing(m_OutputSpacing);
  output->SetOrigin(m_OutputOrigin);

  // Add the metadata set by the user to the output
  output->m_Imd.Add(MDGeom::ProjectionProj, m_Transform->GetInputProjectionRef());
  output->m_Imd.Merge(*m_Transform->GetInputImageMetadata());
}

// InstantiateTransform method
template <class TDEMImage>
void DEMToImageGenerator<TDEMImage>::InstantiateTransform()
{
  m_Transform->InstantiateTransform();
}

template <class TDEMImage>
void DEMToImageGenerator<TDEMImage>::BeforeThreadedGenerateData()
{
  InstantiateTransform();
  DEMImagePointerType DEMImage = this->GetOutput();

  // allocate the output buffer
  DEMImage->SetBufferedRegion(DEMImage->GetRequestedRegion());
  DEMImage->Allocate();
  DEMImage->FillBuffer(0);
}


template <class TDEMImage>
void DEMToImageGenerator<TDEMImage>::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
{
  DEMImagePointerType DEMImage = this->GetOutput();

  // Create an iterator that will walk the output region
  ImageIteratorType outIt = ImageIteratorType(DEMImage, outputRegionForThread);

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Walk the output image, evaluating the height at each pixel
  IndexType currentindex;
  PointType phyPoint;
  double    height;
  PointType geoPoint;

  for (outIt.GoToBegin(); !outIt.IsAtEnd(); ++outIt)
  {
    currentindex = outIt.GetIndex();
    DEMImage->TransformIndexToPhysicalPoint(currentindex, phyPoint);


    if (m_Transform.IsNotNull())
    {
      geoPoint = m_Transform->TransformPoint(phyPoint);
      if (m_AboveEllipsoid)
      {
        height = m_DEMHandler->GetHeightAboveEllipsoid(geoPoint); // Altitude
                                                                  // calculation
      }
      else
      {
        height = m_DEMHandler->GetHeightAboveMSL(geoPoint); // Altitude
                                                            // calculation
      }
    }
    else
    {
      if (m_AboveEllipsoid)
      {
        height = m_DEMHandler->GetHeightAboveEllipsoid(phyPoint); // Altitude
                                                                  // calculation
      }
      else
      {
        height = m_DEMHandler->GetHeightAboveMSL(phyPoint); // Altitude
                                                            // calculation
      }
    }
    /*    otbMsgDevMacro(<< "Index : (" << currentindex[0]<< "," << currentindex[1] << ") -> PhyPoint : ("
                       << phyPoint[0] << "," << phyPoint[1] << ") -> GeoPoint: ("
                       << geoPoint[0] << "," << geoPoint[1] << ") -> height" << height);
    */
    //       otbMsgDevMacro(<< "height" << height);
    // DEM sets a default value (-32768) at point where it doesn't have altitude information.
    // OSSIM has chosen to change this default value in OSSIM_DBL_NAN (-4.5036e15).
    if (!vnl_math_isnan(height))
    {
      // Fill the image
      DEMImage->SetPixel(currentindex, static_cast<PixelType>(height));
    }
    else
    {
      // Back to the MNT default value
      DEMImage->SetPixel(currentindex, m_DefaultUnknownValue);
    }
    progress.CompletedPixel();
  }
}

template <class TDEMImage>
void DEMToImageGenerator<TDEMImage>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Output Spacing:" << m_OutputSpacing[0] << "," << m_OutputSpacing[1] << std::endl;
  os << indent << "Output Origin:" << m_OutputOrigin[0] << "," << m_OutputOrigin[1] << std::endl;
  os << indent << "Output Size:" << m_OutputSize[0] << "," << m_OutputSize[1] << std::endl;
}

} // namespace otb

#endif
