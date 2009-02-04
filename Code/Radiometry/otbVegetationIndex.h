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
#ifndef __otbVegetationIndex_h
#define __otbVegetationIndex_h

#include "otbMath.h"

namespace otb
{
namespace Functor
{
/** \class NDVI
 *  \brief This functor calculate the NormalizeD Vegetation Index (NDVI)
 *
 *  [Pearson et Miller, 1972]
 *
 *  \ingroup Functor
 */
template <class TInput1, class TInput2, class TOutput>
class NDVI
{
public:
  NDVI() {};
  ~NDVI() {};
  inline TOutput operator()(const TInput1 &r, const TInput2 &nir)
  {
    double dr = static_cast<double>(r);
    double dnir = static_cast<double>(nir);
    if ( (nir + r) == 0 )
    {
      return static_cast<TOutput>(0.);
    }

    return ( static_cast<TOutput>((dnir-dr)/(dnir+dr)));
  }
};

/** \class RVI
 *  \brief This functor calculate the Ratio Vegetation Index (RVI)
 *
 *  [Rouse et al., 1973]
 *
 *  \ingroup Functor
 */
template <class TInput1, class TInput2, class TOutput>
class RVI
{
public:
  RVI() {};
  ~RVI() {};
  inline TOutput operator()(const TInput1 &r, const TInput2 &nir)
  {
    double dr = static_cast<double>(r);
    double dnir = static_cast<double>(nir);
    if ( r == 0 )
    {
      return static_cast<TOutput>(0.);
    }
    return ( static_cast<TOutput>(dnir/dr));
  }
};
/** \class PVI
 *  \brief This functor calculate the Perpendicular Vegetation Index (PVI)
 *
 *  [Richardson et Wiegand, 1977]
 *
 *  \ingroup Functor2
 */
template <class TInput1, class TInput2, class TOutput>
class PVI
{
public:
  PVI() {};
  ~PVI() {};
  inline TOutput operator()(const TInput1 &r, const TInput2 &nir)
  {
    double dnir = static_cast<double>(nir);
    double dr = static_cast<double>(r);
    return ( static_cast<TOutput>(  (dnir - m_A*dr - m_B)*m_Coeff) );
  }
  /** Set/Get A and B parameters */
  void SetA(const double A)
  {
    m_A = A;
    m_Coeff = 1./(vcl_sqrt(m_A*m_A + 1.));
  }
  double GetA(void)const
  {
    return (  m_A );
  }
  void SetB(const double B)
  {
    m_B = B;
  }
  double GetB(void)const
  {
    return (  m_B );
  }

private:

  /** A and B parameters */
  double  m_A;
  double  m_B;
  /** Denominator, pre-calculed when the A variable is set */
  double  m_Coeff;

};


/** \class SAVI
 *  \brief This functor calculate the Soil Adjusted Vegetation Index (SAVI)
 *
 *  [Huete, 1988]
 *
 *  \ingroup Functor
 */
template <class TInput1, class TInput2, class TOutput>
class SAVI
{
public:
  SAVI() : m_L(0.5) {};
  ~SAVI() {};
  inline TOutput operator()(const TInput1 &r, const TInput2 &nir)
  {
    double dnir = static_cast<double>(nir);
    double dr = static_cast<double>(r);
    double denominator = dnir + dr + m_L;
    if ( denominator == 0. )
    {
      return static_cast<TOutput>(0.);
    }
    return ( static_cast<TOutput>(  ((dnir-dr)*(1+m_L))/denominator ) );
  }
  /** Set/Get L correction */
  void SetL(const double L)
  {
    m_L = L;
  }
  double GetL(void)const
  {
    return (  m_L );
  }

private:

  /** L correction */
  double  m_L;

};
/** \class TSAVI
 *  \brief This functor calculate the Transformed Soil Adjusted Vegetation Index (TSAVI)
 *
 *  [Baret et al. 1989, Baret et Guyot, 1991]
 *
 *  \ingroup Functor
 */
template <class TInput1, class TInput2, class TOutput>
class TSAVI
{
public:
  TSAVI() : m_X(0.08) {};
  ~TSAVI() {};
  inline TOutput operator()(const TInput1 &r, const TInput2 &nir)
  {
    double dnir = static_cast<double>(nir);
    double dr = static_cast<double>(r);
    double denominator = m_A*dnir + dr + m_X*(1.+m_A*m_A);
    if ( denominator == 0. )
    {
      return static_cast<TOutput>(0.);
    }
    return ( static_cast<TOutput>(  (m_A*(dnir - m_A*dr - m_B))/denominator ) );
  }
  /** Set/Get A and B parameters */
  void SetA(const double A)
  {
    m_A = A;
  }
  double GetA(void)const
  {
    return (m_A);
  }
  void SetB(const double B)
  {
    m_B = B;
  }
  double GetB(void)const
  {
    return (m_B);
  }
  /** Set/Get X parameter */
  void SetX(const double X)
  {
    m_X = X;
  }
  double GetX(void)const
  {
    return (m_X);
  }

private:

  /** A and B parameters */
  double  m_A;
  double  m_B;
  /** X parameter */
  double  m_X;

};

/** \class MSAVI
 *  \brief This functor calculate the Modified Soil Adjusted Vegetation Index (MSAVI)
 *
 *  [Qi et al., 1994]
 *
 *  \ingroup Functor
 */
template <class TInput1, class TInput2, class TOutput>
class MSAVI
{
public:
  MSAVI() {};
  ~MSAVI() {};
  inline TOutput operator()(const TInput1 &r, const TInput2 &nir)
  {
    double dnir = static_cast<double>(nir);
    double dr = static_cast<double>(r);
    double sqrt_value = (2*dnir+1)*(2*dnir+1) - 8*(dnir-dr);
    if ( sqrt_value < 0. )
    {
      return static_cast<TOutput>(0.);
    }
    return ( static_cast<TOutput>(  (2*dnir + 1 - vcl_sqrt(sqrt_value))/2. ) );
  }

};

/** \class ARVI
 *  \brief This functor calculate the Atmospherically Resistant Vegetation Index (ARVI)
 *
 *  This vegetation index use three inputs channels
 *
 *  [Yoram J. Kaufman and Didier Tanr�, 1992]
 *
 *  \ingroup Functor
 */
template <class TInput1, class TInput2, class TInput3, class TOutput>
class ARVI
{
public:
  ARVI() : m_Gamma(0.5) {};
  ~ARVI() {};
  inline TOutput operator()(const TInput1 &r, const TInput2 &b, const TInput3 &nir)
  {
    double dr = static_cast<double>(r);
    double db = static_cast<double>(b);
    double dnir = static_cast<double>(nir);
    double RHOrb = dr - m_Gamma*(db - dr);
    double denominator = dnir + RHOrb;
    if ( denominator == 0. )
    {
      return static_cast<TOutput>(0.);
    }
    return ( static_cast<TOutput>(  (dnir - RHOrb)/denominator ) );
  }
  /** Set/Get Gamma parameter */
  void SetGamma(const double gamma)
  {
    m_Gamma = gamma;
  }
  double GetGamma(void)const
  {
    return (m_Gamma);
  }

private:

  /** Gamma parameter */
  double  m_Gamma;
};




/** \class TSARVI
 *  \brief This functor calculate the Transformed Soil Atmospherical Resistant Vegetation Index (TSARVI)
 *
 *  [Yoram J. Kaufman and Didier Tanré, 1992]
 *
 *  \ingroup Functor
 */
template <class TInput1, class TInput2, class TInput3, class TOutput>
class TSARVI
{
public:
  TSARVI() : m_X(0.08), m_Gamma(0.5) {};
  ~TSARVI() {};
  inline TOutput operator()(const TInput1 &r, const TInput2 &b, const TInput3 &nir)
  {
    double dr = static_cast<double>(r);
    double db = static_cast<double>(b);
    double dnir = static_cast<double>(nir);
    double dRB = dr - m_Gamma*(db - dr);
    double denominator = dRB + m_A*dnir - m_A*m_B + m_X*(1.+m_A*m_A);
    if ( denominator == 0. )
    {
      return static_cast<TOutput>(0.);
    }
    return ( static_cast<TOutput>(  (m_A*(dnir - m_A*dRB - m_B))/denominator ) );
  }
  /** Set/Get A and B parameters */
  void SetA(const double A)
  {
    m_A = A;
  }
  double GetA(void)const
  {
    return (m_A);
  }
  void SetB(const double B)
  {
    m_B = B;
  }
  double GetB(void)const
  {
    return (m_B);
  }
  /** Set/Get X parameter */
  void SetX(const double X)
  {
    m_X = X;
  }
  double GetX(void)const
  {
    return (m_X);
  }
  /** Set/Get the gamma parameter */
  void SetGamma(const double gamma)
  {
    m_Gamma = gamma;
  }
  double GetGamma(void)const
  {
    return (m_Gamma);
  }

private:

  /** A and B parameters */
  double  m_A;
  double  m_B;
  /** X parameter */
  double  m_X;
  /** Gamma parameter */
  double  m_Gamma;

};


/** \class EVI
 *  \brief This functor calculate the Enhanced Vegetation Index (EVI)
 *
 *  This vegetation index use three inputs channels
 *
 *  [Huete, Justice, & Liu, 1994; Huete, Liu, Batchily, & van Leeuwen, 1997]
 *
 *  \ingroup Functor
 */
template <class TInput1, class TInput2, class TInput3, class TOutput>
class EVI
{
public:
  EVI() : m_G(2.5), m_C1(6.0), m_C2(7.5), m_L(1.0) {};
  ~EVI() {};
  inline TOutput operator()(const TInput1 &r, const TInput2 &b, const TInput3 &nir)
  {
    double dr = static_cast<double>(r);
    double db = static_cast<double>(b);
    double dnir = static_cast<double>(nir);
    double denominator = dnir + m_C1*dr - m_C2*db + m_L;
    if ( denominator == 0. )
    {
      return static_cast<TOutput>(0.);
    }
    return ( static_cast<TOutput>( m_G * (dnir - dr)/denominator ) );
  }
  /** Set/Get G parameter */
  void SetG(const double g)
  {
    m_G = g;
  }
  double GetG(void)const
  {
    return (m_G);
  }
  /** Set/Get C1 parameter */
  void SetC1(const double c1)
  {
    m_C1 = c1;
  }
  double GetC1(void)const
  {
    return (m_C1);
  }
  /** Set/Get C2 parameter */
  void SetC2(const double c2)
  {
    m_C2 = c2;
  }
  double GetC2(void)const
  {
    return (m_C2);
  }
  /** Set/Get L parameter */
  void SetL(const double l)
  {
    m_L = l;
  }
  double GetL(void)const
  {
    return (m_L);
  }

private:

  /** Gain factor */
  double  m_G;

  /** Coefficient of the aerosol resistance term */
  double  m_C1;

  /** Coefficient of the aerosol resistance term */
  double  m_C2;

  /** Canopy background adjustment */
  double  m_L;
};

} // namespace Functor
} // namespace otb

#endif

