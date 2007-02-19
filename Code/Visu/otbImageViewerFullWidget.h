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
		std::stringstream oss;
		IndexType newIndex = this->WindowToImageCoordinates(m_MousePos);
		oss<<"Location: "<<newIndex<<", Values:  "<<this->GetInput()->GetPixel(newIndex);
		m_Parent->PrintPixLocVal(oss.str());
		m_MouseMoveCount=0;
	      }
	    m_MouseMoveCount++;
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
