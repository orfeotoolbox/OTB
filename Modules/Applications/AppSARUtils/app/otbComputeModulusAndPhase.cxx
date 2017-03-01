/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) CS Systemes d'information. All rights reserved.
 See CSCopyright.txt for details.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "itkComplexToPhaseImageFilter.h"
#include "itkComplexToModulusImageFilter.h"
#include "itkComposeImageFilter.h"
#include <itkMacro.h>

namespace otb
{
//  Application class is defined in Wrapper namespace.
namespace Wrapper
{

//  ComputeModulusAndPhase class is derived from Application class.
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
  itkTypeMacro(ComputeModulusAndPhase, otb::Application);

  //typedefs for the application
  typedef itk::ComplexToModulusImageFilter<ComplexFloatImageType, FloatImageType>   ModulusFilterType;
  typedef itk::ComplexToPhaseImageFilter<ComplexFloatImageType, FloatImageType>   PhaseFilterType;
  typedef otb::ImageFileReader<ComplexFloatImageType> ComplexReaderType;
  typedef otb::ImageFileReader<FloatImageType> FloatReaderType;
  typedef itk::ComposeImageFilter<FloatImageType,ComplexFloatImageType> ComposeImageFilterType;

private:
  void DoInit()
  {
    SetName("ComputeModulusAndPhase");
    SetDescription("This application computes the modulus and the phase of a complex SAR image.");

    SetDocName("Compute Modulus And Phase");
    SetDocLongDescription(
            "This application computes the modulus and the phase of a "
            "complex SAR image. This complex SAR image can be provided as either: "
            "a monoband image with complex pixels, a 2-bands image with real and "
            "imaginary channels, or 2 monoband images (first one real part and "
            "second one imaginary part)"
    );
    SetDocLimitations("None");
    SetDocAuthors("Alexia Mondot (alexia.mondot@c-s.fr) and Mickael Savinaud (mickael.savinaud@c-s.fr)");
    SetDocSeeAlso("SARPolarMatrixConvert, SARPolarSynth");
    AddDocTag(Tags::SAR);

    // Input images
    // We will need to manually create our otb::ImageFileReader here because its
    // argument type depends on the number of inputs, so the "il" parameter is a
    // StringList, not a ImageList
    AddParameter(ParameterType_InputFilenameList, "il", "Input image list");
    SetParameterDescription("il", "Input image list (one complex monoband, one real dualband or two monoband images)");

    // Outputs
    AddParameter(ParameterType_OutputImage, "modulus", "Modulus");
    SetParameterDescription("modulus", "Modulus of the input: sqrt(real*real + imag*imag).");

    AddParameter(ParameterType_OutputImage, "phase", "Phase");
    SetParameterDescription("phase", "Phase of the input: atan2(imag, real).");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("il", "monobandComplexFloat.tif");
    SetDocExampleParameterValue("modulus", "modulus.tif");
    SetDocExampleParameterValue("phase", "phase.tif");
  }

  // DoUpdateParameters() is called as soon as a parameter value change.
  void DoUpdateParameters()
  {
  }

  // DoExecute() contains the application core.
  void DoExecute()
  {
    m_modulus = ModulusFilterType::New();
    m_phase = PhaseFilterType::New();

    std::vector<std::string> inList = GetParameterStringList("il");
    const size_t numberOfInputs = inList.size();

    if (numberOfInputs == 1)
    {
      // Get the input image
      m_complex_reader = ComplexReaderType::New();
      m_complex_reader->SetFileName(inList[0]);

      m_modulus->SetInput(m_complex_reader->GetOutput());
      m_phase->SetInput(m_complex_reader->GetOutput());
    }
    else if (numberOfInputs == 2)
    {
      // Get the input image
      m_float_reader0 = FloatReaderType::New();
      m_float_reader1 = FloatReaderType::New();

      m_float_reader0->SetFileName(inList[0]);
      m_float_reader1->SetFileName(inList[1]);

      // Combine the two images into one complex image
      m_compose = ComposeImageFilterType::New();
      m_compose->SetInput1(m_float_reader0->GetOutput());
      m_compose->SetInput2(m_float_reader1->GetOutput());

      m_modulus->SetInput(m_compose->GetOutput());
      m_phase->SetInput(m_compose->GetOutput());
    }
    else
    {
        throw std::runtime_error("Two many input images for ComputeModulusAndPhase.");
    }

    SetParameterOutputImage("modulus", m_modulus->GetOutput() );
    SetParameterOutputImage("phase", m_phase->GetOutput());
  }

  ComplexReaderType::Pointer m_complex_reader;
  FloatReaderType::Pointer m_float_reader0;
  FloatReaderType::Pointer m_float_reader1;

  ModulusFilterType::Pointer m_modulus;
  PhaseFilterType::Pointer m_phase;
  ComposeImageFilterType::Pointer m_compose;
};

} // namespace Wrapper
} // namespace otb
OTB_APPLICATION_EXPORT(otb::Wrapper::ComputeModulusAndPhase)
