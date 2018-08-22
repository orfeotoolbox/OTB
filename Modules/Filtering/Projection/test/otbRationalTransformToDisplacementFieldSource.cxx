/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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



#include <fstream>
#include "itkVector.h"
#include "otbImage.h"
#include "otbRationalTransform.h"
#include "itkTransformToDisplacementFieldSource.h"
#include "otbImageFileWriter.h"

int otbRationalTransformToDisplacementFieldSourceTest(int argc, char* argv[])
{

  /** Check command line arguments. */
  if( argc < 2 )
    {
    std::cerr << "You must supply output filename" << std::endl;
    return EXIT_FAILURE;
    }

  std::string fileName = argv[ 1 ];

  /** Typedefs. */
  const unsigned int  Dimension = 2;
  typedef float       ScalarPixelType;
  typedef double      CoordRepresentationType;

  typedef itk::Vector<ScalarPixelType, Dimension >    VectorPixelType;
  typedef otb::Image<VectorPixelType, Dimension >     DisplacementFieldImageType;

  typedef otb::RationalTransform<CoordRepresentationType, Dimension > TransformType;
  typedef TransformType::ParametersType   ParametersType;

  typedef itk::TransformToDisplacementFieldSource<
    DisplacementFieldImageType,
    CoordRepresentationType >             DisplacementFieldGeneratorType;

  typedef DisplacementFieldGeneratorType::SizeType       SizeType;
  typedef DisplacementFieldGeneratorType::SpacingType    SpacingType;
  typedef DisplacementFieldGeneratorType::OriginType     OriginType;
  typedef DisplacementFieldGeneratorType::IndexType      IndexType;
  typedef otb::ImageFileWriter<DisplacementFieldImageType >   WriterType;

  /** Create output information. */
  SizeType size;        size.Fill( 20 );
  IndexType index;      index.Fill( 0 );
  SpacingType spacing;  spacing.Fill( 0.7 );
  OriginType origin;    origin.Fill( -10.0 );

  /** Create transform. */
  TransformType::Pointer transform = TransformType::New();

  /** Create and set parameters. */
  transform->SetNumeratorDegree(4);
  transform->SetDenominatorDegree(4);

  ParametersType parameters( transform->GetNumberOfParameters() );
  // Rational is
  // fx(x, y) = (1+2*x+3*x^2+4*x^3+5*x^4)/(6+7*x+8*x^2+9*x^3+10*x^4)
  // fy(x, y) = (11+12*y+13*y^2+14*y^3+15*y^4)/(16+17*y+18*y^2+19*y^3+20*y^4)
  parameters[0]=1;
  parameters[1]=2;
  parameters[2]=3;
  parameters[3]=4;
  parameters[4]=5;
  parameters[5]=6;
  parameters[6]=7;
  parameters[7]=8;
  parameters[8]=9;
  parameters[9]=10;
  parameters[10]=11;
  parameters[11]=12;
  parameters[12]=13;
  parameters[13]=14;
  parameters[14]=15;
  parameters[15]=16;
  parameters[16]=17;
  parameters[17]=18;
  parameters[18]=19;
  parameters[19]=20;
  transform->SetParameters( parameters );

  /** Create an setup deformation field generator. */
  DisplacementFieldGeneratorType::Pointer defGenerator
    = DisplacementFieldGeneratorType::New();
  std::cout << "Name of Class: " << defGenerator->GetNameOfClass()
            << std::endl;
  defGenerator->SetOutputSize( size );
  defGenerator->SetOutputSpacing( spacing );
  defGenerator->SetOutputOrigin( origin );
  defGenerator->SetOutputIndex( index );
  //
  // for coverage, exercise access methods
  spacing  = defGenerator->GetOutputSpacing();
  origin = defGenerator->GetOutputOrigin();
  DisplacementFieldGeneratorType::DirectionType
    direction = defGenerator->GetOutputDirection();
  std::cout << "Spacing " << spacing
            << " Origin " << origin
            << std::endl << "Direction "
            << direction
            << std::endl;
  //defGenerator->SetOutputDirection( direction );

  defGenerator->SetTransform( transform );

  std::cout << "Transform: " << defGenerator->GetTransform() << std::endl;

  DisplacementFieldGeneratorType::OutputImageRegionType
    outputRegion = defGenerator->GetOutputRegion();
  defGenerator->SetOutputRegion(outputRegion);

  /** Write deformation field to disk. */
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( defGenerator->GetOutput() );
  writer->SetFileName( fileName );

  try
    {
    writer->Update();
    }
  catch ( itk::ExceptionObject & e )
    {
    std::cerr << "Exception detected while generating deformation field" << argv[1];
    std::cerr << " : "  << e.GetDescription();
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
