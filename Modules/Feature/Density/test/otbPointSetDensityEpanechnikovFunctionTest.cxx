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


#include "otbImageFileReader.h"
#include "otbPointSetDensityEpanechnikovFunction.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"

#include <iostream>

int otbPointSetDensityEpanechnikovFunctionTest(int itkNotUsed(argc), char* argv[])
{

  const char* outfname = argv[1];

  const unsigned int Dimension = 2;
  typedef float      PixelType;

  typedef itk::VariableLengthVector<PixelType> RealVectorType;
  typedef itk::PointSet<RealVectorType, Dimension>                          PointSetType;
  typedef otb::PointSetDensityEpanechnikovFunction<PointSetType, PixelType> FunctionType;

  /**Instantiation of a Smart Pointer*/
  PointSetType::Pointer pointSet = PointSetType::New();
  FunctionType::Pointer filter   = FunctionType::New();
  std::ofstream         outfile(outfname);

  /** Construction of the pointSet */
  PointSetType::PointIdentifier count = 0;

  PointSetType::PointType pDst, pSrc;
  pDst[0] = 12.78;
  pDst[1] = 18.76;
  pointSet->SetPoint(count++, pDst);

  pDst[0] = 15.78;
  pDst[1] = 23.76;
  pointSet->SetPoint(count++, pDst);

  pDst[0] = 9.78;
  pDst[1] = 5.76;
  pointSet->SetPoint(count++, pDst);

  filter->SetPointSet(pointSet);
  filter->SetRadius(2);

  /**Point we search around*/
  pDst[0] = 14.9;
  pDst[1] = 24;
  outfile << "Density computed for the point : " << pDst << " is " << filter->Evaluate(pDst) << std::endl;

  pDst[0] = 9;
  pDst[1] = 5;
  outfile << "Density computed for the point : " << pDst << " is " << filter->Evaluate(pDst) << std::endl;

  outfile.close();

  return EXIT_SUCCESS;
}
