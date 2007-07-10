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
#ifndef _otbChangeProfileKernelFunctor_h
#define _otbChangeProfileKernelFunctor_h

#include "svm.h"

namespace otb
{

class ChangeProfileKernelFunctor
		: public GenericKernelFunctorBase
{
public:
	/** Change Profile Kernel.
	 * Performs a decision point of view dedicated to the multiscale
	 * change profile delivered by 
	 * \subdoxygen{otb}{KullbackLeiblerProfileImageFilter}.
	 *
	 * It is implemented as a polynomial kernel:
	 * $\exp - \left( \gamma \left( \max_i | x_i - y_i | \right)^\text{degree} + \text{coef} \right)$.
	 *
	 * The parameters $\gamma$, $\text{coef}$ and $\text{degree}$ are 
	 * to be fixed through \code{SetValue} with keywords: Gamma (def 1.0),
	 * Coef (def 1.0) and Degree (def 1.0).
	 */
	double operator() ( const svm_node * x, const svm_node * y,
						const svm_parameter & param ) const;

	ChangeProfileKernelFunctor ();
	virtual ~ChangeProfileKernelFunctor () { }

	/** Specific implementation of \code{Update} to split m_MapParameters
	 * into specific variables to speed up kernel evaluations */
	void Update ();

private:
	double m_Coef;
	double m_Degree;
	double m_Gamma;
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbChangeProfileKernelFunctor.txx"
#endif


#endif


