/*
 * Copyright(C) 2005-2020 Centre National d'Etudes Spatiales(CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https: //www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0(the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http: //www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "otbSynthetizeFilter.h"
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbImageFileReader.h"
#include <set>

namespace otb
{
namespace Wrapper
{

/**
 * This application synthetizes/reduces multiple inputs into a single one.
 * In that particular case, for each output pixel, this application will
 * consider the corresponding pixels from all the input images, and keep the
 * first one that isn't equal to 0.
 *
 * This application is used to implement the _concatenate_ processing in
 * S1Tiling chain.
 *
 * \author Luc Hermitte (CS Group)
 * \copyright CNES
 * \todo find a better name for the application. Alas `otbConcatenate` is
 * already used...
 */
class Synthetize : public Application
{
public:
  using Self    = Synthetize;
  using Pointer = itk::SmartPointer<Self>;

  itkNewMacro(Self);
  itkTypeMacro(Synthetize, otb::Wrapper::Application);

private:
  using ReaderType = otb::ImageFileReader<FloatImageType>;

  void DoInit() override
  {
    SetName("Synthetize");

    SetDescription("This is the Synthetize application, version X.X.X");
    SetDocLongDescription("Concatenate a list of images of the same size into a single single-channel image.\n\
        It keeps the first non-null pixel value found in the input list.");

    SetDocLimitations("This application will break incoming pipelines.");
    SetDocAuthors("Luc Hermitte (CS Group)");
    SetDocSeeAlso("");
    AddDocTag("otb::Wrapper::Tags::Manip");

    AddParameter(ParameterType_StringList,  "il",  "Input images list");
    SetParameterDescription("il", "Input image list");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out","Output image.");

    AddRAMParameter();
  }

  void DoUpdateParameters() override
  {}

  void DoExecute() override
  {
    // Get the input image list
    auto inNameList = GetParameterStringList("il");

    // checking the input images list validity
    auto const nbImages = inNameList.size();

    if (nbImages == 0)
    {
      itkExceptionMacro("No input Image set...; please set at least one input image");
    }

    auto functor = [](auto input) {
      assert(!input.empty());
      auto const wh = std::find_if(
          input.begin(), input.end()-1,
          [](auto v){ return v != 0;});
      return *wh;
    };
    auto filter = MakeSynthetizeFilter<FloatImageType, FloatImageType>(functor);

    for (unsigned int i = 0; i < nbImages; i++)
    {
      // Given the explicit use of a Reader, this application cannot be used in
      // a in-memory pipeline
      auto reader = ReaderType::New();
      // currentImage->SetExtendedFileName(inNameList[i]);
      reader->SetFileName(inNameList[i]);
      auto currentImage = reader->GetOutput();
      currentImage->UpdateOutputInformation();

      otbAppLogINFO(<< "Image #" << i + 1 << " has " << currentImage->GetNumberOfComponentsPerPixel() << " components");

      filter->SetInput(i, currentImage);
      m_Cache.insert(reader);
    }

    SetParameterOutputImage("out", filter->GetOutput());
    RegisterPipeline(); // TODO: check!!
  }

  // Needed to register the inputs handled manually
  // and not with a VectorImageList through GetParameterImageList
  std::set<ReaderType::Pointer> m_Cache;
};

} // otb::Wrapper namespace
} // otb namespace

OTB_APPLICATION_EXPORT(otb::Wrapper::Synthetize)
