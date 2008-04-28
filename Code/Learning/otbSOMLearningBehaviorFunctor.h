/*=========================================================================

Program:   ORFEO Toolbox
Language:  C++
Date:      $Date$
Version:   $Revision$


Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.

Copyright (c) Institut Telecom ; Telecom bretagne. All rights reserved. 
See ITCopyright.txt for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbSOMLearningBehaviorFunctor_h
#define __otbSOMLearningBehaviorFunctor_h


namespace otb {

namespace Functor {

	/** \class SOMLearningBehaviorFunctor
	* \brief Classical Beta behavior over SOM training phase
	*
	*	This class implements an standart evolution of the $\beta$ weightening
	*	coefficient over the SOM training. 
	*
	*	\f{equation*}
		\beta = \beta_0 + \left( \beta_{\text{end}} - \beta_0 \right)
			\frac{t}{t_{\text{end}}}
		\f{equation*} 
	*
	*	\sa SOM
	*/
	class SOMLearningBehaviorFunctor
	{
		public :
			/** Empty constructor / descructor */
			CzihoSOMLearningBehaviorFunctor () { }
			virtual ~CzihoSOMLearningBehaviorFunctor() { }

			/** Functor */
			virtual double operator() ( unsigned int currentIteration, 
										unsigned int numberOfIterations,
										double betaInit, double betaEnd )
			{
				return betaInit + ( betaEnd - betaInit )
							* static_cast<double>( currentIteration )
							/ static_cast<double>(  numberOfIterations );
			}

	}; // end of class SOMLearningBehaviorFunctor

} // end namespace Functor

} // end namespace otb

#endif

					
