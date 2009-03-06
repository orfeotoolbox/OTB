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
#ifndef __otbHistogramCurve_txx
#define __otbHistogramCurve_txx

#include "otbHistogramCurve.h"

namespace otb
{

template <class THistogram> 
HistogramCurve<THistogram>
::HistogramCurve() : m_Histogram(), m_HistogramColor()
{
  // Default histogram color
  m_HistogramColor.Fill(0.5);
}

template <class THistogram> 
HistogramCurve<THistogram>
::~HistogramCurve()
{}

template <class THistogram> 
void  
HistogramCurve<THistogram>
::Render(const RegionType& extent,const AffineTransformType * space2ScreenTransform)
{
  // Iterate on the histogram
  HistogramIteratorType it = m_Histogram->Begin();

  PointType spacePoint, screenPoint;
  ContinuousIndexType cindex;

  glColor4d(m_HistogramColor[0],m_HistogramColor[1],m_HistogramColor[2],m_HistogramColor[3]);
 
  glBegin(GL_LINE_STRIP);

  while(it!= m_Histogram->End())
    {
    // Render each bin
    spacePoint[0] = it.GetMeasurementVector()[0];
    spacePoint[1] = it.GetFrequency();
    // Transform to screen
    screenPoint = space2ScreenTransform->TransformPoint(spacePoint);

    // Ensure propre clamping
    if(screenPoint[1] > extent.GetIndex()[1]+ extent.GetSize()[1])
      {
      screenPoint[1] = screenPoint[1] > extent.GetIndex()[1]+ extent.GetSize()[1];
      }

    // Convert to check IsIsinde
    cindex[0]=screenPoint[0];
    cindex[1]=screenPoint[1];

    if(extent.IsInside(cindex))
      {
      glVertex2d(screenPoint[0],screenPoint[1]);
      }
    ++it;
    }


  glEnd();
}

template <class THistogram> 
void
HistogramCurve<THistogram>
::BeforeRendering()
{
  // Iterate on the histogram
  HistogramIteratorType it = m_Histogram->Begin();
  
  // Initialize
  m_Minimum[0] = it.GetMeasurementVector()[0];
  m_Minimum[1] = it.GetFrequency();
  m_Maximum = m_Minimum;

  double mean  = it.GetFrequency();
  double squaremean = it.GetFrequency()*it.GetFrequency();
  unsigned int nbSamples = 1;
  double first = it.GetMeasurementVector()[0];
  double last  = 0;
  ++it;

  while(it!= m_Histogram->End())
    {
    if(m_Minimum[0] > it.GetMeasurementVector()[0])
      {
      m_Minimum[0] = it.GetMeasurementVector()[0];
      }
    
    if(m_Minimum[1] > it.GetFrequency())
      {
      m_Minimum[1] = it.GetFrequency();
      }
    if(m_Maximum[0] < it.GetMeasurementVector()[0])
      {
      m_Maximum[0] = it.GetMeasurementVector()[0];
      }
    mean  += it.GetFrequency();
    squaremean += it.GetFrequency()*it.GetFrequency();
    last = it.GetMeasurementVector()[0];
    ++nbSamples;
    ++it;
    }
  mean/=nbSamples;
  squaremean/=nbSamples;
  m_Maximum[1] = mean + vcl_sqrt(squaremean - mean*mean); 
  m_BinWidth = last-first/(2*nbSamples);
}


template <class THistogram> 
typename HistogramCurve<THistogram>
::PointType 
HistogramCurve<THistogram>
::GetMinimum()
{
  return m_Minimum;
}

template <class THistogram> 
typename HistogramCurve<THistogram>
::PointType 
HistogramCurve<THistogram>
::GetMaximum()
{
  return m_Maximum;
}


} // end namespace otb
#endif


