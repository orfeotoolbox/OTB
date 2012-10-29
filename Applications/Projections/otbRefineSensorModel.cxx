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
#include "itkPoint.h"
#include "otbGeographicalDistance.h"

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

  typedef itk::Point<double,3>                 PointType;
  typedef otb::GeographicalDistance<PointType> DistanceType;

  typedef std::pair<PointType,PointType>       TiePointType;
  typedef std::vector<TiePointType>            TiePointsType;
  
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

    AddParameter(ParameterType_OutputFilename,"outstat","Output file containing output precision statistics");
    SetParameterDescription("outstat","Output file containing the following info: ref_lon ref_lat elevation predicted_lon predicted_lat x_error(meters) y_error(meters) overall_error(meters)");
    MandatoryOff("outstat");
    DisableParameter("outstat");

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
   
  TiePointsType tiepoints;

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

      otbAppLogINFO("Adding tie point x="<<x<<", y="<<y<<", z="<<z<<", lon="<<lon<<", lat="<<lat);
      
      sm->AddTiePoint(x,y,z,lon,lat);

      PointType p1,p2;
      p1[0]=x;
      p1[1]=y;
      p1[2]=z;
      p2[0]=lon;
      p2[1]=lat;

      tiepoints.push_back(std::make_pair(p1,p2));

      }
    }
  ifs.close();
  
  otbAppLogINFO("Optimization in progress ...");
  sm->Optimize();
  otbAppLogINFO("Done.");

  bool canWrite = sm->WriteGeomFile(GetParameterString("outgeom"));

  double rmse = 0;
  double rmsex = 0;
  double rmsey = 0;

  DistanceType::Pointer distance = DistanceType::New();


  std::ofstream ofs;
  ofs<<std::fixed;
  ofs.precision(12);
  if(IsParameterEnabled("outstat"))
    {
    ofs.open(GetParameterString("outstat").c_str());
    ofs<<"#ref_lon ref_lat elevation predicted_lon predicted_lat x_error(meters) y_error(meters) global_error(meters)"<<std::endl;
    }

  for(TiePointsType::const_iterator it = tiepoints.begin();
      it!=tiepoints.end();++it)
    {
    PointType tmpPoint,tmpPointX,tmpPointY;
    sm->ForwardTransformPoint(it->first[0],it->first[1],it->first[2],tmpPoint[0],tmpPoint[1],tmpPoint[2]);


    
    tmpPointX = tmpPoint;
    tmpPointX[1] = it->second[1];

    tmpPointY = tmpPoint;
    tmpPointY[0] = it->second[0];

    double gerror = distance->Evaluate(it->second,tmpPoint);
    double xerror = distance->Evaluate(it->second,tmpPointX);
    double yerror = distance->Evaluate(it->second,tmpPointY);

    if(IsParameterEnabled("outstat"))
      ofs<<it->second[0]<<"\t"<<it->second[1]<<"\t"<<it->first[2]<<"\t"<<tmpPoint[0]<<"\t"<<tmpPoint[1]<<"\t"<<xerror<<"\t"<<yerror<<"\t"<<gerror<<std::endl;

    rmse += gerror*gerror;
    rmsex+=xerror*xerror;
    rmsey+=yerror*yerror;
    }

  rmse/=tiepoints.size();
  rmse=vcl_sqrt(rmse);
  rmsex/=tiepoints.size();
  rmsex=vcl_sqrt(rmsex);
  rmsey/=tiepoints.size();
  rmsey=vcl_sqrt(rmsey);

  otbAppLogINFO("Estimated Overall Root Mean Square Error: "<<rmse<<" meters");
  otbAppLogINFO("Estimated Horizontal Root Mean Square Error: "<<rmsex<<" meters");
  otbAppLogINFO("Estimated Vertical Root Mean Square Error: "<<rmsey<<" meters");

  if(IsParameterEnabled("outstat"))
    ofs.close();
}
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::RefineSensorModel)
