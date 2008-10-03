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
#ifndef __otbImageAlternateViewer_txx
#define __otbImageAlternateViewer_txx

#include "otbImageAlternateViewer.h"
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include "otbMath.h"
#include "itkTimeProbe.h"
#include "itkMacro.h"

namespace otb
{
  /**
   * Constructor
   */
  template <class TPixel>
  ImageAlternateViewer<TPixel>
  ::ImageAlternateViewer()
    : Fl_Gl_Window(0,0,0,0,0)
  {
    m_Image = NULL;
    m_OpenGlIsotropicZoom = 1.0;
    m_OpenGlBuffer = NULL;
    m_ViewModelIsRGB=true;
    m_InsightViewModelIsRGB =true;
    m_RedChannelIndex = 0;
    m_GreenChannelIndex = 1;
    m_BlueChannelIndex = 2;

    m_MinComponentValues.SetSize(1);
    m_MaxComponentValues.SetSize(1);
    m_MinComponentValues.Fill(0);
    m_MaxComponentValues.Fill(255);

    m_DecompositionFilter = VectorImageDecompositionFilterType::New();

     typename DefaultInterpolatorType::Pointer defaultInterpolator = DefaultInterpolatorType::New();
    m_ZoomInInterpolator=defaultInterpolator;
    m_ZoomOutInterpolator = defaultInterpolator;

    m_SpacingZoomFactor=1;
    m_Updating = false;
    m_Drag = false;
    m_DragEventCounter = 0;
    m_OldMousePos.Fill(0);
    m_ZoomState = 0;
    m_SubWindowMode = false;
  }

  /**
   * Destructor
   */
  template <class TPixel>
  ImageAlternateViewer<TPixel>
  ::~ImageAlternateViewer()
  {
    if(m_OpenGlBuffer != NULL)
      {
	delete [] m_OpenGlBuffer;
      }
  }

  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::SetViewedRegionCenter(const IndexType & index)
  {
    m_OldViewedRegionCenter = m_ViewedRegionCenter;
    m_ViewedRegionCenter = index;
  }

  /**
   * Reset the viewer
   */
  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::Reset(void)
  {
    const char * label = this->label();
    Init(this->x(),this->y(),this->w(),this->h(),label);
  }

  
  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::Init(int x, int y, int w, int h, const char * l)
  {    
    IndexType index; 
    SizeType size;

    index[0]=0;
    index[1]=0;
    size[0]=w;
    size[1]=h;

    SizeType nullSize;
    nullSize.Fill(0);

    m_RequestedRegion.SetSize(nullSize);
    m_RequestedRegion.SetIndex(index);
    m_BufferedRegion=m_RequestedRegion;
    m_OldBufferedRegion=m_BufferedRegion;

    RegionType newRegion;
    m_DisplayExtent.SetIndex(index);
    m_DisplayExtent.SetSize(size);
    m_OldDisplayExtent=m_DisplayExtent;

    
    
    m_OldSpacingZoomFactor=m_SpacingZoomFactor;

    m_Splitter=SplitterType::New();
   
    m_SubWindowRegion.SetIndex(index);
    m_SubWindowRegion.SetSize(nullSize);

    m_Image->UpdateOutputInformation();
    m_DecompositionFilter->SetInput(m_Image);
    m_OldViewedRegionCenter = m_ViewedRegionCenter;

    typename ImageListType::Pointer bandList = m_DecompositionFilter->GetOutput() ;
    bandList->UpdateOutputInformation();
    bandList->GetNthElement(m_RedChannelIndex)->SetRequestedRegion(m_RequestedRegion);
    
    if(m_ViewModelIsRGB)
      {
	bandList->GetNthElement(m_GreenChannelIndex)->SetRequestedRegion(m_RequestedRegion);
	bandList->GetNthElement(m_BlueChannelIndex)->SetRequestedRegion(m_RequestedRegion);
      }
    bandList->PropagateRequestedRegion();

    if(!this->GetImage())
      {
	itkExceptionMacro("No input image!");
      }
    this->label(l);
    this->resize(x, 
		 y, 
		 m_DisplayExtent.GetSize()[0], 
		 m_DisplayExtent.GetSize()[1]);
  }
  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::resize(int x, int y, int w, int h)
  {
    if(m_Updating)
      return;
    // avoid odd sizes
    if(w%2==1)
      w+=1;
    if(h%2==1)
      h+=1;

    IndexType index,subWindowIndex;
    SizeType size,subWindowSize;
    m_OldDisplayExtent=m_DisplayExtent;
    index[0]=0;
    index[1]=0;
    size[0]=w;
    size[1]=h; 

    subWindowIndex[0] = 3*static_cast<long>(size[0])/8;
    subWindowIndex[1] = 3*static_cast<long>(size[1])/8;
    subWindowSize[0] = size[0]/4;
    subWindowSize[1] = size[1]/4;

    m_SubWindowRegion.SetIndex(subWindowIndex);
    m_SubWindowRegion.SetSize(subWindowSize);

    m_DisplayExtent.SetIndex(index);
    m_DisplayExtent.SetSize(size);

    
    //std::cout<<"New display extent: "<<m_DisplayExtent<<std::endl;
    this->Fl_Gl_Window::resize(x,y,w,h);
    this->redraw();
  }
  /**
   * Set view mode to RGB.
   */
  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::SetViewModelToRGB(void)
  {
    m_ViewModelIsRGB=true;
  }
  /**
   * Set view mode to Grayscale.
   */
  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::SetViewModelToGrayscale(void)
  {
    m_ViewModelIsRGB=false;
  }

 /**
   * Set view mode to RGB.
   */
  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::SetInsightViewModelToRGB(void)
  {
    m_InsightViewModelIsRGB=true;
  }
  /**
   * Set view mode to Grayscale.
   */
  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::SetInsightViewModelToGrayscale(void)
  {
    m_InsightViewModelIsRGB=false;
  }

  /** 
   * Show The widget. 
   */
  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::Show(void)
  { 
    if(!m_Image)
      {
	itkExceptionMacro(<<"No input image !");
      }
    else
      {
	this->show();
	this->redraw();
      }
  }

  /** 
   * Draw the widget 
   */
  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::draw(void)
  {
    if(!m_Updating)
      {
	m_Updating = true;
	IncrementalOpenGlBufferUpdate();
	ResetOpenGlContext();
	this->Draw(m_OpenGlBuffer,m_BufferedRegion);
	//std::cout<<"Buffered region: "<<m_BufferedRegion<<std::endl;
	
	if(!m_Drag)
	  {
	    AdditionalRedraw();
	  }
	m_Updating = false;
      }
  }


  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::AdditionalRedraw(void)
  {
    std::vector<unsigned char *> bufferList;
    std::vector<RegionType> bufferRegionList;
     
    if(m_BufferedRegion!=m_DisplayExtent)
      {
	for(unsigned int i = 0;i<8;++i)
	  {
	    RegionType additionalBufferRegion = GetAdditionalBufferRegion(i);
	    //std::cout<<"Additional region required: "<<additionalBufferRegion<<std::endl;
	    unsigned char * additionalBuffer = CreateAdditionalBuffer(additionalBufferRegion,m_Image,m_ViewModelIsRGB);
	    this->Draw(additionalBuffer,additionalBufferRegion);
	    bufferList.push_back(additionalBuffer);
	    bufferRegionList.push_back(additionalBufferRegion);
	  }
	DecorationRedraw();
	MergeBuffersAndFreeMemory(bufferList,bufferRegionList);
      }
    if(m_SubWindowMode)
      {
	unsigned char * subWindowBuffer = CreateAdditionalBuffer(m_SubWindowRegion,m_SecondImage,m_InsightViewModelIsRGB);
	this->Draw(subWindowBuffer,m_SubWindowRegion);
	delete [] subWindowBuffer;
      }
    DecorationRedraw();
  }


  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::DecorationRedraw(void)
  {
    if(m_SubWindowMode)
      {
	this->DrawRegionBoundary(m_SubWindowRegion);
      }
    itk::OStringStream oss;
    oss<<"Zoom: "<<m_OpenGlIsotropicZoom<<", scale: "<<m_SpacingZoomFactor;
    gl_color(FL_RED);
    gl_font(FL_SCREEN_BOLD,10);
    gl_draw(oss.str().c_str(),static_cast<int>(m_DisplayExtent.GetIndex()[0])+10,static_cast<int>(m_DisplayExtent.GetIndex()[1])+10);
    swap_buffers();
    glFlush();
  }

  template <class TPixel>
  long 
  ImageAlternateViewer<TPixel>
  ::IndexInOldGrid(PointType point, PointType oldUpperLeft, SpacingType spacing, SizeType size)
  {
    long resp;
    double x = (point[0]-oldUpperLeft[0])/spacing[0];
    double y = (point[1]-oldUpperLeft[1])/spacing[1];

   //std::cout<<"x: "<<x<<", y: "<<y<<std::endl;
    
    if ((vcl_floor(x)!=x)||(vcl_floor(y)!=y))
      {
	resp=-1;
      }
    else if(x<0||x>size[0]||y<0||y>size[1])
      {
	resp = -1;
      }
    else 
      {
	resp = 3*(static_cast<long>(y)*size[0]+static_cast<long>(x));
      }

    return resp;
  }


  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::IncrementalOpenGlBufferUpdate()
  {
    IndexType focusOffset;
    focusOffset[0]=static_cast<long>(static_cast<double>(m_ViewedRegionCenter[0]-m_OldViewedRegionCenter[0])/m_SpacingZoomFactor);
    focusOffset[1]=static_cast<long>(static_cast<double>(m_ViewedRegionCenter[1]-m_OldViewedRegionCenter[1])/m_SpacingZoomFactor);

    //std::cout<<"Focus offset: "<<focusOffset<<std::endl;
    
    IndexType newBufferedRegionIndex;

    SizeType newBufferedRegionSize;

    newBufferedRegionSize[0]=static_cast<unsigned long>(static_cast<double>((m_BufferedRegion.GetSize()[0])*m_OldSpacingZoomFactor/m_SpacingZoomFactor));
    newBufferedRegionSize[1]=static_cast<unsigned long>(static_cast<double>((m_BufferedRegion.GetSize()[1])*m_OldSpacingZoomFactor/m_SpacingZoomFactor));

    m_OldBufferedRegion = m_BufferedRegion;

    m_BufferedRegion.SetSize(newBufferedRegionSize);

    newBufferedRegionIndex[0]=(static_cast<long>(m_DisplayExtent.GetSize()[0])
			       -static_cast<long>(m_BufferedRegion.GetSize()[0]))/2;
    newBufferedRegionIndex[1]=(static_cast<long>(m_DisplayExtent.GetSize()[1])
			       -static_cast<long>(m_BufferedRegion.GetSize()[1]))/2;
    newBufferedRegionIndex[0]-=focusOffset[0];
    newBufferedRegionIndex[1]-=focusOffset[1];
    //std::cout<<"NewBufferedRegionIndex: "<<newBufferedRegionIndex<<std::endl;
    m_BufferedRegion.SetIndex(newBufferedRegionIndex);
    //m_OldViewedRegionCenter = m_ViewedRegionCenter;
    
    //std::cout<<"New buffered region: "<<m_BufferedRegion<<std::endl;
    
    PointType center;
    m_Image->TransformIndexToPhysicalPoint(m_ViewedRegionCenter,center);
    //std::cout<<"Center index: "<<m_ViewedRegionCenter<<std::endl;
    //std::cout<<"Center: "<<center<<std::endl;

     if(m_SpacingZoomFactor != m_OldSpacingZoomFactor)
       {
	m_BufferedRegion.Crop(m_DisplayExtent);
 	//std::cout<<"New buffered region2 "<<m_BufferedRegion<<std::endl;


	SpacingType spacing = m_Image->GetSpacing()*m_SpacingZoomFactor;
	SpacingType oldSpacing = m_Image->GetSpacing()*m_OldSpacingZoomFactor;
	
	PointType origin;
	origin[0]=center[0]-(static_cast<double>(this->m_DisplayExtent.GetSize()[0])/2-1)*spacing[0];
	origin[1]=center[1]-(static_cast<double>(this->m_DisplayExtent.GetSize()[1])/2-1)*spacing[1];
	
	PointType oldOrigin;
	oldOrigin[0]=center[0]-(static_cast<double>(this->m_DisplayExtent.GetSize()[0])/2-1)*oldSpacing[0];
	oldOrigin[1]=center[1]-(static_cast<double>(this->m_DisplayExtent.GetSize()[1])/2-1)*oldSpacing[1];
		
	PointType oldBufferedUpperLeft;
	oldBufferedUpperLeft[0]=oldOrigin[0]+static_cast<double>(m_OldBufferedRegion.GetIndex()[0])*oldSpacing[0];
	oldBufferedUpperLeft[1]=oldOrigin[1]+static_cast<double>(m_OldBufferedRegion.GetIndex()[1])*oldSpacing[1];

	PointType bufferedUpperLeft;
	bufferedUpperLeft[0]=origin[0]+static_cast<double>(m_BufferedRegion.GetIndex()[0])*spacing[0];
	bufferedUpperLeft[1]=origin[1]+static_cast<double>(m_BufferedRegion.GetIndex()[1])*spacing[1];

	//std::cout<<"OldBufferedRegion: "<<m_OldBufferedRegion<<std::endl;
	//std::cout<<"DisplayExtent: "<<m_DisplayExtent<<std::endl;
	//std::cout<<"Center: "<<center<<std::endl;
	//std::cout<<"Spacing: "<<spacing<<std::endl;
	//std::cout<<"OldSpacing: "<<oldSpacing<<std::endl;
	//std::cout<<"BufferedUpperLeft: "<<bufferedUpperLeft<<std::endl;
	//std::cout<<"oldBufferedUpperLeft: "<<oldBufferedUpperLeft<<std::endl;
	

	unsigned char *  newBuffer = NULL;
	unsigned int bufferLenght = 3*m_BufferedRegion.GetSize()[0]*m_BufferedRegion.GetSize()[1];
   
	newBuffer = new unsigned char[bufferLenght];
	typename ImageListType::Pointer bandList;
	unsigned int index = 0;
	PixelType interpolatedValue = 0;
	PointType interpolatedPos;
	interpolatedPos.Fill(0);
	unsigned int numberOfSplits=1;
	
	unsigned int optiCount = 0;
	InterpolatorPointerType interpolator;
	if(m_SpacingZoomFactor>1)
	  {
	    numberOfSplits=max((static_cast<unsigned int>(m_SpacingZoomFactor))*(static_cast<unsigned int>(m_SpacingZoomFactor)),1U);
	    interpolator = m_ZoomOutInterpolator;
	  }
	else
	  {
	    interpolator = m_ZoomInInterpolator;
	  }
	
	unsigned int splitterNumberOfSplits = m_Splitter->GetNumberOfSplits(m_BufferedRegion,numberOfSplits);

	//std::cout<<"numberOfSplits: "<<numberOfSplits<<std::endl;
	//std::cout<<"splitterNumberOfSplits: "<<splitterNumberOfSplits<<std::endl;
	
	for(unsigned int splitIndex = 0;splitIndex<splitterNumberOfSplits;++splitIndex)
	  {
	    RegionType splitRegion = m_Splitter->GetSplit(splitIndex,splitterNumberOfSplits,m_BufferedRegion);
	    m_RequestedRegion = ComputeRequestedRegion(splitRegion);
	    if(! m_RequestedRegion.Crop(m_Image->GetLargestPossibleRegion()))
	      {
		SizeType nullSize;
		nullSize.Fill(0);
		IndexType nullIndex;
		nullIndex.Fill(0);
		m_RequestedRegion.SetSize(nullSize);
		m_RequestedRegion.SetIndex(nullIndex);
	      }	    
	    //std::cout<<"Requested region: "<<m_RequestedRegion<<std::endl;
	    m_DecompositionFilter = VectorImageDecompositionFilterType::New();
	    m_DecompositionFilter->SetInput(m_Image);
	    bandList = m_DecompositionFilter->GetOutput();
	    bandList->UpdateOutputInformation();
	    bandList->GetNthElement(m_RedChannelIndex)->SetRequestedRegion(m_RequestedRegion);
	    if(m_ViewModelIsRGB)
	      {
		bandList->GetNthElement(m_GreenChannelIndex)->SetRequestedRegion(m_RequestedRegion);
		bandList->GetNthElement(m_BlueChannelIndex)->SetRequestedRegion(m_RequestedRegion);
	      }
	    bandList->PropagateRequestedRegion();
	    bandList->UpdateOutputData();
	    

	    PointType upperLeft;
	    upperLeft[0]=origin[0]+static_cast<double>(splitRegion.GetIndex()[0])*spacing[0];
	    upperLeft[1]=origin[1]+static_cast<double>(splitRegion.GetIndex()[1])*spacing[1];


	    interpolatedPos[1]=upperLeft[1];
	    for(unsigned int j = 0;j<splitRegion.GetSize()[1];++j)
	      {
		interpolatedPos[0]=upperLeft[0];
		for(unsigned int i = 0;i<splitRegion.GetSize()[0];++i)
		  {
		    
		    long indexInOldBuffer = IndexInOldGrid(interpolatedPos,oldBufferedUpperLeft,oldSpacing,m_OldBufferedRegion.GetSize());
		    if(indexInOldBuffer>0)
		      {
			newBuffer[index] = m_OpenGlBuffer[indexInOldBuffer];
			newBuffer[index+1] = m_OpenGlBuffer[indexInOldBuffer+1];
			newBuffer[index+2] = m_OpenGlBuffer[indexInOldBuffer+2];
			index+=3;
			optiCount++;
		      }
		    else
		      {
			interpolator->SetInputImage(bandList->GetNthElement(m_RedChannelIndex));
			if(interpolator->IsInsideBuffer(interpolatedPos))
			  {
			    interpolatedValue = static_cast<PixelType>(m_ZoomInInterpolator->Evaluate(interpolatedPos));
			  }
			else
			  {
			    interpolatedValue = 0;
			  }
			newBuffer[index] = Normalize(interpolatedValue,m_RedChannelIndex);
			if(m_ViewModelIsRGB)
			  {
			    m_ZoomInInterpolator->SetInputImage(bandList->GetNthElement(m_GreenChannelIndex));
			    if( m_ZoomInInterpolator->IsInsideBuffer(interpolatedPos))
			      {
				interpolatedValue = static_cast<PixelType>(interpolator->Evaluate(interpolatedPos));
			      }
			    else
			      {
				interpolatedValue = 0;
			      }
			    newBuffer[index+1] = Normalize(interpolatedValue,m_GreenChannelIndex);
			    interpolator->SetInputImage(bandList->GetNthElement(m_BlueChannelIndex));
			    if( interpolator->IsInsideBuffer(interpolatedPos))
			      {
				interpolatedValue = static_cast<PixelType>(interpolator->Evaluate(interpolatedPos));
			      }
			    else
			      {
				interpolatedValue = 0;
			      }
			    newBuffer[index+2] = Normalize(interpolatedValue,m_BlueChannelIndex);
			    index+=3;
			  }
			else
			  {
			    newBuffer[index+1] = Normalize(interpolatedValue,m_RedChannelIndex);
			    newBuffer[index+2] = Normalize(interpolatedValue,m_RedChannelIndex);
			    index+=3; 
			  }
 		      }
		    interpolatedPos[0] +=spacing[0];
		  }
		interpolatedPos[1] +=spacing[1];
	      }
	  }
	if(m_OpenGlBuffer!=NULL)
	  {
	    delete [] m_OpenGlBuffer;
	  }
	m_OpenGlBuffer = newBuffer;

	m_OldSpacingZoomFactor = m_SpacingZoomFactor;
  }
	//std::cout<<"Optimisation called "<<optiCount<<" times over "<<m_BufferedRegion.GetNumberOfPixels()<<std::endl;
   
     //std::cout<<"BufferedRegion: "<<m_BufferedRegion<<std::endl;
     //std::cout<<"OldBufferedRegion: "<<m_OldBufferedRegion<<std::endl; 
}
  
  template <class TPixel>
  typename ImageAlternateViewer<TPixel>
  ::RegionType
  ImageAlternateViewer<TPixel>
  ::GetAdditionalBufferRegion(unsigned int part)
  {
    RegionType region;
    SizeType size;
    IndexType index;

    size.Fill(0);
    index.Fill(0);
    
    SizeType deSize = m_DisplayExtent.GetSize();
    IndexType deUL =m_DisplayExtent.GetIndex();
    IndexType deLR;
    deLR[0] = deSize[0] + deUL[0];
    deLR[1] = deSize[1] + deUL[1];

    SizeType bufSize = m_BufferedRegion.GetSize();
    IndexType bufUL = m_BufferedRegion.GetIndex();
    IndexType bufLR;
    bufLR[0] = bufUL[0] + bufSize[0];
    bufLR[1] = bufUL[1] + bufSize[1];

    switch(part)
      {
      case 0:
	index   = deUL;
	size[0] = max(bufUL[0]-deUL[0],0L);
	size[1] = max(bufUL[1]-deUL[1],0L);
	break;

      case 1:
	index[0] = max(deUL[0],bufUL[0]);
	index[1] = deUL[1];
	size[0]  = min(bufLR[0]-max(bufUL[0],0L),deLR[0]-index[0]);
	size[1]  = max(bufUL[1]-deUL[1],0L);
	break;

      case 2:
	index[0] = min(bufLR[0],deLR[0]);
	index[1] = deUL[1];
	size[0]  = max(deLR[0]-bufLR[0],0L);
	size[1]  = max(bufUL[1]-deUL[1],0L);
	break;

      case 3:
	index[0] = deUL[0];
	index[1] = max(bufUL[1],0L);
	size[0]  = max(bufUL[0]-deUL[0],0L);
	size[1]  = min(bufLR[1]-max(bufUL[1],0L),deLR[1]-index[1]);
	break;
	
      case 4:
	index[0] = min(bufLR[0],deLR[0]);
	index[1] = max(bufUL[1],deUL[0]);
	size[0]  = max(deLR[0]-bufLR[0],0L);
	size[1]  = min(bufLR[1]-max(bufUL[1],0L),deLR[1]-index[1]);
	break;

      case 5:
	index[0] = deUL[0];
	index[1] = min(deLR[1],bufLR[1]);
	size[0]  = max(bufUL[0]-deUL[0],0L);
	size[1]  = max(deLR[1]-bufLR[1],0L);
	break;

      case 6:
	index[0] = max(deUL[0],bufUL[0]);
	index[1] = min(deLR[1],bufLR[1]);
	size[0]  = min(bufLR[0]-max(bufUL[0],0L),deLR[0]-index[0]);
	size[1]  = max(deLR[1]-bufLR[1],0L);
	break;

      case 7:
	index[0] = min(bufLR[0],deLR[0]);
	index[1] = min(deLR[1],bufLR[1]);
	size[0]  = max(deLR[0]-bufLR[0],0L);
	size[1]  = max(deLR[1]-bufLR[1],0L);
      }
    region.SetSize(size);
    region.SetIndex(index);
    //std::cout<<"Region "<<part<<": "<<region<<std::endl;
    return region;
  }

  template <class TPixel>
  typename ImageAlternateViewer<TPixel>
  ::RegionType
  ImageAlternateViewer<TPixel>
  ::ComputeRequestedRegion(RegionType &region)
  {
    RegionType outputRegion;
    
    SpacingType spacing = m_Image->GetSpacing()*m_SpacingZoomFactor;
    
    PointType center;
    m_Image->TransformIndexToPhysicalPoint(m_ViewedRegionCenter,center);
    PointType origin;
    origin[0]=center[0]-(static_cast<double>(this->m_DisplayExtent.GetSize()[0])/2-1)*spacing[0];
    origin[1]=center[1]-(static_cast<double>(this->m_DisplayExtent.GetSize()[1])/2-1)*spacing[1];

    PointType oldOrigin;
    oldOrigin[0]=center[0]-(static_cast<double>(this->m_OldDisplayExtent.GetSize()[0])/2-1)*spacing[0];
    oldOrigin[1]=center[1]-(static_cast<double>(this->m_OldDisplayExtent.GetSize()[1])/2-1)*spacing[1];

    PointType upperLeft;
    upperLeft[0]=origin[0]+(static_cast<double>(region.GetIndex()[0]))*spacing[0];
    upperLeft[1]=origin[1]+(static_cast<double>(region.GetIndex()[1]))*spacing[1];

    PointType lowerRight;
    lowerRight[0]=upperLeft[0]+(static_cast<double>(region.GetSize()[0])-1)*spacing[0];
    lowerRight[1]=upperLeft[1]+(static_cast<double>(region.GetSize()[1])-1)*spacing[1];


    PointType bufferedUpperLeft;
    bufferedUpperLeft[0]=origin[0]+static_cast<double>(m_BufferedRegion.GetIndex()[0])*spacing[0];
    bufferedUpperLeft[1]=origin[1]+static_cast<double>(m_BufferedRegion.GetIndex()[1])*spacing[1];

    PointType bufferedLowerRight;
    bufferedLowerRight[0]=bufferedUpperLeft[0]+static_cast<double>(m_BufferedRegion.GetSize()[0]-1)*spacing[0];
    bufferedLowerRight[1]=bufferedUpperLeft[1]+static_cast<double>(m_BufferedRegion.GetSize()[1]-1)*spacing[1];

    //std::cout<<"UpperLeft: "<<upperLeft<<std::endl;
    //std::cout<<"LowerRight: "<<lowerRight<<std::endl;
    //std::cout<<"BufferedUpperLeft: "<<bufferedUpperLeft<<std::endl;
    //std::cout<<"BufferedLowerRight: "<<bufferedLowerRight<<std::endl;

    IndexType lowerRightIndex;
    IndexType requestedIndex;
    SizeType requestedSize;
    m_Image->TransformPhysicalPointToIndex(upperLeft,requestedIndex);
    m_Image->TransformPhysicalPointToIndex(lowerRight,lowerRightIndex);
    requestedSize[0]=lowerRightIndex[0]-requestedIndex[0]+1;
    requestedSize[1]=lowerRightIndex[1]-requestedIndex[1]+1;

    outputRegion.SetIndex(requestedIndex);
    outputRegion.SetSize(requestedSize);
    outputRegion.PadByRadius(2);

    return outputRegion;

  }


  template <class TPixel>
  unsigned char *
  ImageAlternateViewer<TPixel>
  ::CreateAdditionalBuffer(RegionType region,ImagePointerType image, bool rgb)
  {
    itk::TimeProbe total,filter,interpolation;
    total.Start();
    unsigned char *  result = NULL;

    unsigned int bufferLenght = 3*region.GetSize()[0]*region.GetSize()[1];
   
    if(bufferLenght == 0)
      {
	return result;
      }

    result = new unsigned char[bufferLenght];

    filter.Start();
    typename ImageListType::Pointer bandList = m_DecompositionFilter->GetOutput() ;
    bandList->UpdateOutputInformation();

    //std::cout<<bandList->Size()<<std::endl;

    unsigned int index = 0;
    PixelType interpolatedValue = 0;
    PointType interpolatedPos;
    interpolatedPos.Fill(0);
    unsigned int numberOfSplits=1;

    InterpolatorPointerType interpolator;
    if(m_SpacingZoomFactor>1)
      {
	numberOfSplits=max((static_cast<unsigned int>(m_SpacingZoomFactor))*(static_cast<unsigned int>(m_SpacingZoomFactor)),1U);
	interpolator = m_ZoomOutInterpolator;
      }
    else
      {
	interpolator = m_ZoomInInterpolator;
      }
    
    unsigned int splitterNumberOfSplits = m_Splitter->GetNumberOfSplits(region,numberOfSplits);
    
    //std::cout<<"ZoomState: "<<m_ZoomState<<std::endl;
    //std::cout<<"Number of splits: "<<numberOfSplits<<std::endl;
    //std::cout<<"Zoom out number of splits: "<<splitterNumberOfSplits<<std::endl;
    
    SpacingType spacing = image->GetSpacing()*m_SpacingZoomFactor;

    PointType center;
    image->TransformIndexToPhysicalPoint(m_ViewedRegionCenter,center);
   //std::cout<<"Center(ad): "<<center<<std::endl;
    PointType origin;
    origin[0]=center[0]-(static_cast<double>(this->m_DisplayExtent.GetSize()[0])/2-1)*spacing[0];
    origin[1]=center[1]-(static_cast<double>(this->m_DisplayExtent.GetSize()[1])/2-1)*spacing[1];

    for(unsigned int splitIndex = 0;splitIndex<splitterNumberOfSplits;++splitIndex)
      {
	RegionType splitRegion = m_Splitter->GetSplit(splitIndex,splitterNumberOfSplits,region);

	PointType upperLeft;
	upperLeft[0]=origin[0]+(static_cast<double>(splitRegion.GetIndex()[0]))*spacing[0];
	upperLeft[1]=origin[1]+(static_cast<double>(splitRegion.GetIndex()[1]))*spacing[1];
	//std::cout<<"Loop upper left: "<<upperLeft<<std::endl;
	m_RequestedRegion = ComputeRequestedRegion(splitRegion);
	//std::cout<<"Additional requested region: "<<m_RequestedRegion<<std::endl;
	//std::cout<<"Largest possible region: "<<image->GetLargestPossibleRegion()<<std::endl;
	if(!m_RequestedRegion.Crop(image->GetLargestPossibleRegion()))
	  {
	    SizeType nullSize;
	    nullSize.Fill(0);
	    IndexType nullIndex;
	    nullIndex.Fill(0);
	    m_RequestedRegion.SetSize(nullSize);
	    m_RequestedRegion.SetIndex(nullIndex);
	  }
	//std::cout<<"Additional cropped requested region: "<<m_RequestedRegion<<std::endl;
	
	// 	if(!m_RequestedRegion.IsInside(bandList->GetNthElement(m_RedChannelIndex)->GetBufferedRegion()))
	// 	  {
	m_DecompositionFilter = VectorImageDecompositionFilterType::New();
	m_DecompositionFilter->SetInput(image);
	bandList = m_DecompositionFilter->GetOutput();
	
	bandList->UpdateOutputInformation();
	bandList->GetNthElement(m_RedChannelIndex)->SetRequestedRegion(m_RequestedRegion);
	if(rgb)
	  {
	    bandList->GetNthElement(m_GreenChannelIndex)->SetRequestedRegion(m_RequestedRegion);
	    bandList->GetNthElement(m_BlueChannelIndex)->SetRequestedRegion(m_RequestedRegion);
	  }
	bandList->PropagateRequestedRegion();
	bandList->UpdateOutputData();

	filter.Stop();
	// 	  }

	interpolation.Start();
	interpolatedPos[1]=upperLeft[1];
	for(unsigned int j = 0;j<splitRegion.GetSize()[1];++j)
	  {
	    interpolatedPos[0]=upperLeft[0];
	    for(unsigned int i = 0;i<splitRegion.GetSize()[0];++i)
	      {
		// //std::cout<<interpolatedPos<<std::endl;
		interpolator->SetInputImage(bandList->GetNthElement(m_RedChannelIndex));
		if( interpolator->IsInsideBuffer(interpolatedPos))
		  {
		    interpolatedValue = static_cast<PixelType>(interpolator->Evaluate(interpolatedPos));
		  }
		else
		  {
		    interpolatedValue = 0;
		  }
		result[index] = Normalize(interpolatedValue,m_RedChannelIndex);
		if(rgb)
		  {
		    interpolator->SetInputImage(bandList->GetNthElement(m_GreenChannelIndex));
		    if( interpolator->IsInsideBuffer(interpolatedPos))
		      {
			interpolatedValue = static_cast<PixelType>( interpolator->Evaluate(interpolatedPos));
		      }
		    else
		      {
			interpolatedValue = 0;
		      }
		    result[index+1] = Normalize(interpolatedValue,m_GreenChannelIndex);
		    interpolator->SetInputImage(bandList->GetNthElement(m_BlueChannelIndex));
		    if( interpolator->IsInsideBuffer(interpolatedPos))
		      {
			interpolatedValue = static_cast<PixelType>(interpolator->Evaluate(interpolatedPos));
		      }
		    else
		      {
			interpolatedValue = 0;
		      }
		    result[index+2] = Normalize(interpolatedValue,m_BlueChannelIndex);
		    index+=3;
		  }
		else
		  {
		    result[index+1] = Normalize(interpolatedValue,m_RedChannelIndex);
		    result[index+2] = Normalize(interpolatedValue,m_RedChannelIndex);
		    index+=3; 
		  }
		interpolatedPos[0] +=spacing[0];
	      }
	    interpolatedPos[1] +=spacing[1];
	  }
      }




    //  std::cout<<"Last interpolated position: "<<interpolatedPos<<std::endl;
    //     std::cout<<"Additional buffer built"<<std::endl;
    interpolation.Stop();
    total.Stop();
    //  std::cout<<"Profiling:\t"<<std::endl;
    //     std::cout<<"- Total:\t"<<total.GetMeanTime()<<std::endl;
    //     std::cout<<"- Filtering:\t\t"<<filter.GetMeanTime()<<"\t"<<filter.GetMeanTime()/total.GetMeanTime()*100<<" %"<<std::endl;
    //     std::cout<<"- Interpolation:\t"<<interpolation.GetMeanTime()<<"\t\t"<<interpolation.GetMeanTime()/total.GetMeanTime()*100<<" %"<<std::endl;
    return result;
  }
  
  template <class TPixel>
  int  
  ImageAlternateViewer<TPixel>
  ::handle(int event)
  {
    if(m_Updating)
      return 0;
    switch(event)
      {
      case FL_PUSH:
	{
	  if(!m_Drag)
	    {
	      m_OldMousePos[0]= static_cast<long int>(static_cast<double>(m_DisplayExtent.GetSize()[0]/2)
						      +(static_cast<double>(Fl::event_x())-static_cast<double>(m_DisplayExtent.GetSize()[0]/2))/m_OpenGlIsotropicZoom);
	      m_OldMousePos[1]= static_cast<long int>(static_cast<double>(m_DisplayExtent.GetSize()[1]/2)
						      +(static_cast<double>(Fl::event_y())-static_cast<double>(m_DisplayExtent.GetSize()[1]/2))/m_OpenGlIsotropicZoom);
	      //  m_OldMousePos[1]= Fl::event_y();
  	      m_Drag=true;
	      m_DragEventCounter=0;
	      
	      if(m_SubWindowRegion.IsInside(m_OldMousePos))
		{
		  m_SubWindowMove = true;
		}
	      else
		{
		  m_OldViewedRegionCenter = m_ViewedRegionCenter;
		}
	    }
 	  return 1;  
 	}

      case FL_DRAG:
	{
	  //std::cout<<"FL_DRAG"<<std::endl;
 	  m_Drag=true;
	  
	  int x =static_cast<int>(static_cast<double>(m_DisplayExtent.GetSize()[0]/2)
				  +(Fl::event_x()-static_cast<double>(m_DisplayExtent.GetSize()[0]/2))/m_OpenGlIsotropicZoom);
	  int y = static_cast<long int>(static_cast<double>(m_DisplayExtent.GetSize()[1]/2)
					+(Fl::event_y()-static_cast<double>(m_DisplayExtent.GetSize()[1]/2))/m_OpenGlIsotropicZoom);
	  if(Fl::event_button()==FL_MIDDLE_MOUSE)
	    {
	      if(!m_SubWindowMode)
		m_SubWindowMode = true;
	      IndexType newIndex;
	      SizeType newSize;

	      newIndex[0]=(x>m_OldMousePos[0] ? m_OldMousePos[0] : x);
	      newIndex[1]=(y>m_OldMousePos[1] ? m_OldMousePos[1] : y);
	      newSize[0]=vcl_abs(x-m_OldMousePos[0]);
	      newSize[1]=vcl_abs(y-m_OldMousePos[1]);
	      m_SubWindowRegion.SetIndex(newIndex);
	      m_SubWindowRegion.SetSize(newSize);
	      //std::cout<<"new subWindowRegion: "<<m_SubWindowRegion<<std::endl;
	      this->redraw();
	      m_DragEventCounter++;
	    }
	  else if(m_SubWindowMove)
	    {
	      IndexType index = m_SubWindowRegion.GetIndex();
	      index[0]+=(x-m_OldMousePos[0]);
	      index[1]+=(y-m_OldMousePos[1]);
	      m_SubWindowRegion.SetIndex(index);
	      m_OldMousePos[0]=x;
	      m_OldMousePos[1]=y;
	      this->redraw();
	      m_DragEventCounter++;
	    }
	  
	  else
	    {
	      SpacingType spacing = m_Image->GetSpacing()*m_SpacingZoomFactor;
	      PointType origin;
	      origin[0]=static_cast<double>(m_OldViewedRegionCenter[0])-static_cast<double>(this->m_DisplayExtent.GetSize()[0]/2)*spacing[0];
	      origin[1]=static_cast<double>(m_OldViewedRegionCenter[1])-static_cast<double>(this->m_DisplayExtent.GetSize()[1]/2)*spacing[1];
	      PointType newCenter;
	      newCenter[0]=origin[0]+static_cast<double>(m_OldMousePos[0]-x+static_cast<long>(this->m_DisplayExtent.GetSize()[0])/2)*spacing[0];
	      newCenter[1]=origin[1]+static_cast<double>(m_OldMousePos[1]-y+static_cast<long>(this->m_DisplayExtent.GetSize()[1])/2)*spacing[1];
	      //std::cout<<"Drag focus offset: "<<m_OldMousePos[0]-x<<", "<<m_OldMousePos[1]-y<<std::endl;
	      m_Image->TransformPhysicalPointToIndex(newCenter,m_ViewedRegionCenter);
	      this->redraw();
	      m_DragEventCounter++;
	    }
	  
	  DecorationRedraw();
	  return 1;
	}

      case FL_RELEASE:
	{
	  m_OldViewedRegionCenter = m_ViewedRegionCenter;
	  m_Drag=false;
	  AdditionalRedraw();
	  m_SubWindowMove = false;
	  return 1;
	}

      case FL_MOUSEWHEEL:
	{
	  int dy = Fl::event_dy();
	  m_OldSpacingZoomFactor = m_SpacingZoomFactor;
	  m_OpenGlIsotropicZoom -=static_cast<double>(dy)/10;
	  if(m_OpenGlIsotropicZoom>2)
	    {
	      m_SpacingZoomFactor/=2;
	      m_OpenGlIsotropicZoom = 1;
	    }
	  else if(m_OpenGlIsotropicZoom<1)
	    {
	      m_SpacingZoomFactor*=2;
	      m_OpenGlIsotropicZoom = 1.9;	      
	    }
	  this->redraw();
	  return 1;
	}
      case FL_FOCUS:
	{
	  return 1;
	}
      case FL_UNFOCUS:
	{
	  return 1;
	}
      case FL_KEYDOWN:
	{
	  if(Fl::event_key()==116) // T kye
	    {
	      m_SubWindowMode = !m_SubWindowMode;
	      this->redraw();
	    }
	  return 1;
	}
      }
    return 0;
  }

  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
 ::MergeBuffersAndFreeMemory(std::vector<unsigned char *> bufferList, std::vector<RegionType> bufferRegionList)
  {
    //std::cout<<"Entering merge method"<<std::endl;
    if(bufferList.size()!=8 || bufferRegionList.size()!=8)
      {
	itkExceptionMacro("Invalid number of additionnal buffers");
      }

    if(bufferRegionList[0].GetNumberOfPixels()==0
       &&bufferRegionList[1].GetNumberOfPixels()==0
       &&bufferRegionList[2].GetNumberOfPixels()==0
       &&bufferRegionList[3].GetNumberOfPixels()==0
       &&bufferRegionList[4].GetNumberOfPixels()==0
       &&bufferRegionList[5].GetNumberOfPixels()==0
       &&bufferRegionList[6].GetNumberOfPixels()==0
       &&bufferRegionList[7].GetNumberOfPixels()==0)
      {
	return;
      }

      
    // malloc new buffer 
    unsigned char * newBuffer = new unsigned char[3*m_DisplayExtent.GetNumberOfPixels()];

    // fill the new buffer
    unsigned int indexInNewBuffer = 0;

    
    unsigned int indexInBuffer1=0;
    unsigned int indexInBuffer2=0;
    unsigned int indexInBuffer3=0;
    unsigned int indexInBuffer4=0;
    unsigned int indexInBuffer5=0;
    unsigned int indexInBuffer6=0;
    unsigned int indexInBuffer7=0;
    unsigned int indexInBuffer8=0;
    unsigned int indexInCentralBuffer = 0;

    if(bufferRegionList[0].GetSize()[1]!=bufferRegionList[1].GetSize()[1]
       ||bufferRegionList[2].GetSize()[1]!=bufferRegionList[1].GetSize()[1]
       ||bufferRegionList[2].GetSize()[1]!=bufferRegionList[0].GetSize()[1]
       ||bufferRegionList[0].GetIndex()[1]!=bufferRegionList[1].GetIndex()[1]
       ||bufferRegionList[2].GetIndex()[1]!=bufferRegionList[1].GetIndex()[1]
       ||bufferRegionList[2].GetIndex()[1]!=bufferRegionList[0].GetIndex()[1]
       )
      {
	itkExceptionMacro("Additional buffers misaligned.");
      }


    // Fill region 1

    for(unsigned int j = 0;j<bufferRegionList[0].GetSize()[1];++j)
      {
	if(bufferList[0]!=NULL)
	  {
	    for(unsigned int i = 0; i<3*bufferRegionList[0].GetSize()[0];++i)
	      {
		newBuffer[indexInNewBuffer]=bufferList[0][indexInBuffer1];
		++indexInNewBuffer;
		++indexInBuffer1;
	      }
	  }
	// Fill region 2
	if(bufferList[1]!=NULL)
	  {
	    for(unsigned int i = 0; i<3*bufferRegionList[1].GetSize()[0];++i)
	      {
		newBuffer[indexInNewBuffer]=bufferList[1][indexInBuffer2];
		++indexInNewBuffer;
		++indexInBuffer2;
	      }
	  }
	// Fill region 3
	if(bufferList[2]!=NULL)
	  {
	    for(unsigned int i = 0; i<3*bufferRegionList[2].GetSize()[0];++i)
	      {
		newBuffer[indexInNewBuffer]=bufferList[2][indexInBuffer3];
		++indexInNewBuffer;
		++indexInBuffer3;
	      }
	  }
      }

    unsigned int lineOffset = static_cast<unsigned int>(-min(0L,m_BufferedRegion.GetIndex()[0]));
    unsigned int lineOffsetEnd = static_cast<unsigned int>(-min(0L,static_cast<long>(m_DisplayExtent.GetSize()[0])-m_BufferedRegion.GetIndex()[0]-static_cast<long>(m_BufferedRegion.GetSize()[0])));
    unsigned int columnOffset = static_cast<unsigned int>(-min(0L,m_BufferedRegion.GetIndex()[1]));
    unsigned int offsety= columnOffset*(static_cast<int>(m_BufferedRegion.GetSize()[0]))*3;
    unsigned int offsetx = lineOffset*3;

    indexInCentralBuffer+=offsety;

    //comment std::cout<<"Line offset: "<<lineOffset<<std::endl;
    //comment std::cout<<"Line offset end: "<<lineOffsetEnd<<std::endl;
    //comment std::cout<<"Column offset: "<<columnOffset<<std::endl;
    
    // For each line
    for(unsigned int j = 0;j<bufferRegionList[3].GetSize()[1];++j)
      {
	//Fill line from region 4
	if(bufferList[3]!=NULL)
	  {
	    for(unsigned int i = 0;i<3*bufferRegionList[3].GetSize()[0];++i)
	      {
		newBuffer[indexInNewBuffer]=bufferList[3][indexInBuffer4];
		++indexInNewBuffer;
		++indexInBuffer4;
	      }
	  }
	// Fill line from central region
	if(m_OpenGlBuffer!=NULL)
	  {

	    indexInCentralBuffer+=offsetx;	    
	    for(unsigned int i = 0;i<3*bufferRegionList[1].GetSize()[0];++i)
	      {
		newBuffer[indexInNewBuffer]=m_OpenGlBuffer[indexInCentralBuffer];
		++indexInNewBuffer;
		++indexInCentralBuffer;
	      }
	    indexInCentralBuffer+=lineOffsetEnd*3;
	  }
	// Fill line from region 5
	if(bufferList[4]!=NULL)
	  {
	    for(unsigned int i = 0;i<3*bufferRegionList[4].GetSize()[0];++i)
	      {
		newBuffer[indexInNewBuffer]=bufferList[4][indexInBuffer5];
		++indexInNewBuffer;
		++indexInBuffer5;
	      }
	  }
      }

   

    if(bufferRegionList[5].GetSize()[1]!=bufferRegionList[6].GetSize()[1]
       ||bufferRegionList[6].GetSize()[1]!=bufferRegionList[7].GetSize()[1]
       ||bufferRegionList[7].GetSize()[1]!=bufferRegionList[5].GetSize()[1]
       ||bufferRegionList[5].GetIndex()[1]!=bufferRegionList[6].GetIndex()[1]
       ||bufferRegionList[6].GetIndex()[1]!=bufferRegionList[7].GetIndex()[1]
       ||bufferRegionList[7].GetIndex()[1]!=bufferRegionList[5].GetIndex()[1]
       )
      {
	itkExceptionMacro("Additional buffers misaligned.");
      }


    // Fill region 6

    for(unsigned int j = 0;j<bufferRegionList[5].GetSize()[1];++j)
      {
	if(bufferList[5]!=NULL)
	  {
	    for(unsigned int i = 0; i<3*bufferRegionList[5].GetSize()[0];++i)
	      {
		newBuffer[indexInNewBuffer]=bufferList[5][indexInBuffer6];
		++indexInNewBuffer;
		++indexInBuffer6;
	      }
	  }
	// Fill region 2
	if(bufferList[6]!=NULL)
	  {
	    for(unsigned int i = 0; i<3*bufferRegionList[6].GetSize()[0];++i)
	      {
		newBuffer[indexInNewBuffer]=bufferList[6][indexInBuffer7];
		++indexInNewBuffer;
		++indexInBuffer7;
	      }
	  }
	// Fill region 3
	if(bufferList[7]!=NULL)
	  {
	    for(unsigned int i = 0; i<3*bufferRegionList[7].GetSize()[0];++i)
	      {
		newBuffer[indexInNewBuffer]=bufferList[7][indexInBuffer8];
		++indexInNewBuffer;
		++indexInBuffer8;
	      }
	  }
      }
    //std::cout<<"New Buffer size: "<<4*m_DisplayExtent.GetNumberOfPixels()<<", last processed index: "<<indexInNewBuffer-1<<std::endl;


    // Free all intermediate buffers
    typename std::vector<unsigned char *>::iterator it;    
    for(it=bufferList.begin();it!=bufferList.end();++it)
      {
	if((*it)!=NULL)
	  {
	    delete [] (*it);
	  };
      }

    // update buffered region
    m_OldBufferedRegion = m_BufferedRegion;
    m_BufferedRegion=m_DisplayExtent;
    // delete previous buffer
    if (m_OpenGlBuffer!=NULL)
      {
	delete [] m_OpenGlBuffer;
      }
    // replace by current buffer
    m_OpenGlBuffer = newBuffer;

    
    //std::cout<<"Buffers merged and freed"<<std::endl;
  }
  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::Draw(unsigned char * buffer, RegionType& region)
  {
    itk::TimeProbe total;
    total.Start();
    if(buffer!=NULL)
      {
	// This enable negative raster pos
	glRasterPos3d(0,0,0);
	
	double zoomOffsetX = 0;
	double zoomOffsetY = 0;

	zoomOffsetX = (1-m_OpenGlIsotropicZoom)*(static_cast<double>(m_DisplayExtent.GetSize()[0]/2)-static_cast<double>(region.GetIndex()[0]));
	zoomOffsetY = (1-m_OpenGlIsotropicZoom)*( static_cast<double>(m_DisplayExtent.GetSize()[1]/2)-static_cast<double>(region.GetIndex()[1]));
	
	// std::cout<<zoomOffsetX<<" "<<zoomOffsetY<<std::endl;

	double movex = static_cast<double>(region.GetIndex()[0])+zoomOffsetX;
	double movey = static_cast<double>(m_DisplayExtent.GetSize()[1])-static_cast<double>(region.GetIndex()[1])-zoomOffsetY;
	glBitmap(0,0,0,0,movex,movey,NULL);
	glPixelZoom(m_OpenGlIsotropicZoom,-m_OpenGlIsotropicZoom);


	// display the image
	glDrawPixels(region.GetSize()[0],
		     region.GetSize()[1], 
		     GL_RGB,
		     GL_UNSIGNED_BYTE, 
		     buffer);
	glEnd();
	swap_buffers();
	glFlush();
      }
    total.Stop();
  }


  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::DrawRegionBoundary(RegionType& region)
  {
    double zoomOffsetX = 0;
    double zoomOffsetY = 0;
    
    zoomOffsetX = (1-m_OpenGlIsotropicZoom)*(static_cast<double>(m_DisplayExtent.GetSize()[0]/2)-static_cast<double>(region.GetIndex()[0]));
    zoomOffsetY = (1-m_OpenGlIsotropicZoom)*( static_cast<double>(m_DisplayExtent.GetSize()[1]/2)-static_cast<double>(region.GetIndex()[1]));
    double minx,maxx,miny,maxy;
    
    minx = static_cast<double>(region.GetIndex()[0])/**m_OpenGlIsotropicZoom*/+zoomOffsetX;
    maxx = minx + static_cast<double>(region.GetSize()[0])*m_OpenGlIsotropicZoom;
    miny = static_cast<double>(m_DisplayExtent.GetSize()[1])-static_cast<double>(region.GetIndex()[1])/**m_OpenGlIsotropicZoom*/-zoomOffsetY;
    maxy = miny-static_cast<double>(region.GetSize()[1])*m_OpenGlIsotropicZoom;
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor3f(1,0,0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(minx,miny);
    glVertex2f(minx,maxy);
    glVertex2f(maxx,maxy);
    glVertex2f(maxx,miny);
    glEnd();
    glDisable(GL_BLEND);

  }







  // template <class TPixel>
  // void
  // ImageAlternateViewer<TPixel>
  // ::Draw(unsigned char * buffer, RegionType& region)
  // {
  //   if(buffer!=NULL)
  //     {
  //       glMatrixMode(GL_TEXTURE);
  //       glLoadIdentity();
      
  //      //  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
  // //       glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
  // // //       glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_R,GL_CLAMP);
  // //       glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);    

  //       glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,region.GetSize()[0],region.GetSize()[1],0,GL_RGBA,GL_UNSIGNED_BYTE,buffer);
  //       //gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGBA,region.GetSize()[0],region.GetSize()[1],GL_RGBA,GL_UNSIGNED_BYTE,buffer);
  //       //glEnable(GL_TEXTURE_2D);
  //       glBegin(GL_QUADS);
  //       glTexCoord2f(0.,0.);
  //       glVertex2f(region.GetIndex()[0],region.GetIndex()[1]);
  //       glTexCoord2f(1.,0.);
  //       glVertex2f(region.GetIndex()[0]+region.GetSize()[0],region.GetIndex()[1]);
  //       glTexCoord2f(1.,1.);
  //       glVertex2f(region.GetIndex()[0]+region.GetSize()[0],region.GetIndex()[1]+region.GetSize()[1]);
  //       glTexCoord2f(0.,1.);
  //       glVertex2f(region.GetIndex()[0],region.GetIndex()[1]+region.GetSize()[1]);
  //       glEnd();

  //      //  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  // //       glDisable(GL_TEXTURE_2D);
  //  //      swap_buffers();
  // // 	;
  // //       glFlush();
  //     }
  // }

  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::ResetOpenGlContext(void)
  {
    if (!this->valid())
      {
	valid(1);
 	glLoadIdentity();
 	glViewport(0,0,m_DisplayExtent.GetSize()[0],m_DisplayExtent.GetSize()[1]);
	glClearColor((float)0.0, (float)0.0, (float)0.0, (float)0.0);          
	glShadeModel(GL_SMOOTH);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      }
    
    glClear(GL_COLOR_BUFFER_BIT);    //this clears and paints to black
    glMatrixMode(GL_PROJECTION);
    this->ortho();
    glMatrixMode(GL_MODELVIEW);    //clear previous 3D draw params
    glLoadIdentity();
    glDisable(GL_BLEND);

    //     glEnable(GL_TEXTURE_2D);
    glShadeModel(GL_SMOOTH);
    //     glClearColor(0.0f,0.0f,0.0f,0.5f);
    //     glClearDepth(1.0f);
    //     glEnable(GL_DEPTH_TEST);
    //     glDepthFunc(GL_LEQUAL);
    //     glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

  }


  template <class TPixel>
  unsigned char
  ImageAlternateViewer<TPixel>
  ::Normalize(PixelType value, unsigned int channelIndex)
  {
    PixelType max = 255;
    PixelType min = 0;
    if(channelIndex<m_MaxComponentValues.GetSize())
      {
	max = m_MaxComponentValues[channelIndex];
      }
    if(channelIndex<m_MinComponentValues.GetSize())
      {
	min = m_MinComponentValues[channelIndex];
      }
    if(value>=max)
      {
	return 255;
      }

    else if(value<=min)
      {
	return 0;
      }
    else
      {
	return static_cast<unsigned char>(255.*static_cast<double>(value-min)
					  /static_cast<double>(max-min));
      }
  }
  /**
   * PrintSelf Method
   */
  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }
} // End namespace otb
#endif
