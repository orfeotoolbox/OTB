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
#ifndef __otbNAPCAImageFilter_txx
#define __otbNAPCAImageFilter_txx
#include "otbNAPCAImageFilter.h"

#include "itkExceptionObject.h"

#include <vnl/vnl_matrix.h>
#include <vnl/algo/vnl_matrix_inverse.h>
#include <vnl/algo/vnl_generalized_eigensystem.h>

namespace otb 
{

template <class TInputImage, class TOutputImage, 
            class TNoiseImageFilter, 
            Transform::TransformDirection TDirectionOfTransformation >
void
NAPCAImageFilter< TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation >
::GetTransformationMatrixFromCovarianceMatrix ()
{
  InternalMatrixType An = m_NoiseCovarianceMatrix.GetVnlMatrix();
  InternalMatrixType Ax = m_CovarianceMatrix.GetVnlMatrix();

  vnl_svd< MatrixElementType > An_solver ( An );
  InternalMatrixType U_cholesky = An_solver.U();
  InternalMatrixType U = vnl_matrix_inverse< MatrixElementType > ( U_cholesky );
  InternalMatrixType C = U.transpose() * Ax * U;

  InternalMatrixType Id ( C.rows(), C.cols(), vnl_matrix_identity );
  vnl_generalized_eigensystem solver ( C, I );

  InternalMatrixType transf = solver.V;
  transf *= U.transpose();
  transf.fliplr();

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


} // end of namespace otb

#endif


