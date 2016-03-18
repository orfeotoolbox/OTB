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

#include "itkMetaDataObject.h"
#include "otbMetaDataKey.h"

namespace otb
{
/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TMaskImage>
DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::DSFusionOfClassifiersImageFilter()
{
  this->SetNumberOfIndexedInputs(2);
  this->SetNumberOfRequiredInputs(1);

  this->m_Universe.clear();
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
::SetInputMapsOfMassesOfBelief(const VectorOfMapOfMassesOfBeliefType * ptrVectorOfMapOfMassesOfBelief)
{
  this->m_VectorOfMapMOBs = *ptrVectorOfMapOfMassesOfBelief;
}

template <class TInputImage, class TOutputImage, class TMaskImage>
const typename DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::VectorOfMapOfMassesOfBeliefType *
DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::GetInputMapsOfMassesOfBelief()
{
  if (this->GetNumberOfInputs() < 2)
    {
    return 0;
    }
  return &this->m_VectorOfMapMOBs;
}
/* ************************************************************************************************************** */

template <class TInputImage, class TOutputImage, class TMaskImage>
void
DSFusionOfClassifiersImageFilter<TInputImage, TOutputImage, TMaskImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  // Set the NoData value
  std::vector<bool> noDataValueAvailable;
  noDataValueAvailable.push_back(true);
  std::vector<double> noDataValue;
  noDataValue.push_back(m_LabelForNoDataPixels);
  itk::MetaDataDictionary& dict = this->GetOutput()->GetMetaDataDictionary();
  itk::EncapsulateMetaData<std::vector<bool> >(dict,MetaDataKey::NoDataValueAvailable,noDataValueAvailable);
  itk::EncapsulateMetaData<std::vector<double> >(dict,MetaDataKey::NoDataValue,noDataValue);
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

  m_NumberOfClassifiers = m_VectorOfMapMOBs.size();

  // *****************************************************************************************************
  // For each classifier: Definition of the UNIVERSE
  // *****************************************************************************************************
  m_VectorOfUniverseMOBs.clear();
  m_Universe.clear();

  for (unsigned int itClk = 0; itClk < m_NumberOfClassifiers; ++itClk)
    {
    // Map of masses of belief of the itClk^th classifier
    LabelMassMapType mapMOBsClk = m_VectorOfMapMOBs[itClk];

    // mobUniverseClk is set to ZERO in order to assure the correct estimation of the Belief Functions of the
    // complementary sets bel({Ai_}) in the optimized DS combination
    MassType mobUniverseClk = 0.;
    // Vector containing the Mass of Belief of the UNIVERSE for each classifier
    m_VectorOfUniverseMOBs.push_back(mobUniverseClk);

    // Each label present in the itClk^th classifier is pushed in the UNIVERSE if NOT present in it yet
    // thus, this->m_Universe = std::map<Label, Number of classifiers containing the label Label> is calculated
    typename LabelMassMapType::iterator itMapMOBClk;
    for (itMapMOBClk = mapMOBsClk.begin(); itMapMOBClk != mapMOBsClk.end(); ++itMapMOBClk)
      {
      LabelType classLabel = itMapMOBClk->first;

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

  LabelMassMapType mapJointMassesStepI, mapJointMassesStepI_, mapJointMassesUniverseStepI;
  LabelMassMapType mapOfLabelsWithMassOne, mapOfLabelsWithMassZero;

  // Extracting the masses m(Ai), m(Ai_) and m(OMEGA) for each of the K (= m_NumberOfClassifiers) classifiers
  // and grouping them according to the {Ai} singletons
  // (ex: mg(A), mg(B), mg(C),..., with mg(Ai) the joint mass of the masses of classifiers with result Ai for
  // the pixel vectorPixelValue)
  for (unsigned int itClk = 0; itClk < m_NumberOfClassifiers; ++itClk)
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


      if ((mLabelSetClk > 0) && (mLabelSetClk < 1.0))
        {
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
        }// END if ((mLabelSetClk > 0) && (mLabelSetClk < 1.0))
      else
        {
        if (mLabelSetClk == 1.0)
          {
          mapOfLabelsWithMassOne[classLabelk] = mLabelSetClk;
          }
        else
          {
          if (mLabelSetClk == 0)
            {
            mapOfLabelsWithMassZero[classLabelk] = 1.0;
            }
          }
        } // END if ((mLabelSetClk == 0) || (mLabelSetClk == 1.0))

      } // END if (classLabelk != m_LabelForNoDataPixels)
    }

  /* ************************************************************************************************ */
  /* ************************************************************************************************ */
  // If ALL the classifiers give the same result, then there is no need to process the DS combination
  typename LabelMassMapType::iterator itMapMOBClk;
  if (mapJointMassesStepI.size() == 1)
    {
    itMapMOBClk = mapJointMassesStepI.begin();
    classLabelk = itMapMOBClk->first;
    return classLabelk;
    }

  /* ************************************************************************************************ */
  /* ************************************************************************************************ */
  // If at least one classifier returns a label with a mass of belief equal to 1.0, then 2 cases
  // should be considered:
  //
  // -If this label is unique, then this label is considered as the truth, and the output pixel is set
  //  to this label value
  //
  // -If this label is NOT unique, then the corresponding classifiers are in conflict and the output
  //  pixel is set to the m_LabelForUndecidedPixels value

  if (mapOfLabelsWithMassOne.size() > 0)
    {
    if (mapOfLabelsWithMassOne.size() == 1)
      {
      itMapMOBClk = mapOfLabelsWithMassOne.begin();
      classLabelk = itMapMOBClk->first;
      return classLabelk;
      }
    else
      {
      return m_LabelForUndecidedPixels;
      }
    }

  /* ************************************************************************************************ */
  /* ************************************************************************************************ */
  // If ALL the K (= m_NumberOfClassifiers) classifiers of the pixel vectorPixelValue have
  // masses of belief equal to 0, then the output pixel is set to the m_LabelForUndecidedPixels value
  if (mapOfLabelsWithMassZero.size() == m_NumberOfClassifiers)
    {
    return m_LabelForUndecidedPixels;
    }

  /* ************************************************************************************************ */
  /* ************************************************************************************************ */
  // If ALL the K (= m_NumberOfClassifiers) classifiers of the pixel vectorPixelValue are
  // equal to m_LabelForNoDataPixels, then the output pixel is set to the m_LabelForNoDataPixels value
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
  K = 0; //suppress warning. not sure about default value ??
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
  LabelMassMapType mapBelStepII, mapBelStepII_;
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
    belLabelSetClk_ = 1 - belLabelSetClk;
    /*if (nbClkGroupsStepI == m_NumberOfClassesInUniverse)
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
      }*/

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
    /*
    else
      {
      // Adjustment: Bel({Ai_}) = m_stepII({Ai_}) + 1 - SUM(m_stepII({Aj})) here;
      // with {Aj} ALL the labels present in vectorPixelValue
      mapBelStepII_[classLabelk] += (1 - addBelLabelSetClk);
      }
    */

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
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId)
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
