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
  typedef FullResolutionImageWidget<TPixel>   Superclass;
  typedef itk::SmartPointer<Self>            Pointer;
  typedef itk::SmartPointer<const Self>      ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageViewerFullWidget, FullResolutionImageWidget);

  typedef TPixel PixelType;
  typedef typename Superclass::IndexType IndexType;
  typedef typename Superclass::SizeType SizeType;
  typedef typename Superclass::ImageType ImageType;

  typedef ImageViewer<PixelType> ParentType;
  typedef typename ParentType::Pointer ParentPointerType;
  
  itkSetObjectMacro(Parent,ParentType);
  itkGetObjectMacro(Parent,ParentType);
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
	    clickedIndex=this->WindowToImageCoordinates(clickedIndex);
	  m_Parent->ChangeZoomViewedRegion(clickedIndex);
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
	    m_Parent->PrintPixLocVal("");
	    return 1;
	}
	case FL_MOVE:
	  {
	    m_MouseIn=true;
	    if(m_MouseMoveCount%m_ValueUpdateFrequency==0)
	      {
		m_MousePos[0]=Fl::event_x();
		m_MousePos[1]=Fl::event_y();
		IndexType newIndex = this->WindowToImageCoordinates(m_MousePos);
		if(this->GetInput()->GetBufferedRegion().IsInside(newIndex))
		  {
		    std::stringstream oss;
		    typename ImageType::PixelType newPixel = this->GetInput()->GetPixel(newIndex);
		    oss<<" Location: "<<newIndex<<", Values:  "<<newPixel;
		    m_Parent->PrintPixLocVal(oss.str());
		    m_MouseMoveCount=0;
		  }
	      }
	    m_MouseMoveCount++;
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
	    IndexType newIndex = this->GetViewedRegion().GetIndex();
	    SizeType newSize  = this->GetViewedRegion().GetSize();
	    newIndex[0]=newIndex[0]+newSize[0]/2;
	    newIndex[1] = newIndex[1] + newSize[1]/2;
	    switch(Fl::event_key())
	      {
	      case FL_Down:
		{
		  newIndex[1] = newIndex[1]+newSize[1]/8;
		  break;
		}
	      case FL_Up:
		{
		  newIndex[1] = newIndex[1]-newSize[1]/8;
		  break;
		}
	      case FL_Left:
		{
		  newIndex[0] = newIndex[0]-newSize[0]/8;
		  break;
		}
	      case FL_Right:
		{
		  newIndex[0] = newIndex[0]+newSize[0]/8;
		  break;
		}
	      }
	    m_Parent->ChangeFullViewedRegion(newIndex);
	    m_Parent->ChangeZoomViewedRegion(newIndex);
	    return 1;
	  }
	case FL_HIDE:
	  {
	    m_Parent->Hide();
	    return 1;
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
    };
  /**
   * Destructor.
   */
  ~ImageViewerFullWidget(){};

 private:
  ParentPointerType m_Parent;
  IndexType m_MousePos;
  bool m_MouseIn;
  unsigned int m_MouseMoveCount;
  unsigned int  m_ValueUpdateFrequency;
  
};

} // end namespace otb
#endif
