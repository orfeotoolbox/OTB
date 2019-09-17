/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#ifndef otbMNFImageFilter_hxx
#define otbMNFImageFilter_hxx
#include "otbMNFImageFilter.h"

#include "itkMacro.h"

#include <vnl/vnl_matrix.h>
#include <vnl/algo/vnl_cholesky.h>
#include <vnl/algo/vnl_matrix_inverse.h>
#include <vnl/algo/vnl_symmetric_eigensystem.h>
#include <vnl/algo/vnl_generalized_eigensystem.h>

namespace otb
{
template <class TInputImage, class TOutputImage, class TNoiseImageFilter, Transform::TransformDirection TDirectionOfTransformation>
MNFImageFilter<TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation>::MNFImageFilter()
{
  this->SetNumberOfRequiredInputs(1);

  m_NumberOfPrincipalComponentsRequired = 0;

  m_UseNormalization  = false;
  m_GivenMeanValues   = false;
  m_GivenStdDevValues = false;

  m_GivenCovarianceMatrix         = false;
  m_GivenNoiseCovarianceMatrix    = false;
  m_GivenTransformationMatrix     = false;
  m_IsTransformationMatrixForward = true;

  m_Normalizer               = NormalizeFilterType::New();
  m_NoiseImageFilter         = NoiseImageFilterType::New();
  m_CovarianceEstimator      = CovarianceEstimatorFilterType::New();
  m_NoiseCovarianceEstimator = CovarianceEstimatorFilterType::New();
  m_Transformer              = TransformFilterType::New();
  m_Transformer->MatrixByVectorOn();
}

template <class TInputImage, class TOutputImage, class TNoiseImageFilter, Transform::TransformDirection TDirectionOfTransformation>
void MNFImageFilter<TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation>::GenerateOutputInformation()
// throw itk::ExceptionObject
{
  Superclass::GenerateOutputInformation();

  switch (static_cast<int>(DirectionOfTransformation))
  {
  case static_cast<int>(Transform::FORWARD):
  {
    if (m_NumberOfPrincipalComponentsRequired == 0 || m_NumberOfPrincipalComponentsRequired > this->GetInput()->GetNumberOfComponentsPerPixel())
    {
      m_NumberOfPrincipalComponentsRequired = this->GetInput()->GetNumberOfComponentsPerPixel();
    }

    this->GetOutput()->SetNumberOfComponentsPerPixel(m_NumberOfPrincipalComponentsRequired);
    break;
  }
  case static_cast<int>(Transform::INVERSE):
  {
    unsigned int theOutputDimension = 0;
    if (m_GivenTransformationMatrix)
    {
      theOutputDimension = m_TransformationMatrix.Rows() >= m_TransformationMatrix.Cols() ? m_TransformationMatrix.Rows() : m_TransformationMatrix.Cols();
    }
    else if (m_GivenCovarianceMatrix)
    {
      theOutputDimension = m_CovarianceMatrix.Rows() >= m_CovarianceMatrix.Cols() ? m_CovarianceMatrix.Rows() : m_CovarianceMatrix.Cols();
    }
    else
    {
      throw itk::ExceptionObject(__FILE__, __LINE__, "Covariance or transformation matrix required to know the output size", ITK_LOCATION);
    }

    m_NumberOfPrincipalComponentsRequired = 0;
    this->GetOutput()->SetNumberOfComponentsPerPixel(theOutputDimension);
    break;
  }
  default: // should not go so far...
    throw itk::ExceptionObject(__FILE__, __LINE__, "Class should be templeted with FORWARD or INVERSE only...", ITK_LOCATION);
  }


  switch (static_cast<int>(DirectionOfTransformation))
  {
  case static_cast<int>(Transform::FORWARD):
  {
    ForwardGenerateOutputInformation();
    break;
  }
  case static_cast<int>(Transform::INVERSE):
  {
    ReverseGenerateOutputInformation();
    break;
  }
  }
}

template <class TInputImage, class TOutputImage, class TNoiseImageFilter, Transform::TransformDirection TDirectionOfTransformation>
void MNFImageFilter<TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation>::ForwardGenerateOutputInformation()
{
  typename InputImageType::Pointer inputImgPtr = const_cast<InputImageType*>(this->GetInput());

  if (m_GivenMeanValues)
    m_Normalizer->SetMean(this->GetMeanValues());

  if (m_UseNormalization)
  {
    m_Normalizer->SetUseStdDev(true);
    if (m_GivenStdDevValues)
      m_Normalizer->SetStdDev(this->GetStdDevValues());
  }
  else
    m_Normalizer->SetUseStdDev(false);

  m_Normalizer->SetInput(inputImgPtr);
  m_Normalizer->GetOutput()->UpdateOutputInformation();

  if (!m_GivenMeanValues)
    m_MeanValues = m_Normalizer->GetCovarianceEstimator()->GetMean();

  if (m_UseNormalization)
  {
    if (!m_GivenStdDevValues)
      m_StdDevValues = m_Normalizer->GetFunctor().GetStdDev();
  }

  if (!m_GivenTransformationMatrix)
  {
    if (!m_GivenNoiseCovarianceMatrix)
    {
      m_NoiseImageFilter->SetInput(m_Normalizer->GetOutput());
      m_NoiseCovarianceEstimator->SetInput(m_NoiseImageFilter->GetOutput());
      m_NoiseCovarianceEstimator->Update();

      m_NoiseCovarianceMatrix = m_NoiseCovarianceEstimator->GetCovariance();
    }

    if (!m_GivenCovarianceMatrix)
    {
      m_CovarianceEstimator->SetInput(m_Normalizer->GetOutput());
      m_CovarianceEstimator->Update();

      m_CovarianceMatrix = m_CovarianceEstimator->GetCovariance();
    }

    GenerateTransformationMatrix();
  }
  else if (!m_IsTransformationMatrixForward)
  {
    // Prevents from multiple transpose in pipeline
    m_IsTransformationMatrixForward = true;
    if (m_TransformationMatrix.Rows() == m_TransformationMatrix.Cols())
    {
      m_TransformationMatrix = m_TransformationMatrix.GetInverse();
    }
    else
    {
      vnl_svd<MatrixElementType> invertor(m_TransformationMatrix.GetVnlMatrix());
      m_TransformationMatrix = invertor.pinverse();
    }
  }

  if (m_TransformationMatrix.GetVnlMatrix().empty())
  {
    throw itk::ExceptionObject(__FILE__, __LINE__, "Empty transformation matrix", ITK_LOCATION);
  }

  m_Transformer->SetInput(m_Normalizer->GetOutput());
  m_Transformer->SetMatrix(m_TransformationMatrix.GetVnlMatrix());
}

template <class TInputImage, class TOutputImage, class TNoiseImageFilter, Transform::TransformDirection TDirectionOfTransformation>
void MNFImageFilter<TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation>::ReverseGenerateOutputInformation()
{
  if (!m_GivenTransformationMatrix)
  {
    if (!m_GivenCovarianceMatrix)
    {
      throw itk::ExceptionObject(__FILE__, __LINE__, "Inverse Transformation or at least Covariance matrix is required to invert MNF", ITK_LOCATION);
    }

    if (!m_GivenNoiseCovarianceMatrix)
    {
      throw itk::ExceptionObject(__FILE__, __LINE__, "Inverse Transformation or at least Noise Covariance matrix is required to invert MNF", ITK_LOCATION);
    }

    GenerateTransformationMatrix();

    m_IsTransformationMatrixForward = false;
    if (m_TransformationMatrix.Rows() == m_TransformationMatrix.Cols())
      m_TransformationMatrix = vnl_matrix_inverse<MatrixElementType>(m_TransformationMatrix.GetVnlMatrix());
    else
    {
      vnl_svd<MatrixElementType> invertor(m_TransformationMatrix.GetVnlMatrix());
      m_TransformationMatrix = invertor.inverse();
    }
  }
  else if (m_IsTransformationMatrixForward)
  {
    // Prevent from multiple transpose in pipeline
    m_IsTransformationMatrixForward = false;
    if (m_TransformationMatrix.Rows() == m_TransformationMatrix.Cols())
    {
      m_TransformationMatrix = vnl_matrix_inverse<MatrixElementType>(m_TransformationMatrix.GetVnlMatrix());
    }
    else
    {
      vnl_svd<MatrixElementType> invertor(m_TransformationMatrix.GetVnlMatrix());
      m_TransformationMatrix = invertor.pinverse();
    }
  }

  if (m_TransformationMatrix.GetVnlMatrix().empty())
  {
    throw itk::ExceptionObject(__FILE__, __LINE__, "Empty transformation matrix", ITK_LOCATION);
  }

  m_Transformer->SetInput(this->GetInput());
  m_Transformer->SetMatrix(m_TransformationMatrix.GetVnlMatrix());

  if (!m_GivenMeanValues)
  {
    throw itk::ExceptionObject(__FILE__, __LINE__, "Initial means required for correct data centering", ITK_LOCATION);
  }
  if (m_UseNormalization)
  {
    if (!m_GivenStdDevValues)
    {
      throw itk::ExceptionObject(__FILE__, __LINE__, "Initial StdDevs required for de-normalization", ITK_LOCATION);
    }

    VectorType revStdDev(m_StdDevValues.Size());
    for (unsigned int i = 0; i < m_StdDevValues.Size(); ++i)
      revStdDev[i]      = 1. / m_StdDevValues[i];
    m_Normalizer->SetStdDev(revStdDev);

    VectorType revMean(m_MeanValues.Size());
    for (unsigned int i = 0; i < m_MeanValues.Size(); ++i)
      revMean[i]        = -m_MeanValues[i] / m_StdDevValues[i];
    m_Normalizer->SetMean(revMean);
  }
  else
  {
    VectorType revMean(m_MeanValues.Size());
    for (unsigned int i = 0; i < m_MeanValues.Size(); ++i)
      revMean[i]        = -m_MeanValues[i];
    m_Normalizer->SetMean(revMean);
    m_Normalizer->SetUseStdDev(false);
  }

  m_Normalizer->SetInput(m_Transformer->GetOutput());
}


template <class TInputImage, class TOutputImage, class TNoiseImageFilter, Transform::TransformDirection TDirectionOfTransformation>
void MNFImageFilter<TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation>::GenerateData()
{
  switch (static_cast<int>(DirectionOfTransformation))
  {
  case static_cast<int>(Transform::FORWARD):
    return ForwardGenerateData();
  case static_cast<int>(Transform::INVERSE):
    return ReverseGenerateData();
  default: // should not go so far
    throw itk::ExceptionObject(__FILE__, __LINE__, "Class should be templated with FORWARD or INVERSE only...", ITK_LOCATION);
  }
}

template <class TInputImage, class TOutputImage, class TNoiseImageFilter, Transform::TransformDirection TDirectionOfTransformation>
void MNFImageFilter<TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation>::ForwardGenerateData()
{
  m_Transformer->GraftOutput(this->GetOutput());
  m_Transformer->Update();
  this->GraftOutput(m_Transformer->GetOutput());
}

template <class TInputImage, class TOutputImage, class TNoiseImageFilter, Transform::TransformDirection TDirectionOfTransformation>
void MNFImageFilter<TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation>::ReverseGenerateData()
{
  m_Normalizer->GraftOutput(this->GetOutput());
  m_Normalizer->Update();
  this->GraftOutput(m_Normalizer->GetOutput());
}

template <class TInputImage, class TOutputImage, class TNoiseImageFilter, Transform::TransformDirection TDirectionOfTransformation>
void MNFImageFilter<TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation>::GenerateTransformationMatrix()
{
  vnl_cholesky       choleskySolver(m_NoiseCovarianceMatrix.GetVnlMatrix(), vnl_cholesky::estimate_condition);
  InternalMatrixType Rn     = choleskySolver.lower_triangle();
  InternalMatrixType Rn_inv = vnl_matrix_inverse<MatrixElementType>(Rn.transpose());
  InternalMatrixType C      = Rn_inv.transpose() * m_CovarianceMatrix.GetVnlMatrix() * Rn_inv;

  vnl_svd<MatrixElementType> solver(C);
  InternalMatrixType         U    = solver.U();
  InternalMatrixType         valP = solver.W();

  InternalMatrixType transf = Rn_inv * U;

  transf.inplace_transpose();

  if (m_NumberOfPrincipalComponentsRequired != this->GetInput()->GetNumberOfComponentsPerPixel())
    m_TransformationMatrix = transf.get_n_rows(0, m_NumberOfPrincipalComponentsRequired);
  else
    m_TransformationMatrix = transf;

  m_EigenValues.SetSize(m_NumberOfPrincipalComponentsRequired);
  for (unsigned int i = 0; i < m_NumberOfPrincipalComponentsRequired; ++i)
    m_EigenValues[i]  = static_cast<RealType>(valP(i, i));
}

template <class TInputImage, class TOutputImage, class TNoiseImageFilter, Transform::TransformDirection TDirectionOfTransformation>
void MNFImageFilter<TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation>::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  if (m_UseNormalization)
  {
    os << indent << "Normalisation with :\n" << indent << "Mean:  ";
    for (unsigned int i = 0; i < m_MeanValues.Size(); ++i)
      os << m_MeanValues[i] << "  ";
    os << "\n" << indent << "StdDev:  ";
    for (unsigned int i = 0; i < m_StdDevValues.Size(); ++i)
      os << m_StdDevValues[i] << "  ";
    os << "\n";
  }
  else
    os << indent << "No normalisation\n";

  if (!m_NoiseCovarianceMatrix.GetVnlMatrix().empty())
  {
    os << indent << "Noise Covariance matrix";
    if (m_GivenNoiseCovarianceMatrix)
      os << " (given)";
    os << "\n";

    m_NoiseCovarianceMatrix.GetVnlMatrix().print(os);
  }

  if (!m_CovarianceMatrix.GetVnlMatrix().empty())
  {
    os << indent << "Covariance matrix";
    if (m_GivenCovarianceMatrix)
      os << " (given)";
    os << "\n";

    m_CovarianceMatrix.GetVnlMatrix().print(os);
  }

  if (!m_TransformationMatrix.GetVnlMatrix().empty())
  {
    os << indent << "Transformation matrix";
    if (m_GivenTransformationMatrix)
      os << " (given)";
    os << "\n";

    m_TransformationMatrix.GetVnlMatrix().print(os);
  }

  if (m_EigenValues.Size() > 0)
  {
    os << indent << "RMS value :";
    for (unsigned int i = 0; i < m_EigenValues.Size(); ++i)
      os << " " << m_EigenValues[i];
    os << "\n";
  }
}

} // end of namespace otb

#endif // otbMNFImageFilter_hxx
