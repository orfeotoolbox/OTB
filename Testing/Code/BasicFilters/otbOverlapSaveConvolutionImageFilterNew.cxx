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

#include "otbImage.h"
#include "otbOverlapSaveConvolutionImageFilter.h"


int otbOverlapSaveConvolutionImageFilterNew( int argc, char * argv[] )
{
  typedef double        InputPixelType;
  typedef double   	OutputPixelType;
  const   unsigned int  Dimension = 2;
  
  typedef otb::Image< InputPixelType,  Dimension >                  InputImageType;
  typedef otb::Image< OutputPixelType, Dimension >                  OutputImageType;
  typedef otb::OverlapSaveConvolutionImageFilter< InputImageType,OutputImageType >   FilterType;
  
  FilterType::Pointer conv = FilterType::New();
  
  return EXIT_SUCCESS;
}

