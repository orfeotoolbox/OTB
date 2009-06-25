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
#include "otbImageMetadataInterface.h"

#include "otbRenderingFunction.h"
//FIXME just for the enum declaration, might need to move that

#include <FL/Fl.H>

namespace otb
{

template < class TImageLayer >
ImageLayerGenerator<TImageLayer>
::ImageLayerGenerator() : m_Layer(), /*m_RenderingFunction(),*/ m_Image(), m_Quicklook(),
                          m_SubsamplingRate(1), m_GenerateQuicklook(true),
                          m_Resampler(), m_ScreenRatio(0.25)
{
  // Intialize output layer
  m_Layer = ImageLayerType::New();
  // Resampler
  m_Resampler = ResampleFilterType::New();
  // Rendering function
//   m_RenderingFunction = DefaultRenderingFunctionType::New();//Note: created in the layer by default
  // Default blending function
  //m_BlendingFunction = m_Layer->GetBlendingFunction();
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

  // Generate layer information
  this->GenerateLayerInformation();

  // Generate the quicklook
  this->GenerateQuicklook();
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

  // Setup channels
//   switch(m_Image->GetNumberOfComponentsPerPixel())
//   switch( PixelSize(m_Image, m_Image->GetBufferPointer()) )
//   {
//     case 1:
//     {
//       m_RenderingFunction->Initialize(SCALAR);
//       break;
//     }
//     case 2:
//     {
//       m_RenderingFunction->Initialize(TWOBANDS);
//       break;
//     }
//     case 3:
//     {
//       m_RenderingFunction->Initialize(THREEBANDS);
//       break;
//     }
//     case 4:
//     {
//       // Get the sensor ID
//       ImageMetadataInterface::Pointer imageMetadataInterface= ImageMetadataInterface::New();
//       std::string sensorID = imageMetadataInterface->GetSensorID(m_Image->GetMetaDataDictionary());
//       if (sensorID.find("Spot") != std::string::npos)
//       {
//         m_RenderingFunction->Initialize(SENSORINVERTED);
//       }
//       else
//       {
//         m_RenderingFunction->Initialize(SENSORWAVELENTHORDER);
//       }
//       break;
//     }
//     default:
//     {
//       //Discard
//       break;
//     }
//   }

//   // Set the rendering function
//   if (m_RenderingFunction.IsNotNull())
//   {
//     otbMsgDevMacro(<<"ImageLayerGenerator::GenerateLayerInformation(): set the rendering function of the layer");
//     m_Layer->SetRenderingFunction(m_RenderingFunction);
//   }
//   else
//   {
//     otbMsgDevMacro(<<"ImageLayerGenerator::GenerateLayerInformation(): keep the default rendering function of the layer");
//   }
//
//   //Set the blending function
//   m_Layer->SetBlendingFunction(m_BlendingFunction);

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


//Find out the histogram size from the pixel
//FIXME duplication in ImageLayer
template < class TImageLayer >
    unsigned int
    ImageLayerGenerator<TImageLayer>
  ::PixelSize(ImagePointerType image, ScalarType* v) const
{
  return image->GetNumberOfComponentsPerPixel();
}
//Match is done according to InternalPixelType which is scalar also for VectorImage
// template < class TImageLayer >
//     unsigned int
//     ImageLayerGenerator<TImageLayer>
//   ::PixelSize(ImagePointerType image, VectorPixelType* v) const
// {
//   std::cout << "Match on vector" << std::endl;
//   return image->GetNumberOfComponentsPerPixel();
// }
template < class TImageLayer >
    unsigned int
    ImageLayerGenerator<TImageLayer>
  ::PixelSize(ImagePointerType image, RGBPixelType* v) const
{
  return 3;
}
template < class TImageLayer >
    unsigned int
    ImageLayerGenerator<TImageLayer>
  ::PixelSize(ImagePointerType image, RGBAPixelType* v) const
{
  return 3;//We don't really want to normalize the Alpha value
}


} // end namespace otb

#endif


