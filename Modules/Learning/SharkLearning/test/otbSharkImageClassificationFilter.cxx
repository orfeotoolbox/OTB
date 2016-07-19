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
#include "otbSharkImageClassificationFilter.h"
#include "otbVectorImage.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbSharkRandomForestsMachineLearningModelFactory.h"
#include <random>
#include <chrono>

const unsigned int Dimension = 2;
typedef float PixelType;
typedef unsigned short LabeledPixelType;

typedef otb::VectorImage<PixelType, Dimension> ImageType;
typedef otb::Image<LabeledPixelType, Dimension> LabeledImageType;
typedef otb::SharkImageClassificationFilter<ImageType, LabeledImageType> ClassificationFilterType;
typedef ClassificationFilterType::ModelType ModelType;
typedef ClassificationFilterType::ValueType ValueType;
typedef ClassificationFilterType::LabelType LabelType;
typedef otb::SharkRandomForestsMachineLearningModelFactory<ValueType, LabelType> MachineLearningModelFactoryType;
typedef otb::ImageFileReader<ImageType> ReaderType;
typedef otb::ImageFileWriter<LabeledImageType> WriterType;

typedef otb::SharkRandomForestsMachineLearningModel<PixelType,short unsigned int>         MachineLearningModelType;
typedef MachineLearningModelType::InputValueType       InputValueType;
typedef MachineLearningModelType::InputSampleType      InputSampleType;
typedef MachineLearningModelType::InputListSampleType  InputListSampleType;
typedef MachineLearningModelType::TargetValueType      TargetValueType;
typedef MachineLearningModelType::TargetSampleType     TargetSampleType;
typedef MachineLearningModelType::TargetListSampleType TargetListSampleType;

void extractSamples(unsigned int num_classes, unsigned int num_samples,
                    unsigned int num_features,
                    InputListSampleType * samples, 
                    TargetListSampleType * labels)
{
  std::default_random_engine generator;
  std::uniform_int_distribution<int> label_distribution(1,num_classes);
  std::uniform_int_distribution<int> feat_distribution(0,256);
  for(size_t scount=0; scount<num_samples; ++scount)
    {
    LabeledPixelType label = label_distribution(generator);
    InputSampleType sample(num_features);
    for(auto i=0; i<num_features; ++i)
      sample[i]= feat_distribution(generator);
    samples->SetMeasurementVectorSize(num_features);
    samples->PushBack(sample); 
    labels->PushBack(label);
    }
}


int otbSharkImageClassificationFilter(int itkNotUsed(argc), char * argv[])
{
  const char * imfname = argv[1];
  const char * outfname = argv[2];

  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(imfname);
  reader->UpdateOutputInformation();

  auto num_features = reader->GetOutput()->GetNumberOfComponentsPerPixel();

  std::cout << "Image has " << num_features << " bands\n";
  InputListSampleType::Pointer samples = InputListSampleType::New();
  TargetListSampleType::Pointer labels = TargetListSampleType::New();

  std::cout << "Sample generation\n";
  extractSamples(3, 1000, num_features, samples, labels);

  MachineLearningModelType::Pointer classifier = MachineLearningModelType::New();
  classifier->SetInputListSample(samples);
  classifier->SetTargetListSample(labels);
  classifier->SetRegressionMode(false);
  classifier->SetNumberOfTrees(100);
  classifier->SetMTry(0);
  classifier->SetNodeSize(25);
  classifier->SetOobRatio(0.3);
  std::cout << "Training\n";
  classifier->Train();

    
  ClassificationFilterType::Pointer filter = ClassificationFilterType::New();

  filter->SetModel(classifier.GetPointer());
  filter->SetInput(reader->GetOutput());

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  writer->SetFileName(outfname);
  std::cout << "Classification\n";
  using TimeT = std::chrono::milliseconds;
  auto start = std::chrono::system_clock::now();
  writer->Update();
  auto duration = std::chrono::duration_cast< TimeT> 
    (std::chrono::system_clock::now() - start);
  auto elapsed = duration.count();
  std::cout << "Classification took " << elapsed << " ms\n";

  return EXIT_SUCCESS;
}
