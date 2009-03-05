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
#ifndef __otbImageLayerRenderingModel_h
#define __otbImageLayerRenderingModel_h

#include "otbMVCModelBase.h"
#include "otbLayerBasedModel.h"
#include "otbImageLayerBase.h"
#include "otbObjectList.h"
#include "otbImageLayerRenderingModelListener.h"
#include "otbBlendingImageFilter.h"

namespace otb
{
/** \class ImageLayerRenderingModel
*   \brief This class is the model for ImageViewer.
*   It is in charge of rendering to the screen a set of Layer.
*   Each visible layer is rendered separately, and the resulting
*   rendered layers are rasterized using the blending function
*   associated to each layer.
* 
* \sa Layer
* \sa BlendingFunction
*
*/

template <class TOutputImage = otb::Image<itk::RGBPixel<unsigned char>,2 >  > 
class ImageLayerRenderingModel
  : public MVCModelBase<ImageLayerRenderingModelListener>, public LayerBasedModel< ImageLayerBase<TOutputImage> >
{
public:
  /** Standard class typedefs */
  typedef ImageLayerRenderingModel                                 Self;
  typedef LayerBasedModel< ImageLayerBase <TOutputImage> > Superclass;
  typedef itk::SmartPointer<Self>                          Pointer;
  typedef itk::SmartPointer<const Self>                    ConstPointer;
    
  /** Runtime information */
  itkTypeMacro(ImageLayerRenderingModel,LayerBasedModel);

  /** New macro */
  itkNewMacro(Self);

  /** Output image typedef */
  typedef TOutputImage                         OutputImageType;
  typedef typename OutputImageType::Pointer    OutputImagePointerType;
  
  /** Layer typedef */
  typedef typename Superclass::LayerType       LayerType;
  typedef typename LayerType::RegionType       RegionType;
  typedef typename RegionType::IndexType       IndexType;
  
  /** Layer list typedef */
  typedef typename Superclass::LayerListType    LayerListType;
  typedef typename LayerListType::Pointer       LayerListPointerType;
  typedef typename LayerListType::ConstIterator LayerIteratorType;

  /** Listener typedef */
  typedef ImageLayerRenderingModelListener             ListenerType;

  /** Blending filter typedef */
  typedef otb::BlendingImageFilter<OutputImageType> BlendingFilterType;

  /** Blending filter typedef */
  typedef otb::ObjectList<BlendingFilterType>      BlendingFilterListType;
  typedef typename BlendingFilterListType::Pointer BlendingFilterListPointerType;
  typedef typename BlendingFilterListType::Iterator BlendingFilterIteratorType;

  /** Get/Set the viewer name */
  itkGetStringMacro(Name);
  itkSetStringMacro(Name);
  
  /** Get the rasterized views */
  itkGetObjectMacro(RasterizedQuicklook,OutputImageType);
  itkGetObjectMacro(RasterizedExtract,OutputImageType);
  itkGetObjectMacro(RasterizedScaledExtract,OutputImageType);


  /** Set/Get the Extract Region */
  itkSetMacro(ExtractRegion,RegionType);
  itkGetConstReferenceMacro(ExtractRegion,RegionType);
  
  /** Set/Get the Scaled Extract Region */
  itkSetMacro(ScaledExtractRegion,RegionType);
  itkGetConstReferenceMacro(ScaledExtractRegion,RegionType);

  /** Get the state of each view */
  itkGetMacro(HasQuicklook,bool);
  itkGetMacro(HasExtract,bool);
  itkGetMacro(HasScaledExtract,bool);
  
  /** Update will render all visible layers, rasterize all visible
   * layers and notify all listeners. */
  void Update(void);

  /** Change the Scaled extract region by giving the center of the
   * region */
  void SetScaledExtractRegionCenter(const IndexType & index);

  /** Change the extract region by giving the center of the
   * region */
  void SetExtractRegionCenter(const IndexType & index);
  
  /** Get the sumbsampling rate */
  unsigned int GetSubsamplingRate();

protected:
  /** Constructor */
  ImageLayerRenderingModel();
  /** Destructor */
  ~ImageLayerRenderingModel();

  /** Printself method */
  void          PrintSelf(std::ostream& os, itk::Indent indent) const;

  /** Renders all visible layers */
   void         RenderVisibleLayers(void);

  /** Rasterize visible layers */
   void         RasterizeVisibleLayers(void);

  /** Notify a registered listener */
   void         Notify(ListenerType * listener);

  /** Constrains the given region to the largest possible one. */
  RegionType    ConstrainRegion(const RegionType & region, const RegionType & largest);

private:
  ImageLayerRenderingModel(const Self&);     // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  /** Viewer name */
  std::string m_Name;
  
  /** Rasterized quicklook */
  OutputImagePointerType m_RasterizedQuicklook;
  bool                   m_HasQuicklook;

  /** Rendered extract */
  OutputImagePointerType m_RasterizedExtract;
  bool                   m_HasExtract;
  RegionType             m_ExtractRegion;
  
  /** Rendered scaled extract */
  OutputImagePointerType m_RasterizedScaledExtract;
  bool                   m_HasScaledExtract;
  RegionType             m_ScaledExtractRegion;

  /** Wether the model is currently updating or not */
  bool                   m_Updating;

  /** Lists of the active blending filters */
  BlendingFilterListPointerType m_QuicklookBlendingFilterList;
  BlendingFilterListPointerType m_ExtractBlendingFilterList;
  BlendingFilterListPointerType m_ScaledExtractBlendingFilterList;
  
}; // end class 
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbImageLayerRenderingModel.txx"
#endif

#endif


