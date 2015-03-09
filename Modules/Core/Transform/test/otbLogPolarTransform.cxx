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
    pprime[0] = vcl_exp(logRho) * vcl_cos(theta);
    pprime[1] = vcl_exp(logRho) * vcl_sin(theta);

    file << "Original Point: " << (*it) << ", Reference point: " << pprime << ", Transformed point: " << p <<
    std::endl << std::endl;
    }

  file.close();

  return EXIT_SUCCESS;
}
