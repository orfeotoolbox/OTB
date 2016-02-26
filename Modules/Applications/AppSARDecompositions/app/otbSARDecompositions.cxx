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
#include "otbPerBandVectorImageFilter.h"
#include "itkMeanImageFilter.h"


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

  
  
  typedef otb::Functor::SinclairToReciprocalCoherencyMatrixFunctor<ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleVectorImageType::PixelType>								FunctorType;
                                    
                                    								
  typedef SinclairReciprocalImageFilter<ComplexDoubleImageType, 
											 ComplexDoubleImageType, 
											 ComplexDoubleImageType, 
											 ComplexDoubleVectorImageType, 
											 FunctorType > 												SRFilterType;
  
  
  typedef otb::ReciprocalHAlphaImageFilter<ComplexDoubleVectorImageType, DoubleVectorImageType> 			HAFilterType;
  
  
  typedef itk::MeanImageFilter<ComplexDoubleImageType, ComplexDoubleImageType>         MeanFilterType;
  typedef otb::PerBandVectorImageFilter<ComplexDoubleVectorImageType, ComplexDoubleVectorImageType, MeanFilterType> PerBandMeanFilterType;
  //FloatImageType

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SARDecompositions, otb::Application);

private:
  void DoInit()
  {
    SetName("SARDecompositions");
    SetDescription("From one-band complex images (each one related to an element of the Sinclair matrix), returns the selected decomposition.");

    // Documentation
    SetDocName("SARDecompositions");
    SetDocLongDescription("From one-band complex images (HH, HV, VH, VV), returns the selected decomposition.\n \n"
						  "The H-alpha-A decomposition is currently the only one available; it is implemented for the monostatic case (transmitter and receiver are co-located).\n"
						  "User must provide three one-band complex images HH, HV or VH, and VV (monostatic case <=> HV = VH).\n"
						  "The H-alpha-A decomposition consists in averaging 3x3 complex coherency matrices (incoherent analysis); the user must provide the size of the averaging window, thanks to the parameter inco.kernelsize.\n "
						  "The applications returns a float vector image, made up of three channels : H (entropy), Alpha, A (Anisotropy)." );
						  
						  
						  
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("SARPolarMatrixConvert, SARPolarSynth");

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
    
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image");
    
    AddParameter(ParameterType_Choice, "decomp", "Decompositions");
    AddChoice("decomp.haa","H-alpha-A decomposition");
    SetParameterDescription("decomp.haa","H-alpha-A decomposition");
    
    AddParameter(ParameterType_Group,"inco","Incoherent decompositions");
    SetParameterDescription("inco","This group allows setting parameters related to the incoherent decompositions.");
    
    AddParameter(ParameterType_Int, "inco.kernelsize",   "Kernel size for spatial incoherent averaging.");
    SetParameterDescription("inco.kernelsize", "Minute (0-59)");
    SetMinimumParameterIntValue("inco.kernelsize", 1);
    SetDefaultParameterInt("inco.kernelsize", 3);
    MandatoryOff("inco.kernelsize");

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
		m_MeanFilter = PerBandMeanFilterType::New();
		
		if (inhv)
		  m_SRFilter->SetInputHV_VH(GetParameterComplexDoubleImage("inhv"));
	    else if (invh)
		  m_SRFilter->SetInputHV_VH(GetParameterComplexDoubleImage("invh"));

		m_SRFilter->SetInputHH(GetParameterComplexDoubleImage("inhh"));
		m_SRFilter->SetInputVV(GetParameterComplexDoubleImage("invv"));
		
		MeanFilterType::InputSizeType radius;
        radius.Fill( GetParameterInt("inco.kernelsize") );
        m_MeanFilter->GetFilter()->SetRadius(radius);
		
		
		m_MeanFilter->SetInput(m_SRFilter->GetOutput());
		m_HAFilter->SetInput(m_MeanFilter->GetOutput());
		SetParameterOutputImage("out", m_HAFilter->GetOutput() );
    
		break;
	  }
   	 
  }

  //MCPSFilterType::Pointer m_MCPSFilter;
  SRFilterType::Pointer m_SRFilter;
  HAFilterType::Pointer m_HAFilter;
  PerBandMeanFilterType::Pointer m_MeanFilter;
  
}; 

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::SARDecompositions)
