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
#ifndef __otbWaveletHighPassOperator_h
#define __otbWaveletHighPassOperator_h

#include "otbWaveletOperatorBase.h"

namespace otb {

/**
 * \class WaveletHighPassOperator
 * \brief Specific implementation of the mother wavelet part of
 * wavelet transformator
 *
 * This class implements the generic construction of the high pass filter
 * in a wavelet transformation.
 *
 * \sa WaveletOperator
 * \sa WaveletGenerator
 *
 * \ingroup OTBWavelet
 */
template <Wavelet::Wavelet TMotherWaveletOperator,
    Wavelet::WaveletDirection TDirectionOfTransformation,
    class TPixel, unsigned int VDimension,
    class TAllocator = itk::NeighborhoodAllocator<TPixel> >
class ITK_EXPORT WaveletHighPassOperator
  : public WaveletOperatorBase<TMotherWaveletOperator,
      TPixel, VDimension, TAllocator>
{
public:
  /** Standard typedefs */
  typedef WaveletHighPassOperator Self;
  typedef WaveletOperatorBase<TMotherWaveletOperator,
      TPixel, VDimension, TAllocator>  Superclass;

  itkTypeMacro(WaveletHighPassOperator, WaveletOperatorBase);

  typedef typename Superclass::SizeType SizeType;
  static const Wavelet::WaveletDirection DirectionOfTransformation = TDirectionOfTransformation;

protected:

  virtual void PrintSelf(std::ostream& os, itk::Indent i) const
  {
    os << i << "WaveletHighPassOperator {this=" << this << "}" << std::endl;
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
      this->m_WaveletGenerator->GetHighPassCoefficientVector(coeff);
      }
    if (DirectionOfTransformation == Wavelet::INVERSE)
      {
      this->m_WaveletGenerator->GetLowPassCoefficientVector(coeff);
      Superclass::GenerateInverseHighPassFilterFromLowPassFilter(coeff);
      }

    Superclass::UpSamplingCoefficients(coeff);
    return coeff;
  }
}; // end of class

} // end of namespace otb

#endif
