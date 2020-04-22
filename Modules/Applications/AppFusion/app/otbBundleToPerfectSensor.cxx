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

#include "otbWrapperApplicationFactory.h"
#include "otbWrapperCompositeApplication.h"


namespace otb
{
namespace Wrapper
{
class BundleToPerfectSensor : public CompositeApplication
{
public:
  /** Standard class typedefs. */
  typedef BundleToPerfectSensor         Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(BundleToPerfectSensor, otb::Wrapper::CompositeApplication);

private:
  void DoInit() override
  {
    SetName("BundleToPerfectSensor");
    SetDescription("Perform P+XS pansharpening");

    // Documentation
    SetDocLongDescription(
        "This application performs P+XS pansharpening. The default mode use Pan and XS sensor models to estimate the transformation to superimpose XS over Pan "
        "before the fusion (\"default mode\"). The application provides also a PHR mode for Pleiades images which does not use sensor models as Pan and XS "
        "products are already coregistered but only estimate an affine transformation to superimpose XS over the Pan.Note that this option is automatically "
        "activated in case Pleiades images are detected as input.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Geometry);
    AddDocTag(Tags::Pansharpening);

    ClearApplications();
    AddApplication("Superimpose", "superimpose", "Reproject XS onto Pan");
    AddApplication("Pansharpening", "pansharp", "Fusion of XS and Pan");

    ShareParameter("inp", "superimpose.inr", "Input PAN Image", "Input panchromatic image.");
    ShareParameter("inxs", "superimpose.inm", "Input XS Image", "Input XS image.");
    ShareParameter("out", "pansharp.out");
    ShareParameter("elev", "superimpose.elev");
    ShareParameter("mode", "superimpose.mode");
    ShareParameter("method", "pansharp.method");
    ShareParameter("lms", "superimpose.lms", "Spacing of the deformation field",
                   "Spacing of the deformation field. Default is 10 times the PAN image spacing.");
    ShareParameter("interpolator", "superimpose.interpolator");
    ShareParameter("fv", "superimpose.fv");
    ShareParameter("ram", "superimpose.ram");

    Connect("pansharp.inp", "superimpose.inr");
    Connect("pansharp.ram", "superimpose.ram");

    // Doc example parameter settings
    SetDocExampleParameterValue("inp", "QB_Toulouse_Ortho_PAN.tif");
    SetDocExampleParameterValue("inxs", "QB_Toulouse_Ortho_XS.tif");
    SetDocExampleParameterValue("out", "BundleToPerfectSensor.png uchar");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    UpdateInternalParameters("superimpose");
  }

  void DoExecute() override
  {
    ExecuteInternal("superimpose");

    GetInternalApplication("pansharp")->SetParameterInputImage("inxs", GetInternalApplication("superimpose")->GetParameterOutputImage("out"));

    ExecuteInternal("pansharp");
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::BundleToPerfectSensor)
