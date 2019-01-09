/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

class EndmemberNumberEstimation : public Application
{
public:
  /** Standard class typedefs. */
  typedef EndmemberNumberEstimation         Self;
  typedef Application                       Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(EndmemberNumberEstimation, otb::Application);

private:
  void DoInit() override
  {
    SetName("EndmemberNumberEstimation");
    SetDescription(" ");

    // Documentation
    SetDocName("Endmember Number Estimation");
    SetDocLongDescription(" ");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("VertexComponentAnalysis, HyperspectralUnmixing");

    AddDocTag(Tags::Hyperspectral);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image Filename");
    SetParameterDescription("in","The hyperspectral data cube input");

    AddParameter(ParameterType_Choice, "algo", "Unmixing algorithm");
    SetParameterDescription("algo", "The algorithm to use for the estimation");
    AddChoice("algo.elm", "elm");
    SetParameterDescription("algo.elm", "Eigenvalue Likelihood Maximization");
    AddChoice("algo.vd", "vd");
    SetParameterDescription("algo.vd", "virtual dimensionality");

    AddParameter(ParameterType_String,"number","Number of endmembers");
    SetParameterDescription("number", "Estimated number of endmembers");
    SetParameterRole("number", Role_Output);

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "cupriteSubHsi.tif");
    SetDocExampleParameterValue("algo", "elm");

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

OTB_APPLICATION_EXPORT(otb::Wrapper::EndmemberNumberEstimation)
