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
#ifndef _otbMixturePolyRBFKernelFunctor_h
#define _otbMixturePolyRBFKernelFunctor_h

#include "svm.h"

namespace otb
{

class MixturePolyRBFKernelFunctor
		: public GenericKernelFunctorBase
{
public:
	/** Mixture of kernels.
	 * Performs the mixture of kind: $\mu k_1(x,y) + (1-\mu) k_2(x,y)$
	 * with $k_1(x,y)=\left( \gamma_1 x\cdot y + c_0 \right) ^d$ a 
	 * polynomial kernel and 
	 * $k_2(x,y) = \exp\left( - \gamma_2 \| x-y\-^2 \right)$ an RBF one. 
	 *
	 * Variable to be instanciated (through \code{SetValue}) are:
	 * Mixture (def=0.5), GammaPoly (def=1.0), CoefPoly (def=1.0),
	 * DegreePoly (def=2), GammaRBF (def=1.0) */
	double operator() ( const svm_node * x, const svm_node * y,
						const svm_parameter & param ) const;

	MixturePolyRBFKernelFunctor ();
	virtual ~MixturePolyRBFKernelFunctor () { }

	/** Specific implementation of \code{Update} to split m_MapParameters
	 * into specific variables to speed up kernel evaluations */
	void Update ();

protected:
	double m_Mixture;
	double m_GammaPoly;
	double m_CoefPoly;
	int m_DegreePoly;
	double m_GammaRBF;

private:
	inline double powi ( double base, int times ) const;
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbMixturePolyRBFKernelFunctor.txx"
#endif

#endif

