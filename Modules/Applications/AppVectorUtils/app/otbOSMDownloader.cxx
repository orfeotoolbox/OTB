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

#include "otbImageToEnvelopeVectorDataFilter.h"
#include "otbVectorDataProperties.h"
#include "otbOSMDataToVectorDataGenerator.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{
namespace Wrapper
{

class OSMDownloader : public Application
{
public:
  /** Standard class typedefs. */
  typedef OSMDownloader                 Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(OSMDownloader, otb::Application);

  /** Filter typedef */
  typedef otb::OSMDataToVectorDataGenerator VectorDataProviderType;

private:
  void DoInit() override
  {
    SetName("OSMDownloader");
    SetDescription("Download vector data from OSM and store it to file");
    // Documentation
    SetDocLongDescription(
        "The application connects to Open Street Map server"
        ", downloads the data corresponding to the spatial extent of the support"
        " image, and filters the geometries based on OSM tags to produce a vector"
        " data file.\n\n"
        "This application can be used to download reference data to perform the "
        "training of a machine learning model (see for instance [1]).\n\n"
        "By default, the entire layer is downloaded. The application has a "
        "special mode to provide the list of available classes in the layers. "
        "The downloaded features are filtered by giving an OSM tag 'key'. In "
        "addition, the user can also choose what 'value' this key should have. "
        "More information about the OSM project at [2].");
    SetDocLimitations("This application requires an Internet access.");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(
        "[1] TrainImagesClassifier \n"
        "[2] http://www.openstreetmap.fr/");

    AddDocTag(Tags::Vector);

    AddParameter(ParameterType_OutputVectorData, "out", "Output vector data");
    SetParameterDescription("out", "Vector data file to store downloaded features");

    AddParameter(ParameterType_InputImage, "support", "Support image");
    SetParameterDescription("support",
                            "Image used to derive the spatial extent"
                            " to be requested from OSM server (the bounding box of the extent is "
                            "used). Be aware that a request with a large extent may be rejected by "
                            "the server.");

    AddParameter(ParameterType_String, "key", "OSM tag key");
    SetParameterDescription("key",
                            "OSM tag key to extract (highway, building"
                            "...). It defines a category to select features.");
    MandatoryOff("key");

    AddParameter(ParameterType_String, "value", "OSM tag value");
    SetParameterDescription("value",
                            "OSM tag value to extract (motorway, "
                            "footway...). It defines the type of feature to select inside a category.");
    MandatoryOff("value");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    AddParameter(ParameterType_Bool, "printclasses", "Displays available key/value classes");
    SetParameterDescription("printclasses",
                            "Print the key/value classes "
                            "available for the selected support image. If enabled, the OSM tag Key "
                            "(-key) and the output (-out) become optional");

    // Doc example parameter settings
    SetDocExampleParameterValue("support", "qb_RoadExtract.tif");
    SetDocExampleParameterValue("key", "highway");
    SetDocExampleParameterValue("out", "apTvUtOSMDownloader.shp");

    SetOfficialDocLink();
  }


  void DoUpdateParameters() override
  {
    // CASE:  when the -print option is not required and the User
    // does not set the option OSMKey or the option Output or does not
    // set both of them
    if (GetParameterInt("printclasses"))
    {
      MandatoryOff("out");
      MandatoryOff("key");
    }
    else
    {
      MandatoryOn("out");
      MandatoryOn("key");
    }
  }

  void DoExecute() override
  {
    typedef otb::ImageToEnvelopeVectorDataFilter<FloatVectorImageType, VectorDataType> EnvelopeFilterType;
    typedef otb::VectorDataProperties<VectorDataType> VectorDataPropertiesType;

    // Instantiate
    EnvelopeFilterType::Pointer       envelopeFilter = EnvelopeFilterType::New();
    VectorDataPropertiesType::Pointer vdProperties   = VectorDataPropertiesType::New();
    m_VdOSMGenerator                                 = VectorDataProviderType::New();

    // Get the support image
    envelopeFilter->SetInput(this->GetParameterImage("support")); //->Output in WGS84

    // Setup the DEM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this, "elev");

    envelopeFilter->Update();

    vdProperties->SetVectorDataObject(envelopeFilter->GetOutput());
    vdProperties->ComputeBoundingRegion();

    double north, south, east, west;
    north = vdProperties->GetBoundingRegion().GetIndex()[1] + vdProperties->GetBoundingRegion().GetSize()[1];
    south = vdProperties->GetBoundingRegion().GetIndex()[1];
    east  = vdProperties->GetBoundingRegion().GetIndex()[0] + vdProperties->GetBoundingRegion().GetSize()[0];
    west  = vdProperties->GetBoundingRegion().GetIndex()[0];

    m_VdOSMGenerator->SetNorth(north);
    m_VdOSMGenerator->SetSouth(south);
    m_VdOSMGenerator->SetEast(east);
    m_VdOSMGenerator->SetWest(west);

    try
    {
      m_VdOSMGenerator->Update();
    }
    catch (itk::ExceptionObject& err)
    {
      otbAppLogCRITICAL("Exception itk::ExceptionObject raised !");
      otbAppLogCRITICAL(<< err);
      return;
    }

    // If the user wants to print the Key/Values present in the XML file
    // downloaded  :
    if (GetParameterInt("printclasses"))
    {
      // Print the classes
      VectorDataProviderType::KeyMapType keymap = m_VdOSMGenerator->GetKeysMap();

      VectorDataProviderType::KeyMapType::iterator it = keymap.begin();

      while (it != keymap.end())
      {
        otbAppLogINFO(" Key : " << (*it).first << " value : ");
        std::ostringstream oss;
        for (unsigned int i = 0; i < (*it).second.size(); i++)
        {
          oss.str();
          oss << ((*it).second[i]) << " ";
        }
        otbAppLogINFO(<< oss.str());
        ++it;
      }
      return;
    }

    // Get the VectorData By name
    if (this->HasValue("value"))
    {
      SetParameterOutputVectorData(
          "out", const_cast<VectorDataType*>(m_VdOSMGenerator->GetVectorDataByName(this->GetParameterString("key"), this->GetParameterString("value"))));


      otbAppLogINFO(<< m_VdOSMGenerator->GetVectorDataByName(this->GetParameterString("key"), this->GetParameterString("value"))->Size() - 3
                    << " elements retrieved");
    }
    else
    {
      SetParameterOutputVectorData("out", const_cast<VectorDataType*>(m_VdOSMGenerator->GetVectorDataByName(this->GetParameterString("key"))));

      otbAppLogINFO(<< m_VdOSMGenerator->GetVectorDataByName(this->GetParameterString("key"))->Size() - 3 << " elements retrieved");
    }
  }
  VectorDataProviderType::Pointer m_VdOSMGenerator;
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::OSMDownloader)
