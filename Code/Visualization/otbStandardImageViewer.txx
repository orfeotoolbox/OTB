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
#ifndef __otbStandardImageViewer_txx
#define __otbStandardImageViewer_txx

#include "otbStandardImageViewer.h"
#include "otbForwardSensorModel.h"
#include "otbFltkFilterWatcher.h"

namespace otb
{

template <class TImage, class TVectorData, class TWidgetManager>
StandardImageViewer<TImage, TVectorData, TWidgetManager>
::StandardImageViewer() : m_Label("Default label"), m_Image(), m_VectorData(),
  m_ImageLayer(), m_RenderingModel(), m_PixelDescriptionModel(),
  m_View(), m_PixelDescriptionView(), m_CurveWidget(),
  m_Controller(), m_RenderingFunction(), m_DisplayWindow()
{
  // Build a new rendering model
  m_RenderingModel = RenderingModelType::New();
  m_PixelDescriptionModel = PixelDescriptionModelType::New();
  m_PixelDescriptionModel->SetLayers(m_RenderingModel->GetLayers());

  // Build a view
  m_View                 = ViewType::New();
  m_PixelDescriptionView = PixelDescriptionViewType::New();

  // Build a controller
  m_Controller     = WidgetControllerType::New();

  // Build the curve widget
  m_CurveWidget    = CurvesWidgetType::New();

  // Wire the MBC
  m_View->SetModel(m_RenderingModel);
  m_View->SetController(m_Controller);
  m_PixelDescriptionView->SetModel(m_PixelDescriptionModel);

  // Add the resizing handler
  ResizingHandlerType::Pointer resizingHandler = ResizingHandlerType::New();
  resizingHandler->SetModel(m_RenderingModel);
  resizingHandler->SetView(m_View);
  m_Controller->AddActionHandler(resizingHandler);

  // Add the change scaled region handler
  ChangeScaledRegionHandlerType::Pointer changeScaledHandler = ChangeScaledRegionHandlerType::New();
  changeScaledHandler->SetModel(m_RenderingModel);
  changeScaledHandler->SetView(m_View);
  m_Controller->AddActionHandler(changeScaledHandler);

  // Add the change extract region handler
  ChangeRegionHandlerType::Pointer changeHandler = ChangeRegionHandlerType::New();
  changeHandler->SetModel(m_RenderingModel);
  changeHandler->SetView(m_View);
  m_Controller->AddActionHandler(changeHandler);

  // Add the change scaled handler
  ChangeScaleHandlerType::Pointer changeScaleHandler = ChangeScaleHandlerType::New();
  changeScaleHandler->SetModel(m_RenderingModel);
  changeScaleHandler->SetView(m_View);
  m_Controller->AddActionHandler(changeScaleHandler);

// Add the pixel description action handler
  PixelDescriptionActionHandlerType::Pointer pixelActionHandler = PixelDescriptionActionHandlerType::New();
  pixelActionHandler->SetView(m_View);
  pixelActionHandler->SetModel(m_PixelDescriptionModel);
  m_Controller->AddActionHandler(pixelActionHandler);

  // Add the action handler for the arrow key
  ArrowKeyMoveActionHandlerType::Pointer arrowKeyMoveHandler = ArrowKeyMoveActionHandlerType::New();
  arrowKeyMoveHandler->SetModel(m_RenderingModel);
  arrowKeyMoveHandler->SetView(m_View);
  m_Controller->AddActionHandler(arrowKeyMoveHandler);

  /** Display Window*/
  m_DisplayWindow   = WidgetManagerType::New();
  m_DisplayWindow->RegisterFullWidget(m_View->GetFullWidget());
  m_DisplayWindow->RegisterScrollWidget(m_View->GetScrollWidget());
  m_DisplayWindow->RegisterZoomWidget(m_View->GetZoomWidget());
  m_DisplayWindow->RegisterPixelDescriptionWidget(m_PixelDescriptionView->GetPixelDescriptionWidget());
  m_DisplayWindow->RegisterHistogramWidget(m_CurveWidget);
}

template <class TImage, class TVectorData, class TWidgetManager>
StandardImageViewer<TImage, TVectorData, TWidgetManager>
::~StandardImageViewer()
{}

template <class TImage, class TVectorData, class TWidgetManager>
void
StandardImageViewer<TImage, TVectorData, TWidgetManager>
::Update()
{
  // First check if there is actually an input image
  if (m_Image.IsNull())
    {
    itkExceptionMacro(
      << "The image pointer is null, there is nothing to display. You probably forget to set the image.");
    }

  // Update image info for further use
  m_Image->UpdateOutputInformation();

  typename VectorDataProjectionFilterType::Pointer vproj;
  typename VectorDataExtractROIType::Pointer       vdextract;

  // Colors
  typename HistogramCurveType::ColorType red, green, blue, gray;
  red.Fill(0);
  red[0] = 1.;
  red[3] = 0.5;

  green.Fill(0);
  green[1] = 1.;
  green[3] = 0.5;

  blue.Fill(0);
  blue[2] = 1.;
  blue[3] = 0.5;

  gray.Fill(0.6);

  // If there is a VectorData
  if (m_VectorData.IsNotNull())
    {
    // Extract The part of the VectorData that actually overlaps with
    // the image extent
    vdextract = VectorDataExtractROIType::New();
    vdextract->SetInput(m_VectorData);

    // Find the geographic region of interest

    // Ge the index of the corner of the image
    typename ImageType::IndexType ul, ur, ll, lr;
    typename ImageType::PointType pul, pur, pll, plr;
    ul = m_Image->GetLargestPossibleRegion().GetIndex();
    ur = ul;
    ll = ul;
    lr = ul;
    ur[0] += m_Image->GetLargestPossibleRegion().GetSize()[0];
    lr[0] += m_Image->GetLargestPossibleRegion().GetSize()[0];
    lr[1] += m_Image->GetLargestPossibleRegion().GetSize()[1];
    ll[1] += m_Image->GetLargestPossibleRegion().GetSize()[1];

    // Transform to physical point
    m_Image->TransformIndexToPhysicalPoint(ul, pul);
    m_Image->TransformIndexToPhysicalPoint(ur, pur);
    m_Image->TransformIndexToPhysicalPoint(ll, pll);
    m_Image->TransformIndexToPhysicalPoint(lr, plr);

    // Build the cartographic region
    RemoteSensingRegionType                     rsRegion;
    typename RemoteSensingRegionType::IndexType rsOrigin;
    typename RemoteSensingRegionType::SizeType  rsSize;
    rsOrigin[0] = std::min(pul[0], plr[0]);
    rsOrigin[1] = std::min(pul[1], plr[1]);
    rsSize[0] = vcl_abs(pul[0] - plr[0]);
    rsSize[1] = vcl_abs(pul[1] - plr[1]);

    rsRegion.SetOrigin(rsOrigin);
    rsRegion.SetSize(rsSize);
    rsRegion.SetRegionProjection(m_Image->GetProjectionRef());
    rsRegion.SetKeywordList(m_Image->GetImageKeywordlist());

    // Set the cartographic region to the extract roi filter
    vdextract->SetRegion(rsRegion);
    
    // Reproject VectorData in image projection
    vproj = VectorDataProjectionFilterType::New();
    vproj->SetInput(vdextract->GetOutput());
    vproj->SetInputProjectionRef(m_VectorData->GetProjectionRef());
    vproj->SetOutputKeywordList(m_Image->GetImageKeywordlist());
    vproj->SetOutputProjectionRef(m_Image->GetProjectionRef());
    vproj->SetOutputOrigin(m_Image->GetOrigin());
    vproj->SetOutputSpacing(m_Image->GetSpacing());
    vproj->Update();

    // Create a VectorData gl component
    typename VectorDataGlComponentType::Pointer vgl = VectorDataGlComponentType::New();
    vgl->SetVectorData(vproj->GetOutput());
    vgl->SetColor(blue);
    // Add it to the image view
    m_View->GetScrollWidget()->AddGlComponent(vgl);
    m_View->GetFullWidget()->AddGlComponent(vgl);
    m_View->GetZoomWidget()->AddGlComponent(vgl);
    }

  // Generate the layer
  ImageLayerGeneratorPointerType generator = ImageLayerGeneratorType::New();
  generator->SetImage(m_Image);
  FltkFilterWatcher qlwatcher(generator->GetProgressSource(), 0, 0, 200, 20, "Generating QuickLook ...");
  generator->GenerateLayer();
  m_ImageLayer = generator->GetLayer();
  m_RenderingFunction = generator->GetRenderingFunction();

  // Set the window and layer label
  m_DisplayWindow->SetLabel(m_Label.c_str());
  m_ImageLayer->SetName(m_Label);

  // Add the generated layer to the rendering model
  m_RenderingModel->AddLayer(generator->GetLayer());

  // Show everything
  m_DisplayWindow->Show();

  // Update the rendering model
  m_RenderingModel->Update();

  // adding histograms rendering
  unsigned int listSize = m_ImageLayer->GetHistogramList()->Size();

  if (listSize > 0)
    {
    if (listSize == 1)
      {
      typename HistogramCurveType::Pointer grayhistogram = HistogramCurveType::New();
      grayhistogram->SetHistogram(m_ImageLayer->GetHistogramList()->GetNthElement(0));
      grayhistogram->SetHistogramColor(gray);
      grayhistogram->SetLabelColor(gray);
      m_CurveWidget->AddCurve(grayhistogram);
      }
    else
      {
      typename HistogramCurveType::Pointer rhistogram = HistogramCurveType::New();
      rhistogram->SetHistogram(m_ImageLayer->GetHistogramList()->GetNthElement(0));
      rhistogram->SetHistogramColor(red);
      rhistogram->SetLabelColor(red);
      m_CurveWidget->AddCurve(rhistogram);
      }
    if (listSize > 1)
      {
      typename HistogramCurveType::Pointer ghistogram = HistogramCurveType::New();
      ghistogram->SetHistogram(m_ImageLayer->GetHistogramList()->GetNthElement(1));
      ghistogram->SetHistogramColor(green);
      ghistogram->SetLabelColor(green);
      m_CurveWidget->AddCurve(ghistogram);
      }
    if (listSize > 2)
      {
      typename HistogramCurveType::Pointer bhistogram = HistogramCurveType::New();
      bhistogram->SetHistogram(m_ImageLayer->GetHistogramList()->GetNthElement(2));
      bhistogram->SetHistogramColor(blue);
      bhistogram->SetLabelColor(blue);
      m_CurveWidget->AddCurve(bhistogram);
      }
    }

  m_CurveWidget->SetXAxisLabel("Pixels");
  m_CurveWidget->SetYAxisLabel("Frequency");
}
}
#endif
