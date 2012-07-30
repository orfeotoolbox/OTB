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

#include "itkListSample.h"
#include "otbConfusionMatrixCalculator.h"

int otbConfusionMatrixCalculatorNew(int argc, char* argv[])
{

  typedef itk::VariableLengthVector<int>          PLabelType;
  typedef itk::Statistics::ListSample<PLabelType> PListLabelType;
  typedef itk::FixedArray<int, 1>                 RLabelType;
  typedef itk::Statistics::ListSample<RLabelType> RListLabelType;
  typedef otb::ConfusionMatrixCalculator<RListLabelType,
      PListLabelType> CalculatorType;

  CalculatorType::Pointer calculator = CalculatorType::New();

  std::cout << calculator << std::endl;

  return EXIT_SUCCESS;
}

int otbConfusionMatrixCalculatorSetListSamples(int argc, char* argv[])
{

  if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0] << " nbSamples nbClasses " << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::VariableLengthVector<int>          PLabelType;
  typedef itk::Statistics::ListSample<PLabelType> PListLabelType;
  typedef itk::FixedArray<int, 1>                 RLabelType;
  typedef itk::Statistics::ListSample<RLabelType> RListLabelType;
  typedef otb::ConfusionMatrixCalculator<RListLabelType,
      PListLabelType> CalculatorType;

  CalculatorType::Pointer calculator = CalculatorType::New();

  RListLabelType::Pointer refLabels = RListLabelType::New();
  PListLabelType::Pointer prodLabels = PListLabelType::New();

  int nbSamples = atoi(argv[1]);
  int nbClasses = atoi(argv[2]);

  for (int i = 0; i < nbSamples; ++i)
    {
    int        label = (i % nbClasses) + 1;
    PLabelType plab;
    plab.SetSize(1);
    plab[0] = label;
    refLabels->PushBack(label);
    prodLabels->PushBack(plab);
    }

  calculator->SetReferenceLabels(refLabels);
  calculator->SetProducedLabels(prodLabels);

  //calculator->Update();

  return EXIT_SUCCESS;
}


/*
compare to results obtained with source code available here
http://en.wikibooks.org/wiki/Algorithm_Implementation/Statistics/Fleiss%27_kappa
*/
int otbConfusionMatrixCalculatorWrongSize(int argc, char* argv[])
{

  if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0] << " nbSamples nbClasses " << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::VariableLengthVector<int>          PLabelType;
  typedef itk::Statistics::ListSample<PLabelType> PListLabelType;
  typedef itk::FixedArray<int, 1>                 RLabelType;
  typedef itk::Statistics::ListSample<RLabelType> RListLabelType;
  typedef otb::ConfusionMatrixCalculator<RListLabelType,
      PListLabelType> CalculatorType;

  CalculatorType::Pointer calculator = CalculatorType::New();

  RListLabelType::Pointer refLabels = RListLabelType::New();
  PListLabelType::Pointer prodLabels = PListLabelType::New();

  int nbSamples = atoi(argv[1]);
  int nbClasses = atoi(argv[2]);

  for (int i = 0; i < nbSamples; ++i)
    {
    int        label = (i % nbClasses) + 1;
    PLabelType plab;
    plab.SetSize(1);
    plab[0] = label;
    refLabels->PushBack(label);
    prodLabels->PushBack(plab);
    }

  PLabelType plab;
  plab.SetSize(1);
  plab[0] = 0;
  prodLabels->PushBack(plab);

  calculator->SetReferenceLabels(refLabels);
  calculator->SetProducedLabels(prodLabels);

  try
    {
    calculator->Update();
    }
  catch (itk::ExceptionObject)
    {
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int otbConfusionMatrixCalculatorUpdate(int argc, char* argv[])
{

  if (argc != 3)
    {
    std::cerr << "Usage: " << argv[0] << " nbSamples nbClasses " << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::VariableLengthVector<int>          PLabelType;
  typedef itk::Statistics::ListSample<PLabelType> PListLabelType;
  typedef itk::FixedArray<int, 1>                 RLabelType;
  typedef itk::Statistics::ListSample<RLabelType> RListLabelType;
  typedef otb::ConfusionMatrixCalculator<RListLabelType,
      PListLabelType> CalculatorType;
  typedef CalculatorType::ConfusionMatrixType     ConfusionMatrixType;

  CalculatorType::Pointer calculator = CalculatorType::New();

  RListLabelType::Pointer refLabels = RListLabelType::New();
  PListLabelType::Pointer prodLabels = PListLabelType::New();

  int nbSamples = atoi(argv[1]);
  int nbClasses = atoi(argv[2]);

  ConfusionMatrixType confusionMatrix = ConfusionMatrixType(nbClasses, nbClasses);
  confusionMatrix.Fill(0);

  // confusionMatrix(0,1) =;
  // confusionMatrix(0,1) =;
  // confusionMatrix(0,1) =;

  for (int i = 0; i < nbSamples; ++i)
    {
    int label;

    label = (i % nbClasses) + 1;

    PLabelType plab;
    plab.SetSize(1);
    if (i == 0)
      {
      plab[0] = nbClasses;
      }
    else
      {
      plab[0] = label;
      }
    refLabels->PushBack(label);
    prodLabels->PushBack(plab);
    }

  calculator->SetReferenceLabels(refLabels);
  calculator->SetProducedLabels(prodLabels);

  //calculator->SetConfusionMatrix(confusionMatrix);
  calculator->Update();

  if (static_cast<int>(calculator->GetNumberOfClasses()) != nbClasses)
    {
    std::cerr << "Wrong number of classes" << std::endl;
    return EXIT_FAILURE;
    }
  if (static_cast<int>(calculator->GetNumberOfSamples()) != nbSamples)
    {
    std::cerr << "Wrong number of samples" << std::endl;
    return EXIT_FAILURE;
    }

  CalculatorType::ConfusionMatrixType confmat = calculator->GetConfusionMatrix();

  // double totalError = 0.0;

  // for (int i = 0; i < nbClasses; ++i)
  //   for (int j = 0; j < nbClasses; ++j)
  //     {
  //     double goodValue = 0.0;
  //     if (i == j) goodValue = nbSamples / nbClasses;
  //     else
  //     if (confmat(i, j) != goodValue) totalError += confmat(i, j);
  //     }

  // if (totalError > 0.001)
  //   {
  //   std::cerr << confmat << std::endl;
  //   std::cerr << "Error = " << totalError << std::endl;
  //   return EXIT_FAILURE;
  //   }

  // if (calculator->GetKappaIndex() != 1.0)
  //   {
  //   std::cerr << "Kappa = " << calculator->GetKappaIndex() << std::endl;
  //   return EXIT_FAILURE;
  //   }

  // if (calculator->GetOverallAccuracy() != 1.0)
  //   {
  //   std::cerr << "OA = " << calculator->GetOverallAccuracy() << std::endl;
  //   return EXIT_FAILURE;
  //   }

  std::cout << "confusion matrix" << std::endl  << confmat << std::endl;

  for (int itClasses = 0; itClasses < nbClasses; itClasses++)
      {
      std::cout << "Precision of class [" << itClasses << "] vs all: " << calculator->GetPrecisions()[itClasses] << std::endl;
      std::cout <<"Recall of class [" << itClasses << "] vs all: " << calculator->GetRecalls()[itClasses] << std::endl;
      std::cout <<"F-score of class [" << itClasses << "] vs all: " << calculator->GetFScores()[itClasses] << "\n" << std::endl;
      }
  std::cout << "Precision of the different class: " << calculator->GetPrecisions() << std::endl;
  std::cout << "Recall of the different class: " << calculator->GetRecalls() << std::endl;
  std::cout << "F-score of the different class: " << calculator->GetFScores() << std::endl;

  std::cout << "Kappa = " << calculator->GetKappaIndex() << std::endl;
  std::cout << "OA = " << calculator->GetOverallAccuracy() << std::endl;

  const double oa = 3 / static_cast<double>(nbSamples);

  if (vcl_abs (calculator->GetOverallAccuracy() - oa) > 0.000001)
    {
    return EXIT_FAILURE;
    }
  else
    {
    return EXIT_SUCCESS;
    }
}
