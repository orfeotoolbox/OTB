/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbTerraSarFunctors_h
#define __otbTerraSarFunctors_h


#include "itkUnaryFunctorImageFilter.h"
#include "itkMetaDataDictionary.h"
#include "otbMath.h"

namespace otb
{
namespace Functor
{


/**
   * \class TerraSarRadarBrightnessImageFunctor
   *  \brief Compute the radar brightness from an modulus image.
   *
   * \ingroup Functor
   * \ingroup Radiometry
 */
template <class TInput, class TOutput>
class TerraSarRadarBrightnessImageFunctor
{
public:
  TerraSarRadarBrightnessImageFunctor();
 virtual ~TerraSarRadarBrightnessImageFunctor() {};
 
 typedef std::vector<double>           DoubleVectorType;
 typedef std::vector<DoubleVectorType> DoubleVectorVectorType;
 typedef itk::Size<2>                  SizeType;
 
 /** Accessors */
 void SetCalFactor( double val ) { m_CalFactor = val; };
 double GetCalFactor() { return m_CalFactor; };
 
 /** We assume that the input pixel is a scalar -> modulus image */
 inline TOutput operator() (const TInput & inPix);
 /** We assume that the input pixel is a complex -> complex image */
 inline std::complex<TOutput> operator() (const std::complex<TInput> & inPix);

private:
  /** Calibration Factor */
  double m_CalFactor;
};




/**
   * \class TerraSarCalibrationImageFunctor
   *  \brief Compute sigma naught coefficient from a modulus image.
   *
   * \ingroup Functor
   * \ingroup Radiometry
 */
template<class TInput, class TOutput>
class TerraSarCalibrationImageFunctor
{
public:
  TerraSarCalibrationImageFunctor();
  virtual ~TerraSarCalibrationImageFunctor() {};

  typedef std::vector<double>                                 DoubleVectorType;
  typedef std::vector<DoubleVectorType>                       DoubleVectorVectorType;
  typedef std::vector<long int>                               LIntVectorType;
  typedef itk::Size<2>                                        SizeType;
  typedef itk::Index<2>                                       IndexType;
  typedef TerraSarRadarBrightnessImageFunctor<double, double> BrightnessFunctorType;

  /** Accessors */
  void SetCalFactor( double val ) { m_CalFactor =  val; m_RadarBrightness.SetCalFactor(val); };
  double GetCalFactor() const { return m_CalFactor; };
  void SetNoiseRangeValidityMin( double val ) { m_NoiseRangeValidityMin = val; };
  double GetNoiseRangeValidityMin() const { return m_NoiseRangeValidityMin; };
  void SetNoiseRangeValidityMax( double val ) { m_NoiseRangeValidityMax = val; };
  double GetNoiseRangeValidityMax() const { return m_NoiseRangeValidityMax; };
  void SetNoiseRangeValidityRef( double val ) { m_NoiseRangeValidityRef = val; };
  double GetNoiseRangeValidityRef() const { return m_NoiseRangeValidityRef; };
  void SetLocalIncidentAngle( double val )
  {
    m_LocalIncidentAngle = val;
    m_SinLocalIncidentAngle = vcl_sin(m_LocalIncidentAngle*CONST_PI_180);
  };
  double GetLocalIncidentAngle() const { return m_LocalIncidentAngle; };
  double GetSinLocalIncidentAngle() const { return m_SinLocalIncidentAngle; };
  void SetNoisePolynomialCoefficientsList( DoubleVectorVectorType vect ) { m_NoisePolynomialCoefficientsList = vect; };
  DoubleVectorVectorType GetNoisePolynomialCoefficientsList() const { return m_NoisePolynomialCoefficientsList; };
  void SetImageSize( SizeType size ) { m_ImageSize = size; };
  SizeType GetImageSize() const { return m_ImageSize; };
  void SetUseFastCalibrationMethod( bool b ) { m_UseFastCalibrationMethod = b; };
  bool GetUseFastCalibrationMethod() const { return m_UseFastCalibrationMethod; };
  void SetTimeUTC( LIntVectorType vect ) { m_TimeUTC = vect; };
  LIntVectorType GetTimeUTC() const { return m_TimeUTC; };
  void SetPRF( double val ) { m_PRF = val; m_InvPRF = 1./m_PRF; };
  double GetPRF() const { return m_PRF; };
  double GetInvPRF() const { return m_InvPRF; };
  BrightnessFunctorType GetRadarBrightness() { return m_RadarBrightness; };
  double ComputeCurrentNoise( unsigned int colId );
  DoubleVectorType ComputeCurrentCoeffs( unsigned int lineId );

  /** We assume that the input pixel is a scalar -> modulus image */
  inline TOutput operator() (const TInput & inPix, IndexType index);
  /** We assume that the input pixel is a complex -> complex image */
  inline std::complex<TOutput> operator() (const std::complex<TInput> & inPix, IndexType index);

private:
  /** Calibration Factor */
  double m_CalFactor;
  /** Noise minimal range validity */
  double m_NoiseRangeValidityMin;
  /** Noise maxinimal range validity */
  double m_NoiseRangeValidityMax;
  /** Noise reference range */
  double m_NoiseRangeValidityRef;
  /** Sensor local incident angle in degree */
  double m_LocalIncidentAngle;
  /** sin of the LocalIncidentAngle */
  double m_SinLocalIncidentAngle;
  /** Vector of vector that contain noise polinomial coefficient */
  DoubleVectorVectorType m_NoisePolynomialCoefficientsList;
  /** Image Size */
  SizeType m_ImageSize;
  /** Fast Calibration Method. If set to trus, will consider only the first noise coefficient else,
   *  will use all of them and applied it according to its acquisition UTC time and the coordinates
   *  of the pixel in the image. */
  bool m_UseFastCalibrationMethod;
  /** TimeUTC for each noise coefficient acquisition (in second). */
  LIntVectorType m_TimeUTC;
  /** Pulse Repetition Frequency */
  double m_PRF;
  /** Inverse Pulse Repetition Frequency */
  double m_InvPRF;
 /** Radar Brightness functor */
  BrightnessFunctorType m_RadarBrightness;
};



}// end namespace functor
} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbTerraSarFunctors.txx"
#endif

#endif
