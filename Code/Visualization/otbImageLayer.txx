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
#ifndef __otbImageLayer_txx
#define __otbImageLayer_txx

#include "itkImageRegionConstIterator.h"
#include "otbMacro.h"
#include "otbI18n.h"
#include "itkTimeProbe.h"
#include "otbStandardRenderingFunction.h"

#include "otbImageKeywordlist.h"
#include "otbImageMetadataInterfaceBase.h"
#include "itkMetaDataDictionary.h"
#include "itkPixelBuilder.h"

#include "vnl/vnl_random.h"
namespace otb
{

template <class TImage, class TOutputImage>
ImageLayer<TImage, TOutputImage>
::ImageLayer() : m_Quicklook(), m_Image(), m_ListSample(), m_ListSampleProvided(false), m_RenderingFunction(),
  m_QuicklookRenderingFilter(), m_ExtractRenderingFilter(), m_ScaledExtractRenderingFilter(),
  m_ExtractFilter(), m_ScaledExtractFilter()
{
  // Rendering filters
  m_QuicklookRenderingFilter = RenderingFilterType::New();
  m_ExtractRenderingFilter = RenderingFilterType::New();
  m_ScaledExtractRenderingFilter = RenderingFilterType::New();

  m_ListSample = ListSampleType::New();

  // Default rendering function
  typedef Function::StandardRenderingFunction<PixelType,
      typename TOutputImage::PixelType> DefaultRenderingFunctionType;
  m_RenderingFunction = DefaultRenderingFunctionType::New();
  m_QuicklookRenderingFilter->SetRenderingFunction(m_RenderingFunction);
  m_ExtractRenderingFilter->SetRenderingFunction(m_RenderingFunction);
  m_ScaledExtractRenderingFilter->SetRenderingFunction(m_RenderingFunction);

  // Extract filters
  m_ExtractFilter = ExtractFilterType::New();
  m_ScaledExtractFilter = ExtractFilterType::New();

  // Wiring
  m_ExtractRenderingFilter->SetInput(m_ExtractFilter->GetOutput());
  m_ScaledExtractRenderingFilter->SetInput(m_ScaledExtractFilter->GetOutput());

  m_Transform = TransformType::New();
  m_CoordinateToName = CoordinateToName::New();

  m_PlaceName = "";
  m_CountryName = "";

  m_GroundSpacing = GroundSpacingImageType::New();
  m_ApproxGroundSpacing.Fill(itk::NumericTraits<ValueType>::min());
}

template <class TImage, class TOutputImage>
ImageLayer<TImage, TOutputImage>
::~ImageLayer()
{}

template <class TImage, class TOutputImage>
void
ImageLayer<TImage, TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

template <class TImage, class TOutputImage>
void
ImageLayer<TImage, TOutputImage>
::Render()
{
  // Render the histogram
  this->UpdateListSample();

  // Render images
  this->RenderImages();

  // Initialize the geotransform
  this->InitTransform();

  //Get the ground spacing of the central pixel
  this->ComputeApproximativeGroundSpacing();
}

template <class TImage, class TOutputImage>
void
ImageLayer<TImage, TOutputImage>
::RenderImages()
{
//   if (m_ListSample.IsNull()) //FIXME make sure that it is not necessary
//   {
//     this->UpdateListSample();
//   }
// Render quicklook

  if (this->GetHasQuicklook())
    {
    itk::TimeProbe probe;
    probe.Start();

    // Impacting modified on the the rendering function
    if (m_RenderingFunction->GetMTime() > m_QuicklookRenderingFilter->GetOutput()->GetUpdateMTime())
      {
      m_QuicklookRenderingFilter->Modified();
      }

    m_QuicklookRenderingFilter->Update();
    this->SetRenderedQuicklook(m_QuicklookRenderingFilter->GetOutput());
    probe.Stop();
    otbMsgDevMacro(<< "ImageLayer::RenderImages():" << " (" << this->GetName() << ")"
                   << " quicklook regenerated (" << probe.GetMeanTime() << " s.)");
    }
  // If there are pixels to render
  if (this->GetExtractRegion().GetNumberOfPixels() > 0)
    {
    itk::TimeProbe probe;
    probe.Start();

    // Impacting modified on the the rendering function
    if (m_RenderingFunction->GetMTime() > m_ExtractRenderingFilter->GetOutput()->GetUpdateMTime())
      {
      m_ExtractRenderingFilter->Modified();
      }

    m_ExtractRenderingFilter->GetOutput()->SetRequestedRegion(this->GetExtractRegion());
    m_ExtractRenderingFilter->Update();
    this->SetRenderedExtract(m_ExtractRenderingFilter->GetOutput());
    probe.Stop();
    otbMsgDevMacro(<< "ImageLayer::RenderImages():" << " (" << this->GetName() << ")"
                   << " extract regenerated (" << probe.GetMeanTime() << " s.)");
    this->SetHasExtract(true);
    }
  else
    {
    this->SetHasExtract(false);
    }
  // Render scaled extract
  // If there are pixels to render
  if (this->GetScaledExtractRegion().GetNumberOfPixels() > 0)
    {
    itk::TimeProbe probe;
    probe.Start();
    // Impacting modified on the the rendering function
    if (m_RenderingFunction->GetMTime() > m_ScaledExtractRenderingFilter->GetOutput()->GetUpdateMTime())
      {
      m_ScaledExtractRenderingFilter->Modified();
      }

    m_ScaledExtractRenderingFilter->GetOutput()->SetRequestedRegion(this->GetScaledExtractRegion());
    m_ScaledExtractRenderingFilter->Update();
    this->SetRenderedScaledExtract(m_ScaledExtractRenderingFilter->GetOutput());
    this->SetHasScaledExtract(true);
    probe.Stop();
    otbMsgDevMacro(<< "ImageLayer::RenderImages():" << " (" << this->GetName() << ")"
                   << " scaled extract regenerated (" << probe.GetMeanTime() << " s.)");
    }
  else
    {
    this->SetHasScaledExtract(false);
    }
}

template <class TImage, class TOutputImage>
void
ImageLayer<TImage, TOutputImage>
::UpdateListSample()
{
  if (!m_ListSampleProvided)
    {
    //   otbMsgDevMacro(<<"ImageLayer::UpdateListSample():"<<" ("<<this->GetName()<<")"<< " Entering method");
    // Declare the source of the histogram
    ImagePointerType histogramSource;

    // if there is a quicklook, use it for histogram generation
    if (m_Quicklook.IsNotNull())
      {
      histogramSource = m_Quicklook;
      }
    else
      {
      // Else use the full image (update the data)
      // REVIEW: Not sure the region is right here. Should be the
      // largest ?
      // REPLY: might be... didn't change anything
      //
      histogramSource = m_Image;
      histogramSource->SetRequestedRegion(this->GetExtractRegion());
      }

    // Check if we need to generate the histogram again
    if (m_ListSample.IsNull() || m_ListSample->Size() == 0 ||
        (histogramSource->GetUpdateMTime() < histogramSource->GetPipelineMTime()))
      {
      otbMsgDevMacro(<< "ImageLayer::UpdateListSample():" << " (" << this->GetName() << ")"
                     << " Regenerating histogram due to pippeline update.");

      // Update the histogram source
      histogramSource->Update();

      // Iterate on the image
      itk::ImageRegionConstIterator<ImageType> it(histogramSource, histogramSource->GetBufferedRegion());

      // declare a list to store the samples
      m_ListSample->Clear();

      unsigned int sampleSize = VisualizationPixelTraits::PixelSize(it.Get());
      m_ListSample->SetMeasurementVectorSize(sampleSize);

      // Fill the samples list
      it.GoToBegin();
      while (!it.IsAtEnd())
        {
        SampleType sample(sampleSize);
        VisualizationPixelTraits::Convert(it.Get(), sample);
        m_ListSample->PushBack(sample);
        ++it;
        }
      otbMsgDevMacro(<< "ImageLayer::UpdateListSample()" << " (" << this->GetName() << ")"
                     << " Sample list generated (" << m_ListSample->Size() << " samples, " << sampleSize << " bands)");

      m_RenderingFunction->SetListSample(m_ListSample);
      // Init the rendering function with the input metadata dictionary
      m_RenderingFunction->Initialize(m_Image->GetMetaDataDictionary());
      }
    }
}

template <class TImage, class TOutputImage>
std::string
ImageLayer<TImage, TOutputImage>
::GetPixelDescription(const IndexType& index)
{
  //FIXME only if necessary
  this->UpdateListSample();

  // Ensure rendering function intialization
  m_RenderingFunction->Initialize(m_Image->GetMetaDataDictionary()); //FIXME check, but the call must be done in the generator. To be moved to the layer?
  // The ouptut stringstream
  std::ostringstream oss;
  oss << otbGetTextMacro("Layer") << ": " << this->GetName();
  oss << std::endl << otbGetTextMacro("Image size") << ": " << m_Image->GetLargestPossibleRegion().GetSize() << std::endl;

  // If we are inside the buffered region
  if (m_Image->GetBufferedRegion().IsInside(index))
    {
    oss << m_RenderingFunction->Describe(m_Image->GetPixel(index));
    }
  else if (m_Quicklook.IsNotNull())
  // Else we extrapolate the value from the quicklook
    {
    IndexType ssindex = index;
    ssindex[0] /= this->GetQuicklookSubsamplingRate();
    ssindex[1] /= this->GetQuicklookSubsamplingRate();

    if (m_Quicklook->GetBufferedRegion().IsInside(ssindex))
      {
      oss << " (ql)" << std::endl << m_RenderingFunction->Describe(m_Quicklook->GetPixel(ssindex));
      }
    }
  //here, we consider that if the transform is not ready (call to InitTransform)
  //the user of the class don't want to use it
  if (m_Transform->IsUpToDate())
    {
    if (m_Transform->GetTransformAccuracy() != Projection::UNKNOWN)
      {
      PointType point = this->GetPixelLocation (index);

      oss << setiosflags(std::ios::fixed) << std::setprecision(2) << "Ground spacing(in m): (" << m_ApproxGroundSpacing[0] << ";" << m_ApproxGroundSpacing[1] << ")" <<
      std::endl;

      oss << setiosflags(std::ios::fixed) << std::setprecision(6) << "Lon: " << point[0] << " Lat: " << point[1] << std::endl;

      if (m_Transform->GetTransformAccuracy() == Projection::PRECISE) oss << "(precise location)" << std::endl;
      if (m_Transform->GetTransformAccuracy() == Projection::ESTIMATE) oss << "(estimated location)" << std::endl;

      // We do not want to refresh the location if we are pointing in the scroll view
      if (m_Image->GetBufferedRegion().IsInside(index))
        {
        if (m_CoordinateToName->SetLonLat(point))
          {
          m_CoordinateToName->Evaluate();
          }
        }

      m_PlaceName = m_CoordinateToName->GetPlaceName();
      m_CountryName = m_CoordinateToName->GetCountryName();

      if (m_PlaceName != "") oss << otbGetTextMacro("Near") << " " << m_PlaceName << std::endl;
      if (m_CountryName != "") oss << " " << otbGetTextMacro("in") << " " << m_CountryName << std::endl;
      }
    else
      {
      oss << otbGetTextMacro("Location unknown") << std::endl;
      }
    }

  return oss.str();
}

template <class TImage, class TOutputImage>
typename ImageLayer<TImage, TOutputImage>::PointType
ImageLayer<TImage, TOutputImage>
::GetPixelLocation(const IndexType& index)
{
  PointType inputPoint;
  inputPoint[0] = index[0];
  inputPoint[1] = index[1];
  return m_Transform->TransformPoint(inputPoint);
}

template <class TImage, class TOutputImage>
void
ImageLayer<TImage, TOutputImage>
::InitTransform()
{
  const itk::MetaDataDictionary& inputDict = m_Image->GetMetaDataDictionary();
  m_Transform->SetInputDictionary(inputDict);
  m_Transform->SetInputOrigin(m_Image->GetOrigin());
  m_Transform->SetInputSpacing(m_Image->GetSpacing());
  //  m_Transform->SetDEMDirectory(m_DEMDirectory);
  m_Transform->InstanciateTransform();

  //Set parameters of the ground spacing image calculators
  m_GroundSpacing->SetInputImage(m_Image);
}

template <class TImage, class TOutputImage>
void
ImageLayer<TImage, TOutputImage>
::ComputeApproximativeGroundSpacing()
{
  if (m_Transform->IsUpToDate())
    {
    if (m_Transform->GetTransformAccuracy() != Projection::UNKNOWN)
      {
      IndexType  index;
      vnl_random rand;

      index[0] = static_cast<IndexValueType>(rand.lrand32(0, m_Image->GetLargestPossibleRegion().GetSize()[0]));
      index[1] = static_cast<IndexValueType>(rand.lrand32(0, m_Image->GetLargestPossibleRegion().GetSize()[1]));

      m_ApproxGroundSpacing = m_GroundSpacing->EvaluateAtIndex(index);
      }
    }

}


template <class TImage, class TOutputImage>
typename ImageLayer<TImage, TOutputImage>::LayerValueType
ImageLayer<TImage, TOutputImage>
::GetValueAtIndex(const IndexType& index)
{

  //FIXME only if necessary
  this->UpdateListSample();

  PixelType pixel;
  itk::PixelBuilder<PixelType>::Zero(pixel, (m_Image->GetNumberOfComponentsPerPixel()));

  // Ensure rendering function initialization
  m_RenderingFunction->Initialize(m_Image->GetMetaDataDictionary()); //FIXME check, but the call must be done in the generator. To be moved to the layer?

  // If we are inside the buffered region
  if (m_Image->GetBufferedRegion().IsInside(index))
    {
    pixel = m_Image->GetPixel(index);
    }
  else
    {
    if (m_Quicklook.IsNotNull())
      {
      IndexType indexOrigin = index;
      indexOrigin[0] = 0;
      indexOrigin[1] = 0;

//      sampleSize = VisualizationPixelTraits::PixelSize(m_Quicklook->GetPixel(indexOrigin));
//      returnValue.SetSize(sampleSize);

      IndexType ssindex = index;
      ssindex[0] /= this->GetQuicklookSubsamplingRate();
      ssindex[1] /= this->GetQuicklookSubsamplingRate();

      if (m_Quicklook->GetBufferedRegion().IsInside(ssindex))
        {
        pixel = m_Quicklook->GetPixel(ssindex);
        }
      else
        {
        pixel = m_Quicklook->GetPixel(indexOrigin);
        }
      }
    }

  LayerValueType returnValue = LayerValueGenerator<PixelType, LayerValueType>::Convert(pixel);
  return returnValue;
}

}

#endif
