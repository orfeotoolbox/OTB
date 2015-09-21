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
#ifndef __otbWaveletLowPassOperator_h
#define __otbWaveletLowPassOperator_h

#include "otbWaveletOperatorBase.h"

namespace otb {

/**
 * \class WaveletLowPassOperator
 * \brief Specific implementation of scale part of wavelet transformator
 *
 * This class implements the generic construction of the low pass filter
 * in a wavelet transformation.
 *
 * \sa WaveletOperatorBase
 * \sa WaveletGenerator
 *
 * \ingroup OTBWavelet
 */
template <Wavelet::Wavelet TMotherWaveletOperator,
    Wavelet::WaveletDirection TDirectionOfTransformation,
    class TPixel, unsigned int VDimension,
    class TAllocator = itk::NeighborhoodAllocator<TPixel> >
class ITK_EXPORT WaveletLowPassOperator
  : public WaveletOperatorBase<TMotherWaveletOperator,
      TPixel, VDimension, TAllocator>
{
public:
  /** Standard typedefs */
  typedef WaveletLowPassOperator Self;
  typedef WaveletOperatorBase<TMotherWaveletOperator,
      TPixel, VDimension, TAllocator>  Superclass;

  itkTypeMacro(WaveletLowPassOperator, WaveletOperatorBase);

  typedef typename Superclass::SizeType SizeType;
  static const Wavelet::WaveletDirection DirectionOfTransformation = TDirectionOfTransformation;

protected:

  virtual void PrintSelf(std::ostream& os, itk::Indent i) const
  {
    os << i << "WaveletLowPassOperator {this=" << this << "}" << std::endl;
    Superclass::PrintSelf(os, i.GetNextIndent());
  }

  /**
   * Typedef support for coefficient vector type.  Necessary to
   * work around compiler bug on VC++.
   */
  typedef typename Superclass::CoefficientVector CoefficientVector;
  typedef typename Superclass::PixelType         PixelType;

  /**
   * Set operator coefficients.
   */
  CoefficientVector GenerateCoefficients()
  {
    CoefficientVector coeff;
    if (DirectionOfTransformation == Wavelet::FORWARD)
      {
        this->m_WaveletGenerator->GetLowPassCoefficientVector(coeff);
      }
    if (DirectionOfTransformation == Wavelet::INVERSE)
      {
        this->m_WaveletGenerator->GetHighPassCoefficientVector(coeff);
        Superclass::GenerateInverseLowPassFilterFromHighPassFilter(coeff);
      }

    Superclass::UpSamplingCoefficients(coeff);
    return coeff;
  }
}; // end of class

} // end of namespace otb

#endif
