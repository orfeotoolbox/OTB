/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef otbSimulationStep1Base_h
#define otbSimulationStep1Base_h


#include "itkProcessObject.h"
#include "otbSpectralResponse.h"


namespace otb
{

/**
 * \class SimulationStep1Base
 * \brief Base class for all method that simulate the Reflectance and Transmittance of an object. (ex: Prospect)
 *
 * \sa ProcessObject
 *
 * \ingroup OTBSimulation
 */


class ITK_EXPORT SimulationStep1Base : public itk::ProcessObject
{
public:
  /**Standard "Self" & Superclass typedef*/
  typedef SimulationStep1Base           Self;
  typedef itk::ProcessObject            Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Some convenient typedefs. */
  typedef double                          ParametersValueType;
  typedef itk::Array<ParametersValueType> ParametersType;
  typedef otb::SpectralResponse<double, double> SpectralResponseType;


  /** Standard Macro*/
  itkTypeMacro(SimulationStep1Base, ProcessObject);

  itkSetMacro(Parameters, ParametersType);
  itkGetMacro(Parameters, ParametersType);
  // virtual  void SetInput(const ParametersType &) = 0;

  virtual SpectralResponseType* GetReflectance()   = 0;
  virtual SpectralResponseType* GetTransmittance() = 0;


protected:
  SimulationStep1Base(){};
  ~SimulationStep1Base() override
  {
  }


private:
  SimulationStep1Base(const Self&) = delete;
  void operator=(const Self&) = delete;

  ParametersType m_Parameters;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#endif

#endif
