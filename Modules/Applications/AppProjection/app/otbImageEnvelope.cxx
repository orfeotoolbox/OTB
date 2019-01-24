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

#include "otbWrapperApplicationFactory.h"

#include "otbImageToEnvelopeVectorDataFilter.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{
namespace Wrapper
{

class ImageEnvelope : public Application
{
public:
  /** Standard class typedefs. */
  typedef ImageEnvelope                 Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ImageEnvelope, otb::Application);

  /** Filters typedef */
  typedef otb::ImageToEnvelopeVectorDataFilter
      <FloatVectorImageType, VectorDataType>          EnvelopeFilterType;

private:
  void DoInit() override
  {
    SetName("ImageEnvelope");
    SetDescription("Extracts an image envelope.");

    // Documentation
    SetDocName("Image Envelope");
    SetDocLongDescription("Build a vector data containing the image envelope polygon. "
      "Useful for some projection, you can set the polygon with more points with the sr parameter. "
      "This filter supports user-specified output projection. "
      "If no projection is defined, the standard WGS84 projection will be used.");

    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Geometry);

    AddParameter(ParameterType_InputImage, "in", "Input Image");
    SetParameterDescription("in", "Input image filename.");

    AddParameter(ParameterType_OutputVectorData, "out", "Output Vector Data");
    SetParameterDescription("out", "Vector data file containing the envelope.");

    AddParameter(ParameterType_Int, "sr", "Sampling Rate");
    SetParameterDescription("sr", "Sampling rate for image edges (in pixel).");
    SetDefaultParameterInt("sr",0);
    MandatoryOff("sr");
    DisableParameter("sr");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    AddParameter(ParameterType_String, "proj",  "Projection");
    SetParameterDescription("proj", "Projection to be used to compute the envelope (default is WGS84)");
    MandatoryOff("proj");

   // Doc example parameter settings
    SetDocExampleParameterValue("in", "QB_TOULOUSE_MUL_Extract_500_500.tif");
    SetDocExampleParameterValue("out", "ImageEnvelope.shp");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to be done
  }

  void DoExecute() override
  {
    FloatVectorImageType::Pointer input = GetParameterImage("in");

    m_Envelope = EnvelopeFilterType::New();
    m_Envelope->SetInput(input);
    m_Envelope->SetSamplingRate(GetParameterInt("sr"));

    // Setup the DEM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");
    if (HasValue("proj"))
      {
      m_Envelope->SetOutputProjectionRef(GetParameterString("proj"));
      }

    SetParameterOutputVectorData("out", m_Envelope->GetOutput());
  }

  EnvelopeFilterType::Pointer m_Envelope;

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ImageEnvelope)
