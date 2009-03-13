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
#include "otbImageWidgetActionHandler.h"

int otbImageWidgetController( int argc, char * argv[] )
{
  // Typedefs
  typedef otb::ImageWidgetController    ControllerType;
  typedef otb::ImageWidgetActionHandler ActionHandlerType;

  // Instantiate the controller
  ControllerType::Pointer controller = ControllerType::New();

  // Adding three action handler
  controller->AddActionHandler(ActionHandlerType::New());
  controller->AddActionHandler(ActionHandlerType::New());
  controller->AddActionHandler(ActionHandlerType::New());

  // Clearing 
  controller->ClearAllActionHandlers();

  // Adding three handlers again
  controller->AddActionHandler(ActionHandlerType::New());
  controller->AddActionHandler(ActionHandlerType::New());
  controller->AddActionHandler(ActionHandlerType::New());

  // Remove the third
  controller->RemoveActionHandler(2);

  // Simulate an event from a widget
  controller->HandleWidgetEvent("default",10);

  // Simulate a resize from a widget
  controller->HandleWidgetResize("default",0,0);

  // Simulate a move from a widget
  controller->HandleWidgetMove("default",0,0);

  return EXIT_SUCCESS;
}
