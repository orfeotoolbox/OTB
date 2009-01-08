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
#ifndef __otbInverseCosSpectralAngleKernelFunctor_h
#define __otbInverseCosSpectralAngleKernelFunctor_h

#include "svm.h"

namespace otb
{

class InverseCosSpectralAngleKernelFunctor
		: public GenericKernelFunctorBase
{
public:
	/** Inverse Cosinus Spectral Angle Kernel.
	 * Performs an inverse multiquadric kernel evaluation that suit
	 * spectral data.
	 * It is of kind:
	 * $\frac{1}{\sqrt{ c_0 - \cos\textrm{SAM(x,y)} }}$ where $\textrm{SAM(x,y)}$ is
	 * Spectral Angle Measure:
	 * $\textrm{SAM(x,y) = \acos\left( \frac{x \cdot y}{\|x\| \|y\|} \right)$.
	 *
	 * The only parameter $c_0$ is to be fixed by the \code SetValue \endcode
	 * interface with keyword Coef (def 1.0).
	 */
	double operator() ( const svm_node * x, const svm_node * y,
						const svm_parameter & param ) const;

	InverseCosSpectralAngleKernelFunctor ();
	virtual ~InverseCosSpectralAngleKernelFunctor () { }

	/** Specific implementation of \code Update \endcode to split m_MapParameters
	 * into specific variables to speed up kernel evaluations */
	void Update ();

protected:
	inline	double	SAM ( const svm_node * x, const svm_node * y ) const;

	double m_Coef;
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbInverseCosSpectralAngleKernelFunctor.txx"
#endif


#endif


