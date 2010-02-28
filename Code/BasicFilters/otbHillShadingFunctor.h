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
#ifndef __otbHillShadingFunctor_h
#define __otbHillShadingFunctor_h

#include "otbMath.h"

namespace otb
{
namespace Functor
{
/** \class HillShadingFunctor
 *  \brief Unary neighborhood functor to compute the lambertian of a surface
 *
 *  The light source is assumed to be at a given elevation and azimuth
 *  (by default 45 and 30 respectively). This is used to generate hill shading
 *  representation of relief. The output is a value between 0 and 1.
 *
 *  \ingroup Functor
*/
template <class TNeighIter, class TInputImage, class TOutput>
class HillShadingFunctor
{
public:

  typedef HillShadingFunctor Self;
  typedef TNeighIter         IteratorType;

  HillShadingFunctor(): m_AzimuthLight(30.0/180.0*CONST_PI), m_ElevationLight(45.0/180.0*CONST_PI),
                        m_XRes(100.0), m_YRes(100.0), m_Scale(1.0)
  {
    m_SinElev = vcl_sin(m_ElevationLight);
    m_CosElev = vcl_cos(m_ElevationLight);
    m_SinAz = vcl_sin(m_AzimuthLight);
    m_CosAz = vcl_cos(m_AzimuthLight);
  }
  ~HillShadingFunctor() {}

  inline TOutput operator()(const TNeighIter & it)
  {
    const typename IteratorType::OffsetType LEFT   ={{-1,0}};
    const typename IteratorType::OffsetType RIGHT  ={{1,0}};
    const typename IteratorType::OffsetType UP     ={{0,-1}};
    const typename IteratorType::OffsetType DOWN   ={{0,1}};
    const typename IteratorType::OffsetType LEFTUP   ={{-1,-1}};
    const typename IteratorType::OffsetType RIGHTDOWN ={{1,1}};
    const typename IteratorType::OffsetType RIGHTUP  ={{1,-1}};
    const typename IteratorType::OffsetType LEFTDOWN ={{-1,1}};
//    const typename IteratorType::OffsetType CENTER ={{0,0}};

    double xSlope = ((it.GetPixel(LEFTUP)+2*it.GetPixel(LEFT)+it.GetPixel(RIGHTDOWN))
        -(it.GetPixel(RIGHTUP)+2*it.GetPixel(RIGHT)+it.GetPixel(RIGHTDOWN)))
        /(m_XRes*m_Scale);
    // - as the azimuth is given compared to y axis pointing up
    double ySlope = -((it.GetPixel(LEFTUP)+2*it.GetPixel(UP)+it.GetPixel(RIGHTUP))
        -(it.GetPixel(LEFTDOWN)+2*it.GetPixel(DOWN)+it.GetPixel(RIGHTDOWN)))
        /(m_YRes*m_Scale);

    // permutation between x and y as the azimuth angle is given compared to the north-south axis
    double lambertian = ((m_SinElev*m_CosAz*ySlope)+(m_SinElev*m_SinAz*xSlope)+m_CosElev)
        / vcl_sqrt(xSlope*xSlope+ySlope*ySlope+1);

    return (lambertian+1)/2; //normalize between 0 and 1

  }

private:
  HillShadingFunctor(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  double m_AzimuthLight;
  double m_ElevationLight;
  double m_XRes;
  double m_YRes;
  double m_Scale;

  double m_SinElev;
  double m_CosElev;
  double m_SinAz;
  double m_CosAz;

};

}
}

#endif
