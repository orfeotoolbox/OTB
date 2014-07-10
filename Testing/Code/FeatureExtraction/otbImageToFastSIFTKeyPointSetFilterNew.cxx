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
#include "otbSiftFastImageFilter.h"
#include "otbImage.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"

int otbImageToFastSIFTKeyPointSetFilterNew(int itkNotUsed(argc), char ** itkNotUsed(argv))
{
  typedef float RealType;
  const unsigned int Dimension = 2;

  typedef otb::Image<RealType, Dimension>                   ImageType;
  typedef itk::VariableLengthVector<RealType>               RealVectorType;
  typedef itk::PointSet<RealVectorType, Dimension>          PointSetType;
  typedef otb::SiftFastImageFilter<ImageType, PointSetType> ImageToFastSIFTKeyPointSetFilterType;

  // Instantiating object
  ImageToFastSIFTKeyPointSetFilterType::Pointer filter = ImageToFastSIFTKeyPointSetFilterType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}
