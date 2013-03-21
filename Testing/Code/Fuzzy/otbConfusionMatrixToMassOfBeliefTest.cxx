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

int otbConfusionMatrixToMassOfBeliefNew(int argc, char* argv[])
{
  typedef itk::VariableSizeMatrix<double> ConfusionMatrixType;

  // filter type
  typedef otb::ConfusionMatrixToMassOfBelief<ConfusionMatrixType> ConfusionMatrixToMassOfBeliefType;

  // filter
  ConfusionMatrixToMassOfBeliefType::Pointer confMatToMOB = ConfusionMatrixToMassOfBeliefType::New();
  std::cout << confMatToMOB << std::endl;

  return EXIT_SUCCESS;
}


int otbConfusionMatrixToMassOfBeliefTest(int argc, char* argv[])
{
  typedef itk::VariableSizeMatrix<double> ConfusionMatrixType;

  // filter type
  typedef otb::ConfusionMatrixToMassOfBelief<ConfusionMatrixType> ConfusionMatrixToMassOfBeliefType;
  typedef ConfusionMatrixToMassOfBeliefType::MapOfClassesType MapOfClassesType;
  typedef ConfusionMatrixToMassOfBeliefType::MassOfBeliefDefinitionMethod MassOfBeliefDefinitionMethod;
  typedef ConfusionMatrixToMassOfBeliefType::SingleClassLabelMassMapType SingleClassLabelMassMapType;

  // mapOfClasses[label] = index in the rows/columns of the confusion matrix
  MapOfClassesType mapOfClasses;
  mapOfClasses[1] = 0;
  mapOfClasses[2] = 1;
  mapOfClasses[3] = 2;
  mapOfClasses[4] = 3;

  unsigned int nbClasses = mapOfClasses.size();
  ConfusionMatrixType confMat = ConfusionMatrixType(nbClasses, nbClasses);
  confMat(0, 0) = 1, confMat(0, 1) = 0, confMat(0, 2) = 0, confMat(0, 3) = 2;
  confMat(1, 0) = 0, confMat(1, 1) = 1, confMat(1, 2) = 1, confMat(1, 3) = 1;
  confMat(2, 0) = 0, confMat(2, 1) = 0, confMat(2, 2) = 2, confMat(2, 3) = 1;
  confMat(3, 0) = 0, confMat(3, 1) = 0, confMat(3, 2) = 1, confMat(3, 3) = 2;
  std::cout << "confusion matrix = " << std::endl << confMat << std::endl;


  // Converting the Confusion Matrix into a std::map<ClassLabelType, MassType> of Masses of Belief for each label
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
    else
      {
      if (massOfBeliefDefMethodStr.compare("ACCURACY") == 0)
        {
        massOfBeliefDefMethod = ConfusionMatrixToMassOfBeliefType::ACCURACY;
        }
      else
        {
        if (massOfBeliefDefMethodStr.compare("KAPPA") == 0)
          {
          massOfBeliefDefMethod = ConfusionMatrixToMassOfBeliefType::KAPPA;
          }
        }
      }
    }

  confMatToMass->SetConfusionMatrix(confMat);
  confMatToMass->SetMapOfClasses(mapOfClasses);
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

  SingleClassLabelMassMapType mapMOBBLPrecision, mapMOBBLRecall, mapMOBBLAccuracy, mapMOBBLKappa;
  SingleClassLabelMassMapType::iterator itMapMOBBL;

  mapMOBBLPrecision[1] = 1;
  mapMOBBLPrecision[2] = 1;
  mapMOBBLPrecision[3] = 1.0 / 2.0;
  mapMOBBLPrecision[4] = 1.0 / 3.0;

  mapMOBBLRecall[1] = 1.0 / 3.0;
  mapMOBBLRecall[2] = 1.0 / 3.0;
  mapMOBBLRecall[3] = 2.0 / 3.0;
  mapMOBBLRecall[4] = 2.0 / 3.0;

  mapMOBBLAccuracy[1] = 1.0 / 2.0;
  mapMOBBLAccuracy[2] = 1.0 / 2.0;
  mapMOBBLAccuracy[3] = 1.0 / 2.0;
  mapMOBBLAccuracy[4] = 1.0 / 2.0;

  mapMOBBLKappa[1] = 1.0 / 3.0;
  mapMOBBLKappa[2] = 1.0 / 3.0;
  mapMOBBLKappa[3] = 1.0 / 3.0;
  mapMOBBLKappa[4] = 1.0 / 3.0;

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

  if ((mapMOB != mapMOBBLAccuracy) && (massOfBeliefDefMethodStr.compare("ACCURACY") == 0))
      {
      std::cout << std::endl;
      std::cout << "ERROR in Map Mass of Belief Accuracy" << std::endl;
      for (itMapMOBBL = mapMOBBLAccuracy.begin(), itMapMOB = mapMOB.begin();
          itMapMOBBL != mapMOBBLAccuracy.end(); ++itMapMOBBL, ++itMapMOB)
        {
        std::cout << "mapMOBBLAccuracy[" << itMapMOBBL->first << "] = " << itMapMOBBL->second << "; ";
        std::cout << "mapMOB[" << itMapMOB->first << "] = " << itMapMOB->second << std::endl;
        }
      return EXIT_FAILURE;
      }

  if ((mapMOB != mapMOBBLKappa) && (massOfBeliefDefMethodStr.compare("KAPPA") == 0))
      {
      std::cout << std::endl;
      std::cout << "ERROR in Map Mass of Belief Kappa" << std::endl;
      for (itMapMOBBL = mapMOBBLKappa.begin(), itMapMOB = mapMOB.begin();
          itMapMOBBL != mapMOBBLKappa.end(); ++itMapMOBBL, ++itMapMOB)
        {
        std::cout << "mapMOBBLKappa[" << itMapMOBBL->first << "] = " << itMapMOBBL->second << "; ";
        std::cout << "mapMOB[" << itMapMOB->first << "] = " << itMapMOB->second << std::endl;
        }
      return EXIT_FAILURE;
      }

  return EXIT_SUCCESS;
}
