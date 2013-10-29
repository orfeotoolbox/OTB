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

#include "otbBandMathImageFilter.h"
#include "itkComplexToPhaseImageFilter.h"
#include "itkComplexToModulusImageFilter.h"
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
  typedef otb::BandMathImageFilter<FloatImageType>       BandMathType;
  typedef itk::ComplexToModulusImageFilter<ComplexFloatImageType, FloatImageType>   ModulusFilterType; 
  typedef itk::ComplexToPhaseImageFilter<ComplexFloatImageType, FloatImageType>   PhaseFilterType;    
            

private:
  void DoInit()
  {
    SetName("ComputeModulusAndPhase");
    SetDescription("This application computes the modulus and the phase of a complex SAR data.");

    SetDocName("Compute Modulus And Phase");
    SetDocLongDescription("This application computes the modulus and the phase of a complex SAR data. This complex SAR data could be provided as a monoband complex pixel type image or a 2 bands real pixel type image or two monobands (first one real part and second one imaginary part) real pixel type images.");
    SetDocLimitations("None");
    SetDocAuthors("Alexia Mondot (alexia.mondot@c-s.fr) and Mickael Savinaud (mickael.savinaud@c-s.fr)");
    SetDocSeeAlso(" ");
    AddDocTag(Tags::SAR);


    // Choice of number of entries
    AddParameter(ParameterType_Choice, "nbinput", "Number Of inputs");
    SetParameterDescription(
        "nbinput",
        "Choice about the number of input files used to store the real and imaginary part of the SAR image");

    AddChoice("nbinput.one", "One input");
    SetParameterDescription("nbinput.one", "One input: one band (complex pixel type) SAR image or two bands (real complex type) SAR image.");

    AddChoice("nbinput.two", "Two inputs");
    SetParameterDescription("nbinput.two", "Two inputs: the first one is considered as real part and the second one as the imaginary part of the SAR image.");

    // Inputs
    // Real part of a complex image
    AddParameter(ParameterType_InputImage, "nbinput.two.re", "Real part input");
    SetParameterDescription("nbinput.two.re", "Image file with real part of the SAR data.");
    // Imaginary part of a complex image
    AddParameter(ParameterType_InputImage, "nbinput.two.im", "Imaginary part input");
    SetParameterDescription("nbinput.two.im", "Image file with imaginary part of the SAR data.");
    // Complex image
    AddParameter(ParameterType_ComplexInputImage, "nbinput.one.in", "Input image");
    SetParameterDescription("nbinput.one.in", "Image file with SAR data.");

    // Outputs
    AddParameter(ParameterType_OutputImage, "mod", "Modulus");
    SetParameterDescription("mod", "Modulus of the input: sqrt(real*real + imag*imag).");
    
    AddParameter(ParameterType_OutputImage, "pha", "Phase");
    SetParameterDescription("pha", "Phase of the input: atan2(imag, real).");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("nbinput.one.in", "monobandComplexFloat.tif");
    SetDocExampleParameterValue("mod", "modulus.tif");
    SetDocExampleParameterValue("pha", "phase.tif");
  }

  // DoUpdateParameters() is called as soon as a parameter value change.
  void DoUpdateParameters()
  {
    // If one entry is choosen, disabled the re and im part
    // else disable complex
    const std::string numberOfInputs = GetParameterString("nbinput");

    if (numberOfInputs == "one")
      {
      MandatoryOn("nbinput.one.in");

      MandatoryOff("nbinput.two.re");
      DisableParameter("nbinput.two.re");
      
      MandatoryOff("nbinput.two.im");
      DisableParameter("nbinput.two.im");
      
      EnableParameter("nbinput.one.in");
      }
    else
      {
      MandatoryOff("nbinput.one.in");
      DisableParameter("nbinput.one.in");
      
      MandatoryOn("nbinput.two.re");
      MandatoryOn("nbinput.two.im");

      EnableParameter("nbinput.two.re");
      EnableParameter("nbinput.two.im");
      }
  }

  // DoExecute() contains the application core.
  void DoExecute()
  {
 
    m_modulus2 = BandMathType::New();
    m_phase2 = BandMathType::New();

    m_modulus1 = ModulusFilterType::New();
    m_phase1 = PhaseFilterType::New(); 

    const std::string numberOfInputs = GetParameterString("nbinput");

    if (numberOfInputs == "one")
      {
      // Get the input image
	    ComplexFloatImageType::Pointer inImage = this->GetParameterComplexFloatImage("nbinput.one.in");

      m_modulus1->SetInput(inImage);
      m_phase1->SetInput(inImage); 
      
      SetParameterOutputImage("mod", m_modulus1->GetOutput() );
      SetParameterOutputImage("pha", m_phase1->GetOutput());
     
      }
    else if (numberOfInputs == "two")
      {

      // Get the input image re
      FloatImageType::Pointer inImageRe = this->GetParameterFloatImage("nbinput.two.re");
   
      // Get the input image im
      FloatImageType::Pointer inImageIm = this->GetParameterFloatImage("nbinput.two.im");

      m_modulus2->SetNthInput(0, inImageRe,"real");
      m_modulus2->SetNthInput(1, inImageIm,"imag");
      m_modulus2->SetExpression("sqrt(real * real + imag * imag)");

      m_phase2->SetNthInput(0, inImageRe,"real");
      m_phase2->SetNthInput(1, inImageIm,"imag");
      m_phase2->SetExpression("atan2( imag , real )");

      SetParameterOutputImage("mod", m_modulus2->GetOutput() );
      SetParameterOutputImage("pha", m_phase2->GetOutput());
      }
  }
  
BandMathType::Pointer m_modulus2;
BandMathType::Pointer m_phase2;
ModulusFilterType::Pointer m_modulus1;
PhaseFilterType::Pointer m_phase1;
}

;} // namespace Wrapper
} // namespace otb
OTB_APPLICATION_EXPORT(otb::Wrapper::ComputeModulusAndPhase)
