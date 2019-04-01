/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


/* Example usage:
./TrainMachineLearningModelFromSamplesExample Output/clSVMModelFromSamples.svm
*/


// This example illustrates the use of the \doxygen{otb}{SVMMachineLearningModel} class, which inherits from the
// \doxygen{otb}{MachineLearningModel} class. This class allows the
// estimation of a classification model (supervised learning) from samples. In this example, we will train an SVM model
// with 4 output classes, from 1000 randomly generated training samples, each of them having 7 components.
// We start by including the appropriate header files.
// List sample generator
#include "otbListSampleGenerator.h"

// Random number generator

// SVM model Estimator
#include "otbSVMMachineLearningModel.h"


int main(int argc, char* argv[])
{

  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " outputModelFileName" << std::endl;
    return EXIT_FAILURE;
  }

  // The input parameters of the sample generator and of the SVM classifier are initialized.
  int nbSamples          = 1000;
  int nbSampleComponents = 7;
  int nbClasses          = 4;

  const char* outputModelFileName = argv[1];

  // Two lists are generated into a \subdoxygen{itk}{Statistics}{ListSample} which is the structure
  // used to handle both lists of samples and of labels for the machine learning classes derived from
  // \doxygen{otb}{MachineLearningModel}. The first list is composed of feature vectors representing
  // multi-component samples, and the second one is filled with their corresponding class labels. The
  // list of labels is composed of scalar values.


  // Input related typedefs
  typedef float                                        InputValueType;
  typedef itk::VariableLengthVector<InputValueType>    InputSampleType;
  typedef itk::Statistics::ListSample<InputSampleType> InputListSampleType;

  // Target related typedefs
  typedef int                                           TargetValueType;
  typedef itk::FixedArray<TargetValueType, 1>           TargetSampleType;
  typedef itk::Statistics::ListSample<TargetSampleType> TargetListSampleType;

  InputListSampleType::Pointer  InputListSample  = InputListSampleType::New();
  TargetListSampleType::Pointer TargetListSample = TargetListSampleType::New();

  InputListSample->SetMeasurementVectorSize(nbSampleComponents);


  // In this example, the list of multi-component training samples is randomly filled with a random number
  // generator based on the \subdoxygen{itk}{Statistics}{MersenneTwisterRandomVariateGenerator} class.
  // Each component's value is generated from a normal law centered around the corresponding class label of
  // each sample multiplied by 100, with a standard deviation of 10.


  itk::Statistics::MersenneTwisterRandomVariateGenerator::Pointer randGen;
  randGen = itk::Statistics::MersenneTwisterRandomVariateGenerator::GetInstance();

  // Filling the two input training lists
  for (int i = 0; i < nbSamples; ++i)
  {
    InputSampleType sample;
    TargetValueType label = (i % nbClasses) + 1;

    // Multi-component sample randomly filled from a normal law for each component
    sample.SetSize(nbSampleComponents);
    for (int itComp = 0; itComp < nbSampleComponents; ++itComp)
    {
      sample[itComp] = randGen->GetNormalVariate(100 * label, 10);
    }

    InputListSample->PushBack(sample);
    TargetListSample->PushBack(label);
  }

  // Displays the corresponding values
  for (int i = 0; i < nbSamples; ++i)
  {
    std::cout << i + 1 << "-label = " << TargetListSample->GetMeasurementVector(i) << std::endl;
    std::cout << "sample = " << InputListSample->GetMeasurementVector(i) << std::endl << std::endl;
  }

  // Once both sample and label lists are generated, the second step consists in
  // declaring the machine learning classifier. In our case we use an SVM model
  // with the help of the \doxygen{otb}{SVMMachineLearningModel} class which is
  // derived from the \doxygen{otb}{MachineLearningModel} class.
  // This pure virtual class is based on the machine learning framework of the
  // OpenCV library (\cite{opencv_library}) which handles other classifiers than
  // the SVM.

  typedef otb::SVMMachineLearningModel<InputValueType, TargetValueType> SVMType;

  SVMType::Pointer SVMClassifier = SVMType::New();

  SVMClassifier->SetInputListSample(InputListSample);
  SVMClassifier->SetTargetListSample(TargetListSample);

  SVMClassifier->SetKernelType(CvSVM::LINEAR);

  // Once the classifier is parametrized with both input lists and default parameters, except
  // for the kernel type in our example of SVM model estimation, the model
  // training is computed with the \code{Train} method. Finally, the \code{Save} method
  // exports the model to a text file. All the available classifiers based on OpenCV are
  // implemented with these interfaces. Like for the SVM model training, the other classifiers
  // can be parametrized with specific settings.

  SVMClassifier->Train();
  SVMClassifier->Save(outputModelFileName);
}
