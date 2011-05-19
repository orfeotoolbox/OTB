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
#include "otbBandName.h"

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

  //operators !=
  bool operator !=(const WaterIndexBase&) const
  {
    return true;
  }
  //operator ==
  bool operator ==(const WaterIndexBase& other) const
  {
    return !(*this != other);
  }

  // Operator on vector pixel type
  inline TOutput operator ()(const InputVectorType& inputVector) const
  {
    return this->Evaluate(inputVector[m_Index1 - 1], static_cast<TInput2>(inputVector[m_Index2 - 1]));
  }

  // Binary operator
  inline TOutput operator ()(const TInput1& id1, const TInput2& id2) const
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
  unsigned int GetIndex1() const
  {
    return m_Index1;
  }
  /// Set Index 2
  void SetIndex2(unsigned int channel)
  {
    m_Index2 = channel;
  }
  /// Get Index 2
  unsigned int GetIndex2() const
  {
    return m_Index2;
  }

  /** Return the index name */
  virtual std::string GetName() const = 0;

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
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "WaterIndexFunctor";
  }

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
 *   [Zarco-Tejada 2001]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class SRWI : public WaterIndexBase<TInput1, TInput2, TOutput>
{
public:
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "SRWI";
  }

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
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "NDWI";
  }

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
  unsigned int GetNIRIndex() const
  {
    return this->GetIndex1();
  }
  /// Set Index MIR
  void SetMIRIndex(unsigned int channel)
  {
    this->SetIndex2(channel);
  }
  /// Get Index MIR
  unsigned int GetMIRIndex() const
  {
    return this->GetIndex2();
  }

  /** Set index, generic method */
  void SetIndex(BandName::BandName band, unsigned int channel)
  {
    if (band == BandName::NIR)
      {
      this->SetIndex1(channel);
      }
    if (band == BandName::MIR)
      {
      this->SetIndex2(channel);
      }
  }
  /** Get index, generic method */
  unsigned int GetIndex(BandName::BandName band) const
  {
    if (band == BandName::NIR)
      {
      return this->GetIndex1();
      }
    if (band == BandName::MIR)
      {
      return this->GetIndex2();
      }
  }

protected:
  inline TOutput Evaluate(const TInput1& nir, const TInput2& mir) const
  {
    return (static_cast<TOutput>(GetWIFunctor() (nir, mir)));
  }
private:
  // Water Index Classic Functor
  WIFunctorType m_WIFunctor;
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
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "NDWI2";
  }

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
  unsigned int GetGIndex() const
  {
    return this->GetIndex1();
  }
  /// Set Index NIR
  void SetNIRIndex(unsigned int channel)
  {
    this->SetIndex2(channel);
  }
  /// Get Index NIR
  unsigned int GetNIRIndex() const
  {
    return this->GetIndex2();
  }

  /** Set index, generic method */
  void SetIndex(BandName::BandName band, unsigned int channel)
  {
    if (band == BandName::GREEN)
      {
      this->SetIndex1(channel);
      }
    if (band == BandName::NIR)
      {
      this->SetIndex2(channel);
      }
  }
  /** Get index, generic method */
  unsigned int GetIndex(BandName::BandName band) const
  {
    if (band == BandName::GREEN)
      {
      return this->GetIndex1();
      }
    if (band == BandName::NIR)
      {
      return this->GetIndex2();
      }
  }

protected:
  inline TOutput Evaluate(const TInput1& g, const TInput2& nir) const
  {
    return (static_cast<TOutput>(GetWIFunctor() (g, nir)));
  }
private:
  // Water Index Classic Functor
  WIFunctorType m_WIFunctor;
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
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "MNDWI";
  }

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
  unsigned int GetGIndex() const
  {
    return this->GetIndex1();
  }
  /// Set Index MIR
  void SetMIRIndex(unsigned int channel)
  {
    this->SetIndex2(channel);
  }
  /// Get Index MIR
  unsigned int GetMIRIndex() const
  {
    return this->GetIndex2();
  }

  /** Set index, generic method */
  void SetIndex(BandName::BandName band, unsigned int channel)
  {
    if (band == BandName::GREEN)
      {
      this->SetIndex1(channel);
      }
    if (band == BandName::MIR)
      {
      this->SetIndex2(channel);
      }
  }
  /** Get index, generic method */
  unsigned int GetIndex(BandName::BandName band) const
  {
    if (band == BandName::GREEN)
      {
      return this->GetIndex1();
      }
    if (band == BandName::MIR)
      {
      return this->GetIndex2();
      }
  }

protected:
  inline TOutput Evaluate(const TInput1& g, const TInput2& mir) const
  {
    return (static_cast<TOutput>(GetWIFunctor() (g, mir)));
  }
private:
  // Water Index Classic Functor
  WIFunctorType m_WIFunctor;
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
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "NDPI";
  }

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
  unsigned int GetMIRIndex() const
  {
    return this->GetIndex1();
  }
  /// Set Index G
  void SetGIndex(unsigned int channel)
  {
    this->SetIndex2(channel);
  }
  /// Get Index G
  unsigned int GetGIndex() const
  {
    return this->GetIndex2();
  }

  /** Set index, generic method */
  void SetIndex(BandName::BandName band, unsigned int channel)
  {
    if (band == BandName::MIR)
      {
      this->SetIndex1(channel);
      }
    if (band == BandName::GREEN)
      {
      this->SetIndex2(channel);
      }
  }
  /** Get index, generic method */
  unsigned int GetIndex(BandName::BandName band) const
  {
    if (band == BandName::MIR)
      {
      return this->GetIndex1();
      }
    if (band == BandName::GREEN)
      {
      return this->GetIndex2();
      }
  }

protected:
  inline TOutput Evaluate(const TInput1& mir, const TInput2& g) const
  {
    return (static_cast<TOutput>(GetWIFunctor() (mir, g)));
  }
private:
  // Water Index Classic Functor
  WIFunctorType m_WIFunctor;
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
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "NDTI";
  }

  typedef WaterIndexFunctor<TInput1, TInput2, TOutput> WIFunctorType;
  /// Constructor
  NDTI() {}
  /// Desctructor
  virtual ~NDTI() {}
  WIFunctorType GetWIFunctor(void) const
  {
    return (m_WIFunctor);
  }
  // FIXME why now using Red and Green fully spelled as everywhere
  //else ???
  /// Set Index R
  void SetRIndex(unsigned int channel)
  {
    this->SetIndex1(channel);
  }
  /// Get Index R
  unsigned int GetRIndex() const
  {
    return this->GetIndex1();
  }
  /// Set Index G
  void SetGIndex(unsigned int channel)
  {
    this->SetIndex2(channel);
  }
  /// Get Index G
  unsigned int GetGIndex() const
  {
    return this->GetIndex2();
  }

  /** Set index, generic method */
  void SetIndex(BandName::BandName band, unsigned int channel)
  {
    if (band == BandName::RED)
      {
      this->SetIndex1(channel);
      }
    if (band == BandName::GREEN)
      {
      this->SetIndex2(channel);
      }
  }
  /** Get index, generic method */
  unsigned int GetIndex(BandName::BandName band) const
  {
    if (band == BandName::RED)
      {
      return this->GetIndex1();
      }
    if (band == BandName::GREEN)
      {
      return this->GetIndex2();
      }
  }

protected:
  inline TOutput Evaluate(const TInput1& r, const TInput2& g) const
  {
    return (static_cast<TOutput>(GetWIFunctor() (r, g)));
  }
private:
  // Water Index Classic Functor
  WIFunctorType m_WIFunctor;
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
  /** Return the index name */
  virtual std::string GetName() const
  {
    return "WaterSqrtSpectralAngleFunctor";
  }
  typedef WaterSqrtSpectralAngleFunctor                             Self;
  typedef SqrtSpectralAngleFunctor<TInputVectorPixel, TOutputPixel> Superclass;
  typedef TInputVectorPixel                                         InputVectorPixelType;
  WaterSqrtSpectralAngleFunctor()
  {

    //Set the channels indices
    m_BlueIndex = 0;
    m_GreenIndex = 1;
    m_RedIndex = 2;
    m_NIRIndex = 3;

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
    reference[m_BlueIndex] = ref[0]; reference[m_GreenIndex] = ref[1]; reference[m_RedIndex] = ref[2];
    reference[m_NIRIndex] = ref[3];
    this->SetReferencePixel(reference);

  }

  /** Getters and setters */
  void SetBlueChannel(unsigned int channel)
  {
    m_BlueIndex = channel;
  }
  unsigned int GetBlueChannel() const
  {
    return m_BlueIndex;
  }
  void SetGreenChannel(unsigned int channel)
  {
    m_GreenIndex = channel;
  }
  unsigned int GetGreenChannel() const
  {
    return m_GreenIndex;
  }
  void SetRedChannel(unsigned int channel)
  {
    m_RedIndex = channel;
  }
  unsigned int GetRedChannel() const
  {
    return m_RedIndex;
  }
  void SetNIRChannel(unsigned int channel)
  {
    m_NIRIndex = channel;
  }
  unsigned int GetNIRChannel() const
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
    if (band == BandName::GREEN)
      {
      return m_GreenIndex;
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

protected:
  inline TOutputPixel Evaluate(const TInputVectorPixel& inPix) const
  {
    return static_cast<TOutputPixel>(Superclass::Evaluate(inPix));
  }

  /** Channels */
  int m_BlueIndex;
  int m_GreenIndex;
  int m_RedIndex;
  int m_NIRIndex;
};

} // namespace Functor
} // namespace otb

#endif
