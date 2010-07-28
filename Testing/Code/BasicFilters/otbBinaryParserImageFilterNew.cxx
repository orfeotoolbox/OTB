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

#include "otbImage.h"
#include "otbBinaryParserImageFilter.h"

int otbBinaryParserImageFilterNew( int argc, char* argv[])
{
  typedef double                                                                                     PixelType;
  typedef otb::Image<PixelType, 2>                                                                   ImageType;
  typedef otb::BinaryParserImageFilter<ImageType>                                                    FilterType;
  
  FilterType::Pointer filter = FilterType::New();

  std::cout << "Number Of Threads  :  " << filter->GetNumberOfThreads() << std::endl;

  return EXIT_SUCCESS;
}
