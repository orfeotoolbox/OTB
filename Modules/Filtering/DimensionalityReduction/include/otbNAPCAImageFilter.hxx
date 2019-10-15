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

#ifndef otbNAPCAImageFilter_hxx
#define otbNAPCAImageFilter_hxx
#include "otbNAPCAImageFilter.h"

#include "itkMacro.h"

#include <vnl/vnl_matrix.h>
#include <vnl/algo/vnl_matrix_inverse.h>
#include <vnl/algo/vnl_symmetric_eigensystem.h>
#include <vnl/algo/vnl_generalized_eigensystem.h>

namespace otb
{

template <class TInputImage, class TOutputImage, class TNoiseImageFilter, Transform::TransformDirection TDirectionOfTransformation>
void NAPCAImageFilter<TInputImage, TOutputImage, TNoiseImageFilter, TDirectionOfTransformation>::GenerateTransformationMatrix()
{
  InternalMatrixType An = this->GetNoiseCovarianceMatrix().GetVnlMatrix();
  InternalMatrixType Fn;
  vnl_vector<double> vectValPn;
  vnl_symmetric_eigensystem_compute(An, Fn, vectValPn);

  /* We used normalized PCA */
  InternalMatrixType valPn(vectValPn.size(), vectValPn.size(), vnl_matrix_null);
  for (unsigned int i = 0; i < valPn.rows(); ++i)
  {
    if (vectValPn[i] > 0.)
    {
      valPn(i, i) = 1. / std::sqrt(vectValPn[i]);
    }
    else if (vectValPn[i] < 0.)
    {
      otbMsgDebugMacro(<< "ValPn(" << i << ") neg : " << vectValPn[i] << " taking abs value");
      valPn(i, i) = 1. / std::sqrt(std::abs(vectValPn[i]));
    }
    else
    {
      throw itk::ExceptionObject(__FILE__, __LINE__, "Null Eigen value !!", ITK_LOCATION);
    }
  }
  Fn = Fn * valPn;

  InternalMatrixType Ax   = vnl_matrix_inverse<MatrixElementType>(this->GetCovarianceMatrix().GetVnlMatrix());
  InternalMatrixType Aadj = Fn.transpose() * Ax * Fn;

  InternalMatrixType Fadj;
  vnl_vector<double> vectValPadj;
  vnl_symmetric_eigensystem_compute(Aadj, Fadj, vectValPadj);

  InternalMatrixType transf = Fn * Fadj;
  transf.inplace_transpose();

  if (this->GetNumberOfPrincipalComponentsRequired() != this->GetInput()->GetNumberOfComponentsPerPixel())
    this->m_TransformationMatrix = transf.get_n_rows(0, this->GetNumberOfPrincipalComponentsRequired());
  else
    this->m_TransformationMatrix = transf;

  this->m_EigenValues.SetSize(this->GetNumberOfPrincipalComponentsRequired());
  for (unsigned int i                                                           = 0; i < this->GetNumberOfPrincipalComponentsRequired(); ++i)
    this->m_EigenValues[this->GetNumberOfPrincipalComponentsRequired() - 1 - i] = static_cast<RealType>(vectValPadj[i]);
}


} // end of namespace otb

#endif
