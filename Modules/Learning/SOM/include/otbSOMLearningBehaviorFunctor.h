/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom bretagne. All rights reserved.
  See IMTCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef otbSOMLearningBehaviorFunctor_h
#define otbSOMLearningBehaviorFunctor_h

namespace otb
{

namespace Functor
{

/** \class SOMLearningBehaviorFunctor
* \brief Classical Beta behavior over SOM training phase
*
*  This class implements an standart evolution of the \f$ \beta \f$ weightening
*  coefficient over the SOM training.
*
*  \f[
  \beta = \beta_0 + \left( \beta_{\textrm{end}} - \beta_0 \right)
    \frac{t}{t_{\textrm{end}}}
  \f]
*
*  \sa SOM
 *
 * \ingroup OTBSOM
*/
class SOMLearningBehaviorFunctor
{
public:
  /** Empty constructor / descructor */
  SOMLearningBehaviorFunctor () {}
  virtual ~SOMLearningBehaviorFunctor() {}

  /** Functor */
  virtual double operator ()(unsigned int currentIteration,
                             unsigned int numberOfIterations,
                             double betaInit, double betaEnd) const
  {
    return betaInit + (betaEnd - betaInit)
           * static_cast<double>(currentIteration)
           / static_cast<double>(numberOfIterations);
  }

}; // end of class SOMLearningBehaviorFunctor

} // end namespace Functor

} // end namespace otb

#endif
