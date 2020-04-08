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

#include "otbMultiToMonoChannelExtractROI.h"

namespace otb
{

namespace Wrapper
{

class SplitImage : public Application
{

public:
  /** Standard class typedefs. */
  typedef SplitImage                    Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(SplitImage, Application);

  /** Filters typedef */
  typedef otb::MultiToMonoChannelExtractROI<FloatVectorImageType::InternalPixelType, FloatVectorImageType::InternalPixelType> FilterType;

private:
  void DoInit() override
  {
    SetName("SplitImage");
    SetDescription("Split a N multiband image into N images.");

    SetDocLongDescription(
        "This application splits a N-bands image into N mono-band images. "
        "The output images filename will be generated from the output parameter. "
        "Thus, if the input image has 2 channels, and the user has set as output parameter, outimage.tif, "
        "the generated images will be outimage_0.tif and outimage_1.tif.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("ConcatenateImages");

    AddDocTag(Tags::Manip);

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "Input multiband image filename.");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out",
                            "The output filename will be used to get the prefix "
                            "an the extension of the output written's image. For example with outimage.tif as output filename, "
                            "the generated images will had an indice (corresponding at each bands) "
                            "between the prefix and the extension, such as: outimage_0.tif  and outimage_1.tif (if 2 bands).");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "VegetationIndex.hd");
    SetDocExampleParameterValue("out", "splitImage.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here for the parameters : all are independent
  }

  void DoExecute() override
  {
    // Get the input image
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");

    // Get the path/fileWithoutextension/extension of the output images filename
    std::string path, fname, ext;
    std::string ofname = GetParameterString("out");

    // Get the extension and the prefix of the filename
    path  = itksys::SystemTools::GetFilenamePath(ofname);
    fname = itksys::SystemTools::GetFilenameWithoutExtension(ofname);
    ext   = itksys::SystemTools::GetFilenameExtension(ofname);

    // Set the extract filter input image
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(inImage);

    for (unsigned int i = 0; i < inImage->GetNumberOfComponentsPerPixel(); ++i)
    {
      // Set the channel to extract
      filter->SetChannel(i + 1);

      // build the current output filename
      std::ostringstream oss;
      if (!path.empty())
      {
        oss << path << "/";
      }
      oss << fname << "_" << i << ext;

      // Create an output parameter to write the current output image
      OutputImageParameter::Pointer paramOut = OutputImageParameter::New();

      // writer label
      std::ostringstream osswriter;
      osswriter << "writer (Channel : " << i << ")";

      // Set the filename of the current output image
      paramOut->SetFileName(oss.str());
      otbAppLogINFO(<< "File: " << paramOut->GetFileName() << " will be written.");
      paramOut->SetValue(filter->GetOutput());
      paramOut->SetPixelType(this->GetParameterOutputImagePixelType("out"));
      // Add the current level to be written
      paramOut->InitializeWriters();
      AddProcess(paramOut->GetWriter(), osswriter.str());
      paramOut->Write();
    }

    // Disable the output Image parameter to avoid writing
    // the last image (Application::ExecuteAndWriteOutput method)
    DisableParameter("out");
    RegisterPipeline();
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::SplitImage)
