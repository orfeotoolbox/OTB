/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbWrapperApplicationFactory.h"
#include "otbWrapperNumericalParameter.h"
#include "otbGeometriesProjectionFilter.h"
#include "otbGeometriesSet.h"


// MapProjection handler
#include "otbWrapperMapProjectionParametersHandler.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{


namespace Wrapper
{

class VectorDataReprojection : public Application
{
public:
  /** Standard class typedefs. */
  typedef VectorDataReprojection        Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** filter typedef  **/
  typedef otb::GeometriesProjectionFilter ProjectionFilterType;

  typedef otb::GeometriesSet InputGeometriesType;
  typedef otb::GeometriesSet OutputGeometriesType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(VectorDataReprojection, otb::Application);

private:
  void DoInit() override
  {
    SetName("VectorDataReprojection");
    SetDescription("Reproject a vector data using support image projection reference, or a user specified map projection");

    SetDocLongDescription(
        "Reproject vector data using a support image as projection reference or to a user given map projection. "
        " If given an image keywordlist can be added to the reprojected vectordata.");

    SetDocLimitations(" ");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Vector);
    AddDocTag(Tags::Geometry);
    AddDocTag(Tags::Coordinates);

    // Set the parameters

    AddParameter(ParameterType_Group, "in", "Input data");
    AddParameter(ParameterType_InputFilename, "in.vd", "Input vector data");
    SetParameterDescription("in.vd", "The input vector data to reproject");
    AddParameter(ParameterType_InputImage, "in.kwl", "Use image keywords list");
    SetParameterDescription("in.kwl", "Optional input image to fill vector data with image kwl.");
    MandatoryOff("in.kwl");
    DisableParameter("in.kwl");

    AddParameter(ParameterType_Group, "out", "Output data");
    AddParameter(ParameterType_OutputFilename, "out.vd", "Output vector data");
    SetParameterDescription("out.vd", "The reprojected vector data");

    // output projection choice
    AddParameter(ParameterType_Choice, "out.proj", "Output Projection choice");
    AddChoice("out.proj.image", "Use image projection ref");
    SetParameterDescription("out.proj.image", "Vector data will be reprojected in image projection ref.");
    AddParameter(ParameterType_InputImage, "out.proj.image.in", "Image used to get projection map");
    SetParameterDescription("out.proj.image.in", "Projection map will be found using image metadata");
    AddChoice("out.proj.user", "User defined projection");
    MapProjectionParametersHandler::AddMapProjectionParameters(this, "out.proj.user.map");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    // Doc example parameter settings
    SetDocExampleParameterValue("in.vd", "VectorData_QB1.shp");
    SetDocExampleParameterValue("out.proj", "image");
    SetDocExampleParameterValue("out.proj.image.in", "ROI_QB_MUL_1.tif");
    SetDocExampleParameterValue("out.vd", "reprojected_vd.shp");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
  }

  void DoExecute() override
  {
    GetLogger()->Debug("Entering DoExecute\n");

    // Setup the DEM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this, "elev");

    // Get the input image

    otb::ogr::DataSource::Pointer OGRDSin = otb::ogr::DataSource::New(GetParameterString("in.vd"), otb::ogr::DataSource::Modes::Read);

    m_InputGeomSet = InputGeometriesType::New(OGRDSin);

    // Filter instantiation
    m_GeometriesProjFilter = ProjectionFilterType::New();
    m_GeometriesProjFilter->SetInput(m_InputGeomSet);

    if (HasValue("in.kwl"))
    {
      FloatVectorImageType::Pointer inImage = GetParameterFloatVectorImage("in.kwl");
      m_GeometriesProjFilter->SetInputImageMetadata(&(inImage->GetImageMetadata()));
      // otbAppLogINFO(<<"kwl."<<std::endl);
    }

    if (GetParameterInt("out.proj") == 0)
    {
      FloatVectorImageType::Pointer outImage = GetParameterFloatVectorImage("out.proj.image.in");

      if (outImage)
      {
        m_OutputProjectionRef = outImage->GetProjectionRef(); // ~ wkt
        if (m_OutputProjectionRef.empty())
        {
          m_GeometriesProjFilter->SetOutputImageMetadata(&(outImage->GetImageMetadata()));
        }
      }
      else
      {
        // LogFATAl
      }
    }
    else
    {

      // Get the output projection Ref
      m_OutputProjectionRef = MapProjectionParametersHandler::GetProjectionRefFromChoice(this, "out.proj.user.map");
    }
    otbAppLogINFO(<< "Geometries reprojection with following projection reference :" << std::endl << m_OutputProjectionRef << std::endl);

    m_GeometriesProjFilter->SetOutputProjectionRef(m_OutputProjectionRef);

    OGRDSout = otb::ogr::DataSource::New(GetParameterString("out.vd"), otb::ogr::DataSource::Modes::Update_LayerOverwrite);

    m_OutputGeomSet = OutputGeometriesType::New(OGRDSout);

    m_GeometriesProjFilter->SetOutput(m_OutputGeomSet);
    m_GeometriesProjFilter->Update();
    OGRDSout->SyncToDisk();
  }

  otb::ogr::DataSource::Pointer OGRDSout;
  std::string                   m_OutputProjectionRef;
  ImageKeywordlist              m_KeywordList;
  InputGeometriesType::Pointer  m_InputGeomSet;
  OutputGeometriesType::Pointer m_OutputGeomSet;
  // FloatVectorImageType::Pointer m_inImage;
  // FloatVectorImageType::Pointer m_outImage;
  ProjectionFilterType::Pointer m_GeometriesProjFilter;
};

} // namespace Wrapper
} // namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::VectorDataReprojection)
