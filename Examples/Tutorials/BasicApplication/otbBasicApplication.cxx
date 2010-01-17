
#include "itkExceptionObject.h"
#include "otbBasicApplicationController.h"
#include "otbBasicApplicationView.h"
#include "otbImageFileReader.h"
#include "otbCommandLineArgumentParser.h"
#include "otbMsgReporter.h"

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>

int main(int argc, char* argv[])
{

  // Parse command line parameters
  typedef otb::CommandLineArgumentParser ParserType;
  ParserType::Pointer parser = ParserType::New();

  parser->AddInputImage(false); //Optionnal parameter

  typedef otb::CommandLineArgumentParseResult ParserResultType;
  ParserResultType::Pointer  parseResult = ParserResultType::New();

  try
  {
    parser->ParseCommandLine(argc,argv,parseResult);
  }
  catch ( itk::ExceptionObject & err )
  {
    std::string descriptionException = err.GetDescription();
    if (descriptionException.find("ParseCommandLine(): Help Parser") != std::string::npos)
    {
      std::cout << "WARNING : output file pixels are converted in 'unsigned char'" << std::endl;
      return EXIT_SUCCESS;
    }
    if (descriptionException.find("ParseCommandLine(): Version Parser") != std::string::npos)
    {
      return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
  }

  typedef otb::BasicApplicationController ControllerType;
  typedef otb::BasicApplicationView ViewType;
  otb::MsgReporter::GetInstance()->SetTitle("BasicApplication Application");

  ControllerType::Pointer controller = ControllerType::New();
  ViewType::Pointer view = ViewType::New();
  controller->SetView(view);
  view->SetController(controller);
  view->SetWidgetsController(controller->GetWidgetsController());
  view->Build();

  if ( parseResult->IsOptionPresent("--InputImage") )
    view->GetController()->OpenImage(parseResult->GetInputImage().c_str());
  Fl::run();

  return EXIT_SUCCESS;
}
