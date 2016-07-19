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





//monostatic case
#include "otbSinclairReciprocalImageFilter.h"
#include "otbSinclairToReciprocalCoherencyMatrixFunctor.h"
#include "otbSinclairToReciprocalCovarianceMatrixFunctor.h"
#include "otbSinclairToReciprocalCircularCovarianceMatrixFunctor.h"

#include "otbReciprocalCoherencyToReciprocalMuellerImageFilter.h"
#include "otbReciprocalCovarianceToCoherencyDegreeImageFilter.h"
#include "otbReciprocalCovarianceToReciprocalCoherencyImageFilter.h"
#include "otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter.h"


//bistatic case
#include "otbSinclairImageFilter.h"
#include "otbSinclairToCoherencyMatrixFunctor.h"
#include "otbSinclairToCovarianceMatrixFunctor.h"
#include "otbSinclairToCircularCovarianceMatrixFunctor.h"
#include "otbSinclairToMuellerMatrixFunctor.h"

#include "otbMuellerToReciprocalCovarianceImageFilter.h"
#include "otbMuellerToPolarisationDegreeAndPowerImageFilter.h"






namespace otb
{
namespace Wrapper
{

class SARPolarMatrixConvert : public Application
{
public:
  /** Standard class typedefs. */
  typedef SARPolarMatrixConvert                   Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  
  //Monostatic case
    typedef otb::Functor::SinclairToReciprocalCoherencyMatrixFunctor<ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleVectorImageType::PixelType>								RCoherencyFunctorType;
                                    
    typedef otb::Functor::SinclairToReciprocalCovarianceMatrixFunctor<ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleVectorImageType::PixelType>								RCovarianceFunctorType;
                                    
    typedef otb::Functor::SinclairToReciprocalCircularCovarianceMatrixFunctor<ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleVectorImageType::PixelType>								RCircularCovarianceFunctorType;
                                                                  
                                    								
    typedef SinclairReciprocalImageFilter<ComplexDoubleImageType, 
											 ComplexDoubleImageType, 
											 ComplexDoubleImageType, 
											 ComplexDoubleVectorImageType, 
											 RCoherencyFunctorType > 									RCohSRFilterType;
											 
											 
    typedef SinclairReciprocalImageFilter<ComplexDoubleImageType, 
											 ComplexDoubleImageType, 
											 ComplexDoubleImageType, 
											 ComplexDoubleVectorImageType, 
											 RCovarianceFunctorType > 									RCovSRFilterType;
											 
	typedef SinclairReciprocalImageFilter<ComplexDoubleImageType, 
											 ComplexDoubleImageType, 
											 ComplexDoubleImageType, 
											 ComplexDoubleVectorImageType, 
											 RCircularCovarianceFunctorType > 							RCCSRFilterType;
											 
											 
    typedef otb::ReciprocalCoherencyToReciprocalMuellerImageFilter<ComplexDoubleVectorImageType, DoubleVectorImageType> RCRMFilterType;
	
	typedef otb::ReciprocalCovarianceToCoherencyDegreeImageFilter<ComplexDoubleVectorImageType, ComplexDoubleVectorImageType> RCCDFilterType;
	
	typedef otb::ReciprocalCovarianceToReciprocalCoherencyImageFilter<ComplexDoubleVectorImageType, ComplexDoubleVectorImageType> RCRCFilterType;
	
	typedef otb::ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter<ComplexDoubleVectorImageType, ComplexDoubleVectorImageType> RLCRCCFilterType;
											 
  
	//Bistatic case
	typedef otb::Functor::SinclairToCoherencyMatrixFunctor<ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleVectorImageType::PixelType>								CoherencyFunctorType;
                                    
                                    
	typedef otb::Functor::SinclairToCovarianceMatrixFunctor<ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleVectorImageType::PixelType>								CovarianceFunctorType;
                                    
                                   
	typedef otb::Functor::SinclairToCircularCovarianceMatrixFunctor<ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleVectorImageType::PixelType>								CircularCovarianceFunctorType;
                                    
                                    	
    typedef otb::Functor::SinclairToMuellerMatrixFunctor<ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    ComplexDoubleImageType::PixelType,
                                    DoubleVectorImageType::PixelType>									MuellerFunctorType;
                                    
    
    typedef SinclairImageFilter<ComplexDoubleImageType, 
											 ComplexDoubleImageType, 
											 ComplexDoubleImageType,
											 ComplexDoubleImageType, 
											 ComplexDoubleVectorImageType, 
											 CoherencyFunctorType > 									CohSRFilterType;
											 
											 
    typedef SinclairImageFilter<ComplexDoubleImageType, 
											 ComplexDoubleImageType,
											 ComplexDoubleImageType, 
											 ComplexDoubleImageType, 
											 ComplexDoubleVectorImageType, 
											 CovarianceFunctorType > 									CovSRFilterType;
											 
	typedef SinclairImageFilter<ComplexDoubleImageType, 
											 ComplexDoubleImageType, 
											 ComplexDoubleImageType,
											 ComplexDoubleImageType, 
											 ComplexDoubleVectorImageType, 
											 CircularCovarianceFunctorType > 							CCSRFilterType;
                                    
	typedef SinclairImageFilter<ComplexDoubleImageType, 
											 ComplexDoubleImageType, 
											 ComplexDoubleImageType,
											 ComplexDoubleImageType, 
											 DoubleVectorImageType, 
											 MuellerFunctorType > 										MSRFilterType;
											 
											 
	typedef otb::MuellerToReciprocalCovarianceImageFilter<DoubleVectorImageType, ComplexDoubleVectorImageType>  MRCFilterType;									 
	
	typedef otb::MuellerToPolarisationDegreeAndPowerImageFilter<DoubleVectorImageType, DoubleVectorImageType>   MPDPFilterType;
  
  

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SARPolarMatrixConvert, otb::Application);

private:
  void DoInit() ITK_OVERRIDE
  {
    SetName("SARPolarMatrixConvert");
    SetDescription("This applications allows converting classical polarimetric matrices to each other.");

    // Documentation
    SetDocName("SARPolarMatrixConvert");
    SetDocLongDescription(
    
    "This application allows converting classical polarimetric matrices to each other.\n"
    "For instance, it is possible to get the coherency matrix from the Sinclar one, or the Mueller matrix from the coherency one.\n"
    "The filters used in this application never handle matrices, but images where each band is related to their elements.\n"
    "As most of the time SAR polarimetry handles symmetric/hermitian matrices, only the relevant elements are stored, so that the images representing them have a minimal number of bands.\n"
    "For instance, the coherency matrix size is 3x3 in the monostatic case, and 4x4 in the bistatic case : it will thus be stored in a 6-band or a 10-band complex image (the diagonal and the upper elements of the matrix).\n"
    "\n"
    "The Sinclair matrix is a special case : it is always represented as 3 or 4 one-band complex images (for mono- or bistatic case).\n"
    "The available conversions are listed below:\n"
    
	"\n--- Monostatic case ---\n" 
    
    "1 msinclairtocoherency --> Sinclair matrix to coherency matrix (input : 3 x 1 complex channel (HH, HV or VH, VV) | output :  6 complex channels)\n"
    "2 msinclairtocovariance --> Sinclair matrix to covariance matrix (input : 3 x 1 complex channel (HH, HV or VH, VV) | output :  6 complex channels)\n"
    "3 msinclairtocircovariance --> Sinclair matrix to circular covariance matrix (input : 3 x 1 complex channel (HH, HV or VH, VV) | output :  6 complex channels)\n"
    "4 mcoherencytomueller --> Coherency matrix to Mueller matrix (input : 6 complex channels | 16 real channels)\n"
    "5 mcovariancetocoherencydegree --> Covariance matrix to coherency degree (input : 6 complex channels | 3 complex channels)\n"
    "6 mcovariancetocoherency --> Covariance matrix to coherency matrix (input : 6 complex channels | 6 complex channels)\n"
    "7 mlinearcovariancetocircularcovariance --> Covariance matrix to circular covariance matrix (input : 6 complex channels | output : 6 complex channels)\n"
    
    "\n--- Bistatic case ---\n"
    
    "8 bsinclairtocoherency --> Sinclair matrix to coherency matrix (input : 4 x 1 complex channel (HH, HV, VH, VV) | 10 complex channels)\n"
    "9 bsinclairtocovariance --> Sinclair matrix to covariance matrix (input : 4 x 1 complex channel (HH, HV, VH, VV) | output : 10 complex channels)\n"
    "10 bsinclairtocircovariance --> Sinclair matrix to circular covariance matrix (input : 4 x 1 complex channel (HH, HV, VH, VV) | output : 10 complex channels)\n"
    
    "\n--- Both cases ---\n"
    
    "11 sinclairtomueller --> Sinclair matrix to Mueller matrix (input : 4 x 1 complex channel (HH, HV, VH, VV) | output : 16 real channels)\n"
    "12 muellertomcovariance --> Mueller matrix to covariance matrix (input : 16 real channels | output : 6 complex channels)\n"
    "13 muellertopoldegandpower --> Mueller matrix to polarization degree and power (input : 16 real channels | output : 4 real channels)\n"

 );
						  
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("SARPolarSynth, SARDecompositions");

    AddDocTag(Tags::SAR);

    AddParameter(ParameterType_ComplexInputImage,  "inc",   "Input : multi-band complex image");
    SetParameterDescription("inc", "Input : multi-band complex image");
    MandatoryOff("inc");
    
    AddParameter(ParameterType_InputImage,  "inf",   "Input : multi-band real image");
    SetParameterDescription("inf", "Input : multi-band real image");
    MandatoryOff("inf");


    AddParameter(ParameterType_ComplexInputImage,  "inhh",   "Input : one-band complex image (HH)");
    SetParameterDescription("inhh", "Input : one-band complex image (HH)");
    MandatoryOff("inhh");
    
    AddParameter(ParameterType_ComplexInputImage,  "inhv",   "Input : one-band complex image (HV)");
    SetParameterDescription("inhv", "Input : one-band complex image (HV)");
    MandatoryOff("inhv");
    
    AddParameter(ParameterType_ComplexInputImage,  "invh",   "Input : one-band complex image (VH)");
    SetParameterDescription("invh", "Input : one-band complex image (VH)");
    MandatoryOff("invh");
    
    AddParameter(ParameterType_ComplexInputImage,  "invv",   "Input : one-band complex image (VV)");
    SetParameterDescription("invv", "Input : one-band complex image (VV)");
    MandatoryOff("invv");
    
    AddParameter(ParameterType_ComplexOutputImage, "outc",  "Output Complex Image");
    SetParameterDescription("outc", "Output Complex image.");
    MandatoryOff("outc");
    
    AddParameter(ParameterType_OutputImage, "outf",  "Output Real Image");
    SetParameterDescription("outf", "Output Real image.");
    MandatoryOff("outf");
    
    
    
    AddParameter(ParameterType_Choice, "conv", "Conversion");

    //Monostatic case 
    
    // #1
    // SinclairToReciprocalCoherency
    AddChoice("conv.msinclairtocoherency","1 Monostatic : Sinclair matrix to coherency matrix (complex output)");
    SetParameterDescription("conv.msinclairtocoherency","1 Monostatic :Sinclair matrix to coherency matrix (complex output)");
    
    // #2
    // SinclairToReciprocalCovariance
    AddChoice("conv.msinclairtocovariance","2 Monostatic : Sinclair matrix to covariance matrix (complex output)");
    SetParameterDescription("conv.msinclairtocovariance","2 Monostatic : Sinclair matrix to covariance matrix (complex output)");
    
    // #3 
    // SinclairToReciprocalCircularCovariance
    AddChoice("conv.msinclairtocircovariance","3 Monostatic : Sinclair matrix to circular covariance matrix (complex output)");
    SetParameterDescription("conv.msinclairtocircovariance","3 Monostatic : Sinclair matrix to circular covariance matrix (complex output)");
    
    // #4 
    // ReciprocalCoherencyToReciprocalMuellerImageFilter
    AddChoice("conv.mcoherencytomueller","4 Monostatic : Coherency matrix to Mueller matrix"); 
    SetParameterDescription("conv.mcoherencytomueller","4 Monostatic : Coherency matrix to Mueller matrix"); 
    
    // #5
    // ReciprocalCovarianceToCoherencyDegreeImageFilter 
    AddChoice("conv.mcovariancetocoherencydegree","5 Monostatic : Covariance matrix to coherency degree "); 
    SetParameterDescription("conv.mcovariancetocoherencydegree","5 Monostatic : Covariance matrix to coherency degree "); 
    
    // #6
    // ReciprocalCovarianceToReciprocalCoherencyImageFilter
    AddChoice("conv.mcovariancetocoherency","6 Monostatic : Covariance matrix to coherency matrix (complex output)"); 
    SetParameterDescription("conv.mcovariancetocoherency","6 Monostatic : Covariance matrix to coherency matrix (complex output)");  
    
    // #7
    // ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter
    AddChoice("conv.mlinearcovariancetocircularcovariance","7 Monostatic : Covariance matrix to circular covariance matrix (complex output)"); 
    SetParameterDescription("conv.mlinearcovariancetocircularcovariance","7 Monostatic : Covariance matrix to circular covariance matrix (complex output)");  
    
    // #8
    // MuellerToReciprocalCovarianceImageFilter 
    AddChoice("conv.muellertomcovariance","8 Bi/mono : Mueller matrix to monostatic covariance matrix");
    SetParameterDescription("conv.muellertomcovariance","8 Bi/mono : Mueller matrix to monostatic covariance matrix");
    
    //Bistatic case
    
    // #9
    // SinclairToCoherency
    AddChoice("conv.bsinclairtocoherency","9 Bistatic : Sinclair matrix to coherency matrix (complex output)");
    SetParameterDescription("conv.bsinclairtocoherency","9 Bistatic : Sinclair matrix to coherency matrix (complex output)");
    
    // #10
    // SinclairToCovariance  
    AddChoice("conv.bsinclairtocovariance","10 Bistatic : Sinclair matrix to covariance matrix (complex output)");
    SetParameterDescription("conv.bsinclairtocovariance","10 Bistatic : Sinclair matrix to covariance matrix (complex output)");
    
    // #11
    // SinclairToCircularCovariance 
    AddChoice("conv.bsinclairtocircovariance","11 Bistatic : Sinclair matrix to circular covariance matrix (complex output)");
    SetParameterDescription("conv.bsinclairtocircovariance","11 Bistatic : Sinclair matrix to circular covariance matrix (complex output)");
    
    //Both case
    
    // #12
    // SinclairToMueller
    AddChoice("conv.sinclairtomueller","12 Bi/mono : Sinclair matrix to Mueller matrix");
    SetParameterDescription("conv.sinclairtomueller","12 Bi/mono : Sinclair matrix to Mueller matrix");
    
    
    // #13
    // MuellerToPolarisationDegreeAndPowerImageFilter
    AddChoice("conv.muellertopoldegandpower","13 Bi/mono : Mueller matrix to polarisation degree and power");
    SetParameterDescription("conv.muellertopoldegandpower","13 Bi/mono : Mueller matrix to polarisation degree and power");

    AddRAMParameter();

    // Default values
    SetDefaultParameterInt("conv", 0); // SinclairToReciprocalCoherency

    // Doc example parameter settings
    SetDocExampleParameterValue("inhh", "HH.tif");
	SetDocExampleParameterValue("invh", "VH.tif");
	SetDocExampleParameterValue("invv", "VV.tif");
	SetDocExampleParameterValue("conv", "msinclairtocoherency"); 
    SetDocExampleParameterValue("outc", "mcoherency.tif");
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    
    int convType = GetParameterInt("conv");
    
    if ( (convType>=0) && (convType<=2)) //msinclairtocoherency msinclairtocovariance msinclairtocircovariance
	{
	    GetParameterByKey("inc")->SetActive(false);
	    GetParameterByKey("inf")->SetActive(false); 
	    GetParameterByKey("inhh")->SetActive(true);
	    GetParameterByKey("inhv")->SetActive(true); 
	    GetParameterByKey("invh")->SetActive(true);
	    GetParameterByKey("invv")->SetActive(true); 
	    GetParameterByKey("outc")->SetActive(true);
	    GetParameterByKey("outf")->SetActive(false);
	}
	else if ( (convType>=3) && (convType<=6)) // mcoherencytomueller mcovariancetocoherencydegree mcovariancetocoherency mlinearcovariancetocircularcovariance
	{  
	    GetParameterByKey("inc")->SetActive(true);
	    GetParameterByKey("inf")->SetActive(false); 
	    GetParameterByKey("inhh")->SetActive(false);
	    GetParameterByKey("inhv")->SetActive(false); 
	    GetParameterByKey("invh")->SetActive(false);
	    GetParameterByKey("invv")->SetActive(false); 
	    
	    if (convType == 3)
	    {
			GetParameterByKey("outc")->SetActive(false);
			GetParameterByKey("outf")->SetActive(true);
		}
		else
		{
			GetParameterByKey("outc")->SetActive(true);
			GetParameterByKey("outf")->SetActive(false);
		}
	}
	else if ( convType==7) // muellertomcovariance
	{
		GetParameterByKey("inc")->SetActive(false);
	    GetParameterByKey("inf")->SetActive(true); 
	    GetParameterByKey("inhh")->SetActive(false);
	    GetParameterByKey("inhv")->SetActive(false); 
	    GetParameterByKey("invh")->SetActive(false);
	    GetParameterByKey("invv")->SetActive(false); 
	    GetParameterByKey("outc")->SetActive(true);
		GetParameterByKey("outf")->SetActive(false);	
	}
	else if ( (convType>=8) && (convType<=11)) // bsinclairtocoherency bsinclairtocovariance bsinclairtocircovariance sinclairtomueller
	{
	    GetParameterByKey("inc")->SetActive(false);
	    GetParameterByKey("inf")->SetActive(false); 
	    GetParameterByKey("inhh")->SetActive(true);
	    GetParameterByKey("inhv")->SetActive(true); 
	    GetParameterByKey("invh")->SetActive(true);
	    GetParameterByKey("invv")->SetActive(true); 
	    	    
	    if (convType == 11)
	    {
			GetParameterByKey("outc")->SetActive(false);
			GetParameterByKey("outf")->SetActive(true);
		}
		else
		{
			GetParameterByKey("outc")->SetActive(true);
			GetParameterByKey("outf")->SetActive(false);
		}
	    
	}
	else if ( convType==12 )  // muellertopoldegandpower
	{
	    GetParameterByKey("inc")->SetActive(false);
	    GetParameterByKey("inf")->SetActive(true); 
	    GetParameterByKey("inhh")->SetActive(false);
	    GetParameterByKey("inhv")->SetActive(false); 
	    GetParameterByKey("invh")->SetActive(false);
	    GetParameterByKey("invv")->SetActive(false); 
	    GetParameterByKey("outc")->SetActive(false);
		GetParameterByKey("outf")->SetActive(true);	    
	}
	
    
    
  }

  void DoExecute() ITK_OVERRIDE
  {

    //****************************************
	//* Check inputs and outputs consistency *
	//****************************************
	
    bool inc = HasUserValue("inc");
    bool inf = HasUserValue("inf");
	bool inhh = HasUserValue("inhh");
	bool inhv = HasUserValue("inhv");
	bool invh = HasUserValue("invh");
	bool invv = HasUserValue("invv");
	bool outc = HasUserValue("outc"); 
	bool outf = HasUserValue("outf"); 
	
	int convType = GetParameterInt("conv");
	
	
	if ( (!outc) && (!outf) )
	        otbAppLogFATAL( << "No output image provided; please, set the parameter 'outc' or 'outf'.");

	
	if ( (convType>=0) && (convType<=2)) //msinclairtocoherency msinclairtocovariance msinclairtocircovariance
	{
		if ( (!inhv) && (!invh) )
	        otbAppLogFATAL( << "Parameter 'inhv' or 'invh' not set.");
	    if ( !inhh )
	        otbAppLogFATAL( << "Parameter 'inhh' not set.");
	    if ( !invv )
	        otbAppLogFATAL( << "Parameter 'invv' not set.");
	        
	}
	
	else if ( (convType>=3) && (convType<=6)) // mcoherencytomueller mcovariancetocoherencydegree mcovariancetocoherency mlinearcovariancetocircularcovariance
	{
		if ( !inc )
	        otbAppLogFATAL( << "Parameter 'inc' not set.");
	}
	else if ( (convType>=8) && (convType<=11)) // bsinclairtocoherency bsinclairtocovariance bsinclairtocircovariance sinclairtomueller
	{
	    if ( (!inhh) || (!inhv) || (!invh) || (!invv) )
	        otbAppLogFATAL( << "Please, ensure that HH, HV, VH and VV complex images have been provided (paramaters inhh, inhv, invh, invv).");
	}
	else if ( (convType==7) || (convType==12) ) // muellertomcovariance muellertopoldegandpower
	{
		if ( !inf )
	        otbAppLogFATAL( << "Parameter 'inf' not set.");
	}

		
	switch (GetParameterInt("conv"))
      {
		  
		//***************************************
		//*             MONOSTATIC              *
		//***************************************
		
		case 0: // SinclairToReciprocalCoherency
	  	m_RCohSRFilter = RCohSRFilterType::New();
	  	
	    if (inhv)
		  m_RCohSRFilter->SetInputHV_VH(GetParameterComplexDoubleImage("inhv"));
	    else if (invh)
		  m_RCohSRFilter->SetInputHV_VH(GetParameterComplexDoubleImage("invh"));
	  	
		m_RCohSRFilter->SetInputHH(GetParameterComplexDoubleImage("inhh"));
		m_RCohSRFilter->SetInputVV(GetParameterComplexDoubleImage("invv"));
		
		SetParameterComplexOutputImage("outc", m_RCohSRFilter->GetOutput() ); // input : 3 x 1 complex channel | output :  6 complex channels
		
		break;

		
		case 1: // SinclairToReciprocalCovariance	
	
		m_RCovSRFilter = RCovSRFilterType::New();
		
		if (inhv)
		  m_RCovSRFilter->SetInputHV_VH(GetParameterComplexDoubleImage("inhv"));
	    else if (invh)
		  m_RCovSRFilter->SetInputHV_VH(GetParameterComplexDoubleImage("invh"));
		
		m_RCovSRFilter->SetInputHH(GetParameterComplexDoubleImage("inhh"));
		m_RCovSRFilter->SetInputVV(GetParameterComplexDoubleImage("invv"));
		
		SetParameterComplexOutputImage("outc", m_RCovSRFilter->GetOutput() ); // input : 3 x 1 complex channel | output :  6 complex channels
		
		break;
		
		
		
		case 2: // SinclairToReciprocalCircularCovariance
		
		m_RCCSRFilter = RCCSRFilterType::New();
		
		if (inhv)
		  m_RCCSRFilter->SetInputHV_VH(GetParameterComplexDoubleImage("inhv"));
	    else if (invh)
		  m_RCCSRFilter->SetInputHV_VH(GetParameterComplexDoubleImage("invh"));
		
		m_RCCSRFilter->SetInputHH(GetParameterComplexDoubleImage("inhh"));
		m_RCCSRFilter->SetInputVV(GetParameterComplexDoubleImage("invv"));
		
		SetParameterComplexOutputImage("outc", m_RCCSRFilter->GetOutput() ); // input : 3 x 1 complex channel | output :  6 complex channels
		
		break;
		
		
	    case 3: // ReciprocalCoherencyToReciprocalMuellerImageFilter
		
		m_RCRMFilter = RCRMFilterType::New();
		m_RCRMFilter->SetInput(GetParameterComplexDoubleVectorImage("inc"));
		
		SetParameterOutputImage("outf", m_RCRMFilter->GetOutput() ); // input : 6 complex channels | 16 real channels
		
		break;
		
	
		
		case 4: // ReciprocalCovarianceToCoherencyDegreeImageFilter
		
		m_RCCDFilter = RCCDFilterType::New();
		m_RCCDFilter->SetInput(GetParameterComplexDoubleVectorImage("inc"));
		
		SetParameterComplexOutputImage("outc", m_RCCDFilter->GetOutput() ); // input : 6 complex channels | 3 complex channels
		
		break;


	    case 5: // ReciprocalCovarianceToReciprocalCoherencyImageFilter
		
		m_RCRCFilter = RCRCFilterType::New();
		m_RCRCFilter->SetInput(GetParameterComplexDoubleVectorImage("inc"));
		
		SetParameterComplexOutputImage("outc", m_RCRCFilter->GetOutput() ); // input : 6 complex channels | 6 complex channels
		
		break;
		
	    

	    case 6: // ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter
		
		m_RLCRCCFilter = RLCRCCFilterType::New();
		m_RLCRCCFilter->SetInput(GetParameterComplexDoubleVectorImage("inc"));
		
		SetParameterComplexOutputImage("outc", m_RLCRCCFilter->GetOutput() ); // input : 6 complex channels | output : 6 complex channels
		
		break;
		
		
		case 7: // MuellerToReciprocalCovarianceImageFilter
		
		m_MRCFilter = MRCFilterType::New();
		
		m_MRCFilter->SetInput(GetParameterDoubleVectorImage("inf"));
		
		SetParameterComplexOutputImage("outc", m_MRCFilter->GetOutput() ); // input : 16 real channels | output : 6 complex channels
		
		break;
		
		
	    //***************************************
		//*               BISTATIC              * 
		//***************************************
		
	    case 8: // SinclairToCoherency
			
	  	m_CohSRFilter = CohSRFilterType::New();
	  	
		m_CohSRFilter->SetInputHH(GetParameterComplexDoubleImage("inhh"));
		m_CohSRFilter->SetInputHV(GetParameterComplexDoubleImage("inhv"));
		m_CohSRFilter->SetInputVH(GetParameterComplexDoubleImage("invh"));
		m_CohSRFilter->SetInputVV(GetParameterComplexDoubleImage("invv"));
		
		SetParameterComplexOutputImage("outc", m_CohSRFilter->GetOutput() ); // input : 4 x 1 complex channel | 10 complex channels
		
		break;
		
			
    
		case 9: // SinclairToCovariance	
		
		m_CovSRFilter = CovSRFilterType::New();
		
		m_CovSRFilter->SetInputHH(GetParameterComplexDoubleImage("inhh"));
		m_CovSRFilter->SetInputHV(GetParameterComplexDoubleImage("inhv"));
		m_CovSRFilter->SetInputVH(GetParameterComplexDoubleImage("invh"));
		m_CovSRFilter->SetInputVV(GetParameterComplexDoubleImage("invv"));
		
		SetParameterComplexOutputImage("outc", m_CovSRFilter->GetOutput() ); // input : 4 x 1 complex channel | output : 10 complex channels
		
		break;
		
		
		case 10: // SinclairToCircularCovariance
		
		m_CCSRFilter = CCSRFilterType::New();
		
		m_CCSRFilter->SetInputHH(GetParameterComplexDoubleImage("inhh"));
		m_CCSRFilter->SetInputHV(GetParameterComplexDoubleImage("inhv"));
		m_CCSRFilter->SetInputVH(GetParameterComplexDoubleImage("invh"));
		m_CCSRFilter->SetInputVV(GetParameterComplexDoubleImage("invv"));
		
		SetParameterComplexOutputImage("outc", m_CCSRFilter->GetOutput() ); // input : 4 x 1 complex channel | output : 10 complex channels
		
		break;
		
		
		//***************************************
		//*             BOTH CASES              * 
		//***************************************
		
				
		case 11: // SinclairToMueller
		m_MSRFilter = MSRFilterType::New();
		
		m_MSRFilter->SetInputHH(GetParameterComplexDoubleImage("inhh"));
		m_MSRFilter->SetInputHV(GetParameterComplexDoubleImage("inhv"));
		m_MSRFilter->SetInputVH(GetParameterComplexDoubleImage("invh"));
		m_MSRFilter->SetInputVV(GetParameterComplexDoubleImage("invv"));
		
		SetParameterOutputImage("outf", m_MSRFilter->GetOutput() ); // input : 4 x 1 complex channel | output : 16 real channels
		
		break;
		
		
		case 12: // MuellerToPolarisationDegreeAndPowerImageFilter
		m_MPDPFilter = MPDPFilterType::New();
		
		m_MPDPFilter->SetInput(GetParameterDoubleVectorImage("inf"));
		
		SetParameterOutputImage("outf", m_MPDPFilter->GetOutput() ); //  input : 16 real channels | output : 4 real channels
		
		break;
		
	  }
	  

    
  }

  //Monostatic
  RCohSRFilterType::Pointer m_RCohSRFilter;
  RCovSRFilterType::Pointer m_RCovSRFilter;
  RCCSRFilterType::Pointer  m_RCCSRFilter;
  RCRMFilterType::Pointer   m_RCRMFilter;
  RCCDFilterType::Pointer   m_RCCDFilter;
  RCRCFilterType::Pointer   m_RCRCFilter;
  RLCRCCFilterType::Pointer m_RLCRCCFilter;
  
  //Bistatic
  CohSRFilterType::Pointer m_CohSRFilter;
  CovSRFilterType::Pointer m_CovSRFilter;
  CCSRFilterType::Pointer  m_CCSRFilter;
  MSRFilterType::Pointer   m_MSRFilter;
  
  //Both cases
  MRCFilterType::Pointer   m_MRCFilter;
  MPDPFilterType::Pointer  m_MPDPFilter;
  



  
}; 

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::SARPolarMatrixConvert)
