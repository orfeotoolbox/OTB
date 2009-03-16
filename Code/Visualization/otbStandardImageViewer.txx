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

template <class TImage>  
StandardImageViewer<TImage>
::StandardImageViewer() : m_Image(), m_ImageLayer(), m_RenderingModel(), m_View(), m_Controller(), m_Window(),
			  m_Width(800),m_Height(600), m_SideBarWidth(100)
{
  // Build a new rendering model
  m_RenderingModel = RenderingModelType::New();
  // Build a view
  m_View           = ViewType::New();
  // Build a controller
  m_Controller     = WidgetControllerType::New();
  
  // Wire the MBC
  m_View->SetModel(m_RenderingModel);
  m_View->SetController(m_Controller);

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

  // Build the window
  m_Window = new Fl_Window(0,0,m_Width,m_Height);
  
  m_Window->add(m_View->GetFullWidget());
  m_Window->resizable(m_View->GetFullWidget());
  m_View->GetFullWidget()->resize(0,0,m_Width-m_SideBarWidth,m_Height);

  m_Window->add(m_View->GetScrollWidget());
  m_View->GetScrollWidget()->resize(m_Width-m_SideBarWidth,0,m_SideBarWidth,m_Height/4);
  
  m_Window->add(m_View->GetZoomWidget());
  m_View->GetZoomWidget()->resize(m_Width-m_SideBarWidth,m_Height/4,m_SideBarWidth,m_Height/2);
  
}

template <class TImage>  
StandardImageViewer<TImage>
::~StandardImageViewer()
{
  delete m_Window;
}

template <class TImage>  
void
StandardImageViewer<TImage>
::Update()
{
  








  m_Window->show();

}
}
#endif
