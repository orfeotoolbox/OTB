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

class SARConvert : public Application
{
public:
  /** Standard class typedefs. */
  typedef SARConvert                   Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;

  
  //Monostatic case
    typedef otb::Functor::SinclairToReciprocalCoherencyMatrixFunctor<ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatVectorImageType::PixelType>								RCoherencyFunctorType;
                                    
    typedef otb::Functor::SinclairToReciprocalCovarianceMatrixFunctor<ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatVectorImageType::PixelType>								RCovarianceFunctorType;
                                    
    typedef otb::Functor::SinclairToReciprocalCircularCovarianceMatrixFunctor<ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatVectorImageType::PixelType>								RCircularCovarianceFunctorType;
                                                                  
                                    								
    typedef SinclairReciprocalImageFilter<ComplexFloatImageType, 
											 ComplexFloatImageType, 
											 ComplexFloatImageType, 
											 ComplexFloatVectorImageType, 
											 RCoherencyFunctorType > 									RCohSRFilterType;
											 
											 
    typedef SinclairReciprocalImageFilter<ComplexFloatImageType, 
											 ComplexFloatImageType, 
											 ComplexFloatImageType, 
											 ComplexFloatVectorImageType, 
											 RCovarianceFunctorType > 									RCovSRFilterType;
											 
	typedef SinclairReciprocalImageFilter<ComplexFloatImageType, 
											 ComplexFloatImageType, 
											 ComplexFloatImageType, 
											 ComplexFloatVectorImageType, 
											 RCircularCovarianceFunctorType > 							RCCSRFilterType;
											 
											 
    typedef otb::ReciprocalCoherencyToReciprocalMuellerImageFilter<ComplexFloatVectorImageType, FloatVectorImageType> RCRMFilterType;
	
	typedef otb::ReciprocalCovarianceToCoherencyDegreeImageFilter<ComplexFloatVectorImageType, ComplexFloatVectorImageType> RCCDFilterType;
	
	typedef otb::ReciprocalCovarianceToReciprocalCoherencyImageFilter<ComplexFloatVectorImageType, ComplexFloatVectorImageType> RCRCFilterType;
	
	typedef otb::ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter<ComplexFloatVectorImageType, ComplexFloatVectorImageType> RLCRCCFilterType;
											 
  
	//Bistatic case
	typedef otb::Functor::SinclairToCoherencyMatrixFunctor<ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatVectorImageType::PixelType>								CoherencyFunctorType;
                                    
                                    
	typedef otb::Functor::SinclairToCovarianceMatrixFunctor<ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatVectorImageType::PixelType>								CovarianceFunctorType;
                                    
                                   
	typedef otb::Functor::SinclairToCircularCovarianceMatrixFunctor<ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatVectorImageType::PixelType>								CircularCovarianceFunctorType;
                                    
                                    	
    typedef otb::Functor::SinclairToMuellerMatrixFunctor<ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    ComplexFloatImageType::PixelType,
                                    FloatVectorImageType::PixelType>									MuellerFunctorType;
                                    
    
    typedef SinclairImageFilter<ComplexFloatImageType, 
											 ComplexFloatImageType, 
											 ComplexFloatImageType,
											 ComplexFloatImageType, 
											 ComplexFloatVectorImageType, 
											 CoherencyFunctorType > 									CohSRFilterType;
											 
											 
    typedef SinclairImageFilter<ComplexFloatImageType, 
											 ComplexFloatImageType,
											 ComplexFloatImageType, 
											 ComplexFloatImageType, 
											 ComplexFloatVectorImageType, 
											 CovarianceFunctorType > 									CovSRFilterType;
											 
	typedef SinclairImageFilter<ComplexFloatImageType, 
											 ComplexFloatImageType, 
											 ComplexFloatImageType,
											 ComplexFloatImageType, 
											 ComplexFloatVectorImageType, 
											 CircularCovarianceFunctorType > 							CCSRFilterType;
                                    
	typedef SinclairImageFilter<ComplexFloatImageType, 
											 ComplexFloatImageType, 
											 ComplexFloatImageType,
											 ComplexFloatImageType, 
											 FloatVectorImageType, 
											 MuellerFunctorType > 										MSRFilterType;
											 
											 
	typedef otb::MuellerToReciprocalCovarianceImageFilter<FloatVectorImageType, ComplexFloatVectorImageType>  MRCFilterType;									 
	
	typedef otb::MuellerToPolarisationDegreeAndPowerImageFilter<FloatVectorImageType, FloatVectorImageType>   MPDPFilterType;
  
  

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SARConvert, otb::Application);

private:
  void DoInit()
  {
    SetName("SARConvert");
    SetDescription("This applications allows converting classical polarimetric matrices to each other.");

    // Documentation
    SetDocName("SARConvert");
    SetDocLongDescription(
    
    "This application allows converting classical polarimetric matrices to each other.\n"
    "For instance, it is possible to get the coherency matrix from the Sinclar one, or the Mueller matrix from the coherency one.\n"
    "The filters used in this application never handle matrices, but images where each band is related to their elements.\n"
    "As most of the time SAR polarimetry handles symetric/hermitian matrices, only the relevant elements are stored, so that the images representing them have a minimal number of bands.\n"
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
    SetDocSeeAlso("");

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
    
    
    
    AddParameter(ParameterType_Choice, "conv", "Convertion");

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
    
    //Bistatic case
    
    // #8
    // SinclairToCoherency
    AddChoice("conv.bsinclairtocoherency","8 Bistatic : Sinclair matrix to coherency matrix (complex output)");
    SetParameterDescription("conv.bsinclairtocoherency","8 Bistatic : Sinclair matrix to coherency matrix (complex output)");
    
    // #9
    // SinclairToCovariance  
    AddChoice("conv.bsinclairtocovariance","9 Bistatic : Sinclair matrix to covariance matrix (complex output)");
    SetParameterDescription("conv.bsinclairtocovariance","9 Bistatic : Sinclair matrix to covariance matrix (complex output)");
    
    // #10
    // SinclairToCircularCovariance 
    AddChoice("conv.bsinclairtocircovariance","10 Bistatic : Sinclair matrix to circular covariance matrix (complex output)");
    SetParameterDescription("conv.bsinclairtocircovariance","10 Bistatic : Sinclair matrix to circular covariance matrix (complex output)");
    
    //Both case
    
    // #11
    // SinclairToMueller
    AddChoice("conv.sinclairtomueller","11 Bi/mono : Sinclair matrix to Mueller matrix");
    SetParameterDescription("conv.sinclairtomueller","11 Bi/mono : Sinclair matrix to Mueller matrix");
    
    // #12
    // MuellerToReciprocalCovarianceImageFilter 
    AddChoice("conv.muellertomcovariance","12 Bi/mono : Mueller matrix to monostatic covariance matrix");
    SetParameterDescription("conv.muellertomcovariance","12 Bi/mono : Mueller matrix to monostatic covariance matrix");
    
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

  void DoUpdateParameters()
  {
    
    int convType = GetParameterInt("conv");
    
    if ( (convType>=0) && (convType<=2)) //Monostatic case : multi one-band inputs
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
	else if ( (convType>=3) && (convType<=6)) //Monostatic case : one multi-band input
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
	else if ( (convType>=7) && (convType<=10)) //Bistatic case : multi one-band inputs
	{
	    GetParameterByKey("inc")->SetActive(false);
	    GetParameterByKey("inf")->SetActive(false); 
	    GetParameterByKey("inhh")->SetActive(true);
	    GetParameterByKey("inhv")->SetActive(true); 
	    GetParameterByKey("invh")->SetActive(true);
	    GetParameterByKey("invv")->SetActive(true); 
	    	    
	    if (convType == 10)
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
	else if ( (convType>=11) && (convType<=12)) //Bistatic case : one multi-band input
	{
	    GetParameterByKey("inc")->SetActive(false);
	    GetParameterByKey("inf")->SetActive(true); 
	    GetParameterByKey("inhh")->SetActive(false);
	    GetParameterByKey("inhv")->SetActive(false); 
	    GetParameterByKey("invh")->SetActive(false);
	    GetParameterByKey("invv")->SetActive(false); 
	    	    
	    if (convType == 12)
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
	
    
    
  }

  void DoExecute()
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

	
	if ( (convType>=0) && (convType<=2)) //Monostatic case : multi one-band inputs
	{
		if ( (!inhv) && (!invh) )
	        otbAppLogFATAL( << "Parameter 'inhv' or 'invh' not set.");
	    if ( !inhh )
	        otbAppLogFATAL( << "Parameter 'inhh' not set.");
	    if ( !invv )
	        otbAppLogFATAL( << "Parameter 'invv' not set.");
	        
	}
	
	else if ( (convType>=3) && (convType<=6)) //Monostatic case : one multi-band input
	{
		if ( !inc )
	        otbAppLogFATAL( << "Parameter 'inc' not set.");
	}
	else if ( (convType>=7) && (convType<=10)) //Bistatic case : multi one-band inputs
	{
	    if ( (!inhh) || (!inhv) || (!invh) || (!invv) )
	        otbAppLogFATAL( << "Please, ensure that HH, HV, VH and VV complex images have been provided (paramaters inhh, inhv, invh, invv).");
	}
	else if ( (convType>=11) && (convType<=12)) //Bistatic case : one multi-band input
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
		  m_RCohSRFilter->SetInputHV_VH(GetParameterComplexFloatImage("inhv"));
	    else if (invh)
		  m_RCohSRFilter->SetInputHV_VH(GetParameterComplexFloatImage("invh"));
	  	
		m_RCohSRFilter->SetInputHH(GetParameterComplexFloatImage("inhh"));
		m_RCohSRFilter->SetInputVV(GetParameterComplexFloatImage("invv"));
		
		SetParameterComplexOutputImage("outc", m_RCohSRFilter->GetOutput() ); // input : 3 x 1 complex channel | output :  6 complex channels
		
		break;

		
		case 1: // SinclairToReciprocalCovariance	
	
		m_RCovSRFilter = RCovSRFilterType::New();
		
		if (inhv)
		  m_RCohSRFilter->SetInputHV_VH(GetParameterComplexFloatImage("inhv"));
	    else if (invh)
		  m_RCohSRFilter->SetInputHV_VH(GetParameterComplexFloatImage("invh"));
		
		m_RCovSRFilter->SetInputHH(GetParameterComplexFloatImage("inhh"));
		m_RCovSRFilter->SetInputVV(GetParameterComplexFloatImage("invv"));
		
		SetParameterComplexOutputImage("outc", m_RCovSRFilter->GetOutput() ); // input : 3 x 1 complex channel | output :  6 complex channels
		
		break;
		
		
		
		case 2: // SinclairToReciprocalCircularCovariance
		
		m_RCCSRFilter = RCCSRFilterType::New();
		
		if (inhv)
		  m_RCohSRFilter->SetInputHV_VH(GetParameterComplexFloatImage("inhv"));
	    else if (invh)
		  m_RCohSRFilter->SetInputHV_VH(GetParameterComplexFloatImage("invh"));
		
		m_RCCSRFilter->SetInputHH(GetParameterComplexFloatImage("inhh"));
		m_RCCSRFilter->SetInputVV(GetParameterComplexFloatImage("invv"));
		
		SetParameterComplexOutputImage("outc", m_RCCSRFilter->GetOutput() ); // input : 3 x 1 complex channel | output :  6 complex channels
		
		break;
		
		
	    case 3: // ReciprocalCoherencyToReciprocalMuellerImageFilter
		
		m_RCRMFilter = RCRMFilterType::New();
		m_RCRMFilter->SetInput(GetParameterComplexFloatVectorImage("inc"));
		
		SetParameterOutputImage("outf", m_RCRMFilter->GetOutput() ); // input : 6 complex channels | 10 real channels
		
		break;
		
	
		
		case 4: // ReciprocalCovarianceToCoherencyDegreeImageFilter
		
		m_RCCDFilter = RCCDFilterType::New();
		m_RCCDFilter->SetInput(GetParameterComplexFloatVectorImage("inc"));
		
		SetParameterComplexOutputImage("outc", m_RCCDFilter->GetOutput() ); // input : 6 complex channels | 3 complex channels
		
		break;


	    case 5: // ReciprocalCovarianceToReciprocalCoherencyImageFilter
		
		m_RCRCFilter = RCRCFilterType::New();
		m_RCRCFilter->SetInput(GetParameterComplexFloatVectorImage("inc"));
		
		SetParameterComplexOutputImage("outc", m_RCRCFilter->GetOutput() ); // input : 6 complex channels | 6 complex channels
		
		break;
		
	    

	    case 6: // ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter
		
		m_RLCRCCFilter = RLCRCCFilterType::New();
		m_RLCRCCFilter->SetInput(GetParameterComplexFloatVectorImage("inc"));
		
		SetParameterComplexOutputImage("outc", m_RLCRCCFilter->GetOutput() ); // input : 6 complex channels | output : 6 complex channels
		
		break;
		
		
	    //***************************************
		//*               BISTATIC              * 
		//***************************************
		
	    case 7: // SinclairToCoherency
			
	  	m_CohSRFilter = CohSRFilterType::New();
	  	
		m_CohSRFilter->SetInputHH(GetParameterComplexFloatImage("inhh"));
		m_CohSRFilter->SetInputHV(GetParameterComplexFloatImage("inhv"));
		m_CohSRFilter->SetInputVH(GetParameterComplexFloatImage("invh"));
		m_CohSRFilter->SetInputVV(GetParameterComplexFloatImage("invv"));
		
		SetParameterComplexOutputImage("outc", m_CohSRFilter->GetOutput() ); // input : 4 x 1 complex channel | 10 complex channels
		
		break;
		
			
    
		case 8: // SinclairToCovariance	
		
		m_CovSRFilter = CovSRFilterType::New();
		
		m_CovSRFilter->SetInputHH(GetParameterComplexFloatImage("inhh"));
		m_CovSRFilter->SetInputHV(GetParameterComplexFloatImage("inhv"));
		m_CovSRFilter->SetInputVH(GetParameterComplexFloatImage("invh"));
		m_CovSRFilter->SetInputVV(GetParameterComplexFloatImage("invv"));
		
		SetParameterComplexOutputImage("outc", m_CovSRFilter->GetOutput() ); // input : 4 x 1 complex channel | output : 10 complex channels
		
		break;
		
		
		case 9: // SinclairToCircularCovariance
		
		m_CCSRFilter = CCSRFilterType::New();
		
		m_CCSRFilter->SetInputHH(GetParameterComplexFloatImage("inhh"));
		m_CCSRFilter->SetInputHV(GetParameterComplexFloatImage("inhv"));
		m_CCSRFilter->SetInputVH(GetParameterComplexFloatImage("invh"));
		m_CCSRFilter->SetInputVV(GetParameterComplexFloatImage("invv"));
		
		SetParameterComplexOutputImage("outc", m_CCSRFilter->GetOutput() ); // input : 4 x 1 complex channel | output : 10 complex channels
		
		break;
		
				
		case 10: // SinclairToMueller
		
		m_MSRFilter = MSRFilterType::New();
		
		m_MSRFilter->SetInputHH(GetParameterComplexFloatImage("inhh"));
		m_MSRFilter->SetInputHV(GetParameterComplexFloatImage("inhv"));
		m_MSRFilter->SetInputVH(GetParameterComplexFloatImage("invh"));
		m_MSRFilter->SetInputVV(GetParameterComplexFloatImage("invv"));
		
		SetParameterOutputImage("outf", m_MSRFilter->GetOutput() ); // input : 4 x 1 complex channel | output : 16 real channels
		
		break;
		
		
		case 11: // MuellerToReciprocalCovarianceImageFilter
		
		m_MRCFilter = MRCFilterType::New();
		
		m_MRCFilter->SetInput(GetParameterFloatVectorImage("inf"));
		
		SetParameterComplexOutputImage("outc", m_MRCFilter->GetOutput() ); // input : 16 real channels | output : 6 complex channels
		
		break;
		
		
		case 12: // MuellerToPolarisationDegreeAndPowerImageFilter
		m_MPDPFilter = MPDPFilterType::New();
		
		m_MPDPFilter->SetInput(GetParameterFloatVectorImage("inf"));
		
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
  MRCFilterType::Pointer   m_MRCFilter;
  MPDPFilterType::Pointer  m_MPDPFilter;
  



  
}; 

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::SARConvert)
