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

#include "otbMarkovRandomFieldFilter.h"
#include "otbImage.h"


int otbMarkovRandomFieldFilterNew(int argc, char * argv[])
{
  typedef double PixelType;
  typedef otb::Image<PixelType, 2>                             ImageType;
  typedef otb::MarkovRandomFieldFilter< ImageType, ImageType>  MarkovRandomFieldFilterType;

  MarkovRandomFieldFilterType::Pointer object = MarkovRandomFieldFilterType::New();

  return EXIT_SUCCESS;
}


