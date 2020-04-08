/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#include "otbVectorImage.h"
#include "otbImageFileReader.h"

#include "otbVectorData.h"
#include "otbVectorDataFileReader.h"

#include "otbListSampleGenerator.h"


int otbListSampleGenerator(int argc, char* argv[])
{

  if (argc != 9)
  {
    std::cerr << "Usage: " << argv[0] << " inputImage inputVectorData outputTrainingFile outputValidationFile"
              << " maxTrainingSize maxValidationSize validationTrainingProportion boundByMin" << std::endl;
    return EXIT_FAILURE;
  }

  std::string imageFilename                = argv[1];
  std::string vectorDataFilename           = argv[2];
  std::string outputSampleList             = argv[3];
  std::string outputSampleListValidation   = argv[4];
  int         maxTrainingSize              = atoi(argv[5]);
  int         maxValidationSize            = atoi(argv[6]);
  double      validationTrainingProportion = atof(argv[7]);
  bool        boundByMin                   = static_cast<bool>(atoi(argv[8]));

  std::string classKey = "Class";

  typedef double PixelType;
  typedef otb::VectorImage<PixelType, 2> ImageType;
  typedef otb::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(imageFilename);
  reader->UpdateOutputInformation();

  typedef otb::VectorData<float, 2> VectorDataType;
  typedef otb::VectorDataFileReader<VectorDataType> VectorDataReaderType;

  VectorDataReaderType::Pointer vectorReader = VectorDataReaderType::New();
  vectorReader->SetFileName(vectorDataFilename);
  vectorReader->Update();

  std::cout << "Number of elements in the tree: " << vectorReader->GetOutput()->Size() << std::endl;

  typedef otb::ListSampleGenerator<ImageType, VectorDataType> ListSampleGeneratorType;
  ListSampleGeneratorType::Pointer generator = ListSampleGeneratorType::New();
  generator->SetMaxTrainingSize(maxTrainingSize);
  generator->SetMaxValidationSize(maxValidationSize);
  generator->SetValidationTrainingProportion(validationTrainingProportion);
  generator->SetBoundByMin(boundByMin);

  generator->SetInput(reader->GetOutput());
  generator->SetInputVectorData(vectorReader->GetOutput());
  std::cout << generator << std::endl;

  generator->Update();
  std::cout << generator << std::endl;

  std::cout << "Number of classes: " << generator->GetNumberOfClasses() << std::endl;

  {
    ListSampleGeneratorType::ListSamplePointerType samples = generator->GetTrainingListSample();
    ListSampleGeneratorType::ListLabelPointerType  labels  = generator->GetTrainingListLabel();

    typedef ListSampleGeneratorType::ListSampleType::ConstIterator SampleIterator;
    typedef ListSampleGeneratorType::ListLabelType::ConstIterator  LabelIterator;

    std::ofstream trainingFile;
    trainingFile.open(outputSampleList);

    SampleIterator sampleIt = samples->Begin();
    LabelIterator  labelIt  = labels->Begin();
    while (sampleIt != samples->End())
    {
      trainingFile << labelIt.GetMeasurementVector()[0];
      for (unsigned int i = 0; i < sampleIt.GetMeasurementVector().Size(); ++i)
      {
        // Careful, the numbering is 1..N in libsvm
        trainingFile << " " << i + 1 << ":" << sampleIt.GetMeasurementVector()[i];
      }
      trainingFile << "\n";
      ++sampleIt;
      ++labelIt;
    }

    trainingFile.close();
  }

  {

    ListSampleGeneratorType::ListSamplePointerType samples = generator->GetValidationListSample();
    ListSampleGeneratorType::ListLabelPointerType  labels  = generator->GetValidationListLabel();

    typedef ListSampleGeneratorType::ListSampleType::ConstIterator SampleIterator;
    typedef ListSampleGeneratorType::ListLabelType::ConstIterator  LabelIterator;

    std::ofstream validationFile;
    validationFile.open(outputSampleListValidation);

    SampleIterator sampleIt = samples->Begin();
    LabelIterator  labelIt  = labels->Begin();
    while (sampleIt != samples->End())
    {
      validationFile << labelIt.GetMeasurementVector()[0];
      for (unsigned int i = 0; i < sampleIt.GetMeasurementVector().Size(); ++i)
      {
        // Careful, the numbering is 1..N in libsvm
        validationFile << " " << i + 1 << ":" << sampleIt.GetMeasurementVector()[i];
      }
      validationFile << "\n";
      ++sampleIt;
      ++labelIt;
    }

    validationFile.close();
  }

  return EXIT_SUCCESS;
}
