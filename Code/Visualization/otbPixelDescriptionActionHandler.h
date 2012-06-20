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
#ifndef __otbPixelDescriptionActionHandler_h
#define __otbPixelDescriptionActionHandler_h

#include "otbImageWidgetActionHandler.h"

namespace otb
{
/** \class PixelDescriptionActionHandler
*   \brief Implements pixel reporting capabilities.
*
*   \sa ImageWidgetController
*   \sa ImageWidgetActionHandler
*  \ingroup Visualization
 */

template <class TModel, class TView>
class PixelDescriptionActionHandler
  : public ImageWidgetActionHandler
{
public:
  /** Standard class typedefs */
  typedef PixelDescriptionActionHandler Self;
  typedef ImageWidgetActionHandler      Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(PixelDescriptionActionHandler, ImageWidgetActionHandler);

  /** Model typedefs */
  typedef TModel                        ModelType;
  typedef typename ModelType::Pointer   ModelPointerType;
  typedef typename ModelType::IndexType IndexType;

  /** View typedef */
  typedef TView                                     ViewType;
  typedef typename ViewType::Pointer                ViewPointerType;
  typedef typename ViewType::OffsetType             OffsetType;
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
      if (handle)
        {
        switch (event)
          {
          case FL_ENTER:
            {
            return true;
            break;
            }
          case FL_LEAVE:
            {
            return true;
            break;
            }
          case FL_MOVE:
            {
            // Get the clicked index
            typename ViewType::ImageWidgetType::PointType screenPoint, imagePoint;
            screenPoint = sourceWidget->GetMousePosition();

            // Transform to image point
            imagePoint = sourceWidget->GetScreenToImageTransform()->TransformPoint(screenPoint);

            // Transform to index
            typename ViewType::IndexType index;
            index[0] = static_cast<int>(imagePoint[0]);
            index[1] = static_cast<int>(imagePoint[1]);

            //Add the offset
            index += m_Offset;

            // Communicate new index to model
            m_Model->UpdatePixelDescription(index, m_UsePlaceName);

            return true;
            break;
            }
          default:
            {
            break;
            }
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

  /** Set/Get Offset */
  itkSetMacro(Offset, OffsetType);
  itkGetMacro(Offset, OffsetType);

  /** Get PlaceName flag */
  itkSetMacro(UsePlaceName, bool);
  itkGetMacro(UsePlaceName, bool);

protected:
  /** Constructor */
  PixelDescriptionActionHandler() : m_View(), m_Model(), 
                                    m_UsePlaceName(true)
  {
    m_Offset.Fill(0);
  }

  /** Destructor */
  virtual ~PixelDescriptionActionHandler(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  PixelDescriptionActionHandler(const Self&);    // purposely not implemented
  void operator =(const Self&); // purposely not implemented

  //Offset
  OffsetType m_Offset;

  // Pointer to the view
  ViewPointerType m_View;

  // Pointer to the model
  ModelPointerType m_Model;

  // Flag to get the placename
  bool             m_UsePlaceName;

}; // end class
} // end namespace otb
#endif
