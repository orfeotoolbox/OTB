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
#include "otbMorphologicalOpeningProfileFilter.h"
#include "otbProfileToProfileDerivativeFilter.h"
#include "otbProfileDerivativeToMultiScaleCharacteristicsFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

#include "itkMacro.h"

int otbProfileDerivativeToMultiScaleCharacteristicsFilter(int itkNotUsed(argc), char * argv[])
{
  const char *       inputFilename = argv[1];
  const char *       outputFilename1 = argv[2];
  const char *       outputFilename2 = argv[3];
  const unsigned int profileSize = atoi(argv[4]);
  const unsigned int initialValue = atoi(argv[5]);
  const unsigned int step = atoi(argv[6]);

  const unsigned int Dimension = 2;
  typedef double         InputPixelType;
  typedef double         OutputPixelType;
  typedef unsigned short LabeledPixelType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef otb::Image<LabeledPixelType, 2>        LabeledImageType;

  typedef otb::ImageFileReader<InputImageType>            ReaderType;
  typedef otb::ImageFileWriter<OutputImageType>  WriterType;
  typedef otb::ImageFileWriter<LabeledImageType> LabeledWriterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, Dimension> StructuringElementType;
  typedef otb::MorphologicalOpeningProfileFilter<InputImageType, InputImageType, StructuringElementType>
  OpeningProfileFilterType;
  typedef otb::ProfileToProfileDerivativeFilter<InputImageType, InputImageType> DerivativeFilterType;
  typedef otb::ProfileDerivativeToMultiScaleCharacteristicsFilter<InputImageType, OutputImageType, LabeledImageType>
  MultiScaleCharacteristicsFilterType;

  // Reading input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  // Instantiation
  OpeningProfileFilterType::Pointer profileFilter = OpeningProfileFilterType::New();
  profileFilter->SetInput(reader->GetOutput());
  profileFilter->SetProfileSize(profileSize);
  profileFilter->SetInitialValue(initialValue);
  profileFilter->SetStep(step);

  DerivativeFilterType::Pointer derivativeFilter = DerivativeFilterType::New();
  derivativeFilter->SetInput(profileFilter->GetOutput());

  MultiScaleCharacteristicsFilterType::Pointer msCharFilter = MultiScaleCharacteristicsFilterType::New();
  msCharFilter->SetInput(derivativeFilter->GetOutput());
  msCharFilter->SetInitialValue(initialValue);
  msCharFilter->SetStep(step);

  WriterType::Pointer        writer = WriterType::New();
  LabeledWriterType::Pointer labeledWriter = LabeledWriterType::New();

  writer->SetFileName(outputFilename1);
  writer->SetInput(msCharFilter->GetOutput());
  writer->Update();

  labeledWriter->SetFileName(outputFilename2);
  labeledWriter->SetInput(msCharFilter->GetOutputCharacteristics());
  labeledWriter->Update();

  return EXIT_SUCCESS;
}
