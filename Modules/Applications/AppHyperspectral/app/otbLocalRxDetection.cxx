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
#include "otbFunctorImageFilter.h"

namespace otb
{
namespace Wrapper
{

class LocalRxDetection : public Application
{
public:
  /** Standard class typedefs. */
  typedef LocalRxDetection              Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(LocalRxDetection, otb::Application);

  /** Image typedefs */
  typedef DoubleVectorImageType                VectorImageType;
  typedef DoubleImageType                      ImageType;

  /** Filter typedefs */
  typedef otb::LocalRxDetectorFilter<VectorImageType, ImageType> LocalRxDetectorFilterType; //TODO remove this
 
private:
  void DoInit() override
  {
    SetName("LocalRxDetection");
    SetDescription("Performs local Rx score computation on an hyperspectral image.");

    // Documentation
    SetDocName("Local Rx Detection");
    SetDocLongDescription("Performs local Rx score computation on an input "
      "hyperspectral image. For each hyperspectral pixel, the Rx score is "
      "computed using statistics computed on a dual neighborhood. The dual "
      "neighborhood is composed of all pixel that are in between two radiuses "
      "around the center pixel. This score can then be used to detect "
      "anomalies in the image, this can be done for example by thresholding "
      "the result of this application with the BandMath application.");
    
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("BandMath");

    AddDocTag(Tags::Hyperspectral);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in","Input hyperspectral data cube");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out","Output Rx score image");
    MandatoryOff("out");

    AddParameter(ParameterType_Int, "irx", "X Internal radius");
    SetParameterDescription("irx", "Internal radius in pixel along the X axis");
    SetDefaultParameterInt("irx", 1);

    AddParameter(ParameterType_Int,  "iry", "Y Internal radius");
    SetParameterDescription("iry","Internal radius in pixel along the Y axis");
    SetDefaultParameterInt("iry", 1);

    AddParameter(ParameterType_Int,  "erx", "X External radius");
    SetParameterDescription("erx","External radius in pixel along the X axis");
    SetDefaultParameterInt("erx", 3);

    AddParameter(ParameterType_Int,  "ery", "Y External radius");
    SetParameterDescription("ery","External radius in pixel along the Y axis");
    SetDefaultParameterInt("ery", 3);

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "cupriteSubHsi.tif");
    SetDocExampleParameterValue("out", "LocalRxScore.tif");
    SetDocExampleParameterValue("threshold", "0.5");
    SetDocExampleParameterValue("irx", "1");
    SetDocExampleParameterValue("iry", "1");
    SetDocExampleParameterValue("erx", "3");
    SetDocExampleParameterValue("ery", "3");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {
    auto inputImage = GetParameterDoubleVectorImage("in");
    inputImage->UpdateOutputInformation();
    
    #if 0
    auto localRxDetectionFilter = LocalRxDetectorFilterType::New();

    localRxDetectionFilter->SetInput(inputImage);
    
    // the radius are the same along x and y for the filter
    unsigned int externalRadius = GetParameterInt("erx");
    unsigned int internalRadius = GetParameterInt("irx");

    localRxDetectionFilter->SetInternalRadius(internalRadius);
    localRxDetectionFilter->SetExternalRadius(externalRadius);

    #else
    localRxDetectionFunctor<double> detectorFunctor;
    detectorFunctor.SetInternalRadius(GetParameterInt("irx"), GetParameterInt("iry"));

    auto localRxDetectionFilter = otb::NewFunctorFilter
        (detectorFunctor ,{{GetParameterInt("erx"),GetParameterInt("ery")}});

    localRxDetectionFilter->SetVariadicInputs(inputImage);
    #endif

    SetParameterOutputImage("out", localRxDetectionFilter->GetOutput());
    RegisterPipeline();
  }

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::LocalRxDetection)
