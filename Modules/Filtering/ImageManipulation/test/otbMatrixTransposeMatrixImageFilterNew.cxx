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
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbStreamingMatrixTransposeMatrixImageFilter.h"

int otbMatrixTransposeMatrixImageFilterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const unsigned int Dimension = 2;
  typedef unsigned char InputPixelType;

  typedef otb::VectorImage<InputPixelType, Dimension> InputImage1Type;
  typedef otb::VectorImage<InputPixelType, Dimension> InputImage2Type;
  typedef otb::StreamingMatrixTransposeMatrixImageFilter<InputImage1Type,
      InputImage2Type> MatrixTransposeMatrixImageFilterType;

  // Instantiation
  MatrixTransposeMatrixImageFilterType::Pointer filter = MatrixTransposeMatrixImageFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
