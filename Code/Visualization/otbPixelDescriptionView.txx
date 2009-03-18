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
#ifndef __otbPixelDescriptionView_txx
#define __otbPixelDescriptionView_txx

#include "otbPixelDescriptionView.h"

namespace otb
{
template < class TInputPixelDescription >
PixelDescriptionView<TInputPixelDescription>
::PixelDescriptionView() : m_PixelDescriptionWidget(NULL), m_Model()
{
  // build a widget with a default size
  m_PixelDescriptionWidget = new Fl_Multiline_Output(0,0,200,40);
  m_PixelDescriptionWidget->deactivate();
}

template < class TInputPixelDescription >
PixelDescriptionView<TInputPixelDescription>
::~PixelDescriptionView()
{
  if(m_PixelDescriptionWidget != NULL)
    {
    delete m_PixelDescriptionWidget;
    }
}

template < class TInputPixelDescription >
void
PixelDescriptionView<TInputPixelDescription>
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



template < class TInputPixelDescription >
void
PixelDescriptionView<TInputPixelDescription>
::PixelDescriptionModelNotify()
{
  this->Update();
}

template < class TInputPixelDescription >
void
PixelDescriptionView<TInputPixelDescription>
::Update()
{
  m_PixelDescriptionWidget->value(m_Model->GetPixelDescription());
  m_PixelDescriptionWidget->redraw();
}
}
#endif
