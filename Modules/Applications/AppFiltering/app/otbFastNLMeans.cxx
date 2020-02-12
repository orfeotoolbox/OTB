/*
 * Copyright (C) 2005-2018 Centre National d'Etudes Spatiales (CNES)
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

#include "otbFastNLMeansImageFilter.h"

namespace otb
{
namespace Wrapper
{

class FastNLMeans : public Application
{
public:
  typedef FastNLMeans                   Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  itkNewMacro(Self);
  itkTypeMacro(FastNLMeans, otb::Wrapper::Application);

  // Define image types
  typedef float          PixelType;
  typedef FloatImageType ImageType;

  // Define filter
  typedef NLMeansFilter<ImageType, ImageType> NLMeansFilterType;

private:
  void DoInit() override
  {
    SetName("FastNLMeans");
    SetDescription("Apply NL Means filter to an image.");

    SetDocLongDescription("Implementation is an approximation of NL Means, which is faster.");

    // Optional descriptors
    SetDocLimitations(
        "This filter relies on integral images. Overflow may happen though the risk is limited "
        " by OTB mechanism which process data by chunks.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Smoothing");
    AddDocTag(Tags::Filter);

    // Parameter declarations
    AddParameter(ParameterType_InputImage, "in", "Input image");
    SetParameterDescription("in", "Input image to denoise");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "Output image.");

    AddParameter(ParameterType_Int, "patchradius", "Patch radius (patch is a square)");
    SetParameterDescription("patchradius", "Full patch will have a size of 2*patchradius +1.");
    SetDefaultParameterInt("patchradius", 2);
    SetMinimumParameterIntValue("patchradius", 0);
    MandatoryOff("patchradius");

    AddParameter(ParameterType_Int, "searchradius", "Search window radius (search window is a square)");
    SetParameterDescription("searchradius", "Search window is used to find similar patches. Its size will be 2*searchradius+1.");
    SetDefaultParameterInt("searchradius", 7);
    SetMinimumParameterIntValue("searchradius", 0);
    MandatoryOff("searchradius");

    AddParameter(ParameterType_Float, "sig", "Standard deviation in image");
    SetParameterDescription("sig",
                            "Noise standard deviation estimated in image. This parameter is used to correct for the expected difference between two patches. "
                            "This filter works fine without using this tuning.");
    SetDefaultParameterFloat("sig", 0);
    SetMinimumParameterFloatValue("sig", 0);
    MandatoryOff("sig");

    AddParameter(ParameterType_Float, "thresh", "Similarity threshold");
    SetParameterDescription("thresh",
                            "Factor influencing similarity score of two patches. The higher the threshold, the more permissive the filter. It is common to set "
                            "this threshold slightly below the standard deviation (for Gaussian noise), at about 0.8*sigma.");
    SetDefaultParameterFloat("thresh", 1.0);
    SetMinimumParameterFloatValue("thresh", 0.);
    MandatoryOff("thresh");

    AddRAMParameter();

    SetDocExampleParameterValue("in", "GomaAvant.tif");
    SetDocExampleParameterValue("out", "denoisedImage_NLMeans.tif");
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {
    // Get the input parameters
    const auto imIn = this->GetParameterFloatImage("in");
    const auto sigma = this->GetParameterFloat("sig");
    const auto cutoffDistance = this->GetParameterFloat("thresh");
    const auto halfPatchSize  = this->GetParameterInt("patchradius");
    const auto halfSearchSize = this->GetParameterInt("searchradius");
    
    auto nlMeansFilter = NLMeansFilterType::New();
    
    nlMeansFilter->SetInput(imIn);
    nlMeansFilter->SetSigma(sigma);
    nlMeansFilter->SetHalfWindowSize(halfPatchSize);
    nlMeansFilter->SetHalfSearchSize(halfSearchSize);
    nlMeansFilter->SetCutOffDistance(cutoffDistance);

    SetParameterOutputImage("out", nlMeansFilter->GetOutput());
    RegisterPipeline();
  }

}; // end class

} // namespace Wrapper
} // namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::FastNLMeans)
