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

#include "otbImageToPointSetFilter.h"
#include "itkImage.h"
#include "itkPointSet.h"

int otbImageToPointSetFilterTest(int argc, char *argv[] )
{
  const unsigned int Dimension = 2;

  typedef unsigned char  PixelType;

  typedef itk::Image< PixelType, Dimension  >           ImageType;
  typedef itk::PointSet<PixelType, Dimension>           PointSetType;
  typedef otb::ImageToPointSetFilter<ImageType,PointSetType> FunctionType;

  //FunctionType::Pointer function = FunctionType::New();

  return EXIT_SUCCESS;
}



