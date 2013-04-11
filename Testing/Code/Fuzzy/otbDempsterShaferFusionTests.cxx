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

#include "otbMassOfBelief.h"
#include "otbJointMassOfBeliefFilter.h"
#include "otbConfusionMatrixToMassOfBelief.h"


// STRING LABELS
typedef std::string LabelPixelType;
typedef otb::MassOfBelief<LabelPixelType> MassOfBeliefFunctionType;
typedef otb::JointMassOfBeliefFilter<MassOfBeliefFunctionType> JointMassOfBeliefFilterType;

//Function to display MassOfBeliefFunctionType::LabelSetType instances with the "<<" operator
std::ostream& operator <<(std::ostream& os, const MassOfBeliefFunctionType::LabelSetType& l)
{
  MassOfBeliefFunctionType::PrintLabelSet(os, l);
  return os;
}




// OPTIMIZED RECURSIVE DS FUSION WITH CONFUSION MATRICES
int otbDempsterShaferFusionOptRecConfMatTest(int argc, char * argv[])
{
  typedef itk::VariableSizeMatrix<double> ConfusionMatrixType;

  // filter type
  typedef otb::ConfusionMatrixToMassOfBelief<ConfusionMatrixType>         ConfusionMatrixToMassOfBeliefType;
  typedef ConfusionMatrixToMassOfBeliefType::MapOfIndicesType             MapOfIndicesType;
  typedef ConfusionMatrixToMassOfBeliefType::MapOfClassesType             MapOfClassesType;
  typedef ConfusionMatrixToMassOfBeliefType::MassOfBeliefDefinitionMethod MassOfBeliefDefinitionMethod;
  typedef ConfusionMatrixToMassOfBeliefType::MassType                     MassType; //double by default
  typedef ConfusionMatrixToMassOfBeliefType::LabelMassMapType             SingleClassLabelMassMapType;

  typedef int                                                             ClassLabelType;
  typedef std::map<ClassLabelType, unsigned int>                          ClassifierHistogramType;
  typedef otb::MassOfBelief<ClassLabelType>                               MassOfBeliefClassLabelFunctionType;
  typedef MassOfBeliefClassLabelFunctionType::LabelSetType                LabelSetType;


  unsigned int nbClassifiers = 6;

  ClassLabelType undefinedValue = 0;



  // mapOfClasses[label] = index in the rows/columns of the confusion matrix
  MapOfIndicesType mapOfIndicesCl0, mapOfIndicesCl1, mapOfIndicesCl2, mapOfIndicesCl3, mapOfIndicesCl4, mapOfIndicesCl5;

  /* ***************************************************************************************** */
  /* ***************** Expected to be read from input confusion matrix files ***************** */
  /* ***************************************************************************************** */
  ConfusionMatrixType confMatCl0 = ConfusionMatrixType(4, 4);
  ConfusionMatrixType confMatCl1 = ConfusionMatrixType(3, 3);
  ConfusionMatrixType confMatCl2 = ConfusionMatrixType(4, 4);
  ConfusionMatrixType confMatCl3 = ConfusionMatrixType(5, 5);
  ConfusionMatrixType confMatCl4 = ConfusionMatrixType(4, 4);
  ConfusionMatrixType confMatCl5 = ConfusionMatrixType(2, 2);

  mapOfIndicesCl0[0] = 1, mapOfIndicesCl0[1] = 2, mapOfIndicesCl0[2] = 3, mapOfIndicesCl0[3] = 4;
  confMatCl0(0, 0) = 100, confMatCl0(0, 1) = 0, confMatCl0(0, 2) = 12, confMatCl0(0, 3) = 200;
  confMatCl0(1, 0) = 0, confMatCl0(1, 1) = 100, confMatCl0(1, 2) = 100, confMatCl0(1, 3) = 100;
  confMatCl0(2, 0) = 452, confMatCl0(2, 1) = 20, confMatCl0(2, 2) = 200, confMatCl0(2, 3) = 100;
  confMatCl0(3, 0) = 0, confMatCl0(3, 1) = 0, confMatCl0(3, 2) = 100, confMatCl0(3, 3) = 200;
;
  mapOfIndicesCl1[0] = 3, mapOfIndicesCl1[1] = 4, mapOfIndicesCl1[2] = 5;
  confMatCl1(0, 0) = 128, confMatCl1(0, 1) = 256, confMatCl1(0, 2) = 0;
  confMatCl1(1, 0) = 42, confMatCl1(1, 1) = 1500, confMatCl1(1, 2) = 1;
  confMatCl1(2, 0) = 0, confMatCl1(2, 1) = 15, confMatCl1(2, 2) = 1200;

  mapOfIndicesCl2[0] = 1, mapOfIndicesCl2[1] = 2, mapOfIndicesCl2[2] = 3, mapOfIndicesCl2[3] = 4;
  confMatCl2(0, 0) = 1000, confMatCl2(0, 1) = 0, confMatCl2(0, 2) = 51, confMatCl2(0, 3) = 22;
  confMatCl2(1, 0) = 0, confMatCl2(1, 1) = 2000, confMatCl2(1, 2) = 1, confMatCl2(1, 3) = 11;
  confMatCl2(2, 0) = 0, confMatCl2(2, 1) = 0, confMatCl2(2, 2) = 3000, confMatCl2(2, 3) = 154;
  confMatCl2(3, 0) = 14, confMatCl2(3, 1) = 125, confMatCl2(3, 2) = 0, confMatCl2(3, 3) = 4000;

  mapOfIndicesCl3[0] = 2, mapOfIndicesCl3[1] = 10, mapOfIndicesCl3[2] = 15, mapOfIndicesCl3[3] = 20, mapOfIndicesCl3[4] = 4;
  confMatCl3(0, 0) = 18500, confMatCl3(0, 1) = 15, confMatCl3(0, 2) = 20, confMatCl3(0, 3) = 126, confMatCl3(0, 4) = 40;
  confMatCl3(1, 0) = 0, confMatCl3(1, 1) = 15464, confMatCl3(1, 2) = 0, confMatCl3(1, 3) = 15, confMatCl3(1, 4) = 0;
  confMatCl3(2, 0) = 0, confMatCl3(2, 1) = 0, confMatCl3(2, 2) = 20000, confMatCl3(2, 3) = 0, confMatCl3(2, 4) = 32;
  confMatCl3(3, 0) = 23, confMatCl3(3, 1) = 0, confMatCl3(3, 2) = 14, confMatCl3(3, 3) = 2565, confMatCl3(3, 4) = 123;
  confMatCl3(4, 0) = 23, confMatCl3(4, 1) = 0, confMatCl3(4, 2) = 14, confMatCl3(4, 3) = 2565, confMatCl3(4, 4) = 87462;

  mapOfIndicesCl4[0] = 1, mapOfIndicesCl4[1] = 7, mapOfIndicesCl4[2] = 4, mapOfIndicesCl4[3] = 2;
  confMatCl4(0, 0) = 5054, confMatCl4(0, 1) = 2, confMatCl4(0, 2) = 0, confMatCl4(0, 3) = 20;
  confMatCl4(1, 0) = 0, confMatCl4(1, 1) = 1235, confMatCl4(1, 2) = 1, confMatCl4(1, 3) = 0;
  confMatCl4(2, 0) = 456, confMatCl4(2, 1) = 123, confMatCl4(2, 2) = 64984, confMatCl4(2, 3) = 0;
  confMatCl4(3, 0) = 0, confMatCl4(3, 1) = 0, confMatCl4(3, 2) = 45, confMatCl4(3, 3) = 3230;

  mapOfIndicesCl5[0] = 2, mapOfIndicesCl5[1] = 4;
  confMatCl5(0, 0) = 8458, confMatCl5(0, 1) = 45;
  confMatCl5(1, 0) = 564, confMatCl5(1, 1) = 32121;

  /* ***************************************************************************************** */
  /* ***************************************************************************************** */
  /* ***************************************************************************************** */

  // Manipulating the Confusion Matrices and Maps Of Indices for each classifier Clk
  std::vector<MapOfIndicesType> vectorOfMapOfIndices;
  vectorOfMapOfIndices.push_back(mapOfIndicesCl0), vectorOfMapOfIndices.push_back(mapOfIndicesCl1);
  vectorOfMapOfIndices.push_back(mapOfIndicesCl2), vectorOfMapOfIndices.push_back(mapOfIndicesCl3);
  vectorOfMapOfIndices.push_back(mapOfIndicesCl4), vectorOfMapOfIndices.push_back(mapOfIndicesCl5);

  std::vector<ConfusionMatrixType> vectorOfConfMatrices;
  vectorOfConfMatrices.push_back(confMatCl0), vectorOfConfMatrices.push_back(confMatCl1);
  vectorOfConfMatrices.push_back(confMatCl2), vectorOfConfMatrices.push_back(confMatCl3);
  vectorOfConfMatrices.push_back(confMatCl4), vectorOfConfMatrices.push_back(confMatCl5);

  /* ***************************************************************************************** */
  /* ***************************************************************************************** */


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

  confMatToMass->SetDefinitionMethod(massOfBeliefDefMethod);




  /* ***************************************************************************************** */
  /* *** Extraction of the K maps of Masses of Belief (one map of MOBs for each classifier) ** */
  /* ***************************************************************************************** */

  // Masses of the singleton sets {Ai} for each classifier.
  // These masses represent the degree of belief of each classifier that the output class of the pixel X
  // is {Ai} when the input pixel X actually belongs to the class {Ai}

  unsigned int nbClassesClk;
  MapOfIndicesType mapOfIndicesClk;
  ConfusionMatrixType confMatClk;
  MassType mobUniverseClk;
  SingleClassLabelMassMapType mapMOBClk;
  SingleClassLabelMassMapType::iterator itMapMOBClk;

  ClassifierHistogramType universe;
  ClassifierHistogramType::iterator itUniverse;
  std::vector<SingleClassLabelMassMapType> vectorMapMOBs;
  std::vector<MassType> vectorUniverseMOBs;
  for (unsigned int itClk = 0; itClk < nbClassifiers; ++itClk)
    {
    // Classifier Cl_k:
    mapOfIndicesClk = vectorOfMapOfIndices[itClk];
    confMatClk = vectorOfConfMatrices[itClk];
    nbClassesClk = mapOfIndicesClk.size();

    // mobUniverseClk is set to zero in order to assure the correct estimation of the Belief Functions of the
    // complementary sets bel({Ai_}) in the optimized DS combination
    mobUniverseClk = 0.;

    confMatToMass->SetMapOfIndices(mapOfIndicesClk);
    confMatToMass->SetConfusionMatrix(confMatClk);
    confMatToMass->Update();

    mapMOBClk.clear();
    mapMOBClk = confMatToMass->GetMapMassOfBelief();

    // Vector containing ALL the K std::maps of Mass of Belief
    vectorMapMOBs.push_back(mapMOBClk);

    // Vector containing the Mass of Belief of the universe for each classifier
    vectorUniverseMOBs.push_back(mobUniverseClk);

    std::cout << "confusion matrix[Cl_" << itClk << "] = " << std::endl;
    for (unsigned int itLabel = 0; itLabel < nbClassesClk; ++itLabel)
      {
      ClassLabelType classLabel = mapOfIndicesClk[itLabel];
      std::cout << "[" << classLabel << "] ";

      // If the current classLabel has already been added to the universe
      if (universe.count(classLabel) > 0)
        {
        universe[classLabel]++;
        }
      else
        {
        universe[classLabel] = 1;
        }
      }
    std::cout << std::endl << confMatClk << std::endl;
    std::cout << std::endl;

    for (itMapMOBClk = mapMOBClk.begin(); itMapMOBClk != mapMOBClk.end(); ++itMapMOBClk)
      {
      std::cout << "mapMOBCl_" << itClk << "[" << itMapMOBClk->first << "] = " << itMapMOBClk->second << std::endl;
      }

    std::cout << "*******************************************************************************" << std::endl;
    }

  // Number of classes in the universe
  unsigned int nbClasses = universe.size();

  std::cout << "universe:" << std::endl;
  for (itUniverse = universe.begin(); itUniverse != universe.end(); ++itUniverse)
    {
    std::cout << "Class Label " << itUniverse->first;
    std::cout << " present in " << itUniverse->second << " classifier(s)" << std::endl;
    }
  std::cout << std::endl;



  /* ***************************************************************************************** */
  /* ************************ RESULTS OF THE CLASSIFICATION OF PIXEL X *********************** */
  /* ***************************************************************************************** */

  // Values (as ClassLabelType) of the K CLASSIFICATION MAPS for the pixel X
  std::vector<ClassLabelType> classifiedPixelX;
  classifiedPixelX.push_back(2);
  classifiedPixelX.push_back(4);
  classifiedPixelX.push_back(4);
  classifiedPixelX.push_back(15);
  classifiedPixelX.push_back(7);
  classifiedPixelX.push_back(2);


  /* ***************************************************************************************** */
  /* *************************************** DS STEP #1 ************************************** */
  /* ***************************************************************************************** */

  ClassLabelType classLabelk;
  MassType mLabelSetClk, mLabelSetClk_, mUniverseClk;
  MassType mLabelSetClkprev, mLabelSetClkprev_, mUniverseClkprev;
  MassType KClk, mLabelSetClkNew, mLabelSetClkNew_, mUniverseClkNew;

  SingleClassLabelMassMapType mapJointMassesStepI, mapJointMassesStepI_, mapJointMassesUniverseStepI;

  // Extracting the masses m(Ai), m(Ai_) and m(OMEGA) for each of the K = 6 classifiers
  // and grouping them according to the {Ai} singletons
  // (ex: mg(A), mg(B), mg(C),..., with mg(Ai) the joint mass of the masses of classifiers with result Ai for pixel X)
  for (unsigned int itClk = 0; itClk < nbClassifiers; ++itClk)
    {
    classLabelk = classifiedPixelX[itClk];

    // Setting the masses of the three focal elements {Ai}, {Ai_} and OMEGA = {Ai U Ai_}
    // Extraction of mk({Ai}) = mLabelClk of the classifier k for the labelSetClk = {Ai}
    mUniverseClk = vectorUniverseMOBs[itClk];
    mLabelSetClk = vectorMapMOBs[itClk][classLabelk];
    mLabelSetClk_ = 1 - mLabelSetClk - mUniverseClk;

    std::cout << "classifiedPixelX[" << itClk << "] = " << classLabelk;
    std::cout << "; MassOfBelief_Cl_" << itClk << "[" << classLabelk << "] = " << mLabelSetClk;
    std::cout << std::endl;

    // The first time the label {Ai} is found in classifiedPixelX
    if (mapJointMassesStepI.count(classLabelk) == 0)
      {
      mLabelSetClkNew = mLabelSetClk;
      mLabelSetClkNew_ = mLabelSetClk_;
      mUniverseClkNew = mUniverseClk;
      }
    else
      {
      mLabelSetClkprev = mapJointMassesStepI[classLabelk];
      mLabelSetClkprev_ = mapJointMassesStepI_[classLabelk];
      mUniverseClkprev = mapJointMassesUniverseStepI[classLabelk];

      KClk = 1 / (1 - mLabelSetClkprev * mLabelSetClk_ - mLabelSetClkprev_ * mLabelSetClk);
      mLabelSetClkNew = KClk * (mLabelSetClkprev * (mLabelSetClk + mUniverseClk) + mUniverseClkprev * mLabelSetClk);
      mLabelSetClkNew_ = KClk * (mLabelSetClkprev_ * (mLabelSetClk_ + mUniverseClk) + mUniverseClkprev * mLabelSetClk_);
      mUniverseClkNew = KClk * mUniverseClkprev * mUniverseClk;
      }

    mapJointMassesStepI[classLabelk] = mLabelSetClkNew;
    mapJointMassesStepI_[classLabelk] = mLabelSetClkNew_;
    mapJointMassesUniverseStepI[classLabelk] = mUniverseClkNew;
    }

  std::cout << "*******************************************************************************" << std::endl;
  std::cout << std::endl;

  /* ***************************************************************************************** */
  /* *************************************** DS STEP #2 ************************************** */
  /* ***************************************************************************************** */

  // Calculation of the four A, B, C and K constants
  MassType A = 0, B = 1, C = 1, K;
  for (itMapMOBClk = mapJointMassesStepI.begin(); itMapMOBClk != mapJointMassesStepI.end(); ++itMapMOBClk)
    {
    classLabelk = itMapMOBClk->first;

    mLabelSetClk = mapJointMassesStepI[classLabelk];
    mLabelSetClk_ = mapJointMassesStepI_[classLabelk];
    mUniverseClk = mapJointMassesUniverseStepI[classLabelk];

    A = A + (mLabelSetClk / (1 - mLabelSetClk));
    B = B * (1 - mLabelSetClk);
    C = C * mLabelSetClk_;

    std::cout << "****************************************" << std::endl;
    std::cout << "mapJointMassesStepI[" << classLabelk << "] = " << mLabelSetClk << std::endl;
    std::cout << "mapJointMassesStepI_[" << classLabelk << "] = " << mLabelSetClk_ << std::endl;
    std::cout << "mapJointMassesUniverseStepI[" << classLabelk << "] = " << mUniverseClk << std::endl;
    }

  unsigned int nbClkGroupsStepI = mapJointMassesStepI.size();
  if (nbClkGroupsStepI == nbClasses)
    {
    K = 1 / ((1 + A) * B - C);
    }
  else
    {
    if (nbClkGroupsStepI < nbClasses)
      {
      K = 1 / ((1 + A) * B);
      }
    }

  std::cout << "****************************************************************************" << std::endl;
  std::cout << "****************************************************************************" << std::endl;
  std::cout << "nbClasses = " << nbClasses << std::endl;
  std::cout << "nbClassifiers = " << nbClassifiers << std::endl;
  std::cout << "nbClkGroupsStepI = " << nbClkGroupsStepI << std::endl;
  std::cout << "****************************************************************************" << std::endl;
  std::cout << "****************************************************************************" << std::endl;

  // Calculation of the Belief function of each singleton {Ai} and {Ai_}
  SingleClassLabelMassMapType mapBelStepII, mapBelStepII_;
  MassType belLabelSetClk, belLabelSetClk_, addBelLabelSetClk = 0.;
  for (itMapMOBClk = mapJointMassesStepI.begin(); itMapMOBClk != mapJointMassesStepI.end(); ++itMapMOBClk)
    {
    classLabelk = itMapMOBClk->first;

    // Joint Masses AFTER STEP I (one mass per singleton {Ai})
    mLabelSetClk = mapJointMassesStepI[classLabelk];
    mLabelSetClk_ = mapJointMassesStepI_[classLabelk];
    mUniverseClk = mapJointMassesUniverseStepI[classLabelk];

    // Bel(Ai)
    if ((nbClkGroupsStepI == nbClasses) || ((nbClkGroupsStepI == (nbClasses - 1)) && (K == nbClasses)))
      {
      belLabelSetClk = K * ((mLabelSetClk / (1 - mLabelSetClk)) * B + (mUniverseClk * C / mLabelSetClk_));
      }
    else
      {
      belLabelSetClk = K * (mLabelSetClk / (1 - mLabelSetClk)) * B;
      }

    // Bel(Ai_)
    if (nbClkGroupsStepI == nbClasses)
      {
      belLabelSetClk_ = K * ((A - ((mLabelSetClk - mLabelSetClk_) / (1 - mLabelSetClk))) * B - C);
      }
    else
      {
      if ((nbClkGroupsStepI < nbClasses) && (K <= nbClkGroupsStepI))
        {
        belLabelSetClk_ = K * (A - ((mLabelSetClk - mLabelSetClk_) / (1 - mLabelSetClk))) * B;
        }
      else
        {
        belLabelSetClk_ = K * (A - (mLabelSetClk / (1 - mLabelSetClk))) * B;
        }
      }

    mapBelStepII[classLabelk] = belLabelSetClk;
    mapBelStepII_[classLabelk] = belLabelSetClk_;
    addBelLabelSetClk += belLabelSetClk;
    }

  std::cout << "****************************************************************************" << std::endl;


  /* ***************************************************************************************** */
  /* ************************************ DECISION PROCESS *********************************** */
  /* ***************************************************************************************** */

  // The decision for the DS Fusion is made with the MAXIMAL Belief function:
  // {Ai} is chosen if Bel({Ai}) = MAX(Bel({Aj}))

  ClassLabelType fusedDSLabelSet;
  MassType fusedDSBelLabelSetClk = 0.; //Here the MAX of Bel(Ai)

  for (itUniverse = universe.begin(); itUniverse != universe.end(); ++itUniverse)
    {
    classLabelk = itUniverse->first;

    // In order to handle the other sets {Aj} of the universe which are NOT present in classifiedPixelX
    // ONLY correct if ALL the K classifiers have m(universe) = mUniverseClk = 0
    mapBelStepII_[classLabelk] += (1 - addBelLabelSetClk);

    if (itUniverse == universe.begin())
      {
      fusedDSLabelSet = classLabelk;
      fusedDSBelLabelSetClk = mapBelStepII[classLabelk];
      }
    else
      {
      if (mapBelStepII[classLabelk] >= fusedDSBelLabelSetClk)
        {
        fusedDSLabelSet = classLabelk;
        fusedDSBelLabelSetClk = mapBelStepII[classLabelk];
        }
      }

    // For labels {Ai} of the universe which are NOT present in classifiedPixelX,
    // the Bel({Ai_}) = SUM(m_stepII({Aj})) = SUM(Bel({Aj})) here; with {Aj} ALL labels present in classifiedPixelX
    if (mapBelStepII[classLabelk] == 0)
      {
      mapBelStepII_[classLabelk] = addBelLabelSetClk;
      }

    std::cout << "Bel(" << classLabelk << ") = " << mapBelStepII[classLabelk] << std::endl;
    std::cout << "Bel(NOT_" << classLabelk << ") = " << mapBelStepII_[classLabelk] << std::endl;
    std::cout << std::endl;
    }

  // If the DS VOTED LABEL is NOT unique, the result of the DS Fusion is undefinedValue
  for (itUniverse = universe.begin(); itUniverse != universe.end(); ++itUniverse)
    {
    classLabelk = itUniverse->first;
    if ((mapBelStepII[classLabelk] == fusedDSBelLabelSetClk) && (classLabelk != fusedDSLabelSet))
      {
      fusedDSLabelSet = undefinedValue;
      }
    }

  std::cout << "****************************************************************************" << std::endl;
  std::cout << "fusedDSLabelSet = " << fusedDSLabelSet << std::endl;
  std::cout << "fusedDSBelLabelSetClk = " << fusedDSBelLabelSetClk << std::endl;
  std::cout << std::endl;

  return EXIT_SUCCESS;
}

// ********************************************************************************************************
// ********************************************************************************************************
// ********************************************************************************************************




// OPTIMIZED RECURSIVE DS FUSION
int otbDempsterShaferFusionOptRecTest(int argc, char * argv[])
{
  //*****************************
  // For DEBUG PURPOSE ONLY
  unsigned int nbClassesMax = 6;
  //*****************************

  typedef MassOfBeliefFunctionType::MassType MassType; //double by default

  std::vector<LabelPixelType> vectorAllLabels;
  unsigned int nbArgs = static_cast<unsigned int> (argc);
  for (unsigned int it = 1; it < nbArgs; ++it)
    {
    vectorAllLabels.push_back(argv[it]);
    }

  unsigned int nbClasses = vectorAllLabels.size();

  MassOfBeliefFunctionType::LabelSetType universe, labelSet;
  typedef std::map<LabelPixelType, MassOfBeliefFunctionType::LabelSetType> LabelSetMapType;
  LabelSetMapType mapLabelSets;
  LabelSetMapType::iterator itMapLabelSets;
  for (unsigned int it = 0; it < vectorAllLabels.size(); ++it)
    {
    //The universe LabelSet is filled with ALL the possible labels of vectorAllLabels
    universe.insert(vectorAllLabels[it]);
    labelSet.clear();
    labelSet.insert(vectorAllLabels[it]);
    mapLabelSets[vectorAllLabels[it]] = labelSet;
    }

  std::cout << "universe = " << universe << std::endl;

  //**********************************************************************
  //************** RESULTS OF THE CLASSIFICATION OF PIXEL X **************
  //**********************************************************************

  // Values (as MassOfBeliefFunctionType::LabelSetType) of the K CLASSIFICATION MAPS for the pixel X
  typedef std::vector<MassOfBeliefFunctionType::LabelSetType> LabelSetOfVectorType;
  LabelSetOfVectorType classifiedPixelX;
  classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[1]]);
  classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[0]]);
  classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[1]]);
  classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[2]]);
  classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[0]]);
  classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[0]]);

  if ((nbArgs - 1) > nbClassesMax) //For tests with more than 4 labels in the universe TO TEST PROCESSING SPEED
    {
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[3]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[4]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[5]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[6]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[7]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[8]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[9]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[10]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[11]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[12]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[13]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[14]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[15]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[16]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[17]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[18]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[19]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[20]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[21]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[22]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[0]]);
    classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[2]]);

    // NOT used labels of the universe
    /*
     classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[23]]);
     classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[24]]);
     classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[25]]);
     */
    }

  for (unsigned int it = 0; it < classifiedPixelX.size(); ++it)
    {
    std::cout << "classifiedPixelX[" << it << "] = " << classifiedPixelX[it] << std::endl;
    }

  //**********************************************************************
  //**********************************************************************
  //**********************************************************************

  // Masses of the singleton sets {Ai} for each classifier. Expected to be automatically filled with the
  // CONFUSION MATRICES (ex: with the recognition rates of each class {Ai}).
  // These masses represent the degree of belief of each classifier that the output class of the pixel X
  // is {Ai} when the input pixel X actually belongs to the class {Ai}

  typedef MassOfBeliefFunctionType::MassMapType MassMapType;
  MassMapType::iterator itMapMasses;
  MassMapType mCl0, mCl1, mCl2, mCl3, mCl4, mCl5;

  mCl0[mapLabelSets[vectorAllLabels[0]]] = 0.9;
  mCl0[mapLabelSets[vectorAllLabels[1]]] = 0.7;
  mCl0[mapLabelSets[vectorAllLabels[2]]] = 0.8;
  //mCl0[mapLabelSets[vectorAllLabels[3]]] = 0.05;
  //mCl0[universe] = 0.2;

  mCl1[mapLabelSets[vectorAllLabels[0]]] = 0.8;
  mCl1[mapLabelSets[vectorAllLabels[1]]] = 0;
  mCl1[mapLabelSets[vectorAllLabels[2]]] = 0.5;
  //mCl1[mapLabelSets[vectorAllLabels[3]]] = 0.15;
  //mCl1[universe] = 0;

  mCl2[mapLabelSets[vectorAllLabels[0]]] = 0.5;
  mCl2[mapLabelSets[vectorAllLabels[1]]] = 0.65;
  mCl2[mapLabelSets[vectorAllLabels[2]]] = 0.3;
  //mCl2[mapLabelSets[vectorAllLabels[3]]] = 0.1;
  //mCl2[universe] = 0.1;

  mCl3[mapLabelSets[vectorAllLabels[0]]] = 0.5;
  mCl3[mapLabelSets[vectorAllLabels[1]]] = 0.25;
  mCl3[mapLabelSets[vectorAllLabels[2]]] = 0.6;
  //mCl3[mapLabelSets[vectorAllLabels[3]]] = 0.2;
  //mCl3[universe] = 0;

  mCl4[mapLabelSets[vectorAllLabels[0]]] = 0.75;
  mCl4[mapLabelSets[vectorAllLabels[1]]] = 0.3;
  mCl4[mapLabelSets[vectorAllLabels[2]]] = 0.6;
  //mCl4[mapLabelSets[vectorAllLabels[3]]] = 0.5;
  //mCl4[universe] = 0;

  mCl5[mapLabelSets[vectorAllLabels[0]]] = 0.9;
  mCl5[mapLabelSets[vectorAllLabels[1]]] = 0.5;
  mCl5[mapLabelSets[vectorAllLabels[2]]] = 0.3;
  //mCl5[mapLabelSets[vectorAllLabels[3]]] = 0.2;
  //mCl5[universe] = 0;


  // Vector containing the std::maps of masses of ALL the singleton sets for each classifier k = 0,...,27 here
  std::vector<MassMapType> mClVector;
  mClVector.push_back(mCl0);
  mClVector.push_back(mCl1);
  mClVector.push_back(mCl2);
  mClVector.push_back(mCl3);
  mClVector.push_back(mCl4);
  mClVector.push_back(mCl5);

  if ((nbArgs - 1) > nbClassesMax) //For tests with more than 4 labels in the universe TO TEST PROCESSING SPEED
    {
    //**********************************************************************
    // OTHER MASSES TO TEST PROCESSING SPEED
    MassMapType mCl6, mCl7, mCl8, mCl9, mCl10, mCl11, mCl12, mCl13, mCl14, mCl15, mCl16, mCl17, mCl18, mCl19, mCl20,
        mCl21, mCl22, mCl23, mCl24, mCl25, mCl26, mCl27, mCl28, mCl29, mCl30;

    // The other masses of each of the classifiers below (mCl6,...,mCl30) are assumed to be equal to zero,
    // that is why they are not initialized
    mCl6[mapLabelSets[vectorAllLabels[3]]] = 0.6;
    mCl7[mapLabelSets[vectorAllLabels[4]]] = 0.6;
    mCl8[mapLabelSets[vectorAllLabels[5]]] = 0.6;
    mCl9[mapLabelSets[vectorAllLabels[6]]] = 0.6;
    mCl10[mapLabelSets[vectorAllLabels[7]]] = 0.6;
    mCl11[mapLabelSets[vectorAllLabels[8]]] = 0.6;
    mCl12[mapLabelSets[vectorAllLabels[9]]] = 0.6;
    mCl13[mapLabelSets[vectorAllLabels[10]]] = 0.6;
    mCl14[mapLabelSets[vectorAllLabels[11]]] = 0.6;
    mCl15[mapLabelSets[vectorAllLabels[12]]] = 0.6;
    mCl16[mapLabelSets[vectorAllLabels[13]]] = 0.6;
    mCl17[mapLabelSets[vectorAllLabels[14]]] = 0.6;
    mCl18[mapLabelSets[vectorAllLabels[15]]] = 0.6;
    mCl19[mapLabelSets[vectorAllLabels[16]]] = 0.6;
    mCl20[mapLabelSets[vectorAllLabels[17]]] = 0.6;
    mCl21[mapLabelSets[vectorAllLabels[18]]] = 0.6;
    mCl22[mapLabelSets[vectorAllLabels[19]]] = 0.6;
    mCl23[mapLabelSets[vectorAllLabels[20]]] = 0.6;
    mCl24[mapLabelSets[vectorAllLabels[21]]] = 0.6;
    mCl25[mapLabelSets[vectorAllLabels[22]]] = 0.6;
    mCl26[mapLabelSets[vectorAllLabels[0]]] = 0.65;
    mCl27[mapLabelSets[vectorAllLabels[2]]] = 0.65;
    mCl28[mapLabelSets[vectorAllLabels[23]]] = 0.6; // NOT used here
    mCl29[mapLabelSets[vectorAllLabels[24]]] = 0.6; // NOT used here
    mCl30[mapLabelSets[vectorAllLabels[25]]] = 0.6; // NOT used here

    mClVector.push_back(mCl6);
    mClVector.push_back(mCl7);
    mClVector.push_back(mCl8);
    mClVector.push_back(mCl9);
    mClVector.push_back(mCl10);
    mClVector.push_back(mCl11);
    mClVector.push_back(mCl12);
    mClVector.push_back(mCl13);
    mClVector.push_back(mCl14);
    mClVector.push_back(mCl15);
    mClVector.push_back(mCl16);
    mClVector.push_back(mCl17);
    mClVector.push_back(mCl18);
    mClVector.push_back(mCl19);
    mClVector.push_back(mCl20);
    mClVector.push_back(mCl21);
    mClVector.push_back(mCl22);
    mClVector.push_back(mCl23);
    mClVector.push_back(mCl24);
    mClVector.push_back(mCl25);
    mClVector.push_back(mCl26);
    mClVector.push_back(mCl27);
    mClVector.push_back(mCl28); // NOT used here
    mClVector.push_back(mCl29); // NOT used here
    mClVector.push_back(mCl30); // NOT used here
    }
  //**********************************************************************

  //**********************************************************************
  //***************************** DS STEP #1 *****************************
  //**********************************************************************

  MassOfBeliefFunctionType::LabelSetType labelSetClk;
  MassType mLabelSetClk, mLabelSetClk_, mUniverseClk;
  MassType mLabelSetClkprev, mLabelSetClkprev_, mUniverseClkprev;
  MassType KClk, mLabelSetClkNew, mLabelSetClkNew_, mUniverseClkNew;

  MassMapType mapJointMassesStepI, mapJointMassesStepI_, mapJointMassesUniverseStepI;

  // Extracting the masses m(Ai), m(Ai_) and m(OMEGA) for each of the K = 6 classifiers
  // and grouping them according to the {Ai} singletons
  // (ex: mg(A), mg(B), mg(C),..., with mg(Ai) the joint mass of the masses of classifiers with result Ai for pixel X)
  for (unsigned int itk = 0; itk < classifiedPixelX.size(); ++itk)
    {
    labelSetClk = classifiedPixelX[itk]; // Label of the classified pixel X: {a}, {b} or {c} here

    // Setting the masses of the three focal elements {Ai}, {Ai_} and OMEGA = {Ai U Ai_}
    // Extraction of mk({Ai}) = mLabelClk of the classifier k for the labelSetClk = {Ai}
    mUniverseClk = mClVector[itk][universe];
    mLabelSetClk = mClVector[itk][labelSetClk];
    mLabelSetClk_ = 1 - mLabelSetClk - mUniverseClk;

    // The first time the label {Ai} is found in classifiedPixelX
    if (mapJointMassesStepI.count(labelSetClk) == 0)
      {
      mLabelSetClkNew = mLabelSetClk;
      mLabelSetClkNew_ = mLabelSetClk_;
      mUniverseClkNew = mUniverseClk;
      }
    else
      {
      mLabelSetClkprev = mapJointMassesStepI[labelSetClk];
      mLabelSetClkprev_ = mapJointMassesStepI_[labelSetClk];
      mUniverseClkprev = mapJointMassesUniverseStepI[labelSetClk];

      KClk = 1 / (1 - mLabelSetClkprev * mLabelSetClk_ - mLabelSetClkprev_ * mLabelSetClk);
      mLabelSetClkNew = KClk * (mLabelSetClkprev * (mLabelSetClk + mUniverseClk) + mUniverseClkprev * mLabelSetClk);
      mLabelSetClkNew_ = KClk * (mLabelSetClkprev_ * (mLabelSetClk_ + mUniverseClk) + mUniverseClkprev * mLabelSetClk_);
      mUniverseClkNew = KClk * mUniverseClkprev * mUniverseClk;
      }

    mapJointMassesStepI[labelSetClk] = mLabelSetClkNew;
    mapJointMassesStepI_[labelSetClk] = mLabelSetClkNew_;
    mapJointMassesUniverseStepI[labelSetClk] = mUniverseClkNew;
    }

  //**********************************************************************
  //***************************** DS STEP #2 *****************************
  //**********************************************************************

  // Calculation of the four A, B, C and K constants

  MassType A = 0, B = 1, C = 1, K;
  for (itMapMasses = mapJointMassesStepI.begin(); itMapMasses != mapJointMassesStepI.end(); ++itMapMasses)
    {
    labelSetClk = itMapMasses->first;

    mLabelSetClk = mapJointMassesStepI[labelSetClk];
    mLabelSetClk_ = mapJointMassesStepI_[labelSetClk];

    A = A + (mLabelSetClk / (1 - mLabelSetClk));
    B = B * (1 - mLabelSetClk);
    C = C * mLabelSetClk_;

    std::cout << "****************************************" << std::endl;
    std::cout << "mapJointMassesStepI[" << labelSetClk << "] = " << mLabelSetClk << std::endl;
    std::cout << "mapJointMassesStepI_[" << labelSetClk << "] = " << mLabelSetClk_ << std::endl;
    std::cout << "mapJointMassesUniverseStepI[" << labelSetClk << "] = " << mapJointMassesUniverseStepI[labelSetClk]
        << std::endl;
    }

  unsigned int nbClkGroupsStepI = mapJointMassesStepI.size();
  if (nbClkGroupsStepI == nbClasses)
    {
    K = 1 / ((1 + A) * B - C);
    }
  else
    {
    if (nbClkGroupsStepI < nbClasses)
      {
      K = 1 / ((1 + A) * B);
      }
    }

  std::cout << "****************************************************************************" << std::endl;
  std::cout << "****************************************************************************" << std::endl;
  std::cout << "nbClasses = " << nbClasses << std::endl;
  std::cout << "nbClassifiers = " << classifiedPixelX .size() << std::endl;
  std::cout << "nbClkGroupsStepI = " << nbClkGroupsStepI << std::endl;

  // Calculation of the Belief function of each singleton {Ai} and {Ai_}

  MassMapType mapBelStepII, mapBelStepII_;
  MassType belLabelSetClk, belLabelSetClk_, addBelLabelSetClk = 0;
  for (itMapMasses = mapJointMassesStepI.begin(); itMapMasses != mapJointMassesStepI.end(); ++itMapMasses)
    {
    labelSetClk = itMapMasses->first;

    // Joint Masses AFTER STEP I (one mass per singleton {Ai})
    mLabelSetClk = mapJointMassesStepI[labelSetClk];
    mLabelSetClk_ = mapJointMassesStepI_[labelSetClk];
    mUniverseClk = mapJointMassesUniverseStepI[labelSetClk];

    // Bel(Ai)
    if ((nbClkGroupsStepI == nbClasses) || ((nbClkGroupsStepI == (nbClasses - 1)) && (K == nbClasses)))
      {
      belLabelSetClk = K * ((mLabelSetClk / (1 - mLabelSetClk)) * B + (mUniverseClk * C / mLabelSetClk_));
      }
    else
      {
      belLabelSetClk = K * (mLabelSetClk / (1 - mLabelSetClk)) * B;
      }

    // Bel(Ai_)
    if (nbClkGroupsStepI == nbClasses)
      {
      belLabelSetClk_ = K * ((A - ((mLabelSetClk - mLabelSetClk_) / (1 - mLabelSetClk))) * B - C);
      }
    else
      {
      if ((nbClkGroupsStepI < nbClasses) && (K <= nbClkGroupsStepI))
        {
        belLabelSetClk_ = K * (A - ((mLabelSetClk - mLabelSetClk_) / (1 - mLabelSetClk))) * B;
        }
      else
        {
        belLabelSetClk_ = K * (A - (mLabelSetClk / (1 - mLabelSetClk))) * B;
        }
      }

    mapBelStepII[labelSetClk] = belLabelSetClk;
    mapBelStepII_[labelSetClk] = belLabelSetClk_;
    addBelLabelSetClk = addBelLabelSetClk + belLabelSetClk;
    }

  std::cout << "****************************************************************************" << std::endl;

  // The decision for the DS Fusion is made with the MAXIMAL Belief function:
  // {Ai} is chosen if Bel({Ai}) = MAX(Bel({Aj}))

  MassOfBeliefFunctionType::LabelSetType fusedDSLabelSet;
  MassType fusedDSBelLabelSetClk = 0; //Here the MAX of Bel(Ai)
  for (itMapLabelSets = mapLabelSets.begin(); itMapLabelSets != mapLabelSets.end(); ++itMapLabelSets)
    {
    labelSetClk = itMapLabelSets->second;

    // In order to handle the other sets {Aj} of the universe which are NOT present in classifiedPixelX
    // ONLY correct if ALL the K classifiers have m(universe) = mUniverseClk = 0
    mapBelStepII_[labelSetClk] = mapBelStepII_[labelSetClk] + (1 - addBelLabelSetClk);

    if (itMapLabelSets == mapLabelSets.begin())
      {
      fusedDSLabelSet = labelSetClk;
      fusedDSBelLabelSetClk = mapBelStepII[labelSetClk];
      }
    else
      {
      if (mapBelStepII[labelSetClk] >= fusedDSBelLabelSetClk)
        {
        fusedDSLabelSet = labelSetClk;
        fusedDSBelLabelSetClk = mapBelStepII[labelSetClk];
        }
      }

    // For labels {Ai} of the universe which are NOT present in classifiedPixelX,
    // the Bel({Ai_}) = SUM(m_stepII({Aj})) = SUM(Bel({Aj})) here; with {Aj} ALL labels present in classifiedPixelX
    if (mapBelStepII[labelSetClk] == 0)
      {
      mapBelStepII_[labelSetClk] = addBelLabelSetClk;
      }

    std::cout << "Bel(" << labelSetClk << ") = " << mapBelStepII[labelSetClk] << std::endl;
    std::cout << "Bel(NOT_" << labelSetClk << ") = " << mapBelStepII_[labelSetClk] << std::endl;
    std::cout << std::endl;
    }

  // If the DS VOTED LABEL is NOT unique, the result of the DS Fusion is the UNION of all the candidates {Ai}
  // with Bel({Ai}) = MAX(Bel({Aj}))
  for (itMapLabelSets = mapLabelSets.begin(); itMapLabelSets != mapLabelSets.end(); ++itMapLabelSets)
    {
    labelSetClk = itMapLabelSets->second;
    if ((mapBelStepII[labelSetClk] == fusedDSBelLabelSetClk) && (labelSetClk != fusedDSLabelSet))
      {
      fusedDSLabelSet.insert(itMapLabelSets->first);
      }
    }

  std::cout << "****************************************************************************" << std::endl;
  std::cout << "fusedDSLabelSet = " << fusedDSLabelSet << std::endl;
  std::cout << "fusedDSBelLabelSetClk = " << fusedDSBelLabelSetClk << std::endl;
  std::cout << std::endl;

  return EXIT_SUCCESS;
}

// ********************************************************************************************************
// ********************************************************************************************************
// ********************************************************************************************************

// OPTIMIZED DS FUSION
int otbDempsterShaferFusionOptTest(int argc, char * argv[])
{
  typedef MassOfBeliefFunctionType::MassType MassType; //double by default

  std::vector<LabelPixelType> vectorAllLabels;
  unsigned int nbArgs = static_cast<unsigned int> (argc);
  for (unsigned int it = 1; it < nbArgs; ++it)
    {
    vectorAllLabels.push_back(argv[it]);
    }

  unsigned int nbClasses = vectorAllLabels.size();

  MassOfBeliefFunctionType::LabelSetType universe, labelSet;
  typedef std::map<LabelPixelType, MassOfBeliefFunctionType::LabelSetType> LabelSetMapType;
  LabelSetMapType mapLabelSets;
  LabelSetMapType::iterator itMapLabelSets;
  for (unsigned int it = 0; it < vectorAllLabels.size(); ++it)
    {
    //The universe LabelSet is filled with ALL the possible labels of vectorAllLabels
    universe.insert(vectorAllLabels[it]);
    labelSet.clear();
    labelSet.insert(vectorAllLabels[it]);
    mapLabelSets[vectorAllLabels[it]] = labelSet;
    }

  std::cout << "universe = " << universe << std::endl;

  //**********************************************************************
  //************** RESULTS OF THE CLASSIFICATION OF PIXEL X **************
  //**********************************************************************

  // Values (as MassOfBeliefFunctionType::LabelSetType) of the K CLASSIFICATION MAPS for the pixel X
  typedef std::vector<MassOfBeliefFunctionType::LabelSetType> LabelSetOfVectorType;
  LabelSetOfVectorType classifiedPixelX;
  classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[1]]);
  classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[0]]);
  classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[1]]);
  classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[2]]);
  classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[0]]);
  classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[0]]);

  for (unsigned int it = 0; it < classifiedPixelX.size(); ++it)
    {
    std::cout << "classifiedPixelX[" << it << "] = " << classifiedPixelX[it] << std::endl;
    }

  //**********************************************************************
  //**********************************************************************
  //**********************************************************************

  // Masses of the singleton sets {Ai} for each classifier. Expected to be automatically filled with the
  // CONFUSION MATRICES (ex: with the recognition rates of each class {Ai}).
  // These masses represent the degree of belief of each classifier that the output class of the pixel X
  // is {Ai} when the input pixel X actually belongs to the class {Ai}

  typedef MassOfBeliefFunctionType::MassMapType MassMapType;
  MassMapType::iterator itMapMasses;
  MassMapType mCl0, mCl1, mCl2, mCl3, mCl4, mCl5;

  mCl0[mapLabelSets[vectorAllLabels[0]]] = 0.9;
  mCl0[mapLabelSets[vectorAllLabels[1]]] = 0.7;
  mCl0[mapLabelSets[vectorAllLabels[2]]] = 0.8;
  //mCl0[mapLabelSets[vectorAllLabels[3]]] = 0.05;
  //mCl0[universe] = 0.2;

  mCl1[mapLabelSets[vectorAllLabels[0]]] = 0.8;
  mCl1[mapLabelSets[vectorAllLabels[1]]] = 0;
  mCl1[mapLabelSets[vectorAllLabels[2]]] = 0.5;
  //mCl1[mapLabelSets[vectorAllLabels[3]]] = 0.15;
  //mCl1[universe] = 0;

  mCl2[mapLabelSets[vectorAllLabels[0]]] = 0.5;
  mCl2[mapLabelSets[vectorAllLabels[1]]] = 0.65;
  mCl2[mapLabelSets[vectorAllLabels[2]]] = 0.3;
  //mCl2[mapLabelSets[vectorAllLabels[3]]] = 0.1;
  //mCl2[universe] = 0.1;

  mCl3[mapLabelSets[vectorAllLabels[0]]] = 0.5;
  mCl3[mapLabelSets[vectorAllLabels[1]]] = 0.25;
  mCl3[mapLabelSets[vectorAllLabels[2]]] = 0.6;
  //mCl3[mapLabelSets[vectorAllLabels[3]]] = 0.2;
  //mCl3[universe] = 0;

  mCl4[mapLabelSets[vectorAllLabels[0]]] = 0.75;
  mCl4[mapLabelSets[vectorAllLabels[1]]] = 0.3;
  mCl4[mapLabelSets[vectorAllLabels[2]]] = 0.6;
  //mCl4[mapLabelSets[vectorAllLabels[3]]] = 0.5;
  //mCl4[universe] = 0;

  mCl5[mapLabelSets[vectorAllLabels[0]]] = 0.9;
  mCl5[mapLabelSets[vectorAllLabels[1]]] = 0.5;
  mCl5[mapLabelSets[vectorAllLabels[2]]] = 0.3;
  //mCl5[mapLabelSets[vectorAllLabels[3]]] = 0.2;
  //mCl5[universe] = 0;


  // Vector containing the std::maps of masses of ALL the singleton sets for each classifier k = 0,...,5 here
  std::vector<MassMapType> mClVector;
  mClVector.push_back(mCl0);
  mClVector.push_back(mCl1);
  mClVector.push_back(mCl2);
  mClVector.push_back(mCl3);
  mClVector.push_back(mCl4);
  mClVector.push_back(mCl5);

  //**********************************************************************
  //***************************** DS STEP #1 *****************************
  //**********************************************************************

  MassOfBeliefFunctionType::LabelSetType labelSetClk, labelSetClk_;
  MassType mLabelSetClk, mLabelSetClk_, mUniverseClk;
  MassOfBeliefFunctionType::Pointer mOBFClk;
  JointMassOfBeliefFilterType::Pointer jointMassClkFilter;

  typedef std::map<MassOfBeliefFunctionType::LabelSetType, JointMassOfBeliefFilterType::Pointer>
      mapJointMassOfBeliefFilterType;
  mapJointMassOfBeliefFilterType mapJointMassFilters;
  mapJointMassOfBeliefFilterType::iterator itMapJMOBFilters;

  // Extracting the masses m(Ai), m(Ai_) and m(OMEGA) for each of the K = 6 classifiers
  // and grouping them according to the {Ai} singletons
  // (ex: mg(A), mg(B), mg(C),..., with mg(Ai) the joint mass of the masses of classifiers with result Ai for pixel X)
  for (unsigned int itk = 0; itk < classifiedPixelX.size(); ++itk)
    {
    // Allocation of a new MassOfBeliefFunctionType::Pointer object pointing at a new memory address
    mOBFClk = MassOfBeliefFunctionType::New();

    labelSetClk = classifiedPixelX[itk]; // Label of the classified pixel X: {a}, {b} or {c} here
    // Defining the complementary singleton set {Ai_}
    labelSetClk_ = universe;
    labelSetClk_.erase(*labelSetClk.begin());

    // Setting the masses of the three focal elements {Ai}, {Ai_} and OMEGA = {Ai U Ai_}
    // Extraction of mk({Ai}) = mLabelClk of the classifier k for the labelSetClk = {Ai}
    mUniverseClk = mClVector[itk][universe];
    mLabelSetClk = mClVector[itk][labelSetClk];
    mLabelSetClk_ = 1 - mLabelSetClk - mUniverseClk;

    // Defining the MassOfBeliefFunctionType::Pointer object mOBFClk for the classifier k
    mOBFClk->InitializePowerSetMasses(universe);
    mOBFClk->SetMass(labelSetClk, mLabelSetClk);
    mOBFClk->SetMass(labelSetClk_, mLabelSetClk_);
    mOBFClk->EstimateUncertainty();

    if (mapJointMassFilters.count(labelSetClk) == 0)
      {
      // Allocation of a new JointMassOfBeliefFilterType::Pointer object pointing at a new memory address
      jointMassClkFilter = JointMassOfBeliefFilterType::New();
      mapJointMassFilters[labelSetClk] = jointMassClkFilter;
      }

    mapJointMassFilters[labelSetClk]->PushBackInput(mOBFClk);
    }

  typedef std::map<MassOfBeliefFunctionType::LabelSetType, MassOfBeliefFunctionType::Pointer>
      mapMassesOfBeliefFunctionType;
  mapMassesOfBeliefFunctionType mapJMOBFStepI;
  mapMassesOfBeliefFunctionType::iterator itMapJMOBFStepI;
  for (itMapJMOBFilters = mapJointMassFilters.begin(); itMapJMOBFilters != mapJointMassFilters.end(); ++itMapJMOBFilters)
    {
    // JointMassOfBeliefFilterType filters are executed, and the resulting joint masses are inserted in mapJMOBFStepI
    itMapJMOBFilters->second->Update();
    mapJMOBFStepI[itMapJMOBFilters->first] = itMapJMOBFilters->second->GetOutput();

    std::cout << "****************************************************************************" << std::endl;
    std::cout << "jointMassClkFilterStepI[" << itMapJMOBFilters->first << "] = " << itMapJMOBFilters->second
        << std::endl;
    }

  std::cout << "****************************************************************************" << std::endl;
  std::cout << "****************************************************************************" << std::endl;

  //**********************************************************************
  //***************************** DS STEP #2 *****************************
  //**********************************************************************

  // Calculation of the four A, B, C and K constants

  MassType A = 0, B = 1, C = 1, K;
  mapMassesOfBeliefFunctionType::iterator itMapJMOBF;
  for (itMapJMOBF = mapJMOBFStepI.begin(); itMapJMOBF != mapJMOBFStepI.end(); ++itMapJMOBF)
    {
    labelSetClk = itMapJMOBF->first;
    labelSetClk_ = universe;
    labelSetClk_.erase(*labelSetClk.begin());

    mLabelSetClk = itMapJMOBF->second->GetMass(labelSetClk);
    mLabelSetClk_ = itMapJMOBF->second->GetMass(labelSetClk_);

    A = A + (mLabelSetClk / (1 - mLabelSetClk));
    B = B * (1 - mLabelSetClk);
    C = C * mLabelSetClk_;

    std::cout << "****************************************************************************" << std::endl;
    std::cout << "jointMassOfBeliefStepI[" << itMapJMOBF->first << "] = " << itMapJMOBF->second << std::endl;
    }

  unsigned int nbClkGroupsStepI = mapJMOBFStepI.size();
  if (nbClkGroupsStepI == nbClasses)
    {
    K = 1 / ((1 + A) * B - C);
    }
  else
    {
    if (nbClkGroupsStepI < nbClasses)
      {
      K = 1 / ((1 + A) * B);
      }
    }

  // Calculation of the Belief function of each singleton {Ai} and {Ai_}

  MassMapType mapBelStepII;
  MassMapType::iterator itMapBelStepII;
  MassType belLabelSetClk, belLabelSetClk_, addBelLabelSetClk = 0;
  for (itMapJMOBF = mapJMOBFStepI.begin(); itMapJMOBF != mapJMOBFStepI.end(); ++itMapJMOBF)
    {
    labelSetClk = itMapJMOBF->first;
    labelSetClk_ = universe;
    labelSetClk_.erase(*labelSetClk.begin());

    mUniverseClk = itMapJMOBF->second->GetMass(universe);
    mLabelSetClk = itMapJMOBF->second->GetMass(labelSetClk);
    mLabelSetClk_ = itMapJMOBF->second->GetMass(labelSetClk_);

    // Bel(Ai)
    if ((nbClkGroupsStepI == nbClasses) || ((nbClkGroupsStepI == (nbClasses - 1)) && (K == nbClasses)))
      {
      belLabelSetClk = K * ((mLabelSetClk / (1 - mLabelSetClk)) * B + (mUniverseClk * C / mLabelSetClk_));
      }
    else
      {
      belLabelSetClk = K * (mLabelSetClk / (1 - mLabelSetClk)) * B;
      }

    // Bel(Ai_)
    if (nbClkGroupsStepI == nbClasses)
      {
      belLabelSetClk_ = K * ((A - ((mLabelSetClk - mLabelSetClk_) / (1 - mLabelSetClk))) * B - C);
      }
    else
      {
      if ((nbClkGroupsStepI < nbClasses) && (K <= nbClkGroupsStepI))
        {
        belLabelSetClk_ = K * (A - ((mLabelSetClk - mLabelSetClk_) / (1 - mLabelSetClk))) * B;
        }
      else
        {
        belLabelSetClk_ = K * (A - (mLabelSetClk / (1 - mLabelSetClk))) * B;
        }
      }

    mapBelStepII[labelSetClk] = belLabelSetClk;
    mapBelStepII[labelSetClk_] = belLabelSetClk_;
    addBelLabelSetClk = addBelLabelSetClk + belLabelSetClk;
    }

  std::cout << "****************************************************************************" << std::endl;
  std::cout << "****************************************************************************" << std::endl;

  // The decision for the DS Fusion is made with the MAXIMAL Belief function:
  // {Ai} is chosen if Bel({Ai}) = MAX(Bel({Aj}))

  MassOfBeliefFunctionType::LabelSetType fusedDSLabelSet;
  MassType fusedDSBelLabelSetClk = 0; //Here the MAX of Bel(Ai)
  for (itMapLabelSets = mapLabelSets.begin(); itMapLabelSets != mapLabelSets.end(); ++itMapLabelSets)
    {
    labelSetClk = itMapLabelSets->second;
    labelSetClk_ = universe;
    labelSetClk_.erase(*labelSetClk.begin());

    // In order to handle the other sets {Aj} of the universe which are NOT present in classifiedPixelX
    // ONLY correct if ALL the K classifiers have m(universe) = mUniverseClk = 0
    mapBelStepII[labelSetClk_] = mapBelStepII[labelSetClk_] + (1 - addBelLabelSetClk);

    if (itMapLabelSets == mapLabelSets.begin())
      {
      fusedDSLabelSet = labelSetClk;
      fusedDSBelLabelSetClk = mapBelStepII[labelSetClk];
      }
    else
      {
      if (mapBelStepII[labelSetClk] >= fusedDSBelLabelSetClk)
        {
        fusedDSLabelSet = labelSetClk;
        fusedDSBelLabelSetClk = mapBelStepII[labelSetClk];
        }
      }

    // For labels {Ai} of the universe which are NOT present in classifiedPixelX,
    // the Bel({Ai_}) = SUM(m_stepII({Aj})) = SUM(Bel({Aj})) here; with {Aj} ALL labels present in classifiedPixelX
    if (mapBelStepII[labelSetClk] == 0)
      {
      mapBelStepII[labelSetClk_] = addBelLabelSetClk;
      }

    std::cout << "Bel(" << labelSetClk << ") = " << mapBelStepII[labelSetClk] << std::endl;
    std::cout << "Bel(NOT_" << labelSetClk << ") = Bel(" << labelSetClk_ << ") = " << mapBelStepII[labelSetClk_]
        << std::endl;
    std::cout << std::endl;
    }

  // If the DS VOTED LABEL is NOT unique, the result of the DS Fusion is the UNION of all the candidates {Ai}
  // with Bel({Ai}) = MAX(Bel({Aj}))
  for (itMapLabelSets = mapLabelSets.begin(); itMapLabelSets != mapLabelSets.end(); ++itMapLabelSets)
    {
    labelSetClk = itMapLabelSets->second;
    if ((mapBelStepII[labelSetClk] == fusedDSBelLabelSetClk) && (labelSetClk != fusedDSLabelSet))
      {
      fusedDSLabelSet.insert(itMapLabelSets->first);
      }
    }

  std::cout << "****************************************************************************" << std::endl;
  std::cout << "fusedDSLabelSet = " << fusedDSLabelSet << std::endl;
  std::cout << "fusedDSBelLabelSetClk = " << fusedDSBelLabelSetClk << std::endl;
  std::cout << std::endl;

  return EXIT_SUCCESS;
}

// ********************************************************************************************************
// ********************************************************************************************************
// ********************************************************************************************************
// NOT OPTIMIZED DS FUSION
int otbDempsterShaferFusionTest(int argc, char * argv[])
{
  typedef MassOfBeliefFunctionType::MassType MassType; //double by default

  std::vector<LabelPixelType> vectorAllLabels;
  unsigned int nbArgs = static_cast<unsigned int> (argc);
  for (unsigned int it = 1; it < nbArgs; ++it)
    {
    vectorAllLabels.push_back(argv[it]);
    }

  unsigned int nbClasses = vectorAllLabels.size();

  MassOfBeliefFunctionType::LabelSetType universe, labelSet;
  typedef std::map<LabelPixelType, MassOfBeliefFunctionType::LabelSetType> LabelSetMapType;
  LabelSetMapType mapLabelSets;
  LabelSetMapType::iterator itMapLabelSets;
  for (unsigned int it = 0; it < vectorAllLabels.size(); ++it)
    {
    //The universe LabelSet is filled with ALL the possible labels of vectorAllLabels
    universe.insert(vectorAllLabels[it]);
    labelSet.clear();
    labelSet.insert(vectorAllLabels[it]);
    mapLabelSets[vectorAllLabels[it]] = labelSet;
    }

  std::cout << "universe = " << universe << std::endl;

  //**********************************************************************
  //************** RESULTS OF THE CLASSIFICATION OF PIXEL X **************
  //**********************************************************************

  // Values (as MassOfBeliefFunctionType::LabelSetType) of the K CLASSIFICATION MAPS for the pixel X
  typedef std::vector<MassOfBeliefFunctionType::LabelSetType> LabelSetOfVectorType;
  LabelSetOfVectorType classifiedPixelX;
  classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[1]]);
  classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[0]]);
  classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[1]]);
  classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[2]]);
  classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[0]]);
  classifiedPixelX.push_back(mapLabelSets[vectorAllLabels[0]]);

  for (unsigned int it = 0; it < classifiedPixelX.size(); ++it)
    {
    std::cout << "classifiedPixelX[" << it << "] = " << classifiedPixelX[it] << std::endl;
    }

  //**********************************************************************
  //**********************************************************************
  //**********************************************************************

  // Masses of the singleton sets {Ai} for each classifier. Expected to be automatically filled with the
  // CONFUSION MATRICES (ex: with the recognition rates of each class {Ai}).
  // These masses represent the degree of belief of each classifier that the output class of the pixel X
  // is {Ai} when the input pixel X actually belongs to the class {Ai}

  typedef MassOfBeliefFunctionType::MassMapType MassMapType;
  MassMapType::iterator itMapMasses;
  MassMapType mCl0, mCl1, mCl2, mCl3, mCl4, mCl5;

  mCl0[mapLabelSets[vectorAllLabels[0]]] = 0.9;
  mCl0[mapLabelSets[vectorAllLabels[1]]] = 0.7;
  mCl0[mapLabelSets[vectorAllLabels[2]]] = 0.8;
  //mCl0[mapLabelSets[vectorAllLabels[3]]] = 0.05;
  //mCl0[universe] = 0.2;

  mCl1[mapLabelSets[vectorAllLabels[0]]] = 0.8;
  mCl1[mapLabelSets[vectorAllLabels[1]]] = 0;
  mCl1[mapLabelSets[vectorAllLabels[2]]] = 0.5;
  //mCl1[mapLabelSets[vectorAllLabels[3]]] = 0.15;
  //mCl1[universe] = 0;

  mCl2[mapLabelSets[vectorAllLabels[0]]] = 0.5;
  mCl2[mapLabelSets[vectorAllLabels[1]]] = 0.65;
  mCl2[mapLabelSets[vectorAllLabels[2]]] = 0.3;
  //mCl2[mapLabelSets[vectorAllLabels[3]]] = 0.1;
  //mCl2[universe] = 0.1;

  mCl3[mapLabelSets[vectorAllLabels[0]]] = 0.5;
  mCl3[mapLabelSets[vectorAllLabels[1]]] = 0.25;
  mCl3[mapLabelSets[vectorAllLabels[2]]] = 0.6;
  //mCl3[mapLabelSets[vectorAllLabels[3]]] = 0.2;
  //mCl3[universe] = 0;

  mCl4[mapLabelSets[vectorAllLabels[0]]] = 0.75;
  mCl4[mapLabelSets[vectorAllLabels[1]]] = 0.3;
  mCl4[mapLabelSets[vectorAllLabels[2]]] = 0.6;
  //mCl4[mapLabelSets[vectorAllLabels[3]]] = 0.5;
  //mCl4[universe] = 0;

  mCl5[mapLabelSets[vectorAllLabels[0]]] = 0.9;
  mCl5[mapLabelSets[vectorAllLabels[1]]] = 0.5;
  mCl5[mapLabelSets[vectorAllLabels[2]]] = 0.3;
  //mCl5[mapLabelSets[vectorAllLabels[3]]] = 0.2;
  //mCl5[universe] = 0;


  // Vector containing the std::maps of masses of ALL the singleton sets for each classifier k = 0,...,5 here
  std::vector<MassMapType> mClVector;
  mClVector.push_back(mCl0);
  mClVector.push_back(mCl1);
  mClVector.push_back(mCl2);
  mClVector.push_back(mCl3);
  mClVector.push_back(mCl4);
  mClVector.push_back(mCl5);

  //**********************************************************************
  //***************************** DS FUSION ******************************
  //**********************************************************************

  MassOfBeliefFunctionType::LabelSetType labelSetClk, labelSetClk_;
  MassType mLabelSetClk, mLabelSetClk_, mUniverseClk;
  MassOfBeliefFunctionType::Pointer mOBFClk;
  JointMassOfBeliefFilterType::Pointer jointMassClkFilter;
  jointMassClkFilter = JointMassOfBeliefFilterType::New();

  for (unsigned int itk = 0; itk < classifiedPixelX.size(); ++itk)
    {
    // Allocation of a new MassOfBeliefFunctionType::Pointer object pointing at a new memory address
    mOBFClk = MassOfBeliefFunctionType::New();

    labelSetClk = classifiedPixelX[itk]; // Label of the classified pixel X: {a}, {b} or {c} here
    // Defining the complementary singleton set {Ai_}
    labelSetClk_ = universe;
    labelSetClk_.erase(*labelSetClk.begin());

    // Setting the masses of the three focal elements {Ai}, {Ai_} and OMEGA = {Ai U Ai_}
    // Extraction of mk({Ai}) = mLabelClk of the classifier k for the labelSetClk = {Ai}
    mUniverseClk = mClVector[itk][universe];
    mLabelSetClk = mClVector[itk][labelSetClk];
    mLabelSetClk_ = 1 - mLabelSetClk - mUniverseClk;

    // Defining the MassOfBeliefFunctionType::Pointer object mOBFClk for the classifier k
    mOBFClk->InitializePowerSetMasses(universe);
    mOBFClk->SetMass(labelSetClk, mLabelSetClk);
    mOBFClk->SetMass(labelSetClk_, mLabelSetClk_);
    mOBFClk->EstimateUncertainty();

    jointMassClkFilter->PushBackInput(mOBFClk);
    }

  jointMassClkFilter->Update();
  MassOfBeliefFunctionType::Pointer jointMass = jointMassClkFilter->GetOutput();

  std::cout << "****************************************************************************" << std::endl;
  std::cout << "jointMassClkFilter = " << jointMassClkFilter << std::endl;
  std::cout << "****************************************************************************" << std::endl;
  std::cout << std::endl;
  std::cout << "****************************************************************************" << std::endl;
  std::cout << "jointMassOfBelief = " << jointMass << std::endl;
  std::cout << "****************************************************************************" << std::endl;

  // The decision for the DS Fusion is made with the MAXIMAL Belief function:
  // {Ai} is chosen if Bel({Ai}) = MAX(Bel({Aj}))

  MassOfBeliefFunctionType::LabelSetType fusedDSLabelSet;
  MassType fusedDSBelLabelSetClk = 0; //Here the MAX of Bel(Ai)
  for (itMapLabelSets = mapLabelSets.begin(); itMapLabelSets != mapLabelSets.end(); ++itMapLabelSets)
    {
    labelSetClk = itMapLabelSets->second;
    labelSetClk_ = universe;
    labelSetClk_.erase(*labelSetClk.begin());

    if (itMapLabelSets == mapLabelSets.begin())
      {
      fusedDSLabelSet = labelSetClk;
      fusedDSBelLabelSetClk = jointMass->GetBelief(labelSetClk);
      }
    else
      {
      if (jointMass->GetBelief(labelSetClk) >= fusedDSBelLabelSetClk)
        {
        fusedDSLabelSet = labelSetClk;
        fusedDSBelLabelSetClk = jointMass->GetBelief(labelSetClk);
        }
      }

    std::cout << "Bel(" << labelSetClk << ") = " << jointMass->GetBelief(labelSetClk) << std::endl;
    std::cout << "Bel(NOT_" << labelSetClk << ") = Bel(" << labelSetClk_ << ") = "
        << jointMass->GetBelief(labelSetClk_) << std::endl;
    std::cout << std::endl;
    }

  //If the DS VOTED LABEL is NOT unique
  for (itMapLabelSets = mapLabelSets.begin(); itMapLabelSets != mapLabelSets.end(); ++itMapLabelSets)
    {
    labelSetClk = itMapLabelSets->second;
    if ((jointMass->GetBelief(labelSetClk) == fusedDSBelLabelSetClk) && (labelSetClk != fusedDSLabelSet))
      {
      fusedDSLabelSet.insert(itMapLabelSets->first);
      }
    }

  std::cout << "****************************************************************************" << std::endl;
  std::cout << "fusedDSLabelSet = " << fusedDSLabelSet << std::endl;
  std::cout << "fusedDSBelLabelSetClk = " << fusedDSBelLabelSetClk << std::endl;
  std::cout << std::endl;

  return EXIT_SUCCESS;
}
