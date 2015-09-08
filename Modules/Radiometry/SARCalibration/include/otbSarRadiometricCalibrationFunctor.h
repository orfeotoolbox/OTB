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

#ifndef __otbSarRadiometricCalibrationFunctor_h
#define __otbSarRadiometricCalibrationFunctor_h

#include "otbMath.h"
#include "itkNumericTraits.h"
#include <iomanip>
#include <iostream>
namespace otb
{

namespace Functor
{
/**
 * \class SarRadiometricCalibrationFunctor
 * \brief Compute the backscatter value.
 *  \f$ \sigma^{0} = ( ( (scale * DN^{2} + offset) * sin( \theta_{inc}) * OldGain /
 * NewGain * RangeSpreadLoss ) /  LookupValue ) / RescalingFactor ) \f$
 *
 *
 * \ingroup OTBSARCalibration
 */
template<class TInput, class TOutput>
class ITK_EXPORT SarRadiometricCalibrationFunctor
{
public:
  typedef TInput                                            InputType;
  typedef TOutput                                           OutputType;
  typedef typename itk::NumericTraits<InputType>::AbsType   RealType;

  SarRadiometricCalibrationFunctor()
  {
    m_Noise = 0.0;
    m_Scale = 1.0;
    m_IncidenceAngle = CONST_PI_2;
    m_AntennaPatternOldGain = 1.0;
    m_AntennaPatternNewGain = 1.0;
    m_RangeSpreadLoss = 1.0;
    m_LutValue = 1.0;
    m_RescalingFactor = 1.0;
  };

  ~SarRadiometricCalibrationFunctor(){};

  inline TOutput operator ()(const TInput& value) const
  {
    RealType digitalNumber = static_cast<RealType> (vcl_abs(value));
    RealType sigma;

    sigma  = m_Scale * (digitalNumber * digitalNumber - m_Noise);
    sigma *= vcl_sin(m_IncidenceAngle);
    sigma *= m_AntennaPatternOldGain;
    sigma /= m_AntennaPatternNewGain;
    sigma *= m_RangeSpreadLoss;
    /** Lookup value has effect on for some sensors which does not required the
* above values (incidence angle, rangespreadloss etc.. */
    sigma /= m_LutValue;
    /** rescling factor has effect only with CosmoSkymed Products */
    sigma /= m_RescalingFactor;

    if(sigma < 0.0)
    {
      sigma = 0.0;
    }

    return static_cast<OutputType>(sigma);
  }

  /** Set RescalingFactor method */
  void SetRescalingFactor(RealType value)
  {
    m_RescalingFactor = value;
  }

  /** Set LookupValue method */
  void SetLutValue(RealType value)
  {
    m_LutValue = value;
  }

  /** Set noise method */
  void SetNoise(RealType value)
  {
    m_Noise = value;
  }

  /** Get noise method */
  RealType GetNoise() const
  {
    return m_Noise;
  }

  /** Set scale method */
  void SetScale(RealType value)
  {
    m_Scale = value;
  }

  /** Get scale method */
  RealType GetScale() const
  {
    return m_Scale;
  }

  /** Set antennaPatternNewGain method */
  void SetAntennaPatternNewGain(RealType value)
  {
    m_AntennaPatternNewGain = value;
  }

  /** Get antennaPatternNewGain method */
  RealType GetAntennaPatternNewGain() const
  {
    return m_AntennaPatternNewGain;
  }

  /** Set antennaPatternOldGain method */
  void SetAntennaPatternOldGain(RealType value)
  {
    m_AntennaPatternOldGain = value;
  }

  /** Get antennaPatternOldGain method */
  RealType GetAntennaPatternOldGain() const
  {
    return m_AntennaPatternOldGain;
  }

  /** Set incidenceAngle method */
  void SetIncidenceAngle(RealType value)
  {
    m_IncidenceAngle = value;
  }

  /** Get incidenceAngle method */
  RealType GetIncidenceAngle() const
  {
    return m_IncidenceAngle;
  }

  /** Set rangeSpreadLoss method */
  void SetRangeSpreadLoss(RealType value)
  {
    m_RangeSpreadLoss = value;
  }

  /** Get rangeSpreadLoss method */
  RealType GetRangeSpreadLoss() const
  {
    return m_RangeSpreadLoss;
  }

private:
  RealType   m_Noise;
  RealType   m_Scale;
  RealType   m_AntennaPatternNewGain;
  RealType   m_AntennaPatternOldGain;
  RealType   m_IncidenceAngle;
  RealType   m_RangeSpreadLoss;
  RealType  m_LutValue;
  RealType  m_RescalingFactor;

};
}

}

#endif
