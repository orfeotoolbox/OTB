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

namespace otb
{

namespace Functor
{
/**
 * \class SarRadiometricCalibrationFunctor
 * \brief Compute the backscatter value. 
 *  \f$ \sigma^{0} = (scale * DN^{2} + offset) * sin( \theta_{inc}) * OldGain / NewGain * RangeSpreadLoss \f$
 *
 */
template<class TInput, class TOutput>
class ITK_EXPORT SarRadiometricCalibrationFunctor
{
public:
  typedef TInput                           InputType;
  typedef TOutput                          OutputType;
//  typedef double                           RealType;
  typedef typename itk::NumericTraits<InputType>::AbsType   RealType;

  SarRadiometricCalibrationFunctor():
   		m_noise(0.0),
   		m_scale(1.0),
   		m_incidenceAngle(M_PI/2.),
        m_antennaPatternOldGain(1.0),
        m_antennaPatternNewGain(1.0),
        m_rangeSpreadLoss(1.0)
    {
    };

  ~SarRadiometricCalibrationFunctor(){};

  inline TOutput operator ()(const TInput& value) const
  {
	RealType sigma;
	RealType digitalNumber;
	digitalNumber = static_cast<RealType> (value);
	digitalNumber = vcl_abs(digitalNumber);
	sigma  = m_scale * (digitalNumber * digitalNumber) - m_noise;
	sigma *= vcl_sin(m_incidenceAngle);
	sigma *= m_antennaPatternOldGain;
	sigma /= m_antennaPatternNewGain;
	sigma *= m_rangeSpreadLoss;  
	if(sigma < 0.0 )
	{
		sigma = 0.0;
	}

	OutputType result;
	result = static_cast<OutputType>(sigma); 
    return result;
  }

  /** Set offset method */
  void SetNoise(RealType value)
  {
  	m_noise = value;
  }

  /** Get offset method */
  RealType GetNoise() const
  {
  	return m_noise;
  }

  /** Set scale method */
  void SetScale(RealType value)
  {
  	m_scale = value;
  }

  /** Get scale method */
  RealType GetScale() const
  {
  	return m_scale;
  }

  /** Set antennaPatternNewGain method */
  void SetAntennaPatternNewGain(RealType value)
  {
  	m_antennaPatternNewGain = value;
  }

  /** Get antennaPatternNewGain method */
  RealType GetAntennaPatternNewGain() const
  {
  	return m_antennaPatternNewGain;
  }

  /** Set antennaPatternOldGain method */
  void SetAntennaPatternOldGain(RealType value)
  {
  	m_antennaPatternOldGain = value;
  }

  /** Get antennaPatternOldGain method */
  RealType GetAntennaPatternOldGain() const
  {
  	return m_antennaPatternOldGain;
  }
  
  /** Set incidenceAngle method */
  void SetIncidenceAngle(RealType value)
  {
  	m_incidenceAngle = value;
  }

  /** Get incidenceAngle method */
  RealType GetIncidenceAngle() const
  {
  	return m_incidenceAngle;
  }

  /** Set rangeSpreadLoss method */
  void SetRangeSpreadLoss(RealType value)
  {
  	m_rangeSpreadLoss = value;
  }

  /** Get scale method */
  RealType GetRangeSpreadLoss() const
  {
  	return m_rangeSpreadLoss;
  }

private:
  RealType   m_noise;
  RealType   m_scale;
  RealType   m_antennaPatternNewGain;
  RealType   m_antennaPatternOldGain;
  RealType   m_incidenceAngle;
  RealType   m_rangeSpreadLoss;
};
}

}

#endif
