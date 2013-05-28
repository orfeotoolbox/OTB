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
#include <fstream>

#include "otbConfusionMatrixMeasurements.h"



typedef unsigned long                                   ConfusionMatrixEltType;
typedef itk::VariableSizeMatrix<ConfusionMatrixEltType> ConfusionMatrixType;
typedef unsigned char                                   ClassLabelType;
typedef int                                             LabelPixelType;

// filter type
typedef otb::ConfusionMatrixMeasurements<ConfusionMatrixType, ClassLabelType> ConfusionMatrixMeasurementsType;
typedef ConfusionMatrixMeasurementsType::MapOfClassesType                     MapOfClassesType;
typedef ConfusionMatrixMeasurementsType::MapOfIndicesType                     MapOfIndicesType;
typedef ConfusionMatrixMeasurementsType::MeasurementType                      MeasurementType;

int CSVConfusionMatrixFileReader(const std::string fileName, MapOfClassesType &mapOfClassesRefClX, ConfusionMatrixType &confusionMatrixClX)
  {
    std::ifstream inFile;
    inFile.open(fileName.c_str());

    if (!inFile)
      {
      std::cerr << "Confusion Matrix File opening problem with file:" << std::endl;
      std::cerr << fileName.c_str() << std::endl;
      return EXIT_FAILURE;
      }
    else
      {
      LabelPixelType labelRef = 0, labelProd = 0;
      std::string currentLine, refLabelsLine, prodLabelsLine, currentValue;
      const char endCommentChar = ':';
      const char separatorChar = ',';
      const char eolChar = '\n';
      std::getline(inFile, refLabelsLine, endCommentChar); // Skips the comments
      std::getline(inFile, refLabelsLine, eolChar); // Gets the first line after the comment char until the End Of Line char
      std::getline(inFile, prodLabelsLine, endCommentChar); // Skips the comments
      std::getline(inFile, prodLabelsLine, eolChar); // Gets the second line after the comment char until the End Of Line char

      std::istringstream issRefLabelsLine(refLabelsLine);
      std::istringstream issProdLabelsLine(prodLabelsLine);

      MapOfClassesType mapOfClassesProdClX;

      mapOfClassesRefClX.clear();
      mapOfClassesProdClX.clear();
      int itLab = 0;
      while (issRefLabelsLine.good())
        {
        std::getline(issRefLabelsLine, currentValue, separatorChar);
        labelRef = static_cast<LabelPixelType> (std::atoi(currentValue.c_str()));
        mapOfClassesRefClX[labelRef] = itLab;
        ++itLab;
        }

      itLab = 0;
      while (issProdLabelsLine.good())
        {
        std::getline(issProdLabelsLine, currentValue, separatorChar);
        labelProd = static_cast<LabelPixelType> (std::atoi(currentValue.c_str()));
        mapOfClassesProdClX[labelProd] = itLab;
        ++itLab;
        }

      unsigned int nbRefLabelsClk = mapOfClassesRefClX.size();
      unsigned int nbProdLabelsClk = mapOfClassesProdClX.size();
      ConfusionMatrixType confusionMatrixClXTemp;
      confusionMatrixClXTemp = ConfusionMatrixType(nbRefLabelsClk, nbProdLabelsClk);
      confusionMatrixClXTemp.Fill(0);

      // Reading the confusion matrix confusionMatrixClXTemp from the file
      for (unsigned int itRow = 0; itRow < nbRefLabelsClk; ++itRow)
        {
        //Gets the itRow^th line after the header lines with the labels
        std::getline(inFile, currentLine, eolChar);
        std::istringstream issCurrentLine(currentLine);
        unsigned int itCol = 0;
        while (issCurrentLine.good())
          {
          std::getline(issCurrentLine, currentValue, separatorChar);
          confusionMatrixClXTemp(itRow, itCol) = static_cast<ConfusionMatrixEltType> (std::atoi(currentValue.c_str()));
          ++itCol;
          }
        }

      MapOfClassesType::iterator  itMapOfClassesRef, itMapOfClassesProd;

      // Formatting confusionMatrixClX from confusionMatrixClXTemp in order to make confusionMatrixClX a square matrix
      // from the reference labels in mapOfClassesRefClX
      int indiceLabelRef = 0, indiceLabelProd = 0;
      int indiceLabelRefTemp = 0, indiceLabelProdTemp = 0;
      // Initialization of confusionMatrixClX
      confusionMatrixClX = ConfusionMatrixType(nbRefLabelsClk, nbRefLabelsClk);
      confusionMatrixClX.Fill(0);
      for (itMapOfClassesRef = mapOfClassesRefClX.begin(); itMapOfClassesRef != mapOfClassesRefClX.end(); ++itMapOfClassesRef)
        {
        // labels labelRef of mapOfClassesRefClX are already sorted
        labelRef = itMapOfClassesRef->first;
        indiceLabelRefTemp = itMapOfClassesRef->second;

        for (itMapOfClassesProd = mapOfClassesProdClX.begin(); itMapOfClassesProd != mapOfClassesProdClX.end(); ++itMapOfClassesProd)
          {
          // labels labelProd of mapOfClassesProdClX are already sorted
          labelProd = itMapOfClassesProd->first;
          indiceLabelProdTemp = itMapOfClassesProd->second;

          // If labelProd is present in mapOfClassesRefClX
          if (mapOfClassesRefClX.count(labelProd) != 0)
            {
            // Indice of labelProd in mapOfClassesRefClX; itMapOfClassesRef->second elements are already SORTED
            indiceLabelProd = mapOfClassesRefClX[labelProd];
            confusionMatrixClX(indiceLabelRef, indiceLabelProd) = confusionMatrixClXTemp(indiceLabelRefTemp, indiceLabelProdTemp);
            }
          }
        ++indiceLabelRef;
        }
      }
    inFile.close();
    return EXIT_SUCCESS;
  } // END CSVConfusionMatrixFileReader




int otbConfusionMatrixMeasurementsNew(int argc, char* argv[])
{
  /*typedef unsigned long                                   ConfusionMatrixEltType;
  typedef itk::VariableSizeMatrix<ConfusionMatrixEltType> ConfusionMatrixType;
  typedef unsigned char                                   LabelType; */

  // filter types
  typedef otb::ConfusionMatrixMeasurements<ConfusionMatrixType, ClassLabelType> ConfusionMatrixMeasurements2TemplatesType;
  typedef otb::ConfusionMatrixMeasurements<ConfusionMatrixType>                 ConfusionMatrixMeasurements1TemplateType;
  typedef otb::ConfusionMatrixMeasurements<>                                    ConfusionMatrixMeasurements0TemplateType;

  // filters
  ConfusionMatrixMeasurements2TemplatesType::Pointer
      confusionMatrixMeasurements2Templates = ConfusionMatrixMeasurements2TemplatesType::New();
  ConfusionMatrixMeasurements1TemplateType::Pointer
      confusionMatrixMeasurements1Template = ConfusionMatrixMeasurements1TemplateType::New();
  ConfusionMatrixMeasurements0TemplateType::Pointer
      confusionMatrixMeasurements0Template = ConfusionMatrixMeasurements0TemplateType::New();

  std::cout << confusionMatrixMeasurements2Templates << std::endl;
  std::cout << std::endl;
  std::cout << confusionMatrixMeasurements1Template << std::endl;
  std::cout << std::endl;
  std::cout << confusionMatrixMeasurements0Template << std::endl;
  std::cout << std::endl;

  return EXIT_SUCCESS;
}


int otbConfusionMatrixMeasurementsTest(int argc, char* argv[])
{
  /*typedef unsigned long                                   ConfusionMatrixEltType;
  typedef itk::VariableSizeMatrix<ConfusionMatrixEltType> ConfusionMatrixType;
  typedef unsigned char                                   ClassLabelType;

  // filter type
  typedef otb::ConfusionMatrixMeasurements<ConfusionMatrixType, ClassLabelType> ConfusionMatrixMeasurementsType;
  typedef ConfusionMatrixMeasurementsType::MapOfClassesType MapOfClassesType;
  typedef ConfusionMatrixMeasurementsType::MapOfIndicesType MapOfIndicesType;
  typedef ConfusionMatrixMeasurementsType::MeasurementType MeasurementType; */

  // mapOfClasses[label] = index in the rows/columns of the confusion matrix
  MapOfClassesType mapOfClasses;
  mapOfClasses['a'] = 0;
  mapOfClasses['b'] = 1;
  mapOfClasses['c'] = 2;
  mapOfClasses['d'] = 3;

  unsigned int nbClasses = mapOfClasses.size();
  ConfusionMatrixType confMat = ConfusionMatrixType(nbClasses, nbClasses);
  confMat(0, 0) = 1, confMat(0, 1) = 0, confMat(0, 2) = 0, confMat(0, 3) = 2;
  confMat(1, 0) = 0, confMat(1, 1) = 1, confMat(1, 2) = 1, confMat(1, 3) = 1;
  confMat(2, 0) = 0, confMat(2, 1) = 0, confMat(2, 2) = 2, confMat(2, 3) = 1;
  confMat(3, 0) = 0, confMat(3, 1) = 0, confMat(3, 2) = 1, confMat(3, 3) = 2;
  std::cout << "confusion matrix = " << std::endl << confMat << std::endl;

  // filter
  ConfusionMatrixMeasurementsType::Pointer confMatMeasurements = ConfusionMatrixMeasurementsType::New();

  confMatMeasurements->SetMapOfClasses(mapOfClasses);
  confMatMeasurements->SetConfusionMatrix(confMat);
  confMatMeasurements->Compute();

  // mapOfIndices[index] = label associated to the rows/columns of the confusion matrix
  MapOfIndicesType mapOfIndices;
  MapOfIndicesType::iterator itMapOfIndices;
  mapOfIndices = confMatMeasurements->GetMapOfIndices();

  for (itMapOfIndices = mapOfIndices.begin(); itMapOfIndices != mapOfIndices.end(); ++itMapOfIndices)
    {
    unsigned int itClasses = itMapOfIndices->first;
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
  for (unsigned int itC = 0; itC < nbClasses; itC++)
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



int otbConfusionMatrixConcatenateTest(int argc, char* argv[])
{
  unsigned nbClassifiers = argc - 1;

  MapOfClassesType mapOfClasses;
  MapOfIndicesType mapOfIndices;
  MapOfIndicesType::iterator itMapOfIndices;
  ConfusionMatrixType addConfusionMatrix;
  unsigned int nbClasses = 0;
  for (unsigned itClk = 0; itClk < nbClassifiers; ++itClk)
    {
    std::string fileNameCMClk= argv[itClk + 1];
    MapOfClassesType mapOfClassesClk;
    ConfusionMatrixType confusionMatrixClk;

    CSVConfusionMatrixFileReader(fileNameCMClk, mapOfClassesClk, confusionMatrixClk);

    std::cout << fileNameCMClk << std::endl;
    std::cout << "confusion matrix[" << itClk << "] = " << std::endl << confusionMatrixClk << std::endl;

    nbClasses = confusionMatrixClk.Rows();
    if (itClk == 0)
      {
      mapOfClasses = mapOfClassesClk;
      addConfusionMatrix = ConfusionMatrixType(nbClasses, nbClasses);
      addConfusionMatrix.Fill(0);
      }

    for (unsigned itRow = 0; itRow < nbClasses; ++itRow)
      {
      for (unsigned itCol = 0; itCol < nbClasses; ++itCol)
        {
        addConfusionMatrix(itRow, itCol) += confusionMatrixClk(itRow, itCol);
        }
      }
    }

  std::cout << std::endl;
  std::cout << "*****************************************************" << std::endl;
  std::cout << "ADD confusion matrix = " << std::endl << addConfusionMatrix << std::endl;
  std::cout << "*****************************************************" << std::endl << std::endl;

  // filter
  ConfusionMatrixMeasurementsType::Pointer confMatMeasurements = ConfusionMatrixMeasurementsType::New();
  confMatMeasurements->SetMapOfClasses(mapOfClasses);
  confMatMeasurements->SetConfusionMatrix(addConfusionMatrix);
  confMatMeasurements->Compute();

  mapOfIndices = confMatMeasurements->GetMapOfIndices();

  for (itMapOfIndices = mapOfIndices.begin(); itMapOfIndices != mapOfIndices.end(); ++itMapOfIndices)
    {
    unsigned int itClasses = itMapOfIndices->first;
    LabelPixelType label = itMapOfIndices->second;

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

  MeasurementType blTP, blFN, blFP, blTN, blPrecisions, blRecalls, blFScores;

  ConfusionMatrixType  addConfusionMatrixBL = ConfusionMatrixType(nbClasses, nbClasses);
  addConfusionMatrixBL.Fill(0);

  addConfusionMatrixBL(0, 0) = 16162 + 20710 + 19343 + 20774 + 19283 + 19859;
  addConfusionMatrixBL(0, 1) = 8256 + 3670 + 4291 + 3217 + 5071 + 4300;
  addConfusionMatrixBL(0, 2) = 0 + 40 + 778 + 428 + 66 + 261;
  addConfusionMatrixBL(0, 3) = 2 + 0 + 8 + 1 + 0 + 0;
  addConfusionMatrixBL(1, 0) = 1 + 1057 + 133 + 1202 + 459 + 466;
  addConfusionMatrixBL(1, 1) = 49740 + 45567 + 44634 + 62128 + 54688 + 48593;
  addConfusionMatrixBL(1, 2) = 339 + 4 + 0 + 2816 + 2458 + 4654;
  addConfusionMatrixBL(1, 3) = 18213 + 21665 + 23526 + 2147 + 10688 + 14580;
  addConfusionMatrixBL(2, 0) = 4577 + 102 + 0 + 0 + 0 + 0;
  addConfusionMatrixBL(2, 1) = 1993 + 15338 + 17457 + 9438 + 17975 + 13786;
  addConfusionMatrixBL(2, 2) = 134151 + 121792 + 123262 + 130822 + 121608 + 124928;
  addConfusionMatrixBL(2, 3) = 45 + 3534 + 47 + 506 + 1183 + 2052;
  addConfusionMatrixBL(3, 0) = 0 + 0 + 0 + 0 + 0 + 0;
  addConfusionMatrixBL(3, 1) = 2748 + 2583 + 6928 + 4685 + 265 + 54;
  addConfusionMatrixBL(3, 2) = 7879 + 3483 + 2576 + 8007 + 8169 + 9991;
  addConfusionMatrixBL(3, 3) = 18038 + 22599 + 19161 + 15973 + 20231 + 18620;


  blTP.SetSize(nbClasses);
  blTP[0] = 116131;
  blTP[1] = 305350;
  blTP[2] = 756563;
  blTP[3] = 114622;
  blFN.SetSize(nbClasses);
  blFN[0] = 30389;
  blFN[1] = 104408;
  blFN[2] = 88033;
  blFN[3] = 57368;
  blFP.SetSize(nbClasses);
  blFP[0] = 7997;
  blFP[1] = 122055;
  blFP[2] = 51949;
  blFP[3] = 98197;
  blTN.SetSize(nbClasses);
  blTN[0] = 1418347;
  blTN[1] = 1041051;
  blTN[2] = 676319;
  blTN[3] = 1302677;

  blPrecisions.SetSize(nbClasses);
  blRecalls.SetSize(nbClasses);
  blFScores.SetSize(nbClasses);
  for (unsigned int itC = 0; itC < nbClasses; itC++)
    {
    blPrecisions[itC] = blTP[itC] / (blTP[itC] + blFP[itC]);
    blRecalls[itC] = blTP[itC] / (blTP[itC] + blFN[itC]);
    blFScores[itC] = 2 * blPrecisions[itC] * blRecalls[itC] / (blPrecisions[itC] + blRecalls[itC]);
    }


  if (addConfusionMatrix != addConfusionMatrixBL)
      {
      std::cout << std::endl;
      std::cout << "ERROR in addConfusionMatrix" << std::endl;
      std::cout << "baseline addConfusionMatrixBL = " << std::endl << addConfusionMatrixBL << std::endl;
      std::cout << "calculated addConfusionMatrix = " << std::endl << addConfusionMatrix;
      return EXIT_FAILURE;
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
