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

#include "otbVectorDataExtractROI.h"
#include "otbVectorDataProperties.h"

//Misc
#include "otbRemoteSensingRegion.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

#include <iostream>

namespace otb
{
namespace Wrapper
{

class VectorDataExtractROI : public Application
{
public:
  /** Standard class typedefs. */
  typedef VectorDataExtractROI  Self;
  typedef Application                      Superclass;
  typedef itk::SmartPointer<Self>          Pointer;
  typedef itk::SmartPointer<const Self>    ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(VectorDataExtractROI, otb::Application);

  /** Filters typedef */
  // Images

  typedef FloatVectorImageType::PointType                 PointType;
  typedef FloatVectorImageType::IndexType                 IndexType;
  typedef FloatVectorImageType::SizeType                  SizeType;
  typedef FloatVectorImageType::SpacingType               SpacingType;

  typedef otb::VectorDataExtractROI<VectorDataType>       VectorDataExtractROIType;

  // Misc
  typedef otb::RemoteSensingRegion<double>                RemoteSensingRegionType;

private:
  void DoInit() override
  {
    SetName("VectorDataExtractROI");
    SetDescription("Perform an extract ROI on the input vector data according to the input image extent");

    SetDocName("VectorData Extract ROI");
    SetDocLongDescription("This application extracts the vector data features"
                          " belonging to a region specified by the support "
                          "image envelope. Any features intersecting the "
                          "support region is copied to output. The output "
                          "geometries are NOT cropped.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

	AddDocTag(Tags::Vector);
    AddDocTag(Tags::Vector);

    AddParameter(ParameterType_Group,"io","Input and output data");
    SetParameterDescription("io", "Group containing input and output parameters");

    AddParameter(ParameterType_InputVectorData, "io.vd", "Input Vector data");
    SetParameterDescription("io.vd", "Input vector data");

    AddParameter(ParameterType_InputImage,      "io.in", "Support image");
    SetParameterDescription("io.in", "Support image that specifies the extracted region");

    AddParameter(ParameterType_OutputVectorData,"io.out","Output Vector data");
    SetParameterDescription("io.out", "Output extracted vector data");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    // Doc example parameter settings
    SetDocExampleParameterValue("io.in", "qb_RoadExtract.tif");
    SetDocExampleParameterValue("io.vd", "qb_RoadExtract_classification.shp");
    SetDocExampleParameterValue("io.out", "apTvUtVectorDataExtractROIApplicationTest.shp");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
    // Nothing to do here for the parameters : all are independent
  }

  void DoExecute() override
  {
    // Get the inputs
    VectorDataType*        vd      = GetParameterVectorData("io.vd");
    FloatVectorImageType*  inImage = GetParameterImage("io.in");

    // Extracting the VectorData
    m_VdExtract = VectorDataExtractROIType::New();
    m_VdExtract->SetInput(vd);

    // Find the geographic region of interest
    // Get the index of the corner of the image
    itk::ContinuousIndex<double,2> ul(inImage->GetLargestPossibleRegion().GetIndex());
    PointType pul, pur, pll, plr;
    ul[0] += -0.5;
    ul[1] += -0.5;
    itk::ContinuousIndex<double,2> ur, ll, lr;
    ur = ul;
    ll = ul;
    lr = ul;
    ur[0] += inImage->GetLargestPossibleRegion().GetSize()[0];
    lr[0] += inImage->GetLargestPossibleRegion().GetSize()[0];
    lr[1] += inImage->GetLargestPossibleRegion().GetSize()[1];
    ll[1] += inImage->GetLargestPossibleRegion().GetSize()[1];

    // Transform to physical point
    inImage->TransformContinuousIndexToPhysicalPoint(ul, pul);
    inImage->TransformContinuousIndexToPhysicalPoint(ur, pur);
    inImage->TransformContinuousIndexToPhysicalPoint(ll, pll);
    inImage->TransformContinuousIndexToPhysicalPoint(lr, plr);

    // Build the cartographic region
    RemoteSensingRegionType            rsRegion;
    RemoteSensingRegionType::IndexType rsOrigin;
    RemoteSensingRegionType::SizeType  rsSize;
    rsOrigin[0] = std::min(pul[0], plr[0]);
    rsOrigin[1] = std::min(pul[1], plr[1]);
    rsSize[0] = std::abs(pul[0] - plr[0]);
    rsSize[1] = std::abs(pul[1] - plr[1]);

    rsRegion.SetOrigin(rsOrigin);
    rsRegion.SetSize(rsSize);
    rsRegion.SetRegionProjection(inImage->GetProjectionRef());
    rsRegion.SetKeywordList(inImage->GetImageKeywordlist());

    // Set the cartographic region to the extract roi filter
    m_VdExtract->SetRegion(rsRegion);

    // Setup the DEM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");

    // Set the output vectorData
    SetParameterOutputVectorData("io.out", m_VdExtract->GetOutput());
  }

  VectorDataExtractROIType::Pointer       m_VdExtract;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::VectorDataExtractROI)
