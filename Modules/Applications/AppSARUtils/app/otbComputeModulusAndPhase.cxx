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

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "itkComplexToPhaseImageFilter.h"
#include "itkComplexToModulusImageFilter.h"

#include "itkMacro.h"

namespace otb
{
//  Application class is defined in Wrapper namespace.
namespace Wrapper
{

/** \class ComputeModulusAndPhase
 *  \brief ComputeModulusAndPhase is an application that
 *         computes modulus and phase from a complex SAR image.
 *
 * \ingroup AppSARUtils
 */
class ComputeModulusAndPhase : public Application
{
public:
  // Class declaration is followed by ITK public types for the class, the superclass and smart pointers.
  typedef ComputeModulusAndPhase        Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);
  itkTypeMacro(ComputeModulusAndPhase, otb::Wrapper::Application);

  // typedefs for the application
  typedef itk::ComplexToModulusImageFilter<ComplexFloatImageType, FloatImageType> ModulusFilterType;
  typedef itk::ComplexToPhaseImageFilter<ComplexFloatImageType, FloatImageType>   PhaseFilterType;

private:
  void DoInit() override
  {
    SetName("ComputeModulusAndPhase");
    SetDescription("This application computes the modulus and the phase of a complex SAR image or an image with 2 components (real and imaginary parts).");

    SetDocLongDescription(
        "This application computes the modulus and the phase of a "
        "complex SAR image. The input should be a single band image with "
        "complex pixels or a 2 bands image (real and imaginary components in separate bands).");
    SetDocLimitations("The application takes as input single band image with complex pixels or a 2 bands image (real and imaginary part in separate bands).");
    SetDocAuthors("Alexia Mondot (alexia.mondot@c-s.fr) and Mickael Savinaud (mickael.savinaud@c-s.fr)");
    SetDocSeeAlso("Despeckle, SARPolarMatrixConvert, SARPolarSynth");

    AddDocTag(Tags::SAR);
    AddDocTag(Tags::Manip);
    // Input images
    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "Input image (complex single band or 2 bands (real/imaginary parts))");

    // Outputs
    AddParameter(ParameterType_OutputImage, "modulus", "Modulus");
    SetParameterDescription("modulus",
                            "Modulus of the input image computes with the\n"
                            "following formula: :math:`\\sqrt{real*real + imag*imag}` where real and imag \n"
                            "are respectively the real and the imaginary part of the input complex image.\n");

    AddParameter(ParameterType_OutputImage, "phase", "Phase");
    SetParameterDescription("phase",
                            "Phase of the input image computes with the following formula:\n"
                            ":math:`\\tan^{-1}(\\frac{imag}{real})` where real and imag are respectively the real and\n"
                            "the imaginary part of the input complex image.");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "monobandComplexFloat.tif");
    SetDocExampleParameterValue("modulus", "modulus.tif");
    SetDocExampleParameterValue("phase", "phase.tif");

    SetOfficialDocLink();
  }

  // DoUpdateParameters() is called as soon as a parameter value change.
  void DoUpdateParameters() override
  {
  }

  // DoExecute() contains the application core.
  void DoExecute() override
  {
    // Read the input as a complex image (with one component) Input with 2
    // components are also supported (band 1 is interpreted as the real part and
    // band 2 as the imaginary part VectorImage of complex image are not
    // supported by the application
    ComplexFloatImageType::Pointer inImage = GetParameterComplexFloatImage("in");

    ModulusFilterType::Pointer modulus = ModulusFilterType::New();
    PhaseFilterType::Pointer   phase   = PhaseFilterType::New();

    // Compute modulus and phase
    modulus->SetInput(inImage);
    phase->SetInput(inImage);

    SetParameterOutputImage("modulus", modulus->GetOutput());
    SetParameterOutputImage("phase", phase->GetOutput());

    RegisterPipeline();
  }
};

} // namespace Wrapper
} // namespace otb
OTB_APPLICATION_EXPORT(otb::Wrapper::ComputeModulusAndPhase)
