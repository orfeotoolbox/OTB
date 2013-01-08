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
#include "otbMorphologicalClosingProfileFilter.h"
#include "otbProfileToProfileDerivativeFilter.h"
#include "otbProfileDerivativeToMultiScaleCharacteristicsFilter.h"
#include "otbMultiScaleConvexOrConcaveClassificationFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"

#include "itkMacro.h"

int otbMultiScaleConvexOrConcaveClassificationFilter(int argc, char * argv[])
{
  const char *       inputFilename = argv[1];
  const char *       outputFilename = argv[2];
  const unsigned int profileSize = atoi(argv[3]);
  const unsigned int initialValue = atoi(argv[4]);
  const unsigned int step = atoi(argv[5]);
  const double       sigma = atof(argv[6]);

  const unsigned int Dimension = 2;
  typedef double         InputPixelType;
  typedef double         OutputPixelType;
  typedef unsigned short LabeledPixelType;

  typedef otb::Image<InputPixelType, Dimension>  InputImageType;
  typedef otb::Image<OutputPixelType, Dimension> OutputImageType;
  typedef otb::Image<LabeledPixelType, 2>        LabeledImageType;

  typedef otb::ImageFileReader<InputImageType>            ReaderType;
  typedef otb::ImageFileWriter<LabeledImageType> LabeledWriterType;

  typedef itk::BinaryBallStructuringElement<InputPixelType, Dimension> StructuringElementType;
  typedef otb::MorphologicalOpeningProfileFilter<InputImageType, InputImageType, StructuringElementType>
  OpeningProfileFilterType;
  typedef otb::MorphologicalClosingProfileFilter<InputImageType, InputImageType, StructuringElementType>
  ClosingProfileFilterType;
  typedef otb::ProfileToProfileDerivativeFilter<InputImageType, InputImageType> DerivativeFilterType;
  typedef otb::ProfileDerivativeToMultiScaleCharacteristicsFilter<InputImageType, OutputImageType, LabeledImageType>
  MultiScaleCharacteristicsFilterType;
  typedef otb::MultiScaleConvexOrConcaveClassificationFilter<InputImageType,
      LabeledImageType> MultiScaleClassificationFilterType;

  // Reading input image
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputFilename);

  // Instantiation
  OpeningProfileFilterType::Pointer oprofileFilter = OpeningProfileFilterType::New();
  oprofileFilter->SetInput(reader->GetOutput());
  oprofileFilter->SetProfileSize(profileSize);
  oprofileFilter->SetInitialValue(initialValue);
  oprofileFilter->SetStep(step);

  ClosingProfileFilterType::Pointer cprofileFilter = ClosingProfileFilterType::New();
  cprofileFilter->SetInput(reader->GetOutput());
  cprofileFilter->SetProfileSize(profileSize);
  cprofileFilter->SetInitialValue(initialValue);
  cprofileFilter->SetStep(step);

  DerivativeFilterType::Pointer oderivativeFilter = DerivativeFilterType::New();
  oderivativeFilter->SetInput(oprofileFilter->GetOutput());

  DerivativeFilterType::Pointer cderivativeFilter = DerivativeFilterType::New();
  cderivativeFilter->SetInput(cprofileFilter->GetOutput());

  MultiScaleCharacteristicsFilterType::Pointer omsCharFilter = MultiScaleCharacteristicsFilterType::New();
  omsCharFilter->SetInput(oderivativeFilter->GetOutput());
  omsCharFilter->SetInitialValue(initialValue);
  omsCharFilter->SetStep(step);

  MultiScaleCharacteristicsFilterType::Pointer cmsCharFilter = MultiScaleCharacteristicsFilterType::New();
  cmsCharFilter->SetInput(cderivativeFilter->GetOutput());
  cmsCharFilter->SetInitialValue(initialValue);
  cmsCharFilter->SetStep(step);

  MultiScaleClassificationFilterType::Pointer classificationFilter = MultiScaleClassificationFilterType::New();
  classificationFilter->SetOpeningProfileDerivativeMaxima(omsCharFilter->GetOutput());
  classificationFilter->SetOpeningProfileCharacteristics(omsCharFilter->GetOutputCharacteristics());
  classificationFilter->SetClosingProfileDerivativeMaxima(cmsCharFilter->GetOutput());
  classificationFilter->SetClosingProfileCharacteristics(cmsCharFilter->GetOutputCharacteristics());
  classificationFilter->SetSigma(sigma);
  classificationFilter->SetLabelSeparator(initialValue + profileSize * step);

  LabeledWriterType::Pointer labeledWriter = LabeledWriterType::New();

  labeledWriter->SetFileName(outputFilename);
  labeledWriter->SetInput(classificationFilter->GetOutput());
  labeledWriter->Update();

  return EXIT_SUCCESS;
}
