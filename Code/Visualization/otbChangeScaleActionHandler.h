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
#ifndef __otbChangeScaleActionHandler_h
#define __otbChangeScaleActionHandler_h

#include "otbImageWidgetActionHandler.h"

namespace otb
{
/** \class ChangeScaleActionHandler
*   \brief Implements basic Scroll, Full and Zoom  widgets resizing.
* 
*   \sa ImageWidgetController
*   \sa ImageWidgetActionHandler
*/

template <class TModel, class TView> 
class ChangeScaleActionHandler
  : public ImageWidgetActionHandler
{
public:
  /** Standard class typedefs */
  typedef ChangeScaleActionHandler Self;
  typedef ImageWidgetActionHandler               Superclass;
  typedef itk::SmartPointer<Self>                Pointer;
  typedef itk::SmartPointer<const Self>          ConstPointer;
  
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Runtime information */
  itkTypeMacro(ChangeScaleActionHandler,ImageWidgetActionHandler);

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
      if(widgetId == m_View->GetZoomWidget()->GetIdentifier()
         && event == FL_MOUSEWHEEL)
        {
        otbMsgDevMacro(<<"ChangeScaleActionHandler::HandleWidgetEvent(): handling ("<<widgetId<<", "<<event<<")");
        // Get the mousewhell offset
        int dy = Fl::event_dy();
        
        // Compute new scale
        double newScale = m_View->GetZoomWidget()->GetIsotropicZoom() * vcl_pow(m_ScaleRatio,-dy);
        if(newScale>=1.0)
          {
          m_View->GetZoomWidget()->SetIsotropicZoom(newScale);
          RegionType region = m_Model->GetScaledExtractRegion();
          
          typename RegionType::IndexType index = region.GetIndex();
          typename RegionType::SizeType size = region.GetSize();
          index[0]+=size[0]/2;
          index[1]+=size[1]/2;
          size[0] = static_cast<unsigned int>(m_View->GetZoomWidget()->w()/m_View->GetZoomWidget()->GetIsotropicZoom());
          size[1] = static_cast<unsigned int>(m_View->GetZoomWidget()->h()/m_View->GetZoomWidget()->GetIsotropicZoom());
          region.SetSize(size);
          m_Model->SetScaledExtractRegion(region);
          m_Model->SetScaledExtractRegionCenter(index);
          // Update the model
          m_Model->Update();
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

  /** Set/Get scale ratio */
  itkSetMacro(ScaleRatio,double);
  itkGetMacro(ScaleRatio,double);

protected:
  /** Constructor */
  ChangeScaleActionHandler() : m_View(), m_Model(), m_ScaleRatio(1.25)
  {}

  /** Destructor */
  virtual ~ChangeScaleActionHandler(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  }
 
private:
  ChangeScaleActionHandler(const Self&);    // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  // Pointer to the view
  ViewPointerType m_View;
  
  // Pointer to the model
  ModelPointerType m_Model;
  
  /** Scale Ratio */
  double m_ScaleRatio;

}; // end class 
} // end namespace otb
#endif


