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
#ifndef __otbFastICAImageFilter_txx
#define __otbFastICAImageFilter_txx

#include "otbFastICAImageFilter.h"



#include "itkNumericTraits.h"
#include "itkProgressReporter.h"

#include <vnl/vnl_matrix.h>
#include <vnl/algo/vnl_matrix_inverse.h>
#include <vnl/algo/vnl_generalized_eigensystem.h>

namespace otb
{

template < class TInputImage, class TOutputImage,
            Transform::TransformDirection TDirectionOfTransformation >
FastICAImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::FastICAImageFilter ()
{
  this->SetNumberOfRequiredInputs(1);

  m_NumberOfPrincipalComponentsRequired = 0;

  m_GivenTransformationMatrix = false;
  m_IsTransformationForward = true;

  m_NumberOfIterations = 50;
  m_ConvergenceThreshold = 1E-4;
  m_ContrastFunction = &vcl_tanh;
  m_Mu = 1.;

  m_PCAFilter = PCAFilterType::New();
  m_PCAFilter->SetUseNormalization(true);

  m_TransformFilter = TransformFilterType::New();
}

template < class TInputImage, class TOutputImage,
            Transform::TransformDirection TDirectionOfTransformation >
void
FastICAImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::GenerateOutputInformation()
// throw itk::ExceptionObject
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
      else
      {
        throw itk::ExceptionObject(__FILE__, __LINE__,
          "Mixture matrix is required to know the output size",
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
FastICAImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
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
FastICAImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::ForwardGenerateData ()
{
  typename InputImageType::Pointer inputImgPtr
    = const_cast<InputImageType*>( this->GetInput() );

  m_PCAFilter->SetInput( inputImgPtr );
  m_PCAFilter->Update();

  if ( !m_GivenTransformationMatrix )
  {
    GenerateTransformationMatrix();
  }
  else if ( !m_IsTransformationForward )
  {
    // prevent from multiple inversion in the pipelines
    m_IsTransformationForward = true;
    vnl_svd< MatrixElementType > invertor ( m_TransformationMatrix.GetVnlMatrix() );
    m_TransformationMatrix = invertor.pinverse();
  }

  if ( m_TransformationMatrix.GetVnlMatrix().empty() )
  {
    throw itk::ExceptionObject( __FILE__, __LINE__,
          "Empty transformation matrix",
          ITK_LOCATION);
  }

  m_TransformFilter->SetInput( m_PCAFilter->GetOutput() );
  m_TransformFilter->SetMatrix( m_TransformationMatrix.GetVnlMatrix() );
  m_TransformFilter->GraftOutput( this->GetOutput() );
  m_TransformFilter->Update();

  this->GraftOutput( m_TransformFilter->GetOutput() );
}

template < class TInputImage, class TOutputImage,
            Transform::TransformDirection TDirectionOfTransformation >
void
FastICAImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::ReverseGenerateData ()
{
  if ( !m_GivenTransformationMatrix )
  {
    throw itk::ExceptionObject( __FILE__, __LINE__,
                                "No Transformation matrix given",
                                ITK_LOCATION );
  }

  if ( m_TransformationMatrix.GetVnlMatrix().empty() )
  {
    throw itk::ExceptionObject( __FILE__, __LINE__,
          "Empty transformation matrix",
          ITK_LOCATION);
  }

  if ( m_IsTransformationForward )
  {
    // prevent from multiple inversion in the pipelines
    m_IsTransformationForward = false;
    vnl_svd< MatrixElementType > invertor ( m_TransformationMatrix.GetVnlMatrix() );
    m_TransformationMatrix = invertor.pinverse();
  }

  m_TransformFilter->SetInput( this->GetInput() );
  m_TransformFilter->SetMatrix( m_TransformationMatrix.GetVnlMatrix() );

  /*
   * PCA filter may throw exception if
   * the mean, stdDev and transformation matrix
   * have not been given at this point
   */
  m_PCAFilter->SetInput( m_TransformFilter->GetOutput() );
  m_PCAFilter->GraftOutput( this->GetOutput() );
  m_PCAFilter->Update();

  this->GraftOutput( m_PCAFilter->GetOutput() );
}

template < class TInputImage, class TOutputImage,
            Transform::TransformDirection TDirectionOfTransformation >
void
FastICAImageFilter< TInputImage, TOutputImage, TDirectionOfTransformation >
::GenerateTransformationMatrix ()
{
  itk::ProgressReporter reporter ( this, 0, GetNumberOfIterations(), GetNumberOfIterations() );

  double convergence = itk::NumericTraits<double>::max();
  unsigned int iteration = 0;

  const unsigned int size = this->GetInput()->GetNumberOfComponentsPerPixel();

  // transformation matrix
  InternalMatrixType W ( size, size, vnl_matrix_identity );

  while ( iteration++ < GetNumberOfIterations()
          && convergence > GetConvergenceThreshold() )
  {
    InternalMatrixType W_old ( W );

    typename InputImageType::Pointer img = const_cast<InputImageType*>( this->GetInput() );
    TransformFilterPointerType transformer = TransformFilterType::New();
    if ( !W.is_identity() )
    {
      transformer->SetInput( GetPCAFilter()->GetOutput() );
      transformer->SetMatrix( W );
      transformer->Update();
      img = const_cast<InputImageType*>( transformer->GetOutput() );
    }

    for ( unsigned int band = 0; band < size; band++ )
    {
      otbMsgDebugMacro( << "Iteration " << iteration << ", bande " << band
                        << ", convergence " << convergence );

      InternalOptimizerPointerType optimizer = InternalOptimizerType::New();
      optimizer->SetInput( 0, m_PCAFilter->GetOutput() );
      optimizer->SetInput( 1, img );
      optimizer->SetW( W );
      optimizer->SetContrastFunction( this->GetContrastFunction() );
      optimizer->SetCurrentBandForLoop( band );

      MeanEstimatorFilterPointerType estimator = MeanEstimatorFilterType::New();
      estimator->SetInput( optimizer->GetOutput() );
      estimator->Update();

      double norm = 0.;
      for ( unsigned int bd = 0; bd < size; bd++ )
      {
        W(band, bd) -= m_Mu * ( estimator->GetMean()[bd]
                              - optimizer->GetBeta() * W(band, bd) / optimizer->GetDen() );
        norm += vcl_pow( W(band, bd), 2. );
      }
      for ( unsigned int bd = 0; bd < size; bd++ )
        W(band, bd) /= vcl_sqrt( norm );
    }

    // Decorrelation of the W vectors
    InternalMatrixType W_tmp = W * W.transpose();
    vnl_svd< MatrixElementType > solver ( W_tmp );
    InternalMatrixType valP = solver.W();
    for ( unsigned int i = 0; i < valP.rows(); ++i )
      valP(i, i) = 1. / vcl_sqrt( static_cast<double>( valP(i, i) ) ); // Watch for 0 or neg
    InternalMatrixType transf = solver.U();
    W_tmp = transf * valP * transf.transpose();
    W = W_tmp * W;

    // Convergence evaluation
    convergence = 0.;
    for ( unsigned int i = 0; i < W.rows(); ++i )
      for ( unsigned int j = 0; j < W.cols(); ++j )
        convergence += vcl_abs( W(i, j) - W_old(i, j) );

    reporter.CompletedPixel();
  } // end of while loop

  if ( size != this->GetNumberOfPrincipalComponentsRequired() )
    {
    this->m_TransformationMatrix = W.get_n_rows( 0, this->GetNumberOfPrincipalComponentsRequired() );
    }
  else
    {
    this->m_TransformationMatrix = W;
    }

  otbMsgDebugMacro( << "Final convergence " << convergence
    << " after " << iteration << " iterations" );
}

} // end of namespace otb

#endif


