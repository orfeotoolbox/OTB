/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#include "otbWrapperApplication.h"
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
  void DoInit()
  {
    SetName("KmzExport");
    SetDescription("Export the input image in a KMZ product.");
    // Documentation
    SetDocName("Image to KMZ Export");
    SetDocLongDescription("This application exports the input image in a kmz product that can be display in the Google Earth software. The user can set the size of the product size, a logo and a legend to the product. Furthemore, to obtain a product that fits the relief, a DEM can be used.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Conversion");

    AddDocTag("KMZ");
    AddDocTag("Export");

    AddParameter(ParameterType_InputImage,  "in",   "Input image");
    SetParameterDescription("in", "Input image");
    
    AddParameter(ParameterType_OutputFilename, "out",  "Output .kmz product");
    SetParameterDescription("out", "Output Kmz product directory (with .kmz extension)");
    
    AddParameter(ParameterType_Int, "tilesize",  "Tile Size");
    SetParameterDescription("tilesize", "Size of the tiles in the kmz product, in number of pixels.");
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
  }


  void DoUpdateParameters()
  {
    // Nothing to do here for the parameters : all are independent
  }

 void DoExecute()
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
      kmzWriter->SetTileSize( this->GetParameterInt("tilesize") );
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

