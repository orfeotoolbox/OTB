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
#ifndef __otbRadarFunctors_h
#define __otbRadarFunctors_h


#include "itkUnaryFunctorImageFilter.h"
#include "otbTerraSarCalibrationImageFilter.h"
#include "itkMetaDataDictionary.h"
#include "otbMath.h"

namespace otb
{
namespace Functor
{
/**
   * \class TerraSarRadarBrightnessImageFunctor
   *  \brief Compute the radar brightness from a TOA reflectance.
   *
   * \ingroup Functor
   * \ingroup Radiometry
 */
template <class TInput, class TOutput>
class TerraSarRadarBrightnessImageFunctor
{
public:
  TerraSarRadarBrightnessImageFunctor()
  {
    m_CalFactor = 1.;
  };
  virtual ~TerraSarRadarBrightnessImageFunctor() {};

  /** Accessors */
  void SetCalFactor( double val ) { m_CalFactor = val; };
  double GetCalFactor() { return m_CalFactor; };


  inline TOutput operator() (const TInput & inPix)
  {
    double squareInPix = vcl_pow( static_cast<double>(inPix), 2.);
    // Beta naught computation
    double beta = m_CalFactor*squareInPix;

    return static_cast<TOutput>(beta);
  }

private:
  /** Calibration Factor */
  double m_CalFactor;
};



/**
   * \class TerraSarRadarBrightnessComplexImageFunctor
   *  \brief Compute the radar brightness from a TOA reflectance.
   *
   * \ingroup Functor
   * \ingroup Radiometry
 */
template <class TInput, class TOutput>
class TerraSarRadarBrightnessComplexImageFunctor
{
public:
  TerraSarRadarBrightnessComplexImageFunctor() {};
  virtual ~TerraSarRadarBrightnessComplexImageFunctor() {};

  typedef TerraSarRadarBrightnessImageFunctor<TInput, TOutput> FunctorType;

  /** Accessors */
  void SetCalFactor( double val ) { m_BetaNaughtFunctor->SetCalFactor(val); };
  double GetCalFactor() { return m_BetaNaughtFunctor->GetCalFactor(); };

  /* We assume that the input pixel is a complex */
  inline TOutput operator() (const TInput & inPix)
  {


    // Beta naught computation, will be the Modulus of the result
    double beta = m_BetaNaughtFunctor(inPix.abs());

    // Phase 
    double phase = inPix.arg();

    // We retrieve the complex value from the modulus and the phase.
    std::complex<double> res = std::complex<double>(beta*vcl_cos(phase), beta*vcl_sin(phase) );

    return static_cast<TOutput>(res);
  }

private:
  /** Calibration Factor */
  FunctorType m_BetaNaughtFunctor;
};


/**
   * \class TerraSarCalibrationComplexImageFunctor
   *  \brief Compute the surface reflectance pixel from a TOA reflectance with complex images
   *
   * \ingroup Functor
   * \ingroup Radiometry
 */
template <class TInput, class TOutput>
class TerraSarCalibrationComplexImageFunctor
{
public:
  TerraSarCalibrationComplexImageFunctor() {};
  virtual ~TerraSarCalibrationComplexImageFunctor() {};

  typedef TerraSarCalibrationImageFunctor<TInput, TOutput> FunctorType;
  typedef std::vector<double>           DoubleVectorType;
  typedef std::vector<DoubleVectorType> DoubleVectorVectorType;
  typedef itk::Size<2>                  SizeType;

  /** Accessors */
  void SetCalFactor( double val ) { m_SigmaNaughtFunctor.SetCalFactor(val); };
  double GetCalFactor() { return  m_SigmaNaughtFunctor.GetCalFactor(); };
  void SetNoiseRangeValidityMin( double val ) { m_SigmaNaughtFunctor.SetNoiseRangeValidityMin(val); };
  double GetNoiseRangeValidityMin() { return m_SigmaNaughtFunctor.GetNoiseRangeValidityMin(); };
  void SetNoiseRangeValidityMax( double val ) { m_SigmaNaughtFunctor.SetNoiseRangeValidityMax(val); };
  double GetNoiseRangeValidityMax() { return m_SigmaNaughtFunctor.GetNoiseRangeValidityMax(); };
  void SetNoiseRangeValidityRef( double val ) { m_SigmaNaughtFunctor.SetNoiseRangeValidityRe(val); };
  double GetNoiseRangeValidityRef() { return m_SigmaNaughtFunctor.GetNoiseRangeValidityRef(); };
  void SetLocalIncidentAngle( double val ){m_SigmaNaughtFunctor.SetLocalIncidentAngle(val);};
  double GetLocalIncidentAngle() { return m_SigmaNaughtFunctor.GetLocalIncidentAngle(); };
  double GetSinLocalIncidentAngle() const { return m_SigmaNaughtFunctor.GetSinLocalIncidentAngle(); };
  void SetNoisePolynomialCoefficientsList( DoubleVectorVectorType vect ) { m_SigmaNaughtFunctor.SetNoisePolynomialCoefficientsList(vect); };
  DoubleVectorVectorType GetNoisePolynomialCoefficientsList() { return m_SigmaNaughtFunctor.GetNoisePolynomialCoefficientsList(); };
  void SetImageSize( SizeType size ) { m_SigmaNaughtFunctor.SetImageSize(size); };
  SizeType GetImageSize() { return m_SigmaNaughtFunctor.GetImageSize(); };

  /* We assume that the input pixel is a complex */
  inline TOutput operator() (const TInput & inPix)
  {

    // Beta naught computation, will be the Modulus of the result
    double sigma = m_SigmaNaughtFunctor(inPix.abs());

    // Phase 
    double phase = inPix.arg();

    // We retrieve the complex value from the modulus and the phase.
    std::complex<double> res = std::complex<double>(sigma*vcl_cos(phase), sigma*vcl_sin(phase) );

    return static_cast<TOutput>(res);
  }

private:
  /** Calibration Factor */
  FunctorType m_SigmaNaughtFunctor;
};

}
} // end namespace otb


#endif
