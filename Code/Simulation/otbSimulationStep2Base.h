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

#ifndef __otbSimulationStep2Base_h
#define __otbSimulationStep2Base_h

#include "itkProcessObject.h"
#include "otbSpectralResponse.h"

namespace otb
{

/**
 * \class SimulationStep2Base
 * \brief Base class for all methods that generate spectrum of an object using its reflectance and transmittance
 * \brief  (ex: Sail http://teledetection.ipgp.jussieu.fr/prosail/).
 *
 * \sa ProcessObject
 */


class ITK_EXPORT SimulationStep2Base : public itk::ProcessObject
{
public:

  /**Standard "Self" & Superclass typedef*/
  typedef SimulationStep2Base Self;
  typedef itk::ProcessObject Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Some convenient typedefs. */
  typedef SpectralResponse<double, double> SpectralResponseType;
  typedef double ParametersValueType;
  typedef itk::Array<ParametersValueType> ParametersType;

  /** Standard Macro*/
  itkTypeMacro(SimulationStep2Base, ProcessObject)
;

  itkSetMacro(Parameters, ParametersType)
  itkGetMacro(Parameters, ParametersType)
 //;

  virtual void SetReflectance(const SpectralResponseType *) = 0;
  virtual void SetTransmittance(const SpectralResponseType *) = 0;

protected:
  SimulationStep2Base()
  {
  }
;
  virtual ~SimulationStep2Base()
  {
  }

private:
  SimulationStep2Base(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

  ParametersType m_Parameters;

};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#endif

#endif
