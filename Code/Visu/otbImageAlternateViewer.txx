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
#ifndef _otbImageAlternateViewer_txx
#define _otbImageAlternateViewer_txx

#include "otbImageAlternateViewer.h"
#include <Fl/fl_draw.H>
#include <Fl/Fl.h>
#include "otbMath.h"
#include "itkTimeProbe.h"
#include "GL/glu.h"


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
    m_RedChannelIndex = 0;
    m_GreenChannelIndex = 1;
    m_BlueChannelIndex = 2;

    m_MinComponentValues.SetSize(1);
    m_MaxComponentValues.SetSize(1);
    m_MinComponentValues.Fill(0);
    m_MaxComponentValues.Fill(255);

    m_DecompositionFilter = VectorImageDecompositionFilterType::New();

    typename BSplineInterpolatorType::Pointer bsplineInterpolator = BSplineInterpolatorType::New();
    bsplineInterpolator->SetSplineOrder(3);


    m_ZoomInInterpolator = WindowedSincInterpolatorType::New();
    // m_ZoomInInterpolator=DefaultInterpolatorType::New();
    // m_ZoomInInterpolator=bsplineInterpolator;

    m_ZoomOutInterpolator = DefaultInterpolatorType::New();
   
    IndexType index;
    SizeType size;

    index[0]=0;
    index[1]=0;
    size[0]=512;
    size[1]=512;

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

    m_ViewedRegionCenter.Fill(0);
    m_OldViewedRegionCenter = m_ViewedRegionCenter;
    m_SpacingZoomFactor=1;
    m_OldSpacingZoomFactor=m_SpacingZoomFactor;
    m_ZoomState = 0;
    m_Splitter=SplitterType::New();
    m_Updating = false;
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
  /**
   * Reset the viewer
   */
  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::Reset(void)
  {
    const char * label = this->label();
    Init(label);
  }

  
  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::Init(const char * l)
  {    
    m_DecompositionFilter->SetInput(m_Image);
    m_ViewedRegionCenter[0]=m_Image->GetLargestPossibleRegion().GetIndex()[0]
      +m_Image->GetLargestPossibleRegion().GetSize()[0]/2;
    m_ViewedRegionCenter[1]=m_Image->GetLargestPossibleRegion().GetIndex()[1]
      +m_Image->GetLargestPossibleRegion().GetSize()[1]/2;

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
    this->resize(m_DisplayExtent.GetIndex()[0], 
		 m_DisplayExtent.GetIndex()[1], 
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
    m_Updating = true;
    // avoid odd sizes
    if(w%2==1)
      w+=1;
    if(h%2==1)
      h+=1;

    IndexType index;
    SizeType size;
    m_OldDisplayExtent=m_DisplayExtent;
    index[0]=x;
    index[1]=y;
    size[0]=w;
    size[1]=h; 
    m_DisplayExtent.SetIndex(index);
    m_DisplayExtent.SetSize(size);
    //comment std::cout<<"New display extent: "<<m_DisplayExtent<<std::endl;
    this->Fl_Gl_Window::resize(x,y,w,h);
    this->redraw();
    m_Updating = false;
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
    IncrementalOpenGlBufferUpdate();
    ResetOpenGlContext();
    this->Draw(m_OpenGlBuffer,m_BufferedRegion);
    
    std::vector<unsigned char *> bufferList;
    std::vector<RegionType> bufferRegionList;

    if(m_BufferedRegion!=m_DisplayExtent)
      {
	for(unsigned int i = 0;i<8;++i)
	  {
	    RegionType additionalBufferRegion = GetAdditionalBufferRegion(i);
	    //comment std::cout<<"Additional region required: "<<additionalBufferRegion<<std::endl;
	    unsigned char * additionalBuffer = CreateAdditionalBuffer(additionalBufferRegion);
	    this->Draw(additionalBuffer,additionalBufferRegion);
	    bufferList.push_back(additionalBuffer);
	    bufferRegionList.push_back(additionalBufferRegion);
	  }
	MergeBuffersAndFreeMemory(bufferList,bufferRegionList);
      }
  }

  template <class TPixel>
  void
  ImageAlternateViewer<TPixel>
  ::IncrementalOpenGlBufferUpdate()
  {
    IndexType focusOffset;
    focusOffset[0]=static_cast<long>(static_cast<double>(m_ViewedRegionCenter[0]-m_OldViewedRegionCenter[0])/m_SpacingZoomFactor);
    focusOffset[1]=static_cast<long>(static_cast<double>(m_ViewedRegionCenter[1]-m_OldViewedRegionCenter[1])/m_SpacingZoomFactor);


    IndexType newBufferedRegionIndex;
    newBufferedRegionIndex[0]=(static_cast<long>(m_DisplayExtent.GetSize()[0])
			       -static_cast<long>(m_BufferedRegion.GetSize()[0]))/2;
    newBufferedRegionIndex[1]=(static_cast<long>(m_DisplayExtent.GetSize()[1])
			       -static_cast<long>(m_BufferedRegion.GetSize()[1]))/2;
    newBufferedRegionIndex[0]-=focusOffset[0];
    newBufferedRegionIndex[1]-=focusOffset[1];
    //std::cout<<"NewBufferedRegionIndex: "<<newBufferedRegionIndex<<std::endl;
    m_BufferedRegion.SetIndex(newBufferedRegionIndex);
    m_OldViewedRegionCenter = m_ViewedRegionCenter;
    
    

    if(m_SpacingZoomFactor != m_OldSpacingZoomFactor)
      {
	SizeType newSize;
	newSize.Fill(0);
	m_BufferedRegion.SetSize(newSize);
	IndexType newIndex;
	newIndex[0]= static_cast<long>(m_DisplayExtent.GetSize()[0])/2;
	newIndex[1]= static_cast<long>(m_DisplayExtent.GetSize()[1])/2;
	m_BufferedRegion.SetIndex(newIndex);
	m_OldSpacingZoomFactor = m_SpacingZoomFactor;
      }
    //comment std::cout<<"New buffered region: "<<m_BufferedRegion<<std::endl;
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

	// case 0:
	// 	index[0]=deIndex[0];
	// 	index[1]=deIndex[1];
	// 	size[0]=max(bufIndex[0]-deIndex[0],0L);
	// 	size[1]=max(bufIndex[1]-deIndex[1],0L);
	// 	break;

	//       case 1:
	// 	index[0]=max(bufIndex[0],0L);
	// 	index[1]=deIndex[1];
	// 	// size[0]=max(0L,static_cast<long>(bufSize[0])-vcl_abs(bufIndex[0]));
	// 	size[0]=max(0L,min(static_cast<long>(bufSize[0])-vcl_abs(bufIndex[0]),static_cast<long>(deSize[0])-index[0]));
	// 	size[1]=max(bufIndex[1]-deIndex[1],0L);
	// 	break;
	
	//       case 2:
	// 	index[0]=bufIndex[0]+bufSize[0];
	// 	index[1]=deIndex[1];
	// 	size[0]=max(deIndex[0]+static_cast<long>(deSize[0])-bufIndex[0]-static_cast<long>(bufSize[0]),0L);
	// 	size[1]=max(bufIndex[1]-deIndex[1],0L);
	// 	break;

	//       case 3:
	// 	index[0]=deIndex[0];
	// 	index[1]=max(bufIndex[1],0L);
	// 	size[0]=max(bufIndex[0]-deIndex[0], 0L);
	// 	// size[1]=max(0L,static_cast<long>(bufSize[1])-vcl_abs(bufIndex[1]));
	// 	size[1]=max(0L,min(static_cast<long>(bufSize[1])-vcl_abs(bufIndex[1]),static_cast<long>(deSize[1])-index[1]));
	
	// 	break;

	//       case 4:
	// 	index[0]=bufIndex[0]+bufSize[0];
	// 	index[1]=max(bufIndex[1],0L);
	// 	size[0]=max(deIndex[0]+static_cast<long>(deSize[0])-bufIndex[0]-static_cast<long>(bufSize[0]),0L);
	// 	// size[1]=max(0L,static_cast<long>(bufSize[1])-vcl_abs(bufIndex[1]));
	// 	size[1]=max(0L,min(static_cast<long>(bufSize[1])-vcl_abs(bufIndex[1]),static_cast<long>(deSize[1])-index[1]));

	// 	break;

	//       case 5:
	// 	index[0]=deIndex[0];
	// 	index[1]=bufIndex[1]+bufSize[1];
	// 	size[0]=max(bufIndex[0]-deIndex[0],0L);
	// 	size[1]=max(deIndex[1]+static_cast<long>(deSize[1])-bufIndex[1]-static_cast<long>(bufSize[1]),0L);
	// 	break;

	//       case 6:
	// 	index[0]=max(bufIndex[0],0L);
	// 	index[1]=bufIndex[1]+bufSize[1];
	// 	// size[0]=max(0L,static_cast<long>(bufSize[0])-vcl_abs(bufIndex[0]));
	// 	size[0]=max(0L,min(static_cast<long>(bufSize[0])-vcl_abs(bufIndex[0]),static_cast<long>(deSize[0])-index[0]));
	// 	size[1]=max(deIndex[1]+static_cast<long>(deSize[1])-bufIndex[1]-static_cast<long>(bufSize[1]),0L);
	// 	break;

	//       case 7:
	// 	index[0]=bufIndex[0]+bufSize[0];
	// 	index[1]=bufIndex[1]+bufSize[1];
	// 	size[0]=max(deIndex[0]+static_cast<long>(deSize[0])-bufIndex[0]-static_cast<long>(bufSize[0]),0L);
	// 	size[1]=max(deIndex[1]+static_cast<long>(deSize[1])-bufIndex[1]-static_cast<long>(bufSize[1]),0L);
	// 	break;

	// size[0]=deSize[0];
	// 	size[1]=(bufIndex[1] > deIndex[1] ? bufIndex[1]-deIndex[1] : 0);
	// 	index[0]=deIndex[0];
	// 	index[1]=deIndex[1];
	// 	break;
	//       case 1:
	
	// 	break;
	//       case 2:
	// 	size[0]=(deIndex[0]+deSize[0] > bufIndex[0]+bufSize[0] ? deIndex[0]+deSize[0]-bufIndex[0]-bufSize[0] : 0);
	// 	size[1]=bufSize[1];
	// 	index[0]=bufIndex[0]+bufSize[0];
	// 	index[1]=max(bufIndex[1],0L);
	// 	break;
	//       case 3:
	// 	size[0]=deSize[0];
	// 	size[1]=(deIndex[1]+deSize[1] > bufIndex[1]+bufSize[1] ?deIndex[1]+deSize[1]-bufIndex[1]-bufSize[1] : 0);
	// 	index[0]=deIndex[0];
	// 	index[1]=bufIndex[1]+bufSize[1];
	// 	break;
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
  ::CreateAdditionalBuffer(RegionType region)
  {
    itk::TimeProbe total,filter,interpolation;
    total.Start();
    unsigned char *  result = NULL;

    unsigned int bufferLenght = 4*region.GetSize()[0]*region.GetSize()[1];
   
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

    if(m_ZoomState>0)
      {
	numberOfSplits=(m_ZoomState+1)*(m_ZoomState+1);
      }

    
    unsigned int splitterNumberOfSplits = m_Splitter->GetNumberOfSplits(region,numberOfSplits);
    
  //   std::cout<<"ZoomState: "<<m_ZoomState<<std::endl;
//     std::cout<<"Number of splits: "<<numberOfSplits<<std::endl;
//     std::cout<<"Zoom out number of splits: "<<splitterNumberOfSplits<<std::endl;
    
    SpacingType spacing = m_Image->GetSpacing()*m_SpacingZoomFactor;

    PointType center;
    m_Image->TransformIndexToPhysicalPoint(m_ViewedRegionCenter,center);
    
    PointType origin;
    origin[0]=center[0]-(static_cast<double>(this->m_DisplayExtent.GetSize()[0])/2-1)*spacing[0];
    origin[1]=center[1]-(static_cast<double>(this->m_DisplayExtent.GetSize()[1])/2-1)*spacing[1];

    for(unsigned int splitIndex = 0;splitIndex<splitterNumberOfSplits;++splitIndex)
      {
	RegionType splitRegion = m_Splitter->GetSplit(splitIndex,splitterNumberOfSplits,region);

	PointType upperLeft;
	upperLeft[0]=origin[0]+(static_cast<double>(splitRegion.GetIndex()[0]))*spacing[0];
	upperLeft[1]=origin[1]+(static_cast<double>(splitRegion.GetIndex()[1]))*spacing[1];

	m_RequestedRegion = ComputeRequestedRegion(splitRegion);
	m_RequestedRegion.Crop(m_Image->GetLargestPossibleRegion());

	
// 	if(!m_RequestedRegion.IsInside(bandList->GetNthElement(m_RedChannelIndex)->GetBufferedRegion()))
// 	  {
	    m_DecompositionFilter = VectorImageDecompositionFilterType::New();
	    m_DecompositionFilter->SetInput(m_Image);
	    bandList = m_DecompositionFilter->GetOutput();
	    //std::cout<<"Requested region: "<<m_RequestedRegion<<std::endl;
	    bandList->UpdateOutputInformation();
	    bandList->GetNthElement(m_RedChannelIndex)->SetRequestedRegion(m_RequestedRegion);
	    if(m_ViewModelIsRGB)
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
		m_ZoomInInterpolator->SetInputImage(bandList->GetNthElement(m_RedChannelIndex));
		if( m_ZoomInInterpolator->IsInsideBuffer(interpolatedPos))
		  {
		    interpolatedValue = static_cast<PixelType>(m_ZoomInInterpolator->Evaluate(interpolatedPos));
		  }
		else
		  {
		    interpolatedValue = 0;
		  }
		result[index] = Normalize(interpolatedValue,m_RedChannelIndex);
		if(m_ViewModelIsRGB)
		  {
		    m_ZoomInInterpolator->SetInputImage(bandList->GetNthElement(m_GreenChannelIndex));
		    if( m_ZoomInInterpolator->IsInsideBuffer(interpolatedPos))
		      {
			interpolatedValue = static_cast<PixelType>( m_ZoomInInterpolator->Evaluate(interpolatedPos));
		      }
		    else
		      {
			interpolatedValue = 0;
		      }
		    result[index+1] = Normalize(interpolatedValue,m_GreenChannelIndex);
		    m_ZoomInInterpolator->SetInputImage(bandList->GetNthElement(m_BlueChannelIndex));
		    if( m_ZoomInInterpolator->IsInsideBuffer(interpolatedPos))
		      {
			interpolatedValue = static_cast<PixelType>(m_ZoomInInterpolator->Evaluate(interpolatedPos));
		      }
		    else
		      {
			interpolatedValue = 0;
		      }
		    result[index+2] = Normalize(interpolatedValue,m_BlueChannelIndex);
		    result[index+3] = 255;
		    index+=4;
		  }
		else
		  {
		    result[index+1] = Normalize(interpolatedValue,m_RedChannelIndex);
		    result[index+2] = Normalize(interpolatedValue,m_RedChannelIndex);
		    result[index+3] = 255;
		    index+=4; 
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
    if(!m_Updating)
      m_Updating=true;
    switch(event)
      {
      case FL_PUSH:
	{
	  int x = Fl::event_x();
	  int y = Fl::event_y();
	  SpacingType spacing = m_Image->GetSpacing()*m_SpacingZoomFactor;
	  PointType origin;
	  origin[0]=static_cast<double>(m_ViewedRegionCenter[0])-static_cast<double>(this->m_DisplayExtent.GetSize()[0]/2)*spacing[0];
	  origin[1]=static_cast<double>(m_ViewedRegionCenter[1])-static_cast<double>(this->m_DisplayExtent.GetSize()[1]/2)*spacing[1];
	    
	  PointType newCenter;
	  newCenter[0]=origin[0]+static_cast<double>(x)*spacing[0];
	  newCenter[1]=origin[1]+static_cast<double>(y)*spacing[1];
	  m_OldViewedRegionCenter = m_ViewedRegionCenter;
	  m_Image->TransformPhysicalPointToIndex(newCenter,m_ViewedRegionCenter);
	  this->redraw();
	  m_Updating=false;
	  return 1;
	}

      case FL_MOUSEWHEEL:
	{
	  int dy = Fl::event_dy();
	  int oldState = m_ZoomState;
	  
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

	  // std::cout<<"Zoom state: "<<m_ZoomState<<std::endl;
// 	  std::cout<<"Old zoom state: "<<oldState<<std::endl;
// 	  std::cout<<"Opengl zoom: "<<m_OpenGlIsotropicZoom<<std::endl;
// 	  std::cout<<"Interpolator zoom factor: "<<m_SpacingZoomFactor<<std::endl;
	  this->redraw();
	   m_Updating=false;
	   return 1;
	}
      }
    m_Updating=false;
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
    unsigned char * newBuffer = new unsigned char[4*m_DisplayExtent.GetNumberOfPixels()];

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
	    for(unsigned int i = 0; i<4*bufferRegionList[0].GetSize()[0];++i)
	      {
		newBuffer[indexInNewBuffer]=bufferList[0][indexInBuffer1];
		++indexInNewBuffer;
		++indexInBuffer1;
	      }
	  }
	// Fill region 2
	if(bufferList[1]!=NULL)
	  {
	    for(unsigned int i = 0; i<4*bufferRegionList[1].GetSize()[0];++i)
	      {
		newBuffer[indexInNewBuffer]=bufferList[1][indexInBuffer2];
		++indexInNewBuffer;
		++indexInBuffer2;
	      }
	  }
	// Fill region 3
	if(bufferList[2]!=NULL)
	  {
	    for(unsigned int i = 0; i<4*bufferRegionList[2].GetSize()[0];++i)
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
    unsigned int offsety= columnOffset*(static_cast<int>(m_BufferedRegion.GetSize()[0]))*4;
    unsigned int offsetx = lineOffset*4;

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
	    for(unsigned int i = 0;i<4*bufferRegionList[3].GetSize()[0];++i)
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
	    for(unsigned int i = 0;i<4*bufferRegionList[1].GetSize()[0];++i)
	      {
		newBuffer[indexInNewBuffer]=m_OpenGlBuffer[indexInCentralBuffer];
		++indexInNewBuffer;
		++indexInCentralBuffer;
	      }
	    indexInCentralBuffer+=lineOffsetEnd*4;
	  }
	// Fill line from region 5
	if(bufferList[4]!=NULL)
	  {
	    for(unsigned int i = 0;i<4*bufferRegionList[4].GetSize()[0];++i)
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
	    for(unsigned int i = 0; i<4*bufferRegionList[5].GetSize()[0];++i)
	      {
		newBuffer[indexInNewBuffer]=bufferList[5][indexInBuffer6];
		++indexInNewBuffer;
		++indexInBuffer6;
	      }
	  }
	// Fill region 2
	if(bufferList[6]!=NULL)
	  {
	    for(unsigned int i = 0; i<4*bufferRegionList[6].GetSize()[0];++i)
	      {
		newBuffer[indexInNewBuffer]=bufferList[6][indexInBuffer7];
		++indexInNewBuffer;
		++indexInBuffer7;
	      }
	  }
	// Fill region 3
	if(bufferList[7]!=NULL)
	  {
	    for(unsigned int i = 0; i<4*bufferRegionList[7].GetSize()[0];++i)
	      {
		newBuffer[indexInNewBuffer]=bufferList[7][indexInBuffer8];
		++indexInNewBuffer;
		++indexInBuffer8;
	      }
	  }
      }
    //comment std::cout<<"New Buffer size: "<<4*m_DisplayExtent.GetNumberOfPixels()<<", last processed index: "<<indexInNewBuffer-1<<std::endl;


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
		     GL_RGBA,
		     GL_UNSIGNED_BYTE, 
		     buffer);
	glEnd();
	swap_buffers();
	glFlush();
      }
    total.Stop();
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
    //     glShadeModel(GL_SMOOTH);
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
