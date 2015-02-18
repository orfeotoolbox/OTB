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


#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbConvolutionImageFilter.h"
#include "itkConstantBoundaryCondition.h"

int otbConvolutionImageFilter(int itkNotUsed(argc), char * argv[])
{
  const char * inputFileName = argv[1];
  const char * outputFileName = argv[2];

  typedef double InputPixelType;
  typedef double OutputPixelType;
  const unsigned int Dimension = 2;

  typedef otb::Image<InputPixelType,  Dimension> PanchroImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef otb::ImageFileReader<PanchroImageType> ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>  WriterType;
  // Overriding boundary condition to compare with the overlap save convolution results.
  typedef itk::ConstantBoundaryCondition<PanchroImageType>                                      BoundaryConditionType;
  typedef otb::ConvolutionImageFilter<PanchroImageType, OutputImageType, BoundaryConditionType> ConvFilterType;

  ReaderType::Pointer     reader     = ReaderType::New();
  WriterType::Pointer     writer     = WriterType::New();
  ConvFilterType::Pointer convFilter = ConvFilterType::New();

  reader->SetFileName(inputFileName);
  writer->SetFileName(outputFileName);

  ConvFilterType::InputSizeType radius;
  radius[0] = 3;
  radius[1] = 3;
  //itk::Array< double > filterCoeffs;
  ConvFilterType::ArrayType filterCoeffs;
  filterCoeffs.SetSize((2 * radius[0] + 1) * (2 * radius[1] + 1));
  filterCoeffs.Fill(1);

  convFilter->SetRadius(radius);
  convFilter->SetFilter(filterCoeffs);
  convFilter->NormalizeFilterOn();

  convFilter->SetInput(reader->GetOutput());
  writer->SetInput(convFilter->GetOutput());

  writer->Update();

  return EXIT_SUCCESS;
}
