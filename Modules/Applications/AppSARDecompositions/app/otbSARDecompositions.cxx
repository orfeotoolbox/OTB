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


#include "otbReciprocalHAlphaImageFilter.h"
#include "otbSinclairReciprocalImageFilter.h"
#include "otbSinclairToReciprocalCoherencyMatrixFunctor.h"


namespace otb
{
namespace Wrapper
{

class SARDecompositions : public Application
{
public:
  /** Standard class typedefs. */
  typedef SARDecompositions                   Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  
  
  typedef otb::Functor::SinclairToReciprocalCoherencyMatrixFunctor<ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatVectorImageType::PixelType>								FunctorType;
                                    
                                    								
  typedef SinclairReciprocalImageFilter<ComplexFloatImageType, 
											 ComplexFloatImageType, 
											 ComplexFloatImageType, 
											 ComplexFloatVectorImageType, 
											 FunctorType > 												SRFilterType;
  
  
  typedef otb::ReciprocalHAlphaImageFilter<ComplexFloatVectorImageType, FloatVectorImageType> 			HAFilterType;
  
  
  

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SARDecompositions, otb::Application);

private:
  void DoInit()
  {
    SetName("SARDecompositions");
    SetDescription("From one-band complex images (HH, HV or VH, VV), returns the selected decomposition.");

    // Documentation
    SetDocName("SARDecompositions");
    SetDocLongDescription("From one-band complex images (HH, HV, VH, VV), returns the selected decomposition.\n"
						  "The H-alpha-A decomposition is currently the only one available; it is implemented for the monostatic case (transmitter and receiver are co-located).\n"
						  "User must provide three one-band complex images HH, HV or VH, and VV (monostatic case <=> HV = VH).\n"
						  "The applications returns a float vector image, made of three channels : H (entropy), Alpha, A (Anisotropy)." );
						  
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("");

    AddDocTag(Tags::SAR);

    AddParameter(ParameterType_ComplexInputImage,  "inhh",   "Input Image");
    SetParameterDescription("inhh", "Input image (HH)");
    
    AddParameter(ParameterType_ComplexInputImage,  "inhv",   "Input Image");
    SetParameterDescription("inhv", "Input image (HV)");
    MandatoryOff("inhv");
    
    AddParameter(ParameterType_ComplexInputImage,  "invh",   "Input Image");
    SetParameterDescription("invh", "Input image (VH)");
    MandatoryOff("invh");
    
    AddParameter(ParameterType_ComplexInputImage,  "invv",   "Input Image");
    SetParameterDescription("invv", "Input image (VV)");
    
    AddParameter(ParameterType_ComplexOutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image");
    
    AddParameter(ParameterType_Choice, "decomp", "Decompositions");
    AddChoice("decomp.haa","H-alpha-A decomposition");
    SetParameterDescription("decomp.haa","H-alpha-A decomposition");

    AddRAMParameter();

    // Default values
    SetDefaultParameterInt("decomp", 0); // H-alpha-A

    // Doc example parameter settings
    SetDocExampleParameterValue("inhh", "HH.tif");
	SetDocExampleParameterValue("invh", "VH.tif");
	SetDocExampleParameterValue("invv", "VV.tif");
	SetDocExampleParameterValue("decomp", "haa");
    SetDocExampleParameterValue("out", "HaA.tif");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
	  
	bool inhv = HasUserValue("inhv");
	bool invh = HasUserValue("invh");
		
	if ( (!inhv) && (!invh) )
	  otbAppLogFATAL( << "Parameter inhv or invh not set. Please provide a HV or a VH complex image.");
    
    switch (GetParameterInt("decomp"))
      {
		case 0: // H-alpha-A
		
		m_SRFilter = SRFilterType::New();
		m_HAFilter = HAFilterType::New();
		
		if (inhv)
		  m_SRFilter->SetInputHV_VH(GetParameterComplexFloatImage("inhv"));
	    else if (invh)
		  m_SRFilter->SetInputHV_VH(GetParameterComplexFloatImage("invh"));

		m_SRFilter->SetInputHH(GetParameterComplexFloatImage("inhh"));
		m_SRFilter->SetInputVV(GetParameterComplexFloatImage("invv"));
		
		m_HAFilter->SetInput(m_SRFilter->GetOutput());

		SetParameterOutputImage("out", m_HAFilter->GetOutput() );
    
		break;
	  }
   	 
  }

  //MCPSFilterType::Pointer m_MCPSFilter;
  SRFilterType::Pointer m_SRFilter;
  HAFilterType::Pointer m_HAFilter;

  
}; 

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::SARDecompositions)
