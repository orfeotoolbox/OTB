/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE,  See the above copyright notices for more information.

=========================================================================*/
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "otbMapProjections.h"
#include "otbUtils.h"

namespace otb
{
namespace Wrapper
{

class ObtainUTMZoneFromGeoPoint : public Application
{
public:
  /** Standard class typedefs. */
  typedef ObtainUTMZoneFromGeoPoint     Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ObtainUTMZoneFromGeoPoint, otb::Application);
  
private:
  ObtainUTMZoneFromGeoPoint()
  {
  }

  virtual ~ObtainUTMZoneFromGeoPoint()
  {
  }

  void DoInit()
  {
    SetName("ObtainUTMZoneFromGeoPoint");
    SetDescription("UTM zone determination from a geographic point.");

    // Documentation
    SetDocName("Obtain UTM Zone From Geo Point");
    SetDocLongDescription("This application returns the UTM zone of an input geographic point.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Coordinates);

    AddParameter(ParameterType_Float,  "lat", "Latitude");
    SetParameterDescription("lat", "Latitude value of desired point.");
    
    AddParameter(ParameterType_Float,  "lon", "Longitude");
    SetParameterDescription("lon", "Longitude value of desired point.");
    
    AddParameter(ParameterType_Int,"utm","UTMZone");
    SetParameterDescription("utm","UTM Zone");
    MandatoryOff("utm");
    SetParameterRole("utm", Role_Output);
    
    SetExampleComment("Obtain a UTM Zone", 0);
    SetDocExampleParameterValue("lat","10.0");
    SetDocExampleParameterValue("lon","124.0");
  }
  
  void DoUpdateParameters()
  {
    // Nothing to do
  }
  
  void DoExecute()
  {
    int utmZone = otb::Utils::GetZoneFromGeoPoint(GetParameterFloat("lon"),
                                                  GetParameterFloat("lat"));
    SetParameterInt("utm", utmZone);
  }
  
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ObtainUTMZoneFromGeoPoint)
