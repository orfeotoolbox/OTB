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

#include "otbImageFileReader.h"
#include "otbPointSetDensityEpanechnikovFunction.h"
#include "itkPointSet.h"
#include "itkVariableLengthVector.h"

#include <iostream>

int otbPointSetDensityEpanechnikovFunctionTest(int itkNotUsed(argc), char* argv[])
{

  const char * outfname = argv[1];

  const unsigned int Dimension = 2;
  typedef float PixelType;

  typedef itk::VariableLengthVector<PixelType>                               RealVectorType;
  typedef itk::PointSet<RealVectorType, Dimension>                           PointSetType;
  typedef otb::PointSetDensityEpanechnikovFunction <PointSetType, PixelType> FunctionType;

  /**Instantiation of a Smart Pointer*/
  PointSetType::Pointer pointSet = PointSetType::New();
  FunctionType::Pointer filter = FunctionType::New();
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
