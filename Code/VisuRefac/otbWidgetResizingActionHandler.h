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
#ifndef __otbWidgetResizingActionHandler_h
#define __otbWidgetResizingActionHandler_h

#include "otbImageWidgetActionHandler.h"

namespace otb
{
/** \class WidgetResizingActionHandler
*   \brief Implements basic Scroll, Full and Zoom  widgets resizing.
* 
*   \sa ImageWidgetController
*   \sa ImageWidgetActionHandler
*/

template <class TModel, class TView> 
class WidgetResizingActionHandler
  : public ImageWidgetActionHandler
{
public:
  /** Standard class typedefs */
  typedef WidgetResizingActionHandler       Self;
  typedef ImageWidgetActionHandler          Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;
  
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Runtime information */
  itkTypeMacro(WidgetResizingActionHandler,ImageWidgetActionHandler);

  /** Model typedefs */
  typedef TModel                         ModelType;
  typedef typename ModelType::Pointer    ModelPointerType;
  typedef typename ModelType::RegionType RegionType;

  /** View typedefs */
  typedef TView                          ViewType;
  typedef typename ViewType::Pointer     ViewPointerType;

   /** Handle widget resizing
   * \param widgetId The id of the resized widget
   * \param x new x location
   * \param y new y location
   * \param w new width
   * \param h new height
   */
  virtual bool HandleWidgetResize(std::string widgetId, int w, int h)
  {
    if(m_Model.IsNotNull() && m_View.IsNotNull())
      {
      // If resizing the scroll widget, nothing has to be done.
      if(widgetId == m_View->GetScrollWidget()->GetIdentifier() )
        {
        otbMsgDevMacro(<<"WidgetResizingActionHandler::HandleWidgetResize(): handling ("<<widgetId<<", "<<w<<", "<<h<<") ");
        // Nothing has to be acted to the model, juste update the view
        m_View->Update();
        return true;
        }
      else if(widgetId == m_View->GetFullWidget()->GetIdentifier() )
        {
        otbMsgDevMacro(<<"WidgetResizingActionHandler::HandleWidgetResize(): handling ("<<widgetId<<", "<<w<<", "<<h<<")");
        // Enlarge the model extract region
        RegionType region = m_Model->GetExtractRegion();
        typename RegionType::SizeType size = region.GetSize();
        size[0] = static_cast<unsigned int>(w);
        size[1] = static_cast<unsigned int>(h);
        region.SetSize(size);
        m_Model->SetExtractRegion(region);
        // Update the model
        m_Model->Update();
        return true;
        }
      else if(widgetId ==m_View->GetZoomWidget()->GetIdentifier() )
        {
        otbMsgDevMacro(<<"WidgetResizingActionHandler::HandleWidgetResize(): handling ("<<widgetId<<", "<<w<<", "<<h<<")");
        // Enlarge the model scaled extract region
        RegionType region = m_Model->GetScaledExtractRegion();
        typename RegionType::SizeType size = region.GetSize();
        size[0] = static_cast<unsigned int>(static_cast<double>(w)/m_View->GetZoomWidget()->GetIsotropicZoom());
        size[1] = static_cast<unsigned int>(static_cast<double>(h)/m_View->GetZoomWidget()->GetIsotropicZoom());
        region.SetSize(size);
        m_Model->SetScaledExtractRegion(region);
        // Update the model
        m_Model->Update();
        return true;
        }
      }
    return false;
  }
  
  /** Set/Get the pointer to the view */
  itkSetObjectMacro(View,ViewType);
  itkGetObjectMacro(View,ViewType);

  /** Set/Get the pointer to the model */
  itkSetObjectMacro(Model,ModelType);
  itkGetObjectMacro(Model,ModelType);

protected:
  /** Constructor */
  WidgetResizingActionHandler() : m_View(), m_Model()
  {}

  /** Destructor */
  virtual ~WidgetResizingActionHandler(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  }
 
private:
  WidgetResizingActionHandler(const Self&);    // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  // Pointer to the view
  ViewPointerType m_View;
  
  // Pointer to the model
  ModelPointerType m_Model;
  
}; // end class 
} // end namespace otb
#endif


