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
    typedef FastNLMeans                       Self;
    typedef Application                   Superclass;
    typedef itk::SmartPointer<Self>       Pointer;
    typedef itk::SmartPointer<const Self> ConstPointer;

    itkNewMacro(Self);
    itkTypeMacro(FastNLMeans, otb::Wrapper::Application);

    // Define image types
    typedef float                PixelType;
    typedef FloatImageType       ImageType;

    // Define filter
    typedef NLMeansFilter<ImageType, ImageType>   NLMeansFilterType;

  private:
    void DoInit() override
    {
      SetName("FastNLMeans");
      SetDescription("Apply NL Means filter to an image.");

      SetDocLongDescription("Implementation is an approximation of NL Means, which is faster.");

      //Optional descriptors
      SetDocLimitations("This filter relies on integral images. Overflow may happen though the risk is limited "
			" by OTB mechanism which process data by chunks.");
      SetDocAuthors("OTB-Team"); 
      SetDocSeeAlso(" ");
      AddDocTag(Tags::Filter);

      //Parameter declarations
      AddParameter(ParameterType_InputImage,  "in",   "Input image");
      SetParameterDescription("in", "Input image to denoise");

      AddParameter(ParameterType_OutputImage, "out", "Output Image");
      SetParameterDescription("out","Output image.");

      AddParameter(ParameterType_Int, "patchsize", "Half patch size (patch is a square)");
      SetParameterDescription("patchsize", "Full patch will have a size of 2*patchsize +1.");
      SetDefaultParameterInt("patchsize", 2);
      SetMinimumParameterIntValue("patchsize", 0);
      MandatoryOff("patchsize");

      AddParameter(ParameterType_Int, "searchsize", "Half search window size");
      SetParameterDescription("searchsize", "Search window is used to find similar patches. Its size will be 2*searchsize+1. It is a squared window.");
      SetDefaultParameterInt("searchsize", 7);
      SetMinimumParameterIntValue("searchsize", 0);
      MandatoryOff("searchsize");

      AddParameter(ParameterType_Float, "sig", "Standard deviation in image");
      SetParameterDescription("sig", "Noise standard deviation estimated in image. This parameter is used to correct for the expected difference between two patches. This filter works fine without using this tuning.");
      SetDefaultParameterFloat("sig", 0);
      SetMinimumParameterFloatValue("sig", 0);
      MandatoryOff("sig");

      AddParameter(ParameterType_Float, "thresh", "Similarity threshold");
      SetParameterDescription("thresh", "Factor influencing similarity score of two patches. The higher the threshold, the more permissive the filter. It is common to set this threshold slightly below the standard deviation (for Gaussian noise), at about 0.8*sigma.");
      SetDefaultParameterFloat("thresh", 1.0);
      SetMinimumParameterFloatValue("thresh", 0.);
      MandatoryOff("thresh");

      AddRAMParameter();

      SetDocExampleParameterValue("in", "GomaAvant.tif");
      SetDocExampleParameterValue("out","denoisedImage_NLMeans.tif");
    }

    void DoUpdateParameters() override
    {
      // Nothing to do here : all parameters are independent
    }
	
    void DoExecute() override
    {
      // Get the input image
      ImageType::Pointer imIn = this->GetParameterFloatImage("in");
      float sigma = this->GetParameterFloat("sig");
      float cutoffDistance = this->GetParameterFloat("thresh");
      int halfPatchSize = this->GetParameterInt("patchsize");
      int halfSearchSize = this->GetParameterInt("searchsize");
      NLMeansFilterType::Pointer nlMeansFilter = NLMeansFilterType::New();
      nlMeansFilter->SetInput(imIn);
      nlMeansFilter->SetSigma(sigma);
      nlMeansFilter->SetHalfWindowSize(halfPatchSize);
      nlMeansFilter->SetHalfSearchSize(halfSearchSize);
      nlMeansFilter->SetCutOffDistance(cutoffDistance);

      m_FilterRef = nlMeansFilter;
      SetParameterOutputImage("out", nlMeansFilter->GetOutput());
      RegisterPipeline();
    }

    itk::LightObject::Pointer m_FilterRef;
  }; // end class

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::FastNLMeans)
