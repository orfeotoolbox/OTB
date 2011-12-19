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
#include "otbCommandLineArgumentParser.h"
#include "otbMsgReporter.h"

#include "otbI18n.h"

int main(int argc, char* argv[])
{

    otbI18nMacro();

  // Parse command line parameters
//   typedef otb::CommandLineArgumentParser ParserType;
//   ParserType::Pointer parser = ParserType::New();

//   //parser->AddInputImage(false); //Optionnal parameter
//   parser->AddOptionNParams("--InputImage","Input Images","-in", false);

//   typedef otb::CommandLineArgumentParseResult ParserResultType;
//   ParserResultType::Pointer  parseResult = ParserResultType::New();

//   try
//   {
//     parser->ParseCommandLine(argc, argv, parseResult);
//   }
//   catch ( itk::ExceptionObject & err )
//   {
//     std::string descriptionException = err.GetDescription();
//     if (descriptionException.find("ParseCommandLine(): Help Parser") != std::string::npos)
//     {
//       std::cout << "WARNING : output file pixels are converted in 'unsigned char'" << std::endl;
//       return EXIT_SUCCESS;
//     }
//     if (descriptionException.find("ParseCommandLine(): Version Parser") != std::string::npos)
//     {
//       return EXIT_SUCCESS;
//     }
//     return EXIT_FAILURE;
//   }

  typedef otb::ImageViewerManagerController ControllerType;
  typedef otb::ImageViewerManagerViewGUI ViewType;

  ControllerType::Pointer controller = ControllerType::New();
  ViewType::Pointer view = ViewType::New();
  controller->SetView(view);
  view->SetImageViewerManagerController(controller);
  //otb::MsgReporter::GetInstance()->SetTitle("Image Viewer Manager application");

  //
  view->Show();
  for (int i = 1; i<argc; ++i)
    {
      view->OpenImage(argv[i]);
    }

//   if ( parseResult->IsOptionInputImagePresent() )
//     {
//       for(int i = 0; i<parseResult->GetNumberOfParameters("--InputImage"); i++ )
//        {
//          view->OpenImage(parseResult->GetParameterString("--InputImage", i).c_str());
//        }
//     }
  Fl::check();

  otbGenericMsgDebugMacro(<<"Running GUI ...");
  Fl::run();

}
