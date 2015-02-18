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
#include "vcl_complex.h"
#include "vcl_cmath.h"
#include "otbPolarimetricSynthesisFunctor.h"

int otbPolarimetricSynthesisFunctor(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef std::complex<double>                   ComplexType;

  typedef otb::Functor::PolarimetricSynthesisFunctor<ComplexType, ComplexType, ComplexType, ComplexType, double >         FunctorType;

  double result(449.);
  double outputFunct(0.);
  FunctorType funct;
  FunctorType::ComplexArrayType l_Ei;
  FunctorType::ComplexArrayType l_Er;

  l_Ei[0] = ComplexType(0.5, 0);
  l_Ei[1] = ComplexType(1., 1.2);
  l_Er[0] = ComplexType(-2.5, 0.);
  l_Er[1] = ComplexType(0., 1.5);

  funct.SetEi(l_Ei);
  funct.SetEi(l_Er);

  outputFunct = funct.operator ()( ComplexType(1.5, 4.5), ComplexType(2.5, 3.5), ComplexType(3.5, 2.5), ComplexType(4.5, 1.5) );


  if( vcl_abs(result-outputFunct) > 1e-10 )
  {
    std::cout<<"Test gives :"<<std::endl;
    std::cout<<outputFunct<<std::endl;
    std::cout<<"Wanted results are :"<<std::endl;
    std::cout<<result<<std::endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
