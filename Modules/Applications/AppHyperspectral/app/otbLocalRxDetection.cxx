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

    AddParameter(ParameterType_Int, "ir", "Internal radius");
    SetParameterDescription("ir", "Internal radius in pixel");
    SetDefaultParameterInt("ir", 1);

    AddParameter(ParameterType_Int,  "er", "External radius");
    SetParameterDescription("er","External radius in pixel");
    SetDefaultParameterInt("er", 5);

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "cupriteSubHsi.tif");
    SetDocExampleParameterValue("out", "LocalRxScore.tif");
    SetDocExampleParameterValue("ir", "1");
    SetDocExampleParameterValue("er", "5");

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
    
    // The localRxDetectionFilter can be replaced by a functorImageFilter using the appropriate 
    // functor. However using functorImageFilter with neighborhood is buggy (see issue #1802). Still,
    // the functor has been implemented and localRxDetectionFilter will be deprecated when the
    // bug is corrected.
    #if 1 // Using localRxDetectionFilter
    auto localRxDetectionFilter = LocalRxDetectorFilterType::New();

    localRxDetectionFilter->SetInput(inputImage);
    
    unsigned int externalRadius = GetParameterInt("er");
    unsigned int internalRadius = GetParameterInt("ir");

    localRxDetectionFilter->SetInternalRadius(internalRadius);
    localRxDetectionFilter->SetExternalRadius(externalRadius);

    #else // Using a functorImageFilter
    Functor::LocalRxDetectionFunctor<double> detectorFunctor;
    detectorFunctor.SetInternalRadius(GetParameterInt("ir"), GetParameterInt("ir"));

    auto localRxDetectionFilter = otb::NewFunctorFilter
        (detectorFunctor ,{{GetParameterInt("er"),GetParameterInt("er")}});

    localRxDetectionFilter->SetInputs(inputImage);
    #endif

    SetParameterOutputImage("out", localRxDetectionFilter->GetOutput());
    RegisterPipeline();
  }

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::LocalRxDetection)
