/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbCzihoSOMNeighborhoodBehaviorFunctor_h
#define otbCzihoSOMNeighborhoodBehaviorFunctor_h

#include "itkSize.h"
#include "itkMath.h"

namespace otb
{

namespace Functor
{

/** \class CzihoSOMNeighborhoodBehaviorFunctor
*  \brief Neighborhood size behavior over SOM training phase
*
*  This class implements an evolution of the neighborhood size
*  over the SOM training. It is issued from A. Cziho's PhD:
*  "Compression d'images et analyse de contenu par quantification vectorielle"
*  PhD dissertation, University of Rennes I, Rennes, France. May 5th, 1999.
*
*  The behavior of the radius r (\em ie. \code SizeType \endcode component 0, 1, ...) is given by:
*  \f$
    r = r_{\textrm{init}} \left( 1 - \frac{t}{t_{\textrm{end}}} \right)^2
  \f$
*
*  CzihoSOMNeighborhoodBehaviorFunctor uses some parameters of the SOM class such as:
*  \code NeighborhoodSizeInit \endcode, \code NumberOfIterations \endcode which are parameters of the functor function.
*  \code operator() \endcode is templated with the dimension of the neighborhoodSize.
*
*  \sa SOM
 *
 * \ingroup OTBSOM
*/
class CzihoSOMNeighborhoodBehaviorFunctor
{
public:
  /** Empty constructor / descructor */
  CzihoSOMNeighborhoodBehaviorFunctor()
  {
  }
  virtual ~CzihoSOMNeighborhoodBehaviorFunctor()
  {
  }

  /** Functor */
  template <unsigned int VDimension>
  itk::Size<VDimension> operator()(unsigned int currentIteration, unsigned int numberOfIterations, const itk::Size<VDimension>& sizeInit) const
  {
    itk::Size<VDimension> theSize;
    double                weightening = ::std::pow(1.0 - static_cast<double>(currentIteration) / static_cast<double>(numberOfIterations), 2.0);
    for (unsigned int i = 0; i < VDimension; ++i)
    {
      theSize[i] = static_cast<typename itk::Size<VDimension>::SizeValueType>(static_cast<double>(sizeInit[i]) * weightening);
    }

    return theSize;
  }

}; // end of class CzihoSOMNeighborhoodBehaviorFunctor

} // end namespace Functor

} // end namespace otb

#endif
