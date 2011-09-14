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

#include "otbInternalReader.cxx"
#include "otbWrapperStringParameter.h"
#include "otbWrapperOutputImageParameter.h"


int otbInternalReaderNew(int argc, char* argv[])
{
  typedef otb::Wrapper::InternalReader InternalReaderType;
  InternalReaderType::Pointer appli = InternalReaderType::New();

  return EXIT_SUCCESS;
}

int otbInternalReaderTest1(int argc, char* argv[])
{
  typedef otb::Wrapper::InternalReader InternalReaderType;
  InternalReaderType::Pointer appli = InternalReaderType::New();

  appli->SetParameterString("inname", argv[1]);
  appli->SetParameterString("out", argv[2]);
  appli->ExecuteAndWriteOutput();

  return EXIT_SUCCESS;
}
