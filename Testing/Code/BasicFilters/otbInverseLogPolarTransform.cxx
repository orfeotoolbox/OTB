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
#include "otbInverseLogPolarTransform.h"
#include "otbMacro.h"
#include <fstream>
#include <cstdlib>
#include "otbMath.h"

int otbInverseLogPolarTransform(int itkNotUsed(argc), char* argv[])
{
  double       radialStep = atof(argv[1]);
  double       angularStep = atof(argv[2]);
  const char * outputFilename(argv[3]);
  unsigned int nbPoints = atoi(argv[4]);

  typedef double                                       PrecisionType;
  typedef otb::InverseLogPolarTransform<PrecisionType> InverseLogPolarTransformType;
  typedef itk::Point<PrecisionType, 2>                 PointType;
  typedef std::vector<PointType>                       PointsVectorType;

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
  InverseLogPolarTransformType::Pointer        transform = InverseLogPolarTransformType::New();
  InverseLogPolarTransformType::ParametersType params(4);
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
    double    rho = vcl_sqrt((*it)[0] * (*it)[0] + (*it)[1] * (*it)[1]);

    if (rho > 0)
      {
      pprime[0] = (1. / angularStep) * vcl_asin((*it)[1] / rho);
      pprime[0] = pprime[0] * (180. / otb::CONST_PI);
      // Deplacing the range to [0, 90], [270, 360]
      pprime[0] = pprime[0] > 0. ? pprime[0] : pprime[0] + 360.;
      // Avoiding asin indetermination
      if (p[0] >= 0)
        {
        pprime[0] = pprime[0] < 90. ? pprime[0] + 90. : pprime[0] - 90.;
        }
      pprime[1] = (1. / radialStep) * vcl_log(rho);
      }
    else
      {
      pprime[0] = 400.;
      pprime[1] = 0.;
      }

    file << "Original Point: " << (*it) << ", Reference point: " << pprime << ", Transformed point: " << p << std::endl;
    }
  file.close();

  return EXIT_SUCCESS;
}
