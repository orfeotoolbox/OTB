/*
 * GrsirLearningFilter.txx
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

#ifndef __VAHINEGRSIRLEARNINGFILTER_TXX
#define __VAHINEGRSIRLEARNINGFILTER_TXX

#include "GrsirLearningFilter.h"

namespace otb {

template<class TImage>
VahineGrsirLearningFilter<TImage>::VahineGrsirLearningFilter(){
  m_Epsilon = 1E-6;
  m_SaveOnlyBestRegulParam = false;
  m_NumberOfEigenVector = 1;   // several eigen vector not completly implemented
                               // !
  /** default regul parameter is range [1E-15, 1E-2]
   */
  m_RegulParameters = new RegulVectorType();
  for(int i=-15; i<-2; ++i) {
    m_RegulParameters->push_back(pow(10.0,i) );
    }
  m_CenteringFilter = CenteringFilterType::New();
  m_CenteringVectorFilter = PerBandVectorImageFilterType::New();
  m_CenteringVectorFilter->SetFilter(m_CenteringFilter);
  m_ImageBandList = ImageBandListType::New();
  m_XmlLog = new XmlLog("VahineGrsirLearningFilter");
}

template<class TImage>
void VahineGrsirLearningFilter<TImage>::SetRegulParameters(RegulVectorPointer p){
  delete m_RegulParameters;
  m_RegulParameters = p;
}

/**
 * X size is the number of component of one eigen vector : it's the number of band of
 * a learning database image
 * Y size is the sum of :
 *     one : regularization parameters list
 *     one : Xvalues of functionnal
 *     one : Yvalues of functionnal
 *     the number of eigen vector requested
 * multiply by the number of regul parameters requested, one if only the best is requested
 */
template<class TImage>
void VahineGrsirLearningFilter<TImage>::GenerateOutputInformation(){
  vahineDebug("VahineGrsirLearningFilter::GenerateOutputInformation");
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  ImageConstPointer inputPtr  = this->GetInput();
  ImagePointer      outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr ) {
    return;
    }

  typename TImage::SizeType outputSize;
  m_LearningData->UpdateOutputInformation();
  outputSize[0] = m_LearningData->GetNumberOfComponentsPerPixel();
  outputSize[1] = 1+(2+m_NumberOfEigenVector)*m_RegulParameters->size();
  vahineDebug("outputSize "<<outputSize<<","<<m_NumberOfEigenVector<<","<<m_RegulParameters->size() );
  if( m_SaveOnlyBestRegulParam ) {
    outputSize[1] = 3+m_NumberOfEigenVector;
    }

  m_OutputLargestPossibleRegion.SetSize( outputSize );
  outputPtr->SetLargestPossibleRegion( m_OutputLargestPossibleRegion );
  unsigned int nParam = inputPtr->GetNumberOfComponentsPerPixel();

  // paramBand not set then all param are calculated [1, 2, ...]
  if(m_LearningParamBand.empty() ) {
    vahineDebug("learningParamBand is not set ! Using all bands.");
    for(unsigned int i=0; i<nParam; ++i) {
      m_LearningParamBand.push_back(i+1);
      }
    }
  unsigned int numberOfBands = m_LearningParamBand.size();
  vahineDebug("Output number of bands = "<<numberOfBands);
  outputPtr->SetNumberOfComponentsPerPixel( numberOfBands );
}

template<class TImage>
void VahineGrsirLearningFilter<TImage>::GenerateInputRequestedRegion(){
  vahineDebug("VahineGrsirLearningFilter::GenerateInputRequestedRegion");
  ImagePointer inputPtr =  const_cast<ImageType * >( this->GetInput() );
  typename ImageType::IndexType index = inputPtr->GetLargestPossibleRegion().GetIndex();
  typename ImageType::SizeType size;
  size.Fill(0);
  typename ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(index);
  inputPtr->SetRequestedRegion(region);
}

template<class TImage>
void VahineGrsirLearningFilter<TImage>::Slicing(ParameterListType parameterList,  ImagePointer image){
  unsigned int nObserv = image->GetLargestPossibleRegion().GetNumberOfPixels();

  // distribute value in several class

  vahineDebug("VahineGrsirFilter : distribute data into class param="<<parameterList.size()<<" observ="<<nObserv);
  ParameterListIteratorType itParameterList = parameterList.begin();
  ParameterListIteratorType itLastParameter = parameterList.begin();
  if(!m_SliceList.empty() ) {
    vahineDebug("**** ERROR Slice list not empty ****");
    throw GrsirLearningException();
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
  if(m_SliceList.size() <= 1) {
    vahineDebug("*** ERROR Slice list must be have more than one element ***");
    throw GrsirLearningException();
    }
}

template<class TImage>
void VahineGrsirLearningFilter<TImage>::MakeMean(unsigned int nObserv){
  m_SliceList.back().setMeanPixel();
  m_SliceList.back().setPropOfPixel(nObserv);
}

template<class TImage>
typename VahineGrsirLearningFilter<TImage>::SliceListType &
VahineGrsirLearningFilter<TImage>::GetSliceList(){
  return m_SliceList;
}

template<class TImage>
void VahineGrsirLearningFilter<TImage>::SetTikonovFilter(){
  m_TikonovFilter = TikonovFilterType::New();
  m_TikonovFilter->SetInput(m_LearningData);
  //m_LearningData->Update();
  m_TikonovFilter->UpdateCov();
  m_CovarianceMatrix = m_TikonovFilter->GetCovMatrix();
  VnlMatrixType m = m_CovarianceMatrix.GetVnlMatrix();
  for(int i=0; i<1; i++) {
    for(int j=0; j<10; j++) {
      vahineDebug("cov = "<<m.get(i,j) );
      }
    }
}

template<class TImage>
void VahineGrsirLearningFilter<TImage>::SetXmlLog(XmlLogPointer xmlLog){
  delete m_XmlLog;
  m_XmlLog = xmlLog;
}

template<class TImage>
void VahineGrsirLearningFilter<TImage>::UpdateTikonovFilter(double regulParam){
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

template<class TImage>
void VahineGrsirLearningFilter<TImage>::GenerateData(){
  vahineDebug("VahineGrsirLearningFilter : GenerateData");
  m_XmlLog->CreateElement("filter-grsir");

  SetTikonovFilter();

  // centering data
  m_CenteringVectorFilter->SetInput(m_LearningData);
  m_CenteringVectorFilter->Update();
  m_LearningCenteringImage = m_CenteringVectorFilter->GetOutput();
  unsigned int numberOfBand = m_LearningCenteringImage->GetNumberOfComponentsPerPixel();
  unsigned int nObserv = m_LearningCenteringImage->GetLargestPossibleRegion().GetNumberOfPixels();

  // Extract the correct parameter band
  // parameter is input image
  typename ExtractChannelType::Pointer extractParamBand = ExtractChannelType::New();
  extractParamBand->SetExtractionRegion(this->GetInput()->GetLargestPossibleRegion() );
  extractParamBand->SetInput(this->GetInput() );

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
    Learning(numberOfBand, nObserv);
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

  // for debugging
  /*
  ConstVectorIteratorType itt(m_OutputImage, m_OutputImage->GetRequestedRegion());
  int h = 1;
  for(itt.GoToBegin();!itt.IsAtEnd(); ++itt){
    if(h>=120 && h<=128){
      vahineDebug("OutputImage ="<<itt.Get());
    }
    h++;
  }
  */

  this->GraftOutput(m_OutputImage);
}

template<class TImage>
void VahineGrsirLearningFilter<TImage>::PushBackLearningBand(unsigned int band){
  vahineDebug("learned band number "<<band);
  m_LearningParamBand.push_back(band);
}

/**
 * Sortering Y parameters
 * Calculate the best eigen vector and apply it to input image
 * for inversion
 */
template<class TImage>
void VahineGrsirLearningFilter<TImage>::Learning(unsigned int numberOfBand, unsigned int nObserv){

  // Sortering Y !!
  vahineDebug("VahineGrsirFilter : Sortering Y");
  vahineDebug("Number of band learning = "<<numberOfBand);
  vahineDebug("Number of observation learning = "<<nObserv);
  m_Parameters->Update();
  vahineDebug("Number of parameter values = "<<m_Parameters->GetLargestPossibleRegion().GetNumberOfPixels() );

  ConstBandIteratorType itParameters( m_Parameters, m_Parameters->GetLargestPossibleRegion() );
  ParameterListType     parameterList;

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
  RegulVectorType::iterator itRegulParam = m_RegulParameters->begin();
  while( itRegulParam != m_RegulParameters->end() ) {
    // set regularization matrix
    double regul = *itRegulParam;
    UpdateTikonovFilter(regul);
    EigenStruct currentEigenStruct;
    // ********************** ESTIMATE EDR CALL**************************
    EstimateEDR(gamma, numberOfBand, currentEigenStruct);
    vahineDebug("Regul = "<<regul<<" Nrmse = "<<currentEigenStruct.nrmse);
    nrmseVector.push_back(currentEigenStruct.nrmse);
    if(!m_SaveOnlyBestRegulParam) {
      eigenStructVector.push_back(currentEigenStruct);
      }
    if(itRegulParam == m_RegulParameters->begin() ) {
      // first
      bestEigenStruct = currentEigenStruct;
      regulMinNrmse = regul;
      regulIndex = itRegulParam-m_RegulParameters->begin();
      } else {
      if(bestEigenStruct.nrmse>currentEigenStruct.nrmse) {
        bestEigenStruct = currentEigenStruct;
        regulMinNrmse = regul;
        regulIndex = itRegulParam-m_RegulParameters->begin();
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

  /*** Output fonctionnal interpolation data ***/

  BandPointer pLearnedFunctionnal = BandType::New();
  pLearnedFunctionnal->SetRegions(m_OutputLargestPossibleRegion);
  pLearnedFunctionnal->Allocate();
  BandLinearIterator itLearnedFunctionnal(pLearnedFunctionnal, pLearnedFunctionnal->GetRequestedRegion() );
  itLearnedFunctionnal.SetDirection(0);   // x dimension

  AddRegularizationParameters(itLearnedFunctionnal, regulIndex, m_RegulParameters);

  if(m_SaveOnlyBestRegulParam) {
    vahineDebug("Functionnal for best regularization parameter");
    AddLearnedFunctionnal(bestEigenStruct, itLearnedFunctionnal);
    } else {
    vahineDebug("Functionnal for all regularization parameter");
    typename std::vector<EigenStruct>::iterator it = eigenStructVector.begin();
    while(it != eigenStructVector.end() ) {
      AddLearnedFunctionnal(*it, itLearnedFunctionnal);
      ++it;
      }
    }

  m_ImageBandList->PushBack(pLearnedFunctionnal);

  // clear list of slice for next parameter @see Slicing
  m_SliceList.clear();
}

/**
 * this method set several regularization param
 * line is list of regularization params
 * first value is optimal value
 */
template<class TImage>
void VahineGrsirLearningFilter<TImage>::AddRegularizationParameters(BandLinearIterator &itLearnedFunctionnal,
                                                                    unsigned int bestRegul,
                                                                    RegulVectorPointer regulVector){
  itLearnedFunctionnal.GoToBegin();
  itLearnedFunctionnal.Set(m_RegulParameters->size() );
  ++itLearnedFunctionnal;
  itLearnedFunctionnal.Set(bestRegul);
  ++itLearnedFunctionnal;
  RegulVectorType::iterator itRegulParam = m_RegulParameters->begin();
  // regularization not use in inverse filter but we save the logarithm value
  while(itRegulParam != m_RegulParameters->end() ) {
    itLearnedFunctionnal.Set(static_cast<RealType>(log10(*itRegulParam) ) );
    ++itRegulParam;
    ++itLearnedFunctionnal;
    }
  CompleteLine(itLearnedFunctionnal);

}

/**
 * Add eigen vector and functionnal interpolation values to a band pointer for ouput
 */
template<class TImage>
void VahineGrsirLearningFilter<TImage>::AddLearnedFunctionnal(EigenStruct eigenStruct,
                                                              BandLinearIterator &itLearnedFunctionnal){
  itLearnedFunctionnal.NextLine();
  itLearnedFunctionnal.GoToBeginOfLine();
  VnlVectorType xvalues = eigenStruct.interpolation->GetVx();
  VnlVectorType yvalues = eigenStruct.interpolation->GetVy();
  // next line is xvalue
  vahineDebug("size="<<xvalues.size() );
  itLearnedFunctionnal.Set(static_cast<RealType>(xvalues.size() ) );
  ++itLearnedFunctionnal;
  for(int i=0; i<xvalues.size(); ++i) {
    RealType xval = static_cast<RealType>(xvalues[i]);
    itLearnedFunctionnal.Set(xval);
    vahineDebug("index="<<itLearnedFunctionnal.GetIndex()<<" xval="<<xval);
    ++itLearnedFunctionnal;
    }
  CompleteLine(itLearnedFunctionnal);
  itLearnedFunctionnal.NextLine();
  itLearnedFunctionnal.GoToBeginOfLine();
  // next line is yvalue
  itLearnedFunctionnal.Set(static_cast<RealType>(yvalues.size() ) );
  ++itLearnedFunctionnal;
  for(int i=0; i<yvalues.size(); ++i) {
    RealType yval = static_cast<RealType>(yvalues[i]);
    itLearnedFunctionnal.Set(yval);
    vahineDebug("index="<<itLearnedFunctionnal.GetIndex()<<" yval="<<yval);
    ++itLearnedFunctionnal;
    }
  CompleteLine(itLearnedFunctionnal);
  itLearnedFunctionnal.NextLine();
  itLearnedFunctionnal.GoToBeginOfLine();
  // next line is best eigen vector (first eigen vector)
  for(int i=0; i<eigenStruct.eigenVector.size(); ++i) {
    itLearnedFunctionnal.Set(eigenStruct.eigenVector[i]);
    ++itLearnedFunctionnal;
    }
}

template<class TImage>
void VahineGrsirLearningFilter<TImage>::EstimateEDR(VnlMatrixType &gamma, unsigned int numberOfBand,
                                                    EigenStruct &eigenStruct){
  // calculate eigen vector
  CalculateEigenVector(gamma, eigenStruct);

  vahineDebug("VahineGrsirFilter : projection of learning data");

  //TODO refactor this two loop
  // project learning value to first eigen vector
  ConstImageIteratorType itLearningData(m_LearningData, m_LearningData->GetLargestPossibleRegion() );
  m_LearningData->Update();
  ProjPointer learningDataProj = ProjType::New();
  learningDataProj->SetRegions( m_LearningData->GetLargestPossibleRegion() );
  learningDataProj->Allocate();
  IteratorProjType itLearningDataProj(learningDataProj, learningDataProj->GetLargestPossibleRegion() );

  for(itLearningData.GoToBegin(), itLearningDataProj.GoToBegin(); !itLearningData.IsAtEnd();
      ++itLearningData, ++itLearningDataProj) {
    PixelType     currentPixel = itLearningData.Get();
    VnlVectorType pixel = ManagedTypes::ToManagedVariableLengthVector<RealType>(currentPixel);
    RealType      tmp = static_cast<RealType>(dot_product(pixel, eigenStruct.eigenVector) );
    itLearningDataProj.Set(tmp);
    }

  // project learning noise value to first eigen vector
  ConstImageIteratorType itLearningNoise(m_LearningNoiseData, m_LearningNoiseData->GetLargestPossibleRegion() );
  m_LearningNoiseData->Update();
  ProjPointer learningNoiseProj = ProjType::New();
  learningNoiseProj->SetRegions( m_LearningNoiseData->GetLargestPossibleRegion() );
  learningNoiseProj->Allocate();
  IteratorProjType itLearningNoiseProj(learningNoiseProj, learningNoiseProj->GetLargestPossibleRegion() );

  for(itLearningNoise.GoToBegin(), itLearningNoiseProj.GoToBegin(); !itLearningNoise.IsAtEnd();
      ++itLearningNoise, ++itLearningNoiseProj) {
    PixelType     currentPixel = itLearningNoise.Get();
    VnlVectorType pixel = ManagedTypes::ToManagedVariableLengthVector<RealType>(currentPixel);
    RealType      tmp = static_cast<RealType>(dot_product(pixel, eigenStruct.eigenVector) );
    itLearningNoiseProj.Set(tmp);
    }
  //end TODO refactor

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
    vahineDebug("data "<<dataVector[i]<<" param "<<paramVector[i]);
    ++itSlice;
    i++;
    }

  // interpolation for learning noise
  vahineDebug("interpolation for learning noise data "<<m_LearningNoiseData->GetLargestPossibleRegion() );
  ProjPointer learningNoiseEstim = ProjType::New();
  learningNoiseEstim->SetRegions( m_LearningNoiseData->GetLargestPossibleRegion() );
  learningNoiseEstim->Allocate();
  IteratorProjType itLearningNoiseEstim(learningNoiseEstim, learningNoiseEstim->GetLargestPossibleRegion() );

  eigenStruct.interpolation = new VahineInterpol<RealType>(dataVector.size() );
  eigenStruct.interpolation->SetVx(dataVector);
  eigenStruct.interpolation->SetVy(paramVector);
  eigenStruct.interpolation->Update();
  vahineDebug("after allocation of vahine interpol");
  for(itLearningNoiseEstim.GoToBegin(), itLearningNoiseProj.GoToBegin(); !itLearningNoiseProj.IsAtEnd();
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
template<class TImage>
void
VahineGrsirLearningFilter<TImage>::CalculateEigenVector(VnlMatrixType &gamma, EigenStruct &eigenStruct){
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
template<class TImage>
void VahineGrsirLearningFilter<TImage>::CalculateNRMSE(EigenStruct &eigenStruct,
                                                       IteratorProjType &itLearningNoiseEstim) {
  vahineDebug("calculate nmrse");
  typename StatsFilterType::Pointer statsFilter = StatsFilterType::New();
  statsFilter->SetInput(m_Parameters);
  statsFilter->Update();
  RealType              parameterMean = statsFilter->GetMean();
  RealType              diffSum = 0;
  RealType              diffParamSum = 0;
  ConstBandIteratorType itParameters( m_Parameters, m_Parameters->GetLargestPossibleRegion() );
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
template<class TImage>
double VahineGrsirLearningFilter<TImage>::CalculateSIRC(VnlMatrixType &gamma, VnlVectorType &axe){
  RealType sGamma = dot_product(axe, gamma*axe);
  RealType sSigma = dot_product(axe, m_CovarianceMatrix*axe);

  return sGamma/sSigma;
}

/**
   * complete with null value
   */
template<class TImage>
void VahineGrsirLearningFilter<TImage>::CompleteLine(BandLinearIterator &itLearnedFunctionnal){
  while(!itLearnedFunctionnal.IsAtEndOfLine() ) {
    itLearnedFunctionnal.Set(0);         //TODO replace with undefined value
    ++itLearnedFunctionnal;
    }
}

} // end namspace otb

#endif