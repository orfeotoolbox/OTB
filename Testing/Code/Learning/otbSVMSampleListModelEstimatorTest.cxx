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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"
#include "itkListSample.h"
#include <iostream>

#include "otbSVMSampleListModelEstimator.h"
#include "otbSVMKernels.h"

#include "otbVectorImage.h"
#include "otbImageFileReader.h"

#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"

#include "otbListSampleGenerator.h"

int otbSVMSampleListModelEstimatorNew(int argc, char* argv[])
{
  typedef double InputPixelType;
  typedef double LabelPixelType;

  typedef itk::VariableLengthVector<InputPixelType>       SampleType;
  typedef itk::Statistics::ListSample<SampleType>         ListSampleType;
  typedef itk::FixedArray<LabelPixelType, 1>              TrainingSampleType;
  typedef itk::Statistics::ListSample<TrainingSampleType> TrainingListSampleType;

  typedef otb::SVMSampleListModelEstimator<ListSampleType,
      TrainingListSampleType> EstimatorType;

  EstimatorType::Pointer estimator = EstimatorType::New();

  return EXIT_SUCCESS;
}

int otbSVMSampleListModelEstimatorTest(int argc, char* argv[])
{
  if (argc != 4)
    {
    std::cerr << "Usage: " << argv[0] << " inputImage inputVectorData outputModelFileName"
              << std::endl;
    return EXIT_FAILURE;
    }

  std::string imageFilename = argv[1];
  std::string vectorDataFilename = argv[2];
  std::string outputModelFileName= argv[3];
  int         maxTrainingSize = 500;
  int         maxValidationSize = 500;
  double      validationTrainingProportion = 0.5;

  std::string classKey = "Class";

  typedef double                          PixelType;
  typedef int                             LabeledPixelType;
  typedef otb::VectorImage<PixelType, 2>  ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(imageFilename);
  reader->UpdateOutputInformation();

  typedef otb::VectorData<float, 2>                 VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataReaderType;

  VectorDataReaderType::Pointer vectorReader = VectorDataReaderType::New();
  vectorReader->SetFileName(vectorDataFilename);
  vectorReader->Update();

  typedef otb::ListSampleGenerator<ImageType, VectorDataType> ListSampleGeneratorType;
  ListSampleGeneratorType::Pointer generator = ListSampleGeneratorType::New();
  generator->SetMaxTrainingSize(maxTrainingSize);
  generator->SetMaxValidationSize(maxValidationSize);
  generator->SetValidationTrainingProportion(validationTrainingProportion);

  generator->SetInput(reader->GetOutput());
  generator->SetInputVectorData(vectorReader->GetOutput());

  generator->Update();
  std::cout << generator << std::endl;

  typedef ListSampleGeneratorType::ListSampleType ListSampleType;
  typedef ListSampleGeneratorType::ListLabelType TrainingListSampleType;
  typedef otb::SVMSampleListModelEstimator<ListSampleType, TrainingListSampleType> EstimatorType;

  EstimatorType::Pointer estimator = EstimatorType::New();
  estimator->SetInputSampleList(generator->GetTrainingListSample());
  estimator->SetTrainingSampleList(generator->GetTrainingListLabel());
  estimator->SetNumberOfClasses(generator->GetNumberOfClasses());

  estimator->Update();

  estimator->GetModel()->SaveModel(outputModelFileName);

  return EXIT_SUCCESS;
}
