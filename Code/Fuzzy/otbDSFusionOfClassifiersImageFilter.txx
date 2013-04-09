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
#ifndef __otbDSFusionOfClassifiersImageFilter_txx
#define __otbDSFusionOfClassifiersImageFilter_txx

#include "otbDSFusionOfClassifiersImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkProgressReporter.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TMaskImage>
DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::DSFusionOfClassifiersImageFilter()
{
  this->SetNumberOfInputs(2);
  this->SetNumberOfRequiredInputs(1);

  this->m_Universe.clear();
  this->m_ConfusionMatrixToMassOfBeliefFilter = ConfusionMatrixToMassOfBeliefType::New();

  // Possible values for this->m_DefinitionMethod:
  // ConfusionMatrixToMassOfBeliefType::PRECISION, RECALL, ACCURACY and KAPPA
  this->m_DefinitionMethod = ConfusionMatrixToMassOfBeliefType::PRECISION;

  this->m_LabelForNoDataPixels = itk::NumericTraits<LabelType>::ZeroValue();
  this->m_LabelForUndecidedPixels = itk::NumericTraits<LabelType>::ZeroValue();
}

/* ************************************************************************************************************** */
template <class TInputImage, class TOutputImage, class TMaskImage>
void
DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::SetInputMask(const MaskImageType * mask)
{
  this->itk::ProcessObject::SetNthInput(1, const_cast<MaskImageType *>(mask));
}

template <class TInputImage, class TOutputImage, class TMaskImage>
const typename DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::MaskImageType *
DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::GetInputMask()
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }
  return static_cast<const MaskImageType *>(this->itk::ProcessObject::GetInput(1));
}

/* ************************************************************************************************************** */
template <class TInputImage, class TOutputImage, class TMaskImage>
void
DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::SetInputMapsOfIndices(const VectorOfMapOfIndicesType * pointerToVectorOfMapOfIndices)
{
  this->m_VectorOfMapOfIndices = *pointerToVectorOfMapOfIndices;
}

template <class TInputImage, class TOutputImage, class TMaskImage>
const typename DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::VectorOfMapOfIndicesType *
DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::GetInputMapsOfIndices()
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }
  return &this->m_VectorOfMapOfIndices;
}
/* ************************************************************************************************************** */

template <class TInputImage, class TOutputImage, class TMaskImage>
void
DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::SetInputConfusionMatrices(const VectorOfConfusionMatricesType * pointerToVectorOfConfusionMatrices)
{
  this->m_VectorOfConfusionMatrices = *pointerToVectorOfConfusionMatrices;
}

template <class TInputImage, class TOutputImage, class TMaskImage>
const typename DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::VectorOfConfusionMatricesType *
DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::GetInputConfusionMatrices()
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }
  return &this->m_VectorOfConfusionMatrices;
}
/* ************************************************************************************************************** */


template <class TInputImage, class TOutputImage, class TMaskImage>
void
DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::BeforeThreadedGenerateData()
{
  // **********************************************
  // PREPROCESSING
  // **********************************************

  m_NumberOfClassifiers = m_VectorOfMapOfIndices.size();

  // *****************************************************************************************************
  // For each classifier: Conversion of its Confusion Matrix to a MAP of Masses Of Belief map<Label, MOB>
  // with MOB defined according to the this->m_DefinitionMethod parameter
  // + Definition of the UNIVERSE
  // *****************************************************************************************************
  m_VectorOfMapMOBs.clear();
  m_VectorOfUniverseMOBs.clear();
  m_Universe.clear();
  for (unsigned itClk = 0; itClk < m_NumberOfClassifiers; ++itClk)
    {
    // Number of classes present in the itClk^th classifier
    unsigned int nbClassesClk = m_VectorOfMapOfIndices[itClk].size();

    m_ConfusionMatrixToMassOfBeliefFilter->SetMapOfIndices(m_VectorOfMapOfIndices[itClk]);
    m_ConfusionMatrixToMassOfBeliefFilter->SetConfusionMatrix(m_VectorOfConfusionMatrices[itClk]);
    m_ConfusionMatrixToMassOfBeliefFilter->SetDefinitionMethod(m_DefinitionMethod);
    m_ConfusionMatrixToMassOfBeliefFilter->Update();

    // Vector containing ALL the K (= m_NumberOfClassifiers) std::map<Label, MOB> of Mass of Belief
    m_VectorOfMapMOBs.push_back(m_ConfusionMatrixToMassOfBeliefFilter->GetMapMassOfBelief());

    // mobUniverseClk is set to ZERO in order to assure the correct estimation of the Belief Functions of the
    // complementary sets bel({Ai_}) in the optimized DS combination
    MassType mobUniverseClk = 0.;
    // Vector containing the Mass of Belief of the UNIVERSE for each classifier
    m_VectorOfUniverseMOBs.push_back(mobUniverseClk);

    // Each label present in the itClk^th classifier is pushed in the UNIVERSE if NOT present in it yet
    // thus, this->m_Universe = std::map<Label, Number of classifiers containing the label Label> is calculated
    for (unsigned itLabel = 0; itLabel < nbClassesClk; ++itLabel)
      {
      LabelType classLabel = m_VectorOfMapOfIndices[itClk][itLabel];
      // If the current classLabel has already been pushed in the UNIVERSE
      if (m_Universe.count(classLabel) > 0)
        {
        m_Universe[classLabel]++;
        }
      else
        {
        m_Universe[classLabel] = 1;
        }
      }
    }

  m_NumberOfClassesInUniverse = m_Universe.size();
}


template <class TInputImage, class TOutputImage, class TMaskImage>
const typename DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::LabelType
DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::OptimizedDSMassCombination(PixelType vectorPixelValue)
{
  LabelType outFusedLabelOut = itk::NumericTraits<LabelType>::ZeroValue();


  /* ************************************************************************************************ */
  /* ******************************* DS FUSION STEP #1: RECURSIVE METHOD **************************** */
  /* ************************************************************************************************ */

  LabelType classLabelk;
  MassType mLabelSetClk, mLabelSetClk_, mUniverseClk;
  MassType mLabelSetClkprev, mLabelSetClkprev_, mUniverseClkprev;
  MassType KClk, mLabelSetClkNew, mLabelSetClkNew_, mUniverseClkNew;

  SingleClassLabelMassMapType mapJointMassesStepI, mapJointMassesStepI_, mapJointMassesUniverseStepI;

  // Extracting the masses m(Ai), m(Ai_) and m(OMEGA) for each of the K (= m_NumberOfClassifiers) classifiers
  // and grouping them according to the {Ai} singletons
  // (ex: mg(A), mg(B), mg(C),..., with mg(Ai) the joint mass of the masses of classifiers with result Ai for
  // the pixel vectorPixelValue)
  for (unsigned itClk = 0; itClk < m_NumberOfClassifiers; ++itClk)
    {
    // Value of the itClk^th classifier {Ai} in the vectorPixelValue
    classLabelk = vectorPixelValue[itClk]; // label {Ai}

    if (classLabelk != m_LabelForNoDataPixels)
      {
      // Extracting the masses of belief of the three focal elements {Ai}, {Ai_} and OMEGA = {Ai U Ai_} = UNIVERSE
      // of the k^th classifier itClk
      mUniverseClk = m_VectorOfUniverseMOBs[itClk]; // MOB_Clk(OMEGA)
      mLabelSetClk = m_VectorOfMapMOBs[itClk][classLabelk]; // MOB_Clk({Ai})
      mLabelSetClk_ = 1 - mLabelSetClk - mUniverseClk; // MOB_Clk({Ai_})

      /*std::cout << "vectorPixelValue[" << itClk << "] = " << classLabelk;
      std::cout << "; MassOfBelief_Cl_" << itClk << "[" << classLabelk << "] = " << mLabelSetClk;
      std::cout << std::endl; */


      if (mapJointMassesStepI.count(classLabelk) == 0)
        {
        // If the label {Ai} is NOT present in vectorPixelValue yet: initializations of the three DS combined MOBs
        mLabelSetClkNew = mLabelSetClk;
        mLabelSetClkNew_ = mLabelSetClk_;
        mUniverseClkNew = mUniverseClk;
        }
      else
        {
        // If the label {Ai} is already present in vectorPixelValue: recursive DS combination of the three MOBs
        mLabelSetClkprev = mapJointMassesStepI[classLabelk];
        mLabelSetClkprev_ = mapJointMassesStepI_[classLabelk];
        mUniverseClkprev = mapJointMassesUniverseStepI[classLabelk];

        KClk = 1.0 / (1 - mLabelSetClkprev * mLabelSetClk_ - mLabelSetClkprev_ * mLabelSetClk);
        mLabelSetClkNew = KClk * (mLabelSetClkprev * (mLabelSetClk + mUniverseClk) + mUniverseClkprev * mLabelSetClk);
        mLabelSetClkNew_ = KClk * (mLabelSetClkprev_ * (mLabelSetClk_ + mUniverseClk) + mUniverseClkprev * mLabelSetClk_);
        mUniverseClkNew = KClk * mUniverseClkprev * mUniverseClk;
        }

      mapJointMassesStepI[classLabelk] = mLabelSetClkNew;
      mapJointMassesStepI_[classLabelk] = mLabelSetClkNew_;
      mapJointMassesUniverseStepI[classLabelk] = mUniverseClkNew;
      } // END if (classLabelk != m_LabelForNoDataPixels)
    }

  /* ************************************************************************************************ */
  /* ************************************************************************************************ */
  // If ALL the K (= m_NumberOfClassifiers) classifiers of the pixel vectorPixelValue
  // are equal to this->m_LabelForNoDataPixels, they keep their m_LabelForNoDataPixels value
  if (mapJointMassesStepI.size() == 0)
    {
    return m_LabelForNoDataPixels;
    }
  /* ************************************************************************************************ */
  /* ************************************************************************************************ */

  //std::cout << "*******************************************************************************" << std::endl;
  //std::cout << std::endl;

  /* ************************************************************************************************ */
  /* ******************************* DS FUSION STEP #2: OPTIMIZED METHOD **************************** */
  /* ************************************************************************************************ */

  // Calculation of the four constants A, B, C and K
  MassType A = 0, B = 1, C = 1, K;
  SingleClassLabelMassMapType::iterator itMapMOBClk;
  for (itMapMOBClk = mapJointMassesStepI.begin(); itMapMOBClk != mapJointMassesStepI.end(); ++itMapMOBClk)
    {
    classLabelk = itMapMOBClk->first;

    mLabelSetClk = mapJointMassesStepI[classLabelk];
    mLabelSetClk_ = mapJointMassesStepI_[classLabelk];
    mUniverseClk = mapJointMassesUniverseStepI[classLabelk];

    A += (mLabelSetClk / (1 - mLabelSetClk));
    B *= (1 - mLabelSetClk);
    C *= mLabelSetClk_;

    /*std::cout << "****************************************" << std::endl;
    std::cout << "mapJointMassesStepI[" << classLabelk << "] = " << mLabelSetClk << std::endl;
    std::cout << "mapJointMassesStepI_[" << classLabelk << "] = " << mLabelSetClk_ << std::endl;
    std::cout << "mapJointMassesUniverseStepI[" << classLabelk << "] = " << mUniverseClk << std::endl; */
    }

  unsigned int nbClkGroupsStepI = mapJointMassesStepI.size();
  if (nbClkGroupsStepI == m_NumberOfClassesInUniverse)
    {
    K = 1.0 / ((1 + A) * B - C);
    }
  else
    {
    if (nbClkGroupsStepI < m_NumberOfClassesInUniverse)
      {
      K = 1.0 / ((1 + A) * B);
      }
    }

  /*
  std::cout << "****************************************************************************" << std::endl;
  std::cout << "****************************************************************************" << std::endl;
  std::cout << "m_NumberOfClassesInUniverse = " << m_NumberOfClassesInUniverse << std::endl;
  std::cout << "m_NumberOfClassifiers = " << m_NumberOfClassifiers << std::endl;
  std::cout << "nbClkGroupsStepI = " << nbClkGroupsStepI << std::endl;
  std::cout << "****************************************************************************" << std::endl;
  std::cout << "****************************************************************************" << std::endl; */

  // Calculation of the Belief functions Bel({Ai}) and Bel({Ai_}) of the singletons {Ai} and {Ai_}
  SingleClassLabelMassMapType mapBelStepII, mapBelStepII_;
  MassType belLabelSetClk, belLabelSetClk_, addBelLabelSetClk = 0.;
  for (itMapMOBClk = mapJointMassesStepI.begin(); itMapMOBClk != mapJointMassesStepI.end(); ++itMapMOBClk)
    {
    classLabelk = itMapMOBClk->first;

    // Joint Masses of Belief AFTER STEP I (one MOB per singleton class {Ai} in the UNIVERSE)
    mLabelSetClk = mapJointMassesStepI[classLabelk]; // Joint_MOB_Step_I({Ai})
    mLabelSetClk_ = mapJointMassesStepI_[classLabelk]; // Joint_MOB_Step_I({Ai_})
    mUniverseClk = mapJointMassesUniverseStepI[classLabelk]; // Joint_MOB_Step_I(OMEGA)

    // Bel({Ai})
    if ((nbClkGroupsStepI == m_NumberOfClassesInUniverse) || ((nbClkGroupsStepI == (m_NumberOfClassesInUniverse - 1)) && (K == m_NumberOfClassesInUniverse)))
      {
      belLabelSetClk = K * ((mLabelSetClk / (1 - mLabelSetClk)) * B + (mUniverseClk * C / mLabelSetClk_));
      }
    else
      {
      belLabelSetClk = K * (mLabelSetClk / (1 - mLabelSetClk)) * B;
      }

    // Bel({Ai_})
    if (nbClkGroupsStepI == m_NumberOfClassesInUniverse)
      {
      belLabelSetClk_ = K * ((A - ((mLabelSetClk - mLabelSetClk_) / (1 - mLabelSetClk))) * B - C);
      }
    else
      {
      if ((nbClkGroupsStepI < m_NumberOfClassesInUniverse) && (K <= nbClkGroupsStepI))
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
    } // END Calculation of the Belief functions Bel({Ai}) and Bel({Ai_}) of the singletons {Ai} and {Ai_}


  // ADJUSTMENT in order to handle the residual MOB = (1 - addBelLabelSetClk) of the other singleton classes {Ai}
  // of the UNIVERSE which are NOT present in vectorPixelValue.
  // /!\ N.B.: This adjustment is correct ONLY if ALL the K (= m_NumberOfClassifiers) classifiers have mUniverseClk = 0
  typename ClassifierHistogramType::iterator itUniverse;
  for (itUniverse = m_Universe.begin(); itUniverse != m_Universe.end(); ++itUniverse)
    {
    classLabelk = itUniverse->first;

    // For the other singleton classes {Ai} of the UNIVERSE which are NOT present in vectorPixelValue,
    // Bel({Ai_}) = SUM(m_stepII({Aj})) = SUM(Bel({Aj})) here; with {Aj} ALL the labels present in vectorPixelValue
    if (mapBelStepII[classLabelk] == 0)
      {
      mapBelStepII_[classLabelk] = addBelLabelSetClk;
      }
    else
      {
      // Adjustment
      mapBelStepII_[classLabelk] += (1 - addBelLabelSetClk);
      }

    /*std::cout << "Bel(" << classLabelk << ") = " << mapBelStepII[classLabelk] << std::endl;
     std::cout << "Bel(NOT_" << classLabelk << ") = " << mapBelStepII_[classLabelk] << std::endl;
     std::cout << std::endl; */
    }

  //std::cout << "****************************************************************************" << std::endl;


  /* ***************************************************************************************** */
  /* ************************************ DECISION PROCESS *********************************** */
  /* ***************************************************************************************** */

  // The decision for the DS Fusion is made with the MAXIMAL Belief function:
  // {Ai} is chosen if Bel({Ai}) = MAX(Bel({Aj}))

  MassType fusedDSBelLabelSetClk = 0.; // Here the MAX of Bel({Aj}
  for (itUniverse = m_Universe.begin(); itUniverse != m_Universe.end(); ++itUniverse)
    {
    classLabelk = itUniverse->first;

    if (itUniverse == m_Universe.begin())
      {
      outFusedLabelOut = classLabelk;
      fusedDSBelLabelSetClk = mapBelStepII[classLabelk];
      }
    else
      {
      if (mapBelStepII[classLabelk] >= fusedDSBelLabelSetClk)
        {
        outFusedLabelOut = classLabelk;
        fusedDSBelLabelSetClk = mapBelStepII[classLabelk];
        }
      }

    }

  // If the DS VOTED LABEL is NOT unique, the result of the DS Fusion is this->m_LabelForUndecidedPixels
  for (itUniverse = m_Universe.begin(); itUniverse != m_Universe.end(); ++itUniverse)
    {
    classLabelk = itUniverse->first;
    if ((mapBelStepII[classLabelk] == fusedDSBelLabelSetClk) && (classLabelk != outFusedLabelOut))
      {
      outFusedLabelOut = m_LabelForUndecidedPixels;
      break;
      }
    }

  /*std::cout << "****************************************************************************" << std::endl;
  std::cout << "outFusedLabelOut = " << outFusedLabelOut << std::endl;
  std::cout << "Bel(" << outFusedLabelOut << ") = " << fusedDSBelLabelSetClk << std::endl; */

  return outFusedLabelOut;
}




template <class TInputImage, class TOutputImage, class TMaskImage>
void
DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId)
{
  // Get the input pointers
  InputImageConstPointerType inputPtr      = this->GetInput();
  MaskImageConstPointerType  inputMaskPtr  = this->GetInputMask();
  OutputImagePointerType     outputPtr     = this->GetOutput();

  // Progress reporting
  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  // Define iterators
  typedef itk::ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef itk::ImageRegionConstIterator<MaskImageType>  MaskIteratorType;
  typedef itk::ImageRegionIterator<OutputImageType>     OutputIteratorType;

  InputIteratorType inIt(inputPtr, outputRegionForThread);
  OutputIteratorType outIt(outputPtr, outputRegionForThread);

  // Eventually iterate on masks
  MaskIteratorType maskIt;
  if (inputMaskPtr)
    {
    maskIt = MaskIteratorType(inputMaskPtr, outputRegionForThread);
    maskIt.GoToBegin();
    }

  bool validPoint = true;

  // Walk the part of the image
  for (inIt.GoToBegin(), outIt.GoToBegin(); !inIt.IsAtEnd() && !outIt.IsAtEnd(); ++inIt, ++outIt)
    {
    // Check pixel validity
    if (inputMaskPtr)
      {
      validPoint = maskIt.Get() > 0;
      ++maskIt;
      }
    // If point is valid
    if (validPoint)
      {
      // Over ALL the i components inIt.Get()[i] of the input vector pixel (with i between 0 and m_NumberOfClassifiers)
      outIt.Set(this->OptimizedDSMassCombination(inIt.Get()));
      }
    else
      {
      // else, outIt is set to the m_LabelForNoDataPixels value
      outIt.Set(m_LabelForNoDataPixels);
      }
    progress.CompletedPixel();
    }

}
/**
 * PrintSelf Method
 */
template <class TInputImage, class TOutputImage, class TMaskImage>
void
DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}
} // End namespace otb
#endif
