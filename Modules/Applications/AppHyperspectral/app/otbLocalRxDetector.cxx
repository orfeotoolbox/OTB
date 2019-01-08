/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "otbLocalRxDetectorFilter.h"

namespace otb
{
namespace Wrapper
{

class LocalRxDetector : public Application
{
public:
  /** Standard class typedefs. */
  typedef LocalRxDetector         		Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(LocalRxDetector, otb::Application);

  /** Image typedefs */
  typedef FloatVectorImageType                VectorImageType;
  typedef FloatImageType                      ImageType;

  /** Filter typedefs */
  typedef otb::LocalRxDetectorFilter<VectorImageType, ImageType> LocalRxDetectorFilterType;
  
private:
  void DoInit() override
  {
    SetName("LocalRxDetector");
    SetDescription(" ");

    // Documentation
    SetDocName("Local Rx Detector");
    SetDocLongDescription(" ");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Hyperspectral);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in","Input hyperspectral data cube");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out","Output Rx score image");
    MandatoryOn("out");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "cupriteSubHsi.tif");
    SetDocExampleParameterValue("out", "LocalRxScore.tif");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {

    auto detector = LocalRxDetectorFilterType::New();

    detector->SetInput(GetParameterImage("in"));

    //TODO this should be app parameters
    unsigned int externalRadius = 3;
    unsigned int internalRadius = 1;

    detector->SetInternalRadius(internalRadius);
    detector->SetExternalRadius(externalRadius);

    SetParameterOutputImage("out", detector->GetOutput());

    RegisterPipeline();
  }

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::LocalRxDetector)
