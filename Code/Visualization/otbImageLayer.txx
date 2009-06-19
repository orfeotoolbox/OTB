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
#include "itkTimeProbe.h"

namespace otb
{

template <class TImage, class TOutputImage>
ImageLayer<TImage,TOutputImage>
::ImageLayer() : m_Quicklook(), m_Image(), m_HistogramList(), m_RenderingFunction(),
                 m_NumberOfHistogramBins(255),
                 m_QuicklookRenderingFilter(), m_ExtractRenderingFilter(), m_ScaledExtractRenderingFilter(),
                 m_ExtractFilter(), m_ScaledExtractFilter()
{
 // Rendering filters
  m_QuicklookRenderingFilter = RenderingFilterType::New();
  m_ExtractRenderingFilter = RenderingFilterType::New();
  m_ScaledExtractRenderingFilter = RenderingFilterType::New();

  // Default rendering function
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
}

template <class TImage, class TOutputImage>
ImageLayer<TImage,TOutputImage>
::~ImageLayer()
{}


template <class TImage, class TOutputImage>
void
ImageLayer<TImage,TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

template <class TImage, class TOutputImage>
void
ImageLayer<TImage,TOutputImage>
::Render()
{
  // Render the histogram
  this->RenderHistogram();

  // If required, use histogram for auto min/max
//   if(m_AutoMinMax)
//     {
//     this->AutoMinMaxRenderingFunctionSetup();
//     }

  // Render images
  this->RenderImages();
}

template <class TImage, class TOutputImage>
void
ImageLayer<TImage,TOutputImage>
::RenderImages()
{
  // Render quicklook
  if(this->GetHasQuicklook())
    {
    itk::TimeProbe probe;
    probe.Start();
    m_QuicklookRenderingFilter->Update();
    this->SetRenderedQuicklook(m_QuicklookRenderingFilter->GetOutput());
    probe.Stop();
    otbMsgDevMacro(<<"ImageLayer::RenderImages():"<<" ("<<this->GetName()<<")"<< " quicklook regenerated ("<<probe.GetMeanTime()<<" s.)");
    }
  // If there are pixels to render
  if(this->GetExtractRegion().GetNumberOfPixels() > 0)
    {
    itk::TimeProbe probe;
    probe.Start();
    //std::cout<<"Extent: "<<this->GetExtent()<<" Largest: "<<m_Image->GetLargestPossibleRegion()<<" ExtractRegion: "<<this->GetExtractRegion()<<std::endl;
    m_ExtractRenderingFilter->GetOutput()->SetRequestedRegion(this->GetExtractRegion());
    m_ExtractRenderingFilter->Update();
    this->SetRenderedExtract(m_ExtractRenderingFilter->GetOutput());
    probe.Stop();
    otbMsgDevMacro(<<"ImageLayer::RenderImages():"<<" ("<<this->GetName()<<")"<< " extract regenerated ("<<probe.GetMeanTime()<<" s.)");
    this->SetHasExtract(true);
    }
  else
    {
    this->SetHasExtract(false);
    }
  // Render scaled extract
  // If there are pixels to render
  if(this->GetScaledExtractRegion().GetNumberOfPixels() > 0)
      {
      itk::TimeProbe probe;
      probe.Start();
      m_ScaledExtractRenderingFilter->GetOutput()->SetRequestedRegion(this->GetScaledExtractRegion());
      m_ScaledExtractRenderingFilter->Update();
      this->SetRenderedScaledExtract(m_ScaledExtractRenderingFilter->GetOutput());
      this->SetHasScaledExtract(true);
      probe.Stop();
      otbMsgDevMacro(<<"ImageLayer::RenderImages():"<<" ("<<this->GetName()<<")"<< " scaled extract regenerated ("<<probe.GetMeanTime()<<" s.)");
      }
  else
    {
    this->SetHasScaledExtract(false);
    }
}

template <class TImage, class TOutputImage>
void
ImageLayer<TImage,TOutputImage>
::RenderHistogram()
{
  // Declare the source of the histogram
  ImagePointerType histogramSource;

  // if there is a quicklook, use it for histogram generation
  if(m_Quicklook.IsNotNull())
    {
    histogramSource = m_Quicklook;
    }
  else
    {
    // Else use the full image (update the data)
    histogramSource = m_Image;
    histogramSource->SetRequestedRegion(this->GetExtractRegion());
    }

  // Check if we need to generate the histogram again
  if( !m_HistogramList || (histogramSource->GetUpdateMTime() < histogramSource->GetPipelineMTime()) )
    {
    otbMsgDevMacro(<<"ImageLayer::RenderHistogram():"<<" ("<<this->GetName()<<")"<< " Regenerating histogram due to pippeline update.");
//     m_AutoMinMaxUpToDate = false;

    // Update the histogram source
    histogramSource->Update();

    // Iterate on the image
    itk::ImageRegionConstIterator<ImageType> it(histogramSource,histogramSource->GetBufferedRegion());

    // declare a list to store the samples
    typename ListSampleType::Pointer listSample = ListSampleType::New();

    // Set the measurement vector size
//     listSample->SetMeasurementVectorSize(histogramSource->GetNumberOfComponentsPerPixel());

    //Note: The GetBufferPointer is just used for overloading resolution
    listSample->SetMeasurementVectorSize(PixelSize(histogramSource, histogramSource->GetBufferPointer()));
//      listSample->SetMeasurementVectorSize(m_RenderingFunction->GetPixelRepresentationSize());

    // Fill the samples list
    it.GoToBegin();
    while(!it.IsAtEnd())
      {
//       SampleType sample(histogramSource->GetNumberOfComponentsPerPixel());
      SampleType sample(PixelSize(histogramSource, histogramSource->GetBufferPointer()));
//       SampleType sample(m_RenderingFunction->GetPixelRepresentationSize());
      // workaround to handle both scalar and vector pixels the same way
      sample.Fill(itk::NumericTraits<ScalarType>::Zero);
      sample = sample +  it.Get();
      listSample->PushBack(sample);
      ++it;
      }
      otbMsgDevMacro(<<"ImageLayer::RenderHistogram()"<<" ("<<this->GetName()<<")"<< " Sample list generated ("<<listSample->Size()<<" samples, "<< PixelSize(histogramSource, histogramSource->GetBufferPointer())<<" bands)");

      m_RenderingFunction->SetListSample(listSample);

//        otbMsgDevMacro(<<"ImageLayer::RenderHistogram()"<<" ("<<this->GetName()<<")"<< " Sample list generated ("<<listSample->Size()<<" samples, "<< m_RenderingFunction->GetPixelRepresentationSize() <<" bands)");

//     // Create the histogram generation filter
//     typename HistogramFilterType::Pointer histogramFilter = HistogramFilterType::New();
//     histogramFilter->SetListSample(listSample);
//
//     histogramFilter->SetNumberOfBins(m_NumberOfHistogramBins);
//
//     // Generate
//     histogramFilter->Update();
//     otbMsgDevMacro(<<"ImageLayer::RenderHistogram()"<<" ("<<this->GetName()<<")"<< " Histogram has been updated");
//
//     // Retrieve the histogram
//     this->SetHistogramList(histogramFilter->GetOutput());
    }
}

// template <class TImage, class TOutputImage>
// void
// ImageLayer<TImage,TOutputImage>
// ::AutoMinMaxRenderingFunctionSetup()
// {
//   if(!m_AutoMinMaxUpToDate)
//     {
//     // Check for an existing histogram
//     if(m_HistogramList.IsNull())
//       {
//       itkExceptionMacro(<<"Empty histogram, can not use auto min/max evaluation.");
//       }

//     otbMsgDevMacro(<<"ImageLayer::AutoMinMaxRenderingFunctionSetup():"<<" ("<<this->GetName()<<")"<< " Updating min/max from histogram");
//
//     const unsigned int nbComps = m_Image->GetNumberOfComponentsPerPixel();
//     const unsigned int nbComps = PixelSize(m_Image, m_Image->GetBufferPointer());
//     typename RenderingFunctionType::ExtremaVectorType min, max;
//     otbMsgDevMacro(<<"ImageLayer::AutoMinMaxRenderingFunctionSetup(): "<<" ("<<this->GetName()<<") "<<nbComps<<" components, quantile= "<<100*m_AutoMinMaxQuantile<<" %");
//     // For each components, use the histogram to compute min and max
//     typedef typename RenderingFunctionType::ScalarType  RenderingFunctionScalarPixelType;
//     for(unsigned int comp = 0; comp < nbComps;++comp)
//       {
//       // Compute quantiles
//       min.push_back(static_cast<RenderingFunctionScalarPixelType>( m_HistogramList->GetNthElement(comp)->Quantile(0,m_AutoMinMaxQuantile)));
//       max.push_back(static_cast<RenderingFunctionScalarPixelType>(m_HistogramList->GetNthElement(comp)->Quantile(0,1-m_AutoMinMaxQuantile)));
//       otbMsgDevMacro(<<"ImageLayer::AutoMinMaxRenderingFunctionSetup():"<<" ("<<this->GetName()<<")"<< " component "<<comp
//           <<", min= "<< static_cast< typename itk::NumericTraits<typename RenderingFunctionType::ScalarType >::PrintType>(min.back())
//           <<", max= "<<static_cast< typename itk::NumericTraits<typename RenderingFunctionType::ScalarType >::PrintType>(max.back()));
//       }

//     Setup rendering function
//     m_RenderingFunction->SetMinimum(min);
//     m_RenderingFunction->SetMaximum(max);

//     m_QuicklookRenderingFilter->Modified();
//     m_ExtractRenderingFilter->Modified();
//     m_ScaledExtractRenderingFilter->Modified();
//     m_AutoMinMaxUpToDate = true;
//     }
// }

template <class TImage, class TOutputImage>
std::string
ImageLayer<TImage,TOutputImage>
::GetPixelDescription(const IndexType & index)
{
  // If required, use histogram for auto min/max
//   if(m_AutoMinMax)
//     {
    this->RenderHistogram();
//     this->AutoMinMaxRenderingFunctionSetup();
//     }
  // Ensure rendering function intialization
  m_RenderingFunction->Initialize(); //FIXME check, but the call must be done in the generator. To be moved to the layer?
  // The ouptut stringstream
  itk::OStringStream oss;
  oss<<"Layer: "<<this->GetName();
  // If we are inside the buffered region
  if(m_Image->GetBufferedRegion().IsInside(index))
    {
    oss<<std::endl<<m_RenderingFunction->Describe(m_Image->GetPixel(index));
    }
  else if(m_Quicklook.IsNotNull())
    // Else we extrapolate the value from the quicklook
    {
    IndexType ssindex = index;
    ssindex[0]/=this->GetQuicklookSubsamplingRate();
    ssindex[1]/=this->GetQuicklookSubsamplingRate();

    if(m_Quicklook->GetBufferedRegion().IsInside(ssindex))
      {
      oss<<" (ql)"<<std::endl<<m_RenderingFunction->Describe(m_Quicklook->GetPixel(ssindex));
      }
    }
  return oss.str();
}


//Find out the histogram size from the pixel
//FIXME duplication in ImageLayerGenerator
template <class TImage, class TOutputImage>
    unsigned int
        ImageLayer<TImage,TOutputImage>
  ::PixelSize(ImagePointerType image, ScalarType* v) const
{
  return image->GetNumberOfComponentsPerPixel();
}
//Match is done according to InternalPixelType which is scalar also for VectorImage
// template <class TImage, class TOutputImage>
//     unsigned int
//         ImageLayer<TImage,TOutputImage>
//   ::PixelSize(ImagePointerType image, VectorPixelType* v) const
// {
//   return image->GetNumberOfComponentsPerPixel();
// }
template <class TImage, class TOutputImage>
    unsigned int
        ImageLayer<TImage,TOutputImage>
  ::PixelSize(ImagePointerType image, RGBPixelType* v) const
{
  return 3;
}
template <class TImage, class TOutputImage>
    unsigned int
        ImageLayer<TImage,TOutputImage>
  ::PixelSize(ImagePointerType image, RGBAPixelType* v) const
{
  return 3;//We don't really want to normalize the Alpha value
}

}


//This is needed to create the histogram as a VariableLengthVector
//from a RGBPixel or RGBAPixel image.
namespace itk
{
 template< class T > inline VariableLengthVector<T> operator+
     ( const VariableLengthVector<T> &lhs, const RGBPixel< T > &rhs )
{
//   if( lhs.Size() != rhs.Size() )
//   {
//   itkGenericExceptionMacro( << "Cannot add VariableLengthVector of length "
//       <<  lhs.Size() << " and " << rhs.Size() );
//   }
  VariableLengthVector<T> out(lhs);
  for( typename VariableLengthVector<T>::ElementIdentifier i=0; i< rhs.Size(); i++ )
  {
    out[i] += rhs[i];
  }
  return out;
}
 template< class T > inline VariableLengthVector<T> operator+
     ( const VariableLengthVector<T> &lhs, const RGBAPixel< T > &rhs )
{
//   if( lhs.Size() != rhs.Size() )
//   {
//     itkGenericExceptionMacro( << "Cannot add VariableLengthVector of length "
//         <<  lhs.Size() << " and " << rhs.Size() );
//   }
  VariableLengthVector<T> out(lhs);
  for( typename VariableLengthVector<T>::ElementIdentifier i=0; i< rhs.Size(); i++ )
  {
    out[i] += rhs[i];
  }
  return out;
}

}

#endif
