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
#include "otbMorphologicalOpeningProfileFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImage.h"

#include "itkMacro.h"

int otbMorphologicalOpeningProfileFilterNew(int argc, char * argv[])
{
  const unsigned int Dimension = 2;
  typedef double InputPixelType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, Dimension> StructuringElementType;
  typedef otb::MorphologicalOpeningProfileFilter<InputImageType, InputImageType, StructuringElementType>
  OpeningProfileFilterType;

  // Instantiation
  OpeningProfileFilterType::Pointer filter = OpeningProfileFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
