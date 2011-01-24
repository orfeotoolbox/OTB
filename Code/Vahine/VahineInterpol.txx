/**
 * VahineInterpol.txx
 *
 * Interpolation class for vahine project
 * This class set in vector object several tuple of value (vx, vy)
 * and calculate a linear interpolation for each range
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

#ifndef __VAHINEINTERPOL_TXX
#define __VAHINEINTERPOL_TXX

#include "VahineInterpol.h"

namespace vahine {
template< typename RealType >
VahineInterpol<RealType>::VahineInterpol(){
}

template< typename RealType >
VahineInterpol<RealType>::VahineInterpol(unsigned int size) {
  //vahineDebug("Create interpol size = "<<size);
  m_size = size;
  m_vx = VectorType(size);
  m_vy = VectorType(size);
  m_aCoefs = VectorType(size);
}

template< typename RealType >
VahineInterpol<RealType>::~VahineInterpol() {
}

template< typename RealType>
void VahineInterpol<RealType>::SetVx(VectorType vx){
  m_vx = vx;
}

template< typename RealType>
typename VahineInterpol<RealType>::VectorType VahineInterpol<RealType>::GetVx(){
  return m_vx;
}

template< typename RealType>
void VahineInterpol<RealType>::SetVy(VectorType vy){
  m_vy = vy;
}

template< typename RealType>
typename VahineInterpol<RealType>::VectorType VahineInterpol<RealType>::GetVy(){
  return m_vy;
}

/**
 * Calculate the interpolation with the current method
 * Actually only linear method are available
 */
template < typename RealType>
void VahineInterpol<RealType>::Update(){
  this->linear();
}

template< typename RealType >
void
VahineInterpol<RealType>::intervals(VectorType &v, VectorType &h){
  vahineDebug("intervals");
  for(int i=0; i<m_size-1; ++i)
    h[i] = v[i+1]-v[i];
  h[m_size-1] = 0.0;   //to complete vector
}

template< typename RealType >
void VahineInterpol<RealType>::linearCoeffs(VectorType &dx, VectorType &dy){
  for(int i = 0; i<m_size-1; i++)
    m_aCoefs[i] = dy[i]/dx[i];
}

/**
 * This method calculate all linear coef \a m_aCoefs for each range
 */
template< typename RealType >
void VahineInterpol<RealType>::linear(){
  VectorType dx = VectorType(m_size);

  this->intervals(m_vx, dx);
  VectorType dy = VectorType(m_size);
  this->intervals(m_vy, dy);
  this->linearCoeffs(dx, dy);
}

template<typename RealType>
RealType VahineInterpol<RealType>::get(RealType x){
  // find appropriate segment
  //unsigned int n = m_vx.size();
  // interpolation outside the range is a constant value.
  if (x<m_vx[0]) {
    //  TODO add warning
    //std::cout<<"interpolation inf range"<<m_vx[0]<<std::endl;
    return m_vy[0];
    }
  if (x>m_vx[m_size-1]) {
    // TODO add warning
    //std::cout<<"interpolation sup range "<<m_vx[n-1]<<std::endl;
    return m_vy[m_size-1];
    }
  // interpolation in the range
  int i;
  for(i=0; i<m_size; i++) if (x <m_vx[i]) break;
  // evaluate linear interpolation for i--
  i--;
  double t = m_aCoefs[i]*x + m_vy[i] - m_aCoefs[i]*m_vx[i];
  return t;
}

}

#endif