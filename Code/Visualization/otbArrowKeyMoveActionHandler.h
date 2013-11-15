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
#ifndef __otbArrowKeyMoveActionHandler_h
#define __otbArrowKeyMoveActionHandler_h

#include "otbImageWidgetActionHandler.h"

namespace otb
{
/** \class ArrowKeyMoveActionHandler
   *   \brief Handle moving in the image with the arrow key.
   *
   *   \sa ImageWidgetController
   *   \sa ImageWidgetActionHandler
   *  \ingroup Visualization
 */

template <class TModel, class TView>
class ArrowKeyMoveActionHandler
  : public ImageWidgetActionHandler
{
public:
  /** Standard class typedefs */
  typedef ArrowKeyMoveActionHandler     Self;
  typedef ImageWidgetActionHandler      Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(ArrowKeyMoveActionHandler, ImageWidgetActionHandler);

  /** Model typedefs */
  typedef TModel                        ModelType;
  typedef typename ModelType::Pointer   ModelPointerType;
  typedef typename ModelType::IndexType IndexType;

  /** View typedef */
  typedef TView                                     ViewType;
  typedef typename ViewType::Pointer                ViewPointerType;
  typedef typename ViewType::ImageWidgetPointerType WidgetPointerType;

  /** Handle widget event
    */
  virtual bool HandleWidgetEvent(const std::string& widgetId, int event)
  {
    if (m_Model.IsNotNull() &&  m_View.IsNotNull() && this->GetIsActive())
      {
      // Find the source widget
      WidgetPointerType sourceWidget;
      bool              handle = false;
      if (widgetId == m_View->GetScrollWidget()->GetIdentifier())
        {
        sourceWidget = m_View->GetScrollWidget();
        handle = true;
        }
      else if (widgetId == m_View->GetFullWidget()->GetIdentifier())
        {
        sourceWidget = m_View->GetFullWidget();
        handle = true;
        }
      else if (widgetId == m_View->GetZoomWidget()->GetIdentifier())
        {
        sourceWidget = m_View->GetZoomWidget();
        handle = true;
        }
      if(handle && event == FL_FOCUS)
        {
        return true;
        }
      if ((handle)
          && ((event == FL_KEYBOARD) || (event == FL_SHORTCUT)))
        {
        // handle compose mode
        if(m_Composed && Fl::event_state() != (int)m_ComposeKey)
          {
          return false;
          }

        unsigned int eventKey = Fl::event_key();

        if(eventKey == m_UpKey)
          {
          typename ViewType::SizeType size;
          size = m_View->GetFullWidget()->GetExtent().GetSize();

          // Get the current position
          typename ViewType::ImageWidgetType::PointType screenPoint, imagePoint;
          screenPoint[0] = size[0] / 2;
          screenPoint[1] = size[1] / 2;

          // Transform to image point
          imagePoint = m_View->GetFullWidget()->GetScreenToImageTransform()->TransformPoint(screenPoint);

          // Transform to index
          typename ViewType::IndexType index;
          index[0] = static_cast<int>(imagePoint[0]);
          index[1] = static_cast<int>(imagePoint[1]);

          // Move
          index[1] -= size[1]/4;

          // Change scaled extract region center
          m_Model->SetExtractRegionCenter(index);
          // Update model
          m_Model->Update();
          return true;
          }
        else if(eventKey == m_DownKey)
          {
          typename ViewType::SizeType size;
          size = m_View->GetFullWidget()->GetExtent().GetSize();

          // Get the current position
          typename ViewType::ImageWidgetType::PointType screenPoint, imagePoint;
          screenPoint[0] = size[0] / 2;
          screenPoint[1] = size[1] / 2;

          // Transform to image point
          imagePoint = m_View->GetFullWidget()->GetScreenToImageTransform()->TransformPoint(screenPoint);

          // Transform to index
          typename ViewType::IndexType index;
          index[0] = static_cast<int>(imagePoint[0]);
          index[1] = static_cast<int>(imagePoint[1]);

          // Move
          index[1] += size[1]/4;

          // Change scaled extract region center
          m_Model->SetExtractRegionCenter(index);
          // Update model
          m_Model->Update();
          return true;
          }
        else if(eventKey == m_LeftKey)
          {
          typename ViewType::SizeType size;
          size = m_View->GetFullWidget()->GetExtent().GetSize();

          // Get the current position
          typename ViewType::ImageWidgetType::PointType screenPoint, imagePoint;
          screenPoint[0] = size[0] / 2;
          screenPoint[1] = size[1] / 2;

          // Transform to image point
          imagePoint = m_View->GetFullWidget()->GetScreenToImageTransform()->TransformPoint(screenPoint);

          // Transform to index
          typename ViewType::IndexType index;
          index[0] = static_cast<int>(imagePoint[0]);
          index[1] = static_cast<int>(imagePoint[1]);

          // Move
          index[0] -= size[0]/4;

          // Change scaled extract region center
          m_Model->SetExtractRegionCenter(index);
          // Update model
          m_Model->Update();
          return true;
          }
        else if(eventKey ==  m_RightKey)
          {
          typename ViewType::SizeType size;
          size = m_View->GetFullWidget()->GetExtent().GetSize();

          // Get the current position
          typename ViewType::ImageWidgetType::PointType screenPoint, imagePoint;
          screenPoint[0] = size[0] / 2;
          screenPoint[1] = size[1] / 2;

          // Transform to image point
          imagePoint = m_View->GetFullWidget()->GetScreenToImageTransform()->TransformPoint(screenPoint);

          // Transform to index
          typename ViewType::IndexType index;
          index[0] = static_cast<int>(imagePoint[0]);
          index[1] = static_cast<int>(imagePoint[1]);

          // Move
          index[0] += size[0]/4;

          // Change scaled extract region center
          m_Model->SetExtractRegionCenter(index);
          // Update model
          m_Model->Update();
          return true;
          }
        }
      }
    return false;
  }

/** Set/Get the pointer to the model */
  itkSetObjectMacro(Model, ModelType);
  itkGetObjectMacro(Model, ModelType);

/** Set/Get the pointer to the view */
  itkSetObjectMacro(View, ViewType);
  itkGetObjectMacro(View, ViewType);

/** Set key mapping */
  itkSetMacro(UpKey, unsigned int);
  itkGetMacro(UpKey, unsigned int);
  itkSetMacro(DownKey, unsigned int);
  itkGetMacro(DownKey, unsigned int);
  itkSetMacro(LeftKey, unsigned int);
  itkGetMacro(LeftKey, unsigned int);
  itkSetMacro(RightKey, unsigned int);
  itkGetMacro(RightKey, unsigned int);
  itkSetMacro(Composed, bool);
  itkGetMacro(Composed, bool);
  itkSetMacro(ComposeKey, unsigned int);
  itkGetMacro(ComposeKey, unsigned int);

protected:
/** Constructor */
  ArrowKeyMoveActionHandler() : m_View(), m_Model(),
                                m_UpKey(FL_Up), m_DownKey(FL_Down),
                                m_LeftKey(FL_Left), m_RightKey(FL_Right),
                                m_Composed(false), m_ComposeKey(FL_SHIFT)
  {}

/** Destructor */
  virtual ~ArrowKeyMoveActionHandler(){}
/** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  ArrowKeyMoveActionHandler(const Self&);      // purposely not implemented
  void operator =(const Self&);  // purposely not implemented

// Pointer to the view
  ViewPointerType m_View;

// Pointer to the model
  ModelPointerType m_Model;

// Key mapping for up, down, left and right
  unsigned int m_UpKey;
  unsigned int m_DownKey;
  unsigned int m_LeftKey;
  unsigned int m_RightKey;

  // Use composed shortcuts
  bool m_Composed;

// Key state (for Ctrl - shortcuts)
  unsigned int m_ComposeKey;

}; // end class
} // end namespace otb
#endif
