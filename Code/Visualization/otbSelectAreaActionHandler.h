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


namespace otb
{
/** \class SelectAreaActionHandler
*   \brief Implements basic Scroll, Full and Zoom  widgets resizing.
* 
*   \sa ImageWidgetController
*   \sa ImageWidgetActionHandler
*/

template <class TModel, class TView> 
class SelectAreaActionHandler
  : public ImageWidgetActionHandler
{
public:
  /** Standard class typedefs */
  typedef SelectAreaActionHandler Self;
  typedef ImageWidgetActionHandler               Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;
  
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Runtime information */
  itkTypeMacro(SelectAreaActionHandler,ImageWidgetActionHandler);

  /** Model typedefs */
  typedef TModel                         ModelType;
  typedef typename ModelType::Pointer    ModelPointerType;
  typedef typename ModelType::RegionType RegionType;

  /** View typedefs */
  typedef TView                          ViewType;
  typedef typename ViewType::Pointer     ViewPointerType;
  typedef typename ViewType::IndexType   IndexType;



  /** Handle widget event
   * \param widgetId The id of the moved widget
   * \param event The event
   * \return The handling return code
   */
  virtual bool HandleWidgetEvent(std::string widgetId, int event)
  {
    if( m_View.IsNotNull() && m_Model.IsNotNull() )
      {
    if(widgetId == m_View->GetScrollWidget()->GetIdentifier() )
      {
        //otbMsgDevMacro(<<"SelectArea PixelClickedActionHandler::HandleWidgetEvent(): handling ("<<widgetId<<", "<<event<<")");
        
        // Get the clicked index
        //typename ImageViewType::ImageWidgetType::PointType screenPoint, imagePoint;
        typename ViewType::ImageWidgetType::PointType screenPoint, imagePoint;/*, lScreenSizePoint, lImageSizePoint;*/
        screenPoint = m_View->GetScrollWidget()->GetMousePosition();
        
        // Transform to image point
        imagePoint = m_View->GetScrollWidget()->GetScreenToImageTransform()->TransformPoint(screenPoint);

        // Transform to index
        IndexType lIndex;
        lIndex[0]=static_cast<long int>(imagePoint[0]);
        lIndex[1]=static_cast<long int>(imagePoint[1]);

       
        switch (event)
        {
          case FL_PUSH:
          {
              m_FirstPush = false;
              m_StartIndex = lIndex;
            
            break;
          }
          case FL_RELEASE:
          {
            m_FirstPush = true;
            m_StopIndex = lIndex;
            m_Model->SetExtractRegionByIndex(m_StartIndex,m_StopIndex);
           m_Model->Update();
            //m_Model->UpdateSecondScroll();
            break;
          }
          case FL_DRAG:
          {
            m_StopIndex = lIndex;
            m_Model->SetExtractRegionByIndex(m_StartIndex,m_StopIndex);
            m_Model->Update();
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
  itkSetObjectMacro(View,ViewType);
  itkGetObjectMacro(View,ViewType);

  /** Set/Get the pointer to the model */
  itkSetObjectMacro(Model,ModelType);
  itkGetObjectMacro(Model,ModelType);


protected:
  /** Constructor */
  SelectAreaActionHandler() : m_View(), m_Model()
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
    Superclass::PrintSelf(os,indent);
  }
 
private:
  SelectAreaActionHandler(const Self&);    // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  // Pointer to the view
  ViewPointerType m_View;

  // Pointer to the model
  ModelPointerType m_Model;

  bool m_FirstPush;

  // Area selection indexes
  IndexType m_StartIndex;
  IndexType m_StopIndex;

}; // end class 
} // end namespace otb
#endif


