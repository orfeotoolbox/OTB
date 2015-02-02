/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom; Telecom bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbCzihoSOMLearningBehaviorFunctor_h
#define __otbCzihoSOMLearningBehaviorFunctor_h

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
*  why it is necessary to call a specific method for \code IterationThreshold \endcode intialization.
*
*  \sa SOM
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
      static_cast<double>(iterMax) * (1.0 - 1.0 / ::vcl_sqrt(V0)));
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
