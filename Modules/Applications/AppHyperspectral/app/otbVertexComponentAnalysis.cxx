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
  void DoInit() override
  {
    SetName("VertexComponentAnalysis");
    SetDescription("Given a set of mixed spectral vectors, estimate "
    "reference substances also known as endmembers using the Vertex "
    "Component Analysis algorithm.");

    // Documentation
    SetDocLongDescription("Apply the Vertex Component Analysis [1] to "
    "an hyperspectral image to extract endmembers. Given a set of mixed "
    "spectral vectors (multispectral or hyperspectral), the application "
    "estimates the spectral signature of reference substances also known "
    "as endmembers.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("[1] J. M. P. Nascimento and J. M. B. Dias, Vertex "
    "component analysis: a fast algorithm to unmix hyperspectral data, "
    "in IEEE Transactions on Geoscience and Remote Sensing, vol. 43, "
    "no. 4, pp. 898-910, April 2005.J. M. P. Nascimento and "
    "J. M. B. Dias, Vertex component analysis: a fast algorithm to "
    "unmix hyperspectral data, in IEEE Transactions on Geoscience and "
    "Remote Sensing, vol. 43, no. 4, pp. 898-910, April 2005.");

    AddDocTag(Tags::Hyperspectral);
    AddDocTag(Tags::DimensionReduction);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in","Input hyperspectral data cube");

    AddParameter(ParameterType_OutputImage, "outendm", "Output Endmembers");
    SetParameterDescription("outendm","Endmembers, stored in a "
    "one-line multi-spectral image.Each pixel corresponds to one "
    "endmembers and each band values corresponds to the spectral "
    "signature of the corresponding endmember.");
    MandatoryOn("outendm");

    AddParameter(ParameterType_Int, "ne", "Number of endmembers");
    SetParameterDescription("ne", "The number of endmembers to extract from the hyperspectral image.");
    SetParameterInt("ne", 1);
    MandatoryOn("ne");

    AddRANDParameter();
    // Doc example parameter settings
    SetDocExampleParameterValue("in", "cupriteSubHsi.tif");
    SetDocExampleParameterValue("ne", "5");
    SetDocExampleParameterValue("outendm", "VertexComponentAnalysis.tif double");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute() override
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
