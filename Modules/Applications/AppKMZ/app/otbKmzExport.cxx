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

#include "otbWrapperApplicationFactory.h"

#include "otbKmzProductWriter.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"

namespace otb
{
namespace Wrapper
{

class KmzExport : public Application
{
public:
  /** Standard class typedefs. */
  typedef KmzExport                       Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(KmzExport, otb::Application);

private:
  void DoInit() override
  {
    SetName("KmzExport");
    SetDescription("Export the input image in a KMZ product.");
    // Documentation
    SetDocName("Image to KMZ Export");
    SetDocLongDescription("This application exports the input image in a kmz product that can be display in the Google Earth software. The user can set the size of the product size, a logo and a legend to the product. Furthemore, to obtain a product that fits the relief, a DEM can be used.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Conversion");

    AddDocTag(Tags::Vector);

    AddParameter(ParameterType_InputImage,  "in",   "Input image");
    SetParameterDescription("in", "Input image");

    AddParameter(ParameterType_OutputFilename, "out",  "Output KMZ product");
    SetParameterDescription("out", "Output Kmz product directory (with .kmz extension)");

    AddParameter(ParameterType_Int, "tilesize",  "Tile Size");
    SetParameterDescription("tilesize", "Size of the tiles in the kmz product, in number of pixels (default = 512).");
    SetDefaultParameterInt("tilesize", 512);
    MandatoryOff("tilesize");

    AddParameter(ParameterType_InputImage, "logo",  "Image logo");
    SetParameterDescription("logo", "Path to the image logo to add to the KMZ product.");
    MandatoryOff("logo");

    AddParameter(ParameterType_InputImage, "legend",  "Image legend");
    SetParameterDescription("legend", "Path to the image legend to add to the KMZ product.");
    MandatoryOff("legend");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "qb_RoadExtract2.tif");
    SetDocExampleParameterValue("out", "otbKmzExport.kmz");
    SetDocExampleParameterValue("logo", "otb_big.png");

    SetOfficialDocLink();
  }


  void DoUpdateParameters() override
  {
    // Nothing to do here for the parameters : all are independent
  }

 void DoExecute() override
  {
    typedef otb::KmzProductWriter<FloatVectorImageType> KmzProductWriterType;

    // Second part : Image To Kmz
    KmzProductWriterType::Pointer kmzWriter  = KmzProductWriterType::New();
    kmzWriter->SetInput( this->GetParameterImage("in") );
    kmzWriter->SetPath( this->GetParameterString("out") );

    // Setup the DEM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");

    // If the tile size is set
    if( this->HasValue("tilesize") )
      {
      if (this->GetParameterInt("tilesize") >= 0)
        kmzWriter->SetTileSize( this->GetParameterInt("tilesize") );
      else
        itkExceptionMacro(<< "The tile size should be a positive value.");
      }

    // Add the logo if any
    if( this->HasValue("logo") )
      {
      kmzWriter->SetLogo( this->GetParameterImage("logo") );
      }

    // Add the legend if any
    if( this->HasValue("legend") )
      {
      kmzWriter->AddLegend( this->GetParameterImage("legend") );
      }

    // trigger the writing
    kmzWriter->Update();
  }
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::KmzExport)
