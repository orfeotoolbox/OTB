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

#include "otbImageSampleExtractorFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include <fstream>

int otbImageSampleExtractorFilterNew(int itkNotUsed(argc), char* itkNotUsed(argv) [])
{
  typedef otb::VectorImage<float> InputImageType;
  typedef otb::ImageSampleExtractorFilter<InputImageType> FilterType;
  
  FilterType::Pointer filter = FilterType::New();
  std::cout << filter << std::endl;
  return EXIT_SUCCESS;
}

int otbImageSampleExtractorFilter(int argc, char* argv[])
{
  typedef otb::VectorImage<float> InputImageType;
  typedef otb::ImageSampleExtractorFilter<InputImageType> FilterType;
  
  if (argc < 3)
    {
    std::cout << "Usage : "<<argv[0]<< " input_vector  output_stats" << std::endl;
    }
  
  
  
  return EXIT_SUCCESS;
}

