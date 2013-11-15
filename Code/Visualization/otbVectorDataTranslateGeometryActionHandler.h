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
#ifndef __otbVectorDataTranslateGeometryActionHandler_h
#define __otbVectorDataTranslateGeometryActionHandler_h

#include "otbImageWidgetActionHandler.h"
#include "otbMacro.h"

namespace otb
{
/** \class VectorDataTranslateGeometryActionHandler
 *  \brief Handles the user action for geometries translation
 *
 *  \sa VectorDataModel
 *
 */
template<class TModel, class TView>
class VectorDataTranslateGeometryActionHandler : public ImageWidgetActionHandler
{
public:
  /** Standard class typedefs */
  typedef VectorDataTranslateGeometryActionHandler       Self;
  typedef ImageWidgetActionHandler      Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(VectorDataTranslateGeometryActionHandler, ImageWidgetActionHandler);

  /** Model typedefs */
  typedef TModel                       ModelType;
  typedef typename ModelType::Pointer  ModelPointerType;
  typedef typename ModelType::PointType PointType;

  /** View typedefs */
  typedef TView                         ViewType;
  typedef typename ViewType::Pointer    ViewPointerType;

  /** Handle widget event
   * \param widgetId The id of the moved widget
   * \param event The event
   * \return The handling return code
   */
  virtual bool HandleWidgetEvent(const std::string& widgetId, int event)
  {
    if (m_View.IsNotNull() && m_Model.IsNotNull() && this->GetIsActive())
      {
      typename ViewType::ImageWidgetType::Pointer sourceWidget = NULL;

      if (widgetId == m_View->GetFullWidget()->GetIdentifier())
        {
        sourceWidget = m_View->GetFullWidget();
        }
      else if (widgetId == m_View->GetZoomWidget()->GetIdentifier())
        {
        sourceWidget = m_View->GetZoomWidget();
        }

      // Drag using the middle button (2) of the mouse
      if (sourceWidget && Fl::event_button() == m_DragMouseButton)
        {
        switch (event)
          {
          case FL_PUSH:
          {
          m_InScreenPointPushed = m_View->GetFullWidget()->GetMousePosition();
          m_PointPushed = m_View->GetFullWidget()->GetScreenToImageTransform()->TransformPoint(m_InScreenPointPushed);

          m_Model->SearchDataNodeIndexFromPoint(m_PointPushed);
          return true;
          }
          case FL_DRAG:
          {
          // Get the cursor position
          PointType inScreenPoint;
          inScreenPoint[0] = Fl::event_x();
          inScreenPoint[1] = m_View->GetFullWidget()->h() - Fl::event_y();

          double tx = -m_InScreenPointPushed[0] + inScreenPoint[0];
          double ty = -m_InScreenPointPushed[1] + inScreenPoint[1];

          // Call the model methods
          m_Model->UpdateGeometryPosition(tx, -ty);
          m_Model->Update();

          // Update the variables
          m_InScreenPointPushed[0] += tx;
          m_InScreenPointPushed[1] += ty;

          return true;
          }
          default:
          {
          return false;
          }
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

  /** Set/Get End polygon Mouse button */
  itkSetMacro(DragMouseButton, int);
  itkGetMacro(DragMouseButton, int);

protected:
  /** Constructor */
  VectorDataTranslateGeometryActionHandler() :
    m_View(), m_Model(), m_DragMouseButton(2)
  {
  }

  /** Destructor */
  virtual ~VectorDataTranslateGeometryActionHandler()
  {
  }
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  VectorDataTranslateGeometryActionHandler(const Self&); // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  // Pointer to the view
  ViewPointerType m_View;

  // Pointer to the model
  ModelPointerType m_Model;

  /// Drag polygon Mouse button
  int        m_DragMouseButton;

  PointType  m_PointPushed;
  PointType  m_InScreenPointPushed;

}; // end class
} // end namespace otb

#endif
