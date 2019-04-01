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

#include "otbOGRDataToClassStatisticsFilter.h"
#include "otbStatisticsXMLFileWriter.h"
#include "otbGeometriesProjectionFilter.h"
#include "otbGeometriesSet.h"
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{
namespace Wrapper
{

/** Utility function to negate std::isalnum */
bool IsNotAlphaNum(char c)
  {
  return !std::isalnum(c);
  }

class PolygonClassStatistics : public Application
{
public:
  /** Standard class typedefs. */
  typedef PolygonClassStatistics        Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(PolygonClassStatistics, otb::Application);

  /** Filters typedef */
  typedef otb::OGRDataToClassStatisticsFilter<FloatVectorImageType,UInt8ImageType> FilterType;

  typedef otb::StatisticsXMLFileWriter<FloatVectorImageType::PixelType> StatWriterType;

  typedef otb::GeometriesSet GeometriesType;

  typedef otb::GeometriesProjectionFilter ProjectionFilterType;

private:
  PolygonClassStatistics()
    {
    }

  void DoInit() override
  {
    SetName("PolygonClassStatistics");
    SetDescription("Computes statistics on a training polygon set.");

    // Documentation
    SetDocName("Polygon Class Statistics");
    SetDocLongDescription("Process a set of geometries intended for training (they should have a field giving the associated "
      "class). The geometries are analyzed against a support image to compute statistics:\n\n"
      "* Number of samples per class\n"
      "* Number of samples per geometry\n\n"

      "An optional raster mask can be used to discard samples. Different types"
      " of geometry are supported: polygons, lines, points. The behaviour is "
      "different for each type of geometry:\n\n"
      "* Polygon: select pixels whose center is inside the polygon\n"
      "* Lines: select pixels intersecting the line\n"
      "* Points: select closest pixel to the point");

    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Learning);

    AddParameter(ParameterType_InputImage,  "in",   "Input image");
    SetParameterDescription("in", "Support image that will be classified");

    AddParameter(ParameterType_InputImage,  "mask",   "Input validity mask");
    SetParameterDescription("mask", "Validity mask (only pixels corresponding to a mask value greater than 0 will be used for statistics)");
    MandatoryOff("mask");

    AddParameter(ParameterType_InputFilename, "vec", "Input vectors");
    SetParameterDescription("vec","Input geometries to analyze");

    AddParameter(ParameterType_OutputFilename, "out", "Output XML statistics file");
    SetParameterDescription("out","Output file to store statistics (XML format)");

    AddParameter(ParameterType_ListView, "field", "Field Name");
    SetParameterDescription("field","Name of the field carrying the class name in the input vectors.");
    SetListViewSingleSelectionMode("field",true);

    AddParameter(ParameterType_Int, "layer", "Layer Index");
    SetParameterDescription("layer", "Layer index to read in the input vector file.");
    MandatoryOff("layer");
    SetDefaultParameterInt("layer",0);

    ElevationParametersHandler::AddElevationParameters(this, "elev");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "support_image.tif");
    SetDocExampleParameterValue("vec", "variousVectors.sqlite");
    SetDocExampleParameterValue("field", "CLASS");
    SetDocExampleParameterValue("out","polygonStat.xml");

    SetOfficialDocLink();
  }

  void DoUpdateParameters() override
  {
     if ( HasValue("vec") )
      {
      std::string vectorFile = GetParameterString("vec");
      ogr::DataSource::Pointer ogrDS =
        ogr::DataSource::New(vectorFile, ogr::DataSource::Modes::Read);
      ogr::Layer layer = ogrDS->GetLayer(this->GetParameterInt("layer"));
      ogr::Feature feature = layer.ogr().GetNextFeature();

      ClearChoices("field");

      for(int iField=0; iField<feature.ogr().GetFieldCount(); iField++)
        {
        std::string key, item = feature.ogr().GetFieldDefnRef(iField)->GetNameRef();
        key = item;
        std::string::iterator end = std::remove_if(key.begin(),key.end(),IsNotAlphaNum);
        std::transform(key.begin(), end, key.begin(), tolower);

        OGRFieldType fieldType = feature.ogr().GetFieldDefnRef(iField)->GetType();

        if(fieldType == OFTString || fieldType == OFTInteger || fieldType == OFTInteger64)
          {
          std::string tmpKey="field."+key.substr(0, end - key.begin());
          AddChoice(tmpKey,item);
          }
        }
      }

     // Check that the extension of the output parameter is XML (mandatory for
     // StatisticsXMLFileWriter)
     // Check it here to trigger the error before polygons analysis

     if ( HasValue("out") )
       {
       // Store filename extension
       // Check that the right extension is given : expected .xml
       const std::string extension = itksys::SystemTools::GetFilenameLastExtension(this->GetParameterString("out"));

       if (itksys::SystemTools::LowerCase(extension) != ".xml")
         {
         otbAppLogFATAL( << extension << " is a wrong extension for parameter \"out\": Expected .xml" );
         }
       }
  }

  void DoExecute() override
  {
    otb::ogr::DataSource::Pointer vectors = otb::ogr::DataSource::New(this->GetParameterString("vec"));

    // Retrieve the field name
    std::vector<int> selectedCFieldIdx = GetSelectedItems("field");

    if (selectedCFieldIdx.empty())
    {
    otbAppLogFATAL(<<"No field has been selected for data labelling!");
    }

    std::vector<std::string> cFieldNames = GetChoiceNames("field");
    std::string              fieldName   = cFieldNames[selectedCFieldIdx.front()];

    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this, "elev");

    // Reproject geometries
    FloatVectorImageType::Pointer              inputImg            = this->GetParameterImage("in");
    std::string                                imageProjectionRef  = inputImg->GetProjectionRef();
    FloatVectorImageType::ImageKeywordlistType imageKwl            = inputImg->GetImageKeywordlist();
    std::string                                vectorProjectionRef = vectors->GetLayer(GetParameterInt("layer")).GetProjectionRef();

    otb::ogr::DataSource::Pointer reprojVector = vectors;
    GeometriesType::Pointer       inputGeomSet;
    ProjectionFilterType::Pointer geometriesProjFilter;
    GeometriesType::Pointer       outputGeomSet;
    const OGRSpatialReference     imgOGRSref    = OGRSpatialReference(imageProjectionRef.c_str());
    const OGRSpatialReference     vectorOGRSref = OGRSpatialReference(vectorProjectionRef.c_str());
    bool                          doReproj      = true;
    // don't reproject for these cases
    if (vectorProjectionRef.empty() || (imgOGRSref.IsSame(&vectorOGRSref)) || (imageProjectionRef.empty() && imageKwl.GetSize() == 0))
      doReproj = false;

    if (doReproj)
    {
    inputGeomSet = GeometriesType::New(vectors);
    reprojVector = otb::ogr::DataSource::New();
    outputGeomSet = GeometriesType::New(reprojVector);
    // Filter instantiation
    geometriesProjFilter = ProjectionFilterType::New();
    geometriesProjFilter->SetInput(inputGeomSet);
    if (imageProjectionRef.empty())
      {
      geometriesProjFilter->SetOutputKeywordList(inputImg->GetImageKeywordlist()); // nec qd capteur
      }
    geometriesProjFilter->SetOutputProjectionRef(imageProjectionRef);
    geometriesProjFilter->SetOutput(outputGeomSet);
    otbAppLogINFO("Reprojecting input vectors...");
    geometriesProjFilter->Update();
    }

  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(this->GetParameterImage("in"));
  if (IsParameterEnabled("mask") && HasValue("mask"))
    {
      filter->SetMask(this->GetParameterUInt8Image("mask"));
    }
  filter->SetOGRData(reprojVector);
  filter->SetFieldName(fieldName);
  filter->SetLayerIndex(this->GetParameterInt("layer"));
  filter->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));

  AddProcess(filter->GetStreamer(),"Analyze polygons...");
  filter->Update();

  FilterType::ClassCountMapType &classCount = filter->GetClassCountOutput()->Get();
  FilterType::PolygonSizeMapType &polySize = filter->GetPolygonSizeOutput()->Get();

  StatWriterType::Pointer statWriter = StatWriterType::New();
  statWriter->SetFileName(this->GetParameterString("out"));
  statWriter->AddInputMap<FilterType::ClassCountMapType>("samplesPerClass",classCount);
  statWriter->AddInputMap<FilterType::PolygonSizeMapType>("samplesPerVector",polySize);
  statWriter->Update();
  }

};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::PolygonClassStatistics)
