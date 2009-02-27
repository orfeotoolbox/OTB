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
#ifndef __otbSoilIndicesFunctor_h
#define __otbSoilIndicesFunctor_h

#include "otbMath.h"
#include "itkVariableLengthVector.h"

namespace otb
{
namespace Functor
{
/**
   * \class GAndRIndexBase
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
*/
template<class TInput1, class TInput2, class TOutput>
class GAndRIndexBase
{
public:
  /// Vector pixel type used to support both vector images and multiple
  /// input images
  typedef itk::VariableLengthVector<TInput1> InputVectorType;

  // Operator on vector pixel type
  inline TOutput operator()(const InputVectorType & inputVector)
  {
    return this->Evaluate(inputVector[m_GreenIndex-1],static_cast<TInput2>(inputVector[m_RedIndex-1]));
  }

  // Binary operator
  inline TOutput operator()(const TInput1 &g, const TInput2 &r)
  {
    return this->Evaluate(g,r);
  };
  /// Constructor
  GAndRIndexBase() : m_GreenIndex(1), m_RedIndex(2) {};
  /// Desctructor
  virtual ~GAndRIndexBase() {};

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
protected:
  // This method must be reimplemented in subclasses to actually
  // compute the index value
  virtual TOutput Evaluate(const TInput1 & g, const TInput2 & r) const = 0;

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
 */
template<class TInput1, class TInput2, class TInput3, class TOutput>
class GAndRAndNirIndexBase
{
public:
  /// Vector pixel type used to support both vector images and multiple
  /// input images
  typedef itk::VariableLengthVector<TInput1> InputVectorType;

  // Operator on vector pixel type
  inline TOutput operator()(const InputVectorType & inputVector)
  {
    return this->Evaluate(static_cast<TInput1>(inputVector[m_GreenIndex-1]),static_cast<TInput2>(inputVector[m_RedIndex-1]),static_cast<TInput3>(inputVector[m_NIRIndex-1]));
  }

  // Binary operator
  inline TOutput operator()(const TInput1 &g, const TInput2 &r, const TInput2 &nir)
  {
    return this->Evaluate(g,r,nir);
  };
  /// Constructor
  GAndRAndNirIndexBase() : m_GreenIndex(1), m_RedIndex(2),  m_NIRIndex(3) {};
  /// Desctructor
  virtual ~GAndRAndNirIndexBase() {};

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
  /// Set Nir Index
  void SetNIRIndex(unsigned int channel)
  {
    m_NIRIndex = channel;
  }
  /// Get Nir Index
  unsigned int GetNIRIndex()
  {
    return m_NIRIndex;
  }
protected:
  // This method must be reimplemented in subclasses to actually
  // compute the index value
  virtual TOutput Evaluate(const TInput1 & g, const TInput2 & r, const TInput2 & nir) const = 0;

private:
  unsigned int m_GreenIndex;
  unsigned int m_RedIndex;
  unsigned int m_NIRIndex;
};




/** \class IR
 *  \brief This functor computes the Red Index (IR)
 *
 *  [ ]
 *
 *  \ingroup Functor
 *  \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class IR : public GAndRIndexBase<TInput1,TInput2,TOutput>
{
public:
  /// Constructor
  IR() {};
  /// Desctructor
  ~IR() {};
  // Operator on r and nir single pixel values
protected:
  inline TOutput Evaluate(const TInput1 &pGreen, const TInput2 &pRed) const
  {
    double dGreen = static_cast<double>(pGreen);
    double dRed = static_cast<double>(pRed);
    if ( dGreen == 0 )
      {
      return static_cast<TOutput>(0.);
      }

    return ( static_cast<TOutput>( vcl_pow(dRed,2.)/vcl_pow(dGreen,3.) ) );
  }
};

/** \class IC
 *  \brief This functor computes the Color Index (IC)
 *
 *  [ ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class IC : public GAndRIndexBase<TInput1,TInput2,TOutput>
{
public:
  /// Constructor
  IC() {};
  /// Desctructor
  ~IC() {};
  // Operator on r and nir single pixel values
protected:
  inline TOutput Evaluate(const TInput1 &pGreen, const TInput2 &pRed) const
  {
    double dGreen = static_cast<double>(pGreen);
    double dRed = static_cast<double>(pRed);
    if ( dGreen + dRed == 0 )
      {
      return static_cast<TOutput>(0.);
      }

    return ( static_cast<TOutput>( (dRed - dGreen)/(dRed + dGreen) ) );
  }
};

/** \class IB
 *  \brief This functor computes the Brilliance Index (IB)
 *
 *  [ ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class IB : public GAndRIndexBase<TInput1,TInput2,TOutput>
{
public:
  /// Constructor
  IB() {};
  /// Desctructor
  ~IB() {};
  // Operator on r and nir single pixel values
protected:
  inline TOutput Evaluate(const TInput1 &pGreen, const TInput2 &pRed) const
  {
    double dGreen = static_cast<double>(pGreen);
    double dRed = static_cast<double>(pRed);

    return ( static_cast<TOutput>( vcl_sqrt((dRed*dRed + dGreen*dGreen)/2.) ));
  }
};

/** \class IB2
 *  \brief This functor computes the Brilliance Index (IB2)
 *
 *  [ ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TInput3, class TOutput>
class IB2 : public GAndRAndNirIndexBase<TInput1,TInput2,TInput3,TOutput>
{
public:
  /// Constructor
  IB2() {};
  /// Desctructor
  ~IB2() {};
  // Operator on r and nir single pixel values
protected:
  inline TOutput Evaluate(const TInput1 &pGreen, const TInput2 &pRed, const TInput2 &pNir) const
  {
    double dGreen = static_cast<double>(pGreen);
    double dRed = static_cast<double>(pRed);
    double dNir = static_cast<double>(pNir);

    return ( static_cast<TOutput>( vcl_sqrt((dRed*dRed + dGreen*dGreen + dNir*dNir)/3.) ));
  }
};


} // namespace Functor
} // namespace otb

#endif
