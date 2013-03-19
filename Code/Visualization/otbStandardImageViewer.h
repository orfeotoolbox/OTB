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
#ifndef __otbStandardImageViewer_h
#define __otbStandardImageViewer_h

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

#include "otbPackedWidgetManager.h"

namespace otb
{
/** \class StandardImageViewer
 *   \brief This class implements a standard visualization tool to be
 *   plugged at the end of a pipeline.
 *
 *  \ingroup Visualization
 *
 *   \example Tutorials/SimpleViewer.cxx
 */

template <class TImage, class TVectorData = VectorData<double>, class TWidgetManager = otb::PackedWidgetManager>
class StandardImageViewer
  : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef StandardImageViewer           Self;
  typedef itk::Object                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(StandardImageViewer, Object);

  /** Input image type */
  typedef TImage                                ImageType;
  typedef typename ImageType::Pointer           ImagePointerType;
  typedef typename ImageType::PixelType         PixelType;
  typedef typename ImageType::InternalPixelType InternalPixelType;
  typedef typename itk::NumericTraits<PixelType>
  ::ValueType ScalarType;

  /** VectorData typedef */
  typedef TVectorData                      VectorDataType;
  typedef typename VectorDataType::Pointer VectorDataPointerType;

  /** PackedWidgetManager */
  typedef TWidgetManager                      WidgetManagerType;
  typedef typename WidgetManagerType::Pointer WidgetManagerPointerType;

  /** Output image type */
  typedef itk::RGBAPixel<unsigned char> RGBPixelType;
  typedef Image<RGBPixelType, 2>        OutputImageType;

  /** Image layer type */
  typedef ImageLayer<ImageType, OutputImageType> ImageLayerType;
  typedef typename ImageLayerType::Pointer       ImageLayerPointerType;
  typedef typename ImageLayerType::HistogramType HistogramType;

  /** Image layer generator type */
  typedef ImageLayerGenerator<ImageLayerType>       ImageLayerGeneratorType;
  typedef typename ImageLayerGeneratorType::Pointer ImageLayerGeneratorPointerType;

  /** Rendering model type */
  typedef ImageLayerRenderingModel<OutputImageType> RenderingModelType;
  typedef typename RenderingModelType::Pointer      RenderingModelPointerType;

  /** View type */
  typedef ImageView<RenderingModelType> ViewType;
  typedef typename ViewType::Pointer    ViewPointerType;

  /** Widget controller */
  typedef ImageWidgetController                  WidgetControllerType;
  typedef typename WidgetControllerType::Pointer WidgetControllerPointerType;

  /** Curves 2D widget */
  typedef Curves2DWidget                       CurvesWidgetType;
  typedef typename CurvesWidgetType::Pointer   CurvesWidgetPointerType;
  typedef HistogramCurve<HistogramType>        HistogramCurveType;
  typedef typename HistogramCurveType::Pointer HistogramCurvePointerType;

  /** Standard action handlers */
  typedef otb::WidgetResizingActionHandler
  <RenderingModelType, ViewType>                     ResizingHandlerType;
  typedef otb::ChangeScaledExtractRegionActionHandler
  <RenderingModelType, ViewType>                     ChangeScaledRegionHandlerType;
  typedef otb::ChangeExtractRegionActionHandler
  <RenderingModelType, ViewType>                     ChangeRegionHandlerType;
  typedef otb::ChangeScaleActionHandler
  <RenderingModelType, ViewType>                     ChangeScaleHandlerType;
  typedef otb::ArrowKeyMoveActionHandler
  <RenderingModelType, ViewType>                     ArrowKeyMoveActionHandlerType;

  /** Rendering function */
  typedef typename ImageLayerGeneratorType::RenderingFunctionType StandardRenderingFunctionType;
  typedef typename StandardRenderingFunctionType::Pointer         StandardRenderingFunctionPointerType;

  /** Pixel description */
  typedef PixelDescriptionModel<OutputImageType>      PixelDescriptionModelType;
  typedef typename PixelDescriptionModelType::Pointer PixelDescriptionModelPointerType;
  typedef PixelDescriptionActionHandler
  <PixelDescriptionModelType, ViewType>          PixelDescriptionActionHandlerType;
  typedef otb::PixelDescriptionView
  <PixelDescriptionModelType>                   PixelDescriptionViewType;
  typedef typename PixelDescriptionViewType::Pointer PixelDescriptionViewPointerType;

  /** VectorData overlay */
  typedef VectorDataProjectionFilter
  <VectorDataType, VectorDataType>                  VectorDataProjectionFilterType;
  typedef VectorDataExtractROI<VectorDataType>          VectorDataExtractROIType;
  typedef typename VectorDataExtractROIType::RegionType RemoteSensingRegionType;
  typedef VectorDataGlComponent<VectorDataType>         VectorDataGlComponentType;
  typedef VectorDataFileReader<VectorDataType>          VectorDataFileReaderType;

  /** Set/Get the image to render */
  itkSetObjectMacro(Image, ImageType);
  itkGetObjectMacro(Image, ImageType);

  /** Set/Get the VectorData to render */
  itkSetObjectMacro(VectorData, VectorDataType);
  itkGetObjectMacro(VectorData, VectorDataType);

  /** Set/Get the Label */
  itkSetStringMacro(Label);
  itkGetStringMacro(Label);

/** Set center*/
  void SetCenter(typename ImageType::IndexType index)
  {
    m_RenderingModel->SetExtractRegionCenter(index);
    m_RenderingModel->SetScaledExtractRegionCenter(index);
    m_RenderingModel->Update();
  }

  /** Update and show the widget (you should call Fl::run() to hold to
   * display */
  void Update();

protected:
  /** Constructor */
  StandardImageViewer();
  /** Destructor */
  ~StandardImageViewer();
  /** Printself method */
  void PrintSelf(std::ostream&, itk::Indent) const {}

private:
  StandardImageViewer(const Self&);     // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  /** Label of the viewer */
  std::string m_Label;

  /** Pointer to the image */
  ImagePointerType m_Image;

  /** Pointer to the VectorData */
  VectorDataPointerType m_VectorData;

  /** The image layer */
  ImageLayerPointerType m_ImageLayer;

  /** The rendering model */
  RenderingModelPointerType m_RenderingModel;

  /** The pixel description model */
  PixelDescriptionModelPointerType m_PixelDescriptionModel;

  /** The view */
  ViewPointerType m_View;

  /** The pixel description view */
  PixelDescriptionViewPointerType m_PixelDescriptionView;

  /** Curve widget */
  CurvesWidgetPointerType m_CurveWidget;

  /** The widget controller */
  WidgetControllerPointerType m_Controller;

  /** StandardRenderingFunction */
  StandardRenderingFunctionPointerType m_RenderingFunction;

  /** */
  WidgetManagerPointerType m_DisplayWindow;
}; // end class
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStandardImageViewer.txx"
#endif

#endif
