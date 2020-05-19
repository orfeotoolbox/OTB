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

#include "otbImageClassificationFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbSharkRandomForestsMachineLearningModelFactory.h"
#include <random>
#include <chrono>

const unsigned int     Dimension = 2;
typedef float          PixelType;
typedef unsigned short LabeledPixelType;

typedef otb::VectorImage<PixelType, Dimension>                      ImageType;
typedef otb::Image<LabeledPixelType, Dimension>                     LabeledImageType;
typedef otb::ImageClassificationFilter<ImageType, LabeledImageType> ClassificationFilterType;
typedef ClassificationFilterType::ModelType ModelType;
typedef ClassificationFilterType::ValueType ValueType;
typedef ClassificationFilterType::LabelType LabelType;
typedef otb::SharkRandomForestsMachineLearningModelFactory<ValueType, LabelType> MachineLearningModelFactoryType;
typedef otb::ImageFileReader<ImageType>        ReaderType;
typedef otb::ImageFileReader<LabeledImageType> MaskReaderType;
typedef otb::ImageFileWriter<LabeledImageType> WriterType;

typedef otb::SharkRandomForestsMachineLearningModel<PixelType, short unsigned int> MachineLearningModelType;
typedef MachineLearningModelType::InputValueType       LocalInputValueType;
typedef MachineLearningModelType::InputSampleType      LocalInputSampleType;
typedef MachineLearningModelType::InputListSampleType  LocalInputListSampleType;
typedef MachineLearningModelType::TargetValueType      LocalTargetValueType;
typedef MachineLearningModelType::TargetSampleType     LocalTargetSampleType;
typedef MachineLearningModelType::TargetListSampleType LocalTargetListSampleType;

void generateSamples(unsigned int num_classes, unsigned int num_samples, unsigned int num_features, LocalInputListSampleType* samples,
                     LocalTargetListSampleType* labels)
{
  std::default_random_engine         generator;
  std::uniform_int_distribution<int> label_distribution(1, num_classes);
  std::uniform_int_distribution<int> feat_distribution(0, 256);
  for (size_t scount = 0; scount < num_samples; ++scount)
  {
    LabeledPixelType     label = label_distribution(generator);
    LocalInputSampleType sample(num_features);
    for (unsigned int i = 0; i < num_features; ++i)
      sample[i]         = feat_distribution(generator);
    samples->SetMeasurementVectorSize(num_features);
    samples->PushBack(sample);
    labels->PushBack(label);
  }
}

void buildModel(unsigned int num_classes, unsigned int num_samples, unsigned int num_features, std::string modelfname)
{
  LocalInputListSampleType::Pointer  samples = LocalInputListSampleType::New();
  LocalTargetListSampleType::Pointer labels  = LocalTargetListSampleType::New();

  std::cout << "Sample generation\n";
  generateSamples(num_classes, num_samples, num_features, samples, labels);

  MachineLearningModelType::Pointer classifier = MachineLearningModelType::New();
  classifier->SetInputListSample(samples);
  classifier->SetTargetListSample(labels);
  classifier->SetRegressionMode(false);
  classifier->SetNumberOfTrees(100);
  classifier->SetMTry(0);
  classifier->SetNodeSize(25);
  classifier->SetOobRatio(0.3);
  std::cout << "Training\n";
  using TimeT = std::chrono::milliseconds;
  auto start  = std::chrono::system_clock::now();
  classifier->Train();
  auto duration = std::chrono::duration_cast<TimeT>(std::chrono::system_clock::now() - start);
  auto elapsed  = duration.count();
  std::cout << "Training took " << elapsed << " ms\n";
  classifier->Save(modelfname);
}

int otbSharkImageClassificationFilter(int argc, char* argv[])
{
  if (argc < 6 || argc > 8)
  {
    std::cout << "Usage: input_image output_image output_confidence output_proba batchmode [in_model_name] [mask_name]\n";
  }
  std::string imfname    = argv[1];
  std::string outfname   = argv[2];
  std::string conffname  = argv[3];
  std::string probafname = argv[4];
  bool        batch      = (std::string(argv[5]) == "1");
  std::string modelfname = "/tmp/rf_model.txt";
  std::string maskfname{};
  int         num_classes = 3;

  MaskReaderType::Pointer mask_reader = MaskReaderType::New();
  ReaderType::Pointer     reader      = ReaderType::New();
  reader->SetFileName(imfname);
  reader->UpdateOutputInformation();

  auto num_features = reader->GetOutput()->GetNumberOfComponentsPerPixel();

  std::cout << "Image has " << num_features << " bands\n";

  if (argc > 6)
  {
    modelfname = argv[6];
    // We don't know the number of classes, so we set it to a high number
    num_classes = 10;
  }
  else
  {
    buildModel(num_classes, 1000, num_features, modelfname);
  }

  ClassificationFilterType::Pointer filter = ClassificationFilterType::New();

  MachineLearningModelType::Pointer model = MachineLearningModelType::New();
  model->Load(modelfname);
  filter->SetModel(model);
  filter->SetInput(reader->GetOutput());
  filter->SetNumberOfClasses(num_classes);
  if (argc == 8)
  {
    maskfname = argv[7];
    mask_reader->SetFileName(maskfname);
    filter->SetInputMask(mask_reader->GetOutput());
  }

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname);
  std::cout << "Classification\n";
  filter->SetBatchMode(batch);
  filter->SetUseConfidenceMap(true);
  filter->SetUseProbaMap(true);
  using TimeT = std::chrono::milliseconds;
  auto start  = std::chrono::system_clock::now();
  writer->Update();
  auto duration = std::chrono::duration_cast<TimeT>(std::chrono::system_clock::now() - start);
  auto elapsed  = duration.count();
  std::cout << "Classification took " << elapsed << " ms\n";

  auto confWriter = otb::ImageFileWriter<ClassificationFilterType::ConfidenceImageType>::New();
  confWriter->SetInput(filter->GetOutputConfidence());
  confWriter->SetFileName(conffname);
  confWriter->Update();

  auto probaWriter = otb::ImageFileWriter<ClassificationFilterType::ProbaImageType>::New();
  probaWriter->SetInput(filter->GetOutputProba());
  probaWriter->SetFileName(probafname);
  probaWriter->Update();

  // Check that the chosen labels correspond to the max proba

  itk::ImageRegionConstIterator<LabeledImageType>                         labIt(filter->GetOutput(), filter->GetOutput()->GetLargestPossibleRegion());
  itk::ImageRegionConstIterator<ClassificationFilterType::ProbaImageType> probIt(filter->GetOutputProba(),
                                                                                 filter->GetOutputProba()->GetLargestPossibleRegion());

  for (labIt.GoToBegin(), probIt.GoToBegin(); !labIt.IsAtEnd(); ++labIt, ++probIt)
  {
    if (labIt.Get() > 0) // Pixel is not masked
    {
      auto first = probIt.Get().GetDataPointer();
      auto last  = probIt.Get().GetDataPointer();
      std::advance(last, num_classes);
      auto max_proba = std::distance(first, std::max_element(first, last)) + 1;
      if (labIt.Get() != max_proba)
      {
        std::cout << "Chosen label " << labIt.Get() << " and max proba position " << max_proba << " from " << probIt.Get() << " don't match\n";
        return EXIT_FAILURE;
      }
    }
  }

  return EXIT_SUCCESS;
}
