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
#include "otbImageOfVectorsToMonoChannelExtractROI.h"

int otbImageOfVectorsToMonoChannelExtractROINew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  const int Dimension = 2;
  typedef unsigned char                  ScalarPixelType;
  typedef itk::Vector<double, Dimension> VectorPixelType;

  typedef otb::Image<ScalarPixelType, Dimension>                                       ScalarImageType;
  typedef otb::Image<VectorPixelType, Dimension>                                       VectorImageType;
  typedef otb::ImageOfVectorsToMonoChannelExtractROI<VectorImageType, ScalarImageType> FilterType;

  // Instantiating object
  FilterType::Pointer object = FilterType::New();

  std::cout << object << std::endl;

  return EXIT_SUCCESS;
}
