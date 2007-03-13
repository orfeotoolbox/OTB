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
#include "otbFltkFilterWatcher.h"
#include "otbMacro.h"
#include "itkMacro.h"
#include <sstream>
#include "itkImageRegionIterator.h"

namespace otb
{
  /// Constructor
  template <class TPixel>
  ImageViewer<TPixel>
  ::ImageViewer()
  {
    m_UseScroll = false;
    m_ScrollMaxInitialSize = 300;
    m_FullMaxInitialSize = 600;
    m_ZoomMaxInitialSize = 200;
    m_ImageGeometry = 1.0;
    m_ScrollLimitSize = 600;
    m_Color[0]=1.0;
    m_Color[1]=0;
    m_Color[2]=0;
    m_Color[3]=1.0;
    m_ShrinkFactor=1;
    m_RedChannelIndex = 0;
    m_GreenChannelIndex = 1;
    m_BlueChannelIndex = 2;
    m_Built=false;   
    m_Shrink = ShrinkFilterType::New();  
    m_FullWindow=NULL;
    m_ZoomWindow=NULL;
    m_ScrollWindow=NULL;
    m_PixLocWindow=NULL;
    m_PixLocOutput=NULL;
    m_Label="OTB Image viewer";
    m_NormalizationFactor = 3.;
    m_QuicklookRatioCoef = 2;
    m_VectorCastFilter = NULL;
  }
  /// Destructor
  template <class TPixel>
  ImageViewer<TPixel>
  ::~ImageViewer()
  {
    if (m_FullWindow!=NULL)
    {
    	m_FullWindow->remove(m_FullWidget);
      	delete m_FullWindow;
      	m_FullWindow = NULL;
    }
    if(m_ZoomWindow!=NULL)
    {
    	m_ZoomWindow->remove(m_ZoomWidget);
      	delete m_ZoomWindow;
      	m_ZoomWindow = NULL;
    }
    
    if(m_ScrollWindow!=NULL)
    {
		m_ScrollWindow->remove(m_ScrollWidget);
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

  /// Compute the normalization factor
  template <class TPixel>
  void
  ImageViewer<TPixel>
  ::ComputeNormalizationFactors(void)
  {
    typedef itk::ImageRegionIterator<ImageType> IteratorType;
    typename ListSampleType::Pointer listSample = ListSampleType::New();
    unsigned int nbComponents = m_InputImage->GetNumberOfComponentsPerPixel();
    listSample->SetMeasurementVectorSize(nbComponents);
    m_MinComponentValue.SetSize(nbComponents);
    m_MaxComponentValue.SetSize(nbComponents);
    VectorPixelType absolutMax;
    VectorPixelType absolutMin;
    absolutMax.SetSize(nbComponents);
    absolutMin.SetSize(nbComponents);
    absolutMax.Fill(0);
    absolutMin.Fill(0);
    IteratorType it;
    // if scroll is activated, compute the factors from the quicklook
    if(m_UseScroll)
      {
	it = IteratorType(m_Shrink->GetOutput(),m_Shrink->GetOutput()->GetLargestPossibleRegion());
	it.GoToBegin();
      }
    // else, compute the factors from the full viewed region
    else
      {
	m_InputImage->SetRequestedRegion(m_FullWidget->GetViewedRegion());
	m_InputImage->PropagateRequestedRegion();
	m_InputImage->UpdateOutputData();
	it = IteratorType(m_InputImage,m_FullWidget->GetViewedRegion());
	it.GoToBegin();
      }
    while(!it.IsAtEnd())
      {
	listSample->PushBack(it.Get()); 
	for(unsigned int i = 0; i<nbComponents;++i)
	  {
	    if(it.Get()[i]>absolutMax[i])
	      absolutMax[i]=it.Get()[i];
	    if(it.Get()[i]<absolutMin[i])
	      absolutMin[i]=it.Get()[i];
	  } 
	++it;
      }
    otbMsgDebugMacro(<<"Sample list generated.");	       
    typename CovarianceCalculatorType::Pointer calc = CovarianceCalculatorType::New();
    calc->SetInputSample(listSample);
    calc->Update();
    otbMsgDebugMacro(<<"Statistics computed.");
    typename CovarianceCalculatorType::OutputType cov = *(calc->GetOutput());
    for(unsigned int i = 0; i<nbComponents;++i)
      {
	m_MinComponentValue[i] = static_cast<InputPixelType>((calc->GetMean())->GetElement(i)-m_NormalizationFactor*vcl_sqrt(cov(i,i)));
	m_MaxComponentValue[i] = static_cast<InputPixelType>((calc->GetMean())->GetElement(i)+m_NormalizationFactor*vcl_sqrt(cov(i,i)));
 	if(m_MinComponentValue[i]<absolutMin[i])
	  m_MinComponentValue[i]=absolutMin[i];
 	if(m_MaxComponentValue[i]>absolutMax[i])
	  m_MaxComponentValue[i]=absolutMax[i];
      }
    
    //TO UNCOMMENT TO HAVE THE SAME MEAN NORMALIZATION FACTOR FOR EACH BAND

    // InputPixelType min,max;
//     max = (m_MaxComponentValue[m_RedChannelIndex]
// 	   +m_MaxComponentValue[m_GreenChannelIndex]
// 	   +m_MaxComponentValue[m_BlueChannelIndex])/3;
//     min = (m_MinComponentValue[m_RedChannelIndex]
// 	   +m_MinComponentValue[m_GreenChannelIndex]
// 	   +m_MinComponentValue[m_BlueChannelIndex])/3;

    // TO UNCOMMENT TO HAVE THE MIN AND MAX NORMALIZATION FACTOR FOR THE WHOLE SET OF BANDS
//     otbMsgDebugMacro(<<"Normalization between: "<<m_MinComponentValue<<" and "<<m_MaxComponentValue);  
//     for(unsigned int i = 1; i<nbComponents;++i)
//       {
// 	if(min>m_MinComponentValue[i])
// 	  min=m_MinComponentValue[i];
// 	if(max<m_MaxComponentValue[i])
// 	  max=m_MaxComponentValue[i];
//       }
//     m_MinComponentValue.Fill(min);
//     m_MaxComponentValue.Fill(max);

   // END

    otbMsgDebugMacro(<<"Data min: "<<absolutMin<<", Data max: "<<absolutMax);
    otbMsgDebugMacro(<<"Normalization between: "<<m_MinComponentValue<<" and "<<m_MaxComponentValue);  
  }
  
  
  /// Build the HMI
  template <class TPixel>
  void
  ImageViewer<TPixel>
  ::Build(void)
  {
    otbMsgDebugMacro(<<"Entering build method");
    if(!m_InputImage)
      {
	itkExceptionMacro(<<"No input image !");
      } 
    std::stringstream oss;
    // Get the image dimension
    typename ImageType::SizeType size = m_InputImage->GetLargestPossibleRegion().GetSize();
    m_ImageGeometry = static_cast<double>(size[0])/static_cast<double>(size[1]);
    
    // initiate windows dimensions
    int wscroll=200;
    int hscroll=0;
    int wfull = (size[0]<m_FullMaxInitialSize ? size[0] : m_FullMaxInitialSize);
    int hfull = (size[1]<m_FullMaxInitialSize ? size[1] : m_FullMaxInitialSize);

    // Create full windows
    oss.str("");
    oss<<m_Label<<" - Full Window";
    m_FullWindow = new Fl_Window(0,0,wfull,hfull,"");
    m_FullWindow->copy_label(oss.str().c_str());
    m_FullWidget = FullWidgetType::New();
    m_FullWindow->resizable(m_FullWidget);
    m_FullWidget->SetParent(this);
    m_FullWindow->size_range(0,0,size[0],size[1]);
    m_FullWindow->end();
    m_FullWidget->SetInput(m_InputImage);
    m_FullWidget->Init(0,0,wfull,hfull,"");
    m_FullWidget->box( FL_EMBOSSED_BOX );
    m_FullWidget->SetFormOverlayVisible(true);
    
    // decide wether to use scroll view or not
    if(size[0]<m_ScrollLimitSize&&size[1]<m_ScrollLimitSize)
      {
	m_UseScroll=false;
      }
    else 
      {
	m_UseScroll=true;
	// Compute scroll size :
	if(m_ImageGeometry<1)
	  {
	    hscroll = m_ScrollMaxInitialSize;
	    wscroll = static_cast<int>(static_cast<double>(m_ScrollMaxInitialSize)*m_ImageGeometry);
	  }
	else
	  {
	    wscroll = m_ScrollMaxInitialSize;
	    hscroll = static_cast<int>(static_cast<double>(m_ScrollMaxInitialSize)/m_ImageGeometry);
	  }
	// Create the quicklook
	m_Shrink->SetInput(m_InputImage);
	if(size[0]/hscroll < size[1]/wscroll)
	  {
	    m_ShrinkFactor = static_cast<unsigned int>(vcl_ceil((static_cast<double>(size[0])/static_cast<double>(wscroll))/m_QuicklookRatioCoef));
	  }
	else
	  {
	    m_ShrinkFactor = static_cast<unsigned int>(vcl_ceil((static_cast<double>(size[1])/static_cast<double>(hscroll))/m_QuicklookRatioCoef));
	  }



	otbMsgDebugMacro("Shrink factor: "<<m_ShrinkFactor);
	m_Shrink->SetShrinkFactor(m_ShrinkFactor);
	typedef otb::FltkFilterWatcher WatcherType;
	WatcherType watcher(m_Shrink,wfull-200,hfull/2,200,20, "Generating Quicklook ...");
	m_Shrink->Update();
	
	// Create the scroll windows
	oss.str("");
	oss<<m_Label<<" - Scroll Window";
	m_ScrollWindow = new Fl_Window(wfull+15,0,wscroll,hscroll,"");
	m_ScrollWindow->copy_label(oss.str().c_str());
	m_ScrollWidget = ScrollWidgetType::New();
	m_ScrollWindow->resizable(m_ScrollWidget);
	m_ScrollWindow->size_range(wscroll,hscroll,size[0],size[1],0,0,1);
	m_ScrollWindow->end(); 
	m_ScrollWidget->SetInput(m_Shrink->GetOutput());
	m_ScrollWidget->SetParent(this);
	m_ScrollWidget->Init(0,0,wscroll,hscroll,oss.str().c_str());
	m_ScrollWidget->box( FL_EMBOSSED_BOX );
	m_ScrollWidget->SetFormOverlayVisible(true);
	
	// Create the scroll selection box
	BoxPointerType box = BoxType::New();
	SizeType scrollBoxSize;
	IndexType scrollBoxIndex;
	scrollBoxSize[0]=(m_FullWidget->GetViewedRegion().GetSize()[0]/m_ShrinkFactor)+1;
	scrollBoxSize[1]=(m_FullWidget->GetViewedRegion().GetSize()[1]/m_ShrinkFactor)+1;
	scrollBoxIndex[0]=(m_FullWidget->GetViewedRegion().GetIndex()[0]/m_ShrinkFactor)+1;
	scrollBoxIndex[1]=(m_FullWidget->GetViewedRegion().GetIndex()[1]/m_ShrinkFactor)+1;
	otbMsgDebugMacro(<<"Scroll box: "<<scrollBoxIndex<<" "<<scrollBoxSize);
	box->SetSize(scrollBoxSize);
	box->SetIndex(scrollBoxIndex);
	box->SetColor(m_Color);
	m_ScrollWidget->GetFormList()->PushBack(box);
	
	// Set the view model
	if(m_InputImage->GetNumberOfComponentsPerPixel()<=2)
	  {
		m_ScrollWidget->SetViewModelToGrayscale();
		 m_ScrollWidget->SetRedChannelIndex(m_RedChannelIndex);
	  }
	else
	  {
	    m_ScrollWidget->SetRedChannelIndex(m_RedChannelIndex);
	    m_ScrollWidget->SetGreenChannelIndex(m_GreenChannelIndex);
	    m_ScrollWidget->SetBlueChannelIndex(m_BlueChannelIndex);
	  }
      }
    // Create the zoom window
    std::string zoomLabel="Zoom Window";
    m_ZoomWindow = new Fl_Window(wfull+15,hscroll+110,m_ZoomMaxInitialSize,m_ZoomMaxInitialSize,zoomLabel.c_str());
    m_ZoomWidget = ZoomWidgetType::New();
    m_ZoomWidget->SetParent(this);
    m_ZoomWindow->resizable(m_ZoomWidget);
    m_ZoomWindow->size_range(0,0,size[0],size[1]);
    m_ZoomWindow->end();
    m_ZoomWidget->SetZoomFactor(4.0);
    m_ZoomWidget->SetInput(m_InputImage);
    m_ZoomWidget->Init(0,0,m_ZoomMaxInitialSize,m_ZoomMaxInitialSize,zoomLabel.c_str());
    m_ZoomWidget->box( FL_EMBOSSED_BOX );
    m_ZoomWidget->SetFormOverlayVisible(true);

    // Create the zoom selection mode
    BoxPointerType zoomBox = BoxType::New();
    SizeType zoomBoxSize;
    IndexType zoomBoxIndex;
    zoomBoxSize[0]=(m_ZoomWidget->GetViewedRegion().GetSize()[0])+1;
    zoomBoxSize[1]=(m_ZoomWidget->GetViewedRegion().GetSize()[1])+1;
    zoomBoxIndex[0]=(m_ZoomWidget->GetViewedRegion().GetIndex()[0])+1;
    zoomBoxIndex[1]=(m_ZoomWidget->GetViewedRegion().GetIndex()[1])+1;
    zoomBox->SetIndex(zoomBoxIndex);
    zoomBox->SetSize(zoomBoxSize);
    zoomBox->SetColor(m_Color);
    m_FullWidget->GetFormList()->PushBack(zoomBox);

    // Set the view model
    if(m_InputImage->GetNumberOfComponentsPerPixel()<=2)
      {
	m_FullWidget->SetViewModelToGrayscale();
	m_ZoomWidget->SetViewModelToGrayscale();
	otbMsgDebugMacro(<<"View model set to grayscale. Channel: "<<m_RedChannelIndex);
	m_ZoomWidget->SetRedChannelIndex(m_RedChannelIndex);
	m_FullWidget->SetRedChannelIndex(m_RedChannelIndex);
      }
    else
      {
	otbMsgDebugMacro(<<"View model set to RGB Composition.  R: "<<m_RedChannelIndex<<", G: "<<m_GreenChannelIndex<<", B: "<<m_BlueChannelIndex);
	m_ZoomWidget->SetRedChannelIndex(m_RedChannelIndex);
	m_ZoomWidget->SetGreenChannelIndex(m_GreenChannelIndex);
	m_ZoomWidget->SetBlueChannelIndex(m_BlueChannelIndex);
	m_FullWidget->SetRedChannelIndex(m_RedChannelIndex);
	m_FullWidget->SetGreenChannelIndex(m_GreenChannelIndex);
	m_FullWidget->SetBlueChannelIndex(m_BlueChannelIndex);

      }
 
    // Compute the normalization factors
    ComputeNormalizationFactors();

    // Set the normalization factors
    m_ZoomWidget->SetMinComponentValues(m_MinComponentValue);
    m_ZoomWidget->SetMaxComponentValues(m_MaxComponentValue);
    m_FullWidget->SetMinComponentValues(m_MinComponentValue);
    m_FullWidget->SetMaxComponentValues(m_MaxComponentValue);
    if(m_UseScroll)
      {
	m_ScrollWidget->SetMinComponentValues(m_MinComponentValue);
	m_ScrollWidget->SetMaxComponentValues(m_MaxComponentValue);
      }
    
    
    m_PixLocWindow= new Fl_Window(wfull+15,hscroll+50,wscroll,20,"Pixel location & values");
    m_PixLocOutput = new Fl_Output(0,0,wscroll,20,"Pixel location & values");
     m_PixLocWindow->resizable(m_PixLocOutput);
     m_PixLocOutput->textsize(10);
     m_PixLocOutput->box(FL_EMBOSSED_BOX );
     m_PixLocWindow->end();

      m_Built=true;
    // Built done
    // otbMsgDebugMacro(<<"Leaving build method");
  }
  /// Set the image (VectorImage version)
  template <class TPixel>
  void
  ImageViewer<TPixel>
  ::SetImage(ImageType * img)
  {
    m_InputImage = dynamic_cast<ImageType *>( img );
  } 

  /// Set the image (Image version)
  template <class TPixel>
  void
  ImageViewer<TPixel>
  ::SetImage(SingleImageType * img)
  {
    m_VectorCastFilter = VectorCastFilterType::New();
    m_VectorCastFilter->SetInput(img);
    m_VectorCastFilter->UpdateOutputInformation();
    m_InputImage = m_VectorCastFilter->GetOutput();
    
  } 

  /// Show the app
  template <class TPixel>
  void 
  ImageViewer<TPixel>
  ::Show(void)
  {
    if(!m_Built)
      {
	this->Build();
      }
    // otbMsgDebugMacro(<<"Entering show method.");
    Fl::check();
    if(m_UseScroll)
      {
	// otbMsgDebugMacro(<<"Showing scroll widget.");
 	m_ScrollWindow->show();
 	m_ScrollWidget->Show();
      }
    // otbMsgDebugMacro(<<"Showing full widget.");
    m_FullWindow->show();
    m_FullWidget->Show();
    // otbMsgDebugMacro(<<"Showing zoom widget.");
    m_ZoomWindow->show();
    m_ZoomWidget->Show();
    // otbMsgDebugMacro(<<"Between show and check");
    m_PixLocWindow->show();
    m_PixLocOutput->show();
    Fl::check();
    // otbMsgDebugMacro(<<"Leaving Show method.");
  }
  /** This is a helper class that performs a Show() and Fl::run() in order to ease 
      the use of the class for example in wrappings.*/
  template <class TPixel>
  int
  ImageViewer<TPixel>
  ::FlRun(void)
  {
    this->Show();
    return Fl::run();
  }


  /// Hide the app
  template <class TPixel>
  void 
  ImageViewer<TPixel>
  ::Hide(void)
  {
    Fl::check();
    if(m_UseScroll)
      {
	m_ScrollWindow->hide();
	m_ScrollWidget->hide();
      }
    m_FullWindow->hide();
    m_FullWidget->hide();
    m_ZoomWindow->hide();
    m_ZoomWidget->hide();
    m_PixLocWindow->hide();
    m_PixLocOutput->hide();
    Fl::check();

  } 

  /// Update the display
  template <class TPixel>
  void 
  ImageViewer<TPixel>
  ::Update(void)
  {
    Fl::check();
    UpdateScrollWidget();
    UpdateFullWidget();
    UpdateZoomWidget();
    Fl::check();
   
  }

  template <class TPixel>
  void 
  ImageViewer<TPixel>
  ::PrintPixLocVal(std::string str)
  {
    m_PixLocOutput->value(str.c_str());
    m_PixLocOutput->redraw();
    Fl::check();
  }

 template <class TPixel>
  void 
  ImageViewer<TPixel>
  ::UpdateZoomWidget(void)
  {
    std::stringstream oss;
    oss<<"Zoom Window (X"<<m_ZoomWidget->GetOpenGlIsotropicZoom()<<")";
    m_ZoomWindow->copy_label(oss.str().c_str());
    m_ZoomWindow->redraw();
    m_ZoomWidget->redraw();
  }

 template <class TPixel>
  void 
  ImageViewer<TPixel>
  ::UpdateFullWidget(void)
  {
    BoxPointerType zoomBox = BoxType::New();
    SizeType zoomBoxSize;
    IndexType zoomBoxIndex;
    zoomBoxSize[0]=(m_ZoomWidget->GetViewedRegion().GetSize()[0]);
    zoomBoxSize[1]=(m_ZoomWidget->GetViewedRegion().GetSize()[1]);
    zoomBoxIndex[0]=(m_ZoomWidget->GetViewedRegion().GetIndex()[0])+1;
    zoomBoxIndex[1]=(m_ZoomWidget->GetViewedRegion().GetIndex()[1])+1;
    zoomBox->SetIndex(zoomBoxIndex);
    zoomBox->SetSize(zoomBoxSize);
    zoomBox->SetColor(m_Color);
    m_FullWidget->GetFormList()->SetNthElement(0,zoomBox);
    m_FullWidget->redraw();
  }

   template <class TPixel>
  void 
  ImageViewer<TPixel>
  ::UpdateScrollWidget(void)
  {
    if(m_UseScroll)
      {
	BoxPointerType box = BoxType::New();
	SizeType scrollBoxSize;
	IndexType scrollBoxIndex;
	scrollBoxSize[0]=(m_FullWidget->GetViewedRegion().GetSize()[0]/m_ShrinkFactor)-1;
	scrollBoxSize[1]=(m_FullWidget->GetViewedRegion().GetSize()[1]/m_ShrinkFactor)-1;
	scrollBoxIndex[0]=(m_FullWidget->GetViewedRegion().GetIndex()[0]/m_ShrinkFactor)+1;
	scrollBoxIndex[1]=(m_FullWidget->GetViewedRegion().GetIndex()[1]/m_ShrinkFactor)+1;
	box->SetSize(scrollBoxSize);
	box->SetIndex(scrollBoxIndex);
	box->SetColor(m_Color);
	m_ScrollWidget->GetFormList()->SetNthElement(0,box);
	m_ScrollWidget->redraw();
      }
  }

template <class TPixel>
typename ImageViewer<TPixel>
::RegionType
ImageViewer<TPixel>
::ComputeConstrainedRegion(RegionType smallRegion, RegionType bigRegion)
{
  // This function assumes that smallRegion is inside huge region
  if(smallRegion.GetSize()[0]>bigRegion.GetSize()[0]
     ||smallRegion.GetSize()[1]>bigRegion.GetSize()[1])
    {
      itkExceptionMacro("Small region not inside big region !");
    }
  else
    {
      RegionType resp;
      IndexType index = smallRegion.GetIndex();
      SizeType size = smallRegion.GetSize();
      
      if(smallRegion.GetIndex()[0]<bigRegion.GetIndex()[0])
	{
	  index[0]=bigRegion.GetIndex()[0];
	}
      if(smallRegion.GetIndex()[1]<bigRegion.GetIndex()[1])
	{
	  index[1]=bigRegion.GetIndex()[1];
	}
      if(index[0]+size[0]>=bigRegion.GetIndex()[0]+bigRegion.GetSize()[0])
	{
	  index[0]=bigRegion.GetIndex()[0]+bigRegion.GetSize()[0]-size[0]-2;
	}
      if(index[1]+size[1]>=bigRegion.GetIndex()[1]+bigRegion.GetSize()[1])
	{
	  index[1]=bigRegion.GetIndex()[1]+bigRegion.GetSize()[1]-size[1]-2;
	}
      resp.SetSize(size);
      resp.SetIndex(index);
      return resp;
    }
}

template <class TPixel>
  void 
  ImageViewer<TPixel>
::ChangeFullViewedRegion(IndexType clickedIndex)
{
  RegionType region = m_FullWidget->GetViewedRegion();
  IndexType newIndex;
  newIndex[0]=clickedIndex[0]-region.GetSize()[0]/2;
  newIndex[1]=clickedIndex[1]-region.GetSize()[1]/2;
  
 
  region.SetIndex(newIndex);
  otbMsgDebugMacro(<<"New region: "<<region);

  RegionType newRegion = ComputeConstrainedRegion(region,m_InputImage->GetLargestPossibleRegion());
  otbMsgDebugMacro(<<"Constrained region: "<<newRegion);
  m_FullWidget->SetUpperLeftCorner(newRegion.GetIndex());
  this->UpdateScrollWidget();
}

template <class TPixel>
  void 
  ImageViewer<TPixel>
::ChangeZoomViewedRegion(IndexType clickedIndex)
{
  RegionType region = m_ZoomWidget->GetViewedRegion();
  IndexType newIndex;
  newIndex[0]=clickedIndex[0]-region.GetSize()[0]/2;
  newIndex[1]=clickedIndex[1]-region.GetSize()[1]/2;
 
  region.SetIndex(newIndex);
  RegionType newRegion = ComputeConstrainedRegion(region,m_FullWidget->GetViewedRegion());
  m_ZoomWidget->SetZoomUpperLeftCorner(newRegion.GetIndex());
  m_ZoomWidget->redraw();
  this->UpdateFullWidget();
}
} // end namespace otb
#endif

