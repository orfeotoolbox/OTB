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





namespace otb
{
namespace Wrapper
{

class ImageConnectedComponentSegmentation : public Application
{
public:
  /** Standard class typedefs. */
  typedef ImageConnectedComponentSegmentation                       Self;
  typedef Application                         Superclass;
  typedef itk::SmartPointer<Self>             Pointer;
  typedef itk::SmartPointer<const Self>       ConstPointer;
  
  //typedef itk::BinaryShapeM_BinaryOpeningFilterImageFilter<UInt8LabelImageType>    BinaryM_BinaryOpeningFilterFilterType;
  typedef otb::Image<unsigned int, 2>                                 LabelImageType;
  typedef otb::Image<unsigned int, 2>         MaskImageType;

  typedef float InputPixelType;
  typedef otb::VectorImage<InputPixelType, 2> VectorImageType;
  typedef VectorImageType::PixelType VectorImagePixelType;
  
  
  
  typedef Functor::ConnectedComponentMuParserFunctor<VectorImagePixelType> FunctorType;
  typedef itk::ConnectedComponentFunctorImageFilter<
      VectorImageType,
      LabelImageType,
      FunctorType,
      MaskImageType > ConnectedComponentFilterType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ImageConnectedComponentSegmentation, otb::Application);

private:
  void DoInit() ITK_OVERRIDE
  {
    SetName("ImageConnectedComponentSegmentation");
    SetDescription("This application performs the vectorization of an input binary mask");

    // Documentation
    SetDocName("Mask Filtering");
    SetDocLongDescription("Given an input binary raster image, this application will output a vector data file containing a polygon for each connected component of the input raster. Additionnaly, the size of each polygon will be computed and added to the raster");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    
    
    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input image.");
    
    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out","Output vector");

    AddParameter(ParameterType_String, "expr", "Connected Component Expression");
    SetParameterDescription("expr", "Formula used for connected component segmentation");
    
    
    
    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("expr", "\"distance<10\"");
    SetDocExampleParameterValue("in", "sar.tif");
    SetDocExampleParameterValue("out","test.sqlite");
    SetOfficialDocLink();
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() ITK_OVERRIDE
  {
    clock_t tic = clock();
    
    ConnectedComponentFilterType::Pointer connected = ConnectedComponentFilterType::New();
    connected->SetInput(this->GetParameterImage("in"));
    connected->GetFunctor().SetExpression(GetParameterString("expr"));
    
    SetParameterOutputImage<LabelImageType>("out", connected->GetOutput());
    connected->Update();
    clock_t toc = clock();
    otbAppLogINFO( "Elapsed: "<< ((double)(toc - tic) / CLOCKS_PER_SEC)<<" seconds.");
  }
  
  

}; 

} //end namespace Wrapper
} //end namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::ImageConnectedComponentSegmentation)
