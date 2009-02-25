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
#ifndef __otbImageView_txx
#define __otbImageView_txx

#include "otbImageView.h"

namespace otb
{
template < class TInputImage >
ImageView<TInputImage>
::ImageView() : m_ScrollWidget(), m_FullWidget(), m_ZoomWidget(),
		  m_Model(), m_Controller()
{
  // Initializing the widgets
  m_ScrollWidget = ImageWidgetType::New();
  m_FullWidget   = ImageWidgetType::New();
  m_ZoomWidget   = ImageWidgetType::New();

  // Set the widget identifiers
  m_ScrollWidget->SetIdentifier("Scroll");
  m_FullWidget->SetIdentifier("Full");
  m_ZoomWidget->SetIdentifier("Zoom");
}

template < class TInputImage >
ImageView<TInputImage>
::~ImageView()
{
  
}

template < class TInputImage >
void
ImageView<TInputImage>
::SetModel(ModelType * model)
{
  // Unregister from previous model if nay
  if(m_Model.IsNotNull())
    {
    m_Model->UnRegisterListener(this);
    }
  
  // Set and register with new model
  m_Model = model;
  m_Model->RegisterListener(this);
}

template < class TInputImage >
void
ImageView<TInputImage>
::SetController(ControllerType * controller)
{
  m_Controller = controller;
  m_ScrollWidget->SetController(m_Controller);
  m_FullWidget->SetController(m_Controller);
  m_ZoomWidget->SetController(m_Controller);
}

template < class TInputImage>
void
ImageView<TInputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  // Call the superclass implementation
  Superclass::PrintSelf(os,indent);
}


template < class TInputImage >
void
ImageView<TInputImage>
::ImageViewerNotify()
{
  this->Update();
}

template < class TInputImage >
void
ImageView<TInputImage>
::Update()
{
  this->UpdateScrollWidget();
  this->UpdateFullWidget();
  this->UpdateZoomWidget();
}

template < class TInputImage >
void
ImageView<TInputImage>
::UpdateScrollWidget()
{
  // If the model has a quicklook
  if(m_Model->GetHasQuicklook())
    {
    // Read the buffer
    const RegionType qlRegion =  m_Model->GetRasterizedQuicklook()
      ->GetLargestPossibleRegion(); 
    m_ScrollWidget->ReadBuffer(m_Model->GetRasterizedQuicklook(),qlRegion);

    // Compute the appropriate scale
    const double wratio = static_cast<double>(m_ScrollWidget->w())/static_cast<double>(qlRegion.GetSize()[0]);
    const double hratio = static_cast<double>(m_ScrollWidget->h())/static_cast<double>(qlRegion.GetSize()[1]);
    const double scale = std::max(wratio,hratio);
    m_ScrollWidget->SetIsotropicZoom(scale);

    // Redraw
    m_ScrollWidget->redraw();
    }
}

template < class TInputImage >
void
ImageView<TInputImage>
::UpdateFullWidget()
{
  if(m_Model->GetHasExtract())
    {
    m_FullWidget->ReadBuffer(m_Model->GetRasterizedExtract(),m_Model->GetRasterizedExtract()
			     ->GetLargestPossibleRegion());
    m_FullWidget->redraw();
    }
 }

template < class TInputImage >
void
ImageView<TInputImage>
::UpdateZoomWidget()
{
  if(m_Model->GetHasScaledExtract())
    {
    m_ZoomWidget->ReadBuffer(m_Model->GetRasterizedScaledExtract(),m_Model->GetRasterizedScaledExtract()
			     ->GetLargestPossibleRegion());
    m_ZoomWidget->redraw();
    }  
}


}
#endif
