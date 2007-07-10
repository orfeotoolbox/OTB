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
#ifndef _otbNonGaussianRBFKernelFunctor_h
#define _otbNonGaussianRBFKernelFunctor_h

#include "svm.h"

namespace otb
{

class NonGaussianRBFKernelFunctor
		: public GenericKernelFunctorBase
{
public:
	/** Non Gaussian RBF.
	 * Performs an RBF kernel evaluation that better suit sample distribution
	 * with high Kurtosis.
	 * It is of kind 
	 * $\exp\left( - \gamma \sum_i | x_i^\alpha - y_i^\alpha |^\beta \right)$
	 * where $0 \leqslant \alpha \leqslant 1$ and 
	 * $0 \leqslant \beta \leqslant 2$.
	 *
	 * Variables to be instanciated (through \code{SetValue}) are:
	 * Alpha (def=1), Beta (def=2) and Gamma (def 1.0).
	 * */
	double operator() ( const svm_node * x, const svm_node * y,
						const svm_parameter & param ) const;
	
	NonGaussianRBFKernelFunctor ();
	virtual ~NonGaussianRBFKernelFunctor () { }

	/** Specific implementation of \code{Update} to split m_MapParameters
	 * into specific variables to speed up kernel evaluations */
	void Update ();
	
protected:
	double m_Alpha;
	double m_Beta;
	double m_Gamma;
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbNonGaussianRBFKernelFunctor.txx"
#endif

#endif


