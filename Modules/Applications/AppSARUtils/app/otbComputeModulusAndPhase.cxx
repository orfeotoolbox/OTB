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
#include "otbMultiToMonoChannelExtractROI.h"

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
class ComputeModulusAndPhase: public Application
{
public:
  // Class declaration is followed by ITK public types for the class, the superclass and smart pointers.
  typedef ComputeModulusAndPhase Self;
  typedef Application Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro (Self);
  itkTypeMacro(ComputeModulusAndPhase, otb::Wrapper::Application);

  //typedefs for the application
  typedef otb::MultiToMonoChannelExtractROI<ComplexFloatVectorImageType::InternalPixelType, ComplexFloatImageType::PixelType> ExtractFilterType;
  typedef itk::ComplexToModulusImageFilter<ComplexFloatImageType, FloatImageType>   ModulusFilterType;
  typedef itk::ComplexToPhaseImageFilter<ComplexFloatImageType, FloatImageType>   PhaseFilterType;

private:
  void DoInit() override
  {
    SetName("ComputeModulusAndPhase");
    SetDescription("This application computes the modulus and the phase of a complex SAR image.");

    SetDocName("Compute Modulus And Phase");
    SetDocLongDescription(
      "This application computes the modulus and the phase of a "
      "complex SAR image. The input should be a single band image with "
      "complex pixels."
    );
    SetDocLimitations("The application takes as input single band image with complex pixels.");
    SetDocAuthors("Alexia Mondot (alexia.mondot@c-s.fr) and Mickael Savinaud (mickael.savinaud@c-s.fr)");
    SetDocSeeAlso("Despeckle, SARPolarMatrixConvert, SARPolarSynth");

    AddDocTag(Tags::SAR);
    AddDocTag(Tags::Manip);
    // Input images
    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input image (complex single band)");

    // Outputs
    AddParameter(ParameterType_OutputImage, "modulus", "Modulus");
    SetParameterDescription("modulus", "Modulus of the input image computes with the\n"
                            "following formula: :math:`\\sqrt{real*real + imag*imag}` where real and imag \n"
                            "are respectively the real and the imaginary part of the input complex image.\n");
    
    AddParameter(ParameterType_OutputImage, "phase", "Phase");
    SetParameterDescription("phase", "Phase of the input image computes with the following formula:\n"
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
    m_Modulus = ModulusFilterType::New();
    m_Phase = PhaseFilterType::New();

    ComplexFloatVectorImageType::Pointer inImage = GetParameterComplexFloatVectorImage("in");

    if (inImage->GetNumberOfComponentsPerPixel() != 1)
    {
        otbAppLogFATAL("Input must be a single band complex image.");
    }

    // Get first band
    m_Extract = ExtractFilterType::New();
    m_Extract->SetInput(inImage);

    // Compute modulus and phase
    m_Modulus->SetInput(m_Extract->GetOutput());
    m_Phase->SetInput(m_Extract->GetOutput());

    SetParameterOutputImage("modulus", m_Modulus->GetOutput() );
    SetParameterOutputImage("phase", m_Phase->GetOutput());
  }

  ExtractFilterType::Pointer m_Extract;
  ModulusFilterType::Pointer m_Modulus;
  PhaseFilterType::Pointer m_Phase;
};

} // namespace Wrapper
} // namespace otb
OTB_APPLICATION_EXPORT(otb::Wrapper::ComputeModulusAndPhase)
