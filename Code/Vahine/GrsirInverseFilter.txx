/*
 * GrsirInverseFilter.txx
 * Date:     $Date$
 * Version:  $Revision$
 *
 * Vahine Framework
 * Copyright (C) 2008 to 2010 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */

#ifndef __VAHINEGRSIRINVERSEFILTER_TXX
#define __VAHINEGRSIRINVERSEFILTER_TXX

#include "GrsirInverseFilter.h"

namespace otb {

template<class TImage>
VahineGrsirInverseFilter<TImage>::VahineGrsirInverseFilter() :
  INVERSE_MASK(0x01),
  OPTREGUL_MASK(0x02),
  CUSREGUL_MASK(0x04),
  ALLREGUL_MASK(0x06),
  REGULVALUE_MASK(0x78),
  NbEigenVector(1)
{
  m_ResultList = InverseResultListType::New();
}

template<class TImage>
void VahineGrsirInverseFilter<TImage>::PushBackParameterBand(BandParameterType param){
  m_parameter.push_back(param);
}

template<class TImage>
void VahineGrsirInverseFilter<TImage>::GenerateOutputInformation(){
  vahineDebug("VahineGrsirInverseFilter::GenerateOutputInformation");
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  ImageConstPointer inputPtr  = this->GetInput();
  ImagePointer      outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr ) {
    return;
    }

  unsigned int    zSize = 0;
  unsigned int    physicalNumber = 1;
  ItParameterList itParam = m_parameter.begin();
  while(itParam != m_parameter.end() ) {
    if(checkParameter(*itParam, INVERSE_MASK) ) {
      if(checkParameter(*itParam, ALLREGUL_MASK) ) {
        zSize += getNumberOfRegulParameters(getLearningParameters(physicalNumber) );
        } else {
        // only one band for this physical parameter
        ++zSize;
        }
      }
    ++physicalNumber;
    ++itParam;
    }
  vahineDebug("Number of components per pixel for output = "<<zSize);
  //inputPtr->UpdateOutputInformation();
  outputPtr->SetLargestPossibleRegion( inputPtr->GetLargestPossibleRegion() );
  outputPtr->SetNumberOfComponentsPerPixel( zSize );
}

template<class TImage>
void VahineGrsirInverseFilter<TImage>::GenerateInputRequestedRegion(){
  vahineDebug("VahineGrsirLearningFilter::GenerateInputRequestedRegion");
  ImagePointer inputPtr =  const_cast<ImageType * >( this->GetInput() );
  typename ImageType::RegionType region = inputPtr->GetLargestPossibleRegion();
  vahineDebug("###################### region = "<<region);
  inputPtr->SetRequestedRegion(region);
}

/**
* extract all learning parameters for one physical parameter
*/
template<class TImage>
typename VahineGrsirInverseFilter<TImage>::BandPointer
VahineGrsirInverseFilter<TImage>::getLearningParameters(unsigned int bandNumber){
  vahineDebug("getLearningParameters bandNumber="<<bandNumber);
  ExtractROImonoFilterPointer extractFilter = ExtractROImonoFilterType::New();
  extractFilter->SetChannel(bandNumber);
  extractFilter->SetInput(m_LearningParam);
  BandPointer learningParmeters = extractFilter->GetOutput();
  learningParmeters->Update();
  return learningParmeters;
}

/*
 * first value of learned parameters is the number of regul parameters
 */
template<class TImage>
unsigned int VahineGrsirInverseFilter<TImage>::getNumberOfRegulParameters(BandPointer learnedParameters){
  BandLinearIterator itLearnedParameter(learnedParameters, learnedParameters->GetLargestPossibleRegion() );

  itLearnedParameter.SetDirection(0);       // x dimension
  itLearnedParameter.GoToBegin();
  return static_cast<unsigned int>(itLearnedParameter.Get() );
}

/*
 * second value of learned parameter is the best regul parameters
 */
template<class TImage>
unsigned int VahineGrsirInverseFilter<TImage>::getIndexForBestRegul(BandPointer learnedParameters){
  BandLinearIterator itLearnedParameter(learnedParameters, learnedParameters->GetLargestPossibleRegion() );

  itLearnedParameter.SetDirection(0);       // x dimension
  itLearnedParameter.GoToBegin();
  ++itLearnedParameter;       // skip first value
  return static_cast<unsigned int>(itLearnedParameter.Get() );
}

/** convert bitset parameter to unsigend int index
 */
template<class TImage>
unsigned int VahineGrsirInverseFilter<TImage>::getIndexForCustRegul(BandParameterType param){
  BandParameterType number = param &= REGULVALUE_MASK;

  number>>=3;
  unsigned int index = number.to_ulong();
  vahineDebug("getIndexForCustRegul "<< index);
  return index;
}

template<class TImage>
unsigned int VahineGrsirInverseFilter<TImage>::getNbRegularization(BandPointer learnedParameters){
  BandLinearIterator itLearnedParameter(learnedParameters, learnedParameters->GetLargestPossibleRegion() );

  itLearnedParameter.SetDirection(0);       // x dimension
  itLearnedParameter.GoToBegin();
  return static_cast<unsigned int>(itLearnedParameter.Get() );
}

/**
 * line start from 0
 */
template<class TImage>
typename VahineGrsirInverseFilter<TImage>::VnlVectorType
VahineGrsirInverseFilter<TImage>::getDataFromLine(BandPointer band, unsigned int index, bool allLine){
  VnlVectorType      vector;
  BandLinearIterator itLearnedParameter(band, band->GetLargestPossibleRegion() );

  itLearnedParameter.SetDirection(0);
  itLearnedParameter.GoToBegin();
  vahineDebug("getDataFromLine number = "<<index);
  for(unsigned int i=0; i<index; ++i) {
    itLearnedParameter.NextLine();
    }
  unsigned int size = band->GetLargestPossibleRegion().GetSize()[0];
  if(!allLine) {
    // first element is size
    size = static_cast<unsigned int>(itLearnedParameter.Get() );
    } else {
    // no size if all line requested
    --itLearnedParameter;
    }
  vahineDebug("getDataFromLine size = "<<size);
  vector.set_size(size);
  for(unsigned int j=0; j<vector.size(); ++j) {
    ++itLearnedParameter;
    vector[j] = static_cast<RealType>(itLearnedParameter.Get() );
    }
  return vector;
}

template<class TImage>
void VahineGrsirInverseFilter<TImage>::UpdateEigenStruct(BandPointer band, EigenStruct & eigenStruct,
                                                         unsigned int index){
  vahineDebug("UpdateEigenStruct index="<<index);
  // index start from 0
  unsigned int  offset = index*(2+NbEigenVector);
  VnlVectorType xVector = getDataFromLine(band, 1+offset, false);
  VnlVectorType yVector = getDataFromLine(band, 2+offset, false);
  eigenStruct.eigenVector = getDataFromLine(band, 3+offset, true);
  eigenStruct.interpolation = new vahine::VahineInterpol<InternalPixelType>(xVector.size() );
  eigenStruct.interpolation->SetVx(xVector);
  eigenStruct.interpolation->SetVy(yVector);
  eigenStruct.interpolation->Update();
}

template<class TImage>
void VahineGrsirInverseFilter<TImage>::GenerateData(){
  vahineDebug("VahineGrsirInverseFilter : GenerateData");

  // check if we have the same number of physical parameter
  m_LearningParam->Update();
  unsigned int nbBand = m_LearningParam->GetNumberOfComponentsPerPixel();
  unsigned int nbParam = m_parameter.size();
  if( nbBand != nbParam ) {
    vahineDebug(
      "Error parameter bitset list must be have the same number of parameter than the learning parameter cube");
    vahineDebug("nbBand="<<nbBand<<" nbParam="<<nbParam);
    throw GrsirInverseException();
    }

  ItParameterList itParam = m_parameter.begin();
  // band number start with index 1
  unsigned int bandNumber = 1;
  BandPointer  currentLearningBand;
  while(itParam != m_parameter.end() ) {
    if(checkParameter(*itParam, INVERSE_MASK) ) {
      currentLearningBand = getLearningParameters(bandNumber);
      if( checkParameter(*itParam, ALLREGUL_MASK) ) {
        vahineDebug("InverseImage for all regularization parameter");
        unsigned int nbRegularization = getNbRegularization(currentLearningBand);
        vahineDebug("nbRegularization = "<<nbRegularization);
        for(unsigned int i=0; i<nbRegularization; ++i) {
          EigenStruct currentEigenStruct;
          UpdateEigenStruct(currentLearningBand, currentEigenStruct, i);
          InverseImage(currentEigenStruct);
          }
        } else {
        if(checkParameter(*itParam, CUSREGUL_MASK) ) {
          vahineDebug("InverseImage for custom regularization parameter");
          EigenStruct  customEigenStruct;
          unsigned int index = getIndexForCustRegul(*itParam);
          UpdateEigenStruct(currentLearningBand, customEigenStruct, index);
          InverseImage(customEigenStruct);
          } else {
          // default choice
          vahineDebug("InverseImage for best regularization parameter");
          EigenStruct bestEigenStruct;
          // index  = [1 ; n-1]
          unsigned int index = getIndexForBestRegul(currentLearningBand);
          UpdateEigenStruct(currentLearningBand, bestEigenStruct, index);
          InverseImage(bestEigenStruct);
          }
        }
      }
    ++bandNumber;
    ++itParam;
    }    // end while
         // convert image band list to vector image
  ImageListToVectorImageFilterPointer imageBandList2VectorImage = ImageListToVectorImageFilterType::New();
  imageBandList2VectorImage->SetInput(m_ResultList);
  vahineDebug("image size = "<<m_ResultList->Size() );
  imageBandList2VectorImage->Update();
  vahineDebug("get ouput");
  this->GraftOutput(imageBandList2VectorImage->GetOutput() );
}

/**
 * inverse method for one hyperspectral image and eigenStructure
 */
template<class TImage>
void VahineGrsirInverseFilter<TImage>::InverseImage(EigenStruct eigenStruct){
  vahineDebug("InverseImage");
  VahineInversePointer m_Inverse = VahineInverseType::New();
  m_Inverse->SetInput(this->GetInput() );
  m_Inverse->SetGrsirAxe(eigenStruct.eigenVector);
  m_Inverse->SetInterpolation(eigenStruct.interpolation);
  m_ResultList->PushBack(m_Inverse->GetOutput() );
  m_Inverse->Update();
}

} // end namespace otb

#endif