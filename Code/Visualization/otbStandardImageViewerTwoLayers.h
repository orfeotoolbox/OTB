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
#ifndef __otbStandardImageViewerTwoLayers_h
#define __otbStandardImageViewerTwoLayers_h

#include "otbImage.h"
#include "itkRGBAPixel.h"
#include "otbImageLayer.h"
#include "otbImageLayerRenderingModel.h"
#include "otbImageView.h"
#include "otbImageWidgetController.h"
#include "otbImageLayerGenerator.h"
#include "otbWidgetResizingActionHandler.h"
#include "otbArrowKeyMoveActionHandler.h"
#include "otbChangeScaledExtractRegionActionHandler.h"
#include "otbChangeExtractRegionActionHandler.h"
#include "otbChangeScaleActionHandler.h"
#include "otbCurves2DWidget.h"
#include "otbHistogramCurve.h"
#include "otbPixelDescriptionModel.h"
#include "otbPixelDescriptionActionHandler.h"
#include "otbPixelDescriptionView.h"
#include "otbStandardRenderingFunction.h"
#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"
#include "otbVectorDataProjectionFilter.h"
#include "otbVectorDataGlComponent.h"
#include "otbVectorDataExtractROI.h"
#include "otbRemoteSensingRegion.h"

//#include "otbImageWidgetPackedManager.h"
#include "otbPackedWidgetManager.h"

namespace otb
{
/** \class StandardImageViewerTwoLayers
*   \brief This class implements a standard visualization tool to be
*   plugged at the end of a pipeline.
*
*  \ingroup Visualization
 */

template <class TImage, class TSecondImage=TImage, class TVectorData = VectorData<double> , class TWidgetManager = otb::PackedWidgetManager>
class StandardImageViewerTwoLayers
  : public itk::Object//, public ImageWidgetPackedManager
{
public:
  /** Standard class typedefs */
  typedef StandardImageViewerTwoLayers               Self;
  typedef itk::Object                       Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(StandardImageViewerTwoLayers,Object);

  /** Input image type */
  typedef TImage                                    ImageType;
  typedef typename ImageType::Pointer               ImagePointerType;
  typedef typename ImageType::InternalPixelType     InternalPixelType;

  typedef TSecondImage                                    SecondImageType;
  typedef typename SecondImageType::Pointer               SecondImagePointerType;

  /** VectorData typedef */
  typedef TVectorData                               VectorDataType;
  typedef typename VectorDataType::Pointer          VectorDataPointerType;

  /** PackedWidgetManager */
  typedef TWidgetManager                            WidgetManagerType;
  typedef typename WidgetManagerType::Pointer       WidgetManagerPointerType;

  /** Output image type */
  typedef itk::RGBAPixel<unsigned char>              RGBPixelType;
  typedef Image<RGBPixelType,2>                     OutputImageType;

  /** Image layer type */
  typedef ImageLayer<ImageType>                     ImageLayerType;
  typedef typename ImageLayerType::Pointer          ImageLayerPointerType;
  typedef typename ImageLayerType::HistogramType    HistogramType;

  typedef ImageLayer<SecondImageType>               SecondImageLayerType;

  /** Image layer generator type */
  typedef ImageLayerGenerator<ImageLayerType>       ImageLayerGeneratorType;
  typedef typename ImageLayerGeneratorType::Pointer ImageLayerGeneratorPointerType;
  typedef ImageLayerGenerator<SecondImageLayerType>       SecondImageLayerGeneratorType;
  typedef typename SecondImageLayerGeneratorType::Pointer SecondImageLayerGeneratorPointerType;

  /** Rendering model type */
  typedef ImageLayerRenderingModel<OutputImageType> RenderingModelType;
  typedef typename RenderingModelType::Pointer      RenderingModelPointerType;

  /** View type */
  typedef ImageView<RenderingModelType>             ViewType;
  typedef typename ViewType::Pointer                ViewPointerType;

  /** Widget controller */
  typedef ImageWidgetController                     WidgetControllerType;
  typedef typename WidgetControllerType::Pointer    WidgetControllerPointerType;

  /** Curves 2D widget */
  typedef Curves2DWidget                            CurvesWidgetType;
  typedef typename CurvesWidgetType::Pointer        CurvesWidgetPointerType;
  typedef HistogramCurve<HistogramType>             HistogramCurveType;
  typedef typename HistogramCurveType::Pointer      HistogramCurvePointerType;

  /** Standard action handlers */
  typedef otb::WidgetResizingActionHandler
  <RenderingModelType,ViewType>                     ResizingHandlerType;
  typedef otb::ChangeScaledExtractRegionActionHandler
  <RenderingModelType,ViewType>                     ChangeScaledRegionHandlerType;
  typedef otb::ChangeExtractRegionActionHandler
  <RenderingModelType,ViewType>                     ChangeRegionHandlerType;
  typedef otb::ChangeScaleActionHandler
  <RenderingModelType,ViewType>                     ChangeScaleHandlerType;
  typedef otb::ArrowKeyMoveActionHandler
  <RenderingModelType,ViewType>                     ArrowKeyMoveActionHandlerType;
  /** Rendering function */
  typedef typename ImageLayerGeneratorType::RenderingFunctionType StandardRenderingFunctionType;
  typedef typename StandardRenderingFunctionType::Pointer StandardRenderingFunctionPointerType;


  /** Pixel description */
  typedef PixelDescriptionModel<OutputImageType>    PixelDescriptionModelType;
  typedef typename PixelDescriptionModelType::Pointer PixelDescriptionModelPointerType;
  typedef PixelDescriptionActionHandler
    < PixelDescriptionModelType, ViewType>          PixelDescriptionActionHandlerType;
  typedef otb::PixelDescriptionView
    < PixelDescriptionModelType >                   PixelDescriptionViewType;
  typedef typename PixelDescriptionViewType::Pointer PixelDescriptionViewPointerType;

  /** VectorData overlay */
  typedef VectorDataProjectionFilter
  <VectorDataType,VectorDataType>                    VectorDataProjectionFilterType;
  typedef VectorDataExtractROI<VectorDataType>       VectorDataExtractROIType;
  typedef typename VectorDataExtractROIType::RegionType RemoteSensingRegionType;
  typedef VectorDataGlComponent<VectorDataType>      VectorDataGlComponentType;
  typedef VectorDataFileReader<VectorDataType>       VectorDataFileReaderType;

  /** Set/Get the image to render */
  itkSetObjectMacro(Image,ImageType);
  itkGetObjectMacro(Image,ImageType);
  itkSetObjectMacro(SecondImage,SecondImageType);
  itkGetObjectMacro(SecondImage,SecondImageType);

  /** Set/Get the VectorData to render */
  itkSetObjectMacro(VectorData,VectorDataType);
  itkGetObjectMacro(VectorData,VectorDataType);

  /** Set/Get the DEM directory */
  itkSetStringMacro(DEMDirectory);
  itkGetStringMacro(DEMDirectory);

  /** Set/Get the Label */
  itkSetStringMacro(Label);
  itkGetStringMacro(Label);

  /** Update and show the widget (you should call Fl::run() to hold to
   * display */
  void Update();

protected:
  /** Constructor */
  StandardImageViewerTwoLayers();
  /** Destructor */
  ~StandardImageViewerTwoLayers();
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const {};

private:
  StandardImageViewerTwoLayers(const Self&);     // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  /** Label of the viewer */
  std::string m_Label;

  /** Pointer to the image */
  ImagePointerType            m_Image;
  SecondImagePointerType            m_SecondImage;

  /** Pointer to the VectorData */
  VectorDataPointerType       m_VectorData;

  /** The image layer */
  ImageLayerPointerType       m_ImageLayer;

  /** The rendering model */
  RenderingModelPointerType   m_RenderingModel;

  /** The pixel description model */
  PixelDescriptionModelPointerType m_PixelDescriptionModel;

  /** The view */
  ViewPointerType             m_View;

  /** The pixel description view */
  PixelDescriptionViewPointerType m_PixelDescriptionView;

  /** Curve widget */
  CurvesWidgetPointerType     m_CurveWidget;

  /** The widget controller */
  WidgetControllerPointerType m_Controller;

  /** StandardRenderingFunction */
  StandardRenderingFunctionPointerType m_RenderingFunction;

  /** Path to the DEMDirectory (used if a VectorData is rendered */
  std::string m_DEMDirectory;

  /** */
  WidgetManagerPointerType   m_DisplayWindow;
}; // end class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStandardImageViewerTwoLayers.txx"
#endif

#endif
