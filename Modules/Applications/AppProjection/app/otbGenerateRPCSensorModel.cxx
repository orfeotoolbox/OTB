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
#include "otbWrapperApplicationFactory.h"
#include "otbWrapperChoiceParameter.h"
#include "otbWrapperElevationParametersHandler.h"
#include "otbWrapperMapProjectionParametersHandler.h"
#include "otbSensorModelAdapter.h"
#include "otbRPCSolverAdapter.h"
#include "itkEuclideanDistanceMetric.h"
#include "otbGenericRSTransform.h"
#include "otbOGRDataSourceWrapper.h"
#include "ogrsf_frmts.h"

namespace otb
{
namespace Wrapper
{
class GenerateRPCSensorModel : public Application
{
public:
  /** Standard class typedefs. */
  typedef GenerateRPCSensorModel             Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef otb::RPCSolverAdapter::Point3DType              Point3DType;
  typedef otb::RPCSolverAdapter::Point2DType              Point2DType;
  typedef itk::Statistics::EuclideanDistanceMetric<Point3DType> DistanceType;

  typedef otb::RPCSolverAdapter::GCPType           TiePointType;
  typedef otb::RPCSolverAdapter::GCPsContainerType TiePointsType;

  typedef otb::GenericRSTransform<double,3,3>  RSTransformType;

  /** Standard macro */
  itkNewMacro(Self);
  itkTypeMacro(GenerateRPCSensorModel, otb::Application);

private:
  void DoInit()
  {
    SetName("GenerateRPCSensorModel");
    SetDescription("Generate a RPC sensor model from a list of Ground Control Points.");

    SetDocName("Generate a RPC sensor model");
    SetDocLongDescription("This application generates a RPC sensor model from a list of Ground Control Points. At least 20 points are required for estimation wihtout elevation support, and 40 points for estimation with elevation support. Elevation support will be automatically deactivated if an insufficient amount of points is provided. The application can optionnaly output a file containing accuracy statistics for each point, and a vector file containing segments represening points residues. The map projection parameter allows defining a map projection in which the accuracy is evaluated.");

    AddDocTag(Tags::Geometry);

    SetDocLimitations("None");
    SetDocSeeAlso("OrthoRectication,HomologousPointsExtraction,RefineSensorModel");
    SetDocAuthors("OTB-Team");

    AddParameter(ParameterType_OutputFilename,"outgeom","Output geom file");
    SetParameterDescription("outgeom","Geom file containing the generated RPC sensor model");

    AddParameter(ParameterType_InputFilename,"inpoints","Input file containing tie points");
    SetParameterDescription("inpoints","Input file containing tie points. Points are stored in following format: col row lon lat. Line beginning with # are ignored.");

    AddParameter(ParameterType_OutputFilename,"outstat","Output file containing output precision statistics");
    SetParameterDescription("outstat","Output file containing the following info: ref_lon ref_lat elevation predicted_lon predicted_lat x_error_ref(meters) y_error_ref(meters) global_error_ref(meters) x_error(meters) y_error(meters) overall_error(meters)");
    MandatoryOff("outstat");
    DisableParameter("outstat");

    AddParameter(ParameterType_OutputFilename,"outvector","Output vector file with residues");
    SetParameterDescription("outvector","File containing segments representing residues");
    MandatoryOff("outvector");
    DisableParameter("outvector");

    // Build the Output Map Projection
    MapProjectionParametersHandler::AddMapProjectionParameters(this, "map");

    // Elevation
    ElevationParametersHandler::AddElevationParameters(this, "elev");

    // Doc example parameter settings
    SetDocExampleParameterValue("outgeom","output.geom");
    SetDocExampleParameterValue("inpoints","points.txt");
    SetDocExampleParameterValue("map","epsg");
    SetDocExampleParameterValue("map.epsg.code","32631");
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    OGRMultiLineString mls;

    // Setup the DEM Handler
    otb::Wrapper::ElevationParametersHandler::SetupDEMHandlerFromElevationParameters(this,"elev");

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

      z = otb::DEMHandler::Instance()->GetHeightAboveEllipsoid(lon,lat);

      otbAppLogINFO("Adding tie point x="<<x<<", y="<<y<<", z="<<z<<", lon="<<lon<<", lat="<<lat);

      Point2DType p1;
      Point3DType p2;
      p1[0]=x;
      p1[1]=y;
      p2[0]=lon;
      p2[1]=lat;
      p2[2]=z;

      tiepoints.push_back(std::make_pair(p1,p2));

      }
    }
  ifs.close();

  otbAppLogINFO("Optimization in progress ...");

  double rms;

  otb::RPCSolverAdapter::Solve(tiepoints,rms,GetParameterString("outgeom"));

  otbAppLogINFO("Done.\n");

  otb::SensorModelAdapter::Pointer sm     = otb::SensorModelAdapter::New();
  sm->ReadGeomFile(GetParameterString("outgeom"));

  double rmse = 0;
  double rmsex = 0;
  double rmsey = 0;

  double meanx = 0;
  double meany = 0;

  DistanceType::Pointer distance = DistanceType::New();

  RSTransformType::Pointer rsTransform = RSTransformType::New();
  rsTransform->SetOutputProjectionRef(MapProjectionParametersHandler::GetProjectionRefFromChoice(this, "map"));
  rsTransform->InstanciateTransform();

  std::ofstream ofs;
  ofs<<std::fixed;
  ofs.precision(12);
  if(IsParameterEnabled("outstat"))
    {
    ofs.open(GetParameterString("outstat").c_str());
    ofs<<"#ref_lon ref_lat elevation predicted_lon predicted_lat elevation x_error(meters) y_error(meters) global_error(meters)"<<std::endl;
    }

  for(TiePointsType::const_iterator it = tiepoints.begin();
      it!=tiepoints.end(); ++it)
    {
    Point3DType tmpPoint,ref;
    sm->ForwardTransformPoint(it->first[0],it->first[1],it->second[2],tmpPoint[0],tmpPoint[1],tmpPoint[2]);

    tmpPoint = rsTransform->TransformPoint(tmpPoint);

    ref[0] = it->second[0];
    ref[1] = it->second[1];
    ref[2] = it->second[2];

    ref = rsTransform->TransformPoint(ref);

    OGRLineString ls;
    ls.addPoint(tmpPoint[0],tmpPoint[1]);
    ls.addPoint(ref[0],ref[1]);
    mls.addGeometry(&ls);

    double gerror = distance->Evaluate(ref,tmpPoint);
    double xerror = ref[0]-tmpPoint[0];
    double yerror = ref[1]-tmpPoint[1];

    if(IsParameterEnabled("outstat"))
      ofs<<ref[0]<<"\t"<<ref[1]<<"\t"<<it->second[2]<<"\t"<<tmpPoint[0]<<"\t"<<tmpPoint[1]<<"\t"<<tmpPoint[2]<<"\t"<<xerror<<"\t"<<yerror<<"\t"<<gerror<<std::endl;

    rmse += gerror*gerror;
    rmsex+= xerror*xerror;
    rmsey+= yerror*yerror;

    meanx += xerror;
    meany += yerror;
    }

  rmse/=tiepoints.size();

  rmsex/=tiepoints.size();

  rmsey/=tiepoints.size();


  meanx/=tiepoints.size();
  meany/=tiepoints.size();


  double stdevx = vcl_sqrt(rmsex - meanx * meanx);
  double stdevy = vcl_sqrt(rmsey - meany * meany);


  rmse=vcl_sqrt(rmse);
  rmsex=vcl_sqrt(rmsex);
  rmsey=vcl_sqrt(rmsey);

  otbAppLogINFO("Estimation of final accuracy: ");

  otbAppLogINFO("Overall Root Mean Square Error: "<<rmse<<" meters");
  otbAppLogINFO("X Mean Error: "<<meanx<<" meters");
  otbAppLogINFO("X standard deviation: "<<stdevx<<" meters");
  otbAppLogINFO("X Root Mean Square Error: "<<rmsex<<" meters");
  otbAppLogINFO("Y Mean Error: "<<meany<<" meters");
  otbAppLogINFO("Y standard deviation: "<<stdevy<<" meters");
  otbAppLogINFO("Y Root Mean Square Error: "<<rmsey<<" meters");


  if(IsParameterEnabled("outstat"))
    ofs.close();


if(IsParameterEnabled("outvector"))
  {
  // Create the datasource (for matches export)
  otb::ogr::Layer layer(NULL, false);
  otb::ogr::DataSource::Pointer ogrDS;

  ogrDS = otb::ogr::DataSource::New(GetParameterString("outvector"), otb::ogr::DataSource::Modes::Overwrite);
  std::string projref = MapProjectionParametersHandler::GetProjectionRefFromChoice(this, "map");
  OGRSpatialReference oSRS(projref.c_str());

  // and create the layer
  layer = ogrDS->CreateLayer("matches", &oSRS, wkbMultiLineString);
  OGRFeatureDefn & defn = layer.GetLayerDefn();
  ogr::Feature feature(defn);

  feature.SetGeometry(&mls);
  layer.CreateFeature(feature);
  }
  }
};
}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::GenerateRPCSensorModel)
