/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbCzihoSOMLearningBehaviorFunctor_h
#define otbCzihoSOMLearningBehaviorFunctor_h

#include "itkSize.h"
#include "otbMath.h"

namespace otb
{

namespace Functor
{

/** \class CzihoSOMLearningBehaviorFunctor
* \brief Beta behavior over SOM training phase
*
*  This class implements an evolution of the \f$ \beta \f$ weightening
*  coefficient over the SOM training. It is issued from A. Cziho's PhD:
*  "Compression d'images et analyse de contenu par quantification vectorielle"
*  PhD dissertation, University of Rennes I, Rennes, France. May 5th, 1999.
*
*  Its behavior is decomposed into two steps depending on the number of iterations:
*  \f[
       \beta =
       \begin{cases}
   \beta_0 \left( 1 - \frac{t}{t_0} \right) & \textrm{ if } t < t_0 \\
       \beta_{\textrm{end}} \left( 1- \frac{t-t_O}{t_{\textrm{end}}-t_0} \right) & \textrm{ if } t_0 \leqslant t < t_{\textrm{end}}
   \end{cases}
   \f]
* where \f$ t_0 \f$ stands for IterationThreshold.
*
*  CzihoSOMLearningBehaviorFunctor uses some parameters of the SOM class such as:
*  BetaInit, BetaEnd, NumberOfIterations, but also NeighborhoodSizeInit which may be
*  (surprisingly) required for the IterationThreshold.
*
*  The functor function uses \code NumberOfIterations \endcode, \code BetaInit \endcode, \code BetaEnd \endcode parameters, that is
*  why it is necessary to call a specific method for \code IterationThreshold \endcode initialization.
*
*  \sa SOM
 *
 * \ingroup OTBSOM
*/

class CzihoSOMLearningBehaviorFunctor
{
public:
  /** Empty constructor / descructor */
  CzihoSOMLearningBehaviorFunctor ()
  {
    m_IterationThreshold = 0;
  }
  virtual ~CzihoSOMLearningBehaviorFunctor() {}

  /** Accessors */
  unsigned int GetIterationThreshold()
  {
    return this->m_IterationThreshold;
  }

  template <unsigned int VDimension>
  void SetIterationThreshold(const itk::Size<VDimension>& sizeInit, unsigned int iterMax)
  {
    double V0 = static_cast<double>(sizeInit[0]);
    for (unsigned int i = 1; i < VDimension; ++i)
      {
      if (V0 < static_cast<double>(sizeInit[i])) V0 = static_cast<double>(sizeInit[i]);
      }

    m_IterationThreshold = static_cast<unsigned int>(
      static_cast<double>(iterMax) * (1.0 - 1.0 / ::std::sqrt(V0)));
  }

  /** Functor */
  virtual double operator ()(unsigned int currentIteration,
                             unsigned int numberOfIterations,
                             double betaInit, double betaEnd) const
  {
    if (currentIteration < m_IterationThreshold)
      {
      return betaInit * (1.0
                         - static_cast<double>(currentIteration)
                         / static_cast<double>(numberOfIterations));
      }
    else
      {
      return betaEnd * (1.0
                        - static_cast<double>(currentIteration - m_IterationThreshold)
                        / static_cast<double>(numberOfIterations - m_IterationThreshold));
      }
  }

private:
  unsigned int m_IterationThreshold;

}; // end of class CzihoSOMLearningBehaviorFunctor

} // end namespace Functor

} // end namespace otb

#endif
