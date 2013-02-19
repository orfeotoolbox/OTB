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

// OPTIMIZED DS FUSION
int otbDempsterShaferFusionOptTest(int argc, char * argv[])
{
  typedef MassOfBeliefFunctionType::MassType MassValueType; //double by default

  std::vector<LabelPixelType> vectorAllLabels;

  for (unsigned int it = 1; it < argc; ++it)
    {
    vectorAllLabels.push_back(argv[it]);
    }

  unsigned int nbClasses = vectorAllLabels.size();

  MassOfBeliefFunctionType::LabelSetType universe, labelSet;
  typedef std::map<LabelPixelType, MassOfBeliefFunctionType::LabelSetType> MapLabelSetType;
  MapLabelSetType mapLabelSets;
  MapLabelSetType::iterator itMapLabelSets;
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
  typedef std::vector<MassOfBeliefFunctionType::LabelSetType> VectorLabelSetType;
  VectorLabelSetType classifiedPixelX;
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

  // Masses of the singleton sets. Expected to be automatically filled with the CONFUSION MATRICES

  typedef std::map<MassOfBeliefFunctionType::LabelSetType, MassValueType> MapMassType;
  MapMassType mCl0;
  MapMassType mCl1;
  MapMassType mCl2;
  MapMassType mCl3;
  MapMassType mCl4;
  MapMassType mCl5;

  mCl0[mapLabelSets[vectorAllLabels[0]]] = 0.25;
  mCl0[mapLabelSets[vectorAllLabels[1]]] = 0.6;
  mCl0[mapLabelSets[vectorAllLabels[2]]] = 0.10;
  mCl0[mapLabelSets[vectorAllLabels[3]]] = 0.05;
  //mCl0[universe] = 0;

  mCl1[mapLabelSets[vectorAllLabels[0]]] = 0.8;
  mCl1[mapLabelSets[vectorAllLabels[1]]] = 0;
  mCl1[mapLabelSets[vectorAllLabels[2]]] = 0.05;
  mCl1[mapLabelSets[vectorAllLabels[3]]] = 0.15;
  //mCl1[universe] = 0;

  mCl2[mapLabelSets[vectorAllLabels[0]]] = 0.2;
  mCl2[mapLabelSets[vectorAllLabels[1]]] = 0.65;
  mCl2[mapLabelSets[vectorAllLabels[2]]] = 0.1;
  mCl2[mapLabelSets[vectorAllLabels[3]]] = 0.05;
  //mCl2[universe] = 0;

  mCl3[mapLabelSets[vectorAllLabels[0]]] = 0.45;
  mCl3[mapLabelSets[vectorAllLabels[1]]] = 0.22;
  mCl3[mapLabelSets[vectorAllLabels[2]]] = 0.15;
  mCl3[mapLabelSets[vectorAllLabels[3]]] = 0.18;
  //mCl3[universe] = 0;

  mCl4[mapLabelSets[vectorAllLabels[0]]] = 0.15;
  mCl4[mapLabelSets[vectorAllLabels[1]]] = 0.10;
  mCl4[mapLabelSets[vectorAllLabels[2]]] = 0.75;
  mCl4[mapLabelSets[vectorAllLabels[3]]] = 0;
  //mCl4[universe] = 0;

  mCl5[mapLabelSets[vectorAllLabels[0]]] = 0.9;
  mCl5[mapLabelSets[vectorAllLabels[1]]] = 0.05;
  mCl5[mapLabelSets[vectorAllLabels[2]]] = 0.03;
  mCl5[mapLabelSets[vectorAllLabels[3]]] = 0.02;
  //mCl5[universe] = 0;


  // Vector containing the std::maps of masses of ALL the singleton sets for each classifier k = 1,...,6 here
  std::vector<MapMassType> mClVector;
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
  MassValueType mLabelSetClk, mLabelSetClk_, mUniverseClk;
  MassOfBeliefFunctionType::Pointer mOBFClk;
  JointMassOfBeliefFilterType::Pointer jointMassClkFilter;

  typedef std::map<MassOfBeliefFunctionType::LabelSetType, JointMassOfBeliefFilterType::Pointer>
      mapJointMassOfBeliefFilterType;
  mapJointMassOfBeliefFilterType mapJointMassFilters;
  mapJointMassOfBeliefFilterType::iterator itMapJMOBFilters;

  // Extracting the masses m(Ai), m(Ai_) and m(OMEGA) for each of the M = 6 classifiers
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

  MassValueType A = 0, B = 1, C = 1, K;
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

  typedef std::map<MassOfBeliefFunctionType::LabelSetType, MassValueType> mapBeliefPlausibilityType;
  mapBeliefPlausibilityType mapBelStepII;
  mapBeliefPlausibilityType::iterator itMapBelStepII;
  MassValueType belLabelSetClk, belLabelSetClk_;
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
      belLabelSetClk = K * ((mLabelSetClk / (1 - mLabelSetClk)) * B);
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
    }

  std::cout << "****************************************************************************" << std::endl;
  std::cout << "****************************************************************************" << std::endl;

  // The decision for the DS Fusion is made with the MAXIMAL Belief function:
  // {Ai} is chosen if Bel({Ai}) = MAX(Bel({Aj}))

  MassOfBeliefFunctionType::LabelSetType fusedDSLabelSet;
  MassValueType fusedDSBelLabelSetClk = 0; //Here the MAX of Bel(Ai)
  for (itMapLabelSets = mapLabelSets.begin(); itMapLabelSets != mapLabelSets.end(); ++itMapLabelSets)
    {
    if (itMapLabelSets == mapLabelSets.begin())
      {
      fusedDSLabelSet = itMapLabelSets->second;
      fusedDSBelLabelSetClk = mapBelStepII[itMapLabelSets->second];
      }
    else
      {
      if (mapBelStepII[itMapLabelSets->second] >= fusedDSBelLabelSetClk)
        {
        fusedDSLabelSet = itMapLabelSets->second;
        fusedDSBelLabelSetClk = mapBelStepII[itMapLabelSets->second];
        }
      }

    std::cout << "Bel(" << itMapLabelSets->second << ") = " << mapBelStepII[itMapLabelSets->second] << std::endl;
    }

  // If the DS VOTED LABEL is NOT unique, the result of the DS Fusion is the UNION of all the candidates {Ai}
  // with Bel({Ai}) = MAX(Bel({Aj}))
  for (itMapLabelSets = mapLabelSets.begin(); itMapLabelSets != mapLabelSets.end(); ++itMapLabelSets)
    {
    if ((mapBelStepII[itMapLabelSets->second] == fusedDSBelLabelSetClk) && (itMapLabelSets->second != fusedDSLabelSet))
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

// ****************************************************************************
// ****************************************************************************
// ****************************************************************************
// NOT OPTIMIZED DS FUSION
int otbDempsterShaferFusionTest(int argc, char * argv[])
{
  typedef MassOfBeliefFunctionType::MassType MassValueType; //double by default

  std::vector<LabelPixelType> vectorAllLabels;

  for (unsigned int it = 1; it < argc; ++it)
    {
    vectorAllLabels.push_back(argv[it]);
    }

  unsigned int nbClasses = vectorAllLabels.size();

  MassOfBeliefFunctionType::LabelSetType universe, labelSet;
  typedef std::map<LabelPixelType, MassOfBeliefFunctionType::LabelSetType> MapLabelSetType;
  MapLabelSetType mapLabelSets;
  MapLabelSetType::iterator itMapLabelSets;
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
  typedef std::vector<MassOfBeliefFunctionType::LabelSetType> VectorLabelSetType;
  VectorLabelSetType classifiedPixelX;
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

  // Masses of the singleton sets. Expected to be automatically filled with the CONFUSION MATRICES

  typedef std::map<MassOfBeliefFunctionType::LabelSetType, MassValueType> MapMassType;
  MapMassType mCl0;
  MapMassType mCl1;
  MapMassType mCl2;
  MapMassType mCl3;
  MapMassType mCl4;
  MapMassType mCl5;

  mCl0[mapLabelSets[vectorAllLabels[0]]] = 0.25;
  mCl0[mapLabelSets[vectorAllLabels[1]]] = 0.6;
  mCl0[mapLabelSets[vectorAllLabels[2]]] = 0.10;
  mCl0[mapLabelSets[vectorAllLabels[3]]] = 0.05;
  //mCl0[universe] = 0;

  mCl1[mapLabelSets[vectorAllLabels[0]]] = 0.8;
  mCl1[mapLabelSets[vectorAllLabels[1]]] = 0;
  mCl1[mapLabelSets[vectorAllLabels[2]]] = 0.05;
  mCl1[mapLabelSets[vectorAllLabels[3]]] = 0.15;
  //mCl1[universe] = 0;

  mCl2[mapLabelSets[vectorAllLabels[0]]] = 0.2;
  mCl2[mapLabelSets[vectorAllLabels[1]]] = 0.65;
  mCl2[mapLabelSets[vectorAllLabels[2]]] = 0.1;
  mCl2[mapLabelSets[vectorAllLabels[3]]] = 0.05;
  //mCl2[universe] = 0;

  mCl3[mapLabelSets[vectorAllLabels[0]]] = 0.45;
  mCl3[mapLabelSets[vectorAllLabels[1]]] = 0.22;
  mCl3[mapLabelSets[vectorAllLabels[2]]] = 0.15;
  mCl3[mapLabelSets[vectorAllLabels[3]]] = 0.18;
  //mCl3[universe] = 0;

  mCl4[mapLabelSets[vectorAllLabels[0]]] = 0.15;
  mCl4[mapLabelSets[vectorAllLabels[1]]] = 0.10;
  mCl4[mapLabelSets[vectorAllLabels[2]]] = 0.75;
  mCl4[mapLabelSets[vectorAllLabels[3]]] = 0;
  //mCl4[universe] = 0;

  mCl5[mapLabelSets[vectorAllLabels[0]]] = 0.9;
  mCl5[mapLabelSets[vectorAllLabels[1]]] = 0.05;
  mCl5[mapLabelSets[vectorAllLabels[2]]] = 0.03;
  mCl5[mapLabelSets[vectorAllLabels[3]]] = 0.02;
  //mCl5[universe] = 0;


  // Vector containing the std::maps of masses of ALL the singleton sets for each classifier k = 1,...,6 here
  std::vector<MapMassType> mClVector;
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
  MassValueType mLabelSetClk, mLabelSetClk_, mUniverseClk;
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

  MassOfBeliefFunctionType::LabelSetType fusedDSLabelSet;
  MassValueType fusedDSBelLabelSetClk = 0; //Here the MAX of Bel(Ai)
  for (itMapLabelSets = mapLabelSets.begin(); itMapLabelSets != mapLabelSets.end(); ++itMapLabelSets)
    {
    if (itMapLabelSets == mapLabelSets.begin())
      {
      fusedDSLabelSet = itMapLabelSets->second;
      fusedDSBelLabelSetClk = jointMass->GetBelief(itMapLabelSets->second);
      }
    else
      {
      if (jointMass->GetBelief(itMapLabelSets->second) >= fusedDSBelLabelSetClk)
        {
        fusedDSLabelSet = itMapLabelSets->second;
        fusedDSBelLabelSetClk = jointMass->GetBelief(itMapLabelSets->second);
        }
      }

    std::cout << "Bel(" << itMapLabelSets->second << ") = " << jointMass->GetBelief(itMapLabelSets->second)
        << std::endl;
    }

  //If the DS VOTED LABEL is NOT unique
  for (itMapLabelSets = mapLabelSets.begin(); itMapLabelSets != mapLabelSets.end(); ++itMapLabelSets)
    {
    if ((jointMass->GetBelief(itMapLabelSets->second) == fusedDSBelLabelSetClk) && (itMapLabelSets->second
        != fusedDSLabelSet))
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
