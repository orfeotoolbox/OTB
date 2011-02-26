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

#include "itkExceptionObject.h"

#include <vnl/vnl_matrix.h>
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
  m_GivenMeanValues = false;
  m_GivenCovarianceMatrix = false;
  m_GivenTransformationMatrix = false;
  m_IsTransformationMatrixForward = true;

  m_CovarianceEstimator = CovarianceEstimatorFilterType::New();
  m_Transformer = TransformFilterType::New();
  m_Normalizer = NormalizeFilterType::New();
}

template < class TInputImage, class TOutputImage, 
            Transform::TransformDirection TDirectionOfTransformation >
void
PCAImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
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
  switch ( DirectionOfTransformation )
  {
    case Transform::FORWARD:
      return ForwardGenerateData();
    case Transform::INVERSE:
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
      m_Normalizer->SetInput( inputImgPtr );
      m_Normalizer->SetUseStdDev( false );

      if ( m_GivenMeanValues )
        m_Normalizer->SetMean( m_MeanValues );
      
      m_Normalizer->Update();

      if ( !m_GivenMeanValues )
        m_MeanValues = m_Normalizer->GetFunctor().GetMean();

      m_CovarianceMatrix = m_CovarianceEstimator->GetCovariance();

      m_Transformer->SetInput( m_Normalizer->GetOutput() );
    }
    else
    {
      m_Transformer->SetInput( inputImgPtr );
    }

    GetTransformationMatrixFromCovarianceMatrix();
  }
  else if ( !m_IsTransformationMatrixForward )
  {
    m_TransformationMatrix = m_TransformationMatrix.GetTranspose();

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

    GetTransformationMatrixFromCovarianceMatrix();
    m_TransformationMatrix = m_TransformationMatrix.GetTranspose();
  }
  else if ( m_IsTransformationMatrixForward )
  {
    std::cerr << "Transposition\n";
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

  if ( m_GivenMeanValues )
  {
    VectorType revMean ( m_MeanValues.Size() );
    for ( unsigned int i = 0; i < m_MeanValues.Size(); i++ )
      revMean[i] = -m_MeanValues[i];

    m_Normalizer->SetInput( m_Transformer->GetOutput() );
    m_Normalizer->SetMean( revMean );
    m_Normalizer->SetUseStdDev( false );

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
::GetTransformationMatrixFromCovarianceMatrix ()
{
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
  valP.post_multiply( transf );

  if ( m_NumberOfPrincipalComponentsRequired 
      != this->GetInput()->GetNumberOfComponentsPerPixel() )
    m_TransformationMatrix = transf.get_n_rows( 0, m_NumberOfPrincipalComponentsRequired );
  else
    m_TransformationMatrix = transf;
  
 
  m_EigenValues.SetSize( m_NumberOfPrincipalComponentsRequired );
  for ( unsigned int i = 0; i < m_NumberOfPrincipalComponentsRequired; i++ )
    m_EigenValues[i] = static_cast< RealType >( valP[i] );
}


template < class TInputImage, class TOutputImage, 
            Transform::TransformDirection TDirectionOfTransformation >
void
PCAImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::PrintSelf ( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf( os, indent );

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

  if ( !m_TransformationMatrix.GetVnlMatrix().empty() );
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
    for ( unsigned int i = 0; i < m_NumberOfPrincipalComponentsRequired; i++ )
      os << " " << m_EigenValues[i];
    os << "\n";
  }
}

} // end of namespace otb

#endif // __otbPCAImageFilter_txx



