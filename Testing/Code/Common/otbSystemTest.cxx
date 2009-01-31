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

#include "itkExceptionObject.h"
#include <iostream>
#include <cstdlib>

#include "itkMacro.h"
#include "otbSystem.h"


int otbSystemTest( int argc, char* argv[] )
{
  const char * inputFileName  = argv[1];
  const char * inputDirName   = argv[2];
  itk::OStringStream msg;

  if ( otb::System::IsAFileName( inputFileName ) == false )
  {
    itkGenericExceptionMacro( <<"System::IsAFileName() error : the filename "<<inputFileName<<" is not detected.");
  }
  if ( otb::System::IsADirName( inputFileName ) == true )
  {
    itkGenericExceptionMacro( <<"System::IsADirName() error : the filename "<<inputFileName<<" is detected.");
  }
  if ( otb::System::IsAFileName( inputDirName ) == true )
  {
    itkGenericExceptionMacro(<< "System::IsAFileName() error : the dirname "<<inputDirName<<" is detected!!");
  }
  if ( otb::System::IsADirName( inputDirName ) == false )
  {
    itkGenericExceptionMacro(<< "System::IsADirName() error : the dirname "<<inputDirName<<" is not detected!!");
  }



  return EXIT_SUCCESS;
}


