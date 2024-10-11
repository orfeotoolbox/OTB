/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
 * Copyright (C) 2007-2012 Institut Mines Telecom / Telecom Bretagne
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef otbSOMLearningBehaviorFunctor_h
#define otbSOMLearningBehaviorFunctor_h

namespace otb
{

namespace Functor
{

/** \class SOMLearningBehaviorFunctor
* \brief Classical Beta behavior over SOM training phase
*
*  This class implements an standard evolution of the \f$ \beta \f$ weightening
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
  SOMLearningBehaviorFunctor()
  {
  }
  virtual ~SOMLearningBehaviorFunctor()
  {
  }

  /** Functor */
  virtual double operator()(unsigned int currentIteration, unsigned int numberOfIterations, double betaInit, double betaEnd) const
  {
    return betaInit + (betaEnd - betaInit) * static_cast<double>(currentIteration) / static_cast<double>(numberOfIterations);
  }

}; // end of class SOMLearningBehaviorFunctor

} // end namespace Functor

} // end namespace otb

#endif
