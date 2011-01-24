/**
 * Grsir.txx
 *
 * Vahine Framework
 * Copyright (C) 2008, 2009 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */

#ifndef __VAHINEGRSIRFILTER_TXX
#define __VAHINEGRSIRFILTER_TXX

#include "GrsirFilter.h"

namespace otb {

template<class TInputImage, class TOutputImage>
VahineGrsirFilter<TInputImage, TOutputImage>::VahineGrsirFilter(){
  m_Epsilon = 0.000001;
  m_CenteringFilter = CenteringFilterType::New();
  m_CenteringVectorFilter =  PerBandVectorImageFilterType::New();
  m_CenteringVectorFilter->SetFilter(m_CenteringFilter);
  m_ImageBandList = ImageBandListType::New();
  m_SearchBestRegulParam = true;
  //m_ImageBandList->DisconnectPipeline();
  //m_Inverse = VahineInverseType::New();
}

template<class TInputImage, class TOutputImage>
void VahineGrsirFilter<TInputImage, TOutputImage>::Slicing(ParameterListType parameterList,  VectorImagePointer image){
  unsigned int nObserv = image->GetLargestPossibleRegion().GetNumberOfPixels();

  // distribute value in several class

  vahineDebug("VahineGrsirFilter : distribute data into class param="<<parameterList.size()<<" observ="<<nObserv);
  ParameterListIteratorType itParameterList = parameterList.begin();
  ParameterListIteratorType itLastParameter = parameterList.begin();
  if(!m_SliceList.empty() ) {
    vahineDebug("**** ERROR ****");
    throw GrsirException();
    }
  int i = 0;
  while(itParameterList != parameterList.end() ) {
    i++;
    BandIndexType index = (*itParameterList).second;
    PixelType     pixel = image->GetPixel(index);
    if(m_SliceList.empty() ) {
      // push into list the first slice
      m_SliceList.push_back(vahine::Slice<InternalPixelType>(index, pixel, itParameterList->first) );
      } else {
      if( (itParameterList->first - itLastParameter->first)<m_Epsilon) {
        //vahineDebug("value closer : same slice ... current =
        // "<<itParameterList->first<<" last = "<<itLastParameter->first);
        // sum current pixel value to the current slice
        try{
          m_SliceList.back().add(index, pixel, itParameterList->first);
          }catch(...) {
          vahineDebug("add element failed");
          }
        } else {
        //vahineDebug("current pixel is in next class");
        // the current pixel is in next class
        // create the last mean object
        MakeMean(nObserv);
        // create a new slice with current pixel
        //vahineDebug("create a new slice with current pixel");
        m_SliceList.push_back(vahine::Slice<InternalPixelType>(index, pixel, itParameterList->first) );
        }
      itLastParameter++;
      }
    itParameterList++;
    }
  if(!parameterList.empty() ) {
    // create the last mean object
    MakeMean(nObserv);
    }
}

template<class TInputImage, class TOutputImage>
void VahineGrsirFilter<TInputImage, TOutputImage>::MakeMean(unsigned int nObserv){
  m_SliceList.back().setMeanPixel();
  m_SliceList.back().setPropOfPixel(nObserv);
}

template<class TInputImage, class TOutputImage>
typename VahineGrsirFilter<TInputImage, TOutputImage>::SliceListType &
VahineGrsirFilter<TInputImage, TOutputImage>::GetSliceList(){
  return m_SliceList;
}

template<class TInputImage, class TOutputImage>
void VahineGrsirFilter<TInputImage, TOutputImage>::SetTikonovFilter(){
  m_TikonovFilter = TikonovFilterType::New();
  m_TikonovFilter->SetInput(m_Learning);
  m_TikonovFilter->UpdateCov();
  m_CovarianceMatrix = m_TikonovFilter->GetCovMatrix();
  VnlMatrixType m = m_CovarianceMatrix.GetVnlMatrix();
  for(int i=0; i<1; i++) {
    for(int j=0; j<10; j++) {
      vahineDebug("cov = "<<m.get(i,j) );
      }
    }
}

template<class TInputImage, class TOutputImage>
void VahineGrsirFilter<TInputImage, TOutputImage>::SetXmlLog(XmlLogPointer xmlLog){
  m_XmlLog = xmlLog;
}

template<class TInputImage, class TOutputImage>
void VahineGrsirFilter<TInputImage, TOutputImage>::UpdateTikonovFilter(double regulParam){
  m_TikonovFilter->SetRegularizationParam(regulParam);
  m_TikonovFilter->Update();
  m_RegularizationMatrix = m_TikonovFilter->GetTikonovMatrix();
  VnlMatrixType m = m_RegularizationMatrix.GetVnlMatrix();
  for(int i=0; i<1; i++) {
    for(int j=0; j<10; j++) {
      vahineDebug("tiko = "<<m.get(i,j) );
      }
    }
}

template<class TInputImage, class TOutputImage>
void VahineGrsirFilter<TInputImage, TOutputImage>::GenerateData(){
  vahineDebug("VahineGrsirFilter : GenerateData");
  m_XmlLog->CreateElement("filter-grsir");

  // centering data
  m_CenteringVectorFilter->SetInput(m_Learning);
  m_CenteringVectorFilter->Update();
  m_LearningCenteringImage = m_CenteringVectorFilter->GetOutput();
  unsigned int numberOfBand = m_LearningCenteringImage->GetNumberOfComponentsPerPixel();
  unsigned int nObserv = m_LearningCenteringImage->GetLargestPossibleRegion().GetNumberOfPixels();

  // Extract the correct parameter band
  typename ExtractChannelType::Pointer extractParamBand = ExtractChannelType::New();
  extractParamBand->SetExtractionRegion(m_LearningParam->GetLargestPossibleRegion() );
  extractParamBand->SetInput(m_LearningParam);

  // loop on parameter
  BandNumberListIterator it = m_LearningParamBand.begin();
  while(it != m_LearningParamBand.end() ) {
    vahineDebug("Inversion for parameter : "<<(*it) );
    extractParamBand->SetChannel(*it);
    m_Parameters = extractParamBand->GetOutput();
    extractParamBand->Update();

    vahineDebug("Imagebandlist before size ="<<m_ImageBandList->Size() );
    //******************* Inverse call *********************/
    m_XmlLog->CreateElement("optimization");
    m_XmlLog->CreateAttribute("parameter",boost::lexical_cast<std::string>( (*it) ) );
    Inverse(numberOfBand, nObserv);
    m_XmlLog->AppendToSup();
    //***************************************************/
    m_ImageBandList->Update();

    //TODO cleanup internal variable ?
    ++it;
    //break; // for debug one parameter
    }

  // convert image band list to vector image
  ImageListToVectorImageFilterPointer imageBandList2VectorImage = ImageListToVectorImageFilterType::New();
  m_ImageBandList->Update();
  imageBandList2VectorImage->SetInput(m_ImageBandList);
  vahineDebug("image size = "<<m_ImageBandList->Size() );
  imageBandList2VectorImage->UpdateOutputInformation();
  imageBandList2VectorImage->Update();
  vahineDebug("get ouput");
  m_OutputImage = imageBandList2VectorImage->GetOutput();

  ConstVectorIteratorType itt(m_OutputImage, m_OutputImage->GetRequestedRegion() );
  // for debugging
  int h = 1;
  for(itt.GoToBegin(); !itt.IsAtEnd(); ++itt) {
    if(h>=120 && h<=128) {
      vahineDebug("OutputImage ="<<itt.Get() );
      }
    h++;
    }

  this->GraftOutput(m_OutputImage);
}

template<class TInputImage, class TOutputImage>
void VahineGrsirFilter<TInputImage, TOutputImage>::PushBackLearningBand(unsigned int band){
  m_LearningParamBand.push_back(band);
}

/**
 * Sortering Y parameters
 * Calculate the best eigen vector and apply it to input image
 * for inversion
 */
template<class TInputImage, class TOutputImage>
void
VahineGrsirFilter<TInputImage, TOutputImage>::Inverse(unsigned int numberOfBand, unsigned int nObserv){

  // Sortering Y !!
  vahineDebug("VahineGrsirFilter : Sortering Y");
  vahineDebug("Number of band learning = "<<numberOfBand);
  vahineDebug("Number of observation learning = "<<nObserv);
  m_Parameters->Update();
  vahineDebug("Number of parameter values = "<<m_Parameters->GetLargestPossibleRegion().GetNumberOfPixels() );

  ConstIteratorType itParameters( m_Parameters, m_Parameters->GetLargestPossibleRegion() );
  ParameterListType parameterList;

  for(itParameters.GoToBegin(); !itParameters.IsAtEnd(); ++itParameters) {
    BandIndexType index = itParameters.GetIndex();
    typename BandType::PixelType value = itParameters.Get();
    //vahineDebug("VahineGrsirFilter : index = "<<index<<" param v = "<< value);
    ParameterPairType element = make_pair(value, index);
    parameterList.push_back(element);
    }

  // sort pair by Y value
  vahineDebug("VahineGrsirFilter : Sort pair by Y value");
  parameterList.sort(SortPairIncrease<ParameterPairType>() );

  Slicing(parameterList, m_LearningCenteringImage);

  // calculate the between slice mean covariance matrix
  vahineDebug("VahineGrsirFilter : Calculate slice mean");
  vahineDebug("VahineGrsirFilter : number of slice = "<<m_SliceList.size() );
  vahineDebug("VahineGrsirFilter : Gamma size = "<<numberOfBand<<"*"<<numberOfBand);
  VnlMatrixType         gamma = VnlMatrixType(numberOfBand, numberOfBand, 0);
  IteratorSliceListType itSlice = m_SliceList.begin();
  while(itSlice != m_SliceList.end() ) {
    VnlVectorType meanH = ManagedTypes::ToManagedVariableLengthVector<RealType>(itSlice->getMeanPixel() );
    //vahineDebug("VahineGrsirFilter : meanH = "<<meanH);
    //vahineDebug("propOfPixel = "<<itSlice->getPropOfPixel());
    gamma += itSlice->getPropOfPixel() * outer_product(meanH, meanH);
    ++itSlice;
    }

  // Grsir core : estimate GRSIR axes
  std::vector<double> nrmseVector;
  // eigenStructVEctor : used with no auto regularization option
  // all regularization and projection are used
  std::vector<EigenStruct>  eigenStructVector;
  double                    regulMinNrmse;
  int                       regulIndex;
  EigenStruct               bestEigenStruct;
  RegulVectorType::iterator itRegulParam = m_RegulParams->begin();
  while( itRegulParam != m_RegulParams->end() ) {
    // set regularization matrix
    double regul = *itRegulParam;
    UpdateTikonovFilter(regul);
    EigenStruct currentEigenStruct;
    // ********************** ESTIMATE EDR CALL**************************
    EstimateEDR(gamma, numberOfBand, currentEigenStruct);
    //vahineDebug("Regul = "<<regul<<" Nrmse = "<<currentEigenStruct.nrmse);
    nrmseVector.push_back(currentEigenStruct.nrmse);
    if(!m_SearchBestRegulParam) {
      eigenStructVector.push_back(currentEigenStruct);
      }
    if(itRegulParam == m_RegulParams->begin() ) {
      // first
      bestEigenStruct = currentEigenStruct;
      regulMinNrmse = regul;
      regulIndex = itRegulParam-m_RegulParams->begin();
      } else {
      if(bestEigenStruct.nrmse>currentEigenStruct.nrmse) {
        bestEigenStruct = currentEigenStruct;
        regulMinNrmse = regul;
        regulIndex = itRegulParam-m_RegulParams->begin();
        }
      }
    ++itRegulParam;
    }
  std::vector<double>::iterator it = nrmseVector.begin();
  while(it != nrmseVector.end() ) {
    vahineDebug("all nrmse = "<<*it);
    ++it;
    }

  vahineDebug("min nrmse is "<<bestEigenStruct.nrmse<<" regul value is "<<regulMinNrmse);
  //vahineDebug("gamma "<<gamma);
  double cgrsir = CalculateSIRC(gamma, bestEigenStruct.eigenVector);
  vahineDebug("cgrsir = "<<cgrsir);
  m_XmlLog->CreateElementTextApp("nrmse", boost::lexical_cast<std::string>(bestEigenStruct.nrmse) );
  m_XmlLog->CreateElement("regularisation");
  m_XmlLog->CreateAttribute("index",boost::lexical_cast<std::string>(regulIndex) );
  m_XmlLog->CreateTextNode(boost::lexical_cast<std::string>(regulMinNrmse) );
  m_XmlLog->AppendToSup();
  m_XmlLog->CreateElementTextApp("cgrsir",boost::lexical_cast<std::string>(cgrsir) );

  // ************************************************************************
  // ***********************         inverse image
  //         **************************
  // ************************************************************************
  InputImagePointer image = const_cast<InputImageType *>(this->GetInput() );

  if(m_SearchBestRegulParam) {
    vahineDebug("InverseImage for best regularization parameter");
    InverseImage(bestEigenStruct, image);
    } else {
    vahineDebug("InverseImage for all regularization parameter");
    typename std::vector<EigenStruct>::iterator it = eigenStructVector.begin();
    while(it != eigenStructVector.end() ) {
      vahineDebug("inverse...");
      InverseImage(*it, image);
      ++it;
      }
    }
  // clear list of slice for next parameter @see Slicing
  m_SliceList.clear();
}

template<class TInputImage, class TOutputImage>
void
VahineGrsirFilter<TInputImage, TOutputImage>::InverseImage(EigenStruct eigenStruct, InputImagePointer image){
  VahineInversePointer m_Inverse = VahineInverseType::New();

  m_Inverse->SetInput(image);
  //m_Inverse->SetReleaseDataFlag(true);
  //m_Inverse->SetReleaseDataBeforeUpdateFlag(true);
  m_Inverse->SetGrsirAxe(eigenStruct.eigenVector);
  m_Inverse->SetInterpolation(eigenStruct.interpolation);
  m_Inverse->UpdateOutputInformation();
  m_ImageBandList->PushBack(m_Inverse->GetOutput() );
  m_Inverse->Update();
  m_ImageBandList->Update();
}

template<class TInputImage, class TOutputImage>
void
VahineGrsirFilter<TInputImage, TOutputImage>::EstimateEDR(VnlMatrixType &gamma, unsigned int numberOfBand,
                                                          EigenStruct &eigenStruct){
  // calculate eigen vector
  CalculateEigenVector(gamma, eigenStruct);

  // project learning value to first eigen vector
  // project learning noise value to first eigen vector
  vahineDebug("VahineGrsirFilter : projection of learning data");
  ConstVectorIteratorType itLearningData(m_Learning, m_Learning->GetLargestPossibleRegion() );
  ConstVectorIteratorType itLearningNoise(m_Noise, m_Noise->GetLargestPossibleRegion() );

  ProjPointer learningDataProj = ProjType::New();
  learningDataProj->SetRegions( m_Learning->GetLargestPossibleRegion() );
  learningDataProj->Allocate();
  IteratorProjType itLearningDataProj(learningDataProj, learningDataProj->GetLargestPossibleRegion() );

  ProjPointer learningNoiseProj = ProjType::New();
  learningNoiseProj->SetRegions( m_Noise->GetLargestPossibleRegion() );
  learningNoiseProj->Allocate();
  IteratorProjType itLearningNoiseProj(learningNoiseProj, learningNoiseProj->GetLargestPossibleRegion() );

  for(itLearningData.GoToBegin(), itLearningDataProj.GoToBegin(); !itLearningData.IsAtEnd();
      ++itLearningData, ++itLearningDataProj) {
    PixelType     currentPixel = itLearningData.Get();
    VnlVectorType pixel = ManagedTypes::ToManagedVariableLengthVector<RealType>(currentPixel);
    RealType      tmp = static_cast<RealType>(dot_product(pixel, eigenStruct.eigenVector) );
    itLearningDataProj.Set(tmp);
    }

  for(itLearningNoise.GoToBegin(), itLearningNoiseProj.GoToBegin(); !itLearningNoise.IsAtEnd();
      ++itLearningNoise, ++itLearningNoiseProj) {
    PixelType     currentPixel = itLearningNoise.Get();
    VnlVectorType pixel = ManagedTypes::ToManagedVariableLengthVector<RealType>(currentPixel);
    RealType      tmp = static_cast<RealType>(dot_product(pixel, eigenStruct.eigenVector) );
    itLearningNoiseProj.Set(tmp);
    }

  // calculate the mean values of the current functionnal
  IteratorSliceListType itSlice = m_SliceList.begin();
  unsigned int          sizeOfInterpol = m_SliceList.size();
  while( itSlice != m_SliceList.end() ) {
    itSlice->calculateMean(learningDataProj);
    ++itSlice;
    }
  m_SliceList.sort();

  vahineDebug("get mean and param sizeOfInterpol = "<<sizeOfInterpol);
  VnlVectorType dataVector = VnlVectorType(sizeOfInterpol);
  VnlVectorType paramVector = VnlVectorType(sizeOfInterpol);
  unsigned int  i = 0;
  itSlice = m_SliceList.begin();
  while( itSlice != m_SliceList.end() ) {
    dataVector[i] = (*itSlice).getMeanData();
    paramVector[i] = (*itSlice).getMeanParam();
    //vahineDebug("data "<<dataVector[i]<<" param "<<paramVector[i]);
    ++itSlice;
    i++;
    }

  // interpolation for learning noise
  vahineDebug("interpolation for learning noise data");
  ProjPointer learningNoiseEstim = ProjType::New();
  learningNoiseEstim->SetRegions( m_Learning->GetLargestPossibleRegion() );
  learningNoiseEstim->Allocate();
  IteratorProjType itLearningNoiseEstim(learningNoiseEstim, learningNoiseEstim->GetLargestPossibleRegion() );

  eigenStruct.interpolation = new VahineInterpol<RealType>(dataVector.size() );
  eigenStruct.interpolation->SetVx(dataVector);
  eigenStruct.interpolation->SetVy(paramVector);
  eigenStruct.interpolation->Update();
  vahineDebug("after allocation of vahine interpol");
  for(itLearningNoiseEstim.GoToBegin(), itLearningNoiseProj.GoToBegin(); !itLearningNoiseEstim.IsAtEnd();
      ++itLearningNoiseEstim, ++itLearningNoiseProj) {
    RealType data = eigenStruct.interpolation->get(itLearningNoiseProj.Get() );
    itLearningNoiseEstim.Set(data);
    }
  // calculate nrmse
  CalculateNRMSE(eigenStruct, itLearningNoiseEstim);
}

/**
 * Calcul and sort the eigen vector of the gamma matrix
 * Sortering is in decrease order of eigen value
 */
template<class TInputImage, class TOutputImage>
void
VahineGrsirFilter<TInputImage, TOutputImage>::CalculateEigenVector(VnlMatrixType &gamma, EigenStruct &eigenStruct){
  EigenListType eigenList;

  // calculation  of eigenvector
  vahineDebug("VahineGrsirFilter : calculate eigenvector");
  // construct the eigenvector problem
  vnl_real_eigensystem eigen( m_RegularizationMatrix.GetVnlMatrix() * gamma );
  // extract eigenvector and ordering in decrease order
  //EigenListType eigenList;
  vnl_diag_matrix<vcl_complex<double> >::iterator it;
  for (it = eigen.D.begin(); it != eigen.D.end(); ++it) {
    VnlVectorType vector;
    vector.set_size( eigen.V.rows() );
    double value = (*it).real();
    for (int i=0; i < eigen.V.rows(); ++i) {
      vector(i) = eigen.V(i,it-eigen.D.begin() ).real();
      }
    EigenPairType currentEigenPair = std::make_pair(value, vector);
    eigenList.push_back(currentEigenPair);
    //vahineDebug("Eigen value = "<<currentEigenPair.first);
    }

  // sorting eigen Vector by decrease eigen value
  vahineDebug("VahineGrsirFilter : sortering eigen vector");
  eigenList.sort(SortPairDecrease<EigenPairType>() );
  eigenStruct.eigenVector =  eigenList.front().second;
  vahineDebug("FirstEigenValue :"<<eigenList.front().first);
  vahineDebug("FirstEigenVector : "<<eigenStruct.eigenVector);
}

/**
 * Normelize Root Min Square Error calculation
 */
template<class TInputImage, class TOutputImage>
void VahineGrsirFilter<TInputImage, TOutputImage>::CalculateNRMSE(EigenStruct &eigenStruct,
                                                                  IteratorProjType &itLearningNoiseEstim) {
  vahineDebug("calculate nmrse");
  typename StatsFilterType::Pointer statsFilter = StatsFilterType::New();
  statsFilter->SetInput(m_Parameters);
  statsFilter->Update();
  RealType          parameterMean = statsFilter->GetMean();
  RealType          diffSum = 0;
  RealType          diffParamSum = 0;
  ConstIteratorType itParameters( m_Parameters, m_Parameters->GetLargestPossibleRegion() );
  for(itParameters.GoToBegin(), itLearningNoiseEstim.GoToBegin();
      !itParameters.IsAtEnd();
      ++itParameters, ++itLearningNoiseEstim) {
    diffSum += pow(itParameters.Get() - itLearningNoiseEstim.Get(), 2);
    diffParamSum += pow(itParameters.Get() - parameterMean, 2);
    }
  eigenStruct.nrmse = vcl_sqrt(diffSum/diffParamSum);
}

/**
 * Calcul SIR criterion
 * ratio beetween the beetween-slices variance and intra-slice variance
 */
template<class TInputImage, class TOutputImage>
double VahineGrsirFilter<TInputImage, TOutputImage>::CalculateSIRC(VnlMatrixType &gamma, VnlVectorType &axe){
  RealType sGamma = dot_product(axe, gamma*axe);
  RealType sSigma = dot_product(axe, m_CovarianceMatrix*axe);

  return sGamma/sSigma;
}

} // end namspace otb

#endif