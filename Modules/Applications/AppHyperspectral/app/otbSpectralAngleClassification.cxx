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

#include "otbSpectralAngleFunctor.h"
#include "otbFunctorImageFilter.h"
#include "otbSpectralInformationDivergenceFunctor.h"

namespace otb
{
namespace Wrapper
{

class SpectralAngleClassification : public Application
{
public:
  /** Standard class typedefs. */
  using Self = SpectralAngleClassification;
  using Superclass = Application;
  using Pointer = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SpectralAngleClassification, otb::Application);

  using ValueType = float;
  using ImageType = otb::VectorImage<ValueType>;
  using PixelType = ImageType::PixelType;
  using SAMFilterType = otb::FunctorImageFilter<otb::Functor::SpectralAngleMapperFunctor<PixelType, PixelType, PixelType>>;
  using SIDFilterType = otb::FunctorImageFilter<otb::Functor::SpectralInformationDivergenceFunctor<PixelType, PixelType, PixelType>>;

private:
  void DoInit() override
  {
    SetName("SpectralAngleClassification");
    SetDescription("Classifies an image using a spectral angle measure.");

    // Documentation
    SetDocLongDescription("TODO");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("VertexComponentAnalysis \n"
      "Du, Yingzi & Chang, Chein-I & Ren, Hsuan & Chang, Chein-Chi & Jensen, James & D'Amico, Francis. (2004). "
      "New Hyperspectral Discrimination Measure for Spectral Characterization. Optical Engineering - OPT ENG. 43."
      " 1777-1786. 10.1117/1.1766301. ");

    AddDocTag(Tags::Hyperspectral);

    AddParameter(ParameterType_InputImage, "in", "Input Image Filename");
    SetParameterDescription("in", "The hyperspectral data cube input");

    AddParameter(ParameterType_InputImage, "ie", "Input endmembers");
    SetParameterDescription("ie",
                            "The endmembers (estimated pure pixels) to "
                            "use for unmixing. Must be stored as a multispectral image, where "
                            "each pixel is interpreted as an endmember.");

    AddParameter(ParameterType_OutputImage, "measure", "Output spectral angle values");
    SetParameterDescription("measure",
                            "Output image containing for each pixel from the input image the computed measure relative to each endmember");
    MandatoryOff("measure");
    
    AddParameter(ParameterType_OutputImage, "out", "Output classified image");
    SetParameterDescription("out",
                            "Output classified image.");

    MandatoryOff("out");
    
    AddParameter(ParameterType_Choice, "mode", "Measure used for classification");
    SetParameterDescription("mode", "Measure used for classification");
    MandatoryOff("mode");
    
    AddChoice("mode.sam", "Spectral angle mapper");
    SetParameterDescription("mode.sam", "Spectral angle mapper (SAM) measure.");

    AddChoice("mode.sid", "Spectral information divergence");
    SetParameterDescription("mode.sid", "Spectral information divergence (SID) measure.");


    AddParameter(ParameterType_Float, "threshold", "Classification threshold");
    SetParameterDescription("threshold",
                            "Pixel with a measurement greater than this threshold relatively to "
                            "a reference pixel are not classified. The same threshold is used for all classes.");
    MandatoryOff("threshold");

    AddParameter(ParameterType_Int, "bv", "Background value");
    SetParameterDescription("bv",
                            "Value of unclassified pixels in the classification image "
                            "(this parameter is only used if threshold is set).");
    MandatoryOff("bv");
    SetDefaultParameterInt("bv", -1);

    AddRAMParameter();
    SetMultiWriting(true);

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "cupriteSubHsi.tif");
    SetDocExampleParameterValue("ie", "cupriteEndmembers.tif");
    SetDocExampleParameterValue("out", "classification.tif");
    SetDocExampleParameterValue("measure", "measure.tif");
    SetDocExampleParameterValue("mode", "sam");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {
    auto inputEndmemberImage = GetParameterImage("ie");
    inputEndmemberImage->Update();
    itk::ImageRegionConstIterator<ImageType> it(inputEndmemberImage, inputEndmemberImage->GetLargestPossibleRegion());
    std::vector<PixelType> endmembers;
    
    for (it.GoToBegin(); !it.IsAtEnd(); ++it)
    {
      endmembers.push_back(it.Get());
    }
    
    // Process objects to keep references on the actual filter instanciated below and its output
    itk::LightObject::Pointer filter;
    ImageType::Pointer filterOutput;
    
    auto mode = GetParameterString("mode");
    if (mode == "sam")
    {
      auto SAMFilter = SAMFilterType::New();
      
      SAMFilter->GetModifiableFunctor().SetReferencePixels(endmembers);
      SAMFilter->SetInput(GetParameterImage("in"));
      filter = SAMFilter;
      filterOutput = SAMFilter->GetOutput();
    }
    else if (mode == "sid")
    {
      auto SIDFilter = SIDFilterType::New();
      
      SIDFilter->GetModifiableFunctor().SetReferencePixels(endmembers);
      SIDFilter->SetInput(GetParameterImage("in"));
      filter = SIDFilter;
      filterOutput = SIDFilter->GetOutput();
    }

    if (HasValue("measure"))
    {
      SetParameterOutputImage("measure", filterOutput);
    }

    if (HasValue("out"))
    {
      auto threshold = HasValue("threshold") ? GetParameterFloat("threshold") 
                                              : std::numeric_limits<ValueType>::max();
      auto bv = GetParameterInt("bv");
      
      // This lambda return the index of the minimum value in a pixel, values above threshold are not classified.
      auto minIndexLambda = [threshold, bv](PixelType const & pixel)
      {
        auto min = threshold;
        int res = bv;
        
        for (unsigned int i = 0; i < pixel.Size(); i++)
        {
          if (pixel[i] < min)
          {
            min = pixel[i];
            res = i;
          }
        }
        return res;
      };
      
      auto classificationFilter = NewFunctorFilter(minIndexLambda);
      classificationFilter->SetInput(filterOutput);
      
      SetParameterOutputImage("out", classificationFilter->GetOutput());
      RegisterPipeline();
    } 
    else
    {
      RegisterPipeline();
    }
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::SpectralAngleClassification)
