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

#include "otbStreamingMinMaxVectorImageFilter.h"
#include "otbVectorRescaleIntensityImageFilter.h"

namespace otb
{
namespace Wrapper
{


class Rescale : public Application
{
public:
  /** Standard class typedefs. */
  typedef Rescale                       Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Rescale, otb::Application);

  /** Filters typedef */
  typedef otb::StreamingMinMaxVectorImageFilter<FloatVectorImageType>  MinMaxFilterType;
  typedef otb::VectorRescaleIntensityImageFilter<FloatVectorImageType> RescaleImageFilterType;

private:
  void DoInit() override
  {
    SetName("Rescale");
    SetDescription("Rescale the image between two given values.");

    SetDocLongDescription(
        "This application scales the given image pixel intensity between two given values.\n"
        "By default min (resp. max) value is set to 0 (resp. 255).\n"
        "Input minimum and maximum values is automatically computed for all image bands.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("DynamicConvert");

    AddDocTag(Tags::Deprecated);
    AddDocTag(Tags::Manip);
    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "The image to scale.");
    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "The rescaled image filename.");

    AddParameter(ParameterType_Float, "outmin", "Output min value");
    AddParameter(ParameterType_Float, "outmax", "Output max value");
    SetDefaultParameterFloat("outmin", 0.0);
    SetParameterDescription("outmin", "Minimum value of the output image.");
    SetDefaultParameterFloat("outmax", 255.0);
    SetParameterDescription("outmax", "Maximum value of the output image.");

    MandatoryOff("outmin");
    MandatoryOff("outmax");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_Toulouse_Ortho_PAN.tif");
    SetDocExampleParameterValue("out", "rescaledImage.png uchar");
    SetDocExampleParameterValue("outmin", "0");
    SetDocExampleParameterValue("outmax", "255");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here for the parameters : all are independent
  }

  void DoExecute() override
  {
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");

    otbAppLogDEBUG(<< "Starting Min/Max computation")

        MinMaxFilterType::Pointer minMaxFilter = MinMaxFilterType::New();
    minMaxFilter->SetInput(inImage);
    minMaxFilter->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));

    AddProcess(minMaxFilter->GetStreamer(), "Min/Max computing");
    minMaxFilter->Update();

    otbAppLogDEBUG(<< "Min/Max computation done : min=" << minMaxFilter->GetMinimum() << " max=" << minMaxFilter->GetMaximum())

        FloatVectorImageType::PixelType inMin,
        inMax;

    RescaleImageFilterType::Pointer rescaleFilter = RescaleImageFilterType::New();
    rescaleFilter->SetInput(inImage);
    rescaleFilter->SetAutomaticInputMinMaxComputation(false);
    rescaleFilter->SetInputMinimum(minMaxFilter->GetMinimum());
    rescaleFilter->SetInputMaximum(minMaxFilter->GetMaximum());

    FloatVectorImageType::PixelType outMin, outMax;
    outMin.SetSize(inImage->GetNumberOfComponentsPerPixel());
    outMax.SetSize(inImage->GetNumberOfComponentsPerPixel());
    outMin.Fill(GetParameterFloat("outmin"));
    outMax.Fill(GetParameterFloat("outmax"));

    rescaleFilter->SetOutputMinimum(outMin);
    rescaleFilter->SetOutputMaximum(outMax);
    rescaleFilter->UpdateOutputInformation();

    SetParameterOutputImage("out", rescaleFilter->GetOutput());
    RegisterPipeline();
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Rescale)
