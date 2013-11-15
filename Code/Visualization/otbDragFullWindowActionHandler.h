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
#ifndef __otbDragFullWindowActionHandler_h
#define __otbDragFullWindowActionHandler_h

#include "otbImageWidgetActionHandler.h"

namespace otb
{
/** \class DragFullWindowActionHandler
*   \brief Implements basic  Full widget Dragging .
*
*   \sa ImageWidgetController
*   \sa ImageWidgetActionHandler
*  \ingroup Visualization
 */

template <class TModel, class TView>
class ITK_EXPORT DragFullWindowActionHandler
  : public ImageWidgetActionHandler
{
public:
  /** Standard class typedefs */
  typedef DragFullWindowActionHandler   Self;
  typedef ImageWidgetActionHandler      Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(DragFullWindowActionHandler, ImageWidgetActionHandler);

  /** Model typedefs */
  typedef TModel                         ModelType;
  typedef typename ModelType::Pointer    ModelPointerType;
  typedef typename ModelType::RegionType RegionType;
  typedef typename ModelType::IndexType  IndexType;

  /** View typedefs */
  typedef TView                                         ViewType;
  typedef typename ViewType::Pointer                    ViewPointerType;
  typedef typename ViewType::ImageWidgetType::PointType PointType;

  /** Handle Full widget dragging
   * \param widgetId The id of the handled Curve widget
   * \param event kind of event or handle : FL_PUSH, FL_RELEASE
   */
  virtual bool HandleWidgetEvent(const std::string& widgetId, int event)
  {
    // Drag using the middle button (2) of the mouse
    if (widgetId == m_View->GetFullWidget()->GetIdentifier() && Fl::event_button() == 2)
      {
      switch (event)
        {
        case FL_PUSH:
          {
          PointType screenPoint = m_View->GetFullWidget()->GetMousePosition();
          PointType ImagePoint = m_View->GetFullWidget()->GetScreenToImageTransform()->TransformPoint(screenPoint);
          m_IndexPushed[0] = ImagePoint[0];
          m_IndexPushed[1] = ImagePoint[1];
          return true;
          }
        case FL_RELEASE:
          {
          PointType screenPoint = m_View->GetFullWidget()->GetMousePosition();
          PointType ImagePoint = m_View->GetFullWidget()->GetScreenToImageTransform()->TransformPoint(screenPoint);
          m_IndexReleased[0] = ImagePoint[0];
          m_IndexReleased[1] = ImagePoint[1];

          // Compute the shift
          m_MoveX = -m_IndexReleased[0] + m_IndexPushed[0];
          m_MoveY = -m_IndexReleased[1] + m_IndexPushed[1];

          // Compute the origin and the size of the visible region
          IndexType indexBegin, indexEnd;
          typedef typename IndexType::IndexValueType IndexValueType;
          indexBegin[0] = static_cast<IndexValueType> (m_Model->GetExtractRegion().GetIndex()[0] + m_MoveX);
          indexBegin[1] = static_cast<IndexValueType> (m_Model->GetExtractRegion().GetIndex()[1] + m_MoveY);
          indexEnd[0] = indexBegin[0] + m_Model->GetExtractRegion().GetSize()[0];
          indexEnd[1] = indexBegin[1] + m_Model->GetExtractRegion().GetSize()[1];

          if (indexEnd[0] < static_cast<IndexValueType> (m_Model->GetLayer(0)->GetExtent().GetSize()[0])
              && indexEnd[1] < static_cast<IndexValueType> (m_Model->GetLayer(0)->GetExtent().GetSize()[1])
              && indexBegin[0] > static_cast<IndexValueType> (m_Model->GetLayer(0)->GetExtent().GetIndex()[0])
              && indexBegin[1] > static_cast<IndexValueType> (m_Model->GetLayer(0)->GetExtent().GetIndex()[1]))
            {
            m_Model->SetExtractRegionByIndex(indexBegin, indexEnd);
            m_Model->Update();
            }
          return true;
          }
        }
      }
    return false;
  }

  /** Set/Get the pointer to the view */
  itkSetObjectMacro(View, ViewType);
  itkGetObjectMacro(View, ViewType);

  /** Set/Get the pointer to the model */
  itkSetObjectMacro(Model, ModelType);
  itkGetObjectMacro(Model, ModelType);
protected:
  /** Constructor */
  DragFullWindowActionHandler() : m_View(), m_Model()
    { }

  /** Destructor */
  virtual ~DragFullWindowActionHandler(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  DragFullWindowActionHandler(const Self &);    // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  // Pointer to the view
  ViewPointerType m_View;

  // Pointer to the model
  ModelPointerType m_Model;

  // Move
  double m_MoveX;
  double m_MoveY;

  IndexType m_IndexPushed;
  IndexType m_IndexReleased;

}; // end class
} // end namespace otb
#endif
