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

#include "otbReciprocalBarnesDecompImageFilter.h"
#include "otbReciprocalHuynenDecompImageFilter.h"
#include "otbReciprocalPauliDecompImageFilter.h"
#include "otbReciprocalHAlphaImageFilter.h"

#include "otbPerBandVectorImageFilter.h"
#include "itkMeanImageFilter.h"
#include "otbImageListToVectorImageFilter.h"
#include "otbImageList.h"

#include "otbSinclairToReciprocalCoherencyMatrixImageFilter.h"

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


  using SRFilterType = otb::SinclairToReciprocalCoherencyMatrixImageFilter<ComplexDoubleImageType, ComplexDoubleVectorImageType>;

  typedef itk::MeanImageFilter<ComplexDoubleImageType, ComplexDoubleImageType>                                         MeanFilterType;
  typedef otb::PerBandVectorImageFilter<ComplexDoubleVectorImageType, ComplexDoubleVectorImageType, MeanFilterType>    PerBandMeanFilterType;
  //typedef otb::NRIBandImagesToOneNComplexBandsImage<DoubleVectorImageType, ComplexDoubleVectorImageType>               NRITOOneCFilterType;
  typedef otb::ImageList<ComplexDoubleImageType>                                                                       ImageListType;
  typedef ImageListToVectorImageFilter<ImageListType, ComplexDoubleVectorImageType >                                   ListConcatenerFilterType;


  typedef otb::ReciprocalHAlphaImageFilter<ComplexDoubleVectorImageType, ComplexDoubleVectorImageType> 			       HAFilterType;
  typedef otb::ReciprocalBarnesDecompImageFilter<ComplexDoubleVectorImageType, ComplexDoubleVectorImageType>           BarnesFilterType;
  typedef otb::ReciprocalHuynenDecompImageFilter<ComplexDoubleVectorImageType, ComplexDoubleVectorImageType>           HuynenFilterType;
  typedef otb::ReciprocalPauliDecompImageFilter<ComplexDoubleVectorImageType, ComplexDoubleVectorImageType>            PauliFilterType;


  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SARDecompositions, otb::Application);

private:
  void DoInit() override
  {
    SetName("SARDecompositions");
    SetDescription("From one-band complex images (each one related to an element of the Sinclair matrix), returns the selected decomposition.");

    // Documentation
    SetDocName("SARDecompositions");
    SetDocLongDescription(
        "From one-band complex images (HH, HV, VH, VV), returns the selected decomposition.\n \n"
        "All the decompositions implemented are intended for the mono-static case (transmitter and receiver are co-located).\n"
        "There are two kinds of decomposition: coherent ones and incoherent ones.\n"
        "In the coherent case, only the Pauli decomposition is available.\n"
        "In the incoherent case, there the decompositions available: Huynen, Barnes, and H-alpha-A.\n"
        "User must provide three one-band complex images HH, HV or VH, and VV (mono-static case <=> HV = VH).\n"
        "Incoherent decompositions consist in averaging 3x3 complex coherency/covariance matrices; the user must provide the size of the averaging window, "
        "thanks to the parameter inco.kernelsize.");


    SetDocLimitations("Some decompositions output real images, while this application outputs complex images for general purpose.\n"
                      "Users should pay attention to extract the real part of the results provided by this application.\n");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("SARPolarMatrixConvert, SARPolarSynth");

    AddDocTag(Tags::SAR);

    AddParameter(ParameterType_InputImage,  "inhh",   "Input Image");
    SetParameterDescription("inhh", "Input image (HH)");

    AddParameter(ParameterType_InputImage,  "inhv",   "Input Image");
    SetParameterDescription("inhv", "Input image (HV)");
    MandatoryOff("inhv");

    AddParameter(ParameterType_InputImage,  "invh",   "Input Image");
    SetParameterDescription("invh", "Input image (VH)");
    MandatoryOff("invh");

    AddParameter(ParameterType_InputImage,  "invv",   "Input Image");
    SetParameterDescription("invv", "Input image (VV)");

    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output image");

    AddParameter(ParameterType_Choice, "decomp", "Decompositions");
    AddChoice("decomp.haa","H-alpha-A incoherent decomposition");
    SetParameterDescription("decomp.haa","H-alpha-A incoherent decomposition");
    AddChoice("decomp.barnes","Barnes incoherent decomposition");
    SetParameterDescription("decomp.barnes","Barnes incoherent decomposition");
    AddChoice("decomp.huynen","Huynen incoherent decomposition");
    SetParameterDescription("decomp.huynen","Huynen incoherent decomposition");
    AddChoice("decomp.pauli","Pauli coherent decomposition");
    SetParameterDescription("decomp.pauli","Pauli coherent decomposition");

    AddParameter(ParameterType_Group,"inco","Incoherent decompositions");
    SetParameterDescription("inco","This group allows setting parameters related to the incoherent decompositions.");
    AddParameter(ParameterType_Int, "inco.kernelsize",   "Kernel size for spatial incoherent averaging");
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

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here: all parameters are independent
  }

  void DoExecute() override
  {

  bool inhv = HasUserValue("inhv");
	bool invh = HasUserValue("invh");

  if ( (!inhv) && (!invh) )
	  otbAppLogFATAL( << "Parameter inhv or invh not set. Please provide a HV or a VH complex image.");

    m_SRFilter = SRFilterType::New();
    m_HAFilter   = HAFilterType::New();
    m_MeanFilter = PerBandMeanFilterType::New();
    MeanFilterType::InputSizeType radius;
    m_BarnesFilter = BarnesFilterType::New();
    m_HuynenFilter = HuynenFilterType::New();
    m_PauliFilter = PauliFilterType::New();
    m_Concatener = ListConcatenerFilterType::New();
    m_ImageList    = ImageListType::New();

    switch (GetParameterInt("decomp"))
      {
		case 0: // H-alpha-A

    if (inhv)
      m_SRFilter->SetVariadicNamedInput<polarimetry_tags::hv_or_vh>(GetParameterComplexDoubleImage("inhv"));
      else if (invh)
        m_SRFilter->SetVariadicNamedInput<polarimetry_tags::hv_or_vh>(GetParameterComplexDoubleImage("invh"));

      m_SRFilter->SetVariadicNamedInput<polarimetry_tags::hh>(GetParameterComplexDoubleImage("inhh"));
      m_SRFilter->SetVariadicNamedInput<polarimetry_tags::vv>(GetParameterComplexDoubleImage("invv"));

      radius.Fill(GetParameterInt("inco.kernelsize"));
      m_MeanFilter->GetFilter()->SetRadius(radius);

      m_MeanFilter->SetInput(m_SRFilter->GetOutput());
      m_HAFilter->SetVariadicInput<0>(m_MeanFilter->GetOutput());
      SetParameterOutputImage("out", m_HAFilter->GetOutput());

      break;

    case 1: // Barnes

		if (inhv)
      m_SRFilter->SetVariadicNamedInput<polarimetry_tags::hv_or_vh>(GetParameterComplexDoubleImage("inhv"));
      else if (invh)
        m_SRFilter->SetVariadicNamedInput<polarimetry_tags::hv_or_vh>(GetParameterComplexDoubleImage("invh"));

      m_SRFilter->SetVariadicNamedInput<polarimetry_tags::hh>(GetParameterComplexDoubleImage("inhh"));
      m_SRFilter->SetVariadicNamedInput<polarimetry_tags::vv>(GetParameterComplexDoubleImage("invv"));

      radius.Fill(GetParameterInt("inco.kernelsize"));
      m_MeanFilter->GetFilter()->SetRadius(radius);

      m_MeanFilter->SetInput(m_SRFilter->GetOutput());
      m_BarnesFilter->SetVariadicInput<0>(m_MeanFilter->GetOutput());
      SetParameterOutputImage("out", m_BarnesFilter->GetOutput());

      break;

    case 2: // Huynen

		if (inhv)
      m_SRFilter->SetVariadicNamedInput<polarimetry_tags::hv_or_vh>(GetParameterComplexDoubleImage("inhv"));
      else if (invh)
        m_SRFilter->SetVariadicNamedInput<polarimetry_tags::hv_or_vh>(GetParameterComplexDoubleImage("invh"));

      m_SRFilter->SetVariadicNamedInput<polarimetry_tags::hh>(GetParameterComplexDoubleImage("inhh"));
      m_SRFilter->SetVariadicNamedInput<polarimetry_tags::vv>(GetParameterComplexDoubleImage("invv"));

      radius.Fill(GetParameterInt("inco.kernelsize"));
      m_MeanFilter->GetFilter()->SetRadius(radius);

      m_MeanFilter->SetInput(m_SRFilter->GetOutput());
      m_HuynenFilter->SetVariadicInput<0>(m_MeanFilter->GetOutput());
      SetParameterOutputImage("out", m_HuynenFilter->GetOutput());

      break;

    case 3: // Pauli

        m_ImageList->PushBack(GetParameterComplexDoubleImage("inhh"));

		if (inhv)
		  m_ImageList->PushBack(GetParameterComplexDoubleImage("inhv"));
	    else if (invh)
		  m_ImageList->PushBack(GetParameterComplexDoubleImage("invh"));

		m_ImageList->PushBack(GetParameterComplexDoubleImage("invv"));

    m_Concatener->SetInput(m_ImageList);
    m_PauliFilter->SetVariadicInput<0>(m_Concatener->GetOutput());

    SetParameterOutputImage("out", m_PauliFilter->GetOutput() );

    break;
	  }
  }

  SRFilterType::Pointer m_SRFilter;
  HAFilterType::Pointer m_HAFilter;
  BarnesFilterType::Pointer m_BarnesFilter;
  HuynenFilterType::Pointer m_HuynenFilter;
  PauliFilterType::Pointer m_PauliFilter;
  PerBandMeanFilterType::Pointer m_MeanFilter;
  ListConcatenerFilterType::Pointer  m_Concatener;
  ImageListType::Pointer        m_ImageList;
};

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::SARDecompositions)
