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
 */

template <class TModel, class TView>
    class ArrowKeyMoveActionHandler
: public ImageWidgetActionHandler
{
  public:
    /** Standard class typedefs */
    typedef ArrowKeyMoveActionHandler       Self;
    typedef ImageWidgetActionHandler          Superclass;
    typedef itk::SmartPointer<Self>           Pointer;
    typedef itk::SmartPointer<const Self>     ConstPointer;

    /** Method for creation through the object factory */
    itkNewMacro(Self);

    /** Runtime information */
    itkTypeMacro(ArrowKeyMoveActionHandler,ImageWidgetActionHandler);

    /** Model typedefs */
    typedef TModel                         ModelType;
    typedef typename ModelType::Pointer    ModelPointerType;
    typedef typename ModelType::IndexType  IndexType;

    /** View typedef */
    typedef TView                                     ViewType;
    typedef typename ViewType::Pointer                ViewPointerType;
    typedef typename ViewType::ImageWidgetPointerType WidgetPointerType;

    /** Handle widget event
      */
    virtual bool HandleWidgetEvent(std::string widgetId, int event)
    {
      if(m_Model.IsNotNull() &&  m_View.IsNotNull())
      {
        // Find the source widget
        WidgetPointerType sourceWidget;
        bool handle = false;
        if(widgetId == m_View->GetScrollWidget()->GetIdentifier() )
        {
          sourceWidget = m_View->GetScrollWidget();
          handle = true;
        }
        else if(widgetId == m_View->GetFullWidget()->GetIdentifier() )
        {
          sourceWidget = m_View->GetFullWidget();
          handle = true;
        }
        else if(widgetId == m_View->GetZoomWidget()->GetIdentifier() )
        {
          sourceWidget = m_View->GetZoomWidget();
          handle = true;
        }
        if ((handle)
             && ((event == FL_KEYBOARD) || (event == FL_SHORTCUT)))
        {
          switch(Fl::event_key())
          {
            case FL_Up:
            {
              typename ViewType::SizeType size;
              size = m_View->GetFullWidget()->GetExtent().GetSize();

              // Get the current position
              typename ViewType::ImageWidgetType::PointType screenPoint, imagePoint;
              screenPoint[0] = size[0]/2;
              screenPoint[1] = size[1]/2;

              // Transform to image point
              imagePoint = m_View->GetFullWidget()->GetScreenToImageTransform()->TransformPoint(screenPoint);

              // Transform to index
              typename ViewType::IndexType index;
              index[0]=static_cast<int>(imagePoint[0]);
              index[1]=static_cast<int>(imagePoint[1]);

              // Move
              index[1] -= size[1];

              // Change scaled extract region center
              m_Model->SetExtractRegionCenter(index);
              // Update model
              m_Model->Update();
              return true;
              break;
            }
            case FL_Down:
            {
              typename ViewType::SizeType size;
              size = m_View->GetFullWidget()->GetExtent().GetSize();

              // Get the current position
              typename ViewType::ImageWidgetType::PointType screenPoint, imagePoint;
              screenPoint[0] = size[0]/2;
              screenPoint[1] = size[1]/2;

              // Transform to image point
              imagePoint = m_View->GetFullWidget()->GetScreenToImageTransform()->TransformPoint(screenPoint);

              // Transform to index
              typename ViewType::IndexType index;
              index[0]=static_cast<int>(imagePoint[0]);
              index[1]=static_cast<int>(imagePoint[1]);

              // Move
              index[1] += size[1];

              // Change scaled extract region center
              m_Model->SetExtractRegionCenter(index);
              // Update model
              m_Model->Update();
              return true;
              break;
            }
            case FL_Left:
            {
              typename ViewType::SizeType size;
              size = m_View->GetFullWidget()->GetExtent().GetSize();

              // Get the current position
              typename ViewType::ImageWidgetType::PointType screenPoint, imagePoint;
              screenPoint[0] = size[0]/2;
              screenPoint[1] = size[1]/2;

              // Transform to image point
              imagePoint = m_View->GetFullWidget()->GetScreenToImageTransform()->TransformPoint(screenPoint);

              // Transform to index
              typename ViewType::IndexType index;
              index[0]=static_cast<int>(imagePoint[0]);
              index[1]=static_cast<int>(imagePoint[1]);

              // Move
              index[0] -= size[0];

              // Change scaled extract region center
              m_Model->SetExtractRegionCenter(index);
              // Update model
              m_Model->Update();
              return true;
              break;
            }
            case FL_Right:
            {
              typename ViewType::SizeType size;
              size = m_View->GetFullWidget()->GetExtent().GetSize();

              // Get the current position
              typename ViewType::ImageWidgetType::PointType screenPoint, imagePoint;
              screenPoint[0] = size[0]/2;
              screenPoint[1] = size[1]/2;

              // Transform to image point
              imagePoint = m_View->GetFullWidget()->GetScreenToImageTransform()->TransformPoint(screenPoint);

              // Transform to index
              typename ViewType::IndexType index;
              index[0]=static_cast<int>(imagePoint[0]);
              index[1]=static_cast<int>(imagePoint[1]);

              // Move
              index[0] += size[0];

              // Change scaled extract region center
              m_Model->SetExtractRegionCenter(index);
              // Update model
              m_Model->Update();
              return true;
              break;
            }
            default:
            {
              return false;
              break;
            }
          }
        }
      }
      return false;
    }

    /** Set/Get the pointer to the model */
    itkSetObjectMacro(Model,ModelType);
    itkGetObjectMacro(Model,ModelType);

    /** Set/Get the pointer to the view */
    itkSetObjectMacro(View,ViewType);
    itkGetObjectMacro(View,ViewType);

  protected:
    /** Constructor */
    ArrowKeyMoveActionHandler() : m_View(), m_Model()
    {}

    /** Destructor */
    virtual ~ArrowKeyMoveActionHandler(){}
    /** Printself method */
    void PrintSelf(std::ostream& os, itk::Indent indent) const
    {
      Superclass::PrintSelf(os,indent);
    }

  private:
    ArrowKeyMoveActionHandler(const Self&);    // purposely not implemented
    void operator=(const Self&); // purposely not implemented

    // Pointer to the view
    ViewPointerType m_View;

    // Pointer to the model
    ModelPointerType m_Model;

}; // end class
} // end namespace otb
#endif


