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

#ifndef __otbSarBrightnessFunctor_h
#define __otbSarBrightnessFunctor_h

#include "otbMath.h"
#include "itkNumericTraits.h"

namespace otb
{

namespace Functor
{
/**
 * \class SarBrightnessFunctor
 * \brief Compute the brightness value.
 *  \f$ \beta^{0} = (scale * DN^{2} + offset) * OldGain / NewGain * RangeSpreadLoss \f$
 *
 */
template<class TInput, class TOutput>
class ITK_EXPORT SarBrightnessFunctor
{
public:
  typedef TInput                                            InputType;
  typedef TOutput                                           OutputType;
  typedef typename itk::NumericTraits<InputType>::AbsType   RealType;

  SarBrightnessFunctor()
  {
    m_Noise = 0.0;
    m_Scale = 1.0;
    m_AntennaPatternOldGain = 1.0;
    m_AntennaPatternNewGain = 1.0;
    m_RangeSpreadLoss = 1.0;
  };

  ~SarBrightnessFunctor(){};

  inline TOutput operator ()(const TInput& value) const
  {
    RealType digitalNumber = static_cast<RealType> (vcl_abs(value));
    RealType beta;

    beta  = m_Scale * (digitalNumber * digitalNumber - m_Noise);
    beta *= m_AntennaPatternOldGain;
    beta /= m_AntennaPatternNewGain;
    beta *= m_RangeSpreadLoss;

    if(beta < 0.0)
    {
      beta = 0.0;
    }

    return static_cast<OutputType>(beta);
  }

  /** Set offset method */
  void SetNoise(RealType value)
  {
    m_Noise = value;
  }

  /** Get offset method */
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

  /** Set rangeSpreadLoss method */
  void SetRangeSpreadLoss(RealType value)
  {
    m_RangeSpreadLoss = value;
  }

  /** Get scale method */
  RealType GetRangeSpreadLoss() const
  {
    return m_RangeSpreadLoss;
  }

private:
  RealType   m_Noise;
  RealType   m_Scale;
  RealType   m_AntennaPatternNewGain;
  RealType   m_AntennaPatternOldGain;
  RealType   m_RangeSpreadLoss;
};
}

}

#endif
