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
  typedef otb::LocalRxDetectorFilter<VectorImageType, ImageType> LocalRxDetectorFilterType;
  
private:
  void DoInit() override
  {
    SetName("LocalRxDetection");
    SetDescription(" ");

    // Documentation
    SetDocName("Local Rx Detection");
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

    AddParameter(ParameterType_Int, "irx", "X Internal radius");
    SetParameterDescription("irx", "Internal radius in pixel along the X axis");
    SetDefaultParameterInt("irx", 1);

    AddParameter(ParameterType_Int,  "iry", "Y Internal radius");
    SetParameterDescription("iry","Internal radius in pixel along the Y axis");
    SetDefaultParameterInt("iry", 1);

    AddParameter(ParameterType_Int,  "erx", "X External radius");
    SetParameterDescription("erx","External radius in pixel");
    SetDefaultParameterInt("erx", 3);

    AddParameter(ParameterType_Int,  "ery", "Y External radius");
    SetParameterDescription("ery","External radius in pixel");
    SetDefaultParameterInt("ery", 3);

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "cupriteSubHsi.tif");
    SetDocExampleParameterValue("out", "LocalRxScore.tif");
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
    auto detector = LocalRxDetectorFilterType::New();

    detector->SetInput(inputImage);
    
    // the radius are the same along x and y for the filter
    unsigned int externalRadius = GetParameterInt("erx");
    unsigned int internalRadius = GetParameterInt("irx");

    detector->SetInternalRadius(internalRadius);
    detector->SetExternalRadius(externalRadius);
    //detector->Update();

    SetParameterOutputImage("out", detector->GetOutput());
    RegisterPipeline();


    #else

    localRxDetectionFunctor<double> detectorFunctor;
    detectorFunctor.SetInternalRadius(GetParameterInt("irx"), GetParameterInt("iry"));

    auto localRxDetectionFunctorFilter = otb::NewFunctorFilter
        (detectorFunctor ,{{GetParameterInt("erx"),GetParameterInt("ery")}});

    localRxDetectionFunctorFilter->SetVariadicInputs(inputImage);

    SetParameterOutputImage("out", localRxDetectionFunctorFilter->GetOutput());
    RegisterPipeline();
    #endif
  }

};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::LocalRxDetection)
