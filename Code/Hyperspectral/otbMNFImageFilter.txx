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

  m_GivenNoiseCovarianceMatrix = false;
  m_GivenNoiseTransformationMatrix = false;

  m_NoiseImageFilter = NoiseImageFilterType::New();
  m_PCAImageFilter = PCAImageFilterType::New();
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

  /*
   * Even if PCAImageFilter does not use this input,
   * the effective one has the same characteristics...
   */
  GetPCAImageFilter()->SetInput( this->GetInput() );
  GetPCAImageFilter()->UpdateOutputInformation();

  this->GetOutput()->SetNumberOfComponentsPerPixel( 
    GetPCAImageFilter()->GetOutput()->GetNumberOfComponentsPerPixel() );

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
          "Class should be templeted with FORWARD or INVERSE only...",
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

  if ( !m_GivenNoiseTransformationMatrix )
  {
    if ( !m_GivenNoiseTransformationMatrix )
    {
      otbGenericMsgDebugMacro(<< "Covariance estimation");

      m_NoiseImageFilter->SetInput( inputImgPtr );

      m_NoiseCovarianceEstimator->SetInput( m_NoiseImageFilter->GetOutput() );
      m_NoiseCovarianceEstimator->Update();

      //m_NoiseCovarianceMatrix = m_NoiseCovarianceEstimator->GetCovariance();
      m_NoiseCovarianceMatrix = m_NoiseCovarianceEstimator->GetCorrelation();
    }

    GetTransformationMatrixFromCovarianceMatrix();
  }

  if ( m_NoiseTransformationMatrix.GetVnlMatrix().empty() )
  {
    throw itk::ExceptionObject( __FILE__, __LINE__,
          "Emplty noise transformation matrix",
          ITK_LOCATION);
  }

  m_Transformer->SetInput( inputImgPtr );
  m_Transformer->SetMatrix( m_NoiseTransformationMatrix.GetVnlMatrix() );
  m_PCAImageFilter->SetInput( m_Transformer->GetOutput() );
  m_PCAImageFilter->GraftOutput( this->GetOutput() );
  m_PCAImageFilter->Update();

  this->GraftOutput( m_PCAImageFilter->GetOutput() );

  // FIXME Sortir les valeurs propores et calculer les SNR
}

template <class TInputImage, class TOutputImage, 
            class TNoiseImageFilter, 
            Transform::TransformDirection TDirectionOfTransformation >
void
MNFImageFilter< TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation >
::ReverseGenerateData ()
{
  
  m_PCAImageFilter->SetInput( this->GetInput() );
  
  if ( !m_PCAImageFilter->GetGivenTransformationMatrix() )
  {
    if ( !m_PCAImageFilter->GetGivenCovarianceMatrix() )
    {
      // the MNF output is the PCA output
       m_PCAImageFilter->GraftOutput( this->GetOutput() );
       m_PCAImageFilter->Update();
       this->GraftOutput( m_PCAImageFilter->GetOutput() );
       return;
    }
    else
    {
      GetTransformationMatrixFromCovarianceMatrix();
      m_NoiseTransformationMatrix = m_NoiseTransformationMatrix.GetTranspose();
    }
  }

  if ( m_NoiseTransformationMatrix.GetVnlMatrix().empty() )
  {
    throw itk::ExceptionObject( __FILE__, __LINE__,
          "Emplty transformation matrix",
          ITK_LOCATION);
  }

  m_Transformer->SetInput( m_PCAImageFilter->GetOutput() );
  m_Transformer->SetMatrix( m_NoiseTransformationMatrix.GetVnlMatrix() );
  m_Transformer->GraftOutput( this->GetOutput() );
  m_Transformer->Update();

  this->GraftOutput( m_Transformer->GetOutput() );
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

  //std::cerr << "\n0--------------------------\n";
  //std::cerr << m_NoiseCovarianceMatrix << "\n1--------------------------\n";
  //std::cerr << Id << "\n2--------------------------\n";

  typename MatrixType::InternalMatrixType A = m_NoiseCovarianceMatrix.GetVnlMatrix();
  typename MatrixType::InternalMatrixType I = Id.GetVnlMatrix();

  vnl_generalized_eigensystem solver ( A, I );

  typename MatrixType::InternalMatrixType transf = solver.V;
  transf.fliplr();
  transf.inplace_transpose();
  m_NoiseTransformationMatrix = transf;
  
  //std::cerr << m_NoiseTransformationMatrix << "\n3--------------------------\n";

  vnl_vector< double > valP = solver.D.diagonal();
  valP.flip();

  m_NoiseRatioValues.SetSize( valP.size() );
  for ( unsigned int i = 0; i < valP.size(); i++ )
    m_NoiseRatioValues[i] = static_cast< RealType >( valP[i] );

  //std::cerr << m_NoiseEigenValues << "\n4--------------------------\n";
}

template <class TInputImage, class TOutputImage, 
            class TNoiseImageFilter, 
            Transform::TransformDirection TDirectionOfTransformation >
void
MNFImageFilter< TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation >
::PrintSelf ( std::ostream& os, itk::Indent indent ) const
{
  Superclass::PrintSelf( os, indent );

  GetPCAImageFilter()->Print( os, indent.GetNextIndent() );

  if ( !m_NoiseCovarianceMatrix.GetVnlMatrix().empty() )
  {
    os << indent << "Noise Covariance matrix";
    if ( m_GivenNoiseCovarianceMatrix )
      os << " (given)";
    os << "\n";

    m_NoiseCovarianceMatrix.GetVnlMatrix().print(os);

    if ( m_GivenNoiseCovarianceMatrix )
      m_NoiseCovarianceEstimator->Print( os, indent.GetNextIndent() );
  }

  if ( !m_NoiseTransformationMatrix.GetVnlMatrix().empty() );
  {
    os << indent << "Noise Transformation matrix";
    if ( m_GivenNoiseTransformationMatrix )
      os << " (given)";
    os << "\n";

    m_NoiseTransformationMatrix.GetVnlMatrix().print(os);
  }

  if ( m_NoiseRatioValues.Size() > 0 )
  {
    os << indent << "RMS value :";
    for ( unsigned int i = 0; i < m_NoiseRatioValues.Size(); i++ )
      os << " " << m_NoiseRatioValues[i];
    os << "\n";
  }
}

} // end of namespace otb

#endif // __otbMNFImageFilter_txx


