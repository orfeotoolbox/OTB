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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationRegistry.h"
#include "itkStdStreamLogOutput.h"

using otb::Wrapper::Application;
using otb::Wrapper::ApplicationRegistry;

int otbReadImageInfoTest(int argc, char* argv[])
{
  ApplicationRegistry::SetApplicationPath(argv[1]);
  const char* in = argv[2];

  Application::Pointer app = ApplicationRegistry::CreateApplication("ReadImageInfo");

  itk::StdStreamLogOutput::Pointer coutput = itk::StdStreamLogOutput::New();
  coutput->SetStream(std::cout);
  app->GetLogger()->AddLogOutput(coutput);
  app->GetLogger()->SetPriorityLevel(itk::LoggerBase::DEBUG);

  app->SetParameterString("in", in );
  app->ExecuteAndWriteOutput();

  return EXIT_SUCCESS;
}
