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
#ifndef _otbImageViewerFullWidget_h
#define _otbImageViewerFullWidget_h

#include "otbFullResolutionImageWidget.h"
#include "otbImageWidgetBoxForm.h"
#include "otbImageWidgetRectangleForm.h"
#include "otbImageWidgetPointForm.h"
#include "otbPolygon.h"

namespace otb
{
template <class TPixel> class ImageViewer;
/** 
 * \brief Custom full image widget.
 *
 * This class derives from otb::FullResolutionImageWidget, and implements behaviours specific to the image viewer.
 *
 * \sa ImageViewer, FullResolutionImageWidget
 *
 */
 template <class TPixel>
class ITK_EXPORT ImageViewerFullWidget
  : public FullResolutionImageWidget<TPixel>
{
 public:
  /** Standard class typedefs */
  typedef ImageViewerFullWidget              Self;
  typedef FullResolutionImageWidget<TPixel>  Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageViewerFullWidget, FullResolutionImageWidget);
  
  typedef TPixel PixelType;
  typedef typename Superclass::IndexType IndexType;
  typedef typename Superclass::SizeType  SizeType;
  typedef typename Superclass::ImageType ImageType;
  
  typedef ImageViewer<PixelType>         ParentType;
  typedef ParentType*                    ParentPointerType;
  
  typedef ImageWidgetBoxForm             BoxType;
  typedef ImageWidgetRectangleForm       RectangleType;
  typedef RectangleType::ColorType       ColorType;
  typedef ImageWidgetPointForm           PointType;

  itkSetMacro(Parent,ParentPointerType);
  itkGetMacro(Parent,ParentPointerType);
  /** Handle method */
  virtual int  handle(int event)
    {
      switch(event)
	{
	case FL_PUSH:
	  {
	    int x = Fl::event_x();
	    int y = Fl::event_y();
	    IndexType clickedIndex;
	    clickedIndex[0]=x;
	    clickedIndex[1]=y;
	    if (!m_ShortCutRectangle)
	      {
		clickedIndex=this->WindowToImageCoordinates(clickedIndex);
		m_Parent->ChangeZoomViewedRegion(clickedIndex);
	      }
	    if (!m_Drag)
	      {
		m_LastIndex=this->WindowToImageCoordinates(clickedIndex);
		if (m_ShortCutRectangle)
		  {
		    m_Drag = false;
		  }
	      }
	    return 1;
	  }
	case FL_ENTER:
	  {
	    m_MouseIn = true;
	    return 1;
	  }
	case FL_LEAVE:
	  {
	    m_MouseIn = false;
	    m_Parent->ClearPixLocVal();
	    return 1;
	}
	case FL_MOVE:
	  {
	     if (!m_ShortCutRectangle)
	      {
	    m_MouseIn=true;
	    if(m_MouseMoveCount%m_ValueUpdateFrequency==0)
	      {
		m_MousePos[0]=Fl::event_x();
		m_MousePos[1]=Fl::event_y();
		IndexType newIndex = this->WindowToImageCoordinates(m_MousePos);
		if(this->GetInput()->GetBufferedRegion().IsInside(newIndex))
		  {
		   
		    typename ImageType::PixelType newPixel = this->GetInput()->GetPixel(newIndex);
		    m_Parent->PrintPixLocVal(newIndex,newPixel);
		    m_MouseMoveCount=0;
		  }
	      }
	    m_MouseMoveCount++;
	      }
	    return 1;
	  }
	case FL_DRAG:
	  { 
	    if (m_ShortCutRectangle)
	      {	
		int x = Fl::event_x();
		int y = Fl::event_y();
		IndexType index;
		index[0]=x;
		index[1]=y;
		IndexType clickedIndex=this->WindowToImageCoordinates(index);
		//otbMsgDebugMacro(<<"Mouse dragged: "<<clickedIndex);
		
		 typename BoxType::Pointer box =  BoxType::New();
		 box->SetColor(m_ClassColor);
		 IndexType boxIndex;
		 SizeType boxSize;
		 // compute the size of the select box
		 if(clickedIndex[0]>m_LastIndex[0])
		   {
		     boxIndex[0]=m_LastIndex[0];
		     boxSize[0]=clickedIndex[0]-m_LastIndex[0];
		   }
		 else
		   {
		     boxIndex[0]=clickedIndex[0];
		     boxSize[0]=m_LastIndex[0]-clickedIndex[0];
		   }
		 if(clickedIndex[1]>m_LastIndex[1])
		   {
		     boxIndex[1]=m_LastIndex[1];
		     boxSize[1]=clickedIndex[1]-m_LastIndex[1];
		   }
		 else
		   {
		     boxIndex[1]=clickedIndex[1];
		     boxSize[1]=m_LastIndex[1]-clickedIndex[1];
		   }
		 box->SetIndex(boxIndex);
		 box->SetSize(boxSize);
		 if(m_Drag)
		   {
		     this->GetFormList()->PopBack(); 
		   }
		 this->GetFormList()->PushBack(box);
		 m_Parent->Update();
		 m_Drag=true;
	      }
	    return 1; 
	  }
	case FL_RELEASE:
	  { 
	     if (m_ShortCutRectangle)
	      {	 
		int x = Fl::event_x();
		int y = Fl::event_y();
		IndexType clickedIndex;
		clickedIndex[0]=x;
		clickedIndex[1]=y;
		clickedIndex=this->WindowToImageCoordinates(clickedIndex);
		if(m_Drag)
		  {
		    typename RectangleType::Pointer rectangle = RectangleType::New();
		    IndexType boxIndex;
		    SizeType boxSize;
		    // compute the size of the select box
		    if(clickedIndex[0]>m_LastIndex[0])
		      {
			boxIndex[0]=m_LastIndex[0];
			boxSize[0]=clickedIndex[0]-m_LastIndex[0];
		      }
		    else
		      {
			boxIndex[0]=clickedIndex[0];
			boxSize[0]=m_LastIndex[0]-clickedIndex[0];
		      }
		    if(clickedIndex[1]>m_LastIndex[1])
		      {
			boxIndex[1]=m_LastIndex[1];
			boxSize[1]=clickedIndex[1]-m_LastIndex[1];
		      }
		    else
		      {
			boxIndex[1]=clickedIndex[1];
			boxSize[1]=m_LastIndex[1]-clickedIndex[1];
		      }
		    rectangle->SetIndex(boxIndex);
		    rectangle->SetSize(boxSize);
		    rectangle->SetColor(m_ClassColor);
		    
		    this->GetFormList()->PopBack();
		    //otbMsgDebugMacro(<<"FL_RELEASE: PushBack");
		    this->GetFormList()->PushBack(rectangle);
		  }
		else
		  {
		    typename PointType::Pointer point = PointType::New();
		    point->SetColor(m_ClassColor);
		    point->SetIndex(clickedIndex);
		    //otbMsgDebugMacro(<<"FL_RELEASE: PushBack");
		    this->GetFormList()->PushBack(point);
		  }
		m_Parent->Update();
		m_Drag=false;
	      }
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
	    //IndexType newIndex = this->GetViewedRegion().GetIndex();
	    IndexType newIndex;
	    newIndex[0] = m_Parent->GetZoomWidget()->GetViewedRegion().GetIndex()[0];
	    newIndex[1] = m_Parent->GetZoomWidget()->GetViewedRegion().GetIndex()[1];
	    SizeType newSize  = m_Parent->GetZoomWidget()->GetViewedRegion().GetSize();
	    //newIndex[0] = newIndex[0] + newSize[0]/2;
	    //newIndex[1] = newIndex[1] + newSize[1]/2;
	    switch(Fl::event_key())
	      {
	      case FL_Down:
		{
		  if (!m_ShortCutRectangle)
		    {
		      newIndex[1] = newIndex[1]+static_cast<long int>(newSize[1]/2)+ newSize[1]/2;
		      newIndex[0] += newSize[0]/2;
		    }	
		  break;
		}
	      case FL_Up:
		{
		  if (!m_ShortCutRectangle)
		    {
		      newIndex[1] = newIndex[1]-static_cast<long int>(newSize[1]/2)+ newSize[1]/2;
		      newIndex[0] += newSize[0]/2;
		    }
		  break;
		}
	      case FL_Left:
		{
		  if (!m_ShortCutRectangle)
		    {
		      newIndex[0] = newIndex[0]-static_cast<long int>(newSize[0]/2)+ newSize[0]/2;
		      newIndex[1] += newSize[1]/2;
		    }	
		  break;
		}
	      case FL_Right:
		{
		  if (!m_ShortCutRectangle)
		    {
		      newIndex[0] = newIndex[0]+static_cast<long int>(newSize[0]/2)+ newSize[0]/2;
		      newIndex[1] += newSize[1]/2;
		    }
		  break;
		}
		// RECTANGE SELECTION //////////////////////////////////////////////////
	      case FL_Control_L:
		{
		  m_ShortCutRectangle = !m_ShortCutRectangle;
               
		  break;
		}
	      } 
	    if (!m_ShortCutRectangle)
	      {
		m_Parent->ChangeFullViewedRegion(newIndex);
		m_Parent->ChangeZoomViewedRegion(newIndex);
	      }	    
	    return 1;
	  }
	case FL_HIDE:
	  {
	    m_Parent->Hide();
	    return 0;
	  }
	}
      return 0;
    }   
  
  virtual void resize(int x,int y, int w, int h)
   {
     Superclass::resize(x,y,w,h);
      if(m_Parent->GetBuilt()) 
	m_Parent->UpdateScrollWidget();
    }
  
  itkSetMacro(ClassColor,ColorType);
  itkGetMacro(ClassColor,ColorType);


 protected:
  /**
   * Constructor.
   */
  ImageViewerFullWidget()
    {
      m_MouseIn = false;
      m_MousePos.Fill(0);
      m_MouseMoveCount = 0;
      m_ValueUpdateFrequency = 5;
      m_ClassColor[0]=0.;
      m_ClassColor[1]=1.;
      m_ClassColor[2]=0.;
      m_ClassColor[3]=0.5;
      m_LastIndex.Fill(0);
      m_ShortCutRectangle = false;
      m_Drag = false;
    };
  /**
   * Destructor.
   */
  ~ImageViewerFullWidget()
  {
  		m_Parent = NULL;
  }

 private:
  ParentPointerType m_Parent;
  IndexType m_MousePos;
  bool m_MouseIn;
  unsigned int m_MouseMoveCount;
  unsigned int  m_ValueUpdateFrequency;
  ColorType m_ClassColor;
  IndexType m_LastIndex;
  bool m_ShortCutRectangle;
  bool m_Drag;
};

} // end namespace otb
#endif
