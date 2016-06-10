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

#ifndef __otbMRFEnergyGaussian_h
#define __otbMRFEnergyGaussian_h

#include "otbMRFEnergy.h"

#include "otbMath.h"

namespace otb
{
/**
 * \class MRFEnergyGaussian
 * \brief This is the implementation of the Gaussian model for Markov classification.
 *
 * This is the implementation of the Gaussian model for Markov classification,
 * to be used for data fidelity. Energy is:
 *
 * \f[  U(x_s, x_t) = (x_s-x_t)^2 \f]
 *
 * with
 * - \f$ x_s \f$ the label on site s
 * - \f$ y_s \f$ the value on the reference image
   *
   * This class is meant to be used in the MRF framework with the otb::MarkovRandomFieldFilter
   *
 * \ingroup Markov
 *
 * \ingroup OTBMarkov
 */

template<class TInput1, class TInput2>
class ITK_EXPORT MRFEnergyGaussian : public MRFEnergy<TInput1, TInput2>
{
public:
  typedef MRFEnergyGaussian             Self;
  typedef MRFEnergy<TInput1, TInput2>   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TInput1                               InputImageType;
  typedef TInput2                               LabelledImageType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename LabelledImageType::PixelType LabelledImagePixelType;

  typedef itk::Array<double> ParametersType;

  itkTypeMacro(MRFEnergyGaussian, MRFEnergy);

  itkNewMacro(Self);

  double GetSingleValue(const InputImagePixelType& value1, const LabelledImagePixelType& value2) ITK_OVERRIDE
  {
    return vnl_math_sqr((static_cast<double>(value1))
                        - (static_cast<double>(value2)));
  }

protected:
  // The constructor and destructor.
  MRFEnergyGaussian()
    {
    this->m_NumberOfParameters = 0;
    this->m_Parameters.SetSize(this->m_NumberOfParameters);
    };
  ~MRFEnergyGaussian() ITK_OVERRIDE {}

};
}

#endif
