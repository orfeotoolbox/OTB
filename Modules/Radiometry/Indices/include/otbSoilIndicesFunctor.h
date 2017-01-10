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
#ifndef otbSoilIndicesFunctor_h
#define otbSoilIndicesFunctor_h

#include "otbMath.h"
#include "itkVariableLengthVector.h"
#include "otbBandName.h"

namespace otb
{
namespace Functor
{
/**
 * \class GAndRIndexBase
 *
 * \brief Base class for Green And Red channels of Spot Images
 *  XS1 corresponds to the green channel
 *  XS2 corresponds to the red channel
 *  XS3 corresponds to the Nir channel
 *  XS4 corresponds to the Mir channel (for Spot 4 & 5)
 *  Implement operators for UnaryFunctorImageFilter templated with a
 *  VectorImage and BinaryFunctorImageFilter templated with single
 *  images.
 *  Subclasses should NOT overload operators, they must  re-implement
 *  the Evaluate() method.
 *
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
*/
template<class TInput1, class TInput2, class TOutput>
class GAndRIndexBase
{
public:
  /// Vector pixel type used to support both vector images and multiple
  /// input images
  typedef itk::VariableLengthVector<TInput1> InputVectorType;

  //operators !=
  bool operator !=(const GAndRIndexBase&) const
  {
    return true;
  }
  //operator ==
  bool operator ==(const GAndRIndexBase& other) const
  {
    return !(*this != other);
  }

  // Operator on vector pixel type
  inline TOutput operator ()(const InputVectorType& inputVector) const
  {
    return this->Evaluate(inputVector[m_GreenIndex - 1], static_cast<TInput2>(inputVector[m_RedIndex - 1]));
  }

  // Binary operator
  inline TOutput operator ()(const TInput1& g, const TInput2& r) const
  {
    return this->Evaluate(g, r);
  }
  /// Constructor
  GAndRIndexBase() : m_EpsilonToBeConsideredAsZero(0.0000001), m_GreenIndex(1), m_RedIndex(2) {}
  /// Desctructor
  virtual ~GAndRIndexBase() {}

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
  }

  /** Return the index name */
  virtual std::string GetName() const = 0;

protected:
  // This method must be reimplemented in subclasses to actually
  // compute the index value
  virtual TOutput Evaluate(const TInput1& g, const TInput2& r) const = 0;
  double m_EpsilonToBeConsideredAsZero;

private:
  unsigned int m_GreenIndex;
  unsigned int m_RedIndex;
};

/**
 * \class GAndRAndNirIndexBase
 * \brief Base class for Green And Red And NIR channels of Spot Images
 *
 *
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template<class TInput1, class TInput2, class TInput3, class TOutput>
class GAndRAndNirIndexBase
{
public:
  /// Vector pixel type used to support both vector images and multiple
  /// input images
  typedef itk::VariableLengthVector<TInput1> InputVectorType;

  //operators !=
  bool operator !=(const GAndRAndNirIndexBase&) const
  {
    return true;
  }
  //operator ==
  bool operator ==(const GAndRAndNirIndexBase& other) const
  {
    return !(*this != other);
  }

  // Operator on vector pixel type
  inline TOutput operator ()(const InputVectorType& inputVector) const
  {
    return this->Evaluate(static_cast<TInput1>(inputVector[m_GreenIndex - 1]),
                          static_cast<TInput2>(inputVector[m_RedIndex - 1]),
                          static_cast<TInput3>(inputVector[m_NIRIndex - 1]));
  }

  // Binary operator
  inline TOutput operator ()(const TInput1& g, const TInput2& r, const TInput2& nir) const
  {
    return this->Evaluate(g, r, nir);
  }
  /// Constructor
  GAndRAndNirIndexBase() : m_EpsilonToBeConsideredAsZero(0.0000001), m_GreenIndex(1), m_RedIndex(2),  m_NIRIndex(3) {}
  /// Desctructor
  virtual ~GAndRAndNirIndexBase() {}

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
  /// Set Nir Index
  void SetNIRIndex(unsigned int channel)
  {
    m_NIRIndex = channel;
  }
  /// Get Nir Index
  unsigned int GetNIRIndex() const
  {
    return m_NIRIndex;
  }

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

  /** Return the index name */
  virtual std::string GetName() const = 0;

protected:
  // This method must be reimplemented in subclasses to actually
  // compute the index value
  virtual TOutput Evaluate(const TInput1& g, const TInput2& r, const TInput2& nir) const = 0;

  double m_EpsilonToBeConsideredAsZero;

private:
  unsigned int m_GreenIndex;
  unsigned int m_RedIndex;
  unsigned int m_NIRIndex;
};

/** \class IR
 *  \brief This functor computes the Redness Index (IR)
 *
 *  [Pouget et al., "Caracteristiques spectrales des surfaces sableuses
 *   de la region cotiere nord-ouest de l'Egypte: application aux donnees
 *   satellitaires Spot, In: 2eme Journeees de Teledetection: Caracterisation
 *   et suivi des milieux terrestres en regions arides et tropicales. 4-6/12/1990
 *   Ed. ORSTOM, Collection Colloques et Seminaires, Paris, pp. 27-38]
 *
 *  \ingroup Functor
 *  \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput1, class TInput2, class TOutput>
class IR : public GAndRIndexBase<TInput1, TInput2, TOutput>
{
public:
  /** Return the index name */
  std::string GetName() const ITK_OVERRIDE
  {
    return "IR";
  }

  /// Constructor
  IR() {}
  /// Desctructor
  ~IR() ITK_OVERRIDE {}
  // Operator on r and nir single pixel values
protected:
  inline TOutput Evaluate(const TInput1& pGreen, const TInput2& pRed) const ITK_OVERRIDE
  {
    double dGreen = static_cast<double>(pGreen);
    double dRed = static_cast<double>(pRed);
    if (vcl_abs(dGreen) < this->m_EpsilonToBeConsideredAsZero)
      {
      return static_cast<TOutput>(0.);
      }

    return static_cast<TOutput>(dRed * dRed / (dGreen * dGreen * dGreen));
  }
};

/** \class IC
 *  \brief This functor computes the Color Index (IC)
 *
 *  [Pouget et al., "Caracteristiques spectrales des surfaces sableuses
 *   de la region cotiere nord-ouest de l'Egypte: application aux donnees
 *   satellitaires Spot, In: 2eme Journeees de Teledetection: Caracterisation
 *   et suivi des milieux terrestres en regions arides et tropicales. 4-6/12/1990
 *   Ed. ORSTOM, Collection Colloques et Seminaires, Paris, pp. 27-38]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput1, class TInput2, class TOutput>
class IC : public GAndRIndexBase<TInput1, TInput2, TOutput>
{
public:
  /** Return the index name */
  std::string GetName() const ITK_OVERRIDE
  {
    return "IC";
  }

  /// Constructor
  IC() {}
  /// Desctructor
  ~IC() ITK_OVERRIDE {}
  // Operator on r and nir single pixel values
protected:
  inline TOutput Evaluate(const TInput1& pGreen, const TInput2& pRed) const ITK_OVERRIDE
  {
    double dGreen = static_cast<double>(pGreen);
    double dRed = static_cast<double>(pRed);
    if (vcl_abs(dGreen + dRed) < this->m_EpsilonToBeConsideredAsZero)
      {
      return static_cast<TOutput>(0.);
      }

    return (static_cast<TOutput>((dRed - dGreen) / (dRed + dGreen)));
  }
};

/** \class IB
 *  \brief This functor computes the Brilliance Index (IB)
 *
 *  [ ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput1, class TInput2, class TOutput>
class IB : public GAndRIndexBase<TInput1, TInput2, TOutput>
{
public:
  /** Return the index name */
  std::string GetName() const ITK_OVERRIDE
  {
    return "IB";
  }

  /// Constructor
  IB() {}
  /// Desctructor
  ~IB() ITK_OVERRIDE {}
  // Operator on r and nir single pixel values
protected:
  inline TOutput Evaluate(const TInput1& pGreen, const TInput2& pRed) const ITK_OVERRIDE
  {
    double dGreen = static_cast<double>(pGreen);
    double dRed = static_cast<double>(pRed);

    return (static_cast<TOutput>(vcl_sqrt((dRed * dRed + dGreen * dGreen) / 2.)));
  }
};

/** \class IB2
 *  \brief This functor computes the Brilliance Index (IB2)
 *
 *  [ ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput1, class TInput2, class TInput3, class TOutput>
class IB2 : public GAndRAndNirIndexBase<TInput1, TInput2, TInput3, TOutput>
{
public:
  /** Return the index name */
  std::string GetName() const ITK_OVERRIDE
  {
    return "IB2";
  }

  /// Constructor
  IB2() {}
  /// Desctructor
  ~IB2() ITK_OVERRIDE {}
  // Operator on r and nir single pixel values
protected:
  inline TOutput Evaluate(const TInput1& pGreen, const TInput2& pRed, const TInput2& pNir) const ITK_OVERRIDE
  {
    double dGreen = static_cast<double>(pGreen);
    double dRed = static_cast<double>(pRed);
    double dNir = static_cast<double>(pNir);

    return (static_cast<TOutput>(vcl_sqrt((dRed * dRed + dGreen * dGreen + dNir * dNir) / 3.)));
  }
};

} // namespace Functor
} // namespace otb

#endif
