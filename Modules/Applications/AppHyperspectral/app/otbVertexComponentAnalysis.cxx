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

#include "otbVcaImageFilter.h"

namespace otb
{
namespace Wrapper
{


typedef otb::VCAImageFilter<DoubleVectorImageType>                 VCAFilterType;
typedef otb::VectorImageToMatrixImageFilter<DoubleVectorImageType> VectorImageToMatrixImageFilterType;

class VertexComponentAnalysis : public Application
{
public:
  /** Standard class typedefs. */
  typedef VertexComponentAnalysis         Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(VertexComponentAnalysis, otb::Application);

private:
  void DoInit() ITK_OVERRIDE
  {
    SetName("VertexComponentAnalysis");
    SetDescription("Find endmembers in hyperspectral images with Vertex Component Analysis");

    // Documentation
    SetDocName("Vertex Component Analysis");
    SetDocLongDescription("Applies the Vertex Component Analysis to an hyperspectral image to extract endmembers");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

	AddDocTag("Miscellaneous");
    AddDocTag(Tags::Hyperspectral);
    AddDocTag(Tags::DimensionReduction);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in","Input hyperspectral data cube");

    AddParameter(ParameterType_Int, "ne", "Number of endmembers");
    SetParameterDescription("ne","The number of endmembers to extract from the data cube");
    SetParameterInt("ne",1, false);
    MandatoryOn("ne");

    AddParameter(ParameterType_OutputImage, "outendm", "Output Endmembers");
    SetParameterDescription("outendm","The endmebers, stored in a one-line multi-spectral image, each pixel representing an endmember");
    MandatoryOn("outendm");

    AddRANDParameter();
    // Doc example parameter settings
    SetDocExampleParameterValue("in", "cupriteSubHsi.tif");
    SetDocExampleParameterValue("ne", "5");
    SetDocExampleParameterValue("outendm", "VertexComponentAnalysis.tif double");
  }

  void DoUpdateParameters() ITK_OVERRIDE
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() ITK_OVERRIDE
  {
    DoubleVectorImageType::Pointer inputImage = GetParameterDoubleVectorImage("in");
    DoubleVectorImageType::Pointer endmembersImage;

    const unsigned int nbEndmembers = GetParameterInt("ne");
    VCAFilterType::Pointer vca = VCAFilterType::New();
    vca->SetNumberOfEndmembers(nbEndmembers);
    vca->SetInput(inputImage);

    endmembersImage = vca->GetOutput();
    m_Ref = vca.GetPointer();

    SetParameterOutputImage<DoubleVectorImageType>("outendm", endmembersImage);
  }

  itk::LightObject::Pointer m_Ref;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::VertexComponentAnalysis)
