/*
 * VahineInterpol.h
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

#ifndef __VAHINEINTERPOL_H
#define __VAHINEINTERPOL_H

#include <vnl/vnl_vector_ref.h>
#include "VahineMacro.h"

namespace vahine {

/**
 * Class of interpolation
 * For use it you can set \a Vx and \a Vy values and after call \a Update method.
 * You can get the interpolate value y of a input data x with the method \a Get(x)
 */
template< typename RealType >
class VahineInterpol {
public:
  typedef vnl_vector<RealType> VectorType;
  VahineInterpol();
  VahineInterpol(unsigned int size);
  ~VahineInterpol();
  RealType get(RealType x);

  bool GetDebug(){
    return true;
  };
  std::string GetNameOfClass(){
    return "VahineInterpol";
  }
  void SetVx(VectorType vx);

  VectorType GetVx();

  void SetVy(VectorType vy);

  VectorType GetVy();

  void Update();

private:
  unsigned int m_size;
  /**
   * vector of coefs for linear interpolation
   */
  VectorType m_aCoefs;
  VectorType m_vx;
  VectorType m_vy;
  void intervals(VectorType &v, VectorType &h);

  void linearCoeffs(VectorType &dx, VectorType &dy);

  void linear();

};

} // End namespace vahine

#include "VahineInterpol.txx"

#endif /* VAHINEINTERPOL_H_ */