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
#include "itkExceptionObject.h"

#include "otbSinclairToCovarianceFunctor.h"
#include "itkVariableLengthVector.h"

int otbSinclairToCovarianceFunctor(int argc, char * argv[])
{
  typedef std::complex<double>                   ScalarType;
  typedef itk::VariableLengthVector<ScalarType>  OutputType;

  typedef otb::Functor::SinclairToCovarianceFunctor<ScalarType, ScalarType,
      ScalarType, ScalarType, OutputType >         FunctorType;

  OutputType  result(10);
  FunctorType funct;
  OutputType outputFunct;

  result[0] = 1.0;
  result[1] = 0.0;
  result[2] = 0.0;
  result[3] = 1.0;
  result[4] = 0.0;
  result[5] = 0.0;
  result[6] = 0.0;
  result[7] = 0.0;
  result[8] = 0.0;
  result[9] = 1.0;

  outputFunct = funct.operator ()( 1.0, 0.0, 0.0, 1.0);

  if( vcl_abs(result[0]-outputFunct[0]) > 1e-10 ||
      vcl_abs(result[1]-outputFunct[1]) > 1e-10 ||
      vcl_abs(result[2]-outputFunct[2]) > 1e-10 ||
      vcl_abs(result[3]-outputFunct[3]) > 1e-10 ||
      vcl_abs(result[4]-outputFunct[4]) > 1e-10 ||
      vcl_abs(result[5]-outputFunct[5]) > 1e-10 ||
      vcl_abs(result[6]-outputFunct[6]) > 1e-10 ||
      vcl_abs(result[7]-outputFunct[7]) > 1e-10 ||
      vcl_abs(result[8]-outputFunct[8]) > 1e-10 ||
      vcl_abs(result[9]-outputFunct[9]) > 1e-10)
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
