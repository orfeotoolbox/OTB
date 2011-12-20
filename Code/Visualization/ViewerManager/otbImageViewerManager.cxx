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
#include "otbImageViewerManagerViewGUI.h"
#include "otbImageViewerManagerModel.h"
#include "otbImageViewerManagerController.h"

#include "otbMsgReporter.h"

#include "otbI18n.h"

int main(int argc, char* argv[])
{

  otbI18nMacro();
  
  typedef otb::ImageViewerManagerController ControllerType;
  typedef otb::ImageViewerManagerViewGUI ViewType;

  ControllerType::Pointer controller = ControllerType::New();
  ViewType::Pointer view = ViewType::New();
  controller->SetView(view);
  view->SetImageViewerManagerController(controller);

  view->Show();
  for (int i = 1; i<argc; ++i)
    {
      view->OpenImage(argv[i]);
    }

  Fl::check();

  otbGenericMsgDebugMacro(<<"Running GUI ...");
  Fl::run();

}
