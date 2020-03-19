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

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

namespace otb
{
namespace Wrapper
{

class PantexTextureExtraction : public Application
{
public:
  /** Standard class typedefs. */
  typedef PantexTextureExtraction     Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(PantexTextureExtraction, otb::Application);

private:
  void DoInit() override
  {
    SetName("PantexTextureExtraction");
    SetDescription("Computes Pantex textural features on the selected channel of the input image");

    // Documentation
    SetDocLongDescription(
        " TODO"
    );

    SetDocLimitations(
        " TODO ");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(
        " TODO");

    AddDocTag(Tags::FeatureExtraction);
    AddDocTag("Textures");
    
    AddRAMParameter();

    SetOfficialDocLink();
  }
  
  
  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {
    
  }
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::PantexTextureExtraction)
