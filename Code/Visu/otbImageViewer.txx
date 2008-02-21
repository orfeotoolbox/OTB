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
#ifndef _otbImageViewer_txx
#define _otbImageViewer_txx
#include "otbImageViewer.h"

namespace otb
{
  /// Constructor
  template <class TPixel, class TLabel>
  ImageViewer<TPixel,TLabel>
  ::ImageViewer()
  {
    m_FullWindow=NULL;
    m_ZoomWindow=NULL;
    m_ScrollWindow=NULL;
    m_HistogramWindow=NULL;
    m_PixLocWindow=NULL;
    m_PixLocOutput=NULL;
  }
  /// Destructor
  template <class TPixel, class TLabel>
  ImageViewer<TPixel,TLabel>
  ::~ImageViewer()
  {
    if(m_HistogramWindow!=NULL)
      {
	m_HistogramWindow->remove(this->GetRedHistogramWidget());
	if(this->GetFullWidget()->GetViewModelIsRGB())
	  {
	    m_HistogramWindow->remove(this->GetGreenHistogramWidget());
	    m_HistogramWindow->remove(this->GetBlueHistogramWidget());
	  }
	delete m_HistogramWindow;
	m_HistogramWindow = NULL;
      }

    if (m_FullWindow!=NULL)
      {
	m_FullWindow->remove(this->GetFullWidget());
	delete m_FullWindow;
	m_FullWindow = NULL;
      }
    if(m_ZoomWindow!=NULL)
      {
	m_ZoomWindow->remove(this->GetZoomWidget());
	delete m_ZoomWindow;
	m_ZoomWindow = NULL;
      }
    if(m_ScrollWindow!=NULL)
      {
	m_ScrollWindow->remove(this->GetScrollWidget());
	delete m_ScrollWindow;
	m_ScrollWindow = NULL;
      }
    
    if(m_PixLocWindow!=NULL)
      {
	m_PixLocWindow->remove(m_PixLocOutput);
	delete m_PixLocWindow;
	m_PixLocWindow = NULL;
      }
    if(m_PixLocOutput!=NULL)
      {
	delete m_PixLocOutput;
	m_PixLocOutput = NULL;
      }
  }
 
  /// Build the HMI
  template <class TPixel, class TLabel>
  void
  ImageViewer<TPixel,TLabel>
  ::Build(void)
  {
    Superclass::Build();
    itk::OStringStream oss;
  
  int wfull = this->GetFullWidget()->w();
  int hfull = this->GetFullWidget()->h();
    
  oss.str("");
  oss<<this->GetLabel()<<" - Full Window";
  m_FullWindow = new Fl_Window(0,0,wfull,hfull,"");
  m_FullWindow->copy_label(oss.str().c_str());
  m_FullWindow->add(this->GetFullWidget());
  m_FullWindow->resizable(this->GetFullWidget());
  m_FullWindow->end();
    

  oss.str("");
  oss<<this->GetLabel()<<" - Histograms and transfert functions"; 
  if(this->GetFullWidget()->GetViewModelIsRGB())
  {
    m_HistogramWindow = new Fl_Window(0,hfull,3*this->GetRedHistogramWidget()->w(),this->GetRedHistogramWidget()->h());
    m_HistogramWindow->add(this->GetRedHistogramWidget());
    m_HistogramWindow->add(this->GetGreenHistogramWidget());
    m_HistogramWindow->add(this->GetBlueHistogramWidget());
    this->GetGreenHistogramWidget()->resize(this->GetRedHistogramWidget()->w(),
					    0,
					    this->GetGreenHistogramWidget()->w(),
					    this->GetGreenHistogramWidget()->h());
    this->GetBlueHistogramWidget()->resize(2*this->GetRedHistogramWidget()->w(),
					   0,
					   this->GetBlueHistogramWidget()->w(),
					   this->GetBlueHistogramWidget()->h());
    m_HistogramWindow->resizable(m_HistogramWindow);	
    m_HistogramWindow->end();
	
  }
  else
  {
    m_HistogramWindow = new Fl_Window(0,hfull,this->GetRedHistogramWidget()->w(),this->GetRedHistogramWidget()->h());
    m_HistogramWindow->add(this->GetRedHistogramWidget());
    m_HistogramWindow->resizable(this->GetRedHistogramWidget());
    m_HistogramWindow->end();
  }
  m_HistogramWindow->copy_label(oss.str().c_str());


  int hscroll = 0;
  int wscroll = 0;

  if(this->GetUseScroll())
  {
    
    // Create the scroll windows   
    wscroll = this->GetScrollWidget()->w();
    hscroll = this->GetScrollWidget()->h();   
     
    oss.str("");
    oss<<this->GetLabel()<<" - Scroll Window";
    m_ScrollWindow = new Fl_Window(wfull+15,0,wscroll,hscroll,"");
    m_ScrollWindow->copy_label(oss.str().c_str());
    m_ScrollWindow->add(this->GetScrollWidget());
    m_ScrollWindow->resizable(this->GetScrollWidget());
    m_ScrollWindow->end();  
  }
    
  int wzoom = this->GetZoomWidget()->w();
  int hzoom = this->GetZoomWidget()->h();  
    
  // Create the zoom window
  std::string zoomLabel=this->GetLabel();
  zoomLabel.append(" - Zoom Window (X4)");
  m_ZoomWindow = new Fl_Window(wfull+15,hscroll+110,this->GetZoomMaxInitialSize(),this->GetZoomMaxInitialSize(),"");
  m_ZoomWindow->copy_label(zoomLabel.c_str());
  m_ZoomWindow->add(this->GetZoomWidget());
  m_ZoomWindow->resizable(this->GetZoomWidget());
  m_ZoomWindow->end();
    
  std::string pixLocLabel=this->GetLabel();
  pixLocLabel.append("Pixel location & values");
  m_PixLocWindow= new Fl_Window(wfull+15,hscroll+50,(wscroll<100 ? 100 : wscroll),40,"");
  m_PixLocOutput = new Fl_Multiline_Output(0,0,(wscroll<100 ? 100 : wscroll),40,"");
  m_PixLocWindow->copy_label(pixLocLabel.c_str());
  m_PixLocWindow->resizable(m_PixLocOutput);
  m_PixLocOutput->textsize(10);
  m_PixLocOutput->box(FL_EMBOSSED_BOX );
  m_PixLocWindow->end();
  this->SetPixLocOutput(m_PixLocOutput);
  }
  
/// Show the app
template <class TPixel, class TLabel>
void 
ImageViewer<TPixel,TLabel>
::Show(void)
{
  if(!this->GetBuilt())
    {
      this->Build();
    } 
  if(this->GetUseScroll())
    {
      m_ScrollWindow->show();
    }
  m_FullWindow->show();
  m_ZoomWindow->show();
  m_PixLocWindow->show();
  m_PixLocOutput->show();
  m_HistogramWindow->show();
  Superclass::Show();
 
  Fl::check();
}
/// Hide the app
template <class TPixel, class TLabel>
void 
ImageViewer<TPixel,TLabel>
::Hide(void)
{
  Superclass::Hide();
  if(this->GetUseScroll())
    {
      m_ScrollWindow->hide();
    }
  m_FullWindow->hide();   
  m_ZoomWindow->hide();
  m_PixLocWindow->hide();
  m_PixLocOutput->hide();
  m_HistogramWindow->hide();
  Fl::check();
} 
  
template <class TPixel, class TLabel>
void 
ImageViewer<TPixel,TLabel>
::Update(void)
{
  itk::OStringStream oss;
  oss<<this->GetLabel()<<" - Zoom Window (X"<<this->GetZoomWidget()->GetOpenGlIsotropicZoom()<<")";
  if(oss.good())
    {
      m_ZoomWindow->copy_label(oss.str().c_str());
    }
  m_ZoomWindow->redraw();
  Superclass::Update();
} 
  
} // end namespace otb
#endif
