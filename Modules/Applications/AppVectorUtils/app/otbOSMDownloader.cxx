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
  typedef otb::OSMDataToVectorDataGenerator  VectorDataProviderType;

private:
  void DoInit()
  {
    SetName("OSMDownloader");
    SetDescription("Generate a vector data from OSM on the input image extend");
    // Documentation
    SetDocName("Open Street Map layers importations applications");
    SetDocLongDescription("Generate a vector data from Open Street Map data. A DEM could be use. By default, the entire layer is downloaded, an image can be use as support for the OSM data. The application can provide also available classes in layers . This application required an Internet access. Information about the OSM project : http://www.openstreetmap.fr/");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("Convertion");

    AddDocTag(Tags::Meta);

    AddParameter(ParameterType_OutputVectorData,  "out",   "Output vector data");
    SetParameterDescription("out", "Generated output vector data path");

    AddParameter(ParameterType_InputImage,  "support",   "Support image");
    SetParameterDescription("support", "Image used as support to estimate the models");

    AddParameter(ParameterType_String, "key",  "OSM tag key");
    SetParameterDescription("key", "OSM tag key to extract (highway, building...)");
    MandatoryOff("key");

    AddParameter(ParameterType_String, "value",  "OSM tag value");
    SetParameterDescription("value", "OSM tag value to extract (motorway, footway...)");
    MandatoryOff("value");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    AddParameter(ParameterType_Empty, "printclasses", "option to display available key/value classes");
    std::ostringstream oss;
    oss << "Print the key/value classes available for the bounding box of the input image "<<std::endl;
    oss << "\t\t\t\t  ** If not used : Note that the options OSMKey (-key) and Output (-out) become mandatory";
    SetParameterDescription("printclasses", oss.str().c_str());
    MandatoryOff("printclasses");

    // Doc example parameter settings
    SetDocExampleParameterValue("support", "qb_RoadExtract.tif");
    SetDocExampleParameterValue("key", "highway");
    SetDocExampleParameterValue("out", "apTvUtOSMDownloader.shp");
  }


  void DoUpdateParameters()
  {
    // CASE:  when the -print option is not required and the User
    // does not set the option OSMKey or the option Output or does not
    // set both of them
    if ( !this->HasValue("printclasses") )
      {
      MandatoryOn("out");
      MandatoryOn("key");
      }
    else
      {
      MandatoryOff("out");
      MandatoryOff("key");
      }
  }

 void DoExecute()
  {
    typedef otb::ImageToEnvelopeVectorDataFilter<FloatVectorImageType, VectorDataType>
      EnvelopeFilterType;
    typedef otb::VectorDataProperties<VectorDataType>   VectorDataPropertiesType;

  //Instantiate
  EnvelopeFilterType::Pointer       envelopeFilter = EnvelopeFilterType::New();
  VectorDataPropertiesType::Pointer vdProperties = VectorDataPropertiesType::New();
  m_VdOSMGenerator = VectorDataProviderType::New();

  // Get the support image
  envelopeFilter->SetInput( this->GetParameterImage("support") ); //->Output in WGS84

  // Setup the DEM Handler
  otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");

  envelopeFilter->Update();

  vdProperties->SetVectorDataObject(envelopeFilter->GetOutput());
  vdProperties->ComputeBoundingRegion();

  double north, south, east, west;
  north = vdProperties->GetBoundingRegion().GetIndex()[1]
           + vdProperties->GetBoundingRegion().GetSize()[1];
  south = vdProperties->GetBoundingRegion().GetIndex()[1];
  east  = vdProperties->GetBoundingRegion().GetIndex()[0]
           + vdProperties->GetBoundingRegion().GetSize()[0];
  west  = vdProperties->GetBoundingRegion().GetIndex()[0];

  m_VdOSMGenerator->SetNorth(north);
  m_VdOSMGenerator->SetSouth(south);
  m_VdOSMGenerator->SetEast(east);
  m_VdOSMGenerator->SetWest(west);

  try
  {
    m_VdOSMGenerator->Update();
  }
  catch ( itk::ExceptionObject & err )
  {
  otbAppLogCRITICAL("Exception itk::ExceptionObject raised !");
  otbAppLogCRITICAL( << err );
  return;
  }

  // If the user wants to print the Key/Values present in the XML file
  // downloaded  :
  if ( this->HasValue("printclasses"))
    {
    // Print the classes
    VectorDataProviderType::KeyMapType  keymap = m_VdOSMGenerator->GetKeysMap();

    VectorDataProviderType::KeyMapType::iterator  it = keymap.begin();

    while(it != keymap.end())
      {
      otbAppLogINFO(" Key : "<< (*it).first<< " value : ");
      std::ostringstream oss;
      for(unsigned int i = 0; i < (*it).second.size(); i++)
        {
        oss.str();
        oss << ((*it).second[i]) << " ";
        }
      otbAppLogINFO( << oss.str() );
      ++it;
      }
    return;
    }

  // Get the VectorData By name
  if ( this->HasValue("value") )
    {
    SetParameterOutputVectorData("out", const_cast<VectorDataType*>(m_VdOSMGenerator->GetVectorDataByName(this->GetParameterString("key"),
                                                                                                          this->GetParameterString("value"))));


    otbAppLogINFO( << m_VdOSMGenerator->GetVectorDataByName(this->GetParameterString("key"), this->GetParameterString("value"))->Size()-3
                   << " elements retrieved");
    }
  else
    {
    SetParameterOutputVectorData("out", const_cast<VectorDataType*>(m_VdOSMGenerator->GetVectorDataByName(this->GetParameterString("key"))));

    otbAppLogINFO( << m_VdOSMGenerator->GetVectorDataByName(this->GetParameterString("key"))->Size()-3
                   << " elements retrieved");
    }
  }
  VectorDataProviderType::Pointer m_VdOSMGenerator;
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::OSMDownloader)

