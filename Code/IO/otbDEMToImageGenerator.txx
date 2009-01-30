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

namespace otb
{


template<class TDEMImage>
DEMToImageGenerator<TDEMImage>
::DEMToImageGenerator()
{
  m_DEMHandler = DEMHandlerType::New();
  m_OutputSpacing[0]=0.0001;
  m_OutputSpacing[1]=-0.0001;
  m_OutputSize[0]=1;
  m_OutputSize[1]=1;
  m_OutputOrigin[0]=0;
  m_OutputOrigin[1]=0;
  m_DefaultUnknownValue = static_cast<PixelType>(-32768); // Value defined in the norm for points strm doesn't have information.
}

template<class TDEMImage>
DEMToImageGenerator<TDEMImage>
::~DEMToImageGenerator()
{
  // Nothing to be done...
}

// DEM folder specification method
template<class TDEMImage>
void
DEMToImageGenerator<TDEMImage>::
SetDEMDirectoryPath(const char* DEMDirectory)
{
  m_DEMHandler->OpenDEMDirectory(DEMDirectory);
}

// GenerateOutputInformation method
template <class TDEMImage>
void DEMToImageGenerator<TDEMImage>
::GenerateOutputInformation()
{
  DEMImageType *output;
  output = this->GetOutput(0);

  IndexType start;
  start[0]=0;
  start[1]=0;

  // Specify region parameters
  OutputImageRegionType largestPossibleRegion;
  largestPossibleRegion.SetSize( m_OutputSize );
  largestPossibleRegion.SetIndex( start );

  output->SetLargestPossibleRegion( largestPossibleRegion );
  output->SetSpacing(m_OutputSpacing);
  output->SetOrigin(m_OutputOrigin);
}

// GenerateData method
template <class TDEMImage>
void
DEMToImageGenerator<TDEMImage>
::GenerateData()
{
  DEMImagePointerType  DEMImage = this->GetOutput();

  // allocate the output buffer
  DEMImage->SetBufferedRegion( DEMImage->GetRequestedRegion() );
  DEMImage->Allocate();
  DEMImage->FillBuffer(0);
  // Create an iterator that will walk the output region
  ImageIteratorType outIt = ImageIteratorType(DEMImage,DEMImage->GetRequestedRegion());

  // Walk the output image, evaluating the height at each pixel
  IndexType       currentindex;
  PointType       phyPoint;
  double      height;

  for (outIt.GoToBegin(); !outIt.IsAtEnd(); ++outIt)
  {
    currentindex=outIt.GetIndex();
    DEMImage->TransformIndexToPhysicalPoint(currentindex, phyPoint);

//       otbMsgDevMacro(<< "PhyPoint : (" << phyPoint[0] << "," << phyPoint[1] << ")");

    height=m_DEMHandler->GetHeightAboveMSL(phyPoint); // Altitude calculation
//       otbMsgDevMacro(<< "height" << height);
    // MNT sets a default value (-32768) at point where it doesn't have altitude information.
    // OSSIM has chosen to change this default value in OSSIM_DBL_NAN (-4.5036e15).
    if (!ossim::isnan(height))
    {
      // Fill the image
      DEMImage->SetPixel(currentindex, static_cast<PixelType>(height) );
    }
    else
    {
      // Back to the MNT default value
      DEMImage->SetPixel(currentindex, m_DefaultUnknownValue);
    }
  }
}

template <class TDEMImage>
void
DEMToImageGenerator<TDEMImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Output Spacing:"<< m_OutputSpacing[0] << ","<< m_OutputSpacing[1] << std::endl;
  os << indent << "Output Origin:"<< m_OutputOrigin[0] << ","<< m_OutputOrigin[1] << std::endl;
  os << indent << "Output Size:"<< m_OutputSize[0] << ","<< m_OutputSize[1] << std::endl;
}

} // namespace otb

#endif
