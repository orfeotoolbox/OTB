/*
 * MVSAFilter.txx
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

#ifndef __VAHINEELMVCAFILTER_TXX
#define __VAHINEELMVCAFILTER_TXX

#include "ElmVcaFilter.h"

namespace otb {

template<class TImage>
VahineElmVcaFilter<TImage>::VahineElmVcaFilter() :
  m_NbComponents(0) {

}

template<class TImage>
VahineElmVcaFilter<TImage>::~VahineElmVcaFilter(){

}

template<class TImage>
void VahineElmVcaFilter<TImage>::UpdateNumberOfComponents(){
  RegionType   imageRegion = this->GetInput()->GetLargestPossibleRegion();
  SizeType     size = imageRegion.GetSize();
  unsigned int NObserv = size[0]*size[1];

  m_NumberOfBand = this->GetInput()->GetNumberOfComponentsPerPixel();
  if(m_NbComponents == 0) {
    m_NbComponents = Demelange(NObserv);
    std::cout << "Using " << m_NbComponents << " endmembers" << std::endl;
    }
}

template<class TImage>
void VahineElmVcaFilter<TImage>::GenerateData(){
  RegionType imageRegion = this->GetInput()->GetLargestPossibleRegion();

  VCAFilterPointer vcaFilter = VCAFilterType::New();

  vcaFilter->SetInput(this->GetInput() );
  vcaFilter->SetNumberEndMember(m_NbComponents);
  vcaFilter->SetDebug(false);
  vcaFilter->Update();

  VnlMatrix mixingMatrix = vcaFilter->GetMixingMatrix();
  vahineDebug("mixingMatrix=\n");  //<<mixingMatrix);
  vnl_matrix_inverse<double> PseudoInv(mixingMatrix);
  VnlMatrix                  pinv = PseudoInv.pinverse();
  vahineDebug("pinv=\n");  //<<pinv);

  VectorImagePointer output = VectorImageType::New();
  output->SetRegions(imageRegion);
  output->SetNumberOfComponentsPerPixel(m_NbComponents);
  output->Allocate();

  // fill output
  ConstIteratorType itInput( this->GetInput(), imageRegion );
  IteratorType      itOutput( output, imageRegion );

  InternalVnlMatrix   realPinv(pinv.rows(), pinv.cols() );
  VnlMatrix::iterator itPinv = pinv.begin();
  typename InternalVnlMatrix::iterator itRealPinv = realPinv.begin();

  while(itPinv != pinv.end() ) {
    (*itRealPinv) = static_cast<InternalPixelType>(*itPinv);
    ++itRealPinv; ++itPinv;
    }

  for(itInput.GoToBegin(), itOutput.GoToBegin(); !itInput.IsAtEnd(); ++itInput, ++itOutput) {
    PixelType         pixelIn = itInput.Get();
    PixelType         pixelOut = itOutput.Get();
    InternalVnlVector pixIn(pixelIn.GetDataPointer(), pixelIn.GetNumberOfElements() );
    InternalVnlVector u = realPinv * pixIn;
    //vahineDebug("u = "<<u);
    // TODO optimize with mem copy ?
    for(unsigned int i=0; i<u.size(); ++i) {
      pixelOut[i] = u(i);
      }
    itOutput.Set(pixelOut);
    }
  vahineDebug("end output");
  this->GraftOutput(output);
}

/**
 * Compute the mean value of each spectral band
 */
template<class TImage>
void VahineElmVcaFilter<TImage>::SpectralMean(VnlVector & mean){
  mean.set_size(m_NumberOfBand);
  VectorImagePointer     image = const_cast<VectorImageType *>(this->GetInput() );
  DecomposeFilterPointer decomposer = DecomposeFilterType::New();
  decomposer->SetInput(image);
  ImageListPointer imageList = decomposer->GetOutput();
  imageList->Update();
  unsigned int i = 0;
  for (ConstListIteratorType it = imageList->Begin(); it!=imageList->End(); ++it) {
    // Compute the mean for each band with the statistical filter
    BandStatsFilterPointer meanFilter = BandStatsFilterType::New();
    meanFilter->SetInput(it.Get() );
    meanFilter->Update();
    mean(i) = static_cast<double>(meanFilter->GetMean() );
    ++i;
    }
  vahineDebug("mean = "<<mean);
}

template<class TImage>
typename VahineElmVcaFilter<TImage>::VnlMatrix VahineElmVcaFilter<TImage>::myCovariance(VectorImagePointer image,
                                                                                        unsigned int nbObserv){
  image->Update();
  typename SampleType::Pointer sample = SampleType::New();
  RegionType        imageRegion = this->GetInput()->GetLargestPossibleRegion();
  ConstIteratorType it( this->GetInput(), imageRegion );
  for(it.GoToBegin(); !it.IsAtEnd(); ++it) {
    sample->PushBack( it.Get() );
    //vahineDebug("it "<<it.Get() );
    }

  CovarianceAlgorithmPointer covarianceAlgorithm = CovarianceAlgorithmType::New();
  covarianceAlgorithm->SetInputSample(sample);
  covarianceAlgorithm->SetMean(0);
  covarianceAlgorithm->Update();
  const itk::VariableSizeMatrix<double> *covarianceVSMat = covarianceAlgorithm->GetOutput();
  VnlMatrix                              covarianceMatrix = covarianceVSMat->GetVnlMatrix();
  typename CovarianceAlgorithmType::MeanType *mean = covarianceAlgorithm->GetMean();
  m_Mean.set_size(mean->size() );
  m_Mean.copy_in(mean->data_block() );
  return covarianceMatrix;
}

template<class TImage>
typename VahineElmVcaFilter<TImage>::VnlMatrix VahineElmVcaFilter<TImage>::streamingCovariance(VectorImagePointer image,
                                                                                               unsigned int nbObserv){
  image->Update();
  typename StreamingStatisticsVectorImageFilterType::Pointer covComputefilter =
    StreamingStatisticsVectorImageFilterType::New();
  covComputefilter->SetInput(image);
  covComputefilter->Update();
  VnlMatrix covarianceMatrix = (covComputefilter->GetCovariance() ).GetVnlMatrix();
  PixelType mean = covComputefilter->GetMean();
  m_Mean.set_size(mean.Size() );
  for(unsigned int i=0; i< mean.Size(); ++i) {
    m_Mean[i] = mean[i];
    }
  double regul = static_cast<double>(nbObserv)/(nbObserv-1);
  covarianceMatrix *= regul;
  return covarianceMatrix;
}

/**
 * Find the number of element by calculation of :
 * covariance and correlation matrix
 * take care of correlation calculation, the matrix is normalize
 * with the number of observations.
 */
template<class TImage>
unsigned int VahineElmVcaFilter<TImage>::Demelange(unsigned int nbObserv){
  std::time_t start = time(NULL);

  vahineDebug("statistic calculation start");
  VectorImagePointer image = const_cast<VectorImageType *>(this->GetInput() );
  VnlMatrix          covarianceMatrix;
  //covarianceMatrix = myCovariance(image, nbObserv);
  covarianceMatrix = streamingCovariance(image, nbObserv);
  //covarianceMatrix = threadedCovariance(image, nbObserv);
  std::time_t end = time(NULL);
  vahineDebug("statistic calculation ended");
  vahineDebug("covariance calculation : "<< end - start << " seconds.");

  vahineDebug("covariance matrix\n");  //<<covarianceMatrix);

  unsigned int dim = covarianceMatrix.cols();

  VnlMatrix meanProduct(dim, dim);
  for(unsigned int j=0; j<dim; ++j) {
    VnlVector currentColumn(dim, m_Mean[j]);
    meanProduct.set_column(j, currentColumn);
    }

  VnlMatrix correlationMatrix(dim, dim);
  for(unsigned int i=0; i<dim; ++i) {
    meanProduct.set_row(i,meanProduct.get_row(i)*m_Mean[i]);
    }
  vahineDebug("meanProduct\n");  //<<meanProduct);
  correlationMatrix = covarianceMatrix + meanProduct;
  vahineDebug("correlation matrix\n");  //<<correlationMatrix);

  vnl_symmetric_eigensystem<double> eigenK(covarianceMatrix);
  VnlVector                         eigenCovariance = eigenK.D.diagonal();
  vcl_sort(eigenCovariance.begin(),eigenCovariance.end() );
  vnl_symmetric_eigensystem<double> eigenR(correlationMatrix);
  VnlVector                         eigenCorrelation = eigenR.D.diagonal();
  vcl_sort(eigenCorrelation.begin(),eigenCorrelation.end() );
  eigenCorrelation.flip();
  eigenCovariance.flip();

  std::cout << "eigen covariance  = "  << eigenCovariance << std::endl;
  std::cout << "eigen correlation = " << eigenCorrelation << std::endl;
  VnlVector likeHood;
  LikelihoodLog(eigenCorrelation, eigenCovariance, nbObserv, likeHood);
  std::cout << "likeHood = " << likeHood << std::endl;
  LocalMaximum(likeHood);
  for(unsigned int i=0; i<m_MaxLikeHood.size(); ++i) {
    if(m_MaxLikeHood[i]>=2)
      return m_MaxLikeHood[i];
    }
  // if maxlikehood is empty, normally never in real case
  // we search maximum value
  // this is only for test with a small image
  double max = likeHood.max_value();
  for(unsigned int i=0; i<likeHood.size(); ++i) {
    if(abs(likeHood[i] - max) < m_RealLimits.epsilon() ) {
      return i;
      }
    }
  // if we return 0 it's a bug !
  // TODO throw exception
  return 0;
}

/**
 * save all local maximum index
 * min that the index start to 0
 */
template<class TImage>
void VahineElmVcaFilter<TImage>::LocalMaximum(VnlVector & likeHood){
  unsigned int n = likeHood.size()-1;
  VnlVector    df(n), fi(n), fii(n);

  fi.copy_in(likeHood.data_block() );
  fii.copy_in(likeHood.data_block()+1);
  df = fii-fi;
  for(unsigned int i=0; i<df.size()-1; ++i) {
    if(df(i)>=0 && df(i+1)<0) {
      m_MaxLikeHood.push_back(i+1);
      }
    }
}

/**
 * calculate the likelihodd function of eigen vector
 * and the logarithme of this function.
 */
template<class TImage>
void VahineElmVcaFilter<TImage>::LikelihoodLog(VnlVector & eigenR, VnlVector & eigenK, unsigned int NObserv,
                                               VnlVector & lH){
  unsigned int Ns = eigenR.size();

  lH.set_size(Ns);
  double coef = 2.0/NObserv;
  for(unsigned int i=0; i<Ns; ++i) {
    unsigned int Nl = Ns - i;
    VnlVector    r(Nl), k(Nl), ZSquare(Nl), Sigma(Nl), T(Nl);
    r.copy_in(&eigenR.data_block()[i]);
    k.copy_in(&eigenK.data_block()[i]);
    ZSquare = (r-k).apply(&mysquare);
    Sigma = r.apply(&mysquare) + k.apply(&mysquare);
    Sigma *= coef;
    VnlVector::iterator itSigma = Sigma.begin();
    VnlVector::iterator itZSquare = ZSquare.begin();
    VnlVector::iterator itT = T.begin();
    while(itSigma != Sigma.end() ) {
      *itT = (*itZSquare)/(*itSigma);
      itSigma++;
      itZSquare++;
      itT++;
      }
    Sigma=Sigma.apply(&log);
    lH(i) = -0.5*T.sum() - 0.5*Sigma.sum();
    }
  std::cout << "likelihood " << lH << std::endl;
}

template<class TImage>
typename VahineElmVcaFilter<TImage>::IntegerList VahineElmVcaFilter<TImage>::GetMaxLikeHood(){
  return m_MaxLikeHood;
}

/**
 * Standard "PrintSelf" method
 */
template<class TImage>
void VahineElmVcaFilter<TImage>::PrintSelf(std::ostream& os, itk::Indent indent) const {
  Superclass::PrintSelf( os, indent );
}

template<class TImage>
void VahineElmVcaFilter<TImage>::GenerateOutputInformation(){
  Superclass::GenerateOutputInformation();
  // get pointers to the input and output
  VectorImageConstPointer inputPtr = this->GetInput();
  VectorImagePointer      outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr ) {
    return;
    }
  // we need to compute the output spacing, the output image size, and the
  // output image start index
  const SizeType& inputSize = inputPtr->GetLargestPossibleRegion().GetSize();
  RegionType      outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize( inputSize );
  outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );
  UpdateNumberOfComponents();
  outputPtr->SetNumberOfComponentsPerPixel(m_NbComponents);
}

} // end namespace otb

#endif
