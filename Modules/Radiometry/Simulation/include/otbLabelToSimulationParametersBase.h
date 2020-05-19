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


#ifndef otbLabelToSimulationParametersBase_h
#define otbLabelToSimulationParametersBase_h


#include "itkProcessObject.h"
#include "itkArray.h"

namespace otb
{

/**
 * \class LabelToSimulationParametersBase
 * \brief Base class for all classes that take a label and outputs two set of parameters for two steps (step1, step2)
 *
 * step1 and 2 refers to PROSPECT and SAIL parameters.
 *
 * Prospect Parameters
 * Cab    = 30;   % chlorophyll content (ug.cm-2)
 *  Car   = 10;   % carotenoid content (ug.cm-2)
 * Cbrown  = 0.0;  % brown pigment content (arbitrary units)
 * Cw    = 0.015;  % EWT (cm)
 * Cm    = 0.009;  % LMA (g.cm-2)
 * N   = 1.2;  % structure coefficient
 *
 * Sail Parameters
 * lai   = 2;    % leaf area index
 * angl  = 50;   % average leaf angle (deg)
 * psoil = 1;    % soil coefficient
 * skyl  = 70;   % % diffuse/direct radiation
 * hspot = 0.2;  % hot spot
 * ihot  = 1.0;  %
 * tts   = 30;   % solar zenith angle (deg)
 * tto   = 0;    % observer zenith angle (deg)
 * psi   = 0;    % azimuth (deg)
 *
 * \sa ProcessObject
 *
 * \ingroup OTBSimulation
 */

template <class TLabel>
class ITK_EXPORT LabelToSimulationParametersBase : public itk::ProcessObject
{
public:
  /**Standard "Self" & Superclass typedef*/
  typedef LabelToSimulationParametersBase Self;
  typedef itk::ProcessObject              Superclass;
  typedef itk::SmartPointer<Self>         Pointer;
  typedef itk::SmartPointer<const Self>   ConstPointer;

  /** Some convenient typedefs. */
  typedef TLabel                          LabelType;
  typedef double                          ParametersValueType;
  typedef itk::Array<ParametersValueType> ParametersType;


  /** Standard Macro*/
  itkTypeMacro(LabelToSimulationParametersBase, ProcessObject);

  itkSetMacro(Label, LabelType);
  itkGetMacro(Label, LabelType);

  itkSetMacro(Step1Parameters, ParametersType);
  itkGetMacro(Step1Parameters, ParametersType);

  itkSetMacro(Step2Parameters, ParametersType);
  itkGetMacro(Step2Parameters, ParametersType);

  // virtual const ParametersType & GetStep1Parameters() = 0;
  // virtual const ParametersType & GetStep2Parameters() = 0;

protected:
  LabelToSimulationParametersBase(){};
  ~LabelToSimulationParametersBase() override
  {
  }


private:
  LabelToSimulationParametersBase(const Self&) = delete;
  void operator=(const Self&) = delete;

  ParametersType m_Step1Parameters;
  ParametersType m_Step2Parameters;
  LabelType      m_Label;
};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#endif

#endif
