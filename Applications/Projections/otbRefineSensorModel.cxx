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
// Wrappers
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbWrapperChoiceParameter.h"
#include "otbWrapperElevationParametersHandler.h"
#include "otbSensorModelAdapter.h"

namespace otb
{
namespace Wrapper
{
class RefineSensorModel : public Application
{
public:
  /** Standard class typedefs. */
  typedef RefineSensorModel             Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);
  itkTypeMacro(RefineSensorModel, otb::Application);

private:
  void DoInit()
  {
    SetName("RefineSensorModel");
    SetDescription("Perform LSQR to fit sensor model to a set of tie points");
    
    SetDocName("Refine Sensor Model");
    SetDocLongDescription("TODO");
    
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    
    AddParameter(ParameterType_InputFilename,"ingeom","Input geom file");
    SetParameterDescription("ingeom","Geom file containing the sensor model to refine");

    AddParameter(ParameterType_OutputFilename,"outgeom","Output geom file");
    SetParameterDescription("outgeom","Geom file containing the refined sensor model");

    AddParameter(ParameterType_InputFilename,"inpoints","Input file containing tie points");
    SetParameterDescription("inpoints","Input file containing tie points. Points are stored in following format: row col lon lat. Line beginning with # are ignored.");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");
    
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    otb::SensorModelAdapter::Pointer sm = otb::SensorModelAdapter::New();
    
    // Read the geom file
    bool canRead = sm->ReadGeomFile(GetParameterString("ingeom"));
    
  // Setup elevation

    otb::DEMHandler::Pointer demHandler = otb::DEMHandler::New();

    double avg_elevation = 0;
    bool use_avg_elevation = false;
    switch(ElevationParametersHandler::GetElevationType(this, "elev"))
      {
      case Elevation_DEM:
      {
      sm->SetDEMDirectory(ElevationParametersHandler::GetDEMDirectory(this, "elev"));
      sm->SetGeoidFile(ElevationParametersHandler::GetGeoidFile(this, "elev"));
      demHandler->OpenDEMDirectory(ElevationParametersHandler::GetDEMDirectory(this, "elev"));
      demHandler->OpenGeoidFile(ElevationParametersHandler::GetGeoidFile(this, "elev"));
      }
      break;
      case Elevation_Average:
      {
      avg_elevation = ElevationParametersHandler::GetAverageElevation(this, "elev");
      use_avg_elevation = true;
      }
      break;
      }  

  // Parse the input file for ground control points
  std::ifstream ifs;
  ifs.open(GetParameterString("inpoints").c_str());
   
  while(!ifs.eof())
    {
    std::string line;
    std::getline(ifs,line);
    
    double x,y,z,lat,lon;
    
    // Avoid commented lines or too short ones
    if (!line.empty() && line[0] != '#')
      {
      // retrieve the x component
      std::string::size_type pos = 0;
      std::string::size_type nextpos = line.find_first_of("\t", pos);
      x = atof(line.substr(pos, nextpos).c_str());
      pos = nextpos + 1;
      nextpos = line.find_first_of("\t", pos);
      y = atof(line.substr(pos, nextpos).c_str());
      pos = nextpos + 1;
      nextpos = line.find_first_of("\t", pos);
      lon = atof(line.substr(pos, nextpos).c_str());
      pos = nextpos + 1;
      nextpos = line.find_first_of("\t", pos);
      lat = atof(line.substr(pos, nextpos).c_str());
      
      if(!use_avg_elevation)
        z = demHandler->GetHeightAboveMSL(lon,lat);
      else
        z = avg_elevation;

      std::cout<<"x="<<x<<", y="<<y<<", z="<<z<<", lon="<<lon<<", lat="<<lat<<std::endl;

      sm->AddTiePoint(x,y,z,lon,lat);
      }
    }
  ifs.close();
  
  std::cout<<"Optimization..."<<std::endl;
  sm->Optimize();
  std::cout<<"Done."<<std::endl;

  bool canWrite = sm->WriteGeomFile(GetParameterString("outgeom"));
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::RefineSensorModel)
