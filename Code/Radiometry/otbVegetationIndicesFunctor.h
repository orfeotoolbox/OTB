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
#ifndef __otbVegetationIndicesFunctor_h
#define __otbVegetationIndicesFunctor_h

#include "otbMath.h"
#include "itkVariableLengthVector.h"

namespace otb
{
namespace Functor
{
/**
   * \class RAndNIRIndexBase
   * \brief Base class for R And NIR based Index
   *
   *  Implement operators for UnaryFunctorImageFilter templated with a
   *  VectorImage and BinaryFunctorImageFilter templated with single
   *  images.
   *  Subclasses should NOT overload operators, they must  re-implement
   *  the Evaluate() method.
   *
   * \ingroup Radiometry
 */
template<class TInput1, class TInput2, class TOutput>
class RAndNIRIndexBase
{
public:
  /// Vector pixel type used to support both vector images and multiple
  /// input images
  typedef itk::VariableLengthVector<TInput1> InputVectorType;

  // Operator on vector pixel type
  inline TOutput operator()(const InputVectorType & inputVector)
  {
    return this->Evaluate(inputVector[m_RedIndex-1],static_cast<TInput2>(inputVector[m_NIRIndex-1]));
  }

  // Binary operator
  inline TOutput operator()(const TInput1 &r, const TInput2 &nir)
  {
    return this->Evaluate(r,nir);
  };
  /// Constructor
  RAndNIRIndexBase() : m_RedIndex(3), m_NIRIndex(4) {};
  /// Desctructor
  virtual ~RAndNIRIndexBase() {};

  /// Set Red Index
  void SetRedIndex(unsigned int channel)
  {
    m_RedIndex = channel;
  }
  /// Get Red Index
  unsigned int GetRedIndex()
  {
    return m_RedIndex;
  }
  /// Set NIR Index
  void SetNIRIndex(unsigned int channel)
  {
    m_NIRIndex = channel;
  }
  /// Get NIR Index
  unsigned int GetNIRIndex()
  {
    return m_NIRIndex;
  }
protected:
  // This method must be reimplemented in subclasses to actually
  // compute the index value
  virtual TOutput Evaluate(const TInput1 & r, const TInput2 & nir) const = 0;

private:
  unsigned int m_RedIndex;
  unsigned int m_NIRIndex;
};

/**
 * \class RAndBAndNIRIndexBase
 * \brief base class for R, B And NIR based Index
 *  Implement operators for UnaryFunctorImageFilter templated with a
 *  VectorImage and BinaryFunctorImageFilter templated with single
 *  images.
 *  Subclasses should NOT overload operators, they must  re-implement
 *  the Evaluate() method.
 * \ingroup Radiometry
 */
template<class TInput1, class TInput2, class TInput3, class TOutput>
class RAndBAndNIRIndexBase
{
public:
  /// Vector pixel type used to support both vector images and multiple
  /// input images
  typedef itk::VariableLengthVector<TInput1> InputVectorType;

  // Operator on vector pixel type
  inline TOutput operator()(const InputVectorType & inputVector)
  {
    return this->Evaluate(inputVector[m_RedIndex-1],static_cast<TInput2>(inputVector[m_BlueIndex-1]), static_cast<TInput3>(inputVector[m_NIRIndex-1]));
  }

  // Binary operator
  inline TOutput operator()(const TInput1 &r, const TInput2 &b, const TInput2 &nir)
  {
    return this->Evaluate(r,b,nir);
  };
  /// Constructor
  RAndBAndNIRIndexBase() : m_RedIndex(3), m_BlueIndex(1), m_NIRIndex(4) {};
  /// Desctructor
  virtual ~RAndBAndNIRIndexBase() {};

  /// Set Red Index
  void SetRedIndex(unsigned int channel)
  {
    m_RedIndex = channel;
  }
  /// Get Red Index
  unsigned int GetRedIndex()
  {
    return m_RedIndex;
  }
  /// Set Blue Index
  void SetBlueIndex(unsigned int channel)
  {
    m_BlueIndex = channel;
  }
  /// Get Blue Index
  unsigned int GetBlueIndex()
  {
    return m_BlueIndex;
  }

  /// Set NIR Index
  void SetNIRIndex(unsigned int channel)
  {
    m_NIRIndex = channel;
  }
  /// Get NIR Index
  unsigned int GetNIRIndex()
  {
    return m_NIRIndex;
  }
protected:
  // This method must be reimplemented in subclasses to actually
  // compute the index value
  virtual TOutput Evaluate(const TInput1 & r, const TInput2& b, const TInput3 & nir) const = 0;

private:
  unsigned int m_RedIndex;
  unsigned int m_BlueIndex;
  unsigned int m_NIRIndex;
};

/**
 * \class RAndGAndNIRIndexBase
 * \brief base class for R, G And NIR based Index
 *  Implement operators for UnaryFunctorImageFilter templated with a
 *  VectorImage and BinaryFunctorImageFilter templated with single
 *  images.
 *  Subclasses should NOT overload operators, they must  re-implement
 *  the Evaluate() method.
 * \ingroup Radiometry
 */
template<class TInput1, class TInput2, class TInput3, class TOutput>
class RAndGAndNIRIndexBase
{
public:
  /// Vector pixel type used to support both vector images and multiple
  /// input images
  typedef itk::VariableLengthVector<TInput1> InputVectorType;

  // Operator on vector pixel type
  inline TOutput operator()(const InputVectorType & inputVector)
  {
    return this->Evaluate(inputVector[m_RedIndex-1],static_cast<TInput2>(inputVector[m_GreenIndex-1]), static_cast<TInput3>(inputVector[m_NIRIndex-1]));
  }

  // Binary operator
  inline TOutput operator()(const TInput1 &r, const TInput2 &g, const TInput2 &nir)
  {
    return this->Evaluate(r,g,nir);
  };
  /// Constructor
  RAndGAndNIRIndexBase() : m_RedIndex(3), m_GreenIndex(2), m_NIRIndex(4) {};
  /// Desctructor
  virtual ~RAndGAndNIRIndexBase() {};

  /// Set Red Index
  void SetRedIndex(unsigned int channel)
  {
    m_RedIndex = channel;
  }
  /// Get Red Index
  unsigned int GetRedIndex()
  {
    return m_RedIndex;
  }
  /// Set Green Index
  void SetGreenIndex(unsigned int channel)
  {
    m_GreenIndex = channel;
  }
  /// Get Green Index
  unsigned int GetGreenIndex()
  {
    return m_GreenIndex;
  }

  /// Set NIR Index
  void SetNIRIndex(unsigned int channel)
  {
    m_NIRIndex = channel;
  }
  /// Get NIR Index
  unsigned int GetNIRIndex()
  {
    return m_NIRIndex;
  }
protected:
  // This method must be reimplemented in subclasses to actually
  // compute the index value
  virtual TOutput Evaluate(const TInput1 & r, const TInput2& g, const TInput3 & nir) const = 0;

private:
  unsigned int m_RedIndex;
  unsigned int m_GreenIndex;
  unsigned int m_NIRIndex;
};



/** \class NDVI
 *  \brief This functor computes the NormalizeD Vegetation Index (NDVI)
 *
 *  [Pearson et Miller, 1972]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class NDVI : public RAndNIRIndexBase<TInput1,TInput2,TOutput>
{
public:
  /// Constructor
  NDVI() {};
  /// Desctructor
  ~NDVI() {};
  // Operator on r and nir single pixel values
protected:
  inline TOutput Evaluate(const TInput1 &r, const TInput2 &nir) const
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
 *  \brief This functor computes the Ratio Vegetation Index (RVI)
 *
 *  [Rouse et al., 1973]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class RVI : public RAndNIRIndexBase<TInput1, TInput2, TOutput>
{
public:
  RVI() {};
  ~RVI() {};
protected:
  inline TOutput Evaluate(const TInput1 &r, const TInput2 &nir) const
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
 *  \brief This functor computes the Perpendicular Vegetation Index (PVI)
 *
 *  [Richardson et Wiegand, 1977]
 *
 *  \ingroup Functor2
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class PVI : public RAndNIRIndexBase<TInput1, TInput2, TOutput>
{
public:
  PVI() {};
  ~PVI() {};
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
protected:
  inline TOutput Evaluate(const TInput1 &r, const TInput2 &nir) const
  {
    double dnir = static_cast<double>(nir);
    double dr = static_cast<double>(r);
    return ( static_cast<TOutput>(  (dnir - m_A*dr - m_B)*m_Coeff) );
  }

private:

  /** A and B parameters */
  double  m_A;
  double  m_B;
  /** Denominator, pre-calculed when the A variable is set */
  double  m_Coeff;

};


/** \class SAVI
 *  \brief This functor computes the Soil Adjusted Vegetation Index (SAVI)
 *
 *  [Huete, 1988]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class SAVI : public RAndNIRIndexBase<TInput1, TInput2, TOutput>
{
public:
  SAVI() : m_L(0.5) {};
  ~SAVI() {};

  /** Set/Get L correction */
  void SetL(const double L)
  {
    m_L = L;
  }
  double GetL(void)const
  {
    return (  m_L );
  }

protected:
  inline TOutput Evaluate(const TInput1 &r, const TInput2 &nir) const
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

private:

  /** L correction */
  double  m_L;

};
/** \class TSAVI
 *  \brief This functor computes the Transformed Soil Adjusted Vegetation Index (TSAVI)
 *
 *  [Baret et al. 1989, Baret et Guyot, 1991]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class TSAVI : public RAndNIRIndexBase<TInput1, TInput2, TOutput>
{
public:
  TSAVI() : m_X(0.08) {};
  ~TSAVI() {};

  /** Set/Get S and A parameters */
  void SetS(const double S)
  {
    m_S = S;
  }
  double GetS(void)const
  {
    return (m_S);
  }
  void SetA(const double A)
  {
    m_A = A;
  }
  double GetA(void)const
  {
    return (m_A);
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

protected:
  inline TOutput Evaluate(const TInput1 &r, const TInput2 &nir) const
  {
    double dnir = static_cast<double>(nir);
    double dr = static_cast<double>(r);
    double denominator = m_A*dnir + dr + m_X*(1.+m_A*m_A);
    if ( denominator == 0. )
      {
      return static_cast<TOutput>(0.);
      }
    return ( static_cast<TOutput>(  (m_A*(dnir - m_A*dr - m_S))/denominator ) );
  }

private:

  /** A and S parameters */
  double  m_A;
  double  m_S;
  /** X parameter */
  double  m_X;

};

/** \class MSAVI
 *  \brief This functor computes the Modified Soil Adjusted Vegetation Index (MSAVI)
 *
 *  [Qi et al., 1994]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class MSAVI : public RAndNIRIndexBase<TInput1, TInput2, TOutput>
{
public:
  MSAVI() {};
  ~MSAVI() {};
/** Set/Get Slop of soil line */
  void SetS(const double s)
  {
    m_S = s;
  }
  double GetS(void)const
  {
    return (m_S);
  }


protected:
  inline TOutput Evaluate(const TInput1 &r, const TInput2 &nir) const
  {
    double dnir = static_cast<double>(nir);
    double dr = static_cast<double>(r);
/* TODO */
dnir= dnir+dr-dr;
    return ( static_cast<TOutput>( 0 ) );
  }

  /** Slope of soil line */
  double  m_S;

};

/** \class MSAVI2
 *  \brief This functor computes the Modified Soil Adjusted Vegetation Index (MSAVI2)
 *
 *  [Qi et al., 1994]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class MSAVI2 : public RAndNIRIndexBase<TInput1, TInput2, TOutput>
{
public:
  MSAVI2() {};
  ~MSAVI2() {};

protected:
  inline TOutput Evaluate(const TInput1 &r, const TInput2 &nir) const
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

/** \class GEMI
 *  \brief This functor computes the Global Environment Monitoring Index (GEMI)
 *
 *  [Pinty & Verstraete , 1992]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class GEMI : public RAndNIRIndexBase<TInput1, TInput2, TOutput>
{
public:
  GEMI() {};
  ~GEMI() {};

protected:
  inline TOutput Evaluate(const TInput1 &r, const TInput2 &nir) const
  {
    double dnir = static_cast<double>(nir);
    double dr = static_cast<double>(r);

    double dnu;
    double dnumerateur_nu;
    double ddenominateur_nu = dnir + dr + 0.5;
    if ( ddenominateur_nu == 0 )
      {
        dnu = 0;
      }
    else
      {
        dnumerateur_nu = 2*(dnir*dnir - dr*dr) + 1.5*dnir + 0.5*dr;
        dnu = dnumerateur_nu / ddenominateur_nu;
      }

    double ddenominateur_GEMI = 1 - dr;
    if ( ddenominateur_GEMI == 0. )
    {
      return static_cast<TOutput>(0.);
    }
    return ( static_cast<TOutput>(  (dnu*(1 -0.25*dnu)-(dr-0.125))/ddenominateur_GEMI ) );
  }

};

/** \class WDVI
 *  \brief This functor computes the Weighted Difference Vegetation Index (WDVI)
 *
 *  [Clevers, 1988]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class WDVI : public RAndNIRIndexBase<TInput1,TInput2,TOutput>
{
public:
  /// Constructor
  WDVI() {};
  /// Desctructor
  ~WDVI() {};
  // Operator on r and nir single pixel values
/** Set/Get Slop of soil line */
  void SetS(const double s)
  {
    m_S = s;
  }
  double GetS(void)const
  {
    return (m_S);
  }
protected:
  inline TOutput Evaluate(const TInput1 &r, const TInput2 &nir) const
  {
    double dr = static_cast<double>(r);
    double dnir = static_cast<double>(nir);
    return (dnir -m_S*dr);
  }
private:
  /** Slope of soil line */
  double  m_S;
};

/** \class AVI
 *  \brief This functor computes the Angular Vegetation Index (AVI)
 *
 *  This vegetation index use three inputs channels
 *
 *  [Plummer & al., 1994]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TInput3, class TOutput>
class AVI : public RAndGAndNIRIndexBase<TInput1,TInput2,TInput3,TOutput>
{
public:
  AVI() : m_LambdaG(560.), m_LambdaR(660.), m_LambdaNir(830.) {};
  ~AVI() {};
/** Set/Get Lambda red parameter*/
  void SetLambdaR(const double lr)
  {
    m_LambdaR = lr;
  }
  double GetLambdaR(void)const
  {
    return (m_LambdaR);
  }
/** Set/Get Lambda green parameter */
  void SetLambdaG(const double lg)
  {
    m_LambdaG = lg;
  }
  double GetLambdaG(void)const
  {
    return (m_LambdaG);
  }
/** Set/Get Lambda red parameter */
  void SetLambdaNir(const double lnir)
  {
    m_LambdaNir = lnir;
  }
  double GetLambdaNir(void)const
  {
    return (m_LambdaNir);
  }
protected:
  inline TOutput Evaluate(const TInput1 &r, const TInput2 &g, const TInput3 &nir) const
  {
    double dr = static_cast<double>(r);
    double dg = static_cast<double>(g);
    double dnir = static_cast<double>(nir);

    double dfact1 = (m_LambdaNir - m_LambdaR) / m_LambdaR;
    double dfact2 = (m_LambdaR - m_LambdaG) / m_LambdaR;
    double dterm1;
    double dterm2;
    if( (dnir-dr) == 0 )
    {
      dterm1 = 0;
    }
    else
    {
      dterm1 = vcl_atan(dfact1/(dnir - dr));
    }

    if( (dg-dr) == 0 )
    {
      dterm2 = 0;
    }
    else
    {
      dterm2 = vcl_atan(dfact2/(dg - dr));
    }

    return static_cast<TOutput>( dterm1 + dterm2 );

  }
private:

  /**  Central wavelength of the green channel (=Lambda1) */
  double  m_LambdaG;

  /**  Central wavelength of the red channel (=Lambda2) */
  double  m_LambdaR;

  /**  Central wavelength of the nir channel (=Lambda3) */
  double  m_LambdaNir;
};


/** \class ARVI
 *  \brief This functor computes the Atmospherically Resistant Vegetation Index (ARVI)
 *
 *  This vegetation index use three inputs channels
 *
 *  [Yoram J. Kaufman and Didier Tanre, 1992]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TInput3, class TOutput>
class ARVI : public RAndBAndNIRIndexBase<TInput1,TInput2,TInput3,TOutput>
{
public:
  ARVI() : m_Gamma(0.5) {};
  ~ARVI() {};

  /** Set/Get Gamma parameter */
  void SetGamma(const double gamma)
  {
    m_Gamma = gamma;
  }
  double GetGamma(void)const
  {
    return (m_Gamma);
  }

protected:
  inline TOutput Evaluate(const TInput1 &r, const TInput2 &b, const TInput3 &nir) const
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

private:

  /** Gamma parameter */
  double  m_Gamma;
};

/** \class TSARVI
 *  \brief This functor computes the Transformed Soil Atmospherical Resistant Vegetation Index (TSARVI)
 *
 *  [Yoram J. Kaufman and Didier Tanré, 1992]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TInput3, class TOutput>
class TSARVI: public RAndBAndNIRIndexBase<TInput1,TInput2,TInput3,TOutput>
{
public:
  TSARVI() : m_X(0.08), m_Gamma(0.5) {};
  ~TSARVI() {};

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

protected:
  inline TOutput Evaluate(const TInput1 &r, const TInput2 &b, const TInput3 &nir) const
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
 *  \brief This functor computes the Enhanced Vegetation Index (EVI)
 *
 *  This vegetation index use three inputs channels
 *
 *  [Huete, Justice, & Liu, 1994; Huete, Liu, Batchily, & van Leeuwen, 1997]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TInput3, class TOutput>
class EVI : public RAndBAndNIRIndexBase<TInput1,TInput2,TInput3,TOutput>
{
public:
  EVI() : m_G(2.5), m_C1(6.0), m_C2(7.5), m_L(1.0) {};
  ~EVI() {};
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
protected:
  inline TOutput Evaluate(const TInput1 &r, const TInput2 &b, const TInput3 &nir) const
  {
    double dr = static_cast<double>(r);
    double db = static_cast<double>(b);
    double dnir = static_cast<double>(nir);
    double denominator = dnir + m_C1*dr - m_C2*db + m_L;
    if ( denominator == 0. )
      {
      return ( static_cast<TOutput>(0.) );
      }
    return ( static_cast<TOutput>( m_G * (dnir - dr)/denominator ) );
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

/** \class IPVI
 *  \brief This functor computes the Infrared Percentage VI (IPVI)
 *
 *  [Crippen, 1990]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class IPVI : public RAndNIRIndexBase<TInput1, TInput2, TOutput>
{
public:
  IPVI() {};
  ~IPVI() {};

protected:
  inline TOutput Evaluate(const TInput1 &r, const TInput2 &nir) const
  {
    double dr = static_cast<double>(r);
    double dnir = static_cast<double>(nir);
    if ((dnir + dr) == 0)
    {
      return static_cast<TOutput>(0.);
    }
    else
    {
      return ( static_cast<TOutput>( dnir/(dnir+dr) ) );
    }
  }
};

/** \class TNDVI
 *  \brief This functor computes the Transformed NDVI (TNDVI)
 *
 *  [Deering, 1975]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class TNDVI : public RAndNIRIndexBase<TInput1, TInput2, TOutput>
{
public:
  typedef NDVI<TInput1, TInput2, TOutput> NDVIFunctorType;
  TNDVI() {};
  ~TNDVI() {};

  NDVIFunctorType GetNDVI(void)const
  {
    return (m_NDVIfunctor);
  }

protected:
  inline TOutput Evaluate(const TInput1 &r, const TInput2 &nir) const
  {
    return ( static_cast<TOutput>(this->GetNDVI()(r,nir) + 0.5 ));
  }
private:
  const NDVIFunctorType m_NDVIfunctor;
};

} // namespace Functor
} // namespace otb

#endif

