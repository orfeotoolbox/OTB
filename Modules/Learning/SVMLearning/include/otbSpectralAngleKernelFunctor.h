/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) GET / ENST Bretagne. All rights reserved.
  See GETCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbSpectralAngleKernelFunctor_h
#define __otbSpectralAngleKernelFunctor_h

#include "svm.h"

namespace otb
{

/** \class SpectralAngleKernelFunctor
 * \brief Spectral Angle Kernel
 *
 * Performs an inverse multiquadric kernel evaluation that suits
 * spectral data.
 *
 * It is of kind:
 *
 * \f$ frac{1}{\sqrt{ c_0 + \cos\textrm{SAM(x, y)} }} \f$ where \f$ \textrm{SAM(x, y)} \f$ is
 * Spectral Angle Measure:
 * \f$ \textrm{SAM}(x, y) = \cos^{-1}\left( \frac{x \cdot y}{\|x\| \|y\|} \right) \f$.
 *
 * The only parameter \f$ c_0 \f$ is to be fixed by the \code SetValue \endcode
 * interface with keyword Coef (def 2.0).
 *
 */

class SpectralAngleKernelFunctor
  : public GenericKernelFunctorBase
{
public:
  typedef SpectralAngleKernelFunctor   Self;
  typedef GenericKernelFunctorBase     Superclass;

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

  double operator ()(const svm_node * x, const svm_node * y,
                     const svm_parameter& param) const;

  SpectralAngleKernelFunctor ();
  virtual ~SpectralAngleKernelFunctor () {}

  /** Specific implementation of \code Update \endcode to split m_MapParameters
   * into specific variables to speed up kernel evaluations */
  void Update();

protected:
  SpectralAngleKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  SpectralAngleKernelFunctor&
  operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    Update();
    return *this;
  }

private:

  double SAM(const svm_node * x, const svm_node * y) const;

  double m_Coef;
};

} // end of namespace otb

#endif
