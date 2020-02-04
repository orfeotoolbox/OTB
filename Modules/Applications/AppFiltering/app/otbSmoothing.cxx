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

#include "itkMeanImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkGradientAnisotropicDiffusionImageFilter.h"
#include "otbPerBandVectorImageFilter.h"

namespace otb
{
namespace Wrapper
{

enum
{
  Smoothing_Mean,
  Smoothing_Gaussian,
  Smoothing_Anisotropic
};

typedef otb::Image<FloatVectorImageType::InternalPixelType, 2> ImageType;

class Smoothing : public Application
{
public:
  /** Standard class typedefs. */
  typedef Smoothing                     Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Smoothing, otb::Application);

private:
  void DoInit() override
  {
    SetName("Smoothing");
    SetDescription("Apply a smoothing filter to an image");

    SetDocLongDescription(
        "This application applies a smoothing filter to an "
        "image. Three methods can be used: a gaussian filter , a mean filter "
        ", or an anisotropic diffusion using the Perona-Malik algorithm.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Filter);

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "Input image to smooth.");
    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "Output smoothed image.");

    AddParameter(ParameterType_Choice, "type", "Smoothing Type");
    SetParameterDescription("type", "Smoothing kernel to apply");

    AddChoice("type.mean", "Mean");

    AddParameter(ParameterType_Radius, "type.mean.radius", "Radius");
    SetParameterDescription("type.mean.radius", "Kernel's radius (in pixels)");
    SetDefaultParameterInt("type.mean.radius", 2);

    AddChoice("type.gaussian", "Gaussian");

    AddParameter(ParameterType_Float, "type.gaussian.stdev", "Standard deviation");
    SetParameterDescription("type.gaussian.stdev", "Standard deviation of the gaussian kernel used to filter the image");
    SetDefaultParameterFloat("type.gaussian.stdev", 2.0);

    AddParameter(ParameterType_Float, "type.gaussian.maxerror", "Maximum error");
    SetParameterDescription("type.gaussian.stdev", "The algorithm will size the discrete kernel so that the error "
                            "resulting from truncation of the kernel is no greater than maxerror.");
    SetDefaultParameterFloat("type.gaussian.maxerror", 0.01);

    AddParameter(ParameterType_Int, "type.gaussian.maxwidth", "Maximum kernel width");
    SetParameterDescription("type.gaussian.maxwidth", "Set the kernel to be no wider than maxwidth pixels, "
                             "even if type.gaussian.maxerror demands it.");
    SetDefaultParameterInt("type.gaussian.maxwidth", 32);

    AddChoice("type.anidif", "Anisotropic Diffusion");

    AddParameter(ParameterType_Float, "type.anidif.timestep", "Time Step");
    SetParameterDescription("type.anidif.timestep", "Time step that will be used to discretize the diffusion equation");

    AddParameter(ParameterType_Int, "type.anidif.nbiter", "Nb Iterations");
    SetParameterDescription("type.anidif.nbiter", "Number of iterations needed to get the result");

    AddParameter(ParameterType_Float, "type.anidif.conductance", "Conductance");
    SetParameterDescription("type.anidif.conductance",
                            "Controls the sensitivity of the conductance term in the diffusion "
                            "equation. The lower it is the stronger the features will be preserved");

    SetDefaultParameterFloat("type.anidif.timestep", 0.125);
    SetDefaultParameterInt("type.anidif.nbiter", 10);
    SetDefaultParameterInt("type.anidif.conductance", 1.);

    SetParameterString("type", "anidif");

    AddRAMParameter();

    // Doc example parameter settings
    SetExampleComment("Image smoothing using a mean filter.", 0);
    SetDocExampleParameterValue("in", "Romania_Extract.tif");
    SetDocExampleParameterValue("out", "smoothedImage_mean.png uchar");
    SetDocExampleParameterValue("type", "mean");

    unsigned int exId = AddExample(
        "Image smoothing using an anisotropic "
        "diffusion filter.");
    SetDocExampleParameterValue("in", "Romania_Extract.tif", exId);
    SetDocExampleParameterValue("out", "smoothedImage_ani.png float", exId);
    SetDocExampleParameterValue("type", "anidif", exId);
    SetDocExampleParameterValue("type.anidif.timestep", "0.1", exId);
    SetDocExampleParameterValue("type.anidif.nbiter", "5", exId);
    SetDocExampleParameterValue("type.anidif.conductance", "1.5", exId);

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here: all parameters are independent
  }

  void DoExecute() override
  {
    const auto inImage = GetParameterImage("in");

    switch (GetParameterInt("type"))
    {
    case Smoothing_Mean:
    {
      otbAppLogINFO("Using mean smoothing");

      typedef itk::MeanImageFilter<ImageType, ImageType> MeanFilterType;
      typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, MeanFilterType> PerBandMeanFilterType;

      auto perBand = PerBandMeanFilterType::New();
      perBand->SetInput(inImage);

      MeanFilterType::InputSizeType radius;
      radius.Fill(GetParameterInt("type.mean.radius"));
      perBand->GetFilter()->SetRadius(radius);

      SetParameterOutputImage("out", perBand->GetOutput());
      
      RegisterPipeline();
    }
    break;
    case Smoothing_Gaussian:
    {
      otbAppLogINFO("Using gaussian smoothing");

      typedef itk::DiscreteGaussianImageFilter<ImageType, ImageType> DiscreteGaussianFilterType;
      typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, DiscreteGaussianFilterType> PerBandDiscreteGaussianFilterType;

      auto perBand = PerBandDiscreteGaussianFilterType::New();
      perBand->SetInput(inImage);

      const auto stdev = GetParameterFloat("type.gaussian.stdev");
      const auto variance = stdev * stdev;
      
      perBand->GetFilter()->SetVariance(variance);
      perBand->GetFilter()->SetUseImageSpacing(false);
      perBand->GetFilter()->SetMaximumError(GetParameterFloat("type.gaussian.maxerror"));
      perBand->GetFilter()->SetMaximumKernelWidth(GetParameterInt("type.gaussian.maxwidth"));
      
      SetParameterOutputImage("out", perBand->GetOutput());
      
      RegisterPipeline();
    }
    break;
    case Smoothing_Anisotropic:
    {
      otbAppLogINFO("Using anisotropic diffusion smoothing");

      typedef itk::GradientAnisotropicDiffusionImageFilter<ImageType, ImageType> GradientAnisotropicDiffusionFilterType;
      typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, GradientAnisotropicDiffusionFilterType>
          PerBandGradientAnisotropicDiffusionFilterType;

      auto perBand = PerBandGradientAnisotropicDiffusionFilterType::New();
      perBand->SetInput(inImage);

      const auto aniDifNbIter = GetParameterInt("type.anidif.nbiter");
      perBand->GetFilter()->SetNumberOfIterations(static_cast<unsigned int>(aniDifNbIter));

      const auto aniDifTimeStep = GetParameterFloat("type.anidif.timestep");
      perBand->GetFilter()->SetTimeStep(static_cast<double>(aniDifTimeStep));

      perBand->GetFilter()->SetConductanceParameter(GetParameterFloat("type.anidif.conductance"));
      perBand->GetFilter()->SetUseImageSpacing(false);

      SetParameterOutputImage("out", perBand->GetOutput());
      
      RegisterPipeline();
    }
    break;
    }
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Smoothing)
