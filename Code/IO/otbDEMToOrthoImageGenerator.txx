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
#ifndef __otbDEMToOrthoImageGenerator_txx
#define __otbDEMToOrthoImageGenerator_txx

#include "otbDEMToOrthoImageGenerator.h"
#include "otbMacro.h"
#include "itkProgressReporter.h"

namespace otb
{

template<class TDEMImage, class TMapProjection>
DEMToOrthoImageGenerator<TDEMImage, TMapProjection>
::DEMToOrthoImageGenerator()
{
  m_MapProjection = NULL;
}

template <class TDEMImage, class TMapProjection>
void
DEMToOrthoImageGenerator<TDEMImage, TMapProjection>
::BeforeThreadedGenerateData()
{
  if (!m_MapProjection)
    {
    itkExceptionMacro(<<
                      "Please set map projection!");
    }
  DEMImagePointerType DEMImage = this->GetOutput();

  // allocate the output buffer
  DEMImage->SetBufferedRegion(DEMImage->GetRequestedRegion());
  DEMImage->Allocate();
  DEMImage->FillBuffer(0);
}

// GenerateData method
template <class TDEMImage, class TMapProjection>
void
DEMToOrthoImageGenerator<TDEMImage, TMapProjection>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId)
{

  DEMImagePointerType DEMImage = this->GetOutput();

  // Create an iterator that will walk the output region
  ImageIteratorType outIt = ImageIteratorType(DEMImage, outputRegionForThread);

  // support progress methods/callbacks
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Walk the output image, evaluating the height at each pixel
  IndexType currentindex;
  PointType cartoPoint;
  double    height;
  PointType geoPoint;
  for (outIt.GoToBegin(); !outIt.IsAtEnd(); ++outIt)
    {
    currentindex = outIt.GetIndex();

    DEMImage->TransformIndexToPhysicalPoint(currentindex, cartoPoint);

//     otbMsgDevMacro(<< std::setprecision(15) << "CartoPoint: (" << cartoPoint[0] << "," << cartoPoint[1] << ")");

    geoPoint = m_MapProjection->TransformPoint(cartoPoint);

//     otbMsgDevMacro(<< "GeoPoint: (" << geoPoint[0] << "," << geoPoint[1] << ")");

    height = this->m_DEMHandler->GetHeightAboveMSL(geoPoint);  // Altitude calculation
    otbMsgDevMacro(<< "Index : (" << currentindex[0]<< "," << currentindex[1] << ") -> PhyPoint : ("
                   << cartoPoint[0] << "," << cartoPoint[1] << ") -> GeoPoint: ("
                   << geoPoint[0] << "," << geoPoint[1] << ") -> height" << height);

//     otbMsgDevMacro(<< "height: " << height);
    // MNT sets a default value (-32768) at point where it doesn't have altitude information.
    // OSSIM has chosen to change this default value in OSSIM_DBL_NAN (-4.5036e15).
    if (!ossim::isnan(height))
      {
      // Fill the image
      DEMImage->SetPixel(currentindex, static_cast<PixelType>(height));
      }
    else
      {
      // Back to the MNT default value
      DEMImage->SetPixel(currentindex, this->m_DefaultUnknownValue);
      }
    progress.CompletedPixel();
    }
}

template <class TDEMImage, class TMapProjection>
void
DEMToOrthoImageGenerator<TDEMImage, TMapProjection>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  if (m_MapProjection.IsNull())
    os << indent << "Map projection: NULL" << std::endl;
  else
    {
    os << indent << "Map projection:" << std::endl;
    m_MapProjection->Print(os, indent.GetNextIndent());
    }
}

} // namespace otb

#endif
