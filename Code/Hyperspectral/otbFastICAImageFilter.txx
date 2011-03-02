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

#include "otbMacro.h"

#include "itkExceptionObject.h"
#include "itkNumericTraits.h"

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

  m_MaximumOfIterations = 50;
  m_ConvergenceThreshold = 1E-4;
  m_ConstrastFunction = &vcl_tanh;

  m_PCAFilter = PCAFilterType::New();
  m_PCAFilter->UseNormalization();

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
  double convergence = itk::NumericTraits<double>::Max();
  unsigned int iteration = 0;

  unsigned int size = this->GetInput()->GetNumberOfComponentsPerPixel();

  // transformation matrix
  InternalMatrixType W ( size, size, vnl_matrix_identity );

  while ( iteration++ < GetMaximumOfIterations() 
          && convergence > GetConvergenceThreshold() )
  {
    otbMsgDebugMacro( "Iteration " << iteration << " / " << GetMaximumOfIterations() 
      << ", MSE = " << convergence );

    InternalMatrixType W_old ( W );

    // TODO le premier coup ne sert a rien
    TransformFilterPointerType transformer = TransformFilterType::New();
    transformer->SetIntput( GetPCAFilter()->GetOutput() );
    transformer->SetMatrix( W );
    transformer->Update();

    // Faire un image to image filter...

  } // end of while loop
}

} // end of namespace otb

#endif


