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

typedef std::string LabelPixelType;
typedef otb::MassOfBelief<LabelPixelType> MassOfBeliefFunctionType;
typedef otb::JointMassOfBeliefFilter<MassOfBeliefFunctionType> JointMassOfBeliefFilterType;

//Function to display MassOfBeliefFunctionType::LabelSetType instances with the "<<" operator
std::ostream& operator <<(std::ostream& os, const MassOfBeliefFunctionType::LabelSetType& l)
{
  MassOfBeliefFunctionType::PrintLabelSet(os, l);
  return os;
}

// OPTIMIZED RECURSIVE DS FUSION
int otbDempsterShaferFusionOptRecTest(int argc, char * argv[])
{
  //*****************************
  // For DEBUG PURPOSE ONLY
  unsigned int nbClassesMax = 6;
  //*****************************

  typedef MassOfBeliefFunctionType::MassType MassType; //double by default

  std::vector<LabelPixelType> vectorAllLabels;

  for (unsigned int it = 1; it < argc; ++it)
    {
    vectorAllLabels.push_back(argv[it]);
    }

  unsigned int nbClasses = vectorAllLabels.size();

  MassOfBeliefFunctionType::LabelSetType universe, labelSet;
  typedef std::map<LabelPixelType, MassOfBeliefFunctionType::LabelSetType> LabelSetMapType;
  LabelSetMapType mapLabelSets;
  LabelSetMapType::iterator itMapLabelSets;
  for (unsigned it = 0; it < vectorAllLabels.size(); ++it)
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

  if ((argc - 1) > nbClassesMax) //For tests with more than 4 labels in the universe TO TEST PROCESSING SPEED
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

  if ((argc - 1) > nbClassesMax) //For tests with more than 4 labels in the universe TO TEST PROCESSING SPEED
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

  for (unsigned int it = 1; it < argc; ++it)
    {
    vectorAllLabels.push_back(argv[it]);
    }

  unsigned int nbClasses = vectorAllLabels.size();

  MassOfBeliefFunctionType::LabelSetType universe, labelSet;
  typedef std::map<LabelPixelType, MassOfBeliefFunctionType::LabelSetType> LabelSetMapType;
  LabelSetMapType mapLabelSets;
  LabelSetMapType::iterator itMapLabelSets;
  for (unsigned it = 0; it < vectorAllLabels.size(); ++it)
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

  for (unsigned int it = 1; it < argc; ++it)
    {
    vectorAllLabels.push_back(argv[it]);
    }

  unsigned int nbClasses = vectorAllLabels.size();

  MassOfBeliefFunctionType::LabelSetType universe, labelSet;
  typedef std::map<LabelPixelType, MassOfBeliefFunctionType::LabelSetType> LabelSetMapType;
  LabelSetMapType mapLabelSets;
  LabelSetMapType::iterator itMapLabelSets;
  for (unsigned it = 0; it < vectorAllLabels.size(); ++it)
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
