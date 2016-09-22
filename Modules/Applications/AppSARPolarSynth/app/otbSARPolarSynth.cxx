/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbMultiChannelsPolarimetricSynthesisFilter.h"

namespace otb
{
namespace Wrapper
{

class SARPolarSynth : public Application
{
public:
  /** Standard class typedefs. */
  typedef SARPolarSynth                       Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  typedef MultiChannelsPolarimetricSynthesisFilter<ComplexDoubleVectorImageType, FloatImageType>        MCPSFilterType;
  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SARPolarSynth, otb::Application);

private:
  void DoInit() ITK_OVERRIDE
  {
    SetName("SARPolarSynth");
    SetDescription("Gives, for each pixel, the power that would have been received by a SAR system with a basis different from the classical (H,V) one (polarimetric synthetis).");

    // Documentation
    SetDocName("SARPolarSynth");
    SetDocLongDescription("This application gives, for each pixel, the power that would have been received by a SAR system with a basis different from the classical (H,V) one (polarimetric synthetis).\n" 
						  "The new basis A and B are indicated through two Jones vectors, defined by the user thanks to orientation (psi) and ellipticity (khi) parameters.\n"
						  "These parameters are namely psii, khii, psir and khir. The suffixes (i) and (r) refer to the transmitting antenna and the receiving antenna respectively.\n"
						  "Orientations and ellipticities are given in degrees, and are between -90/90 degrees and -45/45 degrees respectively.\n " 
						  "\n"
						  "Four polarization architectures can be processed : \n"
						  "1) HH_HV_VH_VV : full polarization, general bistatic case.\n"
						  "2) HH_HV_VV or HH_VH_VV : full polarization, monostatic case (transmitter and receiver are co-located).\n"
						  "3) HH_HV : dual polarization.\n"
						  "4) VH_VV : dual polarization.\n"
						  "The application takes a complex vector image as input, where each band correspond to a particular emission/reception polarization scheme.\n"
						  "User must comply with the band order given above, since the bands are used to build the Sinclair matrix.\n"
						  "\n"
						  "In order to determine the architecture, the application first relies on the number of bands of the input image.\n" 
						  "1) Architecture HH_HV_VH_VV is the only one with four bands, there is no possible confusion.\n"
						  "2) Concerning HH_HV_VV and HH_VH_VV architectures, both correspond to a three channels image. But they are processed in the same way, as the Sinclair matrix is symmetric in the monostatic case.\n"
						  "3) Finally, the two last architectures (dual polarizations), can't be distinguished only by the number of bands of the input image.\n"
						  "   User must then use the parameters emissionh and emissionv to indicate the architecture of the system : emissionh=1 and emissionv=0 --> HH_HV,  emissionh=0 and emissionv=1 --> VH_VV.\n"
						  "Note : if the architecture is HH_HV, khii and psii are automatically both set to 0 degree; if the architecture is VH_VV, khii and psii are automatically set to 0 degree and 90 degrees respectively.\n"
						  "\n"
						  "It is also possible to force the calculation to co-polar or cross-polar modes.\n"
						  "In the co-polar case, values for psir and khir will be ignored and forced to psii and khii; same as the cross-polar mode, where khir and psir will be forced to (psii + 90 degrees) and -khii.\n"
						  "\n"
						  "Finally, the result of the polarimetric synthetis is expressed in the power domain, through a one-band scalar image.\n"
						  "Note: this application doesn't take into account the terms which do not depend on the polarization of the antennas. \n"
						  "The parameter gain can be used for this purpose.\n"
						  "\n"
						  "More details can be found in the OTB CookBook (SAR processing chapter).");
						  
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("SARDecompositions, SARPolarMatrixConvert");

    AddDocTag(Tags::SAR);

    AddParameter(ParameterType_ComplexInputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input image.");
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image.");
    
    AddParameter(ParameterType_Float,"psii","psii");
    SetParameterDescription("psii","Orientation (transmitting antenna)");
    SetMinimumParameterFloatValue("psii",-90.0);
    SetMaximumParameterFloatValue("psii",90.0);
    
    AddParameter(ParameterType_Float,"khii","khii");
    SetParameterDescription("khii","Ellipticity (transmitting antenna)");
    SetMinimumParameterFloatValue("khii",-45.0);
    SetMaximumParameterFloatValue("khii",45.0);
    
    AddParameter(ParameterType_Float,"psir","psir");
    SetParameterDescription("psir","Orientation (receiving antenna)");
    SetMinimumParameterFloatValue("psir",-90.0);
    SetMaximumParameterFloatValue("psir",90.0);
    
    AddParameter(ParameterType_Float,"khir","khir");
    SetParameterDescription("khir","Ellipticity (receiving antenna)");
    SetMinimumParameterFloatValue("khir",-45.0);
    SetMaximumParameterFloatValue("khir",45.0);
    
    AddParameter(ParameterType_Int,"emissionh","Emission H");
    SetParameterDescription("emissionh","This parameter is useful in determining the polarization architecture (dual polarization case).");
    SetMinimumParameterIntValue("emissionh",0);
    SetMaximumParameterIntValue("emissionh",1);
    MandatoryOff("emissionh");
    
    AddParameter(ParameterType_Int,"emissionv","Emission V");
    SetParameterDescription("emissionv","This parameter is useful in determining the polarization architecture (dual polarization case).");
    SetMinimumParameterIntValue("emissionv",0);
    SetMaximumParameterIntValue("emissionv",1);
    MandatoryOff("emissionv");
    
    AddParameter(ParameterType_Choice, "mode", "Forced mode");
    AddChoice("mode.none","None");
    SetParameterDescription("mode.none","None");
    AddChoice("mode.co","Copolarization");
    SetParameterDescription("mode.none","Copolarization");
    AddChoice("mode.cross","Crosspolarization");
    SetParameterDescription("mode.cross","Crosspolarization");
    

    AddRAMParameter();

    // Default values
    SetDefaultParameterFloat("psii", 0.);
    SetDefaultParameterFloat("khii", 0.);
    SetDefaultParameterFloat("psir",  0.);
    SetDefaultParameterFloat("khir",  0.);
    SetDefaultParameterInt("emissionh", 0);
    SetDefaultParameterInt("emissionv", 0);
	SetDefaultParameterFloat("mode",  0);

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "sar.tif");
    SetDocExampleParameterValue("psii","15.");
    SetDocExampleParameterValue("khii", "5.");
    SetDocExampleParameterValue("psir","-25.");
    SetDocExampleParameterValue("khir", "10.");
    SetDocExampleParameterValue("out", "newbasis.tif");
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() ITK_OVERRIDE
  {
	
	m_MCPSFilter = MCPSFilterType::New();
	m_MCPSFilter->SetPsiI(GetParameterFloat("psii"));
    m_MCPSFilter->SetKhiI(GetParameterFloat("khii"));
    m_MCPSFilter->SetPsiR(GetParameterFloat("psir"));
    m_MCPSFilter->SetKhiR(GetParameterFloat("khir"));
    m_MCPSFilter->SetEmissionH(GetParameterInt("emissionh"));
    m_MCPSFilter->SetEmissionV(GetParameterInt("emissionv"));
	m_MCPSFilter->SetMode(GetParameterInt("mode"));  
	  
    ComplexDoubleVectorImageType* inVImage = GetParameterComplexDoubleVectorImage("in");
    inVImage->UpdateOutputInformation();
    int nbBands = inVImage->GetNumberOfComponentsPerPixel();
    otbAppLogINFO( << "nbBands = " << nbBands);

	m_MCPSFilter->SetInput(inVImage);
   
    SetParameterOutputImage("out", m_MCPSFilter->GetOutput());
  }
  //std::vector<itk::ProcessObject::Pointer> m_Ref;
  MCPSFilterType::Pointer m_MCPSFilter;
}; 

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::SARPolarSynth)
