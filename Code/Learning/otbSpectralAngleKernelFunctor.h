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
#ifndef _otbSpectralAngleKernelFunctor_h
#define _otbSpectralAngleKernelFunctor_h

#include "svm.h"

namespace otb
{

class SpectralAngleKernelFunctor
		: public GenericKernelFunctorBase
{
public:
	/** Spectral Angle Kernel.
	 * Performs an inverse multiquadric kernel evaluation that suit
	 * spectral data.
	 * It is of kind:
	 * $\frac{1}{\sqrt{ c_0 + \cos\text{SAM(x,y)} }}$ where $\text{SAM(x,y)}$ is
	 * Spectral Angle Measure: 
	 * $\text{SAM(x,y) = \acos\left( \frac{x \cdot y}{\|x\| \|y\|} \right)$.
	 *
	 * The only parameter $c_0$ is to be fixed by the \code{SetValue}
	 * interface with keyword Coef (def 2.0).
	 */
	double operator() ( const svm_node * x, const svm_node * y,
						const svm_parameter & param ) const;

	SpectralAngleKernelFunctor ();
	virtual ~SpectralAngleKernelFunctor () { }

	/** Specific implementation of \code{Update} to split m_MapParameters
	 * into specific variables to speed up kernel evaluations */
	void Update ();

protected:
	inline	double	SAM ( const svm_node * x, const svm_node * y ) const;

	double m_Coef;
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbSpectralAngleKernelFunctor.txx"
#endif


#endif


