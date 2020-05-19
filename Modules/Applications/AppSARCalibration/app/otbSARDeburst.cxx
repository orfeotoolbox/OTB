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

#include "otbSarDeburstImageFilter.h"

namespace otb
{
namespace Wrapper
{
class SARDeburst : public Application
{
public:
  /** Standard class typedefs. */
  typedef SARDeburst                    Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SARDeburst, otb::Application);

  typedef otb::SarDeburstImageFilter<FloatVectorImageType> DeburstFilterType;

private:
  void DoInit() override
  {
    SetName("SARDeburst");
    SetDescription("This application performs deburst of Sentinel1 IW SLC images by removing redundant lines.");

    // Documentation
    SetDocLongDescription(
        "Sentinel1 IW SLC products are composed of several burst overlapping in"
        " azimuth time for each subswath, separated by black lines [1]. The deburst"
        " operation consist in generating a continuous image in terms of azimuth"
        " time, by removing black separation lines as well as redundant lines"
        " between bursts.\n\n"

        "Note that the output sensor model is updated accordingly. This deburst"
        " operation is the perfect preprocessing step to orthorectify S1 IW SLC"
        " product with OTB [2] without suffering from artifacts caused by"
        " bursts separation.\n"

        "Two modes are available for the output image : with all samples and"
        "with only valid samples ");

    SetDocLimitations(
        "Only Sentinel1 IW SLC products are supported for now. Processing of"
        " other Sentinel1 modes or TerrasarX images will result in no changes in"
        " the image and metadata. Images from other sensors will lead to an"
        " error.");

    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(
        "[1] Sentinel1 User Handbook, p. 52: https://sentinel.esa.int/documents/247904/685163/Sentinel-1_User_Handbook\n"
        "[2] OrthoRectification application");

    AddDocTag(Tags::SAR);
    AddDocTag(Tags::Calibration);

    AddParameter(ParameterType_InputImage, "in", "Input Sentinel1 IW SLC Image");
    SetParameterDescription("in", "Raw Sentinel1 IW SLC image, or any extract of such made by OTB (geom file needed)");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out",
                            "Deburst image, with updated geom file that can be further used by Orthorectification application. If the input image is a raw "
                            "Sentinel1 product, uint16 output type should be used (encoding of S1 product). Otherwise, output type should match type of input "
                            "image.");

    AddParameter(ParameterType_Bool, "onlyvalidsamples", "Select the modes for output image");
    SetParameterDescription("onlyvalidsamples", "If true, the selected mode is with only valid samples.");

    AddRAMParameter();

    SetDocExampleParameterValue("in", "s1_iw_slc.tif");
    SetDocExampleParameterValue("out", "s1_iw_slc_deburst.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
  }

  void DoExecute() override
  {
    // Get the input complex image
    FloatVectorImageType* in = GetParameterImage("in");

    // Set the filer input
    m_DeburstFilter = DeburstFilterType::New();
    m_DeburstFilter->SetInput(in);

    if (IsParameterEnabled("onlyvalidsamples"))
    {
      m_DeburstFilter->SetOnlyValidSample(true);
    }


    // Set the output image
    SetParameterOutputImage("out", m_DeburstFilter->GetOutput());
  }

  DeburstFilterType::Pointer m_DeburstFilter;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::SARDeburst)
