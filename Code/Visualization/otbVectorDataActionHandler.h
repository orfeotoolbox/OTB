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
  virtual bool HandleWidgetEvent(std::string widgetId, int event)
  {
    if (m_View.IsNotNull() && m_Model.IsNotNull() && this->GetIsActive())
      {
      //Left click
      if (widgetId == m_View->GetFullWidget()->GetIdentifier() && event
          == FL_PUSH && Fl::event_button() == 1)
        {
        otbMsgDevMacro(
          << "VectorDataActionHandler::HandleWidgetEvent(): left click handling (" << widgetId << ", " << event << ")");

        // Get the clicked index
        typename ViewType::ImageWidgetType::PointType screenPoint, imagePoint;
        screenPoint = m_View->GetFullWidget()->GetMousePosition();

        //FIXME: check if the index is inside the widget

        // Transform to image point
        imagePoint
          = m_View->GetFullWidget()->GetScreenToImageTransform()->TransformPoint(
          screenPoint);

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
      if (widgetId == m_View->GetFullWidget()->GetIdentifier() && event
          == FL_PUSH && Fl::event_button() == 3)
        {
        otbMsgDevMacro(
          << "VectorDataActionHandler::HandleWidgetEvent(): right click handling (" << widgetId << ", " << event << ")");
        m_Model->EndGeometry();
        // Update model
        m_Model->Update();
        return true;
        }

      //Delete key pressed
      if ((event == FL_KEYBOARD) || (event == FL_SHORTCUT))
        {
        switch (Fl::event_key())
          {
        case FL_Delete:
          {
          m_Model->DeleteGeometry();
          // Update model
          m_Model->Update();
          return true;
          break;
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

  /** Set/Get Offset */
  itkSetMacro(Offset, OffsetType);
  itkGetMacro(Offset, OffsetType);

protected:
  /** Constructor */
  VectorDataActionHandler() :
    m_Offset(), m_View(), m_Model()
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

}; // end class
} // end namespace otb

#endif
