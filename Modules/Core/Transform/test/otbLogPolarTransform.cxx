/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "otbLogPolarTransform.h"
#include "itkPoint.h"
#include "otbMacro.h"
#include "otbMath.h"
#include <fstream>
#include <cstdlib>

int otbLogPolarTransform(int itkNotUsed(argc), char* argv[])
{
  double       radialStep = atof(argv[1]);
  double       angularStep = atof(argv[2]);
  const char * outputFilename(argv[3]);
  unsigned int nbPoints = atoi(argv[4]);

  typedef double                                PrecisionType;
  typedef otb::LogPolarTransform<PrecisionType> LogPolarTransformType;
  typedef itk::Point<PrecisionType, 2>          PointType;
  typedef std::vector<PointType>                PointsVectorType;

  std::ofstream file;
  file.open(outputFilename);
  file << "input points retrieval : " << std::endl;
  // input points retrieval
  PointsVectorType vect;
  for (unsigned int i = 0; i < nbPoints; ++i)
    {
    PointType p;
    p[0] = atof(argv[5 + 2 * i]);
    p[1] = atof(argv[6 + 2 * i]);
    file << "Adding point " << p << "." << std::endl;
    vect.push_back(p);
    }

  // Instantiation
  LogPolarTransformType::Pointer        transform = LogPolarTransformType::New();
  LogPolarTransformType::ParametersType params(4);
  params[0] = 0.;
  params[1] = 0.;
  params[2] = radialStep;
  params[3] = angularStep;
  transform->SetParameters(params);

  file << "Transform calculation ... :" << std::endl;
  for (PointsVectorType::iterator it = vect.begin(); it != vect.end(); ++it)
    {
    PointType p = transform->TransformPoint(*it);

    PointType pprime;

    double theta = (*it)[0] * angularStep * otb::CONST_PI / 180.0;
    double logRho   = (*it)[1] * radialStep;

    file << "Rho: " << logRho << ", Theta: " << theta << std::endl;
    pprime[0] = std::exp(logRho) * std::cos(theta);
    pprime[1] = std::exp(logRho) * std::sin(theta);

    file << "Original Point: " << (*it) << ", Reference point: " << pprime << ", Transformed point: " << p <<
    std::endl << std::endl;
    }

  file.close();

  return EXIT_SUCCESS;
}
