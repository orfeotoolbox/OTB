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

#include "otbBSplineInterpolateImageFunction.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include <fstream>

int otbBSplineInterpolateImageFunction(int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  typedef otb::Image<double, 2>                                           ImageType;
  typedef otb::BSplineInterpolateImageFunction<ImageType, double, double> InterpolatorType;
  typedef InterpolatorType::ContinuousIndexType                           ContinuousIndexType;
  typedef otb::ImageFileReader<ImageType>                                 ReaderType;

  int i = 3;

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
  InterpolatorType::Pointer interpolator = InterpolatorType::New();

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->Update();

  interpolator->SetInputImage(reader->GetOutput());

  std::ofstream file;
  file.open(outfname);

  for (std::vector<ContinuousIndexType>::iterator it = indicesList.begin(); it != indicesList.end(); ++it)
    {
    file << (*it) << " -> " << interpolator->EvaluateAtContinuousIndex((*it)) << std::endl;
    }

  file.close();

  return EXIT_SUCCESS;
}
