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
#ifndef __otbImageWidgetActionHandler_h
#define __otbImageWidgetActionHandler_h

#include "itkObject.h"
#include "itkObjectFactory.h"

namespace otb
{
/** \class ImageWidgetActionHandler
*   \brief Base class for widget action handler.
*   This class is a base class for all image widget action handlers.
*   To implement a new action handler, derive this class and implement
*   the ListenTo*() and the HandleWidget*() methods. Then register an
*   instance of your handler in the ImageWidgetController.
* 
*   \sa ImageWidgetController
*   \sa ImageWidget
*/

class ImageWidgetActionHandler
  : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef ImageWidgetActionHandler          Self;
  typedef itk::Object                       Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;
  
  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Runtime information */
  itkTypeMacro(ImageWidgetActionHandler,Object);

  virtual int HandleWidgetEvent(std::string widgetId, int event)
  {
    return 0;
  }

  /** Handle widget resizing
   * \param widgetId The id of the resized widget
   * \param x new x location
   * \param y new y location
   * \param w new width
   * \param h new height
   */
  virtual void HandleWidgetResize(std::string widgetId,int x,int y, int w, int h){}
  
  /** Returns true if the given event from the given widget is
   * handled.
   * \param widgetId The id of the widget the event comes from
   * \param event The event
   * \return True if the event is handled
   */
  virtual bool ListenToEvent(std::string widgetId,int event)
    {
      return false;
    }
  /** Returns true if the resizing of the given widget is handled
   * \param widgetId The id of the widget the event comes from
   * \return True if the resizing is handled
   */
  virtual bool ListenToResize(std::string widgetId)
  {
    return false;
  }

protected:
  /** Constructor */
  ImageWidgetActionHandler(){}
  /** Destructor */
  ~ImageWidgetActionHandler(){}
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const
  {
    Superclass::PrintSelf(os,indent);
  }
 
private:
  ImageWidgetActionHandler(const Self&);    // purposely not implemented
  void operator=(const Self&); // purposely not implemented
}; // end class 
} // end namespace otb
#endif


