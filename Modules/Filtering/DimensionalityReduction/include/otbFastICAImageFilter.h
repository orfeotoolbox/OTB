/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbFastICAImageFilter_h
#define otbFastICAImageFilter_h

#include "itkImageToImageFilter.h"
#include "otbPCAImageFilter.h"
#include "otbFastICAInternalOptimizerVectorImageFilter.h"
#include <functional>

namespace otb
{
/** \class FastICAImageFilter
 * \brief Performs a Independent Component Analysis (ICA)
 *
 * This filter is an implementation of the stabilized fixed-point FastICA
 * algorithm described in [1] with a symetric decorrelation at each step.
 *
 * The contrast function and its derivative can be supplied to the filter as
 * lambda functions.
 *
 * [1] Fast and robust fixed-point algorithms for independent component analysis
 *
 * \sa PCAImageFilter
 *
 * \ingroup OTBDimensionalityReduction
 */
template <class TInputImage, class TOutputImage, Transform::TransformDirection TDirectionOfTransformation>
class ITK_EXPORT FastICAImageFilter : public itk::ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard typedefs */
  typedef FastICAImageFilter Self;
  typedef itk::ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(FastICAImageFilter, ImageToImageFilter);

  /** Dimension */
  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int, TOutputImage::ImageDimension);

  typedef Transform::TransformDirection TransformDirectionEnumType;
  itkStaticConstMacro(DirectionOfTransformation, TransformDirectionEnumType, TDirectionOfTransformation);

  /** typedefs */
  typedef TInputImage  InputImageType;
  typedef TOutputImage OutputImageType;

  typedef PCAImageFilter<InputImageType, OutputImageType, TDirectionOfTransformation> PCAFilterType;
  typedef typename PCAFilterType::Pointer PCAFilterPointerType;

  typedef typename PCAFilterType::RealType          RealType;
  typedef typename PCAFilterType::VectorType        VectorType;
  typedef typename PCAFilterType::MatrixType        MatrixType;
  typedef typename MatrixType::InternalMatrixType   InternalMatrixType;
  typedef typename InternalMatrixType::element_type MatrixElementType;

  typedef MatrixImageFilter<TInputImage, TOutputImage> TransformFilterType;
  typedef typename TransformFilterType::Pointer TransformFilterPointerType;

  typedef FastICAInternalOptimizerVectorImageFilter<InputImageType, InputImageType> InternalOptimizerType;
  typedef typename InternalOptimizerType::Pointer InternalOptimizerPointerType;

  typedef StreamingStatisticsVectorImageFilter<InputImageType> MeanEstimatorFilterType;
  typedef typename MeanEstimatorFilterType::Pointer            MeanEstimatorFilterPointerType;

  typedef std::function<double(double)> NonLinearityType;

  /**
   * Set/Get the number of required largest principal components.
   */
  itkGetMacro(NumberOfPrincipalComponentsRequired, unsigned int);
  itkSetMacro(NumberOfPrincipalComponentsRequired, unsigned int);

  itkGetConstMacro(PCAFilter, PCAFilterType*);
  itkGetMacro(PCAFilter, PCAFilterType*);

  itkGetConstMacro(TransformFilter, TransformFilterType*);
  itkGetMacro(TransformFilter, TransformFilterType*);

  VectorType GetMeanValues() const
  {
    return this->GetPCAFilter()->GetMeanValues();
  }
  void SetMeanValues(const VectorType& vec)
  {
    m_PCAFilter->SetMeanValues(vec);
  }

  VectorType GetStdDevValues() const
  {
    return this->GetPCAFilter()->GetStdDevValues();
  }
  void SetStdDevValues(const VectorType& vec)
  {
    m_PCAFilter->SetStdDevValues(vec);
  }

  MatrixType GetPCATransformationMatrix() const
  {
    return this->GetPCAFilter()->GetTransformationMatrix();
  }
  void SetPCATransformationMatrix(const MatrixType& mat, bool isForward = true)
  {
    m_PCAFilter->SetTransformationMatrix(mat, isForward);
  }

  itkGetConstMacro(TransformationMatrix, MatrixType);
  itkGetMacro(TransformationMatrix, MatrixType);
  void SetTransformationMatrix(const MatrixType& mat, bool isForward = true)
  {
    m_IsTransformationForward   = isForward;
    m_GivenTransformationMatrix = true;
    m_TransformationMatrix      = mat;
    this->Modified();
  }

  itkGetMacro(NumberOfIterations, unsigned int);
  itkSetMacro(NumberOfIterations, unsigned int);

  itkGetMacro(ConvergenceThreshold, double);
  itkSetMacro(ConvergenceThreshold, double);

  void SetNonLinearity(NonLinearityType NonLinearity, NonLinearityType NonLinearityDerivative)
  {
    m_NonLinearity           = NonLinearity;
    m_NonLinearityDerivative = NonLinearityDerivative;
    this->Modified();
  }

  itkGetMacro(NonLinearity, NonLinearityType);
  itkGetMacro(NonLinearityDerivative, NonLinearityType);

  itkGetMacro(Mu, double);
  itkSetMacro(Mu, double);

protected:
  FastICAImageFilter();
  ~FastICAImageFilter() override
  {
  }

  /** GenerateOutputInformation
   * Propagate vector length info and modify if needed
   * NumberOfPrincipalComponentsRequired
   *
   * In REVERSE mode, the covariance matrix or the transformation matrix
   * (which may not be square) has to be given,
   * otherwize, GenerateOutputInformation throws an itk::ExceptionObject
   */
  void GenerateOutputInformation() override;

  /** GenerateData
   * Through a filter of filter structure
   */
  void GenerateData() override;

  /** Internal methods */
  void         ForwardGenerateOutputInformation();
  void         ReverseGenerateOutputInformation();
  virtual void ForwardGenerateData();
  virtual void ReverseGenerateData();

  /** this is the specific part of FastICA */
  virtual void GenerateTransformationMatrix();

  unsigned int m_NumberOfPrincipalComponentsRequired;

  /** Transformation matrix refers to the ICA step (not PCA) */
  bool       m_GivenTransformationMatrix;
  bool       m_IsTransformationForward;
  MatrixType m_TransformationMatrix;

  /** FastICA parameters */
  unsigned int     m_NumberOfIterations;     // def is 50
  double           m_ConvergenceThreshold;   // def is 1e-4
  NonLinearityType m_NonLinearity;           // see g() function in the biblio. Def is tanh
  NonLinearityType m_NonLinearityDerivative; // derivative of g().
  double           m_Mu;                     // def is 1. in [0, 1]

  PCAFilterPointerType       m_PCAFilter;
  TransformFilterPointerType m_TransformFilter;

private:
  FastICAImageFilter(const Self&) = delete;
  void operator=(const Self&) = delete;
}; // end of class

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbFastICAImageFilter.hxx"
#endif


#endif
