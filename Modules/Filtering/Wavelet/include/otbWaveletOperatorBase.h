/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Mines-Telecom. All rights reserved.
  See IMTCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef otbWaveletOperatorBase_h
#define otbWaveletOperatorBase_h

#include "itkMacro.h"
#include "itkNeighborhoodOperator.h"

// This include is needed here to define MotherWaveletOperatorEnum...
#include "otbWaveletGenerator.h"

namespace otb
{

namespace Wavelet
{
enum WaveletDirection {FORWARD = 0, INVERSE = 1};
}

/**
 * \class WaveletOperatorBase
 *
 * \brief A NeighborhoodOperator wavelet base class
 *
 * This class is the mother class for any wavelet operator that requires
 * "a-trou" approach for shift-invariant wavelet transform. This class has
 * to be derived, it cannot be used directly since GenerateCoefficients() method
 * is still pure virtual.
 *
 * Any wavelet operator that inherits from this WaveletOperatorBase is to be used
 * as a NeighborhoodOperator that should be applied to a
 * NeighborhoodIterator using the NeighborhoodInnerProduct method.
 *
 * It is assumed that any wavelet definition is directional.
 *
 * Set the level of up-sampling though SetUpSampleFactor() before calling
 * CreateDirectional(). Each class that inherits from WaveletOperatorBase has
 * to re-implement GenerateCoefficients().
 *
 * The name of the wavelet is defined here but left pure virtual. It is defined
 * with the LowPass and HighPass inheritance.
 *
 * \sa WaveletLowPassOperator
 * \sa WaveletHighPassOperator
 * \sa NeighborhoodOperator
 * \sa Neighborhood
 *
 * \ingroup Operators
 *
 * \ingroup OTBWavelet
 */
template <Wavelet::Wavelet TMotherWaveletOperator,
    class TPixel, unsigned int VDimension,
    class TAllocator = itk::NeighborhoodAllocator<TPixel> >
class ITK_EXPORT WaveletOperatorBase
  : public itk::NeighborhoodOperator<TPixel, VDimension, TAllocator>
{
public:
  /** Standard typedefs */
  typedef WaveletOperatorBase                                       Self;
  typedef itk::NeighborhoodOperator<TPixel, VDimension, TAllocator> Superclass;

  itkTypeMacro(WaveletOperatorBase, NeighborhoodOperator);

  typedef typename Superclass::SizeType SizeType;
  typedef Wavelet::Wavelet              MotherWaveletOperatorEnumType;
  itkStaticConstMacro(MotherWaveletOperator, MotherWaveletOperatorEnumType, TMotherWaveletOperator);

  /** Construction */
  WaveletOperatorBase() :
    m_UpSampleFactor(0)
    {
    m_WaveletGenerator = WaveletGeneratorType::New();
    }

  /** Construction by copy */
  WaveletOperatorBase(const Self &other)
    : itk::NeighborhoodOperator<TPixel, VDimension, TAllocator> (other),
    m_UpSampleFactor(other.GetUpSampleFactor())
    {
    m_WaveletGenerator = WaveletGeneratorType::New();
    }

  ~WaveletOperatorBase() ITK_OVERRIDE {}

  /** Assignment operator */
  Self & operator =(const Self& other)
  {
    Superclass::operator =(other);
    m_UpSampleFactor = other.GetUpSampleFactor();
    return *this;
  }

  /**
   * Get the level of up sampling of the filter
   */
  unsigned int GetUpSampleFactor() const
  {
    return this->m_UpSampleFactor;
  }

  /**
   * Set the level of up sampling of the filter
   */
  void SetUpSampleFactor(unsigned int upSampleFactor)
  {
    this->m_UpSampleFactor = upSampleFactor;
  }

  /**
   * Get the name of the wavelet when necessary
   */
  virtual const char * GetWaveletName() const
  {
    return this->m_WaveletGenerator->GetWaveletName();
  }

protected:
  /**
   * Prints some debugging information
   */
  void PrintSelf(std::ostream& os, itk::Indent i) const ITK_OVERRIDE;

  typedef WaveletGenerator<TMotherWaveletOperator> WaveletGeneratorType;
  typedef typename WaveletGeneratorType::Pointer   WaveletGeneratorPointerType;

  /**
   * Typedef support for coefficient vector type.  Necessary to
   * work around compiler bug on VC++.
   */
  typedef typename Superclass::CoefficientVector CoefficientVector;
  typedef typename Superclass::PixelType         PixelType;

  /**
   * Perform the "a-trou" algorithm for shift-invariant transformation.
   * It transforms the filter \f$ H(z) \f$ into \f$ H(z^2) \f$.
   */
  void UpSamplingCoefficients(CoefficientVector& coeff);

  /**
   * Performs filter reversion, ie. \f$ H(z^{-1}) \f$.
   */
  void RevertFilter(CoefficientVector& coeff);

  /**
   * Performs the definition of high pass filter in an orthogonal framework
   * It follows conjugate quadrature filter (CQF) of the chapter 5 of
   * "Ten Lecture on Wavelets", of Ingrid Daubechies,
   * Society for Industrial and Applied Mathematics, 1992.
   *
   * It defines filter coefficients as \f$ G(z) = z H(-z^{-1}) \f$.
   * According to Daubechies'notation, it concerns the synthesis (Inverse)
   * part of the filter banc.
   */
  void GenerateInverseHighPassFilterFromLowPassFilter(CoefficientVector& coeff);

  /**
   * Performs the definition of low pass filter in the reconstruction
   * step of the conjugate quadrature filter (CQF) framework.
   *
   * It defines filter coefficients as \f$ {\tilde H}(z) = G(-z) \f$.
   */
  void GenerateInverseLowPassFilterFromHighPassFilter(CoefficientVector& coeff);

  /**
   * Reduce extra zeros on filters
   */
  void ReduceFilterLength(CoefficientVector& coeff);

  /** Arranges coefficients spatially in the memory buffer. */
  void Fill(const CoefficientVector& coeff) ITK_OVERRIDE
  {
    this->FillCenteredDirectional(coeff);
  }

#if 0
  /**
   * Performs the definition of high pass filter from low pass in a
   * bi-orthogonal framework (ie. Quadrature mirror filter).
   * It defines filter coefficients as \f$ g_n = (-1)^{n+1} h_{-n} \f$
   */
  void GetHighPassFilterFromQuadratureLowPassFilter(CoefficientVector& coeff)
  {
    unsigned int length = coeff.size();

    CoefficientVector highPassCoeff (length);
    int               medianPosition = static_cast<int>(length) / 2;

    highPassCoeff[medianPosition] = -coeff[medianPosition];

    double sign = 1.;
    for (int i = 1; i <= medianPosition; ++i)
      {
      highPassCoeff[medianPosition + i] = sign * coeff[medianPosition - i];
      highPassCoeff[medianPosition - i] = sign * coeff[medianPosition + i];
      sign *= -1.;
      }

    coeff = highPassCoeff;
  }

  /**
   * Performs the definition of synthesis filter from analysis one.
   * Input is the forward low pass filter coefficients.
   * It performs \f$ {\tilde G}(z) = -H(-z) \f$.
   */
  void GetInverseHighPassFilterFromForwardLowPassFilter(CoefficientVector& coeff)
  {
    unsigned long length = static_cast<unsigned long>(coeff.size());
    unsigned long medianPosition = length / 2;

    // Wavelet coefficients are always of add size, so that 2*medianPosition < length
    coeff[medianPosition] *= -1.;
    for (unsigned int i = 2; i <= medianPosition; i += 2)
      {
      coeff[medianPosition + i] *= -1.;
      coeff[medianPosition - i] *= -1.;
      }
  }

  /**
   * Performs the definition of synthesis filter from analysis one.
   * Input is the forward high pass filter coefficients. It is valid
   * for orthogonal and biorthogonal cases.
   * It performs \f$ {\tilde H}(z) = G(-z) \f$.
   */
  void GetInverseLowPassFilterFromForwardHighPassFilter(CoefficientVector& coeff)
  {
    unsigned long length = static_cast<unsigned long>(coeff.size());
    unsigned long medianPosition = length / 2;

    // Wavelet coefficients are always of add size, so that 2*medianPosition < length
    for (unsigned int i = 1; i <= medianPosition; i += 2)
      {
      coeff[medianPosition + i] *= -1.;
      coeff[medianPosition - i] *= -1.;
      }
  }

  /**
   * Performs the definition of high pass filter from the inverse low pass one
   * in a Quadrature Mirror Filter bank framework. It is then valid to
   * define high pass filters from orthogonal and biorthogonal wavelets...
   * Input are the inverse low pass filter coefficients.
   * It performs \f$ G(z) = {\tilde H}(-z)\f$.
   */
  void GetForwardHighPassFilterFromInverseLowPassFilter(CoefficientVector& coeff)
  {
    GetInverseLowPassFilterFromForwardHighPassFilter(coeff);
  }
#endif

  unsigned int                m_UpSampleFactor;
  WaveletGeneratorPointerType m_WaveletGenerator;
};

} // end of namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbWaveletOperatorBase.txx"
#endif

#endif
