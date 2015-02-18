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
#include "itkFixedArray.h"
#include "otbLandsatTMIndices.h"

int otbLandsatTMIndexNDSIVis(int itkNotUsed(argc), char * argv[])
{
  typedef double                           OutputPixelType;
  typedef itk::FixedArray< double, 8 >     InputPixelType;

  typedef otb::Functor::LandsatTM::NDSIVis<InputPixelType, OutputPixelType> FunctorType;

  FunctorType ndsiVisFunct = FunctorType();

  double TM1 = (::atof(argv[1]));
  double TM2 = (::atof(argv[2]));
  double TM3 = (::atof(argv[3]));
  double TM4 = (::atof(argv[4]));
  double TM5 = (::atof(argv[5]));
  double TM61 = (::atof(argv[6]));
  double TM62 = (::atof(argv[7]));
  double TM7 = (::atof(argv[8]));

  double vis = (TM1+TM2+TM3)/3.0;
  double goodResult = (vis-TM5)/(vis+TM5+ndsiVisFunct.GetEpsilonToBeConsideredAsZero());

  std::cout << goodResult;

  InputPixelType pixel;
  pixel[0] = TM1;
  pixel[1] = TM2;
  pixel[2] = TM3;
  pixel[3] = TM4;
  pixel[4] = TM5;
  pixel[5] = TM61;
  pixel[6] = TM62;
  pixel[7] = TM7;

  double result = ndsiVisFunct(pixel);

  std::cout << " " << result << std::endl;

  if( result!=goodResult ) return EXIT_FAILURE;

  return EXIT_SUCCESS;
}
