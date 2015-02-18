/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom / Telecom Bretagne. All rights reserved.
  See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __otbWaveletGenerator_h
#define __otbWaveletGenerator_h

#include <vector>
#include "itkLightObject.h"
#include "itkObjectFactory.h"

namespace otb
{

namespace Wavelet
{
enum Wavelet {
  HAAR = 0,
  DAUBECHIES4 = 1, DB4 = 1,
  DAUBECHIES6 = 2, DB6 = 2,
  DAUBECHIES8 = 3, DB8 = 3,
  DAUBECHIES12 = 4, DB12 = 4,
  DAUBECHIES20 = 5, DB20 = 5,
  SPLINE_BIORTHOGONAL_2_4, // 6
  SPLINE_BIORTHOGONAL_4_4, // 7
  SYMLET8, // 8
  TotalNumberOfDefinedMotherWavelets
  };
}

/** \class WaveletGenerator
 * \brief Wavelet coefficient definition
 *
 * The wavelet coefficent definitions mainly come from
 * "Ten Lecture on Wavelets", of Ingrid Daubechies,
 * Society for Industrial and Applied Mathematics, 1992.
 *
 * The class is templated with the wavelet ID from the
 * Wavelet::Wavelet type. The members throw an
 * exception if the template specialization is not defined
 * according to the wavelet ID.
 */
template <Wavelet::Wavelet TMotherWaveletOperator>
class ITK_EXPORT WaveletGenerator
  : public itk::LightObject
{
public:
  /** Standard typedefs */
  typedef WaveletGenerator              Self;
  typedef itk::LightObject              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Type macro */
  itkNewMacro(Self);

  /** Creation through object factory macro */
  itkTypeMacro(WaveletGenerator, LightObject);

  /** Typedef similar to itk::NeighborhoodOperator */
  typedef std::vector<double> CoefficientVector;

  /**
   * GetWaveletName
   * By default (without template specification) it does nothing usable
   */
  const char * GetWaveletName() const;

  /**
   * GetLowPassCoefficientVector
   * By default (without template specification) it does nothing usable
   */
  void GetLowPassCoefficientVector(CoefficientVector& coeff) const;

  /**
   * GetHighPassCoefficientVector
   * By default (without template specification) it does nothing usable
   */
  void GetHighPassCoefficientVector(CoefficientVector& coeff) const;

protected:
  WaveletGenerator() {}
  ~WaveletGenerator() {}

private:
  WaveletGenerator(const Self &);  // not implemented
  void operator =(const Self&); // not implemented
}; // end of class

} // end of namespace otb

#endif
