/*
 * Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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

#include "otbSarRadiometricCalibrationToImageFilter.h"

namespace otb
{
namespace Wrapper
{
class SARCalibration : public Application
{
public:
  /** Standard class typedefs. */
  typedef SARCalibration                Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SARCalibration, otb::Application);

  typedef otb::SarRadiometricCalibrationToImageFilter<ComplexFloatImageType, FloatImageType> CalibrationFilterType;

private:
  void DoInit() override
  {
    SetName("SARCalibration");
    SetDescription(
        "Perform radiometric calibration of SAR images. Following sensors are supported: TerraSAR-X, Sentinel1 and Radarsat-2.Both Single Look Complex(SLC) "
        "and detected products are supported as input.");

    // Documentation
    SetDocLongDescription(
        "The objective of SAR calibration is to provide imagery in which the pixel values can be directly related to the radar backscatter of the scene. This "
        "application allows computing Sigma Naught (Radiometric Calibration) for TerraSAR-X, Sentinel1 L1 and Radarsat-2 sensors. Metadata are automatically "
        "retrieved from image products.The application supports complex and non-complex images (SLC or detected products).");
    SetDocLimitations("The implementation of TerraSAR-X denoising is still experimental. The results have not been validated recently, see OTB issue #2158");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(
        "References: \n \n"
        "* [1] MPC-0392 DI-MPC-TN Thermal Denoising of Products Generated by the S-1 IPF, issue 1.1 "
        "* [2] Radiometric Calibration of TerraSAR-X Data to Beta Nought and Sigma Nought (Airbus Defence and Space)");

    AddDocTag(Tags::Calibration);
    AddDocTag(Tags::SAR);

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "Input complex image");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "Output calibrated image. This image contains the backscatter (sigmaNought) of the input image.");

    AddParameter(ParameterType_Bool, "removenoise", "Remove Noise");
    SetParameterDescription("removenoise", "Remove the noise of the input product. The type of noise and the "
                            "denoising method depends on the input sensor. The following sensors are supported: \n \n"
            "* Sentinel 1: Remove thermal noise using the denoising LUTs according to [1]  \n"
            "* TerraSAR-X: Noise Equivalent Beta Nought computation [2], see the application limitations");

    AddParameter(ParameterType_Choice, "lut", "Lookup table");
    SetParameterDescription(
        "lut", "Lookup table values are not available with all SAR products. Products that provide lookup table with metadata are: Sentinel1, Radarsat2.");
    AddChoice("lut.sigma", "Use sigma nought lookup");
    SetParameterDescription("lut.sigma", "Use Sigma nought lookup value from product metadata");
    AddChoice("lut.beta", "Use beta nought lookup");
    SetParameterDescription("lut.beta", "Use Beta nought lookup value from product metadata");
    AddChoice("lut.gamma", "Use gamma nought lookup");
    SetParameterDescription("lut.gamma", "Use Gamma nought lookup value from product metadata");
    AddChoice("lut.dn", "Use DN value lookup");
    SetParameterDescription("lut.dn", "Use DN value lookup value from product metadata");
    SetDefaultParameterInt("lut", 0);

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "RSAT_imagery_HH.tif");
    SetDocExampleParameterValue("out", "SarRadiometricCalibration.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
  }

  void DoExecute() override
  {
    // Get the input complex image
    ComplexFloatImageType* floatComplexImage = GetParameterComplexFloatImage("in");

    // Set the filer input
    m_CalibrationFilter = CalibrationFilterType::New();
    m_CalibrationFilter->SetInput(floatComplexImage);

    m_CalibrationFilter->SetEnableNoise(GetParameterInt("removenoise"));
    m_CalibrationFilter->SetLookupSelected(GetParameterInt("lut"));

    // Set the output image
    SetParameterOutputImage("out", m_CalibrationFilter->GetOutput());
  }

  CalibrationFilterType::Pointer m_CalibrationFilter;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::SARCalibration)
