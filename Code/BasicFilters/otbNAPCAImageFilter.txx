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

#include "itkMacro.h"

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
::GenerateTransformationMatrix ()
{
  InternalMatrixType An = this->GetNoiseCovarianceMatrix().GetVnlMatrix();
  InternalMatrixType Fn;
  vnl_vector<double> vectValPn;
  vnl_symmetric_eigensystem_compute( An, Fn, vectValPn );
  Fn.inplace_transpose();

  InternalMatrixType Ax = this->GetCovarianceMatrix().GetVnlMatrix();
  InternalMatrixType Aadj = Fn.transpose() * Ax * Fn;

  InternalMatrixType Fadj;
  vnl_vector<double> vectValPadj;
  vnl_symmetric_eigensystem_compute( Aadj, Fadj, vectValPadj );

  InternalMatrixType transf = Fn * Fadj;
  transf.fliplr();
  transf.inplace_transpose();

  if ( this->GetNumberOfPrincipalComponentsRequired()
      != this->GetInput()->GetNumberOfComponentsPerPixel() )
    this->m_TransformationMatrix = transf.get_n_rows( 0, this->GetNumberOfPrincipalComponentsRequired() );
  else
    this->m_TransformationMatrix = transf;

  this->m_EigenValues.SetSize( this->GetNumberOfPrincipalComponentsRequired() );
  for ( unsigned int i = 0; i < this->GetNumberOfPrincipalComponentsRequired(); ++i )
    this->m_EigenValues[this->GetNumberOfPrincipalComponentsRequired()-1-i]
      = static_cast< RealType >( vectValPadj[i] );
}


} // end of namespace otb

#endif


