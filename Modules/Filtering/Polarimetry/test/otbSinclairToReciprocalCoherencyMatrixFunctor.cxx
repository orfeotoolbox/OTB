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

#include "otbSinclairToReciprocalCoherencyMatrixFunctor.h"
#include "itkVariableLengthVector.h"

int otbSinclairToReciprocalCoherencyMatrixFunctor(int itkNotUsed(argc), char * itkNotUsed(argv)[])
{
  typedef std::complex<double>                   ComplexType;
  typedef itk::VariableLengthVector<ComplexType> OutputType;

  typedef otb::Functor::SinclairToReciprocalCoherencyMatrixFunctor<ComplexType, ComplexType, ComplexType, OutputType >         FunctorType;

  OutputType  result(6);
  FunctorType funct;
  OutputType outputFunct;

  result[0] = ComplexType(26.,  0.);
  result[1] = ComplexType( 2., -10.);
  result[2] = ComplexType(26.,  0.);
  result[3] = ComplexType( 4.,  0.);
  result[4] = ComplexType( 2., 10.);
  result[5] = ComplexType(26.,  0.);

 outputFunct = funct.operator ()( ComplexType(1., 4.), ComplexType(2., 3.), ComplexType(3., 2.) );
  if( vcl_abs(result[0]-outputFunct[0]) > 1e-10 ||
      vcl_abs(result[1]-outputFunct[1]) > 1e-10 ||
      vcl_abs(result[2]-outputFunct[2]) > 1e-10 ||
      vcl_abs(result[3]-outputFunct[3]) > 1e-10 ||
      vcl_abs(result[4]-outputFunct[4]) > 1e-10 ||
      vcl_abs(result[5]-outputFunct[5]) > 1e-10   )
  {
    std::cout<<"Test gives :"<<std::endl;
    std::cout<<outputFunct<<std::endl;
    std::cout<<"Wanted results are :"<<std::endl;
    std::cout<<result<<std::endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
