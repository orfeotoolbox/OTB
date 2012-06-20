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
#include "otbGenericRSTransform.h"

#include "otbCoordinateToName.h"

#include "otbGroundSpacingImageFunction.h"

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

template <class TImage, class TOutputImage>
class ImageLayer
  : public ImageLayerBase<TOutputImage>
{
public:
  /** Standard class typedefs */
  typedef ImageLayer                    Self;
  typedef ImageLayerBase<TOutputImage>  Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(ImageLayer, ImageLayerBase);

  /** Image typedef */
  typedef TImage                                            ImageType;
  typedef typename ImageType::Pointer                       ImagePointerType;
  typedef typename ImageType::PixelType                     PixelType;
  typedef typename itk::NumericTraits<PixelType>::ValueType ScalarType;
  typedef itk::VariableLengthVector<ScalarType>             VectorPixelType;
  typedef itk::RGBPixel<ScalarType>                         RGBPixelType;
  typedef itk::RGBAPixel<ScalarType>                        RGBAPixelType;
  typedef typename ImageType::RegionType                    RegionType;
  typedef typename ImageType::IndexType                     IndexType;
  typedef typename IndexType::IndexValueType                IndexValueType;

  typedef itk::Point<double, 2>                      PointType;
  typedef otb::GenericRSTransform<double>            TransformType;
  typedef otb::GroundSpacingImageFunction<ImageType> GroundSpacingImageType;
  typedef typename GroundSpacingImageType::FloatType FloatType;
  typedef typename GroundSpacingImageType::ValueType ValueType;
  /** Output image typedef */
  typedef TOutputImage                        OutputImageType;
  typedef typename OutputImageType::PixelType OutputPixelType;

  /** Histogram typedef */

  typedef itk::VariableLengthVector<ScalarType>   SampleType;
  typedef itk::Statistics::ListSample<SampleType> ListSampleType;
  typedef typename ListSampleType::Pointer        ListSamplePointerType;

  typedef itk::Statistics::DenseFrequencyContainer DFContainerType;
  typedef itk::Statistics::Histogram<
      typename itk::NumericTraits<ScalarType>::RealType, 1,
      DFContainerType>                                    HistogramType;
  typedef typename HistogramType::Pointer     HistogramPointerType;
  typedef ObjectList<HistogramType>           HistogramListType;
  typedef typename HistogramListType::Pointer HistogramListPointerType;

  typedef typename Superclass::LayerValueType LayerValueType;

  /** Rendering part */
  typedef RenderingImageFilter<TImage, TOutputImage>          RenderingFilterType;
  typedef typename RenderingFilterType::Pointer               RenderingFilterPointerType;
  typedef typename RenderingFilterType::RenderingFunctionType RenderingFunctionType;
  typedef typename RenderingFunctionType::Pointer             RenderingFunctionPointerType;
  typedef itk::ExtractImageFilter<ImageType, ImageType>       ExtractFilterType;
  typedef typename ExtractFilterType::Pointer                 ExtractFilterPointerType;

  /** Set/Get the image */
  virtual void SetImage(ImageType * img)
  {
    if (this->m_Image != img)
      {
      this->m_Image = img;
      this->m_ExtractFilter->SetInput(m_Image);
      this->m_ScaledExtractFilter->SetInput(m_Image);
      }
  }
  itkGetObjectMacro(Image, ImageType);

  /** Set/Get the quicklook */
  virtual void SetQuicklook(ImageType * ql)
  {
    if (this->m_Quicklook != ql)
      {
      this->m_Quicklook = ql;
      this->m_QuicklookRenderingFilter->SetInput(m_Quicklook);
      }

  }
  itkGetObjectMacro(Quicklook, ImageType);

  /** Get the histogram list */
  virtual HistogramListPointerType GetHistogramList()
  {
    //FIXME Update condition?
    return m_RenderingFunction->GetHistogramList();
  }

  /** Set/Get the rendering function */
  virtual void SetRenderingFunction(RenderingFunctionType * function)
  {
    m_RenderingFunction = function;
    m_RenderingFunction->SetListSample(this->GetListSample());
    m_QuicklookRenderingFilter->SetRenderingFunction(m_RenderingFunction);
    m_ExtractRenderingFilter->SetRenderingFunction(m_RenderingFunction);
    m_ScaledExtractRenderingFilter->SetRenderingFunction(m_RenderingFunction);
  }
  itkGetObjectMacro(RenderingFunction, RenderingFunctionType);

  /** Reimplemented to pass the parameter to the extract filter */
  virtual void SetExtractRegion(const RegionType& region)
  {
    // This check should be done in the itk::ExtractImageFilter
    if (this->GetExtractRegion() != region)
      {
      Superclass::SetExtractRegion(region);
      // SetExtractionRegion throws an exception in case of empty region
      if (region.GetNumberOfPixels() > 0)
        {
        m_ExtractFilter->SetExtractionRegion(region);
        }
      }
  }

  /** Reimplemented to pass the parameter to the extract filter */
  virtual void SetScaledExtractRegion(const RegionType& region)
  {
    // This check should be done in the itk::ExtractImageFilter
    if (this->GetScaledExtractRegion() != region)
      {
      Superclass::SetScaledExtractRegion(region);
      // SetExtractionRegion throws an exception in case of empty region
      if (region.GetNumberOfPixels() > 0)
        {
        m_ScaledExtractFilter->SetExtractionRegion(region);
        }
      }
  }

  /** Actually render the image */
  virtual void Render();

  /** Get the pixel description */
  virtual std::string GetPixelDescription(const IndexType& index, bool getPlaceName = true);

  /** Get the pixel location */
  virtual PointType GetPixelLocation(const IndexType& index);

  /** Get the list sample used by the rendering function */
  virtual ListSamplePointerType GetListSample()
  {
//     this->UpdateListSample(); //FIXME condition to IsModified
    return m_ListSample;
  }

  /** Set the list sample used by the rendering function */
  virtual void SetListSample(ListSamplePointerType listSample)
  {
    m_ListSample = listSample;
    m_ListSampleProvided = true;
    m_RenderingFunction->SetListSample(m_ListSample);
  }

  virtual LayerValueType GetValueAtIndex(const IndexType& index);
  
  itkSetMacro(ComputeHistoOnFullResolution, bool);
  itkSetMacro(ComputeHistoOnZoomResolution, bool);
  itkSetMacro(UpdateHisto, bool);

protected:
  /** Constructor */
  ImageLayer();
  /** Destructor */
  virtual ~ImageLayer();
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Update the histogram */
  virtual void UpdateListSample();

  /** Update the images */
  virtual void RenderImages();

  virtual void InitTransform();

  virtual void ComputeApproximativeGroundSpacing();
  /** Find out the histogram size from the pixel */
  unsigned int PixelSize(ImagePointerType image, ScalarType* v) const;
  unsigned int PixelSize(ImagePointerType image, VectorPixelType* v) const;
  unsigned int PixelSize(ImagePointerType image, RGBPixelType* v) const;
  unsigned int PixelSize(ImagePointerType image, RGBAPixelType* v) const;

private:
  ImageLayer(const Self&);     // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  /** Pointer to the quicklook */
  ImagePointerType m_Quicklook;

  /** Pointer to the image */
  ImagePointerType m_Image;

  /** List sample used to compute the histogram by the rendering function*/
  ListSamplePointerType m_ListSample;
  bool                  m_ListSampleProvided; //To remember if the list sample was provided manually by the user
  bool                  m_ComputeHistoOnFullResolution; //to compute histogram on full image, even if quicklook is available.
  bool                  m_ComputeHistoOnZoomResolution; //to compute histogram on zoom image, even if quicklook is available.
  bool                  m_UpdateHisto; //boolean to force histogram computation.

  /** Rendering function */
  RenderingFunctionPointerType m_RenderingFunction;

  /** Rendering filters */
  RenderingFilterPointerType m_QuicklookRenderingFilter;
  RenderingFilterPointerType m_ExtractRenderingFilter;
  RenderingFilterPointerType m_ScaledExtractRenderingFilter;

  /** Extract filters */
  ExtractFilterPointerType m_ExtractFilter;
  ExtractFilterPointerType m_ScaledExtractFilter;

  /** Coordinate transform */
  TransformType::Pointer    m_Transform;
  CoordinateToName::Pointer m_CoordinateToName;

  /** General info about the image*/
  std::string m_PlaceName; //FIXME the call should be done by a more general method outside of the layer
  std::string m_CountryName; //which would also handle the dependance to curl

  /** Ground spacing calculator */
  typename GroundSpacingImageType::Pointer m_GroundSpacing; //FIXME the call should be done by a more general method outside of the layer
  FloatType m_ApproxGroundSpacing;

}; // end class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageLayer.txx"
#endif

#endif
