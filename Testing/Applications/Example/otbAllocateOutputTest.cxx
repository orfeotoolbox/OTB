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

#include "otbAllocateOutput.cxx"
#include "otbWrapperOutputImageParameter.h"


int otbAllocateOutputNew(int argc, char* argv[])
{
  typedef otb::Wrapper::AllocateOutput AllocateOutputType;
  AllocateOutputType::Pointer appli = AllocateOutputType::New();

  return EXIT_SUCCESS;
}


int otbAllocateOutputTest1(int argc, char* argv[])
{
  typedef otb::Wrapper::AllocateOutput AllocateOutputType;
  AllocateOutputType::Pointer appli = AllocateOutputType::New();

  dynamic_cast<otb::Wrapper::OutputImageParameter *>(appli->GetParameterByKey("out"))->SetFileName(argv[1]);
  

  appli->ExecuteAndWriteOutput();

  return EXIT_SUCCESS;
}
