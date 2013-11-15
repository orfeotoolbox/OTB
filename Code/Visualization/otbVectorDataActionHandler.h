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
#ifndef __otbVectorDataActionHandler_h
#define __otbVectorDataActionHandler_h

#include "otbImageWidgetActionHandler.h"
#include "otbMacro.h"

namespace otb
{
/** \class VectorDataActionHandler
 *  \brief Handles the user action creating vector data in an OTB application
 *
 *  \sa VectorDataModel
 *
 */
template<class TModel, class TView>
class VectorDataActionHandler : public ImageWidgetActionHandler
{
public:
  /** Standard class typedefs */
  typedef VectorDataActionHandler       Self;
  typedef ImageWidgetActionHandler      Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self)
;

  /** Runtime information */
  itkTypeMacro(VectorDataActionHandler, ImageWidgetActionHandler)
;

  /** Model typedefs */
  typedef TModel                      ModelType;
  typedef typename ModelType::Pointer ModelPointerType;

  /** View typedefs */
  typedef TView                         ViewType;
  typedef typename ViewType::Pointer    ViewPointerType;
  typedef typename ViewType::OffsetType OffsetType;

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

      //Left click
      if (sourceWidget
          && event == FL_PUSH && Fl::event_button() == m_AddMouseButton)
        {
        otbMsgDevMacro(
          << "VectorDataActionHandler::HandleWidgetEvent(): left click handling (" << widgetId << ", " << event << ")");

        // Get the clicked index
        typename ViewType::ImageWidgetType::PointType screenPoint, imagePoint;
        screenPoint = sourceWidget->GetMousePosition();

        //FIXME: check if the index is inside the widget

        // Transform to image point
        imagePoint
          = sourceWidget->GetScreenToImageTransform()->TransformPoint(
          screenPoint);

        if( m_IsContinuousPoint == false )
          {
          imagePoint[0] = static_cast<unsigned int>(imagePoint[0]) + 0.5;
          imagePoint[1] = static_cast<unsigned int>(imagePoint[1]) + 0.5;
          }


        // Transform to index
        typename ModelType::VertexType index;
        index[0] = imagePoint[0];
        index[1] = imagePoint[1];


        //Add Offset
        index[0] += m_Offset[0];
        index[1] += m_Offset[1];

        // Add the point
        m_Model->AddPointToGeometry(index);
        // Update model
        m_Model->Update();
        return true;
        }
      //Right click
      if (sourceWidget
          && event == FL_PUSH && Fl::event_button() == m_EndMouseButton)
        {
        otbMsgDevMacro(
          << "VectorDataActionHandler::HandleWidgetEvent(): right click handling (" << widgetId << ", " << event << ")");
        try
          {
          m_Model->EndGeometry();
          }
        catch(itk::ExceptionObject& err)
          {
          std::stringstream msg;
          msg << err.GetDescription();
          otbGenericMsgDebugMacro(<< msg.str());
          }
        // Update model
        m_Model->Update();
        return true;
        }

      //Delete key pressed
      if ((event == FL_KEYBOARD) || (event == FL_SHORTCUT))
        {
        switch (Fl::event_key())
          {
          /*
          case FL_Delete:
            {
            m_Model->DeleteGeometry();
            // Update model
            m_Model->Update();
            return true;
            break;
            }
          */
          case FL_F + 1:
            {
            m_Model->SetCurrentNodeType(FEATURE_POINT);
            otbMsgDevMacro(
              << "VectorDataActionHandler::HandleWidgetEvent() : changing node type to point");
            break;
            }
          case FL_F + 2:
            {
            m_Model->SetCurrentNodeType(FEATURE_LINE);
            otbMsgDevMacro(
              << "VectorDataActionHandler::HandleWidgetEvent() : changing node type to line");
            break;
            }
          case FL_F + 3:
            {
            m_Model->SetCurrentNodeType(FEATURE_POLYGON);
            otbMsgDevMacro(
              << "VectorDataActionHandler::HandleWidgetEvent() : changing node type to polygon");
            break;
            }
          default:
            break;
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

  /** Set/Get Offset */
  itkSetMacro(Offset, OffsetType);
  itkGetMacro(Offset, OffsetType);

  /** Set/Get End polygon Mouse button */
  itkSetMacro(EndMouseButton, int);
  itkGetMacro(EndMouseButton, int);

  /** Set/Get Add polygon Mouse button */
  itkSetMacro(AddMouseButton, int);
  itkGetMacro(AddMouseButton, int);

  /** Set/Get if Screen Point Centered */
  itkSetMacro(IsContinuousPoint, bool);
  itkGetMacro(IsContinuousPoint, bool);

protected:
  /** Constructor */
  VectorDataActionHandler() :
    m_Offset(), m_View(), m_Model(), m_EndMouseButton(3),
    m_AddMouseButton(1), m_IsContinuousPoint(true)
  {
  }

  /** Destructor */
  virtual ~VectorDataActionHandler()
  {
  }
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  VectorDataActionHandler(const Self&); // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  //Offset
  OffsetType m_Offset;

  // Pointer to the view
  ViewPointerType m_View;

  // Pointer to the model
  ModelPointerType m_Model;

  /// End polygon Mouse button
  int m_EndMouseButton;

  /// Add polygon Mouse button
  int m_AddMouseButton;

  // Is image point is continous  otherwise centered
  bool m_IsContinuousPoint;

}; // end class
} // end namespace otb

#endif
