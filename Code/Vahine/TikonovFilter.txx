/**
 * TikonovFilter.txx
 *
 * Vahine Framework
 * Copyright (C) 2008, 2009 Ludovic Léau-Mercier and Laboratoire de Planétologie de Grenoble
 * See LICENCE and COPYING for details.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * CeCILLl License for more details and http://www.cecill.info
 *
 */
#ifndef __VAHINETIKONOVFILTER_TXX
#define __VAHINETIKONOVFILTER_TXX

#include "TikonovFilter.h"

namespace otb {

template<class TInputImage>
VahineTikonovFilter<TInputImage>::VahineTikonovFilter(){
  m_statsFilter = StatsFilterType::New();
  // default regularisation parameter
  m_RegularizationParam = 1E-1;
  m_covUpdated = false;
}

template<class TInputImage>
void VahineTikonovFilter<TInputImage>::UpdateCov(){
  InputImagePointer image = const_cast<InputImageType *>(this->GetInput() );

  image->Update();
  SizeType size = image->GetLargestPossibleRegion().GetSize();
  //ConstIterator it(image, image->GetRequestedRegion());

  m_statsFilter->SetInput(image);
  m_statsFilter->Update();
  m_covMatrix = m_statsFilter->GetCovariance();
  unsigned int N = size[0]*size[1];
  m_covMatrix *= static_cast<double>(N)/(N-1);
  m_covUpdated = true;
}

template<class TInputImage>
void VahineTikonovFilter<TInputImage>::GenerateData(){
  if(!m_covUpdated) {
    UpdateCov();
    }
  vahineDebug("regul = "<< m_RegularizationParam);
  vahineDebug("cov matrix size = " << m_covMatrix.Rows() <<"*"<< m_covMatrix.Cols() );
  int        N = m_covMatrix.Cols();
  MatrixType reg, reg_t, cov_t;
  reg.SetSize(N, N);
  reg.SetIdentity();
  // TODO refactor trans(Identity) * Identity = Identity
  reg_t = reg.GetTranspose();
  reg = reg_t * reg;
  reg *= m_RegularizationParam;

  vahineDebug("reg size = "<<reg.Rows()<<"*"<<reg.Cols() );

  cov_t = m_covMatrix.GetTranspose();

  m_tikonovMatrix = (cov_t * m_covMatrix) + reg;
  m_tikonovMatrix = m_tikonovMatrix.GetInverse();
  m_tikonovMatrix = m_tikonovMatrix * cov_t;
  vahineDebug("sigma size = "<<m_tikonovMatrix.Rows()<<"*"<<m_tikonovMatrix.Cols() );
}

}
#endif