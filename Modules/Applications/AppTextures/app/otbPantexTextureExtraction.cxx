/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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

#include "otbMultiToMonoChannelExtractROI.h"
#include "otbScalarImageToPanTexTextureFilter.h"
#include "otbStreamingMinMaxImageFilter.h"

namespace otb
{
namespace Wrapper
{

class PantexTextureExtraction : public Application
{
public:
  
  /** @name Standard class typedefs
   * @{
   */
  using Self = PantexTextureExtraction;
  using Superclass = Application;
  using Pointer = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;
  /** @} */

  /** @name Standard macro
   * @{
   */
  itkNewMacro(Self);
  itkTypeMacro(PantexTextureExtraction, otb::Application);
  /** @} */

  using ExtractorFilterType =
    otb::MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType>;
  using PanTexTextureFilterType = otb::ScalarImageToPanTexTextureFilter<FloatImageType, FloatImageType>;
  using MinMaxImageFilterType = otb::StreamingMinMaxImageFilter<FloatImageType>;

private:
  void DoInit() override
  {
    SetName("PantexTextureExtraction");
    SetDescription("Computes Pantex textural features on the selected channel of the input image");

    SetDocLongDescription(
        "This application computes a texture-derived built-up presence index (PanTex) from textural"
    "characteristics of scalar images. This is a contrast textural measure based on co-occurance."
    );

    SetDocLimitations("None");

    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" Pesari, M., A. Gerhardinger, F. Kayitakire. 2008.  A robust built-up area precense"
      " index by anisotropic rotation-invariant textural measure."
      " IEEE Journal of selected topics in applied earth observations and remote sensing.Vol1, NO3.");

    AddDocTag(Tags::FeatureExtraction);
    AddDocTag("Textures");
    
    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "The input image to compute the features on.");

    AddParameter(ParameterType_Int, "channel", "Selected Channel");
    SetParameterDescription("channel", "The selected channel index");
    SetDefaultParameterInt("channel", 1);
    SetMinimumParameterIntValue("channel", 1);

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "Output image containing the selected texture features.");

    AddParameter(ParameterType_Float, "min", "Image minimum");
    SetParameterDescription("min", "Input image minimum. If this parameter is not set, the application will compute "
                            "the minumum of the image.");
    MandatoryOff("min");

    AddParameter(ParameterType_Float, "max", "Image maximum");
    SetParameterDescription("max", "Input image maximum. If this parameter is not set, the application will compute "
                            "the maximum of the image.");
    MandatoryOff("max");

    AddParameter(ParameterType_Int, "sradx", "Window radius (x direction)");
    SetParameterDescription("sradx", "Radius of the window on which textures are computed (x direction)");
    SetMinimumParameterIntValue("sradx", 0);
    SetDefaultParameterInt("sradx", 4);

    AddParameter(ParameterType_Int, "srady", "Window radius (y direction)");
    SetParameterDescription("srady", "Radius of the window on which textures are computed (y direction)");
    SetMinimumParameterIntValue("srady", 0);
    SetDefaultParameterInt("srady", 4);

    AddParameter(ParameterType_Int, "nbin", "Number of bins per axis for histogram generation");
    SetParameterDescription("nbin", "Number of bins per axis for histogram generation "
                                    "(number of gray levels considered in the computation of co-occurance).");
    SetDefaultParameterInt("nbin", 8);

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "qb_RoadExtract.tif");
    SetDocExampleParameterValue("channel", "2");
    SetDocExampleParameterValue("parameters.min", "0");
    SetDocExampleParameterValue("parameters.max", "255");
    SetDocExampleParameterValue("nbin", "8");
    SetDocExampleParameterValue("srady", "4");
    SetDocExampleParameterValue("sradx", "4");

    SetOfficialDocLink();
  }
  
  
  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {
    auto inImage = GetParameterImage("in");
    inImage->UpdateOutputInformation();

    if ((unsigned int)GetParameterInt("channel") > inImage->GetNumberOfComponentsPerPixel())
    {
      itkExceptionMacro(<< "The specified channel index is invalid.");
    }

    auto extractorFilter = ExtractorFilterType::New();
    extractorFilter->SetInput(inImage);
    extractorFilter->SetStartX(inImage->GetLargestPossibleRegion().GetIndex(0));
    extractorFilter->SetStartY(inImage->GetLargestPossibleRegion().GetIndex(1));
    extractorFilter->SetSizeX(inImage->GetLargestPossibleRegion().GetSize(0));
    extractorFilter->SetSizeY(inImage->GetLargestPossibleRegion().GetSize(1));
    extractorFilter->SetChannel(GetParameterInt("channel"));

    auto textureFilter = PanTexTextureFilterType::New();

    textureFilter->SetNumberOfBinsPerAxis(GetParameterInt("nbin"));
    textureFilter->SetRadius( {(unsigned int) GetParameterInt("sradx"),
                               (unsigned int) GetParameterInt("srady")} );

    // Compute min and max only if one the corresponding parameter has not been set
    if (!HasValue("min") || !HasValue("max"))
    {
      auto minMaxFilter = MinMaxImageFilterType::New();
      minMaxFilter->SetInput(extractorFilter->GetOutput());
      minMaxFilter->Update();
      
      if (!HasValue("min"))
        otbAppLogINFO(<< "Computed Minimum: " << minMaxFilter->GetMinimum());

      if (!HasValue("max"))
        otbAppLogINFO(<< "Computed Maximum: " << minMaxFilter->GetMaximum());

      textureFilter->SetInputImageMinimum(HasValue("min") ? GetParameterFloat("min") 
                                            : minMaxFilter->GetMinimum());
      textureFilter->SetInputImageMaximum(HasValue("max") ? GetParameterFloat("max") 
                                            : minMaxFilter->GetMaximum());

    }
    else
    {
      textureFilter->SetInputImageMinimum(GetParameterFloat("min"));
      textureFilter->SetInputImageMaximum(GetParameterFloat("max"));
    }
    
    textureFilter->SetInput(extractorFilter->GetOutput());
    SetParameterOutputImage("out", textureFilter->GetOutput());

    RegisterPipeline();
  }
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::PantexTextureExtraction)
