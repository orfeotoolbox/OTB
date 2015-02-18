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
#ifndef __otbPCAImageFilter_txx
#define __otbPCAImageFilter_txx
#include "otbPCAImageFilter.h"

#include "itkMacro.h"

#include <vnl/vnl_matrix.h>
#include <vnl/algo/vnl_symmetric_eigensystem.h>
#include <vnl/algo/vnl_generalized_eigensystem.h>

namespace otb
{

template < class TInputImage, class TOutputImage,
            Transform::TransformDirection TDirectionOfTransformation >
PCAImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::PCAImageFilter ()
{
  this->SetNumberOfRequiredInputs(1);

  m_NumberOfPrincipalComponentsRequired = 0;

  m_UseNormalization = false;
  m_UseVarianceForNormalization = false;
  m_GivenMeanValues = false;
  m_GivenStdDevValues = false;

  m_GivenCovarianceMatrix = false;
  m_GivenTransformationMatrix = false;
  m_IsTransformationMatrixForward = true;

  m_CovarianceEstimator = CovarianceEstimatorFilterType::New();
  m_Transformer = TransformFilterType::New();
  m_Transformer->MatrixByVectorOn();
  m_Normalizer = NormalizeFilterType::New();
}

template < class TInputImage, class TOutputImage,
            Transform::TransformDirection TDirectionOfTransformation >
void
PCAImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  switch ( static_cast<int>(DirectionOfTransformation) )
    {
    case static_cast<int>(Transform::FORWARD):
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
    case static_cast<int>(Transform::INVERSE):
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
    default:
      throw itk::ExceptionObject(__FILE__, __LINE__,
                                 "Class should be templeted with FORWARD or INVERSE only...",
                                 ITK_LOCATION );
    }
}

template < class TInputImage, class TOutputImage,
            Transform::TransformDirection TDirectionOfTransformation >
void
PCAImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::GenerateData ()
{
  switch ( static_cast<int>(DirectionOfTransformation) )
  {
  case static_cast<int>(Transform::FORWARD):
      return ForwardGenerateData();
    case static_cast<int>(Transform::INVERSE):
      return ReverseGenerateData();
    default:
      throw itk::ExceptionObject(__FILE__, __LINE__,
          "Class should be templated with FORWARD or INVERSE only...",
          ITK_LOCATION );
  }
}

template < class TInputImage, class TOutputImage,
            Transform::TransformDirection TDirectionOfTransformation >
void
PCAImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::ForwardGenerateData ()
{
  typename InputImageType::Pointer inputImgPtr
    = const_cast<InputImageType*>( this->GetInput() );

  if ( !m_GivenTransformationMatrix )
  {
    if ( !m_GivenCovarianceMatrix )
    {
      if ( m_UseNormalization )
      {
        m_Normalizer->SetInput( inputImgPtr );
        m_Normalizer->SetUseStdDev( m_UseVarianceForNormalization );

        if ( m_GivenMeanValues )
          m_Normalizer->SetMean( m_MeanValues );

        if ( m_GivenStdDevValues )
          m_Normalizer->SetStdDev( m_StdDevValues );

        m_Normalizer->Update();

        if ( !m_GivenMeanValues )
        {
          m_MeanValues = m_Normalizer->GetCovarianceEstimator()->GetMean();

          if ( !m_GivenStdDevValues )
            m_StdDevValues = m_Normalizer->GetFunctor().GetStdDev();

          if ( m_UseVarianceForNormalization )
            m_CovarianceMatrix = m_Normalizer->GetCovarianceEstimator()->GetCorrelation();
          else
            m_CovarianceMatrix = m_Normalizer->GetCovarianceEstimator()->GetCovariance();
        }
        else
        {
          m_CovarianceEstimator->SetInput( m_Normalizer->GetOutput() );
          m_CovarianceEstimator->Update();

          m_CovarianceMatrix = m_CovarianceEstimator->GetCovariance();
        }

        m_Transformer->SetInput( m_Normalizer->GetOutput() );
      }
      else
      {
        m_CovarianceEstimator->SetInput( inputImgPtr );
        m_CovarianceEstimator->Update();

        m_CovarianceMatrix = m_CovarianceEstimator->GetCovariance();

        m_Transformer->SetInput( inputImgPtr );
      }
    }
    else
    {
      m_Transformer->SetInput( inputImgPtr );
    }

    GenerateTransformationMatrix();
  }
  else if ( !m_IsTransformationMatrixForward )
  {
    //m_TransformationMatrix = m_TransformationMatrix.GetTranspose();
    m_TransformationMatrix = m_TransformationMatrix.GetInverse();

    m_Transformer->SetInput( inputImgPtr );
  }

  if ( m_TransformationMatrix.GetVnlMatrix().empty() )
  {
    throw itk::ExceptionObject( __FILE__, __LINE__,
          "Empty transformation matrix",
          ITK_LOCATION);
  }

  m_Transformer->SetMatrix( m_TransformationMatrix.GetVnlMatrix() );
  m_Transformer->GraftOutput( this->GetOutput() );
  m_Transformer->Update();
  this->GraftOutput( m_Transformer->GetOutput() );
}

template < class TInputImage, class TOutputImage,
            Transform::TransformDirection TDirectionOfTransformation >
void
PCAImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::ReverseGenerateData ()
{
  if ( !m_GivenTransformationMatrix )
  {
    if ( !m_GivenCovarianceMatrix )
    {
      throw itk::ExceptionObject( __FILE__, __LINE__,
          "No Covariance nor Transformation matrix given",
          ITK_LOCATION );
    }

    GenerateTransformationMatrix();
    //m_TransformationMatrix = m_TransformationMatrix.GetTranspose();
    m_TransformationMatrix = m_TransformationMatrix.GetInverse();
  }
  else if ( m_IsTransformationMatrixForward )
  {
    // prevents from multiple transpositions...
    m_IsTransformationMatrixForward = false;
    //m_TransformationMatrix = m_TransformationMatrix.GetTranspose();
    m_TransformationMatrix = m_TransformationMatrix.GetInverse();
  }

  if ( m_TransformationMatrix.GetVnlMatrix().empty() )
  {
    throw itk::ExceptionObject( __FILE__, __LINE__,
          "Empty transformation matrix",
          ITK_LOCATION);
  }

  m_Transformer->SetInput( this->GetInput() );
  m_Transformer->SetMatrix( m_TransformationMatrix.GetVnlMatrix() );

  if ( m_GivenMeanValues || m_GivenStdDevValues )
  {
    m_Normalizer->SetInput( m_Transformer->GetOutput() );

    if ( m_GivenStdDevValues )
    {
      VectorType revStdDev ( m_StdDevValues.Size() );
      for ( unsigned int i = 0; i < m_StdDevValues.Size(); ++i )
        revStdDev[i] = 1. / m_StdDevValues[i];
      m_Normalizer->SetStdDev( revStdDev );
    }

    if ( m_GivenMeanValues )
    {
      VectorType revMean ( m_MeanValues.Size() );
      if ( m_GivenStdDevValues )
      {
        for ( unsigned int i = 0; i < m_MeanValues.Size(); ++i )
          revMean[i] = - m_MeanValues[i] / m_StdDevValues[i];
        m_Normalizer->SetUseStdDev( true );
      }
      else
      {
        for ( unsigned int i = 0; i < m_MeanValues.Size(); ++i )
          revMean[i] = -m_MeanValues[i];
        m_Normalizer->SetUseStdDev( false );
      }
      m_Normalizer->SetMean( revMean );
    }

    m_Normalizer->GraftOutput( this->GetOutput() );
    m_Normalizer->Update();
    this->GraftOutput( m_Normalizer->GetOutput() );
  }
  else
  {
    m_Transformer->GraftOutput( this->GetOutput() );
    m_Transformer->Update();
    this->GraftOutput( m_Transformer->GetOutput() );
  }
}

template < class TInputImage, class TOutputImage,
            Transform::TransformDirection TDirectionOfTransformation >
void
PCAImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::GenerateTransformationMatrix ()
{
#if 0
  /*
   * Old stuff but did work !
   */

  MatrixType Id ( m_CovarianceMatrix );
  Id.SetIdentity();

  typename MatrixType::InternalMatrixType A = m_CovarianceMatrix.GetVnlMatrix();
  typename MatrixType::InternalMatrixType I = Id.GetVnlMatrix();

  vnl_generalized_eigensystem solver ( A, I );

  typename MatrixType::InternalMatrixType transf = solver.V;
  transf.fliplr();
  transf.inplace_transpose();

  vnl_vector< double > valP = solver.D.diagonal();
  valP.flip();

  /*
   * We used normalized PCA
   */
  for ( unsigned int i = 0; i < valP.size(); ++i )
  {
    if (  valP[i] != 0. )
      valP[i] = 1. / vcl_sqrt( vcl_abs( valP[i] ) );
    else
      throw itk::ExceptionObject( __FILE__, __LINE__,
            "Null Eigen value !!", ITK_LOCATION );
  }
  valP.post_multiply( transf );

  if ( m_NumberOfPrincipalComponentsRequired
      != this->GetInput()->GetNumberOfComponentsPerPixel() )
    m_TransformationMatrix = transf.get_n_rows( 0, m_NumberOfPrincipalComponentsRequired );
  else
    m_TransformationMatrix = transf;


  m_EigenValues.SetSize( m_NumberOfPrincipalComponentsRequired );
  for ( unsigned int i = 0; i < m_NumberOfPrincipalComponentsRequired; ++i )
    m_EigenValues[i] = static_cast< RealType >( valP[i] );
#else
  InternalMatrixType transf;
  vnl_vector<double> vectValP;
  vnl_symmetric_eigensystem_compute( m_CovarianceMatrix.GetVnlMatrix(), transf, vectValP );

  InternalMatrixType valP ( vectValP.size(), vectValP.size(), vnl_matrix_null );
  for ( unsigned int i = 0; i < vectValP.size(); ++i )
    valP(i, i) = vectValP[i];

  /* We used normalized PCA */
  for ( unsigned int i = 0; i < valP.rows(); ++i )
  {
    if (  valP(i, i) > 0. )
    {
      valP(i, i) = 1. / vcl_sqrt( valP(i, i) );
    }
    else if ( valP(i, i) < 0. )
    {
      otbMsgDebugMacro( << "ValP(" << i << ") neg : " << valP(i, i) << " taking abs value" );
      valP(i, i) = 1. / vcl_sqrt( vcl_abs( valP(i, i) ) );
    }
    else
    {
      throw itk::ExceptionObject( __FILE__, __LINE__,
            "Null Eigen value !!", ITK_LOCATION );
    }
  }
  transf = valP * transf.transpose();
  transf.flipud();

  if ( m_NumberOfPrincipalComponentsRequired
      != this->GetInput()->GetNumberOfComponentsPerPixel() )
    m_TransformationMatrix = transf.get_n_rows( 0, m_NumberOfPrincipalComponentsRequired );
  else
    m_TransformationMatrix = transf;

  m_EigenValues.SetSize( m_NumberOfPrincipalComponentsRequired );
  for ( unsigned int i = 0; i < m_NumberOfPrincipalComponentsRequired; ++i )
    m_EigenValues[i] = static_cast< RealType >( valP(i, i) );

#endif
}


template < class TInputImage, class TOutputImage,
            Transform::TransformDirection TDirectionOfTransformation >
void
PCAImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::PrintSelf ( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf( os, indent );

  os << indent << "m_UseNormalization = ";
  if ( m_UseNormalization )
    os << "true\n";
  else
    os << "false\n";

  if ( m_GivenMeanValues )
    os << indent << "Given Mean : " << m_MeanValues << "\n";

  if ( m_GivenStdDevValues )
    os << indent << "Given StdDev : " << m_StdDevValues << "\n";

  if ( !m_CovarianceMatrix.GetVnlMatrix().empty() )
  {
    os << indent << "Covariance matrix";
    if ( m_GivenCovarianceMatrix )
      os << " (given)";
    os << "\n";

    m_CovarianceMatrix.GetVnlMatrix().print(os);

    if ( m_GivenCovarianceMatrix )
      m_CovarianceEstimator->Print( os, indent.GetNextIndent() );
  }

  if ( !m_TransformationMatrix.GetVnlMatrix().empty() )
  {
    os << indent;
    if ( !m_IsTransformationMatrixForward )
      os << "Invert ";
    os << "Transformation matrix";
    if ( m_GivenTransformationMatrix )
      os << " (given)";
    os << "\n";

    m_TransformationMatrix.GetVnlMatrix().print(os);
  }

  if ( m_EigenValues.Size() > 0 )
  {
    os << indent << "Eigen value :";
    for ( unsigned int i = 0; i < m_NumberOfPrincipalComponentsRequired; ++i )
      os << " " << m_EigenValues[i];
    os << "\n";
  }
}

} // end of namespace otb

#endif // __otbPCAImageFilter_txx


