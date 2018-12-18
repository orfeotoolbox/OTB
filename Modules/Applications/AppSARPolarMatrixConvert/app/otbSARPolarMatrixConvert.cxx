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

#include "otbSinclairToCoherencyMatrixImageFilter.h"
#include "otbSinclairToCovarianceMatrixImageFilter.h"
#include "otbSinclairToCircularCovarianceMatrixImageFilter.h"
#include "otbSinclairToMuellerMatrixImageFilter.h"

#include "otbSinclairToReciprocalCoherencyMatrixImageFilter.h"
#include "otbSinclairToReciprocalCovarianceMatrixImageFilter.h"
#include "otbSinclairToReciprocalCircularCovarianceMatrixImageFilter.h"

#include "otbMuellerToReciprocalCovarianceImageFilter.h"
#include "otbMuellerToPolarisationDegreeAndPowerImageFilter.h"

#include "otbReciprocalCoherencyToReciprocalMuellerImageFilter.h"
#include "otbReciprocalCovarianceToCoherencyDegreeImageFilter.h"
#include "otbReciprocalCovarianceToReciprocalCoherencyImageFilter.h"
#include "otbReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter.h"

namespace otb
{
namespace Wrapper
{

class SARPolarMatrixConvert : public Application
{
public:
  /** Standard class typedefs. */
  typedef SARPolarMatrixConvert         Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  // Monostatic case
  using RCohSRFilterType = SinclairToReciprocalCoherencyMatrixImageFilter<ComplexDoubleImageType, ComplexDoubleVectorImageType>;
  using RCovSRFilterType = SinclairToReciprocalCovarianceMatrixImageFilter<ComplexDoubleImageType, ComplexDoubleVectorImageType>;
  using RCCSRFilterType  = SinclairToReciprocalCircularCovarianceMatrixImageFilter<ComplexDoubleImageType, ComplexDoubleVectorImageType>;

  using RCRMFilterType = otb::ReciprocalCoherencyToReciprocalMuellerImageFilter<ComplexDoubleVectorImageType, DoubleVectorImageType>;
  using RCCDFilterType = otb::ReciprocalCovarianceToCoherencyDegreeImageFilter<ComplexDoubleVectorImageType, ComplexDoubleVectorImageType>;
  using RCRCFilterType = otb::ReciprocalCovarianceToReciprocalCoherencyImageFilter<ComplexDoubleVectorImageType, ComplexDoubleVectorImageType>;

  using RLCRCCFilterType = otb::ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter<ComplexDoubleVectorImageType, ComplexDoubleVectorImageType>;

  // Bistatic case
  using CohSRFilterType = SinclairToCoherencyMatrixImageFilter<ComplexDoubleImageType, ComplexDoubleVectorImageType>;
  using CovSRFilterType = SinclairToCovarianceMatrixImageFilter<ComplexDoubleImageType, ComplexDoubleVectorImageType>;
  using CCSRFilterType  = SinclairToCircularCovarianceMatrixImageFilter<ComplexDoubleImageType, ComplexDoubleVectorImageType>;
  using MSRFilterType   = SinclairToMuellerMatrixImageFilter<ComplexDoubleImageType, DoubleVectorImageType>;

  using MRCFilterType = otb::MuellerToReciprocalCovarianceImageFilter<DoubleVectorImageType, ComplexDoubleVectorImageType>;
  using MPDPFilterType = otb::MuellerToPolarisationDegreeAndPowerImageFilter<DoubleVectorImageType, DoubleVectorImageType>;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SARPolarMatrixConvert, otb::Application);

private:
  void DoInit() override
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
    "For instance, the coherency matrix size is 3x3 in the monostatic case, and 4x4 in the bistatic case: it will thus be stored in a 6-band or a 10-band complex image (the diagonal and the upper elements of the matrix).\n"
    "\n"
    "The Sinclair matrix is a special case: it is always represented as 3 or 4 one-band complex images (for mono or bistatic case).\n"
    "The available conversions are listed below:\n"

	"\n--- Monostatic case ---\n"

    "1 msinclairtocoherency --> Sinclair matrix to coherency matrix (input: 3 x 1 complex channel (HH, HV or VH, VV) | output:  6 complex channels)\n"
    "2 msinclairtocovariance --> Sinclair matrix to covariance matrix (input: 3 x 1 complex channel (HH, HV or VH, VV) | output:  6 complex channels)\n"
    "3 msinclairtocircovariance --> Sinclair matrix to circular covariance matrix (input: 3 x 1 complex channel (HH, HV or VH, VV) | output:  6 complex channels)\n"
    "4 mcoherencytomueller --> Coherency matrix to Mueller matrix (input: 6 complex channels | 16 real channels)\n"
    "5 mcovariancetocoherencydegree --> Covariance matrix to coherency degree (input: 6 complex channels | 3 complex channels)\n"
    "6 mcovariancetocoherency --> Covariance matrix to coherency matrix (input: 6 complex channels | 6 complex channels)\n"
    "7 mlinearcovariancetocircularcovariance --> Covariance matrix to circular covariance matrix (input: 6 complex channels | output: 6 complex channels)\n"

    "\n--- Bistatic case ---\n"

    "8 bsinclairtocoherency --> Sinclair matrix to coherency matrix (input: 4 x 1 complex channel (HH, HV, VH, VV) | 10 complex channels)\n"
    "9 bsinclairtocovariance --> Sinclair matrix to covariance matrix (input: 4 x 1 complex channel (HH, HV, VH, VV) | output: 10 complex channels)\n"
    "10 bsinclairtocircovariance --> Sinclair matrix to circular covariance matrix (input: 4 x 1 complex channel (HH, HV, VH, VV) | output: 10 complex channels)\n"

    "\n--- Both cases ---\n"

    "11 sinclairtomueller --> Sinclair matrix to Mueller matrix (input: 4 x 1 complex channel (HH, HV, VH, VV) | output: 16 real channels)\n"
    "12 muellertomcovariance --> Mueller matrix to covariance matrix (input: 16 real channels | output: 6 complex channels)\n"
    "13 muellertopoldegandpower --> Mueller matrix to polarization degree and power (input: 16 real channels | output: 4 real channels)"

 );
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("SARPolarSynth, SARDecompositions");

    AddDocTag(Tags::SAR);

    AddParameter(ParameterType_InputImage, "inc", "Input multi-band complex image");
    SetParameterDescription("inc", "Input: multi-band complex image");
    MandatoryOff("inc");

    AddParameter(ParameterType_InputImage, "inf", "Input multi-band real image");
    SetParameterDescription("inf", "Input: multi-band real image");
    MandatoryOff("inf");


    AddParameter(ParameterType_InputImage, "inhh", "Input one-band complex image (HH)");
    SetParameterDescription("inhh", "Input: one-band complex image (HH)");
    MandatoryOff("inhh");

    AddParameter(ParameterType_InputImage, "inhv", "Input one-band complex image (HV)");
    SetParameterDescription("inhv", "Input: one-band complex image (HV)");
    MandatoryOff("inhv");

    AddParameter(ParameterType_InputImage, "invh", "Input one-band complex image (VH)");
    SetParameterDescription("invh", "Input: one-band complex image (VH)");
    MandatoryOff("invh");

    AddParameter(ParameterType_InputImage, "invv", "Input one-band complex image (VV)");
    SetParameterDescription("invv", "Input: one-band complex image (VV)");
    MandatoryOff("invv");

    AddParameter(ParameterType_OutputImage, "outc", "Output Complex Image");
    SetParameterDescription("outc", "Output Complex image.");
    MandatoryOff("outc");

    AddParameter(ParameterType_OutputImage, "outf", "Output Real Image");
    SetParameterDescription("outf", "Output Real image.");
    MandatoryOff("outf");


    AddParameter(ParameterType_Choice, "conv", "Conversion");

    // Monostatic case

    // #1
    // SinclairToReciprocalCoherency
    AddChoice("conv.msinclairtocoherency","1 Monostatic: Sinclair matrix to coherency matrix (complex output)");
    SetParameterDescription("conv.msinclairtocoherency","1 Monostatic: Sinclair matrix to coherency matrix (complex output)");

    // #2
    // SinclairToReciprocalCovariance
    AddChoice("conv.msinclairtocovariance","2 Monostatic: Sinclair matrix to covariance matrix (complex output)");
    SetParameterDescription("conv.msinclairtocovariance","2 Monostatic: Sinclair matrix to covariance matrix (complex output)");

    // #3
    // SinclairToReciprocalCircularCovariance
    AddChoice("conv.msinclairtocircovariance","3 Monostatic: Sinclair matrix to circular covariance matrix (complex output)");
    SetParameterDescription("conv.msinclairtocircovariance","3 Monostatic: Sinclair matrix to circular covariance matrix (complex output)");

    // #4
    // ReciprocalCoherencyToReciprocalMuellerImageFilter
    AddChoice("conv.mcoherencytomueller","4 Monostatic: Coherency matrix to Mueller matrix");
    SetParameterDescription("conv.mcoherencytomueller","4 Monostatic: Coherency matrix to Mueller matrix");

    // #5
    // ReciprocalCovarianceToCoherencyDegreeImageFilter
    AddChoice("conv.mcovariancetocoherencydegree","5 Monostatic: Covariance matrix to coherency degree");
    SetParameterDescription("conv.mcovariancetocoherencydegree","5 Monostatic: Covariance matrix to coherency degree ");

    // #6
    // ReciprocalCovarianceToReciprocalCoherencyImageFilter
    AddChoice("conv.mcovariancetocoherency","6 Monostatic: Covariance matrix to coherency matrix (complex output)");
    SetParameterDescription("conv.mcovariancetocoherency","6 Monostatic: Covariance matrix to coherency matrix (complex output)");

    // #7
    // ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter
    AddChoice("conv.mlinearcovariancetocircularcovariance","7 Monostatic: Covariance matrix to circular covariance matrix (complex output)");
    SetParameterDescription("conv.mlinearcovariancetocircularcovariance","7 Monostatic: Covariance matrix to circular covariance matrix (complex output)");

    // #8
    // MuellerToReciprocalCovarianceImageFilter
    AddChoice("conv.muellertomcovariance","8 Bi/mono: Mueller matrix to monostatic covariance matrix");
    SetParameterDescription("conv.muellertomcovariance","8 Bi/mono: Mueller matrix to monostatic covariance matrix");

    //Bistatic case

    // #9
    // SinclairToCoherency
    AddChoice("conv.bsinclairtocoherency","9 Bistatic: Sinclair matrix to coherency matrix (complex output)");
    SetParameterDescription("conv.bsinclairtocoherency","9 Bistatic: Sinclair matrix to coherency matrix (complex output)");

    // #10
    // SinclairToCovariance
    AddChoice("conv.bsinclairtocovariance","10 Bistatic: Sinclair matrix to covariance matrix (complex output)");
    SetParameterDescription("conv.bsinclairtocovariance","10 Bistatic: Sinclair matrix to covariance matrix (complex output)");

    // #11
    // SinclairToCircularCovariance
    AddChoice("conv.bsinclairtocircovariance","11 Bistatic: Sinclair matrix to circular covariance matrix (complex output)");
    SetParameterDescription("conv.bsinclairtocircovariance","11 Bistatic: Sinclair matrix to circular covariance matrix (complex output)");

    //Both case

    // #12
    // SinclairToMueller
    AddChoice("conv.sinclairtomueller","12 Bi/mono: Sinclair matrix to Mueller matrix");
    SetParameterDescription("conv.sinclairtomueller","12 Bi/mono: Sinclair matrix to Mueller matrix");


    // #13
    // MuellerToPolarisationDegreeAndPowerImageFilter
    AddChoice("conv.muellertopoldegandpower","13 Bi/mono: Mueller matrix to polarisation degree and power");
    SetParameterDescription("conv.muellertopoldegandpower","13 Bi/mono: Mueller matrix to polarisation degree and power");

    AddRAMParameter();

    // Default values
    SetDefaultParameterInt("conv", 0); // SinclairToReciprocalCoherency

    // Doc example parameter settings
    SetDocExampleParameterValue("inhh", "HH.tif");
    SetDocExampleParameterValue("invh", "VH.tif");
    SetDocExampleParameterValue("invv", "VV.tif");
    SetDocExampleParameterValue("conv", "msinclairtocoherency");
    SetDocExampleParameterValue("outc", "mcoherency.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {

    int convType = GetParameterInt("conv");

    if ((convType >= 0) && (convType <= 2)) // msinclairtocoherency msinclairtocovariance msinclairtocircovariance
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
    else if ((convType >= 3) &&
             (convType <= 6)) // mcoherencytomueller mcovariancetocoherencydegree mcovariancetocoherency mlinearcovariancetocircularcovariance
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
    else if (convType == 7) // muellertomcovariance
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
    else if ((convType >= 8) && (convType <= 11)) // bsinclairtocoherency bsinclairtocovariance bsinclairtocircovariance sinclairtomueller
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
    else if (convType == 12) // muellertopoldegandpower
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

  void DoExecute() override
  {

    //****************************************
    //* Check inputs and outputs consistency *
    //****************************************

    bool inc  = HasUserValue("inc");
    bool inf  = HasUserValue("inf");
    bool inhh = HasUserValue("inhh");
    bool inhv = HasUserValue("inhv");
    bool invh = HasUserValue("invh");
    bool invv = HasUserValue("invv");
    bool outc = HasUserValue("outc");
    bool outf = HasUserValue("outf");

    int convType = GetParameterInt("conv");


    if ((!outc) && (!outf))
      otbAppLogFATAL(<< "No output image provided; please, set the parameter 'outc' or 'outf'.");


    if ((convType >= 0) && (convType <= 2)) // msinclairtocoherency msinclairtocovariance msinclairtocircovariance
    {
      if ((!inhv) && (!invh))
        otbAppLogFATAL(<< "Parameter 'inhv' or 'invh' not set.");
      if (!inhh)
        otbAppLogFATAL(<< "Parameter 'inhh' not set.");
      if (!invv)
        otbAppLogFATAL(<< "Parameter 'invv' not set.");
    }

    else if ((convType >= 3) &&
             (convType <= 6)) // mcoherencytomueller mcovariancetocoherencydegree mcovariancetocoherency mlinearcovariancetocircularcovariance
    {
      if (!inc)
        otbAppLogFATAL(<< "Parameter 'inc' not set.");
    }
    else if ((convType >= 8) && (convType <= 11)) // bsinclairtocoherency bsinclairtocovariance bsinclairtocircovariance sinclairtomueller
    {
      if ((!inhh) || (!inhv) || (!invh) || (!invv))
        otbAppLogFATAL(<< "Please, ensure that HH, HV, VH and VV complex images have been provided (parameters inhh, inhv, invh, invv).");
    }
    else if ((convType == 7) || (convType == 12)) // muellertomcovariance muellertopoldegandpower
    {
      if (!inf)
        otbAppLogFATAL(<< "Parameter 'inf' not set.");
    }


    switch (GetParameterInt("conv"))
    {

      //***************************************
      //*             MONOSTATIC              *
      //***************************************

    case 0: // SinclairToReciprocalCoherency
      m_RCohSRFilter = RCohSRFilterType::New();

      if (inhv)
        m_RCohSRFilter->SetVariadicNamedInput<polarimetry_tags::hv_or_vh>(GetParameterComplexDoubleImage("inhv"));
      else if (invh)
        m_RCohSRFilter->SetVariadicNamedInput<polarimetry_tags::hv_or_vh>(GetParameterComplexDoubleImage("invh"));

      m_RCohSRFilter->SetVariadicNamedInput<polarimetry_tags::hh>(GetParameterComplexDoubleImage("inhh"));
      m_RCohSRFilter->SetVariadicNamedInput<polarimetry_tags::vv>(GetParameterComplexDoubleImage("invv"));

      SetParameterOutputImage("outc", m_RCohSRFilter->GetOutput()); // input: 3 x 1 complex channel | output :  6 complex channels

      break;


    case 1: // SinclairToReciprocalCovariance

      m_RCovSRFilter = RCovSRFilterType::New();

      if (inhv)
        m_RCovSRFilter->SetVariadicNamedInput<polarimetry_tags::hv_or_vh>(GetParameterComplexDoubleImage("inhv"));
      else if (invh)
        m_RCovSRFilter->SetVariadicNamedInput<polarimetry_tags::hv_or_vh>(GetParameterComplexDoubleImage("invh"));

      m_RCovSRFilter->SetVariadicNamedInput<polarimetry_tags::hh>(GetParameterComplexDoubleImage("inhh"));
      m_RCovSRFilter->SetVariadicNamedInput<polarimetry_tags::vv>(GetParameterComplexDoubleImage("invv"));

      SetParameterOutputImage("outc", m_RCovSRFilter->GetOutput()); // input: 3 x 1 complex channel | output :  6 complex channels

      break;


    case 2: // SinclairToReciprocalCircularCovariance

      m_RCCSRFilter = RCCSRFilterType::New();

      if (inhv)
        m_RCCSRFilter->SetVariadicNamedInput<polarimetry_tags::hv_or_vh>(GetParameterComplexDoubleImage("inhv"));
      else if (invh)
        m_RCCSRFilter->SetVariadicNamedInput<polarimetry_tags::hv_or_vh>(GetParameterComplexDoubleImage("invh"));

      m_RCCSRFilter->SetVariadicNamedInput<polarimetry_tags::hh>(GetParameterComplexDoubleImage("inhh"));
      m_RCCSRFilter->SetVariadicNamedInput<polarimetry_tags::vv>(GetParameterComplexDoubleImage("invv"));

      SetParameterOutputImage("outc", m_RCCSRFilter->GetOutput()); // input: 3 x 1 complex channel | output :  6 complex channels

      break;


    case 3: // ReciprocalCoherencyToReciprocalMuellerImageFilter

      m_RCRMFilter = RCRMFilterType::New();
      m_RCRMFilter->SetVariadicInput<0>(GetParameterComplexDoubleVectorImage("inc"));

      SetParameterOutputImage("outf", m_RCRMFilter->GetOutput()); // input: 6 complex channels | 16 real channels

      break;


    case 4: // ReciprocalCovarianceToCoherencyDegreeImageFilter

      m_RCCDFilter = RCCDFilterType::New();
      m_RCCDFilter->SetInput1(GetParameterComplexDoubleVectorImage("inc"));

      SetParameterOutputImage("outc", m_RCCDFilter->GetOutput()); // input: 6 complex channels | 3 complex channels

      break;


    case 5: // ReciprocalCovarianceToReciprocalCoherencyImageFilter

      m_RCRCFilter = RCRCFilterType::New();
      m_RCRCFilter->SetInput1(GetParameterComplexDoubleVectorImage("inc"));

      SetParameterOutputImage("outc", m_RCRCFilter->GetOutput()); // input: 6 complex channels | 6 complex channels

      break;


    case 6: // ReciprocalLinearCovarianceToReciprocalCircularCovarianceImageFilter

      m_RLCRCCFilter = RLCRCCFilterType::New();
      m_RLCRCCFilter->SetInput1(GetParameterComplexDoubleVectorImage("inc"));

      SetParameterOutputImage("outc", m_RLCRCCFilter->GetOutput()); // input: 6 complex channels | output : 6 complex channels

      break;


    case 7: // MuellerToReciprocalCovarianceImageFilter

      m_MRCFilter = MRCFilterType::New();

      m_MRCFilter->SetInput1(GetParameterDoubleVectorImage("inf"));

      SetParameterOutputImage("outc", m_MRCFilter->GetOutput()); // input: 16 real channels | output : 6 complex channels

      break;


      //***************************************
      //*               BISTATIC              *
      //***************************************

    case 8: // SinclairToCoherency

      m_CohSRFilter = CohSRFilterType::New();
      m_CohSRFilter->SetVariadicNamedInput<polarimetry_tags::hh>(GetParameterComplexDoubleImage("inhh"));
      m_CohSRFilter->SetVariadicNamedInput<polarimetry_tags::hv>(GetParameterComplexDoubleImage("inhv"));
      m_CohSRFilter->SetVariadicNamedInput<polarimetry_tags::vh>(GetParameterComplexDoubleImage("invh"));
      m_CohSRFilter->SetVariadicNamedInput<polarimetry_tags::vv>(GetParameterComplexDoubleImage("invv"));

      SetParameterOutputImage("outc", m_CohSRFilter->GetOutput()); // input: 4 x 1 complex channel | 10 complex channels

      break;


    case 9: // SinclairToCovariance

      m_CovSRFilter = CovSRFilterType::New();
      m_CovSRFilter->SetVariadicNamedInput<polarimetry_tags::hh>(GetParameterComplexDoubleImage("inhh"));
      m_CovSRFilter->SetVariadicNamedInput<polarimetry_tags::hv>(GetParameterComplexDoubleImage("inhv"));
      m_CovSRFilter->SetVariadicNamedInput<polarimetry_tags::vh>(GetParameterComplexDoubleImage("invh"));
      m_CovSRFilter->SetVariadicNamedInput<polarimetry_tags::vv>(GetParameterComplexDoubleImage("invv"));

      SetParameterOutputImage("outc", m_CovSRFilter->GetOutput()); // input: 4 x 1 complex channel | output : 10 complex channels

      break;


    case 10: // SinclairToCircularCovariance

      m_CCSRFilter = CCSRFilterType::New();
      m_CCSRFilter->SetVariadicNamedInput<polarimetry_tags::hh>(GetParameterComplexDoubleImage("inhh"));
      m_CCSRFilter->SetVariadicNamedInput<polarimetry_tags::hv>(GetParameterComplexDoubleImage("inhv"));
      m_CCSRFilter->SetVariadicNamedInput<polarimetry_tags::vh>(GetParameterComplexDoubleImage("invh"));
      m_CCSRFilter->SetVariadicNamedInput<polarimetry_tags::vv>(GetParameterComplexDoubleImage("invv"));

      SetParameterOutputImage("outc", m_CCSRFilter->GetOutput()); // input: 4 x 1 complex channel | output : 10 complex channels

      break;


      //***************************************
      //*             BOTH CASES              *
      //***************************************


    case 11: // SinclairToMueller
      m_MSRFilter = MSRFilterType::New();

      m_MSRFilter->SetVariadicNamedInput<polarimetry_tags::hh>(GetParameterComplexDoubleImage("inhh"));
      m_MSRFilter->SetVariadicNamedInput<polarimetry_tags::hv>(GetParameterComplexDoubleImage("inhv"));
      m_MSRFilter->SetVariadicNamedInput<polarimetry_tags::vh>(GetParameterComplexDoubleImage("invh"));
      m_MSRFilter->SetVariadicNamedInput<polarimetry_tags::vv>(GetParameterComplexDoubleImage("invv"));

      SetParameterOutputImage("outf", m_MSRFilter->GetOutput()); // input: 4 x 1 complex channel | output : 16 real channels

      break;


    case 12: // MuellerToPolarisationDegreeAndPowerImageFilter
      m_MPDPFilter = MPDPFilterType::New();

      m_MPDPFilter->SetInput1(GetParameterDoubleVectorImage("inf"));

      SetParameterOutputImage("outf", m_MPDPFilter->GetOutput()); //  input: 16 real channels | output : 4 real channels

      break;
    }
  }

  // Monostatic
  RCohSRFilterType::Pointer m_RCohSRFilter;
  RCovSRFilterType::Pointer m_RCovSRFilter;
  RCCSRFilterType::Pointer  m_RCCSRFilter;
  RCRMFilterType::Pointer   m_RCRMFilter;
  RCCDFilterType::Pointer   m_RCCDFilter;
  RCRCFilterType::Pointer   m_RCRCFilter;
  RLCRCCFilterType::Pointer m_RLCRCCFilter;

  // Bistatic
  CohSRFilterType::Pointer m_CohSRFilter;
  CovSRFilterType::Pointer m_CovSRFilter;
  CCSRFilterType::Pointer  m_CCSRFilter;
  MSRFilterType::Pointer   m_MSRFilter;

  // Both cases
  MRCFilterType::Pointer  m_MRCFilter;
  MPDPFilterType::Pointer m_MPDPFilter;
};

} // end namespace Wrapper
} // end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::SARPolarMatrixConvert)
