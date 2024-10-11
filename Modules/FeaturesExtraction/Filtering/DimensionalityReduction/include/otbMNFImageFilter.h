/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbMNFImageFilter_h
#define otbMNFImageFilter_h

#include "otbPCAImageFilter.h"


namespace otb
{

/** \class MNFImageFilter
 * \brief Performs a Maximum Noise Fraction analysis of a vector image.
 *
 * The internal structure of this filter is a filter-to-filter like structure.
 * The estimation of the covariance matrix is streamed
 *
 * The high pass filter which has to be used for the noise estimation is templated
 * for a better scalability.
 *
 * TODO? Use a 2nd input to give a noise image directly?
 *
 * \sa otbStreamingStatisticsVectorImageFilter
 * \sa PCAImageFiler
 *
 * \ingroup OTBDimensionalityReduction
 */
template <class TInputImage, class TOutputImage, class TNoiseImageFilter, Transform::TransformDirection TDirectionOfTransformation>
class ITK_EXPORT MNFImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef MNFImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(MNFImageFilter, ImageToImageFilter);

  /** Dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  typedef Transform::TransformDirection TransformDirectionEnumType;
  itkStaticConstMacro(DirectionOfTransformation, TransformDirectionEnumType, TDirectionOfTransformation);

  /** Template parameters typedefs */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  /** Filter types and related */
  typedef StreamingStatisticsVectorImageFilter<InputImageType> CovarianceEstimatorFilterType;
  typedef typename CovarianceEstimatorFilterType::Pointer      CovarianceEstimatorFilterPointerType;

  typedef typename CovarianceEstimatorFilterType::RealType         RealType;
  typedef typename CovarianceEstimatorFilterType::RealPixelType    VectorType;
  typedef typename CovarianceEstimatorFilterType::MatrixObjectType MatrixObjectType;
  typedef typename MatrixObjectType::ComponentType                 MatrixType;
  typedef typename MatrixType::InternalMatrixType                  InternalMatrixType;
  typedef typename InternalMatrixType::element_type                MatrixElementType;

  typedef MatrixImageFilter<InputImageType, OutputImageType> TransformFilterType;
  typedef typename TransformFilterType::Pointer TransformFilterPointerType;

  typedef TNoiseImageFilter                      NoiseImageFilterType;
  typedef typename NoiseImageFilterType::Pointer NoiseImageFilterPointerType;

  typedef NormalizeVectorImageFilter<InputImageType, OutputImageType> NormalizeFilterType;
  typedef typename NormalizeFilterType::Pointer NormalizeFilterPointerType;

  /**
   * Set/Get the number of required largest principal components.
   */
  itkGetMacro(NumberOfPrincipalComponentsRequired, unsigned int);
  itkSetMacro(NumberOfPrincipalComponentsRequired, unsigned int);

  itkGetConstMacro(Normalizer, NormalizeFilterType*);
  itkGetMacro(Normalizer, NormalizeFilterType*);
  itkGetMacro(NoiseCovarianceEstimator, CovarianceEstimatorFilterType*);
  itkGetMacro(Transformer, TransformFilterType*);
  itkGetMacro(NoiseImageFilter, NoiseImageFilterType*);

  /** Normalization only impact the use of variance. The data is always centered */
  itkGetMacro(UseNormalization, bool);
  itkSetMacro(UseNormalization, bool);

  itkGetConstMacro(MeanValues, VectorType);
  void SetMeanValues(const VectorType& vec)
  {
    m_MeanValues      = vec;
    m_GivenMeanValues = true;
  }

  itkGetConstMacro(StdDevValues, VectorType);
  void SetStdDevValues(const VectorType& vec)
  {
    m_StdDevValues      = vec;
    m_UseNormalization  = true;
    m_GivenStdDevValues = true;
  }

  itkGetConstMacro(CovarianceMatrix, MatrixType);
  void SetCovarianceMatrix(const MatrixType& cov)
  {
    m_CovarianceMatrix      = cov;
    m_GivenCovarianceMatrix = true;
  }

  itkGetConstMacro(NoiseCovarianceMatrix, MatrixType);
  void SetNoiseCovarianceMatrix(const MatrixType& mat)
  {
    m_NoiseCovarianceMatrix      = mat;
    m_GivenNoiseCovarianceMatrix = true;
  }

  itkGetConstMacro(TransformationMatrix, MatrixType);
  void SetTransformationMatrix(const MatrixType& transf, bool isForward = true)
  {
    m_TransformationMatrix          = transf;
    m_GivenTransformationMatrix     = true;
    m_IsTransformationMatrixForward = isForward;
  }

  void SetStatisticsUserIgnoredValue(RealType value)
  {
    /** User ignored value for the normalizer */
    m_Normalizer->GetCovarianceEstimator()->SetUserIgnoredValue(value);
    m_Normalizer->GetCovarianceEstimator()->SetIgnoreUserDefinedValue(true);
    /** User ignored value for the covariance estimator */
    m_CovarianceEstimator->SetUserIgnoredValue(value);
    m_CovarianceEstimator->SetIgnoreUserDefinedValue(true);
    /** User ignored value for the noise covariance estimator */
    m_NoiseCovarianceEstimator->SetUserIgnoredValue(value);
    m_NoiseCovarianceEstimator->SetIgnoreUserDefinedValue(true);
  }

  itkGetConstMacro(EigenValues, VectorType);

protected:
  MNFImageFilter();
  ~MNFImageFilter() override
  {
  }

  /** GenerateOutputInformation
   * Propagate vector length info and modify if needed
   * NumberOfPrincipalComponentsRequired
   *
   * In REVERSE mode, the covariance matrix or the transformation matrix
   * (which may not be square) has to be given,
   * otherwise, GenerateOutputInformation throws an itk::ExceptionObject
   */
  void GenerateOutputInformation() override;

  /** GenerateData
   * Through a filter of filter structure
   */
  void GenerateData() override;

  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  /** Internal methods */
  void         ForwardGenerateOutputInformation();
  void         ReverseGenerateOutputInformation();
  virtual void ForwardGenerateData();
  virtual void ReverseGenerateData();

  /** Specific functionality of MNF */
  virtual void GenerateTransformationMatrix();

  /** Internal attributes */
  unsigned int m_NumberOfPrincipalComponentsRequired;

  bool m_UseNormalization;
  bool m_GivenMeanValues;
  bool m_GivenStdDevValues;

  bool m_GivenCovarianceMatrix;
  bool m_GivenNoiseCovarianceMatrix;
  bool m_GivenTransformationMatrix;
  bool m_IsTransformationMatrixForward;

  VectorType m_MeanValues;
  VectorType m_StdDevValues;
  MatrixType m_CovarianceMatrix;
  MatrixType m_NoiseCovarianceMatrix;
  MatrixType m_TransformationMatrix;
  VectorType m_EigenValues;

  NormalizeFilterPointerType           m_Normalizer;
  NoiseImageFilterPointerType          m_NoiseImageFilter;
  CovarianceEstimatorFilterPointerType m_CovarianceEstimator;
  CovarianceEstimatorFilterPointerType m_NoiseCovarianceEstimator;
  TransformFilterPointerType           m_Transformer;

private:
  MNFImageFilter(const Self&); // not implemented
  void operator=(const Self&); // not implemented
};                             // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMNFImageFilter.hxx"
#endif


#endif
