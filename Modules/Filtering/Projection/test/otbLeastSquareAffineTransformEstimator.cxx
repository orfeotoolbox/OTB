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

#include <fstream>
#include <iostream>

#include "otbMacro.h"
#include "otbLeastSquareAffineTransformEstimator.h"
#include "itkPoint.h"

int otbLeastSquareAffineTransformEstimator(int argc, char* argv[])
{
  if (argc < 8)
    {
    std::cerr << "Usage: " << argv[0] << " outfname a1x a1y a1z b1x b1y b1z ... aNx aNy aNz bNx bNy bNz" << std::endl;
    return EXIT_FAILURE;
    }

  if ((argc - 2) % 6 != 0)
    {
    std::cerr << "Inconsistent points description." << std::endl;
    return EXIT_FAILURE;
    }

  const char * outfname = argv[1];

  typedef itk::Point<float, 3>                                PointType;
  typedef otb::LeastSquareAffineTransformEstimator<PointType> EstimatorType;

  // instantiation
  EstimatorType::Pointer estimator = EstimatorType::New();

  // nb points
  unsigned int nbTiePoints = (argc - 2) / 6;

  std::cout << "Reading " << nbTiePoints << " tie points from command line." << std::endl;

  for (unsigned int pId = 0; pId < nbTiePoints; ++pId)
    {
    PointType p1, p2;

    // Reading first point
    p1[0] = atof(argv[2 + pId * 6]);
    p1[1] = atof(argv[3 + pId * 6]);
    p1[2] = atof(argv[4 + pId * 6]);

    // Reading second point
    p2[0] = atof(argv[5 + pId * 6]);
    p2[1] = atof(argv[6 + pId * 6]);
    p2[2] = atof(argv[7 + pId * 6]);

    std::cout << "Adding tie points " << p1 << " <-> " << p2 << std::endl;

    // Add it to the estimator
    estimator->AddTiePoints(p1, p2);
    }

  // Trigger computation
  estimator->Compute();

  std::ofstream ofs;
  ofs.open(outfname);

  // Set floatfield to format properly
  ofs.setf(std::ios::fixed, std::ios::floatfield);
  ofs.precision(10);

  ofs << "Estimated affine matrix: " << std::endl;
  ofs << estimator->GetMatrix() << std::endl;
  ofs << "Estimated affine offset: " << std::endl;
  ofs << estimator->GetOffset() << std::endl;
  ofs << "RMS error: " << std::endl;
  ofs << estimator->GetRMSError() << std::endl;
  ofs << "Relative residual: " << std::endl;
  ofs << estimator->GetRelativeResidual() << std::endl;
  ofs.close();

  // Testing the GetTiePointsContainer() method
  EstimatorType::TiePointsContainerType container = estimator->GetTiePointsContainer();

  // Get the output transform */
  EstimatorType::AffineTransformPointerType transform = estimator->GetAffineTransform();

  return EXIT_SUCCESS;
}
