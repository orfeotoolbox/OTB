/*
 * VCAFilter.txx
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

#ifndef __VAHINEVCAFILTER_TXX
#define __VAHINEVCAFILTER_TXX
#include "VcaFilter.h"

namespace otb {

template<class TImage>
VahineVCAFilter<TImage>::VahineVCAFilter() :
  m_SNR(NAN),
  m_MixingMatrix(),
  m_PurePixelsIndices(),
  m_PurePixelsIndex(),
  m_ProjectedData(),
  m_Y(),
  m_NumberEndMember(0) {
  m_RandomGenerator.reseed();
}

template<class TImage>
VahineVCAFilter<TImage>::~VahineVCAFilter(){
  /*m_Y.clear();
  m_ProjectedData.clear();
  m_MixingMatrix.clear();*/
}

template<class TImage>
void VahineVCAFilter<TImage>::GenerateOutputInformation(){
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  ImageConstPointer  inputPtr = this->GetInput();
  VectorImagePointer outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr ) {
    return;
    }
  unsigned int numberOfBands = inputPtr->GetNumberOfComponentsPerPixel();
  RegionType   imageRegion = this->GetInput()->GetLargestPossibleRegion();
  outputPtr->SetLargestPossibleRegion( imageRegion );
  outputPtr->SetNumberOfComponentsPerPixel( numberOfBands );
}

template<class TImage>
void VahineVCAFilter<TImage>::GenerateData(){

  RegionType imageRegion = this->GetInput()->GetLargestPossibleRegion();

  m_InputSize = imageRegion.GetSize();
  m_NumberOfBand = this->GetInput()->GetNumberOfComponentsPerPixel();

  if(m_NumberEndMember < 1 || m_NumberEndMember > m_NumberOfBand) {
    itkExceptionMacro("Endmember parameter must be integer between 1 and number of band of input image");
    }

  VectorImagePointer output = VectorImageType::New();

  // fill the data matrix from the input image
  // TODO add threading for this
  ConstIteratorType itInput( this->GetInput(), imageRegion );
  vahineDebug("input band="<<m_NumberOfBand<<" size[0]="<<m_InputSize[0]<<" size[1]="<<m_InputSize[1]);
  VnlMatrix data(m_NumberOfBand, m_InputSize[0]*m_InputSize[1]);

  unsigned int idx(0);
  for(itInput.GoToBegin(); !itInput.IsAtEnd(); ++itInput) {
    PixelType pixel = itInput.Get();
    VnlVector column(m_NumberOfBand);
    for(unsigned int j(0); j<m_NumberOfBand; ++j) {
      column[j] = static_cast<double>(pixel[j]);
      }
    data.set_column(idx, column);
    ++idx;
    }
  //vahineDebug("data = "<<data);

  // run calculation
  VCA(data);

  // convert projected data to image for output
  // TODO add threading for this
  vahineDebug("start output");
  //VectorImagePointer output = VectorImageType::New();
  output->SetRegions(imageRegion);
  output->SetNumberOfComponentsPerPixel(m_NumberOfBand);
  output->Allocate();
  IteratorType itOutput( output, imageRegion );
  idx = 0;
  for(itOutput.GoToBegin(); !itOutput.IsAtEnd(); ++itOutput) {
    VnlVector column = m_ProjectedData.get_column(idx);
    PixelType pixel = itOutput.Get();
    for(unsigned int j(0); j<m_NumberOfBand; ++j) {
      pixel[j] = static_cast<InternalPixelType>(column(j) );
      }
    itOutput.Set(pixel);
    ++idx;
    }
  vahineDebug("end output");
  this->GraftOutput(output);
}

template<class TImage>
void VahineVCAFilter<TImage>::FillRandom(VnlVector & w){
  VnlVector::iterator it = w.begin();

  while(it != w.end() ) {
    (*it) = m_RandomGenerator.drand32(1.0);
    ++it;
    }
}

template<class TImage>
void VahineVCAFilter<TImage>::VCA(VnlMatrix data){
  double snr, snr_th;

  //VnlMatrix Y;
  //m_PurePixelsIndices.SetSize(m_NumberEndMember);
  m_MixingMatrix.set_size(m_NumberOfBand, m_NumberEndMember);
  //m_PurePixelsIndices.resize(m_NumberEndMember, 0);

  if(isnan(m_SNR) ) {
    // SNR Estimates
    snr = SNR(data);
    }
  snr_th = TheoricalSNR(m_NumberEndMember);
  vahineDebug("Snr estimated or set = "<<snr);
  vahineDebug("Snr theorical = "<<snr_th);
  if(snr < snr_th) {
    vahineDebug("Select the projectiv projection");
    ProjectivProjection(data, m_ProjectedData);
    } else {
    vahineDebug("Select  projection to subspace");
    SubspaceProjection(data, m_ProjectedData);
    }

  // VCA Algorithm
  vahineDebug("start VCA algorithm");
  VnlMatrix A(m_NumberEndMember, m_NumberEndMember, 0);
  A(m_NumberEndMember-1, 0) = 1;
  VnlVector w(m_NumberEndMember);
  VnlVector f, fsquare, v, col;
  for(unsigned int i=0; i<m_NumberEndMember; i++) {
    FillRandom(w);
    vnl_svd<double> Asvd(A);
    VnlMatrix       Ainv = Asvd.pinverse();
    f = w - A*Ainv*w;
    fsquare = f.apply(&mysquare);
    f = f / sqrt(fsquare.sum() );
    v = f*m_Y;
    v=v.apply(&myabs);
    // search pure pixel
    unsigned int j = arg_max(v);
    col = m_Y.get_column(j);          // save Y columns values
    m_PurePixelsIndices.push_back(j); // save indice number
    A.set_column(i, col);
    }

  for(unsigned int i=0; i<m_NumberEndMember; ++i) {
    vahineDebug("pure pixel="<<m_PurePixelsIndices[i]);
    m_MixingMatrix.set_column(i, m_ProjectedData.get_column(m_PurePixelsIndices[i]) );
    }

  UpdateIndex();
  vahineDebug("end VCA method");
}

/**
 * deprecated with last vnl_vector : use vnl_vector::arg_max
 */
template<class TImage>
unsigned int VahineVCAFilter<TImage>::arg_max(const VnlVector &v){
  unsigned int n = v.size();

  if (n==0) return unsigned(-1);   // the minimum of an empty set is undefined
  double       tmp = v[0];
  unsigned int idx = 0;
  for (unsigned int i=1; i<n; ++i)
    if (v[i] > tmp) {
      tmp = v[i];
      idx = i;
      }
  return idx;
}

/**
 * update index of pure pixel vector with indice of each pure pixel
 * in matrix calculation
 */
template<class TImage>
void VahineVCAFilter<TImage>::UpdateIndex(){
  vahineDebug("Update index");
  //m_PurePixelsIndex.resize(2,IndexType());
  for(unsigned int i = 0; i < m_NumberEndMember; ++i) {
    unsigned int curIndice = m_PurePixelsIndices[i];
    IndexType    pixelIndex;
    pixelIndex[0] = curIndice % m_InputSize[0];
    pixelIndex[1] = static_cast<unsigned int>(curIndice/m_InputSize[0]);
    vahineDebug("pixelIndex "<<pixelIndex);
    m_PurePixelsIndex.push_back(pixelIndex);
    }
}

template<class TImage>
double VahineVCAFilter<TImage>::SNR(VnlMatrix & M){
  VnlMatrix Ud, Centered, Xp, MMean, Result;
  VnlVector mean;

  MeanCalculation(M, MMean, Centered, Result, mean);
  SingularValueAndVector(Result, Ud, m_NumberEndMember);   // compute the
                                                           // p-projection
                                                           // matrix
  //vahineDebug("Ud="<<Ud);
  //vahineDebug("Centered="<<Centered);
  Xp = Ud.transpose() * Centered;   // project thezeros mean data onto
                                    // p-subspace
  //vahineDebug("Xp="<<Xp);
  double snr = EstimateSNR(M, mean, Xp);
  //vahineDebug("snr = "<<snr);
  return snr;
}

/**
 * SNR Estimation
 * L number of bands
 * N number of pixels
 * p number of endmembers
 */
template<class TImage>
double VahineVCAFilter<TImage>::EstimateSNR(VnlMatrix & M, VnlVector mean, VnlMatrix & X){
  unsigned int      L = M.rows();
  unsigned int      N = M.cols();
  unsigned int      p = X.rows();
  double            px(0.0), py(0.0);
  VnlMatrixIterator itM = M.begin();

  while(itM != M.end() ) {
    py += pow( (*itM),2);
    ++itM;
    }
  py /= N;
  //TODO with apply() square and sum
  VnlMatrixIterator itX = X.begin();
  //X.apply(&mysquare);
  while(itX != X.end() ) {
    px += mysquare(*itX);
    ++itX;
    }
  px /= N;
  px += dot_product(mean,mean);
  //vahineDebug("px="<<px<<" py="<<py<<" p="<<p<<" L="<<L);
  return 10*log10( (px-p/static_cast<double>(L)*py)/(py-px) );
}

template<class TImage>
double VahineVCAFilter<TImage>::TheoricalSNR(unsigned int p){
  return 15+10*log10(p);
}

template<class TImage>
void VahineVCAFilter<TImage>::MeanCalculation(VnlMatrix const& M, VnlMatrix& MMean, VnlMatrix & Centered,
                                              VnlMatrix & Result,
                                              VnlVector & mean){
  unsigned int col = M.cols();
  unsigned int row = M.rows();

  // row mean calculation
  mean.set_size(row);
  for(unsigned int i = 0; i<row; ++i) {
    VnlVector currentRow = M.get_row(i);
    mean(i) = currentRow.mean();
    }
  MMean.set_size(row,col);
  for(unsigned int i = 0; i < col; ++i) {
    MMean.set_column(i,mean);
    }
  Centered.set_size(row, col);
  Centered = M;
  Centered -= MMean;
  Result = Centered * Centered.transpose();
  Result /= col;
}

template<class TImage>
void VahineVCAFilter<TImage>::ProjectivProjection(VnlMatrix const& M, VnlMatrix & Rp){
  VnlMatrix    Ud, Centered, Xp, MMean, Result;
  VnlVector    mean;
  unsigned int d = m_NumberEndMember - 1;
  unsigned int cols = M.cols();

  MeanCalculation(M, MMean, Centered, Result, mean);
  SingularValueAndVector(Result, Ud, d); // compute the p-projection matrix
  Xp = Ud.transpose() * Centered;        // project thezeros mean data onto
                                         // p-subspace
  Rp = Ud * Xp + MMean;                  // again in dimension L see matlab
                                         // script
  //double(*f)(double) = &square;
  VnlMatrix XpSquare = Xp.apply(&mysquare);   // Square root all elements
  //unsigned int cols = ProjASq.cols();
  VnlVector Sum(cols);
  for(unsigned int i =0; i<cols; ++i) {
    Sum(i) = (XpSquare.get_column(i) ).sum();
    }
  double c = sqrt(Sum.max_value() );
  m_Y.set_size(m_NumberEndMember,cols);
  for(unsigned int i=0; i<d; ++i) {
    m_Y.set_row(i,VnlVector(Xp.get_row(i) ) );
    }
  m_Y.set_row(d,VnlVector(cols,c) );
}

template<class TImage>
void VahineVCAFilter<TImage>::SubspaceProjection(VnlMatrix const& M, VnlMatrix & Rp){
  VnlMatrix    Result, Ud, Xp, MMean, Mult, Temp;
  unsigned int d = m_NumberEndMember;
  unsigned int cols = M.cols();
  unsigned int rows = M.rows();

  Result = M*M.transpose();
  Result /= cols;
  SingularValueAndVector(Result, Ud, d);   // compute the p-projection matrix
  Xp = Ud.transpose()*M;
  Rp = Ud * Xp;   // again in dimension L see matlab script
  // row mean calculation
  VnlVector mean(d);
  for(unsigned int i = 0; i<d; ++i) {
    VnlVector currentRow = Xp.get_row(i);
    mean(i) = currentRow.mean();
    }
  MMean.set_size(d,cols);
  for(unsigned int i = 0; i < cols; ++i) {
    MMean.set_column(i,mean);
    }
  // element by element multiplication
  Mult.set_size(d, cols);
  VnlMatrixIterator itMMean = MMean.begin();
  VnlMatrixIterator itXp = Xp.begin();
  VnlMatrixIterator itMult = Mult.begin();
  while(itMMean != MMean.end() ) {
    (*itMult) = (*itMMean) * (*itXp);
    ++itMMean;
    ++itXp;
    ++itMult;
    }
  VnlVector sums(cols);
  for(unsigned int i=0; i< cols; ++i) {
    sums(i) = Mult.get_column(i).sum();
    }
  Temp.set_size(d,cols);
  for(unsigned int i=0; i<d; ++i) {
    Temp.set_row(i,sums);
    }
  m_Y.set_size(d,cols);
  VnlMatrixIterator itTemp = Temp.begin();
  VnlMatrixIterator itY = m_Y.begin();
  itXp = Xp.begin();
  while(itTemp != Temp.end() ) {
    *itY = (*itXp)/(*itTemp);
    ++itXp;
    ++itTemp;
    ++itY;
    }
}

template<class TImage>
void VahineVCAFilter<TImage>::SingularValueAndVector(VnlMatrix const& M, VnlMatrix & Ud, unsigned int p){
  vnl_svd<double> svd(M);
  VnlMatrix       fullUd = svd.U();
  unsigned int    row = fullUd.rows();
  unsigned int    col = fullUd.cols();
  // keep p columns
  Ud.set_size(row,p);
  for(unsigned int i=0; i<p; ++i) {
    Ud.set_column(i,fullUd.get_column(i) );
    }
}

template<class TImage>
typename VahineVCAFilter<TImage>::IndexArray VahineVCAFilter<TImage>::GetPurePixelsIndex(){
  return m_PurePixelsIndex;
}

/**
 * Standard "PrintSelf" method
 */
template<class TImage>
void VahineVCAFilter<TImage>::PrintSelf(std::ostream& os, itk::Indent indent) const {
  Superclass::PrintSelf( os, indent );
}

} //end namespace otb

#endif