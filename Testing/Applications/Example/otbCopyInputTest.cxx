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

#include "otbCopyInput.cxx"
#include "otbWrapperInputImageParameter.h"
#include "otbWrapperOutputImageParameter.h"


int otbCopyInputNew(int argc, char* argv[])
{
  typedef otb::Wrapper::CopyInput CopyInputType;
  CopyInputType::Pointer appli = CopyInputType::New();

  return EXIT_SUCCESS;
}


int otbCopyInputTest1(int argc, char* argv[])
{
  typedef otb::Wrapper::CopyInput CopyInputType;
  CopyInputType::Pointer appli = CopyInputType::New();

  dynamic_cast<otb::Wrapper::InputImageParameter *>(appli->GetParameterByKey("in"))->SetFromFileName(argv[1]);
  dynamic_cast<otb::Wrapper::OutputImageParameter *>(appli->GetParameterByKey("out"))->SetFileName(argv[2]);
  
  appli->ExecuteAndWriteOutput();

  return EXIT_SUCCESS;
}
