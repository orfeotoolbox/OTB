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

//Random
#include "itkMersenneTwisterRandomVariateGenerator.h"

//List sample generator
#include "otbListSampleGenerator.h"

//Estimator
# include "otbSVMMachineLearningModel.h"
#include "otbMachineLearningModelFactory.h"


//  Software Guide : BeginCommandLineArgs
//    INPUTS: {1000}, {4}, {5}, {121212}
//    OUTPUTS: {clSVMModelFromSamples.svm}
//  Software Guide : EndCommandLineArgs

//  Software Guide : BeginLatex
// This example illustrates the use of the \doxygen{otb}{SVMMachineLearningModel} class, which inherits from the
// \doxygen{otb}{MachineLearningModel} class. This class allows the
// estimation of a classification model (supervised learning) from samples. In this example, we will train an SVM model
// with 4 output classes, from 1000 randomly generated training samples, each of them having 7 components.
//
//  Software Guide : EndLatex

int main(int argc, char* argv[])
{

  /*
  if (argc != 6)
    {
    std::cerr << "Usage: " << argv[0] << " nbSamples nbSampleComponents nbClasses inputSeed outputModelFileName" << std::endl;
    return EXIT_FAILURE;
    }
  */

  if (argc != 2)
    {
    std::cerr << "Usage: " << argv[0] << " outputModelFileName" << std::endl;
    return EXIT_FAILURE;
    }

  // Software Guide : BeginLatex
  // The input parameters of the sample generator and of the SVM classifier are initialized.
  // Software Guide : EndLatex

  /*
  int nbSamples = atoi(argv[1]);
  int nbSampleComponents = atoi(argv[2]);
  int nbClasses = atoi(argv[3]);
  unsigned int inputSeed = atoi(argv[4]);
  */

  // Software Guide : BeginCodeSnippet
  int nbSamples = 1000;
  int nbSampleComponents = 7;
  int nbClasses = 4;
  // Software Guide : EndCodeSnippet

  unsigned int inputSeed = 121212;
  const char* outputModelFileName = argv[1];//argv[5];


  // Software Guide : BeginLatex
  // Two lists are generated into a \subdoxygen{itk}{Statistics}{ListSample} which is the structure
  // used to handle both lists of samples and of labels for the machine learning classes derived from
  // \doxygen{otb}{MachineLearningModel}. The first list is composed of feature vectors representing
  // multi-component samples, and the second one is filled with their corresponding class labels. The
  // list of labels is composed of scalar values.
  // Software Guide : EndLatex


  // Software Guide : BeginCodeSnippet
  // Input related typedefs
  typedef float InputValueType;
  typedef itk::VariableLengthVector<InputValueType> InputSampleType;
  typedef itk::Statistics::ListSample<InputSampleType> InputListSampleType;

  // Target related typedefs
  typedef int TargetValueType;
  typedef itk::FixedArray<TargetValueType, 1> TargetSampleType;
  typedef itk::Statistics::ListSample<TargetSampleType> TargetListSampleType;

  InputListSampleType::Pointer InputListSample = InputListSampleType::New();
  TargetListSampleType::Pointer TargetListSample = TargetListSampleType::New();
  // Software Guide : EndCodeSnippet


  // Software Guide : BeginLatex
  // In this example, the list of multi-component training samples is randomly filled with a random number
  // generator based on the \subdoxygen{itk}{Statistics}{MersenneTwisterRandomVariateGenerator} class.
  // Each component's value is generated from a normal law centered around the corresponding class label of
  // each sample multiplied by 100, with a standard deviation of 10.
  // Software Guide : EndLatex


  // Software Guide : BeginCodeSnippet
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
  // Software Guide : EndCodeSnippet

  // Displays the corresponding values (not into the Software Guide)
  for (int i = 0; i < nbSamples; ++i)
    {
    std::cout << i + 1 << "-label = " << TargetListSample->GetMeasurementVector(i) << std::endl;
    std::cout << "sample = " << InputListSample->GetMeasurementVector(i) << std::endl << std::endl;
    }

  // Software Guide : BeginLatex
  //Once both sample and label lists are generated, the second step consists in
  //declaring the machine learning classifier. In our case we use an SVM model
  //with the help of the \doxygen{otb}{SVMMachineLearningModel} class which is
  //derived from the \doxygen{otb}{MachineLearningModel} class.
  //This pure virtual class is based on the machine learning framework of the
  //OpenCV library (\cite{opencv_library}) which handles other classifiers than
  //the SVM.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  typedef otb::SVMMachineLearningModel<InputValueType, TargetValueType> SVMType;

  SVMType::Pointer SVMClassifier = SVMType::New();

  SVMClassifier->SetInputListSample(InputListSample);
  SVMClassifier->SetTargetListSample(TargetListSample);

  SVMClassifier->SetKernelType(CvSVM::LINEAR);
  // Software Guide : EndCodeSnippet

  // Software Guide : BeginLatex
  //
  // Once the classifier is parametrized with both input lists and default parameters, except
  // for the kernel type in our example of SVM model estimation, the model
  // training is computed with the \code{Train} method. Finally, the \code{Save} method
  // exports the model to a text file. All the available classifiers based on OpenCV are
  // implemented with these interfaces. Like for the SVM model training, the other classifiers
  // can be parametrized with specific settings.
  //
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  SVMClassifier->Train();
  SVMClassifier->Save(outputModelFileName);
  // Software Guide : EndCodeSnippet

}

