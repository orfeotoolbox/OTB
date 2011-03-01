/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbMNFImageFilter_txx
#define __otbMNFImageFilter_txx
#include "otbMNFImageFilter.h"

#include "itkExceptionObject.h"

#include <vnl/vnl_matrix.h>
#include <vnl/algo/vnl_matrix_inverse.h>
#include <vnl/algo/vnl_generalized_eigensystem.h>

namespace otb
{
template <class TInputImage, class TOutputImage, 
            class TNoiseImageFilter, 
            Transform::TransformDirection TDirectionOfTransformation >
MNFImageFilter< TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation >
::MNFImageFilter ()
{
  this->SetNumberOfRequiredInputs(1);

  m_NumberOfPrincipalComponentsRequired = 0;

  m_UseNormalization = false;
  m_GivenMeanValues = false;
  m_GivenStdDevValues = false;

  m_GivenCovarianceMatrix = false;
  m_GivenNoiseCovarianceMatrix = false;
  m_GivenTransformationMatrix = false;
  m_IsTransformationMatrixForward = true;

  m_Normalizer = NormalizeFilterType::New();
  m_NoiseImageFilter = NoiseImageFilterType::New();
  m_CovarianceEstimator = CovarianceEstimatorFilterType::New();
  m_NoiseCovarianceEstimator = CovarianceEstimatorFilterType::New();
  m_Transformer = TransformFilterType::New();
}

template <class TInputImage, class TOutputImage, 
            class TNoiseImageFilter, 
            Transform::TransformDirection TDirectionOfTransformation >
void
MNFImageFilter< TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation >
::GenerateOutputInformation()
// throw itk::ExceptionObject
{
  Superclass::GenerateOutputInformation();

  switch ( DirectionOfTransformation )
  {
    case Transform::FORWARD:
    {
      if ( m_NumberOfPrincipalComponentsRequired == 0 
          || m_NumberOfPrincipalComponentsRequired 
            > this->GetInput()->GetNumberOfComponentsPerPixel() )
      {
        m_NumberOfPrincipalComponentsRequired = 
          this->GetInput()->GetNumberOfComponentsPerPixel();
      }

      this->GetOutput()->SetNumberOfComponentsPerPixel(
        m_NumberOfPrincipalComponentsRequired );
      break;
    }
    case Transform::INVERSE:
    {
      unsigned int theOutputDimension = 0;
      if ( m_GivenTransformationMatrix )
      {
        theOutputDimension = m_TransformationMatrix.Rows() >= m_TransformationMatrix.Cols() ?
          m_TransformationMatrix.Rows() : m_TransformationMatrix.Cols();
      }
      else if ( m_GivenCovarianceMatrix )
      {
        theOutputDimension = m_CovarianceMatrix.Rows() >= m_CovarianceMatrix.Cols() ?
          m_CovarianceMatrix.Rows() : m_CovarianceMatrix.Cols();
      }
      else
      {
        throw itk::ExceptionObject(__FILE__, __LINE__,
          "Covariance or transformation matrix required to know the output size",
          ITK_LOCATION);
      }

      this->GetOutput()->SetNumberOfComponentsPerPixel( theOutputDimension );

      break;
    }
    default: // should not go so far...
      throw itk::ExceptionObject(__FILE__, __LINE__,
          "Class should be templeted with FORWARD or INVERSE only...",
          ITK_LOCATION );
  }
}

template <class TInputImage, class TOutputImage, 
            class TNoiseImageFilter, 
            Transform::TransformDirection TDirectionOfTransformation >
void
MNFImageFilter< TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation >
::GenerateData ()
{
  switch ( DirectionOfTransformation )
  {
    case Transform::FORWARD:
      return ForwardGenerateData();
    case Transform::INVERSE:
      return ReverseGenerateData();
    default: // should not go so far
      throw itk::ExceptionObject(__FILE__, __LINE__,
          "Class should be templated with FORWARD or INVERSE only...",
          ITK_LOCATION );
  }
}

template <class TInputImage, class TOutputImage, 
            class TNoiseImageFilter, 
            Transform::TransformDirection TDirectionOfTransformation >
void
MNFImageFilter< TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation >
::ForwardGenerateData ()
{
  typename InputImageType::Pointer inputImgPtr 
    = const_cast<InputImageType*>( this->GetInput() );

  if ( m_UseNormalization )
  {
    if ( m_GivenMeanValues )
      m_Normalizer->SetMean( this->GetMeanValues() );

    if ( m_GivenStdDevValues )
      m_Normalizer->SetStdDev( this->GetStdDevValues() );

    m_Normalizer->SetInput( inputImgPtr );

    std::cerr << m_Normalizer << "\n";
  }

  if ( !m_GivenTransformationMatrix )
  {
    if ( !m_GivenNoiseCovarianceMatrix )
    {
      if ( m_UseNormalization )
        m_NoiseImageFilter->SetInput( m_Normalizer->GetOutput() );
      else
        m_NoiseImageFilter->SetInput( inputImgPtr );

      m_NoiseCovarianceEstimator->SetInput( m_NoiseImageFilter->GetOutput() );
      m_NoiseCovarianceEstimator->Update();

      m_NoiseCovarianceMatrix = m_NoiseCovarianceEstimator->GetCovariance();
    }

    if ( !m_GivenCovarianceMatrix )
    {
      if ( m_UseNormalization )
        m_CovarianceEstimator->SetInput( m_Normalizer->GetOutput() );
      else
        m_CovarianceEstimator->SetInput( inputImgPtr );
      m_CovarianceEstimator->Update();

      m_CovarianceMatrix = m_CovarianceEstimator->GetCovariance();
    }

    GetTransformationMatrixFromCovarianceMatrix();
  }
  else if ( !m_IsTransformationMatrixForward )
  {
    // Prevents from multiple transpose in pipeline
    m_IsTransformationMatrixForward = true;
    m_TransformationMatrix = m_TransformationMatrix.GetTranspose();
  }

  if ( m_TransformationMatrix.GetVnlMatrix().empty() )
  {
    throw itk::ExceptionObject( __FILE__, __LINE__,
          "Empty transformation matrix",
          ITK_LOCATION);
  }

  if ( m_UseNormalization )
  {
    m_Transformer->SetInput( m_Normalizer->GetOutput() );

    if ( !m_GivenMeanValues )
      m_MeanValues = m_Normalizer->GetFunctor().GetMean();

    if ( !m_GivenStdDevValues )
      m_StdDevValues = m_Normalizer->GetFunctor().GetStdDev();
  }
  else
    m_Transformer->SetInput( inputImgPtr );

  m_Transformer->SetMatrix( m_TransformationMatrix.GetVnlMatrix() );
  m_Transformer->GraftOutput( this->GetOutput() );
  m_Transformer->Update();

  this->GraftOutput( m_Transformer->GetOutput() );
}

template <class TInputImage, class TOutputImage, 
            class TNoiseImageFilter, 
            Transform::TransformDirection TDirectionOfTransformation >
void
MNFImageFilter< TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation >
::ReverseGenerateData ()
{
  if ( !m_GivenTransformationMatrix )
  {
    if ( !m_GivenCovarianceMatrix )
    {
      throw itk::ExceptionObject( __FILE__, __LINE__,
        "Inverse Transformation or at least Covariance matrix is required to invert MNF",
        ITK_LOCATION );
    }

    if ( !m_GivenNoiseCovarianceMatrix )
    {
      throw itk::ExceptionObject( __FILE__, __LINE__,
        "Inverse Transformation or at least Noise Covariance matrix is required to invert MNF",
        ITK_LOCATION );
    }

    GetTransformationMatrixFromCovarianceMatrix();
    
    m_IsTransformationMatrixForward = false;
    m_TransformationMatrix = m_TransformationMatrix.GetTranspose();
  }
  else if ( m_IsTransformationMatrixForward )
  {
    // Prevent from multiple transpose in pipeline
    m_IsTransformationMatrixForward = false;
    m_TransformationMatrix = m_TransformationMatrix.GetTranspose();
  }

  if ( m_TransformationMatrix.GetVnlMatrix().empty() )
  {
    throw itk::ExceptionObject( __FILE__, __LINE__,
          "Empty transformation matrix",
          ITK_LOCATION);
  }

  m_Transformer->SetInput( this->GetInput() );
  m_Transformer->SetMatrix( m_TransformationMatrix.GetVnlMatrix() );

  if ( m_UseNormalization )
  {
    if ( !m_GivenMeanValues || !m_GivenStdDevValues )
    {
      throw itk::ExceptionObject( __FILE__, __LINE__,
            "Initial means and StdDevs required for de-normalization",
            ITK_LOCATION );
    }

    VectorType revStdDev ( m_StdDevValues.Size() );
    for ( unsigned int i = 0; i < m_StdDevValues.Size(); i++ )
      revStdDev[i] = 1. / m_StdDevValues[i];
    m_Normalizer->SetStdDev( revStdDev );

    VectorType revMean ( m_MeanValues.Size() );
    for ( unsigned int i = 0; i < m_MeanValues.Size(); i++ )
      revMean[i] = - m_MeanValues[i] / m_StdDevValues[i];
    m_Normalizer->SetMean( revMean );

    m_Normalizer->SetInput( m_Transformer->GetOutput() );
    m_Normalizer->GraftOutput( this->GetOutput() );
    m_Normalizer->Update();

    std::cerr << m_Normalizer << "\n";

    this->GraftOutput( m_Normalizer->GetOutput() );
  }
  else
  {
    m_Transformer->GraftOutput( this->GetOutput() );
    m_Transformer->Update();

    this->GraftOutput( m_Transformer->GetOutput() );
  }
}

template <class TInputImage, class TOutputImage, 
            class TNoiseImageFilter, 
            Transform::TransformDirection TDirectionOfTransformation >
void
MNFImageFilter< TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation >
::GetTransformationMatrixFromCovarianceMatrix ()
{
  MatrixType Id ( m_NoiseCovarianceMatrix );
  Id.SetIdentity();

  typename MatrixType::InternalMatrixType Ax_inv = 
    vnl_matrix_inverse< typename MatrixType::InternalMatrixType::element_type>
    ( m_CovarianceMatrix.GetVnlMatrix() );
  typename MatrixType::InternalMatrixType An = m_NoiseCovarianceMatrix.GetVnlMatrix();
  typename MatrixType::InternalMatrixType W = An * Ax_inv;
  typename MatrixType::InternalMatrixType I = Id.GetVnlMatrix();

  vnl_generalized_eigensystem solver ( W, I );

  typename MatrixType::InternalMatrixType transf = solver.V;
  typename MatrixType::InternalMatrixType normMat 
    = transf.transpose() * m_CovarianceMatrix.GetVnlMatrix() * transf;

  for ( unsigned int i = 0; i < transf.rows(); i++ )
  {
    double norm = 1. / vcl_sqrt( normMat.get(i,i) );
    for ( unsigned int j = 0; j < transf.cols(); j++ )
      transf.put( i, j, transf.get(i,j) * norm );
  }

  transf.fliplr();
  transf.inplace_transpose();

  if ( m_NumberOfPrincipalComponentsRequired 
      != this->GetInput()->GetNumberOfComponentsPerPixel() )
    m_TransformationMatrix = transf.get_n_rows( 0, m_NumberOfPrincipalComponentsRequired );
  else
    m_TransformationMatrix = transf;

  vnl_vector< double > valP = solver.D.diagonal();
  valP.flip();

  m_EigenValues.SetSize( m_NumberOfPrincipalComponentsRequired );
  for ( unsigned int i = 0; i < m_NumberOfPrincipalComponentsRequired; i++ )
    m_EigenValues[i] = static_cast< RealType >( valP[i] );
}

template <class TInputImage, class TOutputImage, 
            class TNoiseImageFilter, 
            Transform::TransformDirection TDirectionOfTransformation >
void
MNFImageFilter< TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation >
::PrintSelf ( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf( os, indent );

  if ( m_UseNormalization )
  {
    os << indent << "Normalisation with :\n" << indent << "Mean:  ";
    for ( unsigned int i = 0; i < m_MeanValues.Size(); i++ )
      os << m_MeanValues[i] << "  ";
    os << "\n" << indent << "StdDev:  ";
    for ( unsigned int i = 0; i < m_StdDevValues.Size(); i++ )
      os << m_StdDevValues[i] << "  ";
    os << "\n";
  }
  else
    os << indent << "No normalisation\n";

  if ( !m_NoiseCovarianceMatrix.GetVnlMatrix().empty() )
  {
    os << indent << "Noise Covariance matrix";
    if ( m_GivenNoiseCovarianceMatrix )
      os << " (given)";
    os << "\n";

    m_NoiseCovarianceMatrix.GetVnlMatrix().print(os);
  }

  if ( !m_CovarianceMatrix.GetVnlMatrix().empty() )
  {
    os << indent << "Covariance matrix";
    if ( m_GivenCovarianceMatrix )
      os << " (given)";
    os << "\n";

    m_CovarianceMatrix.GetVnlMatrix().print(os);
  }

  if ( !m_TransformationMatrix.GetVnlMatrix().empty() );
  {
    os << indent << "Transformation matrix";
    if ( m_GivenTransformationMatrix )
      os << " (given)";
    os << "\n";

    m_TransformationMatrix.GetVnlMatrix().print(os);
  }

  if ( m_EigenValues.Size() > 0 )
  {
    os << indent << "RMS value :";
    for ( unsigned int i = 0; i < m_EigenValues.Size(); i++ )
      os << " " << m_EigenValues[i];
    os << "\n";
  }
}

} // end of namespace otb

#endif // __otbMNFImageFilter_txx


