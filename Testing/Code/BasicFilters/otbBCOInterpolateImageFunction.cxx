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
#include "itkExceptionObject.h"

#include "otbBCOInterpolateImageFunction.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include <fstream>

int otbBCOInterpolateImageFunctionNew(int argc, char * argv[])
{
  typedef otb::Image<double, 2>                                           ImageType;
  typedef otb::BCOInterpolateImageFunction<ImageType, double>            InterpolatorType;

  // Instantiating object
  InterpolatorType::Pointer filter = InterpolatorType::New();

  return EXIT_SUCCESS;
}

int otbBCOInterpolateImageFunction(int argc, char * argv[])
{
  const char * infname      = argv[1];
  const char * outfname     = argv[2];
  const unsigned int radius = atoi(argv[3]);
  const double alpha        = atof(argv[4]);
  
  //typedef otb::VectorImage<double, 2>                         ImageType;
  
  typedef otb::Image<double, 2>                               ImageType;
  typedef otb::BCOInterpolateImageFunction<ImageType, double> InterpolatorType;
  typedef InterpolatorType::ContinuousIndexType               ContinuousIndexType;
  typedef otb::ImageFileReader<ImageType>                     ReaderType;

  int i = 5;
  
  std::vector<ContinuousIndexType> indicesList;
  
  while (i < argc && (i + 1) < argc)
    {
    ContinuousIndexType idx;
    
    idx[0] = atof(argv[i]);
    idx[1] = atof(argv[i + 1]);
    
    indicesList.push_back(idx);
    
    i += 2;
    }

  // Instantiating object
  InterpolatorType::Pointer filter = InterpolatorType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->Update();//TODO check if necessary

  std::cout << "Alpha Checking : " << std::endl << filter->GetAlpha() << std::endl;
  filter->SetAlpha(-1.0);
  std::cout << filter->GetAlpha() << std::endl;
  filter->SetAlpha(alpha);
  std::cout << filter->GetAlpha() << std::endl;
  std::cout << "Radius Checking : " << std::endl << filter->GetRadius() << std::endl;
  filter->SetRadius(radius);
  std::cout << filter->GetRadius() << std::endl;
  std::cout << "Coef Checking : " << std::endl;
  filter->EvaluateCoef();
  std::cout << "Coef Generated ! " << std::endl;

  filter->SetInputImage(reader->GetOutput());
  
  std::ofstream file;
  file.open(outfname);

  for (std::vector<ContinuousIndexType>::iterator it = indicesList.begin(); it != indicesList.end(); ++it)
    {
    file << (*it) << " -> " << filter->EvaluateAtContinuousIndex((*it)) << std::endl;
    }

  file.close();


  return EXIT_SUCCESS;
}
