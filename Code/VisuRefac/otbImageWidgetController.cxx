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
#ifndef __otbImageWidgetController_cxx
#define __otbImageWidgetController_cxx

#include "otbImageWidgetController.h"

namespace otb
{
ImageWidgetController::ImageWidgetController()
{
  // Instantiate action handlers list
  m_ActionHandlersList = ActionHandlerListType::New();
}

ImageWidgetController::~ImageWidgetController()
{}

void ImageWidgetController::AddActionHandler(ActionHandlerType * handler)
{
  // Add the handler to the list
  m_ActionHandlersList->PushBack(handler);
}

void ImageWidgetController::RemoveActionHandler(unsigned int index)
{
  // Remove the given handler
  m_ActionHandlersList->Erase(index);
}

void ImageWidgetController::ClearAllActionHandlers()
{
  // Clear the handlers list
  m_ActionHandlersList->Clear();
}

int ImageWidgetController::HandleWidgetEvent(std::string widgetId, int event)
{
  // Define an iterator on the action handlers list
  ActionHandlerListType::Iterator it = m_ActionHandlersList->Begin();

  // Found indicates if a handler was found to respond to this event
  bool found = false;

  // The action handler found
  ActionHandlerType * handler;

  while (!found && it!=m_ActionHandlersList->End())
  {
    // Get the current handler
    handler = it.Get();
    // Check if it listens to (widget,event)
    found   =  handler->HandleWidgetEvent(widgetId,event);
    ++it;
  }
  // If an handler was found, use it
  if (found)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

void ImageWidgetController::HandleWidgetResize(std::string widgetId, int w, int h)
{
  // Define an iterator on the action handlers list
  ActionHandlerListType::Iterator it = m_ActionHandlersList->Begin();

  // Found indicates if a handler was found to respond to this event
  bool found = false;

  // The action handler found
  ActionHandlerType * handler;

  while (!found && it!=m_ActionHandlersList->End())
  {
    // Get the current handler
    handler = it.Get();
    // try to handle the event with the current handler
    found = handler->HandleWidgetResize(widgetId,w,h);
    ++it;
  }
}

void ImageWidgetController::HandleWidgetMove(std::string widgetId, int x, int y)
{
  // Define an iterator on the action handlers list
  ActionHandlerListType::Iterator it = m_ActionHandlersList->Begin();

  // Found indicates if a handler was found to respond to this event
  bool found = false;

  // The action handler found
  ActionHandlerType * handler;

  while (!found && it!=m_ActionHandlersList->End())
  {
    // Get the current handler
    handler = it.Get();
    // Check if it listens to (widget,event)
    found   = handler->HandleWidgetMove(widgetId,x,y);
    ++it;
  }
}


void ImageWidgetController::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os<<indent<<"Number of action handlers: "<<m_ActionHandlersList->Size()<<std::endl;
}


} // end namespace otb

#endif
