/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  This code is from a Jan Wegner contribution.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbMRFEnergyFisherClassification_h
#define __otbMRFEnergyFisherClassification_h

#include "otbMRFEnergy.h"
#include "otbMath.h"
#include <boost/math/special_functions/gamma.hpp>

namespace otb
{

/**
 * \class MRFEnergyFisherClassification
 * \brief Fisher model for Markov classification
 *
 * This is the implementation of the Fisher model for Markov classification
 * to be used for the fidelity (likelihood) term for the classification.
 *
 * This class is meant to be used in the MRF framework with the
 * otb::MarkovRandomFieldFilter
 *
 *
 * \ingroup OTBMarkov
 */
template< class TInput1, class TInput2>
class ITK_EXPORT MRFEnergyFisherClassification: public MRFEnergy< TInput1, TInput2>
{
public:
  typedef MRFEnergyFisherClassification   Self;
  typedef MRFEnergy< TInput1, TInput2>    Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  typedef TInput1                               InputImageType;
  typedef TInput2                               LabelledImageType;
  typedef typename InputImageType::PixelType    InputImagePixelType;
  typedef typename LabelledImageType::PixelType LabelledImagePixelType;
  typedef itk::Array < double >                 ParametersType;

  itkNewMacro(Self);
  itkTypeMacro(MRFEnergyFisherClassification, MRFEnergy);

  void SetNumberOfParameters(const unsigned int nParameters)
  {
    Superclass::SetNumberOfParameters(nParameters);
    this->m_Parameters.SetSize(nParameters);
    this->Modified();
  }

  double GetSingleValue(const InputImagePixelType & value1,  const LabelledImagePixelType & value2)
  {
    if ((unsigned int)value2 >= this->GetNumberOfParameters()/3)
      {
      itkExceptionMacro(<<"Number of parameters does not correspond to number of classes" );
      }
    double val1 = static_cast<double>(value1);
    double mu = this->m_Parameters[3*value2];
    double l  = this->m_Parameters[3*value2+1];
    double m  = this->m_Parameters[3*value2+2];

    double result = -vcl_log((boost::math::tgamma(l+m)/(boost::math::tgamma(l)*boost::math::tgamma(m)))
                             * (2/(mu)) * (vcl_sqrt(l/m)) *
                             ((vcl_pow((vcl_sqrt(l/m)*(val1/mu)), ((2*l)-1))) /
                              (vcl_pow(1+(vcl_sqrt(l/m)*(val1/mu)*vcl_sqrt(l/m)*(val1/mu)), (l+m)))));

    return result;
  }

protected:
  // The constructor and destructor.
  MRFEnergyFisherClassification() {};
  virtual ~MRFEnergyFisherClassification() {};
};
}
#endif
