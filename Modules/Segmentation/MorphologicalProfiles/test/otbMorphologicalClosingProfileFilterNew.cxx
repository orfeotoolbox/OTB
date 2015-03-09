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
#include "otbMorphologicalClosingProfileFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImage.h"

#include "itkMacro.h"

int otbMorphologicalClosingProfileFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef double InputPixelType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef itk::BinaryBallStructuringElement<InputPixelType, Dimension> StructuringElementType;
  typedef otb::MorphologicalClosingProfileFilter<InputImageType, InputImageType, StructuringElementType>
  ClosingProfileFilterType;

  // Instantiation
  ClosingProfileFilterType::Pointer filter = ClosingProfileFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
