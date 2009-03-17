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

namespace otb
{

template <class TImage,class TVectorData>  
StandardImageViewer<TImage,TVectorData>
::StandardImageViewer() : m_Label("Default label"), m_Image(), m_VectorData(), m_ImageLayer(), m_RenderingModel(),m_PixelDescriptionModel(), 
			  m_View(), m_PixelDescriptionView(), m_CurveWidget(), m_Controller(), m_RenderingFunction(), m_Window(),
			  m_FullGroup(),m_SideGroup(),m_Tile(),m_Width(800),m_Height(600), m_SideBarWidth(200)
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
  ChangeScaledRegionHandlerType::Pointer changeScaledHandler =ChangeScaledRegionHandlerType::New();
  changeScaledHandler->SetModel(m_RenderingModel);
  changeScaledHandler->SetView(m_View);
  m_Controller->AddActionHandler(changeScaledHandler);

  // Add the change extract region handler
  ChangeRegionHandlerType::Pointer changeHandler =ChangeRegionHandlerType::New();
  changeHandler->SetModel(m_RenderingModel);
  changeHandler->SetView(m_View);
  m_Controller->AddActionHandler(changeHandler);

  // Add the change scaled handler
  ChangeScaleHandlerType::Pointer changeScaleHandler =ChangeScaleHandlerType::New();
  changeScaleHandler->SetModel(m_RenderingModel);
  changeScaleHandler->SetView(m_View);
  m_Controller->AddActionHandler(changeScaleHandler);

// Add the pixel description action handler
  PixelDescriptionActionHandlerType::Pointer pixelActionHandler = PixelDescriptionActionHandlerType::New();
  pixelActionHandler->SetView(m_View);
  pixelActionHandler->SetModel(m_PixelDescriptionModel);
  m_Controller->AddActionHandler(pixelActionHandler);


  // Build the window
  m_Window = new Fl_Window(0,0,m_Width,m_Height);
  m_Tile = new Fl_Tile(0,0,m_Width,m_Height);
  m_Window->add(m_Tile);
  m_Window->resizable(m_Tile);
    
  m_Tile->add(m_View->GetFullWidget());
  //   m_Tile->resizable(m_View->GetFullWidget());

  m_Tile->add(m_View->GetScrollWidget());
  m_Tile->add(m_View->GetZoomWidget());
  m_Tile->add(m_CurveWidget);
  m_Tile->add(m_PixelDescriptionView->GetPixelDescriptionWidget());

  m_View->GetZoomWidget()->resize(m_Width-m_SideBarWidth,m_Height/4,m_SideBarWidth,m_Height/4);
  m_View->GetFullWidget()->resize(0,0,m_Width-m_SideBarWidth,m_Height);
  m_View->GetScrollWidget()->resize(m_Width-m_SideBarWidth,0,m_SideBarWidth,m_Height/4);
  m_CurveWidget->resize(m_Width-m_SideBarWidth,m_Height/2,m_SideBarWidth,m_Height/4);
  m_PixelDescriptionView->GetPixelDescriptionWidget()->resize(m_Width-m_SideBarWidth,3*m_Height/4,m_SideBarWidth,m_Height/4);
}

template <class TImage,class TVectorData>  
StandardImageViewer<TImage,TVectorData>
::~StandardImageViewer()
{
  m_Tile->remove(m_View->GetScrollWidget());
  m_Tile->remove(m_View->GetFullWidget());
  m_Tile->remove(m_View->GetZoomWidget());
  m_Tile->remove(m_CurveWidget);
  m_Tile->remove(m_PixelDescriptionView->GetPixelDescriptionWidget());
  delete m_Window;
}

template <class TImage,class TVectorData>  
void
StandardImageViewer<TImage,TVectorData>
::Update()
{
  // First check if there is actually an input image
  if(m_Image.IsNull())
    {
    itkExceptionMacro(<<"The image pointer is null, there is nothing to display. You probably forget to set the image.");
    }

  // Update image info for further use
  m_Image->UpdateOutputInformation();
  
  // If there is a VectorData
  if(m_VectorData.IsNotNull())
    {
    // Reproject VectorData in image projection
     typename VectorDataProjectionFilterType::Pointer vproj = VectorDataProjectionFilterType::New();
     vproj->SetInput(m_VectorData);
     vproj->SetOutputKeywordList(m_Image->GetImageKeywordlist());
     vproj->SetOutputOrigin(m_Image->GetOrigin());
     vproj->SetOutputSpacing(m_Image->GetSpacing());
     vproj->SetDEMDirectory(m_DEMDirectory);
     vproj->Update();
     
     // Create a VectorData gl component
     typename VectorDataGlComponentType::Pointer vgl = VectorDataGlComponentType::New();
     vgl->SetVectorData(vproj->GetOutput());

     // Add it to the image view
      m_View->GetScrollWidget()->AddGlComponent(vgl);
      m_View->GetFullWidget()->AddGlComponent(vgl);
      m_View->GetZoomWidget()->AddGlComponent(vgl);
    }

  // Generate the layer
 ImageLayerGeneratorPointerType generator = ImageLayerGeneratorType::New();
  generator->SetImage(m_Image);
  generator->GenerateLayer();
  m_ImageLayer = generator->GetLayer();
  m_RenderingFunction = generator->GetDefaultRenderingFunction();

  // Set the window and layer label
  m_Window->label(m_Label.c_str());
  m_ImageLayer->SetName(m_Label);

  // Add the generated layer to the rendering model
  m_RenderingModel->AddLayer(generator->GetLayer());

  // Show everything
  m_Window->show();
  m_View->GetScrollWidget()->show();
  m_View->GetFullWidget()->show();
  m_View->GetZoomWidget()->show();
  m_CurveWidget->show();
  
  // Update the rendering model
  m_RenderingModel->Update();

  // adding histograms rendering
  typename HistogramCurveType::ColorType red,green,blue, gray;
  red.Fill(0);
  red[0]=1.;
  red[3]=0.5;

  green.Fill(0);
  green[1]=1.;
  green[3]=0.5;

  blue.Fill(0);
  blue[2]=1.;
  blue[3]=0.5;

  gray.Fill(0.5);

  typename HistogramCurveType::Pointer rhistogram = HistogramCurveType::New();
  rhistogram->SetHistogram(m_ImageLayer->GetHistogramList()->GetNthElement(m_RenderingFunction->GetRedChannelIndex()));
  rhistogram->SetHistogramColor(red);
  rhistogram->SetLabelColor(red);
  
  typename HistogramCurveType::Pointer ghistogram = HistogramCurveType::New();
  ghistogram->SetHistogram(m_ImageLayer->GetHistogramList()->GetNthElement(m_RenderingFunction->GetGreenChannelIndex()));
  ghistogram->SetHistogramColor(green);
  ghistogram->SetLabelColor(green);

  typename HistogramCurveType::Pointer bhistogram = HistogramCurveType::New();
  bhistogram->SetHistogram(m_ImageLayer->GetHistogramList()->GetNthElement(m_RenderingFunction->GetBlueChannelIndex()));
  bhistogram->SetHistogramColor(blue);
  bhistogram->SetLabelColor(blue);
  m_CurveWidget->AddCurve(rhistogram);
  m_CurveWidget->AddCurve(ghistogram);
  m_CurveWidget->AddCurve(bhistogram);
  m_CurveWidget->SetXAxisLabel("Pixels");
  m_CurveWidget->SetYAxisLabel("Amount");
}
}
#endif
