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
#ifndef __otbImageWidgetController_h
#define __otbImageWidgetController_h

#include "otbImageWidgetActionHandler.h"
#include "otbObjectList.h"

namespace otb
{
/** \class ImageWidgetController
*   \brief This controller processes the events from a set of image
*   widget.
*   Events are procesed by instances of the ImageWidgetActionHandler
*   class which are registered dynamically via the AddActionHandler
*   method.
*  \sa ImageWidget
*  \sa ImageWidgetActionHandler
*/

class ImageWidgetController
      : public itk::Object
{
public:
  /** Standard class typedefs */
  typedef ImageWidgetController             Self;
  typedef itk::Object                       Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Runtime information */
  itkTypeMacro(ImageWidgetController,Object);

  /** Handler typedef */
  typedef otb::ImageWidgetActionHandler      ActionHandlerType;
  typedef otb::ObjectList<ActionHandlerType> ActionHandlerListType;
  typedef ActionHandlerListType::Pointer     ActionHandlerListPointerType;

  /** Add an action handler
   *  \param handler The action handler.
   **/
  virtual void AddActionHandler(ActionHandlerType * handler);

  /** Remove an action handler if it exists
   *  \param index The index of the action handler.
   **/
  virtual void RemoveActionHandler(unsigned int index);

  /** Remove all actions handler
   */
  virtual void ClearAllActionHandlers();

  /** Handle an event from a widget
   * \param widgetId The id of the widget the event comes from
   * \param event The event code
   */
  int HandleWidgetEvent(std::string widgetId, int event);

  /** Handle widget resize
   * \param widgetId The id of the resized widget
   * \param w new width
   * \param h new height
   */
  void HandleWidgetResize(std::string widgetId, int w, int h);

  /** Handle widget move
  * \param widgetId The id of the resized widget
  * \param x new x location
  * \param y new y location
  */
  void HandleWidgetMove(std::string widgetId, int x, int y);

protected:
  /** Constructor */
  ImageWidgetController();
  /** Destructor */
  ~ImageWidgetController();
  /** Printself method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const;

private:
  ImageWidgetController(const Self&);    // purposely not implemented
  void operator=(const Self&); // purposely not implemented

  /** Action handlers list */
  ActionHandlerListPointerType m_ActionHandlersList;

}; // end class
} // end namespace otb
#endif


