/*
 * CovPixelSelectFilter.txx
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
#ifndef __VAHINEFILTRAGEFILTER_TXX
#define __VAHINEFILTRAGEFILTER_TXX
#include "CovPixelSelectFilter.h"
#include "limits"

namespace otb {

template<class TInputImage, class TOutputImage>
VahineCovPixelSelectFilter<TInputImage,TOutputImage>::VahineCovPixelSelectFilter() :
  m_NumberOfPCAAxes(0),
  m_Explanation(95.0) {
  vahineDebug("VahineCovPixelSelectFilter constructor");
}

template<class TInputImage, class TOutputImage>
VahineCovPixelSelectFilter<TInputImage,TOutputImage>::~VahineCovPixelSelectFilter(){
  /*if(m_PCAReferenceProjections != NULL){
      delete m_PCAReferenceProjections;
  }
delete m_PCAInputProjections;
delete m_Distances;
delete m_PCAValues;*/
}

/**
 * vnl_symmetric_eigensystem :
 * The columns of V are the eigenvectors, sorted by increasing eigenvalue,
 * from most negative to most positive.
 */
template<class TInputImage, class TOutputImage>
void VahineCovPixelSelectFilter<TInputImage, TOutputImage>::CalculatePCAVectors(){
  vahineDebug("CalculatePCAVectors");
  StatsFilterPointer statsFilter = StatsFilterType::New();
  statsFilter->SetInput(m_ReferenceImage);
  statsFilter->Update();
  MatrixType   covarianceMatrix = statsFilter->GetCovariance();
  unsigned int N = covarianceMatrix.Cols();
  covarianceMatrix *= static_cast<double>(N)/(N-1);
  vnl_symmetric_eigensystem<RealType> eigen(covarianceMatrix.GetVnlMatrix() );
  m_PCAVectors = eigen.V;
  m_PCAValues = new ArrayType();
  (*m_PCAValues) = eigen.D.diagonal();
  double sum = static_cast<double>(m_PCAValues->sum() );
  (*m_PCAValues) *= 100.0/sum;
}

/**
 * Update the number of interresting PCA axes
 * for an explanation pourcentage
 */
template<class TInputImage, class TOutputImage>
void VahineCovPixelSelectFilter<TInputImage, TOutputImage>::UpdateNumberOfPCAAxes(){
  vahineDebug("UpdateNumberOfPCAAxes");
  double       explanationSum = 0.0;
  unsigned int numberOfVectors = 0;
  for(unsigned int i = m_PCAValues->size(); i>0; i--) {
    explanationSum += (*m_PCAValues)(i-1);
    numberOfVectors++;
    vahineDebug("### explanation "<<explanationSum<<" "<<m_Explanation);
    if(explanationSum >= m_Explanation) {
      vahineDebug("Break number of vector = "<<numberOfVectors);
      break;
      }
    }
  m_NumberOfPCAAxes = numberOfVectors;
}

template<class TInputImage, class TOutputImage>
void VahineCovPixelSelectFilter<TInputImage, TOutputImage>::CalculatePCAProjection(
  const unsigned int ImageNumberOfBands, ImagePointer image, ArrayMatrixType * matrix){
  vahineDebug("CalculatePCAProjection");
  MatrixType tempMatrix;
  RegionType region = image->GetLargestPossibleRegion();
  // size along X
  unsigned int cols = region.GetSize(0);
  // size along Y
  unsigned int rows = region.GetSize(1);
  vahineDebug("### size  "<<rows<<","<<cols);
  tempMatrix.SetSize(rows, cols);
  tempMatrix.Fill(itk::NumericTraits<InternalPixelType>::Zero);
  matrix->assign(m_NumberOfPCAAxes, tempMatrix);

  ImageRegionConstIterator imageIt(image, region);
  // number of total PCA axes
  //const unsigned int nbVectors = m_PCAVectors.Cols();
  unsigned int i = 0;
  for(imageIt.GoToBegin(); !imageIt.IsAtEnd(); ++imageIt) {
    PixelType                     pixel = imageIt.Get();
    const InternalPixelType *     buffer = pixel.GetDataPointer();
    vnl_vector<InternalPixelType> pix = vnl_vector<InternalPixelType>(buffer, ImageNumberOfBands);
    // convert from InternalPixelType to RealType
    ArrayType mypix( pix.size() );
    for ( unsigned int j=0; j<pix.size(); ++j ) {
      mypix[j] = static_cast<RealType>(pix[j]);
      }
    for(unsigned int n=0; n<m_NumberOfPCAAxes; n++) {
      //cout<<"###  "<<i<<" : "<<floor(i/cols)<<","<< i%cols<<endl;
      ArrayType currentPCAVector = m_PCAVectors.GetVnlMatrix().get_column(ImageNumberOfBands-1-n);
      //cout<<"# PCA vect "<<currentPCAVector<<endl;
      matrix->at(n) (static_cast<unsigned int>(floor(i/cols) ), i%cols) = dot_product(mypix, currentPCAVector);
      }
    i++;
    }

}

/**
 * Nearest neighbor search : euclidian distance
 * //TODO use a another algo like kd-tree for decrease time calculation
 */
template<class TInputImage, class TOutputImage>
void VahineCovPixelSelectFilter<TInputImage, TOutputImage>::CalculateNNS(){
  vahineDebug("CalculateNNS");
  m_Distances = new MatrixType();
  // size of input image
  unsigned int icols = m_PCAInputProjections->at(1).Cols();
  unsigned int irows = m_PCAInputProjections->at(1).Rows();
  m_Distances->SetSize(irows, icols);
  // size of reference image
  unsigned int                rcols = m_PCAReferenceProjections->at(1).Cols();
  unsigned int                rrows = m_PCAReferenceProjections->at(1).Rows();
  double                      inCoord, refCoord, sum, distance, curDistance;
  std::numeric_limits<double> doubleLimits;
  m_BoundDistance = ItkArrayType(2);
  m_BoundDistance[0] = doubleLimits.max();
  m_BoundDistance[1] = doubleLimits.min();
  for(unsigned int i=0; i<irows; ++i) {
    for(unsigned int j=0; j<icols; ++j) {
      (*m_Distances)(i,j) = doubleLimits.max();
      for(unsigned int k=0; k<rrows; ++k) {
        for(unsigned int h=0; h<rcols; ++h) {
          sum = 0.0;
          for(unsigned int axe=0; axe<m_NumberOfPCAAxes; ++axe) {
            inCoord = m_PCAInputProjections->at(axe) (i,j);
            refCoord = m_PCAReferenceProjections->at(axe) (k,h);
            sum += pow(inCoord-refCoord,2);
            }
          curDistance = pow(sum, 0.5);
          if(curDistance < (*m_Distances)(i,j) ) {
            (*m_Distances)(i,j) = curDistance;
            if(m_BoundDistance[0]> curDistance) {
              m_BoundDistance[0] = curDistance;
              }
            if(m_BoundDistance[1]<curDistance) {
              m_BoundDistance[1] = curDistance;
              }
            }
          }
        }
      }
    }
}

template<class TInputImage, class TOutputImage>
void VahineCovPixelSelectFilter<TInputImage, TOutputImage>::ClassificationEM(){
  vahineDebug("ClassificationEM");
  unsigned int numberOfClasses = 2;
  m_GMMMean = ItkArrayType();
  m_GMMCovariance = ItkArrayType();
  m_GMMProportion = ItkArrayType();
  m_GMMMean.SetSize(numberOfClasses);
  m_GMMCovariance.SetSize(numberOfClasses);
  m_GMMProportion.SetSize(numberOfClasses);

  SampleType::Pointer sample = SampleType::New();
  sample->SetMeasurementVectorSize(1);
  MeasurementVectorType mv;
  unsigned int          cols = m_Distances->Cols();
  unsigned int          rows = m_Distances->Rows();
  std::cout<<"rows = "<<rows<<" cols "<<cols<<std::endl;
  for(unsigned int i=0; i<rows;  ++i) {
    for(unsigned int j=0; j<cols; ++j) {
      mv[0] = (*m_Distances)(i,j);
      sample->PushBack(mv);
      }
    }

  std::vector<ItkArrayType> initialParameters(numberOfClasses);
  double                    delta = (m_BoundDistance[1]-m_BoundDistance[0])/3;
  // parameters of first gaussian
  initialParameters[0] = ItkArrayType(2);
  initialParameters[0][0] = m_BoundDistance[0] + delta;
  initialParameters[0][1] = delta;
  // parameters of second gaussian
  initialParameters[1] = ItkArrayType(2);
  initialParameters[1][0] = m_BoundDistance[1] - delta;
  initialParameters[1][1] = delta;

  ItkArrayType initialProportions(numberOfClasses);

  EstimatorType::Pointer estimator = EstimatorType::New();
  estimator->SetSample(sample);
  estimator->SetMaximumIteration(1000);

  std::vector<ComponentType::Pointer> components;
  for(unsigned int i=0; i<numberOfClasses; ++i) {
    components.push_back(ComponentType::New() );
    (components[i])->SetSample(sample);
    (components[i])->SetParameters(initialParameters[i]);
    estimator->AddComponent( (ComponentType::Superclass*)( (components[i]).GetPointer() ) );
    initialProportions[i] = 1.0/numberOfClasses;
    }

  estimator->SetInitialProportions(initialProportions);
  estimator->Update();

  for(unsigned int i=0; i<numberOfClasses; ++i) {
    m_GMMMean[i] = (components[i])->GetFullParameters()[0];
    m_GMMCovariance[i] = (components[i])->GetFullParameters()[1];
    //std::cout<<" params "<<(components[i])->GetFullParameters()<<std::endl;
    //std::cout<<" prop "<<(*estimator->GetProportions())[i]<<std::endl;
    //std::cout<<" compo "<<(components[i])<<std::endl;
    }
  ComponentType::WeightArrayType *firstCompoWeight = (components[0])->GetWeights();
  m_DistanceWeight = ItkArrayType();
  m_DistanceWeight.SetSize(firstCompoWeight->GetSize() );
  m_DistanceWeight = (*firstCompoWeight);
}

template<class TInputImage, class TOutputImage>
void VahineCovPixelSelectFilter<TInputImage, TOutputImage>::WeightFilter(){
  vahineDebug("WeightFilter");
  // size along X
  unsigned int cols = this->GetInput()->GetLargestPossibleRegion().GetSize(0);
  // size along Y
  unsigned int rows = this->GetInput()->GetLargestPossibleRegion().GetSize(1);
  //std::cout<<"cols "<<cols<<" "<<"rows "<<rows<<endl;
  // create mask
  OutputImagePointer mask = OutputImageType::New();
  OutputIndexType    start;
  start[0] = 0;
  start[1] = 0;
  OutputSizeType size;
  size[0] = cols;
  size[1] = rows;
  OutputRegionType region;
  region.SetSize(size);
  region.SetIndex(start);
  mask->SetRegions(region);
  mask->Allocate();
  vahineDebug("weight size "<<m_DistanceWeight.Size() );
  vahineDebug("mask size "<<cols*rows);
  OutputRegionIterator   maskIt(mask, mask->GetLargestPossibleRegion() );
  ItkArrayType::iterator weightIt = m_DistanceWeight.begin();
  for(maskIt.GoToBegin(); !maskIt.IsAtEnd(); ++maskIt, ++weightIt) {
    if( (*weightIt)>0.5) { // we have two class
      maskIt.Set(static_cast<OutputPixelType>(1.0) );
      } else {
      maskIt.Set(static_cast<OutputPixelType>(0.0) );
      }

    }
  this->GraftOutput(mask);
  vahineDebug("end weightfilter");
}

template<class TInputImage, class TOutputImage>
void VahineCovPixelSelectFilter<TInputImage, TOutputImage>::GenerateData(){
  m_ReferenceImage->UpdateOutputInformation();
  const unsigned int ReferenceImageNumberOfBands = m_ReferenceImage->GetNumberOfComponentsPerPixel();
  const unsigned int InputNumberOfBands = this->GetInput()->GetNumberOfComponentsPerPixel();
  if( ReferenceImageNumberOfBands != InputNumberOfBands ) {
    itkExceptionMacro(
      "The number of bands of the reference image ("<<ReferenceImageNumberOfBands<<
      ") is different from the number of bands of the input image ("<<InputNumberOfBands<<"). ");
    }
  CalculatePCAVectors();
  UpdateNumberOfPCAAxes();
  m_PCAReferenceProjections = new ArrayMatrixType();
  vahineDebug("First PCA");
  m_ReferenceImage->Update();
  CalculatePCAProjection(ReferenceImageNumberOfBands, m_ReferenceImage, m_PCAReferenceProjections);
  m_PCAInputProjections = new ArrayMatrixType();
  vahineDebug("Second PCA");
  CalculatePCAProjection(ReferenceImageNumberOfBands,
                         const_cast<InputImageType *>(this->GetInput() ), m_PCAInputProjections);
  CalculateNNS();
  ClassificationEM();
  WeightFilter();
}

/**
 * Standard "PrintSelf" method
 */
template<class TInputImage, class TOutputImage>
void VahineCovPixelSelectFilter<TInputImage, TOutputImage>::PrintSelf(std::ostream& os, itk::Indent indent) const {
  Superclass::PrintSelf( os, indent );
}

} // end namespace otb

#endif