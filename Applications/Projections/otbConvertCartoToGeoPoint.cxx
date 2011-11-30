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

#include <iostream>
#include <iomanip>

#include "otbMapProjections.h"
#include "otbWrapperNumericalParameter.h"

namespace otb
{
namespace Wrapper
{

enum
{
  UTM,
  LAMBERT,
  LAMBERT2,
  LAMBERT93,
  SINUS,
  ECKERT4,
  TRANSMERCATOR,
  MOLLWEID,
  SVY21
};

class ConvertCartoToGeoPoint : public Application
{
public:
  /** Standard class typedefs. */
  typedef ConvertCartoToGeoPoint                    Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ConvertCartoToGeoPoint, otb::Application);
  
private:
  ConvertCartoToGeoPoint()
  {
    SetName("ConvertCartoToGeoPoint");
    SetDescription("Convert cartographic coordinates to geographic one.");

    // Documentation
    SetDocName("Cartographic to geographic coordinates conversion");
    SetDocLongDescription("This application computes the geographic coordinates from a cartographic one. user has to give the X and Y coordinate and the cartographic projection (UTM/LAMBERT/LAMBERT2/LAMBERT93/SINUS/ECKERT4/TRANSMERCATOR/MOLLWEID/SVY21).");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");
  
    AddDocTag(Tags::Coordinates);
    AddDocTag(Tags::Geometry);
  }

  virtual ~ConvertCartoToGeoPoint()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_Group, "carto", "Input cartographic coordinates");
    AddParameter(ParameterType_Float, "carto.x", "X cartographic coordinates");
    SetParameterDescription("carto.x", "X cartographic coordinates in the specified projection.");

    AddParameter(ParameterType_Float, "carto.y", "Y cartographic coordinates");
    SetParameterDescription("carto.y", "Y cartographic coordinates in the specified projection.");

    AddParameter(ParameterType_Choice, "mapproj", "Map projection type");
    SetParameterDescription("mapproj", "Type of projection used for the conversion. Possible values are: utm, lambert, lambert2, lambert93, sinus, eckert4, transmercator, mollweid and svy21.");

    AddChoice("mapproj.utm", "utm");
    AddParameter(ParameterType_Int, "mapproj.utm.zone", "zone");
    SetParameterDescription( "mapproj.utm.zone", "UTM zone of the point.");
    SetMinimumParameterIntValue("mapproj.utm.zone", 1);
    SetParameterInt( "mapproj.utm.zone", 1);
    AddParameter(ParameterType_Empty, "mapproj.utm.hemisphere", "Is in north hemisphere");
    SetParameterDescription( "mapproj.utm.hemisphere", "Is the point is in the north hemisphere or not.");
    EnableParameter( "mapproj.utm.hemisphere");

    AddChoice("mapproj.lambert", "lambert");
    AddParameter(ParameterType_Float, "mapproj.lambert.parallel1degree", "First parallele degree");
    SetParameterDescription( "mapproj.lambert.parallel1degree", "First Lambert parallele degree.");
    AddParameter(ParameterType_Float, "mapproj.lambert.parallel2degree", "Second parallele degree");
    SetParameterDescription( "mapproj.lambert.parallel2degree", "Second Lambert parallele degree.");
    AddParameter(ParameterType_Float, "mapproj.lambert.falseeasting", "False easting");
    SetParameterDescription( "mapproj.lambert.falseeasting", "Lambert false easting value.");
    AddParameter(ParameterType_Float, "mapproj.lambert.falsenorthing", "False easting");
    SetParameterDescription( "mapproj.lambert.falsenorthing", "Lambert false easting value.");

    AddChoice("mapproj.lambert", "lambert3");

    AddChoice("mapproj.lambert", "lambert93");

    AddChoice("mapproj.sinus", "sinus");
    AddParameter(ParameterType_Float, "mapproj.sinus.falseeasting", "False easting");
    SetParameterDescription( "mapproj.sinus.falseeasting", "Sinus false easting value.");
    AddParameter(ParameterType_Float, "mapproj.sinus.falsenorthing", "False easting");
    SetParameterDescription( "mapproj.sinus.falsenorthing", "Sinus false easting value.");

    AddChoice("mapproj.eckert4", "eckert4");
    AddParameter(ParameterType_Float, "mapproj.eckert4.falseeasting", "False easting");
    SetParameterDescription( "mapproj.eckert4.falseeasting", "Eckert4 false easting value.");
    AddParameter(ParameterType_Float, "mapproj.eckert4.falsenorthing", "False easting");
    SetParameterDescription( "mapproj.eckert4.falsenorthing", "Eckert4 false easting value.");

    AddChoice("mapproj.transmercator", "transmercator");
    AddParameter(ParameterType_Float, "mapproj.transmercator.falseeasting", "False easting");
    SetParameterDescription( "mapproj.transmercator.falseeasting", " Transmercator false easting value.");
    AddParameter(ParameterType_Float, "mapproj.transmercator.falsenorthing", "False easting");
    SetParameterDescription( "mapproj.transmercator.falsenorthing", " Transmercator false easting value.");
    AddParameter(ParameterType_Float, "mapproj.transmercator.scale", "Scale factor");
    SetParameterDescription( "mapproj.transmercator.falsenorthing", " Transmercator scale factor value.");

    AddChoice("mapproj.mollweid", "mollweid");
    AddParameter(ParameterType_Float, "mapproj.mollweid.falseeasting", "False easting");
    SetParameterDescription( "mapproj.mollweid.falseeasting", "Mollweid false easting value.");
    AddParameter(ParameterType_Float, "mapproj.mollweid.falsenorthing", "False easting");
    SetParameterDescription( "mapproj.mollweid.falsenorthing", "Mollweid false easting value.");

    AddChoice("mapproj.svy21", "svy21");
    
    AddParameter(ParameterType_Float, "long", "Output long");
    SetParameterDescription("long", "Point longitude coordinates.");
    SetParameterRole("long", Role_Output);

    AddParameter(ParameterType_Float, "lat", "Output lat");
    SetParameterDescription("lat", "Point latitude coordinates.");
    SetParameterRole("lat", Role_Output);
  
    // Doc example parameter settings
    SetDocExampleParameterValue("carto.x", "367074.625");
    SetDocExampleParameterValue("carto.y", "4835740");
    SetDocExampleParameterValue("mapproj", "utm");
    SetDocExampleParameterValue("mapproj.utm.hemisphere", "true");
    SetDocExampleParameterValue("mapproj.utm.zone", "31");
  }

  void DoUpdateParameters()
  {
  }


  template<typename TMapProjection>
  void Generic_DoExecute( TMapProjection* mapProjection )
  {
    typedef TMapProjection MapProjectionType;
    
    typename MapProjectionType::InputPointType cartoPoint;
    typename MapProjectionType::OutputPointType geoPoint;

    cartoPoint[0] = this->GetParameterFloat("carto.x");
    cartoPoint[1] = this->GetParameterFloat("carto.y");

    geoPoint = mapProjection->TransformPoint(cartoPoint);

    otbAppLogINFO( << std::setprecision(10) << "Cartographic Point  (x , y)  : (" << cartoPoint[0] << ", " << cartoPoint[1] << ")" );
    otbAppLogINFO( << std::setprecision(10) << "Geographic   Point (Long, Lat) : (" << geoPoint[0] << ", " <<  geoPoint[1] << ")" );


    SetParameterFloat( "long", geoPoint[0] );
    SetParameterFloat( "lat", geoPoint[1] );

  }
  
  void DoExecute()
  {
    const int typeMap = this->GetParameterInt("mapproj");
    
    switch( typeMap )
      {
      case UTM:
      {
      typedef otb::UtmInverseProjection UtmProjectionType;
      UtmProjectionType::Pointer utmProjection = UtmProjectionType::New();
      
      utmProjection->SetZone(this->GetParameterInt("mapproj.utm.zone"));

      if( this->IsParameterEnabled("mapproj.utm.hemisphere"))
        utmProjection->SetHemisphere('N');
      else
        utmProjection->SetHemisphere('S');
      
      this->Generic_DoExecute<UtmProjectionType>(utmProjection);
      break;
      }
      case LAMBERT:
      {
      typedef otb::LambertConformalConicInverseProjection LambertProjectionType;
      LambertProjectionType::Pointer lambertProjection = LambertProjectionType::New();
      
      lambertProjection->SetParameters(this->GetParameterFloat( "mapproj.lambert.parallel1degree" ),
                                       this->GetParameterFloat( "mapproj.lambert.parallel2degree" ),
                                       this->GetParameterFloat( "mapproj.lambert.falseeasting" ),
                                       this->GetParameterFloat( "mapproj.lambert.falsenorthing" ));
      
      this->Generic_DoExecute<LambertProjectionType>(lambertProjection);
      break;
      }
      case LAMBERT2:
      {
      typedef otb::Lambert2EtenduInverseProjection Lambert2ProjectionType;
      Lambert2ProjectionType::Pointer lambert2Projection = Lambert2ProjectionType::New();
      
      this->Generic_DoExecute<Lambert2ProjectionType>(lambert2Projection);
      break;
      }
      case LAMBERT93:
      {
      typedef otb::Lambert93InverseProjection Lambert93ProjectionType;
      Lambert93ProjectionType::Pointer lambert93Projection = Lambert93ProjectionType::New();
      
      this->Generic_DoExecute<Lambert93ProjectionType>(lambert93Projection);
      break;
      }
      case SINUS:
      {
      typedef otb::SinusoidalInverseProjection SinusoidalProjectionType;
      SinusoidalProjectionType::Pointer sinusoidalProjection = SinusoidalProjectionType::New();
      
      sinusoidalProjection->SetParameters(  this->GetParameterFloat( "mapproj.sinus.falseeasting" ),
                                            this->GetParameterFloat( "mapproj.sinus.falsenorthing" ) );
      
      this->Generic_DoExecute<SinusoidalProjectionType>(sinusoidalProjection);
      break;
      }
      case ECKERT4:
      {
      typedef otb::Eckert4InverseProjection Eckert4ProjectionType;
      Eckert4ProjectionType::Pointer eckert4Projection = Eckert4ProjectionType::New();
      
      eckert4Projection->SetParameters(this->GetParameterFloat( "mapproj.eckert4.falseeasting" ),
                                       this->GetParameterFloat( "mapproj.eckert4.falsenorthing" ));

      this->Generic_DoExecute<Eckert4ProjectionType>(eckert4Projection);
      }
     case TRANSMERCATOR:
      {
      typedef otb::TransMercatorInverseProjection TransMercatorProjectionType;
      TransMercatorProjectionType::Pointer transMercatorProjection = TransMercatorProjectionType::New();

      transMercatorProjection->SetParameters(this->GetParameterFloat( "mapproj.transmercator.falseeasting" ),
                                             this->GetParameterFloat( "mapproj.transmercator.falsenorthing" ),
                                             this->GetParameterFloat( "mapproj.transmercator.scale" ) );

      this->Generic_DoExecute<TransMercatorProjectionType>(transMercatorProjection);
      break;
      }
      case MOLLWEID:
      {
        typedef otb::MollweidInverseProjection MollweidProjectionType;
        MollweidProjectionType::Pointer mollweidProjection = MollweidProjectionType::New();

        mollweidProjection->SetParameters(this->GetParameterFloat( "mapproj.mollweid.falseeasting" ),
                                          this->GetParameterFloat( "mapproj.mollweid.falsenorthing" ) );

        this->Generic_DoExecute<MollweidProjectionType>(mollweidProjection);
        break;
      }
      case SVY21:
      {
      typedef otb::SVY21InverseProjection SVY21ProjectionType;
      SVY21ProjectionType::Pointer svy21Projection = SVY21ProjectionType::New();
      
      this->Generic_DoExecute<SVY21ProjectionType>(svy21Projection);
      break;
      }
      default:
      {
      otbAppLogFATAL( << "Unknow map projection "<<typeMap<<".");
      }
      }
}
};

}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ConvertCartoToGeoPoint)
