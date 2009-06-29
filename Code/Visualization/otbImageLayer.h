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
#ifndef __otbImageLayer_h
#define __otbImageLayer_h

#include "otbImageLayerBase.h"
#include "itkVariableLengthVector.h"
#include "itkDenseFrequencyContainer.h"

#include "itkExtractImageFilter.h"
#include "itkListSample.h"
#include "otbListSampleToHistogramListGenerator.h"

#include "otbRenderingImageFilter.h"

namespace otb
{
/** \class ImageLayer
*   \brief This class is a layer container.
*   It contains everything related to a layer in the viewer model.
*
*   \sa ImageViewerModel
*   \sa Layer
*  \ingroup Visualization
 */

template <class TImage, class TOutputImage = otb::Image<itk::RGBAPixel<unsigned char>, 2 > >
class ImageLayer
  : public ImageLayerBase<TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef ImageLayer                        Self;
  typedef ImageLayerBase<TOutputImage>      Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(ImageLayer,ImageLayerBase);

  /** Image typedef */
  typedef TImage                                                      ImageType;
  typedef typename ImageType::Pointer                                 ImagePointerType;
  typedef typename ImageType::PixelType                               PixelType;
  typedef typename itk::NumericTraits<PixelType>::ValueType           ScalarType;
  typedef itk::VariableLengthVector<ScalarType>                       VectorPixelType;
  typedef itk::RGBPixel<ScalarType>                                   RGBPixelType;
  typedef itk::RGBAPixel<ScalarType>                                  RGBAPixelType;
  typedef typename ImageType::RegionType                              RegionType;
  typedef typename ImageType::IndexType                               IndexType;

  /** Output image typedef */
  typedef TOutputImage                                                OutputImageType;
  typedef typename OutputImageType::PixelType                         OutputPixelType;


  /** Histogram typedef */

  typedef itk::VariableLengthVector<ScalarType>                       SampleType;
  typedef itk::Statistics::ListSample<SampleType>                     ListSampleType;
  typedef typename ListSampleType::Pointer                       ListSamplePointerType;

  typedef itk::Statistics::DenseFrequencyContainer                   DFContainerType;
  typedef itk::Statistics::Histogram<
                  typename itk::NumericTraits<ScalarType>::RealType,1,
                  DFContainerType> HistogramType;
  typedef typename HistogramType::Pointer                             HistogramPointerType;
  typedef ObjectList<HistogramType>                                   HistogramListType;
  typedef typename HistogramListType::Pointer                         HistogramListPointerType;

  /** Rendering part */
  typedef RenderingImageFilter<TImage,TOutputImage>                   RenderingFilterType;
  typedef typename RenderingFilterType::Pointer                       RenderingFilterPointerType;
  typedef typename RenderingFilterType::RenderingFunctionType         RenderingFunctionType;
  typedef typename RenderingFunctionType::Pointer                     RenderingFunctionPointerType;
  typedef itk::ExtractImageFilter<ImageType,ImageType>                ExtractFilterType;
  typedef typename ExtractFilterType::Pointer                         ExtractFilterPointerType;

  /** Set/Get the image */
  void SetImage(ImageType * img)
  {
    if(m_Image != img)
      {
      m_Image = img;
      m_ExtractFilter->SetInput(m_Image);
      m_ScaledExtractFilter->SetInput(m_Image);
      }
  }
  itkGetObjectMacro(Image,ImageType);

  /** Set/Get the quicklook */
  void SetQuicklook(ImageType * ql)
  {
    if(m_Quicklook != ql)
      {
      m_Quicklook = ql;
      m_QuicklookRenderingFilter->SetInput(m_Quicklook);
      }

  }
  itkGetObjectMacro(Quicklook,ImageType);

  /** Get the histogram list */
  HistogramListPointerType GetHistogramList()
  {
    //FIXME Update condition?
    return m_RenderingFunction->GetHistogramList();
  }

  /** Set/Get the rendering function */
  void SetRenderingFunction(RenderingFunctionType * function)
  {
    m_RenderingFunction = function;
    m_RenderingFunction->SetListSample(this->GetListSample());
    m_QuicklookRenderingFilter->SetRenderingFunction(m_RenderingFunction);
    m_ExtractRenderingFilter->SetRenderingFunction(m_RenderingFunction);
    m_ScaledExtractRenderingFilter->SetRenderingFunction(m_RenderingFunction);
  }
  itkGetObjectMacro(RenderingFunction,RenderingFunctionType);

  /** Reimplemented to pass the parameter to the extract filter */
  virtual void SetExtractRegion(const RegionType & region)
  {
    // This check should be done in the itk::ExtractImageFilter
    if(this->GetExtractRegion() != region)
      {
      Superclass::SetExtractRegion(region);
      // SetExtractionRegion throws an exception in case of empty region
      if(region.GetNumberOfPixels() > 0)
        {
        m_ExtractFilter->SetExtractionRegion(region);
        }
      }
  }

 /** Reimplemented to pass the parameter to the extract filter */
  virtual void SetScaledExtractRegion(const RegionType & region)
  {
    // This check should be done in the itk::ExtractImageFilter
    if(this->GetScaledExtractRegion() != region)
      {
      Superclass::SetScaledExtractRegion(region);
      // SetExtractionRegion throws an exception in case of empty region
      if(region.GetNumberOfPixels() > 0)
        {
        m_ScaledExtractFilter->SetExtractionRegion(region);
        }
      }
  }

  /** Actually render the image */
  virtual void Render();

  /** Get the pixel description */
  virtual std::string GetPixelDescription(const IndexType & index);

protected:
  /** Constructor */
  ImageLayer();
  /** Destructor */
  ~ImageLayer();
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Update the histogram */
  virtual void UpdateListSample();

  virtual ListSamplePointerType GetListSample()
  {
//     this->UpdateListSample();//FIXME condition to IsModified
    return m_ListSample;
  }

  /** Update the images */
  virtual void RenderImages();

  /** Find out the histogram size from the pixel */
  unsigned int PixelSize(ImagePointerType image, ScalarType* v) const;
  unsigned int PixelSize(ImagePointerType image, VectorPixelType* v) const;
  unsigned int PixelSize(ImagePointerType image, RGBPixelType* v) const;
  unsigned int PixelSize(ImagePointerType image, RGBAPixelType* v) const;


private:
  ImageLayer(const Self&);     // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  /** Pointer to the quicklook */
  ImagePointerType             m_Quicklook;

  /** Pointer to the image */
  ImagePointerType             m_Image;

  /** List sample used to compute the histogram by the rendering function*/
  ListSamplePointerType m_ListSample;

  /** Rendering function */
  RenderingFunctionPointerType m_RenderingFunction;

  /** Rendering filters */
  RenderingFilterPointerType  m_QuicklookRenderingFilter;
  RenderingFilterPointerType  m_ExtractRenderingFilter;
  RenderingFilterPointerType  m_ScaledExtractRenderingFilter;

  /** Extract filters */
  ExtractFilterPointerType    m_ExtractFilter;
  ExtractFilterPointerType    m_ScaledExtractFilter;

}; // end class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageLayer.txx"
#endif

#endif
