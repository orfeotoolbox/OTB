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
#ifndef __otbNonGaussianRBFKernelFunctor_h
#define __otbNonGaussianRBFKernelFunctor_h

#include "otb_libsvm.h"

//FIXME: shouldn't it be in the Functor namespace?
namespace otb
{
/** \class NonGaussianRBFKernelFunctor
 * \brief Performs an RBF kernel evaluation that better suit sample distribution with high Kurtosis.
 *
 * It is of kind
 * \f$ \exp\left( - \gamma \sum_i | x_i^\alpha - y_i^\alpha |^\beta \right) \f$
 * where \f$ 0 \leqslant \alpha \leqslant 1 \f$ and
 * \f$ 0 \leqslant \beta \leqslant 2 \f$.
 *
 * Variables to be instanciated (through \code SetValue \endcode) are:
 * Alpha (def=1), Beta (def=2) and Gamma (def 1.0).
 *
 *
 * \ingroup OTBSVMLearning
 */
class NonGaussianRBFKernelFunctor
  : public GenericKernelFunctorBase
{
public:
  typedef NonGaussianRBFKernelFunctor  Self;
  typedef GenericKernelFunctorBase     Superclass;

  // Deep copy operator
  virtual GenericKernelFunctorBase* Clone() const
  {
    return new Self(*this);
  }

  double operator ()(const svm_node * x, const svm_node * y,
                     const svm_parameter& param) const;

  NonGaussianRBFKernelFunctor ();
  virtual ~NonGaussianRBFKernelFunctor () {}

  /** Specific implementation of \code Update \endcode to split m_MapParameters
   * into specific variables to speed up kernel evaluations */
  void Update();

protected:
  NonGaussianRBFKernelFunctor(const Self& copy)
  : Superclass(copy)
  {
    *this = copy;
  }

  NonGaussianRBFKernelFunctor&
  operator=(const Self& copy)
  {
    Superclass::operator =(copy);
    Update();
    return *this;
  }

protected:
  double m_Alpha;
  double m_Beta;
  double m_Gamma;

};

} // end of namespace otb

#endif
