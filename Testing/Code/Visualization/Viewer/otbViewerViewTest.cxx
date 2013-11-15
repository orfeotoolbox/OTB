/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.


    This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE,  See the above copyright notices for more information.

=========================================================================*/
#include <iostream>

#include "otbViewerModel.h"
#include "otbViewerController.h"


int otbViewerViewTest(int argc, char* argv[])
{
  typedef otb::ViewerController ControllerType;
  typedef otb::ViewerViewGUI ViewType;


  ControllerType::Pointer controller = ControllerType::New();
  ViewType::Pointer view = ViewType::New();

  controller->SetView(view);
  view->SetViewerController(controller);

  view->Show();

  controller->OpenInputImage(argv[1]);

  Fl::check();
  view->guiMainWindow->hide();

  return EXIT_SUCCESS;
}
