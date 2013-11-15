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
#ifndef __otbHistogramAndTransferFunctionWidget_txx
#define __otbHistogramAndTransferFunctionWidget_txx

#include "otbHistogramAndTransferFunctionWidget.h"
#include <FL/gl.h>
#include <FL/Fl.H>

namespace otb
{

template <class THistogram, class TPixel>
HistogramAndTransferFunctionWidget<THistogram, TPixel>
::HistogramAndTransferFunctionWidget()
  : Fl_Gl_Window(0, 0, 0, 0, 0)
{
  m_Label = "Default label";
  m_TransferFunctionLabel = "Function label";
  m_HistogramColor[0] = 0.5;
  m_HistogramColor[1] = 0.5;
  m_HistogramColor[2] = 0.5;
  m_BackgroundColor[0] = 1;
  m_BackgroundColor[1] = 1;
  m_BackgroundColor[2] = 1;
  m_AxisColor[0] = 0;
  m_AxisColor[1] = 0;
  m_AxisColor[2] = 0;
  m_TextColor[0] = 0;
  m_TextColor[1] = 0;
  m_TextColor[2] = 0;
  m_GridColor[0] = 0.75;
  m_GridColor[1] = 0.75;
  m_GridColor[2] = 0.75;
  m_TransferFunctionColor[0] = 1;
  m_TransferFunctionColor[1] = 0;
  m_TransferFunctionColor[2] = 0;
  m_BoundColor[0] = 0;
  m_BoundColor[1] = 1;
  m_BoundColor[2] = 0;
  m_TransferFunction = NULL;
  m_Histogram = NULL;
  m_MarginX = 30;
  m_MarginY = 30;
  m_GridSizeX = 5;
  m_GridSizeY = 5;
  m_OutputHistogramMargin = 0.2;
  m_HistogramClamping = 1;
  m_Updating = false;
}

template <class THistogram, class TPixel>
void
HistogramAndTransferFunctionWidget<THistogram, TPixel>
::OpenGlSetup()
{
  if (!this->valid())
    {
    valid(1);
    glLoadIdentity();
    glViewport(0, 0, w(), h());
    glClearColor(m_BackgroundColor[0],
                 m_BackgroundColor[1],
                 m_BackgroundColor[2],
                 1);
    }
  glShadeModel(GL_FLAT);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glClear(GL_COLOR_BUFFER_BIT);    //this clears and paints to black
  glMatrixMode(GL_MODELVIEW);      //clear previous 3D draw params
  glLoadIdentity();
  glMatrixMode(GL_PROJECTION);
  this->ortho();
  glDisable(GL_BLEND);
}

template <class THistogram, class TPixel>
void
HistogramAndTransferFunctionWidget<THistogram, TPixel>
::GridRendering(double gridXSpacing, double gridYSpacing)
{
  double x, y;
  // Rendering grid
  glBegin(GL_LINES);
  glColor3d(m_GridColor[0], m_GridColor[1], m_GridColor[2]);
  for (unsigned int i = 1; i < m_GridSizeX; ++i)
    {
    x = static_cast<double>(i) * gridXSpacing + m_MarginX;
    y = m_MarginY;
    glVertex2d(x, y);
    y = static_cast<double>(this->h()) - m_MarginY;
    glVertex2d(x, y);
    }

  for (unsigned int i = 1; i < m_GridSizeY; ++i)
    {
    y = static_cast<double>(i) * gridYSpacing + m_MarginY;
    x = m_MarginX;
    glVertex2d(x, y);
    x = static_cast<double>(this->w()) - m_MarginX - m_OutputHistogramMargin*static_cast<double>(this->w());
    glVertex2d(x, y);
    }
  glEnd();
}

template <class THistogram, class TPixel>
void
HistogramAndTransferFunctionWidget<THistogram, TPixel>
::AxisRendering(void)
{
  glBegin(GL_LINES);
  glColor3d(m_AxisColor[0], m_AxisColor[1], m_AxisColor[2]);
  glVertex2d(m_MarginX, m_MarginY);
  glVertex2d(m_MarginX, static_cast<double>(this->h()) - m_MarginY / 2);
  glVertex2d(m_MarginX, m_MarginY);
  glVertex2d(static_cast<double>(this->w()) - m_MarginX / 2 - m_OutputHistogramMargin * static_cast<double>(
               this->w()), m_MarginY);
  glEnd();
}

template <class THistogram, class TPixel>
void
HistogramAndTransferFunctionWidget<THistogram, TPixel>
::HistogramRendering(double binWidth, double binHeightRatio, double maxFrequency)
{

  double                startx = m_MarginX;
  // Temporary vertex coordinates
  double x, y;
  // Rendering histogram
  typename HistogramType::ConstIterator it = m_Histogram->Begin();
  typename HistogramType::ConstIterator end = m_Histogram->End();
  while( it != end )
    {

    glBegin(GL_POLYGON);
    glColor3d(m_HistogramColor[0], m_HistogramColor[1], m_HistogramColor[2]);

    x = startx;
    y = m_MarginY;
    glVertex2d(x, y);
    y += binHeightRatio *
         static_cast<double>((it.GetFrequency() > maxFrequency / m_HistogramClamping ? maxFrequency /
                              m_HistogramClamping : it.GetFrequency()));
    glVertex2d(x, y);
    x += binWidth;
    glVertex2d(x, y);
    y = m_MarginY;
    glVertex2d(x, y);
    glEnd();
    ++it;
    startx += binWidth;
    }
}

template <class THistogram, class TPixel>
void
HistogramAndTransferFunctionWidget<THistogram, TPixel>
::BoundRendering(void)
{
  double x, y;
  // Rendering bounds
  glColor3d(m_BoundColor[0], m_BoundColor[1], m_BoundColor[2]);
  glBegin(GL_LINES);

  double factor = (m_Histogram->Quantile(0, 1.) - m_Histogram->Quantile(0, 0.))
                  / (static_cast<double>(this->w()) - 2 * m_MarginX - m_OutputHistogramMargin *
                     static_cast<double>(this->w()));

  x = m_MarginX + static_cast<double>(m_TransferFunction->GetLowerBound() - m_Histogram->Quantile(0, 0.)) / factor;

  y = m_MarginY;
  glVertex2d(x, y);
  y = static_cast<double>(this->h()) - m_MarginY;
  glVertex2d(x, y);
  x = m_MarginX + static_cast<double>(m_TransferFunction->GetUpperBound() - m_Histogram->Quantile(0, 0.)) / factor;
  glVertex2d(x, y);
  y =  m_MarginY;
  glVertex2d(x, y);
  glEnd();
}

template <class THistogram, class TPixel>
void
HistogramAndTransferFunctionWidget<THistogram, TPixel>
::LegendRendering(double gridXSpacing, double gridYSpacing, double maxFrequency)
{
  double x, y;
  // rendering axis values
  std::ostringstream oss;
  glColor3d(m_TextColor[0], m_TextColor[1], m_TextColor[2]);
  gl_font(FL_COURIER_BOLD, 10);
  double step = (m_Histogram->Quantile(0, 1.) - m_Histogram->Quantile(0, 0.))
                / static_cast<double>(m_GridSizeX);
  for (unsigned int i = 0; i < m_GridSizeX; ++i)
    {
    ValueType value = static_cast<ValueType>(m_Histogram->Quantile(0, 0.)
                                             + static_cast<double>(i) * step);
//      oss<<(int)value;
    oss << value;
    x = static_cast<double>(i) * gridXSpacing + m_MarginX;
    y = m_MarginY / 2;
    gl_draw(oss.str().c_str(), (float) x, (float) y);
    oss.str("");
    }
  for (unsigned int i = 0; i < m_GridSizeY; ++i)
    {
    unsigned int value = static_cast<unsigned int>(i
                                                   * maxFrequency /
                                                   (m_HistogramClamping * static_cast<double>(m_GridSizeY)));
    oss << value;
    y = static_cast<double>(i) * gridYSpacing + m_MarginY;
    x = 0;
    gl_draw(oss.str().c_str(), (float) x, (float) y);
    oss.str("");
    }
//rendering label
  gl_font(FL_COURIER_BOLD, 12);
  gl_draw(m_Label.c_str(), static_cast<float>(3 * this->w()) / 8,
          static_cast<float>(static_cast<double>(this->h()) - m_MarginY / 2));

}

template <class THistogram, class TPixel>
void
HistogramAndTransferFunctionWidget<THistogram, TPixel>
::TransferFunctionRendering()
{
  glColor3d(m_TransferFunctionColor[0], m_TransferFunctionColor[1], m_TransferFunctionColor[2]);
  glBegin(GL_LINE_STRIP);
  double x, y;
  double step = (m_Histogram->Quantile(0, 1.) - m_Histogram->Quantile(0, 0.))
                / static_cast<double>(m_Histogram->GetSize()[0]);
  double xscale =
    (static_cast<double>(this->w()) - 2 * m_MarginX - m_OutputHistogramMargin *
     static_cast<double>(this->w())) / static_cast<double>(m_Histogram->GetSize()[0]);
  double yscale = (static_cast<double>(this->h()) - 2 * m_MarginY) / 255.;
  for (unsigned int i = 0; i < m_Histogram->Size() + 1; ++i)
    {
    PixelType value = static_cast<PixelType>(m_Histogram->Quantile(0, 0.)
                                             + static_cast<double>(i) * step);

    x = m_MarginX + static_cast<double>(i) * xscale;
    y = m_MarginY + yscale * static_cast<double>(m_TransferFunction->Map(value));
    glVertex2d(x, y);
    }
  glEnd();
}

template <class THistogram, class TPixel>
void
HistogramAndTransferFunctionWidget<THistogram, TPixel>
::OutputHistogramRendering()
{
  std::vector<unsigned int> outputHistogram(256, 0);

  typename HistogramType::ConstIterator it = m_Histogram->Begin();
  typename HistogramType::ConstIterator end = m_Histogram->End();
  while( it != end )
    {
     outputHistogram[m_TransferFunction->Map(static_cast < PixelType >
                                            (it.GetMeasurementVector()[0]))] += static_cast<unsigned int>(
     it.GetFrequency());
     ++it;
    }
  // Setting the extremity to 0 to avoid sides effect
  outputHistogram[0] = 0;
  outputHistogram[255] = 0;

  std::vector<unsigned int>::iterator vit;
  unsigned int                        maxFrequency = 0;
  for (vit = outputHistogram.begin(); vit != outputHistogram.end(); ++vit)
    {
    if (maxFrequency < (*vit))
      {
      maxFrequency = (*vit);
      }
    }

  // Temporary vertex coordinates
  double x, y;
  if (maxFrequency > 0)
    {
    double binWidth = (static_cast<double>(this->h()) - 2 * m_MarginY) / 255.;
    double binLengthRatio = m_HistogramClamping * m_OutputHistogramMargin * static_cast<double>(this->w()) /
                            static_cast<double>(maxFrequency);
    double starty = m_MarginY;

    // Rendering histogram
    for (vit = outputHistogram.begin(); vit != outputHistogram.end(); ++vit, starty += binWidth)
      {
      glBegin(GL_POLYGON);
      glColor3d(m_TransferFunctionColor[0], m_TransferFunctionColor[1], m_TransferFunctionColor[2]);

      x = static_cast<double>(this->w()) - m_OutputHistogramMargin*static_cast<double>(this->w()) - m_MarginX / 2;
      y = starty;
      glVertex2d(x, y);
      x += binLengthRatio * static_cast<double>(
        (*vit) > maxFrequency / m_HistogramClamping ? maxFrequency / m_HistogramClamping : (*vit));
      glVertex2d(x, y);
      y += binWidth;
      glVertex2d(x, y);
      x = static_cast<double>(this->w()) - m_OutputHistogramMargin*static_cast<double>(this->w()) - m_MarginX / 2;
      glVertex2d(x, y);
      glEnd();
      }
    }
  glBegin(GL_LINE_LOOP);
  x = static_cast<double>(this->w()) - m_OutputHistogramMargin*static_cast<double>(this->w()) - m_MarginX / 2;
  y = m_MarginY;
  glVertex2d(x, y);
  x += m_OutputHistogramMargin * static_cast<double>(this->w());
  glVertex2d(x, y);
  y = static_cast<double>(this->h()) - m_MarginY;
  glVertex2d(x, y);
  x = static_cast<double>(this->w()) - m_OutputHistogramMargin*static_cast<double>(this->w()) - m_MarginX / 2;
  glVertex2d(x, y);
  glEnd();
}

template <class THistogram, class TPixel>
void
HistogramAndTransferFunctionWidget<THistogram, TPixel>
::TransferFunctionLabelRendering(void)
{
//rendering label
  gl_font(FL_COURIER_BOLD, 12);
  gl_draw(m_TransferFunctionLabel.c_str(),
          static_cast<float>(this->w() - m_MarginX / 2 - m_OutputHistogramMargin * static_cast<double>(
                               this->w())), static_cast<float>(m_MarginY / 2));

}

template <class THistogram, class TPixel>
void
HistogramAndTransferFunctionWidget<THistogram, TPixel>
::draw()
{
  if (!m_Histogram)
    {
    return;
    }

  if (m_Updating) return;

  m_Updating = true;

  double                maxFrequency = 0;

  // Computing histogram max frequency
  typename HistogramType::ConstIterator itr = m_Histogram->Begin();
  typename HistogramType::ConstIterator end = m_Histogram->End();
  while( itr != end )
    {
    if (itr.GetFrequency() > maxFrequency)
      {
      maxFrequency = itr.GetFrequency();
      }
    ++itr;
    }

  double binWidth =
    static_cast<double>(this->w() - 2 * m_MarginX - m_OutputHistogramMargin *
                        static_cast<double>(this->w())) / static_cast<double>(m_Histogram->GetSize()[0]);
  double binHeightRatio = m_HistogramClamping * static_cast<double>(this->h() - 2 * m_MarginY) / static_cast<double>(
    maxFrequency);
  double gridXSpacing =
    (static_cast<double>(this->w()) - 2 * m_MarginX - m_OutputHistogramMargin *
     static_cast<double>(this->w())) / static_cast<double>(m_GridSizeX);
  double gridYSpacing = (static_cast<double>(this->h()) - 2 * m_MarginY) / static_cast<double>(m_GridSizeY);

  OpenGlSetup();
  GridRendering(gridXSpacing, gridYSpacing);
  AxisRendering();
  HistogramRendering(binWidth, binHeightRatio, maxFrequency);
  LegendRendering(gridXSpacing, gridYSpacing, maxFrequency);

  if (m_TransferFunction)
    {
    BoundRendering();
    TransferFunctionRendering();
    OutputHistogramRendering();
    TransferFunctionLabelRendering();
    }
  m_Updating = false;
}

template <class THistogram, class TPixel>
void
HistogramAndTransferFunctionWidget<THistogram, TPixel>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
}
#endif
