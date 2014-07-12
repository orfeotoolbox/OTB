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

#include "otbMuellerToReciprocalCovarianceImageFilter.h"
#include "itkVariableLengthVector.h"
#include <complex>

int otbMuellerToReciprocalCovarianceFunctor(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef std::complex<double>                   ComplexType;
 typedef itk::VariableLengthVector<double> VectorDoubleType;
  typedef itk::VariableLengthVector<ComplexType> VectorComplexType;

  typedef otb::Functor::MuellerToReciprocalCovarianceFunctor<VectorDoubleType, VectorComplexType> FunctorType;

  VectorDoubleType input;
  input.SetSize(16);
  VectorComplexType outputFunct;
  outputFunct.SetSize(6);
  VectorComplexType result;
  result.SetSize(6);

  double val = 0;
  for(unsigned int i=0; i<input.Size(); ++i)
    {
      input[i] =  val;
      val += 0.5;
    }

  result[0] = ComplexType(2.5, 0.);
  result[1] = ComplexType(4., 5.);
  result[2] = ComplexType(-6.25, 0.75);
  result[3] = ComplexType(-8, 0.);
  result[4] = ComplexType(0.5, -0.5);
  result[5] = ComplexType(0., 0.);

  FunctorType funct;
  outputFunct = funct.operator ()( input );
  std::cout<<outputFunct<<std::endl;

  if( vcl_abs(result[0].real()-outputFunct[0].real()) > 1e-10 ||
      vcl_abs(result[0].imag()-outputFunct[0].imag()) > 1e-10 ||
      vcl_abs(result[1].real()-outputFunct[1].real()) > 1e-10 ||
      vcl_abs(result[1].imag()-outputFunct[1].imag()) > 1e-10 ||
      vcl_abs(result[2].real()-outputFunct[2].real()) > 1e-10 ||
      vcl_abs(result[2].imag()-outputFunct[2].imag()) > 1e-10 ||
      vcl_abs(result[3].real()-outputFunct[3].real()) > 1e-10 ||
      vcl_abs(result[3].imag()-outputFunct[3].imag()) > 1e-10 ||
      vcl_abs(result[4].real()-outputFunct[4].real()) > 1e-10 ||
      vcl_abs(result[4].imag()-outputFunct[4].imag()) > 1e-10 ||
      vcl_abs(result[5].real()-outputFunct[5].real()) > 1e-10 ||
      vcl_abs(result[5].imag()-outputFunct[5].imag()) > 1e-10    )

    {
      std::cout<<"Test gives :"<<std::endl;
      std::cout<<outputFunct<<std::endl;
      std::cout<<"Wanted results are :"<<std::endl;
      std::cout<<result<<std::endl;

      return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
