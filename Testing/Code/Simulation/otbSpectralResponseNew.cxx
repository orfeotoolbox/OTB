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

#include "otbMacro.h"

#include "otbSpectralResponse.h"

int otbSpectralResponseNew(int argc, char * argv[])
{
  typedef otb::SpectralResponse< double, double>  ResponseType;
  typedef ResponseType::Pointer  ResponsePointerType;


  //Instantiation
  ResponsePointerType  myResponse=ResponseType::New();

  return EXIT_SUCCESS;
}
