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

#include "otbClosingOpeningMorphologicalFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImage.h"

int otbClosingOpeningMorphologicalFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  const unsigned int Dimension = 2;
  typedef unsigned char                                                InputPixelType;
  typedef unsigned char                                                OutputPixelType;
  typedef otb::Image<InputPixelType, Dimension>                        InputImageType;
  typedef otb::Image<OutputPixelType, Dimension>                       OutputImageType;
  typedef itk::BinaryBallStructuringElement<InputPixelType, Dimension> StructuringElementType;
  typedef otb::ClosingOpeningMorphologicalFilter<InputImageType, OutputImageType,
      StructuringElementType> ClosingOpeningFilterType;

  // instantiating the opening closing filter
  ClosingOpeningFilterType::Pointer openingClosing = ClosingOpeningFilterType::New();

  std::cout << openingClosing << std::endl;

  return EXIT_SUCCESS;
}
