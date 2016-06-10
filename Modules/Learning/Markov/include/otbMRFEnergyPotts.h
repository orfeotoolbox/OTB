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

#ifndef __otbMRFEnergyPotts_h
#define __otbMRFEnergyPotts_h

#include "otbMRFEnergy.h"

namespace otb
{
/**
 * \class MRFEnergyPotts
 * \brief This is the implementation of the Potts model for Markov classification.
 *
 * This is the implementation of the Potts model for Markov classification, to be used for
 * regularization. Energy is:
 * \f[  U(x_s, x_t) = -\beta \textrm{ if } x_s = x_t \f]
 * \f[  U(x_s, x_t) = +\beta \textrm{ if } x_s \neq x_t \f]
 * with
 * - \f$ x_s \f$ the label on site s
 * - \f$ y_s \f$ the value on the reference image
 * - \f$ \sigma^2_{x_s} \f$ the noise variance
   *
   * This class is meant to be used in the MRF framework with the otb::MarkovRandomFieldFilter
   *
 * \ingroup Markov
 *
 * \ingroup OTBMarkov
 */

template<class TInput1, class TInput2>
class ITK_EXPORT MRFEnergyPotts : public MRFEnergy<TInput1, TInput2>
{
public:
  typedef MRFEnergyPotts                Self;
  typedef MRFEnergy<TInput1, TInput2>   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TInput1                               InputImageType;
  typedef TInput2                               LabelledImageType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename LabelledImageType::PixelType LabelledImagePixelType;

  typedef itk::Array<double> ParametersType;

  itkTypeMacro(MRFEnergyPotts, MRFEnergy);

  itkNewMacro(Self);

  double GetSingleValue(const InputImagePixelType& value1, const LabelledImagePixelType& value2) ITK_OVERRIDE
  {
    if (value1 != value2)
      {
      return this->m_Parameters[0];
      }
    else
      {
      return -this->m_Parameters[0];
      }
  }

protected:
  // The constructor and destructor.
  MRFEnergyPotts()
    {
    this->m_NumberOfParameters = 1;
    this->m_Parameters.SetSize(this->m_NumberOfParameters);
    this->m_Parameters[0] = 1.0;
    };
  ~MRFEnergyPotts() ITK_OVERRIDE {}

};
}

#endif
