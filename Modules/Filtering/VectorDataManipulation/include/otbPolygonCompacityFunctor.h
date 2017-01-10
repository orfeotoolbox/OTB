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
#ifndef otbPolygonCompacityFunctor_h
#define otbPolygonCompacityFunctor_h

#include "otbMath.h"

namespace otb
{

/** \class PolygonCompacityFunctor
 *  \brief Select polygons according to their compacity
 *
* This functor compute the compacity of a polygon
* and return true is the distance is above the threshold, false otherwise.
*
* The compacity is defined as:
*
*  \f$ 4\pi \frac{A}{L^2}\f$
*
* where \f$ A \f$ is the area (obtained by the method GetArea() )
* and \f$ L \f$ the perimeter (obtained by the method GetLength() ).
 *
 *  \ingroup Functor
 *
 * \ingroup OTBVectorDataManipulation
 */
template <class TInput1>
class PolygonCompacityFunctor
{
public:
  void SetThreshold(double threshold)
  {
    m_Threshold = threshold;
  }
  double GetThreshold(void) const
  {
    return m_Threshold;
  }

  PolygonCompacityFunctor()
  {
    m_Threshold = 0.2;
  }
  ~PolygonCompacityFunctor() {}

  inline bool operator ()(const TInput1& input)
  {
    double circularityRatio = 4*CONST_PI*input->GetArea()
                              / vnl_math_sqr(input->GetLength());

    if (circularityRatio > m_Threshold)
      {
      return true;
      }
    else
      {
      return false;
      }
  }

private:
  double m_Threshold;
};

}

#endif
