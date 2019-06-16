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

#include "otbVectorDataTransformFilter.h"
#include "otbVectorDataProjectionFilter.h"
#include "itkCenteredSimilarity2DTransform.h"
#include "otbMath.h"

namespace otb
{

namespace Wrapper
{

class VectorDataTransform : public Application
{
public:
/** Standard class typedefs. */
  typedef VectorDataTransform           Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(VectorDataTransform, otb::Application);

  /** Convenient typedefs */
  typedef otb::VectorDataTransformFilter<VectorDataType,
                                         VectorDataType>   VectorDataTransformFilterType;
  typedef otb::VectorDataProjectionFilter<VectorDataType,
                                          VectorDataType>  VectorDataProjectionFilterType;
  typedef itk::CenteredSimilarity2DTransform<double>       TransformType;

private:
  void DoInit() override
  {
    SetName("VectorDataTransform");
    SetDescription("Apply a transform to each vertex of the input VectorData");

    SetDocLongDescription("This application iterates over each vertex in the "
      "input vector data file and performs a transformation on this vertex.\n\n"
      "It is the equivalent of [1] that transforms images. For instance, if you"
      " extract the envelope of an image with [2], and you transform this image"
      " with [1], you may want to use this application to operate the same "
      "transform on the envelope.\n\n"
      "The applied transformation is a 2D similarity. It manages translation, "
      "rotation, scaling, and can be centered or not. Note that the support "
      "image is used to define the reference coordinate system in which the "
      "transform is applied. For instance the input vector data can have WGS84"
      " coordinates, the support image is in UTM, so a translation of 1 pixel "
      "along X corresponds to the X pixel size of the input image along the "
      "X axis of the UTM coordinates frame. This image can also be in sensor "
      "geometry.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("[1] RigidTransformResample\n"
      "[2] ImageEnvelope");

    AddDocTag(Tags::Vector);

    AddParameter(ParameterType_InputVectorData, "vd", "Input Vector data");
    SetParameterDescription("vd", "Input vector data file to transform");

    AddParameter(ParameterType_OutputVectorData,"out","Output Vector data");
    SetParameterDescription("out", "Output vector data with ");

    AddParameter(ParameterType_InputImage, "in", "Support image");
    SetParameterDescription("in","Image defining the reference coordinate "
      "system in which the transform is applied. Both projected and sensor "
      "images are supported.");

    // Transform Group
    AddParameter(ParameterType_Group, "transform", "Transform parameters");
    SetParameterDescription("transform", "Group of parameters to define the transform");

    AddParameter(ParameterType_Float, "transform.tx", "X Translation");
    SetParameterDescription("transform.tx","Translation in the X direction (in pixels)");
    AddParameter(ParameterType_Float, "transform.ty", "Y Translation");
    SetParameterDescription("transform.ty","Translation in the Y direction (in pixels)");
    SetDefaultParameterFloat("transform.tx", 0.);
    SetDefaultParameterFloat("transform.ty", 0.);

    AddParameter(ParameterType_Float, "transform.ro", "Rotation Angle");
    SetParameterDescription("transform.ro","Angle of the rotation (in degrees)");
    SetDefaultParameterFloat("transform.ro", 0.);

    AddParameter(ParameterType_Float, "transform.centerx", "Center X");
    SetParameterDescription("transform.centerx","X coordinate of the rotation "
      "and scaling center (in physical units)");
    AddParameter(ParameterType_Float, "transform.centery", "Center Y");
    SetParameterDescription("transform.centery","Y coordinate of the rotation "
      "and scaling center (in physical units)");
    SetDefaultParameterFloat("transform.centerx", 0.);
    SetDefaultParameterFloat("transform.centery", 0.);

    AddParameter(ParameterType_Float, "transform.scale", "Scale");
    SetParameterDescription("transform.scale","The scale coefficient to apply");
    SetDefaultParameterFloat("transform.scale", 1.);

    // Doc example parameter settings
    SetDocExampleParameterValue("vd", "qb_RoadExtract_easyClassification.shp");
    SetDocExampleParameterValue("in", "qb_RoadExtract.tif");
    SetDocExampleParameterValue("out", "VectorDataTransform.shp");
    SetDocExampleParameterValue("transform.ro", "5");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // nothing to update
  }

  void DoExecute() override
  {
    // Get the support image
    FloatVectorImageType*  inImage = GetParameterImage("in");

    // Get the VectorData to apply the transform on
    VectorDataType*        vd      = GetParameterVectorData("vd");

    // Reproject the VectorData in the image coordinate system
    m_VectorDataProj = VectorDataProjectionFilterType::New();
    m_VectorDataProj->SetInput(vd);
    m_VectorDataProj->SetInputProjectionRef(vd->GetProjectionRef());
    m_VectorDataProj->SetOutputKeywordList(inImage->GetImageKeywordlist());
    m_VectorDataProj->SetOutputProjectionRef(inImage->GetProjectionRef());

    // Set up the transform
    m_Transform = TransformType::New();
    TransformType::ParametersType parameters(6);

    // Get parameters if any
    parameters[0] = GetParameterFloat("transform.scale");
    parameters[1] = CONST_PI * GetParameterFloat("transform.ro")/180.;
    parameters[2] = GetParameterFloat("transform.centerx");
    parameters[3] = GetParameterFloat("transform.centery");
    parameters[4] = inImage->GetSignedSpacing()[0] * GetParameterFloat("transform.tx");
    parameters[5] = std::abs(inImage->GetSignedSpacing()[1]) * GetParameterFloat("transform.ty");

    // Set the parameters to the transform
    m_Transform->SetParameters(parameters);

    m_TransformFilter = VectorDataTransformFilterType::New();
    m_TransformFilter->SetInput(m_VectorDataProj->GetOutput());
    m_TransformFilter->SetTransform(m_Transform);

    // retransform int the input vector projection
    m_ReverseVectorDataProj = VectorDataProjectionFilterType::New();
    m_ReverseVectorDataProj->SetInput(m_TransformFilter->GetOutput());
    m_ReverseVectorDataProj->SetOutputProjectionRef(vd->GetProjectionRef());
    m_ReverseVectorDataProj->SetInputKeywordList(inImage->GetImageKeywordlist());
    m_ReverseVectorDataProj->SetInputProjectionRef(inImage->GetProjectionRef());

    // Set the output image
    SetParameterOutputVectorData("out", m_ReverseVectorDataProj->GetOutput());
  }

  VectorDataTransformFilterType::Pointer   m_TransformFilter;
  VectorDataProjectionFilterType::Pointer  m_VectorDataProj;
  VectorDataProjectionFilterType::Pointer  m_ReverseVectorDataProj;
  TransformType::Pointer                   m_Transform;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::VectorDataTransform)
