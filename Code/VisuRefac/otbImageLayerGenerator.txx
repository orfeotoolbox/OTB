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
#ifndef __otbImageLayerGenerator_txx
#define __otbImageLayerGenerator_txx

#include "otbImageLayerGenerator.h"

#include <FL/Fl.H>

namespace otb
{

template < class TImageLayer >
ImageLayerGenerator<TImageLayer>
::ImageLayerGenerator() : m_Layer(), m_Image(), m_Quicklook(),
			  m_SubsamplingRate(1), m_GenerateQuicklook(true), 
			  m_Resampler(), m_ScreenRatio(0.25)
{
  // Intialize output layer
  m_Layer = ImageLayerType::New();
  // Resampler
  m_Resampler = ResampleFilterType::New();
}

template < class TImageLayer >
ImageLayerGenerator<TImageLayer>
::~ImageLayerGenerator()
{}

template < class TImageLayer >
void
ImageLayerGenerator<TImageLayer>
::GenerateLayer()
{
// Check if there is an input image
  if(m_Image.IsNull())
    {
    return;
    }

  // Update image information
  m_Image->UpdateOutputInformation();

  // Build a new layer
  m_Layer = ImageLayerType::New();

  // Generate layer information
  this->GenerateLayerInformation();

  // If we need to generate the quicklook
  if(m_GenerateQuicklook)
    {
    // Generate it
    this->GenerateQuicklook();
    }
}

template < class TImageLayer >
unsigned int
ImageLayerGenerator<TImageLayer>
::GetOptimalSubSamplingRate()
{
  // Check if there is an input image
  if(m_Image.IsNull())
    {
    return 1;
    }
  // The optimal rate will be computed according to the screen size.
  // We want a quality quicklook up to a quarter of the screen.
  // Get the screen size
  unsigned int wscreen = static_cast<unsigned int>(Fl::w());
  unsigned int hscreen = static_cast<unsigned int>(Fl::h());

  // Update image information
  m_Image->UpdateOutputInformation();

  // Get the image largest possible region
  typename ImageType::RegionType largestRegion = m_Image->GetLargestPossibleRegion();

  // Shannon (finner generation could be added later)
  unsigned int wrequested = static_cast<unsigned int>(wscreen*m_ScreenRatio);
  unsigned int hrequested = static_cast<unsigned int>(hscreen*m_ScreenRatio);

  // Compute ratio in both directions
  unsigned int wratio = m_Image->GetLargestPossibleRegion().GetSize()[0]/wrequested;
  unsigned int hratio = m_Image->GetLargestPossibleRegion().GetSize()[1]/hrequested;

  // find max ratio
  unsigned int ratio = std::min(wratio,hratio);

  // Ensure a non null ratio
  if(ratio == 0)
    {
    ratio = 1;
    }
  // return the ratio
  return ratio;
}
template < class TImageLayer >
void
ImageLayerGenerator<TImageLayer>
::GenerateLayerInformation()
{
  // Set the layer extent
  m_Layer->SetExtent(m_Image->GetLargestPossibleRegion());
  m_Layer->SetImage(m_Image);
  m_Layer->SetHasExtract(true);
  m_Layer->SetHasScaledExtract(true);
  m_Layer->VisibleOn();
}

template < class TImageLayer >
void
ImageLayerGenerator<TImageLayer>
::GenerateQuicklook()
{
  if(m_GenerateQuicklook)
    {
    // Compute optimal subsampling rate
    unsigned int ssrate = this->GetOptimalSubSamplingRate();

    // If no subsampling is needed
    if(ssrate == 1)
      {
      otbMsgDevMacro(<<"ImageLayerGenerator::GenerateQuicklook(): subsampling rate is 1, Image itself is used as quicklook");
      m_Layer->SetHasQuicklook(true);
      m_Layer->SetQuicklookSubsamplingRate(1);
      m_Image->Update();
      m_Layer->SetQuicklook(m_Image);
      }
    else
      {
      // build the quicklook
      m_Layer->SetQuicklookSubsamplingRate(ssrate);
      m_Resampler->SetInput(m_Image);
      m_Resampler->SetShrinkFactor(ssrate);
      m_Resampler->Update();
      otbMsgDevMacro(<<"ImageLayerGenerator::GenerateQuicklook(): Quicklook generated (ssrate= "<<ssrate<<", size= "<<m_Resampler->GetOutput()->GetLargestPossibleRegion().GetSize()<<")");

      // Set the quicklook to the layer
      m_Layer->SetQuicklook(m_Resampler->GetOutput());
      m_Layer->SetHasQuicklook(true);
      // Disconnect pipeline and prepare a new resampler
      m_Resampler = ResampleFilterType::New();
      }
    }
  else
    {
    // If there is a quicklook
    if(m_Quicklook.IsNotNull())
      {
      // Set it to the layer
      m_Layer->SetQuicklook(m_Quicklook);
      m_Layer->SetQuicklookSubsamplingRate(m_SubsamplingRate);
      m_Layer->SetHasQuicklook(true);
      }
    else
      {
      // else there is no quicklook, disable it
      m_Layer->SetHasQuicklook(false);
      }
    }
}

template < class TImageLayer >
void
ImageLayerGenerator<TImageLayer>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}
} // end namespace otb

#endif


