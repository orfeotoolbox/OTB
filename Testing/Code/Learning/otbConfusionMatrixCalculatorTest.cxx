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

  //calculator->Compute();

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
    calculator->Compute();
    }
  catch (itk::ExceptionObject)
    {
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}

int otbConfusionMatrixCalculatorCompute(int argc, char* argv[])
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
  calculator->Compute();

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


int otbConfusionMatrixCalculatorComputeWithBaseline(int argc, char* argv[])
{
  typedef char                                                           ClassLabelType;
  typedef itk::VariableLengthVector<ClassLabelType>                      PLabelType;
  typedef itk::Statistics::ListSample<PLabelType>                        PListLabelType;
  typedef itk::FixedArray<ClassLabelType, 1>                             RLabelType;
  typedef itk::Statistics::ListSample<RLabelType>                        RListLabelType;
  typedef otb::ConfusionMatrixCalculator<RListLabelType, PListLabelType> CalculatorType;

  typedef CalculatorType::MeasurementType MeasurementType;

  CalculatorType::Pointer calculator = CalculatorType::New();

  RListLabelType::Pointer refLabels = RListLabelType::New();
  PListLabelType::Pointer prodLabels = PListLabelType::New();

  RListLabelType::Iterator itRefLabels;
  PListLabelType::Iterator itProdLabels;

  int nbSamples = 12;
  int nbClasses = 4;

  // Reference samples: a b c d a b c d a b c d
  // Classified reference samples: a c c d d b c d d d d c
  std::vector<ClassLabelType> labelsUniverse, labelsClassified;

  labelsUniverse.push_back('a');
  labelsUniverse.push_back('b');
  labelsUniverse.push_back('c');
  labelsUniverse.push_back('d');

  labelsClassified.push_back('a');
  labelsClassified.push_back('c');
  labelsClassified.push_back('c');
  labelsClassified.push_back('d');
  labelsClassified.push_back('d');
  labelsClassified.push_back('b');
  labelsClassified.push_back('c');
  labelsClassified.push_back('d');
  labelsClassified.push_back('d');
  labelsClassified.push_back('d');
  labelsClassified.push_back('d');
  labelsClassified.push_back('c');

  for (int i = 0; i < nbSamples; ++i)
    {
    ClassLabelType label = labelsUniverse[(i % nbClasses)];
    PLabelType plab;
    plab.SetSize(1);
    plab[0] = labelsClassified[i];
    refLabels->PushBack(label);
    prodLabels->PushBack(plab);
    }

  int k = 0;
  for (itRefLabels = refLabels->Begin(), itProdLabels = prodLabels->Begin();
      itRefLabels != refLabels->End(); ++itRefLabels, ++itProdLabels)
    {
    std::cout << "refLabels[" << k << "] = " << itRefLabels.GetMeasurementVector()[0] << "; prodLabels[" << k
        << "] = " << itProdLabels.GetMeasurementVector()[0] << std::endl;
    ++k;
    }

  calculator->SetReferenceLabels(refLabels);
  calculator->SetProducedLabels(prodLabels);

  calculator->Compute();

  CalculatorType::ConfusionMatrixType confmat = calculator->GetConfusionMatrix();


  std::cout << std::endl;
  std::cout << "confusion matrix" << std::endl << confmat << std::endl;

  for (int itClasses = 0; itClasses < nbClasses; itClasses++)
    {
    std::cout << "Number of True Positives of class [" << labelsUniverse[itClasses] << "] = "
        << calculator->GetTruePositiveValues()[itClasses] << std::endl;
    std::cout << "Number of False Negatives of class [" << labelsUniverse[itClasses] << "] = "
            << calculator->GetFalseNegativeValues()[itClasses] << std::endl;
    std::cout << "Number of False Positives of class [" << labelsUniverse[itClasses] << "] = "
            << calculator->GetFalsePositiveValues()[itClasses] << std::endl;
    std::cout << "Number of True Negatives of class [" << labelsUniverse[itClasses] << "] = "
            << calculator->GetTrueNegativeValues()[itClasses] << std::endl;

    std::cout << "Precision of class [" << labelsUniverse[itClasses] << "] vs all: " << calculator->GetPrecisions()[itClasses]
        << std::endl;
    std::cout << "Recall of class [" << labelsUniverse[itClasses] << "] vs all: " << calculator->GetRecalls()[itClasses] << std::endl;
    std::cout << "F-score of class [" << labelsUniverse[itClasses] << "] vs all: " << calculator->GetFScores()[itClasses] << "\n"
        << std::endl;
    }
  std::cout << "Number of True Positives of the different classes: "<< calculator->GetTruePositiveValues()<< std::endl;
  std::cout << "Number of False Negatives of the different classes: "<< calculator->GetFalseNegativeValues()<< std::endl;
  std::cout << "Number of False Positives of the different classes: "<< calculator->GetFalsePositiveValues()<< std::endl;
  std::cout << "Number of True Negatives of the different classes: "<< calculator->GetTrueNegativeValues()<< std::endl;
  std::cout << "Precision of the different classes: " << calculator->GetPrecisions() << std::endl;
  std::cout << "Recall of the different classes: " << calculator->GetRecalls() << std::endl;
  std::cout << "F-score of the different classes: " << calculator->GetFScores() << std::endl;

  std::cout << "Kappa = " << calculator->GetKappaIndex() << std::endl;
  std::cout << "OA = " << calculator->GetOverallAccuracy() << std::endl;


  //******************************************
  // Baselines for the different measurements
  //******************************************
  /* The elements of the confusion matrix of the baseline blConfmat are assumed to be organized the following way,
   * for a set of labels {A, B, C, D}:
   *
   *               Aclassified    Bclassified    Cclassified    Dclassified
   * Areference        cm11           cm12           cm13           cm14
   * Breference        cm21           cm22           cm23           cm24
   * Creference        cm31           cm32           cm33           cm34
   * Dreference        cm41           cm42           cm43           cm44
   *
   *
   * which implies the following layout for the measurements:
   *
   * TruePositives    FalseNegatives
   * FalsePositives   TrueNegatives
   *
*/

  MeasurementType blTP, blFN, blFP, blTN, blPrecisions, blRecalls, blFScores;
  CalculatorType::ConfusionMatrixType blConfmat;

  blConfmat.SetSize(nbClasses, nbClasses);
  blConfmat[0][0] = 1;
  blConfmat[0][1] = 0;
  blConfmat[0][2] = 0;
  blConfmat[0][3] = 2;
  blConfmat[1][0] = 0;
  blConfmat[1][1] = 1;
  blConfmat[1][2] = 1;
  blConfmat[1][3] = 1;
  blConfmat[2][0] = 0;
  blConfmat[2][1] = 0;
  blConfmat[2][2] = 2;
  blConfmat[2][3] = 1;
  blConfmat[3][0] = 0;
  blConfmat[3][1] = 0;
  blConfmat[3][2] = 1;
  blConfmat[3][3] = 2;

  blTP.SetSize(nbClasses);
  blTP[0] = 1;
  blTP[1] = 1;
  blTP[2] = 2;
  blTP[3] = 2;
  blFN.SetSize(nbClasses);
  blFN[0] = 2;
  blFN[1] = 2;
  blFN[2] = 1;
  blFN[3] = 1;
  blFP.SetSize(nbClasses);
  blFP[0] = 0;
  blFP[1] = 0;
  blFP[2] = 2;
  blFP[3] = 4;
  blTN.SetSize(nbClasses);
  blTN[0] = 9;
  blTN[1] = 9;
  blTN[2] = 7;
  blTN[3] = 5;

  blPrecisions.SetSize(nbClasses);
  blRecalls.SetSize(nbClasses);
  blFScores.SetSize(nbClasses);
  for (int itC = 0; itC < nbClasses; itC++)
    {
    blPrecisions[itC] = blTP[itC] / (blTP[itC] + blFP[itC]);
    blRecalls[itC] = blTP[itC] / (blTP[itC] + blFN[itC]);
    blFScores[itC] = 2 * blPrecisions[itC] * blRecalls[itC] / (blPrecisions[itC] + blRecalls[itC]);
    }

  if (confmat != blConfmat)
    {
    std::cout << std::endl;
    std::cout << "ERROR in Confusion Matrix" << std::endl;
    std::cout << "baseline confmat = " << std::endl << blConfmat << std::endl;
    std::cout << "calculated confmat = " << std::endl << confmat;
    return EXIT_FAILURE;
    }

  if (calculator->GetTruePositiveValues() != blTP)
    {
    std::cout << std::endl;
    std::cout << "ERROR in True Positive Values" << std::endl;
    std::cout << "baseline TPs = " << blTP << std::endl;
    std::cout << "calculated TPs = " << calculator->GetTruePositiveValues();
    return EXIT_FAILURE;
    }

  if (calculator->GetFalseNegativeValues() != blFN)
    {
    std::cout << std::endl;
    std::cout << "ERROR in False Negative Values" << std::endl;
    std::cout << "baseline FNs = " << blFN << std::endl;
    std::cout << "calculated FNs = " << calculator->GetFalseNegativeValues();
    return EXIT_FAILURE;
    }

  if (calculator->GetFalsePositiveValues() != blFP)
    {
    std::cout << std::endl;
    std::cout << "ERROR in False Positive Values" << std::endl;
    std::cout << "baseline FPs = " << blFP << std::endl;
    std::cout << "calculated FPs = " << calculator->GetFalsePositiveValues();
    return EXIT_FAILURE;
    }

  if (calculator->GetTrueNegativeValues() != blTN)
    {
    std::cout << std::endl;
    std::cout << "ERROR in True Negative Values" << std::endl;
    std::cout << "baseline TNs = " << blTN << std::endl;
    std::cout << "calculated TNs = " << calculator->GetTrueNegativeValues();
    return EXIT_FAILURE;
    }

  if (calculator->GetPrecisions() != blPrecisions)
    {
    std::cout << std::endl;
    std::cout << "ERROR in Precisions" << std::endl;
    std::cout << "baseline Precisions = " << blPrecisions << std::endl;
    std::cout << "calculated Precisions = " << calculator->GetPrecisions();
    return EXIT_FAILURE;
    }

  if (calculator->GetRecalls() != blRecalls)
    {
    std::cout << std::endl;
    std::cout << "ERROR in Recalls" << std::endl;
    std::cout << "baseline Recalls = " << blRecalls << std::endl;
    std::cout << "calculated Recalls = " << calculator->GetRecalls();
    return EXIT_FAILURE;
    }

  if (calculator->GetFScores() != blFScores)
    {
    std::cout << std::endl;
    std::cout << "ERROR in FScores" << std::endl;
    std::cout << "baseline FScores = " << blFScores << std::endl;
    std::cout << "calculated FScores = " << calculator->GetFScores();
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;

}
