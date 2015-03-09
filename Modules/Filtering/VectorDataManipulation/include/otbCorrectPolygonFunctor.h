/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbCorrectPolygonFunctor_h
#define __otbCorrectPolygonFunctor_h

#include "otbSimplifyPathFunctor.h"
#include "otbClosePathFunctor.h"

namespace otb
{

/** \class CorrectPolygonFunctor
*    \brief This filter simplify and close the input polygon, making the last point equal to the first one.
*
* This filter may be useful when a truely closed polygon is needed (to draw it for example)
*
* \sa UnaryFunctorObjectListFilter
*
 *  \ingroup Functor
 *
 * \ingroup OTBVectorDataManipulation
 */
template <class TPolygon>
class CorrectPolygonFunctor
{
public:

  typedef TPolygon                      PolygonType;
  typedef typename PolygonType::Pointer PolygonPointerType;

  /** Some typedefs specific to functors*/
  typedef otb::SimplifyPathFunctor<PolygonType, PolygonType> SimplifyFunctorType;
  typedef ClosePathFunctor <PolygonType, PolygonType>        CloseFunctorType;
  CorrectPolygonFunctor()
  {}
  ~CorrectPolygonFunctor() {}

  inline PolygonPointerType operator ()(const TPolygon * input)
  {
    /**create functors */
    SimplifyFunctorType simplifyFunctor;
    CloseFunctorType    closeFunctor;

    simplifyFunctor.SetTolerance(0.0);

    /** Erase aligned points and close polygon*/
    return closeFunctor (simplifyFunctor(input));

  }

};

}

#endif
