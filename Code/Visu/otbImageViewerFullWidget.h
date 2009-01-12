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
#ifndef __otbImageViewerFullWidget_h
#define __otbImageViewerFullWidget_h

#include "otbFullResolutionImageWidget.h"
#include "otbImageViewerFullResolutionEventsInterface.h"
#include "otbImageWidgetBoxForm.h"
#include "otbImageWidgetRectangleForm.h"
#include "otbImageWidgetPointForm.h"
#include "otbImageWidgetPolygonForm.h"

namespace otb
{
template <class TPixel, class TLabel> class ImageViewerBase;
/**
 * \class ImageViewerFullWidget
 *
 * \brief Custom full image widget.
 *
 * This class derives from otb::FullResolutionImageWidget, and implements behaviours specific to the image viewer.
 *
 * \sa ImageViewer, FullResolutionImageWidget
 *
 */
 template <class TPixel, class TLabel = double>
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

 /** This interface is provided in case some other application wants to intercept polygon an rectangle selection events */
 typedef ImageViewerFullResolutionEventsInterface EventsInterfaceType;
 typedef EventsInterfaceType::Pointer             EventsInterfacePointerType;


  typedef TPixel PixelType;
  typedef TLabel LabelType;
  typedef typename Superclass::IndexType IndexType;
  typedef typename Superclass::SizeType  SizeType;
 typedef typename Superclass::ImageType ImageType;
 typedef typename Superclass::RegionType RegionType;
 typedef typename Superclass::OverlayImageType OverlayImageType;

  typedef ImageViewerBase<PixelType, LabelType>  ParentType;
  typedef ParentType*                    ParentPointerType;

  typedef ImageWidgetBoxForm             BoxType;
  typedef ImageWidgetRectangleForm       RectangleType;
  typedef RectangleType::ColorType       ColorType;
  typedef ImageWidgetPointForm           PointType;
  typedef Polygon<LabelType>       PolygonType;
  typedef typename PolygonType::ContinuousIndexType ContinuousIndexType;
  typedef typename PolygonType::Superclass::VertexListPointer VertexListPointerType;
  typedef typename PolygonType::Superclass::VertexListType VertexListType;

  itkSetMacro(Parent,ParentPointerType);
  itkGetMacro(Parent,ParentPointerType);
  itkGetObjectMacro(EventsInterface,EventsInterfaceType);
  itkSetObjectMacro(EventsInterface,EventsInterfaceType);

 virtual void SetUpperLeftCorner(IndexType index)
 {
   Superclass::SetUpperLeftCorner(index);
   if(m_EventsInterface.IsNotNull())
     {
       m_EventsInterface->ViewedRegionChanged();
     }
 }

 /** Handle method */


  /** Default mode handling, without ROI selection */
  virtual int DefaultHandle(int event)
  {
    switch(event)
	{
	case FL_PUSH:
          {
        // in case of mouse click, change the point of view
          int x = Fl::event_x();
	  int y = Fl::event_y();
	  IndexType clickedIndex;
	  clickedIndex[0]=x;
	  clickedIndex[1]=y;
          clickedIndex=this->WindowToImageCoordinates(clickedIndex);

	  if(m_EventsInterface.IsNotNull())
	  {
	    m_EventsInterface->PixelClicked(clickedIndex);
	  }
	  if(m_EventsInterface.IsNull() ||  m_EventsInterface->GetForwardEvents())
	  {
	    m_Parent->ChangeZoomViewedRegion(clickedIndex);
	    m_Parent->Update();
	  }
          return 1;
          }
       case FL_KEYDOWN:
	  {
	    IndexType newIndex = m_Parent->GetZoomWidget()->GetViewedRegion().GetIndex();
	    SizeType newSize  = m_Parent->GetZoomWidget()->GetViewedRegion().GetSize();
            bool moved = false;
	    switch(Fl::event_key())
	      {
                case FL_Down:
		  {
                    newIndex[1] += static_cast<long int>(newSize[1]/2)+newSize[1]/2;
		    newIndex[0] += newSize[0]/2;
                    moved = true;
                    break;
                  }
                case FL_Up:
		  {
                    newIndex[1] += -static_cast<long int>(newSize[1]/2)
                                +newSize[1]/2;
		    newIndex[0] += newSize[0]/2;
                    moved = true;
                    break;
                  }
                case FL_Left:
		{
                    newIndex[0] += -static_cast<long int>(newSize[0]/2)
                                + newSize[0]/2;
		    newIndex[1] += newSize[1]/2;
                    moved = true;
                    break;
                }
                case FL_Right:
		{
                    newIndex[0] += static_cast<long int>(newSize[0]/2)
                                + newSize[0]/2;
		    newIndex[1] += newSize[1]/2;
                    moved = true;
                    break;
                }
                // if the view center was moved:
                if(moved)
                {
                  m_Parent->ChangeFullViewedRegion(newIndex);
		  m_Parent->ChangeZoomViewedRegion(newIndex);
                  m_Parent->Update();
                }
              }
                return 1;
            }
          }
    return 0;
  }


  virtual int RectangleROISelectionHandle(int event)
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
        if (!m_Drag)
	{
	  m_LastIndex=this->WindowToImageCoordinates(clickedIndex);
        }
        return 1;
      }
      case FL_DRAG:
      {
        int x = Fl::event_x();
	int y = Fl::event_y();
	IndexType index;
	index[0]=x;
	index[1]=y;
	IndexType clickedIndex=this->WindowToImageCoordinates(index);
        typename BoxType::Pointer box =  BoxType::New();
	 box->SetColor(m_Parent->GetInterfaceBoxesColor());
	 IndexType boxIndex;
	 SizeType boxSize;
         // compute the size of the selection box
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
	     m_Parent->GetInterfaceBoxesList()->PopBack();

	   }
	 m_Parent->GetInterfaceBoxesList()->PushBack(box);
	 m_Drag=true;
	m_Parent->Update();
	 return 1;
      }

      case FL_RELEASE:
      {
	int x = Fl::event_x();
	int y = Fl::event_y();
	IndexType clickedIndex;
	clickedIndex[0]=x;
	clickedIndex[1]=y;
	clickedIndex=this->WindowToImageCoordinates(clickedIndex);
	if(m_Drag)
	{
	  m_Parent->GetInterfaceBoxesList()->PopBack();
	}
	if(m_EventsInterface.IsNull() || m_EventsInterface->GetForwardEvents())
	{
	  if(m_Parent->GetPolygonROIList()->Size()==0)
	  {
	    m_Parent->GetPolygonROIList()->PushBack(PolygonType::New());
	    m_Parent->GetPolygonROIList()->Back()->SetValue(m_Parent->GetNextROILabel());
	  }
	}
	IndexType boxIndex;
	SizeType boxSize;
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

	if(m_EventsInterface.IsNotNull())
	{
	  RegionType selectedRegion;
	  selectedRegion.SetIndex(boxIndex);
	  selectedRegion.SetSize(boxSize);
	  m_EventsInterface->RegionSelected(selectedRegion);
	}
	if(m_EventsInterface.IsNull() || m_EventsInterface->GetForwardEvents())
	{
	  ContinuousIndexType newVertex;
	  // Up Left corner
	  newVertex[0] = boxIndex[0];
	  newVertex[1] = boxIndex[1];
	  m_Parent->GetPolygonROIList()->Back()->AddVertex(newVertex);
	  // Up Right corner
	  newVertex[0] += boxSize[0];
	  m_Parent->GetPolygonROIList()->Back()->AddVertex(newVertex);
	  // Down Right corner
	  newVertex[1] += boxSize[1];
	  m_Parent->GetPolygonROIList()->Back()->AddVertex(newVertex);
	  // Down Left corner
	  newVertex[0]= boxIndex[0];
	  m_Parent->GetPolygonROIList()->Back()->AddVertex(newVertex);
	  m_Parent->GetPolygonROIList()->PushBack(PolygonType::New());
	  m_Parent->GetPolygonROIList()->Back()->SetValue(m_Parent->GetNextROILabel());

	  m_Parent->Update();
	}
	m_Drag=false;
	return 1;
      }
    }
    return 0;
  }

  virtual int PolygonROISelectionHandle(int event)
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
          // If left mouse click
          if(Fl::event_button()==FL_LEFT_MOUSE)
          {
            // If not already editing a polygon, start a new one.
            if(m_Parent->GetPolygonROIList()->Size()==0)
	      {
		m_Parent->GetPolygonROIList()->PushBack(PolygonType::New());
		m_Parent->GetPolygonROIList()->Back()->SetValue(m_Parent->GetNextROILabel());
	      }
	    m_Parent->GetPolygonROIList()->Back()->AddVertex(clickedIndex);
	    m_Parent->Update();
          }
          else if(Fl::event_button()==FL_RIGHT_MOUSE)
          {
	    m_Parent->GetPolygonROIList()->PushBack(PolygonType::New());
	    m_Parent->GetPolygonROIList()->Back()->SetValue(m_Parent->GetNextROILabel());

          }
          return 1;
        }
        case FL_KEYDOWN:
        {
          // erase the last vertex of the current polygon
          if(Fl::event_key()==FL_Page_Down)
          {
	    if(m_Parent->GetPolygonROIList()->Size()>0)
	    {
	      unsigned int sizeOfThePolygon = m_Parent->GetPolygonROIList()->Back()->GetVertexList()->Size();
	      if(sizeOfThePolygon>0)
	      {
		// itk::PolylineParametricPath does not provide a RemoveVertex() method, and the access to the vertex list is const, so we have no other choice to remove a vertex.
		VertexListPointerType list = const_cast<VertexListType *>(m_Parent->GetPolygonROIList()->Back()->GetVertexList());
		list->pop_back();
		m_Parent->Update();
	      }
	    }
          }
        return 1;
        }
      }
    return 0;
}

  virtual int handle(int event)
  {
  // Handle the mode selection to call the specific handle methods */
  if(event == FL_KEYDOWN)
    {
      // Erase the last ROI
      if(Fl::event_key()==FL_Delete && m_Parent->GetPolygonROIList()->Size() > 0)
      {
        m_Parent->GetPolygonROIList()->Erase(m_Parent->GetPolygonROIList()->Size()-1);
        m_Parent->Update();
      }
//       else if(Fl::event_key()==FL_Control_L)
//       {
//         m_ShortCutRectangle = !m_ShortCutRectangle;
//         m_ShortCutPolygon = false;
//
//         if(m_ShortCutRectangle)
//         {
//           std::cout<<"Rectangle ROI selection mode ON"<<std::endl;
//         }
//         else
//         {
//           std::cout<<"Rectangle ROI selection mode OFF"<<std::endl;
//         }
//       }
//       else if(Fl::event_key()==FL_Shift_L)
//       {
//         m_ShortCutPolygon = !m_ShortCutPolygon;
//         m_ShortCutRectangle = false;
//
//         if(m_ShortCutPolygon)
//         {
//           std::cout<<"Polygon ROI selection mode ON"<<std::endl;
//         }
//         else
//         {
//           m_PolygonInProgress = false;
//           std::cout<<"Polygon ROI selection mode OFF"<<std::endl;
//         }
//       }
    }
    // handle the pixel value reporting
    switch(event)
    {
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
            // If we move inside the point of view, then report the pixel location and value.
	    m_MouseIn=true;
	    if(m_MouseMoveCount%m_ValueUpdateFrequency==0)
	      {
		m_MousePos[0]=Fl::event_x();
		m_MousePos[1]=Fl::event_y();
		IndexType newIndex = this->WindowToImageCoordinates(m_MousePos);
		m_Parent->ReportPixel(newIndex);
		m_MouseMoveCount=0;
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
       case FL_HIDE:
	  {
	    m_Parent->Hide();
	    return 0;
	  }
    }
    // If the current mode is rectangle selection, call the right handle method
    if(m_Parent->GetRectangularROISelectionMode())
    {
      return RectangleROISelectionHandle(event);
    }
    // If the current mode is polygon selection, call the right handle method
    else if(m_Parent->GetPolygonalROISelectionMode())
    {
      return PolygonROISelectionHandle(event);
    }
    // else call the default handle method
    else
    {
      return DefaultHandle(event);
    }
  }

  virtual void resize(int x,int y, int w, int h)
   {
     Superclass::resize(x,y,w,h);
      if(m_Parent->GetBuilt())
	m_Parent->Update();
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
      m_LastIndex.Fill(0);
      m_ShortCutRectangle = false;
      m_ShortCutPolygon = false;
      m_Drag = false;
      m_ValueUpdateFrequency = 5;

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
  IndexType m_LastIndex;
  bool m_ShortCutRectangle;
  bool m_ShortCutPolygon;
  bool m_Drag;
 EventsInterfacePointerType m_EventsInterface;
};

} // end namespace otb
#endif
