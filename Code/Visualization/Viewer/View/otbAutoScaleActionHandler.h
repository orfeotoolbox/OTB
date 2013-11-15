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
#ifndef __otbAutoScaleActionHandler_h
#define __otbAutoScaleActionHandler_h

#include "otbImageWidgetActionHandler.h"

namespace otb
{
/** \class AutoScaleActionHandler
*   \brief Implements basic Scroll, Full and Zoom  widgets resizing.
*
*   \sa ImageWidgetController
*   \sa ImageWidgetActionHandler
*/

template <class TWidgetType>
class AutoScaleActionHandler
  : public ImageWidgetActionHandler
{
public:
  /** Standard class typedefs */
  typedef AutoScaleActionHandler        Self;
  typedef ImageWidgetActionHandler      Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(AutoScaleActionHandler, ImageWidgetActionHandler);

  /** Preview Typedef typedefs */
  typedef TWidgetType                  WidgetType;
  typedef typename WidgetType::Pointer WidgetPointerType;

   /** Handle widget resizing
   * \param widgetId The id of the resized widget
   * \param x new x location
   * \param y new y location
   * \param w new width
   * \param h new height
   */
  virtual bool HandleWidgetResize(std::string widgetId, int w, int h)
  {
    if(widgetId == m_PreviewWidget->GetIdentifier() )
      {
       otbMsgDevMacro(<<"AutoScaleActionHandler::HandleWidgetResize(): handling ("<<widgetId<<", "<<w<<", "<<h<<")");
       typename WidgetType::SizeType    size = m_PreviewWidget->GetOpenGlBufferedRegion().GetSize();

       double zoomW = static_cast<double>(w)/static_cast<double>(size[0]);
       double zoomH = static_cast<double>(h)/static_cast<double>(size[1]);
       m_PreviewWidget->SetIsotropicZoom(std::min(zoomW, zoomH));

       return true;
      }

    return false;
  }

  /** Set/Get the pointer to PreviewWidget */
 itkSetObjectMacro(PreviewWidget, WidgetType);

protected:
  /** Constructor */
  AutoScaleActionHandler()
    {};

  /** Destructor */
  virtual ~AutoScaleActionHandler(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os, indent);
  }

private:
  AutoScaleActionHandler(const Self&);    // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  WidgetPointerType              m_PreviewWidget;

}; // end class
} // end namespace otb
#endif
