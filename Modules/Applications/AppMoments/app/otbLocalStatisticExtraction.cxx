/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbRadiometricMomentsImageFilter.h"

#include "otbMultiToMonoChannelExtractROI.h"

namespace otb
{
namespace Wrapper
{

class LocalStatisticExtraction : public Application
{
public:
  /** Standard class typedefs. */
  typedef LocalStatisticExtraction      Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType> ExtractorFilterType;

  typedef RadiometricMomentsImageFilter<FloatImageType, FloatVectorImageType> FilterType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(LocalStatisticExtraction, otb::Application);

private:
  void DoInit() override
  {
    SetName("LocalStatisticExtraction");
    SetDescription("Computes local statistical moments on every pixel in the selected channel of the input image");

    // Documentation
    SetDocLongDescription(
        "This application computes the 4 local statistical moments on every pixel in the "
        "selected channel of the input image, over a specified neighborhood. The output image is multi band "
        "with one statistical moment (feature) per band. Thus, the 4 output features are the Mean, the "
        "Variance, the Skewness and the Kurtosis. They are provided in this exact order in the output image.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("otbRadiometricMomentsImageFunction class");

    AddDocTag(Tags::FeatureExtraction);
    AddDocTag("Statistics");

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "The input image to compute the features on.");

    AddParameter(ParameterType_OutputImage, "out", "Feature Output Image");
    SetParameterDescription("out", "Output image containing the local statistical moments.");

    AddParameter(ParameterType_Int, "channel", "Selected Channel");
    SetParameterDescription("channel", "The selected channel index (1 based)");
    SetDefaultParameterInt("channel", 1);
    SetMinimumParameterIntValue("channel", 1);

    AddParameter(ParameterType_Int, "radius", "Neighborhood radius");
    SetParameterDescription("radius", "The computational window radius.");
    SetMinimumParameterIntValue("radius", 1);
    SetDefaultParameterInt("radius", 3);

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "qb_RoadExtract.tif");
    SetDocExampleParameterValue("channel", "1");
    SetDocExampleParameterValue("radius", "3");
    SetDocExampleParameterValue("out", "Statistics.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");
    int                           nbChan  = inImage->GetNumberOfComponentsPerPixel();

    if (GetParameterInt("channel") > nbChan)
    {
      otbAppLogCRITICAL("Selected band is not available...");
      return;
    }

    m_ExtractorFilter = ExtractorFilterType::New();
    m_ExtractorFilter->SetInput(inImage);
    m_ExtractorFilter->SetStartX(inImage->GetLargestPossibleRegion().GetIndex(0));
    m_ExtractorFilter->SetStartY(inImage->GetLargestPossibleRegion().GetIndex(1));
    m_ExtractorFilter->SetSizeX(inImage->GetLargestPossibleRegion().GetSize(0));
    m_ExtractorFilter->SetSizeY(inImage->GetLargestPossibleRegion().GetSize(1));
    m_ExtractorFilter->SetChannel(GetParameterInt("channel"));

    m_Filter = FilterType::New();
    m_Filter->SetInput(m_ExtractorFilter->GetOutput());
    m_Filter->SetRadius(GetParameterInt("radius"));

    SetParameterOutputImage("out", m_Filter->GetOutput());
  }
  ExtractorFilterType::Pointer m_ExtractorFilter;
  FilterType::Pointer          m_Filter;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::LocalStatisticExtraction)
