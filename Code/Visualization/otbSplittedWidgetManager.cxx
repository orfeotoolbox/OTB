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
#include "otbSplittedWidgetManager.h"

namespace otb
{

SplittedWidgetManager
::SplittedWidgetManager()
{
  m_SplittedWidget = new ImageWidgetSplittedManagerType();
}

SplittedWidgetManager
::~SplittedWidgetManager()
{
  delete m_SplittedWidget;
}

void
SplittedWidgetManager
::RegisterFullWidget(WidgetType* fullWidget)
{
  m_InternalFullWidget = fullWidget;
  m_SplittedWidget->m_FullGroup->add(fullWidget);
  m_SplittedWidget->m_FullGroup->resizable(fullWidget);
  fullWidget->resize(0, 0, m_SplittedWidget->m_FullGroup->w(), m_SplittedWidget->m_FullGroup->h());
}

void
SplittedWidgetManager
::RegisterScrollWidget(WidgetType* scrollWidget)
{
  m_InternalScrollWidget = scrollWidget;
  m_SplittedWidget->m_QuicklookGroup->add(scrollWidget);
  m_SplittedWidget->m_QuicklookGroup->resizable(scrollWidget);
  scrollWidget->resize(0, 0, m_SplittedWidget->m_QuicklookGroup->w(), m_SplittedWidget->m_QuicklookGroup->h());

}

void
SplittedWidgetManager
::RegisterZoomWidget(WidgetType* zoomWidget)
{
  m_InternalZoomWidget = zoomWidget;
  m_SplittedWidget->m_ZoomGroup->add(zoomWidget);
  m_SplittedWidget->m_ZoomGroup->resizable(zoomWidget);
  zoomWidget->resize(0, 0, m_SplittedWidget->m_ZoomGroup->w(), m_SplittedWidget->m_ZoomGroup->h());
}
void
SplittedWidgetManager
::RegisterPixelDescriptionWidget(Fl_Output * pixelWidget)
{
  m_IntenalPixelWidget = pixelWidget;
  m_SplittedWidget->m_PixelDescriptionGroup->add(pixelWidget);
  m_SplittedWidget->m_PixelDescriptionGroup->resizable(pixelWidget);
  pixelWidget->resize(0, 0,
                      m_SplittedWidget->m_PixelDescriptionGroup->w(), m_SplittedWidget->m_PixelDescriptionGroup->h());

}

void
SplittedWidgetManager
::RegisterHistogramWidget(WidgetType* curveWidget)
{
  m_InternalCurveWidget = curveWidget;
  m_SplittedWidget->m_HistogramsGroup->add(curveWidget);
  m_SplittedWidget->m_HistogramsGroup->resizable(curveWidget);
  curveWidget->resize(0, 0, m_SplittedWidget->m_HistogramsGroup->w(), m_SplittedWidget->m_HistogramsGroup->h());
}

void
SplittedWidgetManager
::Show()
{
  m_SplittedWidget->m_FullGroup->show();
  m_SplittedWidget->m_ZoomGroup->show();
  m_SplittedWidget->m_PixelDescriptionGroup->show();
  m_SplittedWidget->m_QuicklookGroup->show();
  m_SplittedWidget->m_HistogramsGroup->show();

  m_InternalCurveWidget->show();
  m_IntenalPixelWidget->show();
  m_InternalFullWidget->show();
  m_InternalScrollWidget->show();
  m_InternalZoomWidget->show();
}

void
SplittedWidgetManager
::Hide()
{
  m_SplittedWidget->m_FullGroup->hide();
  m_SplittedWidget->m_ZoomGroup->hide();
  m_SplittedWidget->m_PixelDescriptionGroup->hide();
  m_SplittedWidget->m_QuicklookGroup->hide();
  m_SplittedWidget->m_HistogramsGroup->hide();
}

void
SplittedWidgetManager
::Refresh()
{
  m_InternalFullWidget->redraw();
  m_InternalScrollWidget->redraw();
  m_InternalZoomWidget->redraw();
  m_IntenalPixelWidget->redraw();
  m_InternalCurveWidget->redraw();
}

void
SplittedWidgetManager
::SetLabel(const char * label)
{
  std::ostringstream oss1;
  oss1.str("");
  oss1 << "Full Widget - " << label;
  m_SplittedWidget->m_FullGroup->copy_label(oss1.str().c_str());

  std::ostringstream oss2;
  oss2.str("");
  oss2 << "Zoom Widget - " << label;
  m_SplittedWidget->m_ZoomGroup->copy_label(oss2.str().c_str());

  std::ostringstream oss3;
  oss3.str("");
  oss3 << "Pixel Information Widget - " << label;
  m_SplittedWidget->m_PixelDescriptionGroup->copy_label(oss3.str().c_str());

  std::ostringstream oss4;
  oss4.str("");
  oss4 << "Scroll Widget - " << label;
  m_SplittedWidget->m_QuicklookGroup->copy_label(oss4.str().c_str());

  std::ostringstream oss5;
  oss5.str("");
  oss5 << "Histogram Widget - " << label;
  m_SplittedWidget->m_HistogramsGroup->copy_label(oss5.str().c_str());
}

void
SplittedWidgetManager
::UnRegisterAll()
{
  m_SplittedWidget->m_FullGroup->remove(m_InternalFullWidget);
  m_SplittedWidget->m_ZoomGroup->remove(m_InternalZoomWidget);
  m_SplittedWidget->m_PixelDescriptionGroup->remove(m_IntenalPixelWidget);
  m_SplittedWidget->m_QuicklookGroup->remove(m_InternalScrollWidget);
  m_SplittedWidget->m_HistogramsGroup->remove(m_InternalCurveWidget);
}

void
SplittedWidgetManager
::SetLabelScrollWidget(const char * label)
{
  m_SplittedWidget->m_QuicklookGroup->label(label);
}

void
SplittedWidgetManager
::SetLabelFullWidget(const char * label)
{
  m_SplittedWidget->m_FullGroup->label(label);
}

void
SplittedWidgetManager
::SetLabelZoomWidget(const char * label)
{
  m_SplittedWidget->m_ZoomGroup->label(label);
}

void
SplittedWidgetManager
::SetLabelHistogramWidget(const char * label)
{
  m_SplittedWidget->m_HistogramsGroup->label(label);
}

void
SplittedWidgetManager
::SetLabelPixelDescriptionWidget(const char * label)
{
  m_SplittedWidget->m_PixelDescriptionGroup->label(label);
}

int
SplittedWidgetManager
::ZoomWidgetShown()
{
  return m_SplittedWidget->m_ZoomGroup->shown();
}

int
SplittedWidgetManager
::FullWidgetShown()
{
  return m_SplittedWidget->m_FullGroup->shown();
}


void
SplittedWidgetManager
::SetResizable(bool resize)
{
  if (resize)
    {
    m_SplittedWidget->m_FullGroup->resizable(m_SplittedWidget->m_FullGroup);
    m_SplittedWidget->m_ZoomGroup->resizable(m_SplittedWidget->m_ZoomGroup);
    }
  else
    {
    // If the resizable is NULL then all widgets remain a fixed size
    m_SplittedWidget->m_FullGroup->resizable(NULL);
    m_SplittedWidget->m_ZoomGroup->resizable(NULL);
    }
}

}
