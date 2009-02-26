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
#ifndef __otbChangeScaledExtractRegionActionHandler_h
#define __otbChangeScaledExtractRegionActionHandler_h

#include "otbImageWidgetActionHandler.h"

namespace otb
{
/** \class ChangeScaledExtractRegionActionHandler
*   \brief Implements basic Scroll, Full and Zoom  widgets resizing.
* 
*   \sa ImageWidgetController
*   \sa ImageWidgetActionHandler
*/

template <class TModel, class TView> 
class ChangeScaledExtractRegionActionHandler
  : public ImageWidgetActionHandler
{
public:
  /** Standard class typedefs */
  typedef ChangeScaledExtractRegionActionHandler Self;
  typedef ImageWidgetActionHandler               Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;
  
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Runtime information */
  itkTypeMacro(ChangeScaledExtractRegionActionHandler,ImageWidgetActionHandler);

  /** Model typedefs */
  typedef TModel                         ModelType;
  typedef typename ModelType::Pointer    ModelPointerType;
  typedef typename ModelType::RegionType RegionType;

  /** View typedefs */
  typedef TView                          ViewType;
  typedef typename ViewType::Pointer     ViewPointerType;


  /** Handle widget event
   * \param widgetId The id of the moved widget
   * \param event The event
   * \return The handling return code
   */
  virtual bool HandleWidgetEvent(std::string widgetId, int event)
  {
    if( m_View.IsNotNull() && m_Model.IsNotNull() )
      {
      if(widgetId == m_View->GetFullWidget()->GetIdentifier()
	 && event == FL_PUSH)
	{
	// Get the clicked index
	typename ViewType::IndexType index;
	index[0] = Fl::event_x();
	index[1] = m_View->GetFullWidget()->h()-Fl::event_y();
	// Convert to image index
	index = m_View->GetFullWidget()->ScreenIndexToRegionIndex(index);
	// Change scaled extract region center
	m_Model->SetScaledExtractRegionCenter(m_View->GetFullWidget()->ScreenIndexToRegionIndex(index));
	// Update model
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
  ChangeScaledExtractRegionActionHandler() : m_View(), m_Model()
  {}

  /** Destructor */
  virtual ~ChangeScaledExtractRegionActionHandler(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  }
 
private:
  ChangeScaledExtractRegionActionHandler(const Self&);    // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  // Pointer to the view
  ViewPointerType m_View;
  
  // Pointer to the model
  ModelPointerType m_Model;
  
}; // end class 
} // end namespace otb
#endif


