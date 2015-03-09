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
#include <cstdlib>
#include <cmath>

#include "otbVariableLengthVectorConverter.h"

int otbVariableLengthVectorConverterNew(int itkNotUsed(argc), char * itkNotUsed(argv) [])
{
  typedef itk::VariableLengthVector<double>                      InputType0;
  typedef std::vector< std::vector< double > >                   InputType1;
  typedef std::vector< std::vector< std::complex< double > > >   InputType2;
  typedef itk::FixedArray<double>                                InputType3;

  typedef float                                                  PrecisionType;

  typedef otb::VariableLengthVectorConverter<InputType0, PrecisionType>
                                                                 ConverterType0;
  typedef otb::VariableLengthVectorConverter<InputType1, PrecisionType>
                                                                 ConverterType1;
  typedef otb::VariableLengthVectorConverter<InputType2, PrecisionType>
                                                                 ConverterType2;
  typedef otb::VariableLengthVectorConverter<InputType3, PrecisionType>
                                                                 ConverterType3;

  // Instantiating object
  ConverterType0::Pointer converter0 = ConverterType0::New();
  ConverterType1::Pointer converter1 = ConverterType1::New();
  ConverterType2::Pointer converter2 = ConverterType2::New();
  ConverterType3::Pointer converter3 = ConverterType3::New();

  std::cout << converter0 << std::endl;
  std::cout << converter1 << std::endl;
  std::cout << converter2 << std::endl;
  std::cout << converter3 << std::endl;

  return EXIT_SUCCESS;
}
