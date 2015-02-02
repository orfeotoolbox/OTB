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

#ifndef __otbCzihoSOMNeighborhoodBehaviorFunctor_h
#define __otbCzihoSOMNeighborhoodBehaviorFunctor_h

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
*/
class CzihoSOMNeighborhoodBehaviorFunctor
{
public:
  /** Empty constructor / descructor */
  CzihoSOMNeighborhoodBehaviorFunctor () {}
  virtual ~CzihoSOMNeighborhoodBehaviorFunctor() {}

  /** Functor */
  template <unsigned int VDimension>
  itk::Size<VDimension> operator ()(unsigned int currentIteration,
                                    unsigned int numberOfIterations,
                                    const itk::Size<VDimension>& sizeInit) const
  {
    itk::Size<VDimension> theSize;
    double                weightening = ::vcl_pow(1.0
                                                  - static_cast<double>(currentIteration)
                                                  / static_cast<double>(numberOfIterations),
                                                  2.0);
    for (unsigned int i = 0; i < VDimension; ++i)
      {
      theSize[i] = static_cast<typename itk::Size<VDimension>::SizeValueType> (
        static_cast<double>(sizeInit[i]) * weightening);
      }

    return theSize;
  }

}; // end of class CzihoSOMNeighborhoodBehaviorFunctor

} // end namespace Functor

} // end namespace otb

#endif
