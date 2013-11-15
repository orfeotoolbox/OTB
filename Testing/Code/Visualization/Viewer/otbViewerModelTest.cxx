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
#include "otbViewerViewGUI.h"
#include "otbViewerModel.h"
#include "otbViewerController.h"


/**
* Pay attention, we have to instanciated the full MVC because the
* model calls NotyfyAll taht changes the View...
*/

int otbViewerModelTest(int argc, char* argv[])
{
  if (argc!=3)
  {
    std::cout<<"Usage: "<<argv[0]<<" fname1 fname2"<<std::endl;
    return EXIT_FAILURE;
  }

  const char * inName = argv[1];
  const char * inName2 = argv[2];

  typedef otb::ViewerController ControllerType;
  typedef otb::ViewerViewGUI ViewType;
  typedef otb::ViewerModel ModelType;
  typedef ModelType::ObjectTrackedList ObjectTrackedList;


  ControllerType::Pointer controller = ControllerType::New();
  ViewType::Pointer view = ViewType::New();
  controller->SetView(view);
  view->SetViewerController(controller);

  view->Show();

  // model instance
  ModelType::Pointer model = ModelType::GetInstance();
  model->OpenImage(inName);

  Fl::check();

  /** Adding a second image*/
  model->OpenImage(inName2);
  Fl::check();

  view->guiMainWindow->hide();

  /** Reading all the component stored in the list*/
  ObjectTrackedList  modelList = model->GetObjectList();


  ObjectTrackedList::iterator it = modelList.begin();
  while(it != modelList.end() )
    {
      std::cout << " filename  "<<  (*it).pFileName << std::endl;
      std::cout << " reader    "<<  (*it).pReader->GetOutput()->GetLargestPossibleRegion().GetSize() << std::endl;
      std::cout << " quicklook "<<  (*it).pQuicklook->GetLargestPossibleRegion().GetSize() << std::endl;
      std::cout << " layer     "<<  (*it).pLayer << std::endl;
      ++it;
    }


  return EXIT_SUCCESS;
}

int otbViewerModelWithAddNumTest(int argc, char* argv[])
{
  if (argc!=3)
  {
    std::cout<<"Usage: "<<argv[0]<<" fname id"<<std::endl;
    return EXIT_FAILURE;
  }

  const char * inName = argv[1];
  unsigned int id     = atoi(argv[2]);

  typedef otb::ViewerController ControllerType;
  typedef otb::ViewerViewGUI ViewType;
  typedef otb::ViewerModel ModelType;
  typedef ModelType::ObjectTrackedList ObjectTrackedList;

  ControllerType::Pointer controller = ControllerType::New();
  ViewType::Pointer view = ViewType::New();
  controller->SetView(view);
  view->SetViewerController(controller);

  view->Show();

  // model instance
  ModelType::Pointer model = ModelType::GetInstance();
  model->OpenImage(inName, id);

  Fl::check();
  view->guiMainWindow->hide();

  /** Reading all the component stored in the list*/
  ObjectTrackedList  modelList = model->GetObjectList();


  ObjectTrackedList::iterator it = modelList.begin();
  while(it != modelList.end() )
    {
      std::cout << " filename "<<  (*it).pFileName << std::endl;
      std::cout << " reader   "<<  (*it).pReader->GetOutput()->GetLargestPossibleRegion().GetSize() << std::endl;
      std::cout << " quicklook "<< (*it).pQuicklook->GetLargestPossibleRegion().GetSize() << std::endl;
      std::cout << " layer    "<<  (*it).pLayer << std::endl;
      ++it;
    }


  return EXIT_SUCCESS;
}
