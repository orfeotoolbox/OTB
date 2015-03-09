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

#include "otbWindowedSincInterpolateImageLanczosFunction.h"
#include "itkConstantBoundaryCondition.h"
#include "otbImage.h"
#include "otbImageFileReader.h"

int otbWindowedSincInterpolateImageLanczosFunction(int argc, char * argv[])
{
  const char * infname = argv[1];
  const char * outfname = argv[2];

  typedef otb::Image<double, 2>                                       ImageType;
  typedef otb::WindowedSincInterpolateImageLanczosFunction<ImageType> InterpolatorType;
  typedef InterpolatorType::ContinuousIndexType                       ContinuousIndexType;

  typedef otb::ImageFileReader<ImageType> ReaderType;

  // Instantiating objects
  InterpolatorType::Pointer interp = InterpolatorType::New();

  unsigned int i = 4;

  std::vector<ContinuousIndexType> indicesList;

  while (i < static_cast<unsigned int>(argc) && (i + 1) < static_cast<unsigned int>(argc))
    {
    ContinuousIndexType idx1;
    idx1[0] = atof(argv[i]);
    idx1[1] = atof(argv[i + 1]);
    indicesList.push_back(idx1);

    i += 2;
    }

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->Update();
  interp->SetInputImage(reader->GetOutput());
  interp->SetRadius(atoi(argv[3]));
  interp->Initialize();

  std::ofstream file;
  file.open(outfname);

  file << "Lanczos Window Function" << std::endl;
  for (std::vector<ContinuousIndexType>::iterator it = indicesList.begin(); it != indicesList.end(); ++it)
    {
    file << (*it) << " -> " << interp->EvaluateAtContinuousIndex((*it)) << std::endl;
    }
  file << std::endl;

  file.close();

  return EXIT_SUCCESS;
}
