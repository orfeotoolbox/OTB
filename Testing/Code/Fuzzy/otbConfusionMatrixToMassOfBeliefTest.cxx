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

#include "otbConfusionMatrixToMassOfBelief.h"

int otbConfusionMatrixToMassOfBeliefTest(int argc, char* argv[])
{
  // Filling the Confusion Matrix
  typedef unsigned int LabelType;
  typedef itk::FixedArray<LabelType, 1> RLabelType;
  typedef itk::Statistics::ListSample<RLabelType> RListLabelType;
  typedef itk::FixedArray<LabelType, 1> PLabelType;
  typedef itk::Statistics::ListSample<PLabelType> PListLabelType;
  typedef otb::ConfusionMatrixCalculator<RListLabelType, PListLabelType> ConfusionMatrixCalculatorType;

  //Filling the Confusion Matrix from samples
  ConfusionMatrixCalculatorType::Pointer confusionMatrixCalculator = ConfusionMatrixCalculatorType::New();

  RListLabelType::Pointer refLabels = RListLabelType::New();
  PListLabelType::Pointer prodLabels = PListLabelType::New();

  RListLabelType::Iterator itRefLabels;
  PListLabelType::Iterator itProdLabels;

  int nbSamples = 12;
  int nbClasses = 4;

  // Reference samples: 1 2 3 4 1 2 3 4 1 2 3 4
  // Classified reference samples: 1 3 3 4 4 2 3 4 4 4 4 3
  std::vector<LabelType> labelsClassified;
  labelsClassified.push_back(1);
  labelsClassified.push_back(3);
  labelsClassified.push_back(3);
  labelsClassified.push_back(4);
  labelsClassified.push_back(4);
  labelsClassified.push_back(2);
  labelsClassified.push_back(3);
  labelsClassified.push_back(4);
  labelsClassified.push_back(4);
  labelsClassified.push_back(4);
  labelsClassified.push_back(4);
  labelsClassified.push_back(3);

  for (int i = 0; i < nbSamples; ++i)
    {
    int label = (i % nbClasses) + 1;
    refLabels->PushBack(label);
    prodLabels->PushBack(labelsClassified[i]);
    }

  int k = 0;
  for (itRefLabels = refLabels->Begin(), itProdLabels = prodLabels->Begin(); itRefLabels != refLabels->End(); ++itRefLabels, ++itProdLabels)
    {
    std::cout << "refLabels[" << k << "] = " << itRefLabels.GetMeasurementVector()[0] << "; prodLabels[" << k << "] = "
        << itProdLabels.GetMeasurementVector()[0] << std::endl;
    ++k;
    }

  confusionMatrixCalculator->SetReferenceLabels(refLabels);
  confusionMatrixCalculator->SetProducedLabels(prodLabels);
  confusionMatrixCalculator->Update();

  ConfusionMatrixCalculatorType::ConfusionMatrixType confusionMatrix = confusionMatrixCalculator->GetConfusionMatrix();
  std::cout << std::endl;
  std::cout << "confusion matrix" << std::endl << confusionMatrix << std::endl;


  // Converting the Confusion Matrix into a std::map<ClassLabelType, MassType> of Masses of Belief for each label
  typedef otb::ConfusionMatrixToMassOfBelief<ConfusionMatrixCalculatorType> ConfusionMatrixToMassOfBeliefType;
  typedef ConfusionMatrixToMassOfBeliefType::MassOfBeliefDefinitionMethod MassOfBeliefDefinitionMethod;
  typedef ConfusionMatrixToMassOfBeliefType::SingleClassLabelMassMapType SingleClassLabelMassMapType;

  ConfusionMatrixToMassOfBeliefType::Pointer confMatToMass = ConfusionMatrixToMassOfBeliefType::New();
  MassOfBeliefDefinitionMethod massOfBeliefDefMethod;

  std::string massOfBeliefDefMethodStr = argv[1];
  if (massOfBeliefDefMethodStr.compare("PRECISION") == 0)
    {
    massOfBeliefDefMethod = ConfusionMatrixToMassOfBeliefType::PRECISION;
    }
  else
    {
    if (massOfBeliefDefMethodStr.compare("RECALL") == 0)
      {
      massOfBeliefDefMethod = ConfusionMatrixToMassOfBeliefType::RECALL;
      }
    }

  confMatToMass->SetConfusionMatrixCalculator(confusionMatrixCalculator);
  confMatToMass->SetDefinitionMethod(massOfBeliefDefMethod);
  confMatToMass->Update();
  SingleClassLabelMassMapType mapMOB = confMatToMass->GetMapMassOfBelief();
  SingleClassLabelMassMapType::iterator itMapMOB;

  for (itMapMOB = mapMOB.begin(); itMapMOB != mapMOB.end(); ++itMapMOB)
    {
    std::cout << "mapMOB[" << itMapMOB->first << "] = " << itMapMOB->second << std::endl;
    }


  //******************************************
  // Baselines for the different measurements
  //******************************************

  SingleClassLabelMassMapType mapMOBBLPrecision, mapMOBBLRecall;
  SingleClassLabelMassMapType::iterator itMapMOBBL;

  mapMOBBLPrecision[1] = 1;
  mapMOBBLPrecision[2] = 1;
  mapMOBBLPrecision[3] = 1.0 / 2.0;
  mapMOBBLPrecision[4] = 1.0 / 3.0;

  mapMOBBLRecall[1] = 1.0 / 3.0;
  mapMOBBLRecall[2] = 1.0 / 3.0;
  mapMOBBLRecall[3] = 2.0 / 3.0;
  mapMOBBLRecall[4] = 2.0 / 3.0;

  if ((mapMOB != mapMOBBLPrecision) && (massOfBeliefDefMethodStr.compare("PRECISION") == 0))
    {
    std::cout << std::endl;
    std::cout << "ERROR in Map Mass of Belief Precision" << std::endl;
    for (itMapMOBBL = mapMOBBLPrecision.begin(), itMapMOB = mapMOB.begin();
        itMapMOBBL != mapMOBBLPrecision.end(); ++itMapMOBBL, ++itMapMOB)
      {
      std::cout << "mapMOBBLPrecision[" << itMapMOBBL->first << "] = " << itMapMOBBL->second << "; ";
      std::cout << "mapMOB[" << itMapMOB->first << "] = " << itMapMOB->second << std::endl;
      }
    return EXIT_FAILURE;
    }

  if ((mapMOB != mapMOBBLRecall) && (massOfBeliefDefMethodStr.compare("RECALL") == 0))
    {
    std::cout << std::endl;
    std::cout << "ERROR in Map Mass of Belief Recall" << std::endl;
    for (itMapMOBBL = mapMOBBLRecall.begin(), itMapMOB = mapMOB.begin();
        itMapMOBBL != mapMOBBLRecall.end(); ++itMapMOBBL, ++itMapMOB)
      {
      std::cout << "mapMOBBLRecall[" << itMapMOBBL->first << "] = " << itMapMOBBL->second << "; ";
      std::cout << "mapMOB[" << itMapMOB->first << "] = " << itMapMOB->second << std::endl;
      }
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;
}
