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
#ifndef __otbDEMToImageGenerator_txx
#define __otbDEMToImageGenerator_txx

#include "otbDEMToImageGenerator.h"
#include "otbMacro.h"
#include "itkProgressReporter.h"

namespace otb
{

template<class TDEMImage>
DEMToImageGenerator<TDEMImage>
::DEMToImageGenerator()
{
  m_OutputSpacing[0] = 0.0001;
  m_OutputSpacing[1] = -0.0001;
  m_OutputSize[0] = 1;
  m_OutputSize[1] = 1;
  m_OutputOrigin[0] = 0;
  m_OutputOrigin[1] = 0;

  m_Transform         = GenericRSTransformType::New();
  m_DEMFunction       = SRTMFunctionType::New();

}

// DEM folder specification method
template<class TDEMImage>
void
DEMToImageGenerator<TDEMImage>::
SetDEMDirectoryPath(const char* DEMDirectory)
{
  typename SRTMFunctionType::Pointer  srtmFunction = SRTMFunctionType::New();
  srtmFunction->OpenDEMDirectory(DEMDirectory);
  this->SetDEMFunction( srtmFunction.GetPointer() );
}


template<class TDEMImage>
void
DEMToImageGenerator<TDEMImage>::
SetDEMDirectoryPath(const std::string& DEMDirectory)
{
  this->SetDEMDirectoryPath(DEMDirectory.c_str());
}

// GenerateOutputInformation method
template <class TDEMImage>
void DEMToImageGenerator<TDEMImage>
::GenerateOutputInformation()
{
  DEMImageType *output;
  output = this->GetOutput(0);

  IndexType start;
  start[0] = 0;
  start[1] = 0;

  // Specify region parameters
  OutputImageRegionType largestPossibleRegion;
  largestPossibleRegion.SetSize(m_OutputSize);
  largestPossibleRegion.SetIndex(start);

  output->SetLargestPossibleRegion(largestPossibleRegion);
  output->SetSpacing(m_OutputSpacing);
  output->SetOrigin(m_OutputOrigin);
}


template <class TDEMImage>
void DEMToImageGenerator<TDEMImage>
::BeforeThreadedGenerateData()
{
  m_Transform->InstanciateTransform();
  DEMImagePointerType DEMImage = this->GetOutput();

  // allocate the output buffer
  DEMImage->SetBufferedRegion(DEMImage->GetRequestedRegion());
  DEMImage->Allocate();
  DEMImage->FillBuffer(0);
}


template <class TDEMImage>
void
DEMToImageGenerator<TDEMImage>
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
  PointType phyPoint;
  PointType geoPoint;

  for (outIt.GoToBegin(); !outIt.IsAtEnd(); ++outIt)
    {
    currentindex = outIt.GetIndex();
    DEMImage->TransformIndexToPhysicalPoint(currentindex, phyPoint);

    if(m_Transform.IsNotNull())
      {
        geoPoint = m_Transform->TransformPoint(phyPoint);
        DEMImage->SetPixel(currentindex, m_DEMFunction->Evaluate(geoPoint) );
      }
    else
      {
        DEMImage->SetPixel(currentindex, m_DEMFunction->Evaluate(phyPoint) );
      }
    progress.CompletedPixel();
    }
}

template <class TDEMImage>
void
DEMToImageGenerator<TDEMImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Output Spacing:" << m_OutputSpacing[0] << "," << m_OutputSpacing[1] << std::endl;
  os << indent << "Output Origin:" << m_OutputOrigin[0] << "," << m_OutputOrigin[1] << std::endl;
  os << indent << "Output Size:" << m_OutputSize[0] << "," << m_OutputSize[1] << std::endl;
}

} // namespace otb

#endif
