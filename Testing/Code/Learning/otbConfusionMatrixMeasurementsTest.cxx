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
#include "itkMacro.h"
#include <iostream>

#include "otbConfusionMatrixMeasurements.h"

int otbConfusionMatrixMeasurementsNew(int argc, char* argv[])
{
  typedef itk::VariableSizeMatrix<double> ConfusionMatrixType;

  // filter type
  typedef otb::ConfusionMatrixMeasurements<ConfusionMatrixType> ConfusionMatrixMeasurementsType;

  // filter
  ConfusionMatrixMeasurementsType::Pointer ConfMatMeasurements = ConfusionMatrixMeasurementsType::New();
  std::cout << ConfMatMeasurements << std::endl;

  return EXIT_SUCCESS;
}


int otbConfusionMatrixMeasurementsTest(int argc, char* argv[])
{
  typedef itk::VariableSizeMatrix<double> ConfusionMatrixType;

  // filter type
  typedef otb::ConfusionMatrixMeasurements<ConfusionMatrixType> ConfusionMatrixMeasurementsType;
  typedef ConfusionMatrixMeasurementsType::ClassLabelType ClassLabelType;
  typedef ConfusionMatrixMeasurementsType::MapOfClassesType MapOfClassesType;
  typedef ConfusionMatrixMeasurementsType::MapOfIndicesType MapOfIndicesType;
  typedef ConfusionMatrixMeasurementsType::MeasurementType MeasurementType;

  // mapOfClasses[label] = index in the rows/columns of the confusion matrix
  MapOfClassesType mapOfClasses;
  mapOfClasses[1] = 0;
  mapOfClasses[2] = 1;
  mapOfClasses[3] = 2;
  mapOfClasses[4] = 3;

  unsigned int nbClasses = mapOfClasses.size();
  ConfusionMatrixType ConfMat = ConfusionMatrixType(nbClasses, nbClasses);
  ConfMat(0, 0) = 1, ConfMat(0, 1) = 0, ConfMat(0, 2) = 0, ConfMat(0, 3) = 2;
  ConfMat(1, 0) = 0, ConfMat(1, 1) = 1, ConfMat(1, 2) = 1, ConfMat(1, 3) = 1;
  ConfMat(2, 0) = 0, ConfMat(2, 1) = 0, ConfMat(2, 2) = 2, ConfMat(2, 3) = 1;
  ConfMat(3, 0) = 0, ConfMat(3, 1) = 0, ConfMat(3, 2) = 1, ConfMat(3, 3) = 2;
  std::cout << "confusion matrix = " << std::endl << ConfMat << std::endl;

  // filter
  ConfusionMatrixMeasurementsType::Pointer confMatMeasurements = ConfusionMatrixMeasurementsType::New();

  confMatMeasurements->SetMapOfClasses(mapOfClasses);
  confMatMeasurements->SetConfusionMatrix(ConfMat);
  confMatMeasurements->Update();

  // mapOfIndices[index] = label associated to the rows/columns of the confusion matrix
  MapOfIndicesType mapOfIndices;
  MapOfIndicesType::iterator itMapOfIndices;
  mapOfIndices = confMatMeasurements->GetMapOfIndices();

  for (itMapOfIndices = mapOfIndices.begin(); itMapOfIndices != mapOfIndices.end(); ++itMapOfIndices)
    {
    int itClasses = itMapOfIndices->first;
    ClassLabelType label = itMapOfIndices->second;

    std::cout << "Number of True Positives of class [" << label << "] = "
        << confMatMeasurements->GetTruePositiveValues()[itClasses] << std::endl;
    std::cout << "Number of False Negatives of class [" << label << "] = "
            << confMatMeasurements->GetFalseNegativeValues()[itClasses] << std::endl;
    std::cout << "Number of False Positives of class [" << label << "] = "
            << confMatMeasurements->GetFalsePositiveValues()[itClasses] << std::endl;
    std::cout << "Number of True Negatives of class [" << label << "] = "
            << confMatMeasurements->GetTrueNegativeValues()[itClasses] << std::endl;

    std::cout << "Precision of class [" << label << "] vs all: " << confMatMeasurements->GetPrecisions()[itClasses]
        << std::endl;
    std::cout << "Recall of class [" << label << "] vs all: " << confMatMeasurements->GetRecalls()[itClasses] << std::endl;
    std::cout << "F-score of class [" << label << "] vs all: " << confMatMeasurements->GetFScores()[itClasses] << "\n"
        << std::endl;
    }
  std::cout << "Number of True Positives of the different classes: "<< confMatMeasurements->GetTruePositiveValues()<< std::endl;
  std::cout << "Number of False Negatives of the different classes: "<< confMatMeasurements->GetFalseNegativeValues()<< std::endl;
  std::cout << "Number of False Positives of the different classes: "<< confMatMeasurements->GetFalsePositiveValues()<< std::endl;
  std::cout << "Number of True Negatives of the different classes: "<< confMatMeasurements->GetTrueNegativeValues()<< std::endl;
  std::cout << "Precision of the different classes: " << confMatMeasurements->GetPrecisions() << std::endl;
  std::cout << "Recall of the different classes: " << confMatMeasurements->GetRecalls() << std::endl;
  std::cout << "F-score of the different classes: " << confMatMeasurements->GetFScores() << std::endl;

  std::cout << "Kappa = " << confMatMeasurements->GetKappaIndex() << std::endl;
  std::cout << "OA = " << confMatMeasurements->GetOverallAccuracy() << std::endl;


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


  if (confMatMeasurements->GetTruePositiveValues() != blTP)
    {
    std::cout << std::endl;
    std::cout << "ERROR in True Positive Values" << std::endl;
    std::cout << "baseline TPs = " << blTP << std::endl;
    std::cout << "calculated TPs = " << confMatMeasurements->GetTruePositiveValues();
    return EXIT_FAILURE;
    }

  if (confMatMeasurements->GetFalseNegativeValues() != blFN)
    {
    std::cout << std::endl;
    std::cout << "ERROR in False Negative Values" << std::endl;
    std::cout << "baseline FNs = " << blFN << std::endl;
    std::cout << "calculated FNs = " << confMatMeasurements->GetFalseNegativeValues();
    return EXIT_FAILURE;
    }

  if (confMatMeasurements->GetFalsePositiveValues() != blFP)
    {
    std::cout << std::endl;
    std::cout << "ERROR in False Positive Values" << std::endl;
    std::cout << "baseline FPs = " << blFP << std::endl;
    std::cout << "calculated FPs = " << confMatMeasurements->GetFalsePositiveValues();
    return EXIT_FAILURE;
    }

  if (confMatMeasurements->GetTrueNegativeValues() != blTN)
    {
    std::cout << std::endl;
    std::cout << "ERROR in True Negative Values" << std::endl;
    std::cout << "baseline TNs = " << blTN << std::endl;
    std::cout << "calculated TNs = " << confMatMeasurements->GetTrueNegativeValues();
    return EXIT_FAILURE;
    }

  if (confMatMeasurements->GetPrecisions() != blPrecisions)
    {
    std::cout << std::endl;
    std::cout << "ERROR in Precisions" << std::endl;
    std::cout << "baseline Precisions = " << blPrecisions << std::endl;
    std::cout << "calculated Precisions = " << confMatMeasurements->GetPrecisions();
    return EXIT_FAILURE;
    }

  if (confMatMeasurements->GetRecalls() != blRecalls)
    {
    std::cout << std::endl;
    std::cout << "ERROR in Recalls" << std::endl;
    std::cout << "baseline Recalls = " << blRecalls << std::endl;
    std::cout << "calculated Recalls = " << confMatMeasurements->GetRecalls();
    return EXIT_FAILURE;
    }

  if (confMatMeasurements->GetFScores() != blFScores)
    {
    std::cout << std::endl;
    std::cout << "ERROR in FScores" << std::endl;
    std::cout << "baseline FScores = " << blFScores << std::endl;
    std::cout << "calculated FScores = " << confMatMeasurements->GetFScores();
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
