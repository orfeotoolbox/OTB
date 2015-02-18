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
#include "otbBandName.h"

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

  //operators !=
  bool operator !=(const RAndNIRIndexBase&) const
  {
    return true;
  }
  //operator ==
  bool operator ==(const RAndNIRIndexBase& other) const
  {
    return !(*this != other);
  }

  // Operator on vector pixel type
  inline TOutput operator ()(const InputVectorType& inputVector) const
  {
    return this->Evaluate(inputVector[m_RedIndex - 1], static_cast<TInput2>(inputVector[m_NIRIndex - 1]));
  }

  // Binary operator
  inline TOutput operator ()(const TInput1& r, const TInput2& nir) const
  {
    return this->Evaluate(r, nir);
  }
  /// Constructor
  RAndNIRIndexBase() :  m_EpsilonToBeConsideredAsZero(0.0000001), m_RedIndex(3), m_NIRIndex(4) {}
  /// Desctructor
  virtual ~RAndNIRIndexBase() {}

  /** Set index, generic method */
  void SetIndex(BandName::BandName band, unsigned int channel)
  {
    if (band == BandName::RED)
      {
      m_RedIndex = channel;
      }
    if (band == BandName::NIR)
      {
      m_NIRIndex = channel;
      }
  }
  /** Get index, generic method */
  unsigned int GetIndex(BandName::BandName band) const
  {
    if (band == BandName::RED)
      {
      return m_RedIndex;
      }
    if (band == BandName::NIR)
      {
      return m_NIRIndex;
      }
  }

  /// Set Red Index
  void SetRedIndex(unsigned int channel)
  {
    m_RedIndex = channel;
  }
  /// Get Red Index
  unsigned int GetRedIndex() const
  {
    return m_RedIndex;
  }
  /// Set NIR Index
  void SetNIRIndex(unsigned int channel)
  {
    m_NIRIndex = channel;
  }
  /// Get NIR Index
  unsigned int GetNIRIndex() const
  {
    return m_NIRIndex;
  }

  /** Return the index name */
  virtual std::string GetName() const = 0;

protected:
  // This method must be reimplemented in subclasses to actually
  // compute the index value
  virtual TOutput Evaluate(const TInput1& r, const TInput2& nir) const = 0;
  double m_EpsilonToBeConsideredAsZero;

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

  //operators !=
  bool operator !=(const RAndBAndNIRIndexBase&) const
  {
    return true;
  }

  //operator ==
  bool operator ==(const RAndBAndNIRIndexBase& other) const
  {
    return !(*this != other);
  }

  // Operator on vector pixel type
  inline TOutput operator ()(const InputVectorType& inputVector)
  {
    return this->Evaluate(inputVector[m_RedIndex - 1],
                          static_cast<TInput2>(inputVector[m_BlueIndex - 1]),
                          static_cast<TInput3>(inputVector[m_NIRIndex - 1]));
  }
  // Binary operator
  inline TOutput operator ()(const TInput1& r, const TInput2& b, const TInput2& nir)
  {
    return this->Evaluate(r, b, nir);
  }
  /// Constructor
  RAndBAndNIRIndexBase() : m_EpsilonToBeConsideredAsZero(0.0000001), m_RedIndex(3), m_BlueIndex(1), m_NIRIndex(4) {}
  /// Desctructor
  virtual ~RAndBAndNIRIndexBase() {}

  /** Set index, generic method */
  void SetIndex(BandName::BandName band, unsigned int channel)
  {
    if (band == BandName::RED)
      {
      m_RedIndex = channel;
      }
    if (band == BandName::BLUE)
      {
      m_BlueIndex = channel;
      }
    if (band == BandName::NIR)
      {
      m_NIRIndex = channel;
      }
  }
  /** Get index, generic method */
  unsigned int GetIndex(BandName::BandName band) const
  {
    if (band == BandName::RED)
      {
      return m_RedIndex;
      }
    if (band == BandName::BLUE)
      {
      return m_BlueIndex;
      }
    if (band == BandName::NIR)
      {
      return m_NIRIndex;
      }
  }

  /// Set Red Index
  void SetRedIndex(unsigned int channel)
  {
    m_RedIndex = channel;
  }
  /// Get Red Index
  unsigned int GetRedIndex() const
  {
    return m_RedIndex;
  }
  /// Set Blue Index
  void SetBlueIndex(unsigned int channel)
  {
    m_BlueIndex = channel;
  }
  /// Get Blue Index
  unsigned int GetBlueIndex() const
  {
    return m_BlueIndex;
  }

  /// Set NIR Index
  void SetNIRIndex(unsigned int channel)
  {
    m_NIRIndex = channel;
  }
  /// Get NIR Index
  unsigned int GetNIRIndex() const
  {
    return m_NIRIndex;
  }

  /** Return the index name */
  virtual std::string GetName() const = 0;

protected:
  // This method must be reimplemented in subclasses to actually
  // compute the index value
  virtual TOutput Evaluate(const TInput1& r, const TInput2& b, const TInput3& nir) const = 0;
  double m_EpsilonToBeConsideredAsZero;

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

  //operators !=
  bool operator !=(const RAndGAndNIRIndexBase&) const
  {
    return true;
  }
  //operator ==
  bool operator ==(const RAndGAndNIRIndexBase& other) const
  {
    return !(*this != other);
  }

  // Operator on vector pixel type
  inline TOutput operator ()(const InputVectorType& inputVector)
  {
    return this->Evaluate(inputVector[m_RedIndex - 1],
                          static_cast<TInput2>(inputVector[m_GreenIndex - 1]),
                          static_cast<TInput3>(inputVector[m_NIRIndex - 1]));
  }

  // Binary operator
  inline TOutput operator ()(const TInput1& r, const TInput2& g, const TInput2& nir)
  {
    return this->Evaluate(r, g, nir);
  }
  /// Constructor
  RAndGAndNIRIndexBase() : m_EpsilonToBeConsideredAsZero(0.0000001), m_RedIndex(3), m_GreenIndex(2), m_NIRIndex(4)  {}
  /// Desctructor
  virtual ~RAndGAndNIRIndexBase() {}

  /** Set index, generic method */
  void SetIndex(BandName::BandName band, unsigned int channel)
  {
    if (band == BandName::RED)
      {
      m_RedIndex = channel;
      }
    if (band == BandName::GREEN)
      {
      m_GreenIndex = channel;
      }
    if (band == BandName::NIR)
      {
      m_NIRIndex = channel;
      }
  }
  /** Get index, generic method */
  unsigned int GetIndex(BandName::BandName band) const
  {
    if (band == BandName::RED)
      {
      return m_RedIndex;
      }
    if (band == BandName::GREEN)
      {
      return m_GreenIndex;
      }
    if (band == BandName::NIR)
      {
      return m_NIRIndex;
      }
  }

  /// Set Red Index
  void SetRedIndex(unsigned int channel)
  {
    m_RedIndex = channel;
  }
  /// Get Red Index
  unsigned int GetRedIndex() const
  {
    return m_RedIndex;
  }
  /// Set Green Index
  void SetGreenIndex(unsigned int channel)
  {
    m_GreenIndex = channel;
  }
  /// Get Green Index
  unsigned int GetGreenIndex() const
  {
    return m_GreenIndex;
  }

  /// Set NIR Index
  void SetNIRIndex(unsigned int channel)
  {
    m_NIRIndex = channel;
  }
  /// Get NIR Index
  unsigned int GetNIRIndex() const
  {
    return m_NIRIndex;
  }

  /** Return the index name */
  virtual std::string GetName() const = 0;

protected:
  // This method must be reimplemented in subclasses to actually
  // compute the index value
  virtual TOutput Evaluate(const TInput1& r, const TInput2& g, const TInput3& nir) const = 0;
  double m_EpsilonToBeConsideredAsZero;

private:
  unsigned int m_RedIndex;
  unsigned int m_GreenIndex;
  unsigned int m_NIRIndex;
};

/** \class NDVI
 *  \brief This functor computes the Normalized Difference Vegetation Index (NDVI)
 *
 *  [Rouse et al., 1973]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class NDVI : public RAndNIRIndexBase<TInput1, TInput2, TOutput>
{
public:

  /** Return the index name */
  virtual std::string GetName() const
  {
    return "NDVI";
  }

  /// Constructor
  NDVI() {}
  /// Desctructor
  virtual ~NDVI() {}
  // Operator on r and nir single pixel values
protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& nir) const
  {
    double dr = static_cast<double>(r);
    double dnir = static_cast<double>(nir);
    if (vcl_abs(dnir + dr) < this->m_EpsilonToBeConsideredAsZero)
      {
      return static_cast<TOutput>(0.);
      }

    return (static_cast<TOutput>((dnir - dr) / (dnir + dr)));
  }
};

/** \class RVI
 *  \brief This functor computes the Ratio Vegetation Index (RVI)
 *
 *  [Pearson et Miller, 1972]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class RVI : public RAndNIRIndexBase<TInput1, TInput2, TOutput>
{
public:

  /** Return the index name */
  virtual std::string GetName() const
  {
    return "RVI";
  }

  RVI() {}
  virtual ~RVI() {}
protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& nir) const
  {
    double dr = static_cast<double>(r);
    double dnir = static_cast<double>(nir);
    if (vcl_abs(dr)  < this->m_EpsilonToBeConsideredAsZero)
      {
      return static_cast<TOutput>(0.);
      }
    return (static_cast<TOutput>(dnir / dr));
  }
};

/** \class PVI
 *  \brief This functor computes the Perpendicular Vegetation Index (PVI)
 *
 *  [Richardson et Wiegand, 1977]
 *
 * Default values for A and B are from
 * C. L. Wiegand, A. J. Richardson, D. E. Escobar, and A. H. Gerbermann,
 * "Vegetation Indices in Crop Assessments", REMOTE SENS. ENVIRON. 35:105-119 (1991)
 *
 *  \ingroup Functor2
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class PVI : public RAndNIRIndexBase<TInput1, TInput2, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "PVI";
  }

  PVI() : m_A(0.90893), m_B(7.46216), m_Coeff(0.74) {}
  virtual ~PVI() {}
  /** Set/Get A and B parameters */
  void SetA(const double A)
  {
    m_A = A;
    m_Coeff = 1. / (vcl_sqrt(m_A * m_A + 1.));
  }
  double GetA(void) const
  {
    return (m_A);
  }
  void SetB(const double B)
  {
    m_B = B;
  }
  double GetB(void) const
  {
    return (m_B);
  }
protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& nir) const
  {
    double dnir = static_cast<double>(nir);
    double dr = static_cast<double>(r);
    return (static_cast<TOutput>((dnir - m_A * dr - m_B) * m_Coeff));
  }

private:

  /** A and B parameters */
  double m_A;
  double m_B;
  /** Denominator, pre-calculed when the A variable is set */
  double m_Coeff;

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

  /** Return the index name */
  virtual std::string GetName() const
  {
    return "SAVI";
  }

  SAVI() : m_L(0.5) {}
  virtual ~SAVI() {}

  /** Set/Get L correction */
  void SetL(const double L)
  {
    m_L = L;
  }
  double GetL(void) const
  {
    return (m_L);
  }

protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& nir) const
  {
    double dnir = static_cast<double>(nir);
    double dr = static_cast<double>(r);
    double denominator = dnir + dr + m_L;
    if (vcl_abs(denominator)  < this->m_EpsilonToBeConsideredAsZero)
      {
      return static_cast<TOutput>(0.);
      }
    return (static_cast<TOutput>(((dnir - dr) * (1 + m_L)) / denominator));
  }

private:

  /** L correction */
  double m_L;

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

  /** Return the index name */
  virtual std::string GetName() const
  {
    return "TSAVI";
  }

  TSAVI() : m_A(0.7), m_S(0.9), m_X(0.08) {}
  virtual ~TSAVI() {}

  /** Set/Get S and A parameters */
  void SetS(const double S)
  {
    m_S = S;
  }
  double GetS(void) const
  {
    return (m_S);
  }
  void SetA(const double A)
  {
    m_A = A;
  }
  double GetA(void) const
  {
    return (m_A);
  }
  /** Set/Get X parameter */
  void SetX(const double X)
  {
    m_X = X;
  }
  double GetX(void) const
  {
    return (m_X);
  }

protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& nir) const
  {
    double dnir = static_cast<double>(nir);
    double dr = static_cast<double>(r);
    double denominator = m_A * dnir + dr + m_X * (1. + m_A * m_A);
    if (vcl_abs(denominator) < this->m_EpsilonToBeConsideredAsZero)
      {
      return static_cast<TOutput>(0.);
      }
    return (static_cast<TOutput>((m_A * (dnir - m_A * dr - m_S)) / denominator));
  }

private:

  /** A and S parameters */
  double m_A;
  double m_S;
  /** X parameter */
  double m_X;

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
class WDVI : public RAndNIRIndexBase<TInput1, TInput2, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "WDVI";
  }

  /// Constructor
  WDVI() : m_S(0.4) {}
  /// Desctructor
  virtual ~WDVI() {}
  // Operator on r and nir single pixel values
/** Set/Get Slop of soil line */
  void SetS(const double s)
  {
    m_S = s;
  }
  double GetS(void) const
  {
    return (m_S);
  }
protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& nir) const
  {
    double dr = static_cast<double>(r);
    double dnir = static_cast<double>(nir);

    return (dnir - m_S * dr);
  }
private:
  /** Slope of soil line */
  double m_S;
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
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "MSAVI";
  }

  typedef NDVI<TInput1, TInput2, TOutput> NDVIFunctorType;
  typedef SAVI<TInput1, TInput2, TOutput> SAVIFunctorType;
  typedef WDVI<TInput1, TInput2, TOutput> WDVIFunctorType;
  MSAVI() : m_S(0.4)
  {
    m_WDVIfunctor.SetS(m_S);
  }
  virtual ~MSAVI() {}
/** Set/Get Slop of soil line */
  void SetS(const double s)
  {
    m_S = s;
    m_WDVIfunctor.SetS(m_S);
  }
  double GetS(void) const
  {
    return (m_S);
  }
  NDVIFunctorType GetNDVI(void) const
  {
    return (m_NDVIfunctor);
  }
  WDVIFunctorType GetWDVI(void) const
  {
    return (m_WDVIfunctor);
  }

protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& nir) const
  {
    double dnir = static_cast<double>(nir);
    double dr = static_cast<double>(r);

    double dNDVI = this->GetNDVI() (r, nir);
    double dWDVI = this->GetWDVI() (r, nir);
    double dL = 1 - 2 * m_S * dNDVI * dWDVI;

    double denominator = dnir + dr + dL;

    if (vcl_abs(denominator)  < this->m_EpsilonToBeConsideredAsZero)
      {
      return static_cast<TOutput>(0.);
      }

    return (static_cast<TOutput>(((dnir - dr) * (1 + dL)) / denominator));
  }

private:
  /** Slope of soil line */
  double                m_S;
  NDVIFunctorType m_NDVIfunctor;
  WDVIFunctorType       m_WDVIfunctor;

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
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "MSAVI2";
  }

  MSAVI2() {}
  virtual ~MSAVI2() {}

protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& nir) const
  {
    double dnir = static_cast<double>(nir);
    double dr = static_cast<double>(r);
    double sqrt_value = (2 * dnir + 1) * (2 * dnir + 1) - 8 * (dnir - dr);
    if (sqrt_value < 0.)
      {
      return static_cast<TOutput>(0.);
      }
    return (static_cast<TOutput>((2 * dnir + 1 - vcl_sqrt(sqrt_value)) / 2.));
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
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "GEMI";
  }

  GEMI() {}
  virtual ~GEMI() {}

protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& nir) const
  {
    double dnir = static_cast<double>(nir);
    double dr = static_cast<double>(r);

    double dnu;
    double dnumerateur_nu;
    double ddenominateur_nu = dnir + dr + 0.5;
    if (vcl_abs(ddenominateur_nu)  < this->m_EpsilonToBeConsideredAsZero)
      {
      dnu = 0;
      }
    else
      {
      dnumerateur_nu = 2 * (dnir * dnir - dr * dr) + 1.5 * dnir + 0.5 * dr;
      dnu = dnumerateur_nu / ddenominateur_nu;
      }

    double ddenominateur_GEMI = 1 - dr;
    if (vcl_abs(ddenominateur_GEMI)  < this->m_EpsilonToBeConsideredAsZero)
      {
      return static_cast<TOutput>(0.);
      }
    return (static_cast<TOutput>((dnu * (1 - 0.25 * dnu) - (dr - 0.125)) / ddenominateur_GEMI));
  }

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
class AVI : public RAndGAndNIRIndexBase<TInput1, TInput2, TInput3, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "AVI";
  }

  AVI() : m_LambdaG(560.), m_LambdaR(660.), m_LambdaNir(830.) {}
  virtual ~AVI() {}
/** Set/Get Lambda red parameter*/
  void SetLambdaR(const double lr)
  {
    m_LambdaR = lr;
  }
  double GetLambdaR(void) const
  {
    return (m_LambdaR);
  }
/** Set/Get Lambda green parameter */
  void SetLambdaG(const double lg)
  {
    m_LambdaG = lg;
  }
  double GetLambdaG(void) const
  {
    return (m_LambdaG);
  }
/** Set/Get Lambda red parameter */
  void SetLambdaNir(const double lnir)
  {
    m_LambdaNir = lnir;
  }
  double GetLambdaNir(void) const
  {
    return (m_LambdaNir);
  }
protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& g, const TInput3& nir) const
  {
    double dr = static_cast<double>(r);
    double dg = static_cast<double>(g);
    double dnir = static_cast<double>(nir);

    double dfact1 = (m_LambdaNir - m_LambdaR) / m_LambdaR;
    double dfact2 = (m_LambdaR - m_LambdaG) / m_LambdaR;
    double dterm1;
    double dterm2;
    if (vcl_abs(dnir - dr)  < this->m_EpsilonToBeConsideredAsZero)
      {
      dterm1 = 0;
      }
    else
      {
      dterm1 = vcl_atan(dfact1 / (dnir - dr));
      }

    if (vcl_abs(dg - dr)  < this->m_EpsilonToBeConsideredAsZero)
      {
      dterm2 = 0;
      }
    else
      {
      dterm2 = vcl_atan(dfact2 / (dg - dr));
      }

    return static_cast<TOutput>(dterm1 + dterm2);

  }
private:

  /**  Central wavelength of the green channel (=Lambda1) */
  double m_LambdaG;

  /**  Central wavelength of the red channel (=Lambda2) */
  double m_LambdaR;

  /**  Central wavelength of the nir channel (=Lambda3) */
  double m_LambdaNir;
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
class ARVI : public RAndBAndNIRIndexBase<TInput1, TInput2, TInput3, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "ARVI";
  }

  ARVI() : m_Gamma(0.5) {}
  virtual ~ARVI() {}

  /** Set/Get Gamma parameter */
  void SetGamma(const double gamma)
  {
    m_Gamma = gamma;
  }
  double GetGamma(void) const
  {
    return (m_Gamma);
  }

protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& b, const TInput3& nir) const
  {
    double dr = static_cast<double>(r);
    double db = static_cast<double>(b);
    double dnir = static_cast<double>(nir);
    double RHOrb = dr - m_Gamma * (db - dr);
    double denominator = dnir + RHOrb;
    if (vcl_abs(denominator)  < this->m_EpsilonToBeConsideredAsZero)
      {
      return static_cast<TOutput>(0.);
      }
    return (static_cast<TOutput>((dnir - RHOrb) / denominator));
  }

private:

  /** Gamma parameter */
  double m_Gamma;
};

/** \class TSARVI
 *  \brief This functor computes the Transformed Soil Atmospherical Resistant Vegetation Index (TSARVI)
 *
 *  [Yoram J. Kaufman and Didier Tanre, 1992]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TInput3, class TOutput>
class TSARVI : public RAndBAndNIRIndexBase<TInput1, TInput2, TInput3, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "TSARVI";
  }

  TSARVI() : m_A(0.0), m_B(0.0), m_X(0.08), m_Gamma(0.5) {}
  virtual ~TSARVI() {}

  /** Set/Get A and B parameters */
  void SetA(const double A)
  {
    m_A = A;
  }
  double GetA(void) const
  {
    return (m_A);
  }
  void SetB(const double B)
  {
    m_B = B;
  }
  double GetB(void) const
  {
    return (m_B);
  }
  /** Set/Get X parameter */
  void SetX(const double X)
  {
    m_X = X;
  }
  double GetX(void) const
  {
    return (m_X);
  }
  /** Set/Get the gamma parameter */
  void SetGamma(const double gamma)
  {
    m_Gamma = gamma;
  }
  double GetGamma(void) const
  {
    return (m_Gamma);
  }

protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& b, const TInput3& nir) const
  {
    double dr = static_cast<double>(r);
    double db = static_cast<double>(b);
    double dnir = static_cast<double>(nir);
    double dRB = dr - m_Gamma * (db - dr);
    double denominator = dRB + m_A * dnir - m_A * m_B + m_X * (1. + m_A * m_A);
    if (vcl_abs(denominator)  < this->m_EpsilonToBeConsideredAsZero)
      {
      return static_cast<TOutput>(0.);
      }
    return (static_cast<TOutput>((m_A * (dnir - m_A * dRB - m_B)) / denominator));
  }

private:

  /** A and B parameters */
  double m_A;
  double m_B;
  /** X parameter */
  double m_X;
  /** Gamma parameter */
  double m_Gamma;

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
class EVI : public RAndBAndNIRIndexBase<TInput1, TInput2, TInput3, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "EVI";
  }

  EVI() : m_G(2.5), m_C1(6.0), m_C2(7.5), m_L(1.0) {}
  virtual ~EVI() {}
/** Set/Get G parameter */
  void SetG(const double g)
  {
    m_G = g;
  }
  double GetG(void) const
  {
    return (m_G);
  }
  /** Set/Get C1 parameter */
  void SetC1(const double c1)
  {
    m_C1 = c1;
  }
  double GetC1(void) const
  {
    return (m_C1);
  }
  /** Set/Get C2 parameter */
  void SetC2(const double c2)
  {
    m_C2 = c2;
  }
  double GetC2(void) const
  {
    return (m_C2);
  }
  /** Set/Get L parameter */
  void SetL(const double l)
  {
    m_L = l;
  }
  double GetL(void) const
  {
    return (m_L);
  }
protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& b, const TInput3& nir) const
  {
    double dr = static_cast<double>(r);
    double db = static_cast<double>(b);
    double dnir = static_cast<double>(nir);
    double denominator = dnir + m_C1 * dr - m_C2 * db + m_L;
    if (vcl_abs(denominator) < this->m_EpsilonToBeConsideredAsZero)
      {
      return (static_cast<TOutput>(0.));
      }
    return (static_cast<TOutput>(m_G * (dnir - dr) / denominator));
  }

private:

  /** Gain factor */
  double m_G;

  /** Coefficient of the aerosol resistance term */
  double m_C1;

  /** Coefficient of the aerosol resistance term */
  double m_C2;

  /** Canopy background adjustment */
  double m_L;
};

/** \class IPVI
 *  \brief This functor computes the Infrared Percentage Vegetation Index (IPVI)
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
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "IPVI";
  }

  IPVI() {}
  virtual ~IPVI() {}

protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& nir) const
  {
    double dr = static_cast<double>(r);
    double dnir = static_cast<double>(nir);
    if (vcl_abs(dnir + dr)  < this->m_EpsilonToBeConsideredAsZero)
      {
      return static_cast<TOutput>(0.);
      }
    else
      {
      return (static_cast<TOutput>(dnir / (dnir + dr)));
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
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "TNDVI";
  }

  typedef NDVI<TInput1, TInput2, TOutput> NDVIFunctorType;
  TNDVI() {}
  virtual ~TNDVI() {}

  NDVIFunctorType GetNDVI(void) const
  {
    return (m_NDVIfunctor);
  }

protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& nir) const
  {
    double dval = this->GetNDVI() (r, nir) + 0.5;
    if (dval < 0)
      {
      return  (static_cast<TOutput>(0));
      }
    else
      {
      return (static_cast<TOutput>(vcl_sqrt(dval)));
      }
  }
private:
  NDVIFunctorType m_NDVIfunctor;
};

/** \class LAIFromNDVILogarithmic
 *  \brief computes the LAI from NDVI
 *
 *  This functor computes the LAI from NDVI using a
 *  logarithmic relationship. Asrar et al. (1984), Baret and Guyot
 *  (1991) and Wilson and Meyers (2007). Default values for the
 *  parameters are taken from A. Bsaibes et al. / Remote Sensing of
 *  Environment 113 (2009) 716-729
 *
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 * \ingroup VegetationIndices
 */
template <class TInput1, class TInput2, class TOutput>
class LAIFromNDVILogarithmic : public RAndNIRIndexBase<TInput1, TInput2, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "LAIFromNDVILogarithmic";
  }

  typedef NDVI<TInput1, TInput2, TOutput> NDVIFunctorType;
  LAIFromNDVILogarithmic() : m_NdviSoil(0.10), m_NdviInf(0.89), m_ExtinctionCoefficient(0.71) {}
  virtual ~LAIFromNDVILogarithmic() {}

  NDVIFunctorType GetNDVI(void) const
  {
    return (m_NDVIfunctor);
  }

  void SetNdviSoil(const double val)
  {
    m_NdviSoil = val;
  }
  double GetNdviSoil(void) const
  {
    return (m_NdviSoil);
  }

  void SetNdviInf(const double val)
  {
    m_NdviInf = val;
  }
  double GetNdviInf(void) const
  {
    return (m_NdviInf);
  }

  void SetExtinctionCoefficient(const double val)
  {
    m_ExtinctionCoefficient = val;
  }
  double GetExtinctionCoefficient(void) const
  {
    return (m_ExtinctionCoefficient);
  }

protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& nir) const
  {
    double dval = this->GetNDVI() (r, nir);
    if (dval < 0)
      {
      return  (static_cast<TOutput>(0));
      }
    else
      {
      return (static_cast<TOutput>(
    -(1.0/m_ExtinctionCoefficient)*vcl_log((dval- m_NdviInf)/(m_NdviSoil-m_NdviInf))
    ));
      }
  }
private:
  NDVIFunctorType m_NDVIfunctor;
  double m_NdviSoil;
  double m_NdviInf;
  double m_ExtinctionCoefficient;
};


/** \class LAIFromReflectancesLinear
 * \brief computes the LAI from reflectances
 *
 *  This functor computes the LAI from reflectances using a
 *  linear relationship.
 *  \f$ LAI = \beta_0 + \sum_j \beta_j \rho_j \f$ where \f$ \rho \f$ are the
 *  reflectances
 *  Default values for the parameters are taken from A. Bsaibes et
 *  al. / Remote Sensing of Environment 113 (2009) 716-729
 *
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 * \ingroup VegetationIndices
 */
template <class TInput1, class TInput2, class TOutput>
class LAIFromReflectancesLinear : public RAndNIRIndexBase<TInput1, TInput2, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "LAIFromReflectancesLinear";
  }

  typedef NDVI<TInput1, TInput2, TOutput> NDVIFunctorType;
  LAIFromReflectancesLinear() : m_RedCoef(-17.91), m_NirCoef(12.26) {}
  virtual ~LAIFromReflectancesLinear() {}

  NDVIFunctorType GetReflectances(void) const
  {
    return (m_NDVIfunctor);
  }

  void SetRedCoef(const double val)
  {
    m_RedCoef = val;
  }
  double GetRedCoef(void) const
  {
    return (m_RedCoef);
  }

  void SetNirCoef(const double val)
  {
    m_NirCoef = val;
  }
  double GetNirCoef(void) const
  {
    return (m_NirCoef);
  }

protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& nir) const
  {
      return (static_cast<TOutput>(m_RedCoef*r+m_NirCoef*nir));
  }
private:
  NDVIFunctorType m_NDVIfunctor;
  double m_RedCoef;
  double m_NirCoef;
};


/** \class LAIFromNDVIFormosat2Functor
 *
 * \brief use red and nir image band to compute LAI image using formula
 *   a*(exp(nir-red)/((red+nir)*b)-exp(c*b));
 *   with a = 0.1519 b = 3.9443 c = 0.13
 *  this formula is only valid for Formosat 2 reflectance TOC
 *
 *  This relationship have been presented by :
 *  M. Claverie, V. Demarez, B. Duchemin,
 *  Spatialization of crop leaf area index and biomass by combining a simple crop model
 *  safy and high spatial and temporal resolutions remote sensing data.
 *
 *  \ingroup Functor
 * \ingroup Radiometry
  */


  template <class TInput1, class TInput2, class TOutput>
  class LAIFromNDVIFormosat2Functor : public RAndNIRIndexBase<TInput1, TInput2, TOutput>
  {
  public:

    /** Return the index name */
    virtual std::string GetName() const
    {
      return "LAIFromNDVIFormosat2Functor";
    }

    /// Constructor
    LAIFromNDVIFormosat2Functor() {}
    /// Desctructor
    virtual ~LAIFromNDVIFormosat2Functor() {}
    // Operator on r and nir single pixel values
  protected:
    inline TOutput Evaluate(const TInput1& r, const TInput2& nir) const
    {
    double a = 0.1519;
    double b = 3.9443;
    double c = 0.13;

      double dr = static_cast<double>(r);
      double dnir = static_cast<double>(nir);
      if (vcl_abs(dnir + dr) < this->m_EpsilonToBeConsideredAsZero)
        {
        return static_cast<TOutput>(0.);
        }

      return  static_cast<TOutput>(a*(vcl_exp(static_cast<double>(dnir-dr)/static_cast<double>(dr+dnir)*b)-vcl_exp(c*b)));
    };


};


} // namespace Functor
} // namespace otb

#endif
