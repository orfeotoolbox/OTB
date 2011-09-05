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

#include "otbRescale.cxx"
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperOutputImageParameter.h"
#include "otbWrapperNumericalParameter.h"
#include "itkStdStreamLogOutput.h"

int otbRescaleNew(int argc, char* argv[])
{
  typedef otb::Wrapper::Rescale RescaleType;
  RescaleType::Pointer appli = RescaleType::New();

  return EXIT_SUCCESS;
}


int otbRescaleTest1(int argc, char* argv[])
{
  typedef otb::Wrapper::Rescale RescaleType;
  RescaleType::Pointer appli = RescaleType::New();
  
  itk::StdStreamLogOutput::Pointer coutput = itk::StdStreamLogOutput::New();
  coutput->SetStream(std::cout);

  appli->GetLogger()->AddLogOutput(coutput);
  appli->GetLogger()->SetPriorityLevel(itk::LoggerBase::DEBUG);

  appli->SetParameterString("in", argv[1]);
  appli->SetParameterString("out", argv[2]);
  appli->SetParameterFloat("outmin", atof(argv[3]));
  appli->SetParameterFloat("outmax", atof(argv[4]));
  appli->ExecuteAndWriteOutput();

  return EXIT_SUCCESS;
}
