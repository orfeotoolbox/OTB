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
#ifndef __otbChangeProfileKernelFunctor_h
#define __otbChangeProfileKernelFunctor_h

#include "svm.h"

namespace otb
{

/**
 * \class ChangeProfileKernelFunctor
 * \brief Change Profile Kernel.
 *
 * Performs a decision point of view dedicated to the multiscale
 * change profile delivered by otb::KullbackLeiblerProfileImageFilter.
 *
 * It is implemented as a polynomial kernel:
 * \f$ \exp - \left( \gamma \left( \max_i | x_i - y_i | \right)^\textrm{degree} + \textrm{coef} \right) \f$.
 *
 * The parameters \f$ \gamma \f$, \f$ \textrm{coef} \f$ and \f$ \textrm{degree} \f$ are
 * to be fixed through \code SetValue \endcode with keywords: Gamma (def 1.0),
 * Coef (def 1.0) and Degree (def 1.0).
 */

class ChangeProfileKernelFunctor
  : public GenericKernelFunctorBase
{
public:
  typedef ChangeProfileKernelFunctor   Self;
  typedef GenericKernelFunctorBase     Superclass;

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

  double operator ()(const svm_node * x, const svm_node * y,
                     const svm_parameter& param) const;

  ChangeProfileKernelFunctor ();
  virtual ~ChangeProfileKernelFunctor () {}

  /** Specific implementation of \code Update \endcode to split m_MapParameters
   * into specific variables to speed up kernel evaluations */
  void Update();

protected:
  ChangeProfileKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  ChangeProfileKernelFunctor&
  operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    Update();
    return *this;
  }

private:
  double m_Coef;
  double m_Degree;
  double m_Gamma;
};

} // end of namespace otb

#endif
