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
#include "otbImageWidgetController.h"
#include "otbMacro.h"

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

void ImageWidgetController::InsertActionHandler(const unsigned int id, ActionHandlerType * handler)
{
  // Insert the handler to the specified index in the list
  m_ActionHandlersList->Insert(m_ActionHandlersList->Begin()+id, handler);
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

int ImageWidgetController::HandleWidgetEvent(const std::string& widgetId, int event)
{
  // Define an iterator on the action handlers list
  ActionHandlerListType::Iterator it = m_ActionHandlersList->Begin();

  // Found indicates if a handler was found to respond to this event
  bool found = false;

  // The action handler found
  ActionHandlerType * handler;

  while (it != m_ActionHandlersList->End())
    {
    // Get the current handler
    handler = it.Get();
    // Check if it listens to (widget, event)
    bool newFound = handler->HandleWidgetEvent(widgetId, event);
    found   =  found || newFound;
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

void ImageWidgetController::HandleWidgetResize(const std::string& widgetId, int w, int h)
{
  // Define an iterator on the action handlers list
  ActionHandlerListType::Iterator it = m_ActionHandlersList->Begin();

  // The action handler found
  ActionHandlerType * handler;

  while (it != m_ActionHandlersList->End())
    {
    // Get the current handler
    handler = it.Get();
    // try to handle the event with the current handler
    handler->HandleWidgetResize(widgetId, w, h);
    ++it;
    }
}

void ImageWidgetController::HandleWidgetMove(const std::string& widgetId, int x, int y)
{
  otbMsgDevMacro(<< "ImageWidgetController::HandleWidgetMove(): (" << widgetId << ", " << x << ", " << y << ")");
  // Define an iterator on the action handlers list
  ActionHandlerListType::Iterator it = m_ActionHandlersList->Begin();

  // The action handler found
  ActionHandlerType * handler;

  while (it != m_ActionHandlersList->End())
    {
    // Get the current handler
    handler = it.Get();
    // Check if it listens to (widget, event)
    handler->HandleWidgetMove(widgetId, x, y);
    ++it;
    }
}

void ImageWidgetController::ActivateActionHandlers()
{
//   otbMsgDevMacro(<<"ImageWidgetController::DectivateActionHandler(): ("<<widgetId<<")");
// Define an iterator on the action handlers list
  ActionHandlerListType::Iterator it = m_ActionHandlersList->Begin();

  // The action handler found
  ActionHandlerType * handler;

  while (it != m_ActionHandlersList->End())
    {
    // Get the current handler
    handler = it.Get();
    // Check if it listens to (widget, event)
    handler->SetIsActive(true);
    ++it;
    }
}

void ImageWidgetController::DeactivateActionHandlers()
{
//   otbMsgDevMacro(<<"ImageWidgetController::DectivateActionHandler(): ("<<widgetId<<")");
// Define an iterator on the action handlers list
  ActionHandlerListType::Iterator it = m_ActionHandlersList->Begin();

  // The action handler found
  ActionHandlerType * handler;

  while (it != m_ActionHandlersList->End())
    {
    // Get the current handler
    handler = it.Get();
    // Check if it listens to (widget, event)
    handler->SetIsActive(false);
    ++it;
    }
}

void ImageWidgetController::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Number of action handlers: " << m_ActionHandlersList->Size() << std::endl;
}

} // end namespace otb
