/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbMaximumAutocorrelationFactorImageFilter_h
#define otbMaximumAutocorrelationFactorImageFilter_h


#include "otbStreamingStatisticsVectorImageFilter.h"
#include "otbConcatenateVectorImageFilter.h"
#include "itkNumericTraits.h"

#include "vnl/vnl_vector.h"
#include "vnl/vnl_matrix.h"

namespace otb
{

/** \class MaximumAutocorrelationFactorImageFilter
 * \brief This filter implements the Maximum Autocorrelation Factor
 *
 * This filter implements the Maximum Autocorrelation Factor, based
 * on the following work:
 *
 * A. A. Nielsen, "Kernel maximum autocorrelation factor and minimum
 * noise fraction transformations," IEEE Transactions on Image
 * Processing, vol. 20, no. 3, pp. 612-624, (2011)
 *
 * Maximum Autocorrelation Factor can be considered as a spatial
 * extension of the PCA, in which new variates try to maximize
 * auto-correlation between neighboring pixels instead of
 * variance. Though the inverse transform can be computed, this filter
 * only provides the forward transform for now.
 *
 * The GetV() method allows retrieving the linear combinations used
 * to generate new variates, and the GetAutoCorrelation() method
 * allows retrieving the auto-correlation associated to each variate.
 *
 * This filter has been implemented from the Matlab code kindly made
 * available by the authors here:
 * http://www2.imm.dtu.dk/~aa/software.html
 *
 * this filter have been validated by comparing the output image to
 * the output produced by the Matlab code, and the reference image
 * for testing has been generated from the Matlab code using Octave.

 * \ingroup Multithreaded, Streamed
 * \sa otbPCAImageFilter
 *
 * \ingroup OTBDimensionalityReduction
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT MaximumAutocorrelationFactorImageFilter
  : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef MaximumAutocorrelationFactorImageFilter             Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage>  Superclass;
  typedef itk::SmartPointer<Self>                             Pointer;
  typedef itk::SmartPointer<const Self>                       ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MaximumAutocorrelationFactorImageFilter, ImageToImageFilter);

  /** Some convenient typedefs. */
  typedef TInputImage                                             InputImageType;
  typedef typename InputImageType::Pointer                        InputImagePointer;
  typedef typename InputImageType::ConstPointer                   InputImageConstPointer;
  typedef typename InputImageType::RegionType                     InputImageRegionType;
  typedef typename InputImageRegionType::SizeType                 InputImageSizeType;
  typedef typename InputImageRegionType::IndexType                InputImageIndexType;
  typedef typename InputImageType::PixelType                      InputImagePixelType;
  typedef typename InputImageType::ValueType                      InputImageValueType;
  typedef TOutputImage                                            OutputImageType;
  typedef typename OutputImageType::Pointer                       OutputImagePointer;
  typedef typename OutputImageType::RegionType                    OutputImageRegionType;
  typedef typename OutputImageType::PixelType                     OutputImagePixelType;
  typedef typename InputImageType::InternalPixelType              InputInternalPixelType;
  typedef typename
    itk::NumericTraits<InputInternalPixelType>::RealType          InternalPixelType;

  typedef VectorImage<InternalPixelType, 2>                        InternalImageType;


  /** Internal filters types */
  typedef StreamingStatisticsVectorImageFilter<InternalImageType> CovarianceEstimatorType;
  typedef typename CovarianceEstimatorType::Pointer               CovarianceEstimatorPointer;
  typedef typename CovarianceEstimatorType::MatrixObjectType      MatrixObjectType;
  typedef typename MatrixObjectType::ComponentType                MatrixType;
  typedef typename MatrixType::InternalMatrixType                 InternalMatrixType;
  typedef typename CovarianceEstimatorType::RealPixelType         VectorType;

  typedef typename VectorType::ValueType                          RealType;
  typedef vnl_vector<RealType>                                    VnlVectorType;
  typedef vnl_matrix<RealType>                                    VnlMatrixType;

  /** Get the linear combination used to compute Maf */
  itkGetMacro(V, VnlMatrixType);

  /** Get the auto-correlation associated with each Maf */
  itkGetMacro(AutoCorrelation, VnlVectorType);

  /** Get the covariance estimator for image (use for progress
   * reporting purposes) */
  itkGetObjectMacro(CovarianceEstimator, CovarianceEstimatorType);

  /** Get the covariance estimator for horizontal autocorrelation (use
   * for progress reporting purposes) */
  itkGetObjectMacro(CovarianceEstimatorH, CovarianceEstimatorType);

  /** Get the covariance estimator for vertical autocorrelation (use
   * for progress reporting purposes) */
  itkGetObjectMacro(CovarianceEstimatorV, CovarianceEstimatorType);

protected:
  MaximumAutocorrelationFactorImageFilter();
  ~MaximumAutocorrelationFactorImageFilter() override {}

  void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, itk::ThreadIdType threadId) override;

  void GenerateOutputInformation() override;

private:
  MaximumAutocorrelationFactorImageFilter(const Self &) = delete;
  void operator =(const Self&) = delete;

  /** The covariance estimator for the image */
  CovarianceEstimatorPointer m_CovarianceEstimator;

  /** The covariance estimator for auto-correlation in horizontal
   *  direction */
  CovarianceEstimatorPointer m_CovarianceEstimatorH;

  /** The covariance estimator for auto-correlation in vertical
   *  direction */
  CovarianceEstimatorPointer m_CovarianceEstimatorV;

  /** The linear combination for Maf */
  VnlMatrixType m_V;

  /** The mean used to center data before computing Maf */
  VnlVectorType m_Mean;

  /** The auto-correlation associated with each Maf */
  VnlVectorType m_AutoCorrelation;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMaximumAutocorrelationFactorImageFilter.hxx"
#endif

#endif
