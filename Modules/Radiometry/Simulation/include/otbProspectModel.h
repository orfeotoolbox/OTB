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

#ifndef otbProspectModel_h
#define otbProspectModel_h

#include "OTBSimulationExport.h"
#include "otbLeafParameters.h"

#include "otbDataSpecP5B.h"
#include "otbSimulationStep1Base.h"

namespace otb
{
/** \class ProspectModel
   * \brief This class implements the Prospect Model (see http://teledetection.ipgp.jussieu.fr/prosail/),
   *
   * It computes leaf reflectance and transmittance from a set of parameters (contained in a LeafParameters class)
   *
   * \sa itk::DataObject
   * \sa SimulationStep1Base
 *
 * \ingroup OTBSimulation
 */

class OTBSimulation_EXPORT ProspectModel : public SimulationStep1Base
{
public:
  /** Standard class typedefs */
  typedef ProspectModel                 Self;
  typedef SimulationStep1Base           Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef otb::LeafParameters              LeafParametersType;
  typedef Superclass::SpectralResponseType SpectralResponseType;
  typedef Superclass::ParametersType       ParametersType;

  typedef itk::ProcessObject::DataObjectPointerArraySizeType DataObjectPointerArraySizeType;

  /** Standard macros */
  itkNewMacro(Self);
  itkTypeMacro(ProspectModel, SpectrumGeneratorBase);

  /** Set/Get Input */
  using Superclass::SetInput;
  virtual void SetInput(const ParametersType&);
  void SetInput(const LeafParametersType* object);
  LeafParametersType* GetInput();

  /** GenerateData */
  void GenerateData() override;

  /** Get Output reflectance/transmittance*/
  SpectralResponseType* GetReflectance() override;
  SpectralResponseType* GetTransmittance() override;

protected:
  /** Constructor */
  ProspectModel();
  /** Destructor */
  ~ProspectModel() override;
  /** PrintSelf method */
  void PrintSelf(std::ostream& os, itk::Indent indent) const override;

  DataObjectPointer MakeOutput(DataObjectPointerArraySizeType) override;
  using Superclass::MakeOutput;

  /** Compute Transmission of isotropic radiation across an interface between two dielectrics*/
  double Tav(const int theta, double ref);

private:
  ProspectModel(const Self&) = delete;
  void operator=(const Self&) = delete;
};

} // end namespace otb

#endif
