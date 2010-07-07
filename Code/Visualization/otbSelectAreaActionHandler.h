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
#ifndef __otbSelectAreaActionHandler_h
#define __otbSelectAreaActionHandler_h

#include "otbImageWidgetActionHandler.h"
#include "otbRegionGlComponent.h"

namespace otb
{
/** \class SelectAreaActionHandler
*   \brief Implements basic Scroll, Full and Zoom  widgets resizing.
*
*   \sa ImageWidgetController
*   \sa ImageWidgetActionHandler
*  \ingroup Visualization
 */

template <class TModel, class TWidget>
class SelectAreaActionHandler
  : public ImageWidgetActionHandler
{
public:
  /** Standard class typedefs */
  typedef SelectAreaActionHandler       Self;
  typedef ImageWidgetActionHandler      Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(SelectAreaActionHandler, ImageWidgetActionHandler);

  /** Model typedefs */
  typedef TModel                         ModelType;
  typedef typename ModelType::Pointer    ModelPointerType;
  typedef typename ModelType::RegionType RegionType;

  /** View typedefs */
  typedef TWidget                        WidgetType;
  typedef typename WidgetType::Pointer   WidgetPointerType;
  typedef typename WidgetType::IndexType IndexType;

  /** RegionGlComponent View */
  typedef RegionGlComponent                       RegionGlComponentType;
  typedef typename RegionGlComponentType::Pointer RegionGlComponentPointerType;

  /** Handle widget event
   * \param widgetId The id of the moved widget
   * \param event The event
   * \return The handling return code
   */
  virtual bool HandleWidgetEvent(std::string widgetId, int event)
  {
    // if left mouse button pressed
    if (Fl::event_button() != FL_LEFT_MOUSE)
      {
      return true;
      }

    if (m_Model.IsNotNull() && this->GetIsActive())
      {
      if (widgetId == m_Widget->GetIdentifier())
        {
        // Get the clicked index
        typename WidgetType::PointType screenPoint, imagePoint;    /*, lScreenSizePoint, lImageSizePoint;*/
        screenPoint = m_Widget->GetMousePosition();

        // Transform to image point
        imagePoint = m_Widget->GetScreenToImageTransform()->TransformPoint(screenPoint);

        // Transform to index
        IndexType lIndex;
        lIndex[0] = static_cast<long int>(imagePoint[0]);
        lIndex[1] = static_cast<long int>(imagePoint[1]);

        switch (event)
          {
          case FL_PUSH:
            {
            m_FirstPush = false;
            m_StartIndex = lIndex;
            // ImageView.txx hide the GlComponent when Update
            m_RegionGlComponent->SetVisible(true);
            break;
            }
          case FL_RELEASE:
            {
            if (m_StartIndex[0] != lIndex[0] && m_StartIndex[1] != lIndex[1])
              {
              m_FirstPush = true;
              m_StopIndex = lIndex;
              m_Model->SetExtractRegionByIndex(m_StartIndex, m_StopIndex);
              m_Model->Update();
              }
            break;
            }
          case FL_DRAG:
            {
            // only redraw the red box in the widget
            m_StopIndex = lIndex;
            m_Model->SetExtractRegionByIndex(m_StartIndex, m_StopIndex);
            m_RegionGlComponent->SetRegion(m_Model->GetExtractRegion());
            m_Widget->redraw();
            break;
            }
          default:
            {
            }
          }
        return true;
        }
      }
    return false;

  }

  /** Set/Get the pointer to the view */
  itkSetObjectMacro(Widget, WidgetType);
  itkGetObjectMacro(Widget, WidgetType);

  /** Set/Get the pointer to the model */
  itkSetObjectMacro(Model, ModelType);
  itkGetObjectMacro(Model, ModelType);

  /** Set/Get the pointer to the component */
  itkSetObjectMacro(RegionGlComponent, RegionGlComponentType);
  itkGetObjectMacro(RegionGlComponent, RegionGlComponentType);

protected:
  /** Constructor */
  SelectAreaActionHandler() : m_Widget(), m_Model(), m_RegionGlComponent()
  {
    m_FirstPush = true;
    m_StartIndex.Fill(0);
    m_StopIndex.Fill(0);
  }

  /** Destructor */
  virtual ~SelectAreaActionHandler(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  SelectAreaActionHandler(const Self&);    // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  // Pointer to the widget
  WidgetPointerType m_Widget;
  // Pointer to the model
  ModelPointerType m_Model;
  // Pointer to the GlComponent
  RegionGlComponentPointerType m_RegionGlComponent;

  bool m_FirstPush;

  // Area selection indexes
  IndexType m_StartIndex;
  IndexType m_StopIndex;

}; // end class
} // end namespace otb
#endif
