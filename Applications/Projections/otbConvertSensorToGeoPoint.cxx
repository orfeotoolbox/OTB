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

#include "otbForwardSensorModel.h"
#include "otbCoordinateToName.h"
#include "otbMacro.h"

namespace otb
{
namespace Wrapper
{

class ConvertSensorToGeoPoint : public Application
{
public:
  /** Standard class typedefs. */
  typedef ConvertSensorToGeoPoint       Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(ConvertSensorToGeoPoint, otb::Application);

  /** Filters typedef */
  typedef otb::ForwardSensorModel<double> ModelType;
  typedef itk::Point<double, 2>           PointType;

private:
  ConvertSensorToGeoPoint()
  {
    SetName("ConvertSensorToGeoPoint");
    SetDescription("Convert Sensor Point To Geographic Point using a Forward Sensor Model");

    // Documentation
    SetDocName("Convert Sensor Point To Geographic Point");
    SetDocLongDescription("Sensor to geographic coordinates conversion");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("ConvertCartoToGeoPoint application, otbObtainUTMZoneFromGeoPoint application");
  
    AddDocTag("Projections");
  }

  virtual ~ConvertSensorToGeoPoint()
  {
  }

  void DoCreateParameters()
  {
    AddParameter(ParameterType_InputImage,  "in",   "Sensor image");

    AddParameter(ParameterType_Group, "input", "Point Coordinates");
    AddParameter(ParameterType_Float, "input.idx","X value of desired point");
    AddParameter(ParameterType_Float, "input.idy","Y value of desired point");

    // Output with Output Role
    AddParameter(ParameterType_Group, "output", "Geographic Coordinates");
    AddParameter(ParameterType_Float, "output.idx","Output Point Longitude");
    AddParameter(ParameterType_Float, "output.idy","Output Point Latitude");
    
    AddParameter(ParameterType_String,"output.town","Main town near the coordinates computed");
    AddParameter(ParameterType_String,"output.country","Country of the image");
    
    // Set the parameter role for the output parameters
    SetParameterRole("output.idx", Role_Output );
    SetParameterRole("output.idy", Role_Output );
    
    SetParameterRole("output.town", Role_Output );
    SetParameterRole("output.country", Role_Output );
  }

  void DoUpdateParameters()
  {
  }

  void DoExecute()
  {
    // Get input Image
    FloatVectorImageType::Pointer inImage = GetParameterImage("in");

    // Instanciate a ForwardSensor Model
    ModelType::Pointer model = ModelType::New();
    model->SetImageGeometry(inImage->GetImageKeywordlist());
    if ( model->IsValidSensorModel() == false )
      {
      std::cerr << "Unable to create a model" << std::endl;
      }

    // Convert the desired point
    PointType point;
    point[0] = GetParameterFloat("input.idx");
    point[1] = GetParameterFloat("input.idy");

    ModelType::OutputPointType outputPoint;
    outputPoint = model->TransformPoint(point);

    // Set the value computed
    SetParameterFloat("output.idx", outputPoint[0]);
    SetParameterFloat("output.idy", outputPoint[1]);

    // Set the town and the neaerest city
    CoordinateToName::Pointer coord2name = CoordinateToName::New();
    coord2name->SetLon(outputPoint[0]);
    coord2name->SetLat(outputPoint[1]);
    coord2name->Evaluate();

    SetParameterString("output.town", coord2name->GetPlaceName());
    SetParameterString("output.country", coord2name->GetCountryName());
  }

};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::ConvertSensorToGeoPoint)
