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
#ifndef __otbWaterIndicesFunctor_h
#define __otbWaterIndicesFunctor_h

#include "otbMath.h"
#include "itkVariableLengthVector.h"
#include "otbSqrtSpectralAngleFunctor.h"

namespace otb
{
namespace Functor
{
/**
   * \class WaterIndexBase
   * \brief Base class
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
class WaterIndexBase
{
public:
  /// Vector pixel type used to support both vector images and multiple
  /// input images
  typedef itk::VariableLengthVector<TInput1> InputVectorType;

  // Operator on vector pixel type
  inline TOutput operator ()(const InputVectorType& inputVector)
  {
    return this->Evaluate(inputVector[m_Index1 - 1], static_cast<TInput2>(inputVector[m_Index2 - 1]));
  }

  // Binary operator
  inline TOutput operator ()(const TInput1& id1, const TInput2& id2)
  {
    return this->Evaluate(id1, id2);
  }
  /// Constructor
  WaterIndexBase() {}
  /// Desctructor
  virtual ~WaterIndexBase() {}

  /// Set Index 1
  void SetIndex1(unsigned int channel)
  {
    m_Index1 = channel;
  }
  /// Get Index 1
  unsigned int GetIndex1()
  {
    return m_Index1;
  }
  /// Set Index 2
  void SetIndex2(unsigned int channel)
  {
    m_Index2 = channel;
  }
  /// Get Index 2
  unsigned int GetIndex2()
  {
    return m_Index2;
  }
protected:
  // This method must be reimplemented in subclasses to actually
  // compute the index value
  virtual TOutput Evaluate(const TInput1& id1, const TInput2& id2) const = 0;

private:
  unsigned int m_Index1;
  unsigned int m_Index2;
};

/** \class WaterIndexFunctor
 *  \brief This functor will be used for most of water index functors.
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class WaterIndexFunctor : public WaterIndexBase<TInput1, TInput2, TOutput>
{
public:
  WaterIndexFunctor() {}
  virtual ~WaterIndexFunctor() {}
protected:
  inline TOutput Evaluate(const TInput1& id1, const TInput2& id2) const
  {
    double dindex1 = static_cast<double>(id1);
    double dindex2 = static_cast<double>(id2);
    double ddenom = dindex1 + dindex2;
    if (ddenom == 0)
      {
      return static_cast<TOutput>(0.);
      }
    return (static_cast<TOutput>((dindex1 - dindex2) / ddenom));
  }
};

/** \class SRWI
 *  \brief This functor computes the Simple Ratio Water Index (SRWI)
 *  \brief For MODIS bands 860 & 1240
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class SRWI : public WaterIndexBase<TInput1, TInput2, TOutput>
{
public:
  SRWI() {}
  virtual ~SRWI() {}
protected:
  inline TOutput Evaluate(const TInput1& rho860, const TInput2& rho1240) const
  {
    double drho860 = static_cast<double>(rho860);
    double drho1240 = static_cast<double>(rho1240);
    if (drho1240 == 0)
      {
      return static_cast<TOutput>(0.);
      }
    return (static_cast<TOutput>(drho860 / drho1240));
  }
};

/** \class NDWI
 *  \brief This functor computes the Normalized Difference Water Index (NDWI)
 *  \brief Also called :
 *  \brief NDII (Normalized Difference Infrared Index)
 *  \brief LSWI (Land Surface Water Index)
 *  \brief NDWI (Normalized Difference Moisture Index)
 *
 *  [Gao, 1996 ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class NDWI : public WaterIndexBase<TInput1, TInput2, TOutput>
{
public:
  typedef WaterIndexFunctor<TInput1, TInput2, TOutput> WIFunctorType;
  /// Constructor
  NDWI() {}
  /// Desctructor
  virtual ~NDWI() {}
  WIFunctorType GetWIFunctor(void) const
  {
    return (m_WIFunctor);
  }
  /// Set Index NIR
  void SetNIRIndex(unsigned int channel)
  {
    this->SetIndex1(channel);
  }
  /// Get Index NIR
  unsigned int GetNIRIndex()
  {
    return this->GetIndex1();
  }
  /// Set Index MIR
  void SetMIRIndex(unsigned int channel)
  {
    this->SetIndex2(channel);
  }
  /// Get Index MIR
  unsigned int GetMIRIndex()
  {
    return this->GetIndex2();
  }

protected:
  inline TOutput Evaluate(const TInput1& nir, const TInput2& mir) const
  {
    return (static_cast<TOutput>(GetWIFunctor() (nir, mir)));
  }
private:
  // Water Index Classic Functor
  const WIFunctorType m_WIFunctor;
};

/** \class NDWI2
 *  \brief This functor computes the Normalized Difference Water Index (NDWI2)
 *
 *  [McFeeters, 1996 ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class NDWI2 : public WaterIndexBase<TInput1, TInput2, TOutput>
{
public:
  typedef WaterIndexFunctor<TInput1, TInput2, TOutput> WIFunctorType;
  /// Constructor
  NDWI2() {}
  /// Desctructor
  virtual ~NDWI2() {}
  WIFunctorType GetWIFunctor(void) const
  {
    return (m_WIFunctor);
  }
  /// Set Index G
  void SetGIndex(unsigned int channel)
  {
    this->SetIndex1(channel);
  }
  /// Get Index G
  unsigned int GetGIndex()
  {
    return this->GetIndex1();
  }
  /// Set Index NIR
  void SetNIRIndex(unsigned int channel)
  {
    this->SetIndex2(channel);
  }
  /// Get Index NIR
  unsigned int GetNIRIndex()
  {
    return this->GetIndex2();
  }

protected:
  inline TOutput Evaluate(const TInput1& g, const TInput2& nir) const
  {
    return (static_cast<TOutput>(GetWIFunctor() (g, nir)));
  }
private:
  // Water Index Classic Functor
  const WIFunctorType m_WIFunctor;
};

/** \class MNDWI
 *  \brief This functor computes the Modified Normalized Difference Water Index (MNDWI)
 *
 *  [Xu & al., 2006 ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class MNDWI : public WaterIndexBase<TInput1, TInput2, TOutput>
{
public:
  typedef WaterIndexFunctor<TInput1, TInput2, TOutput> WIFunctorType;
  /// Constructor
  MNDWI() {}
  /// Desctructor
  virtual ~MNDWI() {}
  WIFunctorType GetWIFunctor(void) const
  {
    return (m_WIFunctor);
  }
  /// Set Index G
  void SetGIndex(unsigned int channel)
  {
    this->SetIndex1(channel);
  }
  /// Get Index G
  unsigned int GetGIndex()
  {
    return this->GetIndex1();
  }
  /// Set Index MIR
  void SetMIRIndex(unsigned int channel)
  {
    this->SetIndex2(channel);
  }
  /// Get Index MIR
  unsigned int GetMIRIndex()
  {
    return this->GetIndex2();
  }

protected:
  inline TOutput Evaluate(const TInput1& g, const TInput2& mir) const
  {
    return (static_cast<TOutput>(GetWIFunctor() (g, mir)));
  }
private:
  // Water Index Classic Functor
  const WIFunctorType m_WIFunctor;
};

/** \class NDPI
 *  \brief This functor computes the Normalized Difference Pond Index (NDPI)
 *
 *  [J.P Lacaux & al., 2006 ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class NDPI : public WaterIndexBase<TInput1, TInput2, TOutput>
{
public:
  typedef WaterIndexFunctor<TInput1, TInput2, TOutput> WIFunctorType;
  /// Constructor
  NDPI() {}
  /// Desctructor
  virtual ~NDPI() {}
  WIFunctorType GetWIFunctor(void) const
  {
    return (m_WIFunctor);
  }
  /// Set Index MIR
  void SetMIRIndex(unsigned int channel)
  {
    this->SetIndex1(channel);
  }
  /// Get Index MIR
  unsigned int GetMIRIndex()
  {
    return this->GetIndex1();
  }
  /// Set Index G
  void SetGIndex(unsigned int channel)
  {
    this->SetIndex2(channel);
  }
  /// Get Index G
  unsigned int GetGIndex()
  {
    return this->GetIndex2();
  }

protected:
  inline TOutput Evaluate(const TInput1& mir, const TInput2& g) const
  {
    return (static_cast<TOutput>(GetWIFunctor() (mir, g)));
  }
private:
  // Water Index Classic Functor
  const WIFunctorType m_WIFunctor;
};

/** \class NDTI
 *  \brief This functor computes the Normalized Difference Turbidity Index (NDTI)
 *
 *  [J.P Lacaux & al., 2006 ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class NDTI : public WaterIndexBase<TInput1, TInput2, TOutput>
{
public:
  typedef WaterIndexFunctor<TInput1, TInput2, TOutput> WIFunctorType;
  /// Constructor
  NDTI() {}
  /// Desctructor
  virtual ~NDTI() {}
  WIFunctorType GetWIFunctor(void) const
  {
    return (m_WIFunctor);
  }
  /// Set Index R
  void SetRIndex(unsigned int channel)
  {
    this->SetIndex1(channel);
  }
  /// Get Index R
  unsigned int GetRIndex()
  {
    return this->GetIndex1();
  }
  /// Set Index G
  void SetGIndex(unsigned int channel)
  {
    this->SetIndex2(channel);
  }
  /// Get Index G
  unsigned int GetGIndex()
  {
    return this->GetIndex2();
  }

protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& g) const
  {
    return (static_cast<TOutput>(GetWIFunctor() (r, g)));
  }
private:
  // Water Index Classic Functor
  const WIFunctorType m_WIFunctor;
};

/** \class WaterSqrtSpectralAngleFunctor
 *  \brief This functor uses a spectral angle with a particular reference pixel.
 *
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInputVectorPixel, class TOutputPixel>
class WaterSqrtSpectralAngleFunctor : public SqrtSpectralAngleFunctor<TInputVectorPixel, TOutputPixel>
{
public:

  typedef WaterSqrtSpectralAngleFunctor                             Self;
  typedef SqrtSpectralAngleFunctor<TInputVectorPixel, TOutputPixel> Superclass;
  typedef TInputVectorPixel                                         InputVectorPixelType;
  WaterSqrtSpectralAngleFunctor()
  {

    //Set the channels indices
    m_BlueChannel = 0;
    m_GreenChannel = 1;
    m_RedChannel = 2;
    m_NIRChannel = 3;

    //Set reference water value
    InputVectorPixelType reference;
    reference.SetSize(4);
    reference[0] = 136.0; reference[1] = 132.0; reference[2] = 47.0; reference[3] = 24.0;
    this->SetReferenceWaterPixel(reference);
  }
  virtual ~WaterSqrtSpectralAngleFunctor() {}

  /** Set Reference Pixel */
  void SetReferenceWaterPixel(InputVectorPixelType ref)
  {
    if (ref.GetSize() != 4)
      {
      }
    InputVectorPixelType reference;
    reference.SetSize(4);
    reference[m_BlueChannel] = ref[0]; reference[m_GreenChannel] = ref[1]; reference[m_RedChannel] = ref[2];
    reference[m_NIRChannel] = ref[3];
    this->SetReferencePixel(reference);

  }

  /** Getters and setters */
  void SetBlueChannel(unsigned int channel)
  {
    m_BlueChannel = channel;
  }
  unsigned int GetBlueChannel()
  {
    return m_BlueChannel;
  }
  void SetGreenChannel(unsigned int channel)
  {
    m_GreenChannel = channel;
  }
  unsigned int GetGreenChannel()
  {
    return m_GreenChannel;
  }
  void SetRedChannel(unsigned int channel)
  {
    m_RedChannel = channel;
  }
  unsigned int GetRedChannel()
  {
    return m_RedChannel;
  }
  void SetNIRChannel(unsigned int channel)
  {
    m_NIRChannel = channel;
  }
  unsigned int GetNIRChannel()
  {
    return m_NIRChannel;
  }

protected:
  inline TOutputPixel Evaluate(const TInputVectorPixel& inPix) const
  {
    return static_cast<TOutputPixel>(Superclass::Evaluate(inPix));
  }

  /** Channels */
  int m_BlueChannel;
  int m_GreenChannel;
  int m_RedChannel;
  int m_NIRChannel;
};

} // namespace Functor
} // namespace otb

#endif
