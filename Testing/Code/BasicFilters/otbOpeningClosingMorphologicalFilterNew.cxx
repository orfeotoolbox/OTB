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
#include "itkMacro.h"

#include "otbOpeningClosingMorphologicalFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImage.h"

int otbOpeningClosingMorphologicalFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  const unsigned int Dimension = 2;
  typedef unsigned char                                                InputPixelType;
  typedef unsigned char                                                OutputPixelType;
  typedef otb::Image<InputPixelType, Dimension>                        InputImageType;
  typedef otb::Image<OutputPixelType, Dimension>                       OutputImageType;
  typedef itk::BinaryBallStructuringElement<InputPixelType, Dimension> StructuringElementType;
  typedef otb::OpeningClosingMorphologicalFilter<InputImageType, OutputImageType,
      StructuringElementType> OpeningClosingFilterType;

  // instantiating the opening closing filter
  OpeningClosingFilterType::Pointer openingClosing = OpeningClosingFilterType::New();

  std::cout << openingClosing << std::endl;

  return EXIT_SUCCESS;
}
