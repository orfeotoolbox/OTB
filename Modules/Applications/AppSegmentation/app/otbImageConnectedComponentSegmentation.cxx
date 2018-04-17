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

#include "itkConnectedComponentFunctorImageFilter.h"
#include "otbConnectedComponentMuParserFunctor.h"

#include "otbMaskMuParserFilter.h"

namespace otb
{
namespace Wrapper
{

class ImageConnectedComponentSegmentation : public Application
{
public:
  /** Standard class typedefs. */
  typedef ImageConnectedComponentSegmentation Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;
  
  typedef otb::Image<unsigned int, 2>         LabelImageType;
  typedef otb::Image<unsigned int, 2>         MaskImageType;

  typedef float InputPixelType;
  typedef otb::VectorImage<InputPixelType, 2> VectorImageType;
  typedef VectorImageType::PixelType VectorImagePixelType;
  
  // mask filter typedef
  typedef otb::MaskMuParserFilter<VectorImageType, MaskImageType> 
                                                  MaskMuParserFilterType;

  // connected components filter typedef
  typedef Functor::ConnectedComponentMuParserFunctor<VectorImagePixelType> 
                                                  FunctorType;
  typedef itk::ConnectedComponentFunctorImageFilter<
      VectorImageType,
      LabelImageType,
      FunctorType,
      MaskImageType > ConnectedComponentFilterType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ImageConnectedComponentSegmentation, otb::Application);

private:
  void DoInit() override
  {
    SetName("ImageConnectedComponentSegmentation");
    SetDescription("This application performs connected component "
      "segmentation using an user criterion for connected components");

    // Documentation
    SetDocName("ImageConnectedComponentSegmentation");
    SetDocLongDescription("This application performs connected component "
      "segmentation using an user criterion for connected components");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    
    
    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input multi-band image.");
    
    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out","Output label image");

    AddParameter(ParameterType_String, "expr", 
      "Connected Component Expression");
    SetParameterDescription("expr", 
      "Formula used for connected component segmentation");
    
    AddParameter(ParameterType_String, "mask", "Mask expression");
    SetParameterDescription("mask", 
      "Mask mathematical expression (only if support image is given)");
    MandatoryOff("mask");
    
    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("expr", "\"distance<10\"");
    SetDocExampleParameterValue("in", "sar.tif");
    SetDocExampleParameterValue("out","test.sqlite");
    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
  {
    typename MaskImageType::Pointer mask;
    typename MaskMuParserFilterType::Pointer maskFilter;
    maskFilter = MaskMuParserFilterType::New();
    ConnectedComponentFilterType::Pointer connected = 
      ConnectedComponentFilterType::New();
    if (IsParameterEnabled("mask") && HasValue("mask"))
    {
      // Compute the mask
      maskFilter->SetInput(this->GetParameterImage("in"));
      maskFilter->SetExpression(this->GetParameterString("mask"));
      maskFilter->UpdateOutputInformation();
      connected->SetMaskImage(maskFilter->GetOutput());
    }
    
    connected->SetInput(this->GetParameterImage("in"));
    connected->GetFunctor().SetExpression(GetParameterString("expr"));

    SetParameterOutputImage<LabelImageType>("out", connected->GetOutput());
    RegisterPipeline();
  }
  
}; 

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ImageConnectedComponentSegmentation)
