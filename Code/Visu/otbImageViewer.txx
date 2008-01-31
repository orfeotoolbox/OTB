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
#include "itkListSampleToHistogramGenerator.h"
#include "itkListSample.h"

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
    m_InterfaceBoxesColor[0]=1.0;
    m_InterfaceBoxesColor[1]=0;
    m_InterfaceBoxesColor[2]=0;
    m_InterfaceBoxesColor[3]=1.0;
    m_NextROIColor[0]=0;
    m_NextROIColor[1]=0;
    m_NextROIColor[2]=1.0;
    m_NextROIColor[3]=0.5;
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
    m_NormalizationFactor = 0.02;
    m_QuicklookRatioCoef = 2;
    m_VectorCastFilter = NULL;
    m_LinkedViewerList = ViewerListType::New();
    m_Updating = false;
    m_PolygonROIList = PolygonListType::New(); 
    m_InterfaceBoxesList = FormListType::New();


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

    typedef itk::ImageRegionConstIterator< ImageType >  InputIteratorType;
    typedef itk::Vector<typename ImageType::ValueType,1> MeasurementVectorType;
    typedef itk::Statistics::ListSample<MeasurementVectorType> ListSampleType;
    typedef float HistogramMeasurementType;
    typedef itk::Statistics::ListSampleToHistogramGenerator<ListSampleType,HistogramMeasurementType,
      itk::Statistics::DenseFrequencyContainer,1> HistogramGeneratorType;
    
    typedef otb::ObjectList<ListSampleType> ListSampleListType;
    

 
  m_MinComponentValue.SetSize(m_InputImage->GetNumberOfComponentsPerPixel());
  m_MaxComponentValue.SetSize(m_InputImage->GetNumberOfComponentsPerPixel());
  typename ListSampleListType::Pointer sl =  ListSampleListType::New();
  
  sl->Reserve(m_InputImage->GetNumberOfComponentsPerPixel());

  for(unsigned int i = 0;i<m_MaxComponentValue.GetSize();++i)
    {
      sl->PushBack(ListSampleType::New());
    }
  InputIteratorType it;
  // if scroll is activated, compute the factors from the quicklook
  if(m_UseScroll)
    {
      it = InputIteratorType(m_Shrink->GetOutput(),m_Shrink->GetOutput()->GetLargestPossibleRegion());
      it.GoToBegin();
    }
  // else, compute the factors from the full viewed region
  else
    {
      m_InputImage->SetRequestedRegion(m_FullWidget->GetViewedRegion());
      m_InputImage->PropagateRequestedRegion();
      m_InputImage->UpdateOutputData();
      it = InputIteratorType(m_InputImage,m_InputImage->GetRequestedRegion());
      it.GoToBegin();
    }
  
  
  while( !it.IsAtEnd() )
    {
      PixelType pixel = it.Get();
      for(unsigned int i = 0;i<m_MaxComponentValue.GetSize();++i)
	{
	  sl->GetNthElement(i)->PushBack(pixel[i]);
	}
      ++it;
    }

  for(unsigned int i = 0;i<m_MaxComponentValue.GetSize();++i)
    {
      typename HistogramGeneratorType::Pointer generator = HistogramGeneratorType::New();
      generator->SetListSample(sl->GetNthElement(i));
      typename HistogramGeneratorType::HistogramType::SizeType size;
      size.Fill(static_cast<unsigned int>(vcl_ceil(1/m_NormalizationFactor)*10));
      generator->SetNumberOfBins(size);
      generator->Update();
      m_MinComponentValue[i]=static_cast<typename ImageType::ValueType>(generator->GetOutput()->Quantile(0,m_NormalizationFactor));
      m_MaxComponentValue[i]=static_cast<typename ImageType::ValueType>(generator->GetOutput()->Quantile(0,1-m_NormalizationFactor));
    }
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
    itk::OStringStream oss;
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
    

    // Create the zoom window
    std::string zoomLabel="Zoom Window (X4)";
    m_ZoomWindow = new Fl_Window(wfull+15,hscroll+110,m_ZoomMaxInitialSize,m_ZoomMaxInitialSize,"");
    m_ZoomWindow->copy_label(zoomLabel.c_str());
    m_ZoomWidget = ZoomWidgetType::New();
    m_ZoomWidget->SetParent(this);
    m_ZoomWindow->resizable(m_ZoomWidget);
    m_ZoomWindow->size_range(0,0,size[0],size[1]);
    m_ZoomWindow->end();
    m_ZoomWidget->SetZoomFactor(4.0);
    m_ZoomWidget->SetInput(m_InputImage);
    m_ZoomWidget->Init(0,0,m_ZoomMaxInitialSize,m_ZoomMaxInitialSize,"");
    m_ZoomWidget->box( FL_EMBOSSED_BOX );
    m_ZoomWidget->SetFormOverlayVisible(true);
    

    // Create the zoom selection mode
    BoxPointerType zoomBox = BoxType::New();
    SizeType zoomBoxSize;
    IndexType zoomBoxIndex;
    zoomBoxSize[0]=(m_ZoomWidget->GetViewedRegion().GetSize()[0]);
    zoomBoxSize[1]=(m_ZoomWidget->GetViewedRegion().GetSize()[1]);
    zoomBoxIndex[0]=(m_ZoomWidget->GetViewedRegion().GetIndex()[0]);
    zoomBoxIndex[1]=(m_ZoomWidget->GetViewedRegion().GetIndex()[1]);
    zoomBox->SetIndex(zoomBoxIndex);
    zoomBox->SetSize(zoomBoxSize);
    zoomBox->SetColor(m_InterfaceBoxesColor);
    m_InterfaceBoxesList->PushBack(zoomBox);

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



	otbMsgDebugMacro(<<"Shrink factor: "<<m_ShrinkFactor);
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
	m_ScrollWidget->SetSubSamplingRate(m_ShrinkFactor);

	// Create the scroll selection box
	BoxPointerType box = BoxType::New();
	SizeType scrollBoxSize;
	IndexType scrollBoxIndex;
	scrollBoxSize[0]=(m_FullWidget->GetViewedRegion().GetSize()[0]);
	scrollBoxSize[1]=(m_FullWidget->GetViewedRegion().GetSize()[1]);
	scrollBoxIndex[0]=(m_FullWidget->GetViewedRegion().GetIndex()[0]);
	scrollBoxIndex[1]=(m_FullWidget->GetViewedRegion().GetIndex()[1])+1;
	otbMsgDebugMacro(<<"Scroll box: "<<scrollBoxIndex<<" "<<scrollBoxSize);
	box->SetSize(scrollBoxSize);
	box->SetIndex(scrollBoxIndex);
	box->SetColor(m_InterfaceBoxesColor);
	m_InterfaceBoxesList->PushBack(box);
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


    InitializeViewModel();
    GenerateOverlayList();
    
    m_Built=true;
  }
  /// Set the image (VectorImage version)
  template <class TPixel>
  void
  ImageViewer<TPixel>
  ::SetImage(ImageType * img)
  {
    m_InputImage = dynamic_cast<ImageType *>( img );
  } 

   template <class TPixel>
   typename ImageViewer<TPixel>
   ::ImageType *
   ImageViewer<TPixel>
   ::GetShrinkedImage(void)
   {
     if(m_UseScroll)
       {
	 return m_Shrink->GetOutput();
       }
     else
       {
	 return m_InputImage;
       }
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

  template <class TPixel>
  void
  ImageViewer<TPixel>
  ::GenerateOverlayList(void)
  {
    FormListPointerType new_list = FormListType::New();
    
    BoxPointerType zoomBox = BoxType::New();
    SizeType zoomBoxSize;
    IndexType zoomBoxIndex;
    zoomBoxSize[0]=(m_ZoomWidget->GetViewedRegion().GetSize()[0]);
    zoomBoxSize[1]=(m_ZoomWidget->GetViewedRegion().GetSize()[1]);
    zoomBoxIndex[0]=(m_ZoomWidget->GetViewedRegion().GetIndex()[0]);
    zoomBoxIndex[1]=(m_ZoomWidget->GetViewedRegion().GetIndex()[1]);
    zoomBox->SetIndex(zoomBoxIndex);
    zoomBox->SetSize(zoomBoxSize);
    zoomBox->SetColor(m_InterfaceBoxesColor);
    m_InterfaceBoxesList->SetNthElement(0,zoomBox);
    
    BoxPointerType box = BoxType::New();
    SizeType scrollBoxSize;
    IndexType scrollBoxIndex;
    scrollBoxSize[0]=(m_FullWidget->GetViewedRegion().GetSize()[0]);
    scrollBoxSize[1]=(m_FullWidget->GetViewedRegion().GetSize()[1]);
    scrollBoxIndex[0]=(m_FullWidget->GetViewedRegion().GetIndex()[0]);
    scrollBoxIndex[1]=(m_FullWidget->GetViewedRegion().GetIndex()[1])+1;
    box->SetSize(scrollBoxSize);
    box->SetIndex(scrollBoxIndex);
    box->SetColor(m_InterfaceBoxesColor);
    m_InterfaceBoxesList->SetNthElement(1,box);
    
    for(FormListIteratorType it1 = m_InterfaceBoxesList->Begin();
        it1!=m_InterfaceBoxesList->End();++it1)
        {
            new_list->PushBack(it1.Get());
        }
        
    for(PolygonListIteratorType it2 = m_PolygonROIList->Begin();
        it2!=m_PolygonROIList->End();++it2)
        {
            ImageWidgetPolygonFormPointerType new_poly = ImageWidgetPolygonFormType::New();
            new_poly->SetPolygon(it2.Get());
            new_poly->SetColor(m_NextROIColor);
            
            for(PolygonIteratorType pIt = it2.Get()->GetVertexList()->Begin();
              pIt != it2.Get()->GetVertexList()->End();++pIt)
              {
              ImageWidgetCircleFormPointerType new_circle = ImageWidgetCircleFormType::New();
              
              new_circle->SetCenter(pIt.Value());
              new_circle->SetRadius(4);
              new_circle->SetSolid(false);
              new_circle->SetColor(m_InterfaceBoxesColor);
              new_list->PushBack(new_circle);
              
              }
              new_list->PushBack(new_poly);
        }
        
        if(m_UseScroll)
        {
            m_ScrollWidget->SetFormListOverlay(new_list);
        }
     
    m_FullWidget->SetFormListOverlay(new_list);
    m_ZoomWidget->SetFormListOverlay(new_list);
  }



  /// Update the display
  template <class TPixel>
  void 
  ImageViewer<TPixel>
  ::Update(void)
  {
    GenerateOverlayList();
    Fl::check();
    UpdateScrollWidget();
    UpdateFullWidget();
    UpdateZoomWidget();
    
    // update the linked viewer
    typename ViewerListType::Iterator linkedIt = m_LinkedViewerList->Begin();
    typename OffsetListType::iterator offIt = m_LinkedViewerOffsetList.begin();
  
    while(linkedIt!=m_LinkedViewerList->End()&&offIt!=m_LinkedViewerOffsetList.end())
    {
      if(!linkedIt.Get()->GetUpdating())
	{
          linkedIt.Get()->Update();
        }   
    }
  Fl::check();
  }

  template <class TPixel>
  void 
  ImageViewer<TPixel>
  ::PrintPixLocVal(IndexType index, PixelType pixel)
  {
    itk::OStringStream oss;
    oss<<"Location: "<<index<<", values: "<<pixel;
    if(oss.good())
      {
	m_PixLocOutput->value(oss.str().c_str());
	m_PixLocOutput->redraw();
	Fl::check();
      }
  }

 template <class TPixel>
  void 
  ImageViewer<TPixel>
  ::ClearPixLocVal(void)
  {
    m_PixLocOutput->value("");
    m_PixLocOutput->redraw();
    Fl::check();
  }
  

 template <class TPixel>
  void 
  ImageViewer<TPixel>
  ::UpdateZoomWidget(void)
  {
    itk::OStringStream oss;
    oss<<"Zoom Window (X"<<m_ZoomWidget->GetOpenGlIsotropicZoom()<<")";
    if(oss.good())
      {
	m_ZoomWindow->copy_label(oss.str().c_str());
      }
    m_ZoomWindow->redraw();
    m_ZoomWidget->redraw();
  }

 template <class TPixel>
  void 
  ImageViewer<TPixel>
  ::UpdateFullWidget(void)
  {
    m_FullWidget->redraw();
  }

   template <class TPixel>
  void 
  ImageViewer<TPixel>
  ::UpdateScrollWidget(void)
  {
    if(m_UseScroll)
      {	
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
	  index[0]=bigRegion.GetIndex()[0]+bigRegion.GetSize()[0]-size[0];
	}
      if(index[1]+size[1]>=bigRegion.GetIndex()[1]+bigRegion.GetSize()[1])
	{
	  index[1]=bigRegion.GetIndex()[1]+bigRegion.GetSize()[1]-size[1];
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
  m_Updating = true;
  RegionType region = m_FullWidget->GetViewedRegion();
  IndexType newIndex;
  newIndex[0]=clickedIndex[0]-region.GetSize()[0]/2;
  newIndex[1]=clickedIndex[1]-region.GetSize()[1]/2;

  region.SetIndex(newIndex);

  RegionType newRegion = ComputeConstrainedRegion(region,m_InputImage->GetLargestPossibleRegion());
  m_FullWidget->SetUpperLeftCorner(newRegion.GetIndex());

  typename ViewerListType::Iterator linkedIt = m_LinkedViewerList->Begin();
  typename OffsetListType::iterator offIt = m_LinkedViewerOffsetList.begin();
  
  while(linkedIt!=m_LinkedViewerList->End()&&offIt!=m_LinkedViewerOffsetList.end())
    {
      if(!linkedIt.Get()->GetUpdating())
	{
	  IndexType linkedIndex;
	  linkedIndex[0] = clickedIndex[0]+(*offIt)[0];
	  linkedIndex[1] = clickedIndex[1]+(*offIt)[1];
	  linkedIt.Get()->ChangeFullViewedRegion(linkedIndex);
	}
      ++offIt;
      ++linkedIt;			    
    }
  m_Updating = false;
}

template <class TPixel>
  void 
  ImageViewer<TPixel>
::ChangeZoomViewedRegion(IndexType clickedIndex)
{ 
  m_Updating = true;
  RegionType region = m_ZoomWidget->GetViewedRegion();
  IndexType newIndex;
  newIndex[0]=clickedIndex[0]-region.GetSize()[0]/2;
  newIndex[1]=clickedIndex[1]-region.GetSize()[1]/2;
 
  region.SetIndex(newIndex);
  RegionType newRegion = ComputeConstrainedRegion(region,m_FullWidget->GetViewedRegion());
  m_ZoomWidget->SetZoomUpperLeftCorner(newRegion.GetIndex());
 
  typename ViewerListType::Iterator linkedIt = m_LinkedViewerList->Begin();
  typename OffsetListType::iterator offIt = m_LinkedViewerOffsetList.begin();
  
  while(linkedIt!=m_LinkedViewerList->End()&&offIt!=m_LinkedViewerOffsetList.end())
    {
      if(!linkedIt.Get()->GetUpdating())
	{
	  IndexType linkedIndex;
	  linkedIndex[0] = clickedIndex[0]+(*offIt)[0];
	  linkedIndex[1] = clickedIndex[1]+(*offIt)[1];
	  linkedIt.Get()->ChangeZoomViewedRegion(linkedIndex);
	}
      ++offIt;
      ++linkedIt;			    
    }
  m_Updating = false; 
}

template <class TPixel>
void 
ImageViewer<TPixel>
::Link(Self * viewer, OffsetType offset, bool backwardLinkFlag)
{
  // Search if this viewer is already linked
  typename ViewerListType::Iterator it = m_LinkedViewerList->Begin();
  while(it!=m_LinkedViewerList->End())
    {
      if(it.Get()==viewer)
	{
	  itkGenericExceptionMacro(<<"This viewer is already linked !");
	}
      ++it;
    }
  // If not, add it with its offset
  m_LinkedViewerList->PushBack(viewer);
  m_LinkedViewerOffsetList.push_back(offset);
  
  // If backward link flag is set, add the backward link
  if(backwardLinkFlag)
    {
      OffsetType invertOffset;
      invertOffset[0]=-offset[0];
      invertOffset[1]=-offset[1];
      viewer->Link(this,invertOffset,false);
    }
}

template <class TPixel>
void 
ImageViewer<TPixel>
::Link(Self * viewer, OffsetType offset)
{
  this->Link(viewer,offset,true);
}

template <class TPixel>
void 
ImageViewer<TPixel>
::Link(Self * viewer)
{
  OffsetType offset;
  offset.Fill(0);
  this->Link(viewer,offset,true);
}

template <class TPixel>
void 
ImageViewer<TPixel>
::Unlink(Self * viewer,bool backwardLinkFlag)
{
  unsigned int counter = 0;
  bool found = false;
  // Search the given viewer in the linked list
  typename ViewerListType::Iterator it = m_LinkedViewerList->Begin();
  while(!found&&it!=m_LinkedViewerList->End())
    {
      if(it.Get()==viewer)
	{
	  found = true;
	}
      else
	{
	  ++counter;
	}
      ++it;
    }
  
  // If found, erase
  m_LinkedViewerList->Erase(counter);
  m_LinkedViewerOffsetList.erase(m_LinkedViewerOffsetList.begin()+counter);

  // If backward link flag is set, remove the backward link
  if(backwardLinkFlag)
    {
      viewer->Unlink(this,false);
    }
}
template <class TPixel>
void 
ImageViewer<TPixel>
::Unlink(Self * viewer)
{
  this->Unlink(viewer,true);
}


template <class TPixel>
int
ImageViewer<TPixel>
::IsLinkedTo(Self * viewer)
{
  int counter = 0;
  typename ViewerListType::Iterator it = m_LinkedViewerList->Begin();
  while(it!=m_LinkedViewerList->End())
    {
      if(it.Get()==viewer)
	{
	  return counter;
	}
      ++counter;
      ++it;
    }
  return -1;
}

template<class TPixel>
void
ImageViewer<TPixel>
::ClearLinks(void)
{
  typename ViewerListType::Iterator it = m_LinkedViewerList->Begin();
  while(it!=m_LinkedViewerList->End())
    {
      it.Get()->Unlink(this,false);
      ++it;
    }
  m_LinkedViewerList->Clear();
  m_LinkedViewerOffsetList.clear();
}

template<class TPixel>
typename ImageViewer<TPixel>
::OffsetType
ImageViewer<TPixel>
::GetOffset(int index)
{
  return m_LinkedViewerOffsetList[index];
}

template<class TPixel>
bool
ImageViewer<TPixel>
::GetViewModelIsRGB()
{
  return m_FullWidget->GetViewModelIsRGB();
}

template<class TPixel>
bool
ImageViewer<TPixel>
::IsRGBViewModelAllowed()
{
  return (m_InputImage->GetNumberOfComponentsPerPixel()>2);
}

template<class TPixel>
void
ImageViewer<TPixel>
::SetViewModelIsRGB(bool flag)
{
  if(flag)
    {
      if(IsRGBViewModelAllowed())
	{
	  if(m_UseScroll)
	    {
	      m_ScrollWidget->SetViewModelToRGB();
	      m_ScrollWidget->SetRedChannelIndex(m_RedChannelIndex);
	      m_ScrollWidget->SetGreenChannelIndex(m_GreenChannelIndex);
	      m_ScrollWidget->SetBlueChannelIndex(m_BlueChannelIndex);
	    }
	  m_FullWidget->SetViewModelToRGB();
	  m_ZoomWidget->SetViewModelToRGB();
	  m_ZoomWidget->SetRedChannelIndex(m_RedChannelIndex);
	  m_ZoomWidget->SetGreenChannelIndex(m_GreenChannelIndex);
	  m_ZoomWidget->SetBlueChannelIndex(m_BlueChannelIndex);
	  m_FullWidget->SetRedChannelIndex(m_RedChannelIndex);
	  m_FullWidget->SetGreenChannelIndex(m_GreenChannelIndex);
	  m_FullWidget->SetBlueChannelIndex(m_BlueChannelIndex);
	}
    }
  else
    {
	  if(m_UseScroll)
	    {
	      m_ScrollWidget->SetViewModelToGrayscale();
	      m_ScrollWidget->SetRedChannelIndex(m_RedChannelIndex);
	    }
	  m_FullWidget->SetViewModelToGrayscale();
	  m_ZoomWidget->SetViewModelToGrayscale();
	  m_ZoomWidget->SetRedChannelIndex(m_RedChannelIndex);
	  m_FullWidget->SetRedChannelIndex(m_RedChannelIndex);
    }
}

template<class TPixel>
void
ImageViewer<TPixel>
::InitializeViewModel(void)
{
   if(this->IsRGBViewModelAllowed())
       {
	 this->SetViewModelIsRGB(true);
       }
     else
       {
	 this->SetViewModelIsRGB(false);
       }
}

template<class TPixel>
void
ImageViewer<TPixel>
::Reset(void)
{
  m_FullWidget->Reset();
  m_ZoomWidget->Reset();
  if(m_UseScroll)
    {
      m_ScrollWidget->Reset();
    }
}

} // end namespace otb
#endif

