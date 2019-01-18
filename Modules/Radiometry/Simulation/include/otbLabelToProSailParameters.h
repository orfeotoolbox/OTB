/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


#ifndef otbLabelToProSailParameters_h
#define otbLabelToProSailParameters_h

#include "otbLabelToSimulationParametersBase.h"

namespace otb
{

/**
 * \class LabelToProSailParameters
 * \brief Take a label corresponding to a spectrum type for vegetation and outputs the ProSail parameters needed to get a spectrum of that type.
 *
 * Output parameters concerns:
 *  - Step1: parameters for PROSPECT simulation.
 *  - Step2: parameters for SAIL simulation (see http://teledetection.ipgp.jussieu.fr/prosail/).
 *
 * WARNING: an only set of parameters is defined for all vegetation types.
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
 *
 * TODO : define different vegetation types and ProSail parameters associated.
 *
 * \sa LabelToSimulationParametersBase
 *
 * \ingroup OTBSimulation
 */

template <class TLabel>
class ITK_EXPORT LabelToProSailParameters : public LabelToSimulationParametersBase<TLabel>
{
public:

   /**Standard "Self" & Superclass typedef*/
   typedef LabelToProSailParameters                    Self;
   typedef LabelToSimulationParametersBase<TLabel>     Superclass;
   typedef itk::SmartPointer<Self>                     Pointer;
   typedef itk::SmartPointer<const Self>               ConstPointer;

   /** Some convenient typedefs. */
   typedef typename Superclass::ParametersType           ParametersType;
   typedef typename Superclass::LabelType                LabelType;

   /** Standard Macro*/
   itkTypeMacro(LabelToProSailParameters, LabelToSimulationParametersBase);
   itkNewMacro(Self);

  // virtual const ParametersType & GetStep1Parameters();
  //virtual const ParametersType & GetStep2Parameters();

   void GenerateData() override;

protected:
   LabelToProSailParameters();
   ~LabelToProSailParameters() override {}


private:
   LabelToProSailParameters(const Self &) = delete;
   void operator =(const Self&) = delete;


};

} // end namespace otb

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbLabelToProSailParameters.hxx"
#endif

#endif
