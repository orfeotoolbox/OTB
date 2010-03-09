// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "kml/convenience/convenience.h"
#include "boost/scoped_ptr.hpp"
#include "kml/base/attributes.h"
#include "kml/base/date_time.h"
#include "kml/base/math_util.h"
#include "kml/engine/bbox.h"
#include "kml/engine/clone.h"
#include "kml/engine/feature_view.h"
#include "kml/engine/location_util.h"

using kmlbase::Attributes;
using kmlbase::DateTime;
using kmlbase::Vec3;
using kmldom::AbstractViewPtr;
using kmldom::CameraPtr;
using kmldom::ChangePtr;
using kmldom::CoordinatesPtr;
using kmldom::DataPtr;
using kmldom::ExtendedDataPtr;
using kmldom::FeaturePtr;
using kmldom::GxAnimatedUpdatePtr;
using kmldom::GxFlyToPtr;
using kmldom::GxWaitPtr;
using kmldom::KmlFactory;
using kmldom::LatLonAltBoxPtr;
using kmldom::LodPtr;
using kmldom::LookAtPtr;
using kmldom::OuterBoundaryIsPtr;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;
using kmldom::PolygonPtr;
using kmldom::RegionPtr;
using kmldom::TimeStampPtr;
using kmldom::UpdatePtr;

namespace kmlconvenience {

void AddExtendedDataValue(const string& name, const string& value,
                          FeaturePtr feature) {
  if (!feature) {
    return;
  }
  if (!feature->has_extendeddata()) {
    feature->set_extendeddata(KmlFactory::GetFactory()->CreateExtendedData());
  }
  feature->get_extendeddata()->add_data(CreateDataNameValue(name, value));
}

kmldom::GxAnimatedUpdatePtr CreateAnimatedUpdateChangePoint(
    const string& target_id, const kmlbase::Vec3& vec3, double duration) {
  KmlFactory* factory = KmlFactory::GetFactory();
  PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_targetid(target_id);
  placemark->set_geometry(CreatePointFromVec3(vec3));
  ChangePtr change = factory->CreateChange();
  change->add_object(placemark);
  UpdatePtr update = factory->CreateUpdate();
  update->add_updateoperation(change);
  update->set_targethref("");
  GxAnimatedUpdatePtr animated_update = factory->CreateGxAnimatedUpdate();
  animated_update->set_update(update);
  animated_update->set_gx_duration(duration);
  return animated_update;
}

PlacemarkPtr CreateBasicPolygonPlacemark(
    const kmldom::LinearRingPtr& lr) {
  KmlFactory* factory = KmlFactory::GetFactory();
  OuterBoundaryIsPtr obi = factory->CreateOuterBoundaryIs();
  obi->set_linearring(lr);
  PolygonPtr poly = factory->CreatePolygon();
  poly->set_outerboundaryis(obi);
  PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_geometry(poly);
  return placemark;
}

CameraPtr CreateCamera(double latitude, double longitude, double altitude,
                       double heading, double tilt, double roll,
                       int altitudemode) {
  CameraPtr camera = KmlFactory::GetFactory()->CreateCamera();
  camera->set_longitude(longitude);
  camera->set_latitude(latitude);
  camera->set_altitude(altitude);
  camera->set_heading(heading);
  camera->set_tilt(tilt);
  camera->set_roll(roll);
  camera->set_altitudemode(altitudemode);
  return camera;
}

CoordinatesPtr CreateCoordinatesCircle(double lat, double lng,
                                       double radius, size_t segments) {
  CoordinatesPtr coords = KmlFactory::GetFactory()->CreateCoordinates();
  for (size_t i = 0; i < segments; ++i) {
    coords->add_vec3(kmlbase::LatLngOnRadialFromPoint(lat, lng, radius, i));
  }
  return coords;
}

DataPtr CreateDataNameValue(const string& name, const string& value) {
  DataPtr data = KmlFactory::GetFactory()->CreateData();
  data->set_name(name);
  data->set_value(value);
  return data;
}

LookAtPtr CreateLookAt(double latitude, double longitude, double altitude,
                       double heading, double tilt, double range,
                       int altitudemode) {
  LookAtPtr lookat = KmlFactory::GetFactory()->CreateLookAt();
  lookat->set_longitude(longitude);
  lookat->set_latitude(latitude);
  lookat->set_altitude(altitude);
  lookat->set_heading(heading);
  lookat->set_tilt(tilt);
  lookat->set_range(range);
  lookat->set_altitudemode(altitudemode);
  return lookat;
}

PointPtr CreatePointFromLatLonAtts(const char** atts) {
  boost::scoped_ptr<Attributes> attributes(Attributes::Create(atts));
  if (attributes.get()) {
    double latitude;
    double longitude;
    if (attributes->GetValue("lat", &latitude) &&
        attributes->GetValue("lon", &longitude)) {
      return CreatePointLatLon(latitude, longitude);
    }
  }
  return NULL;
}

PointPtr CreatePointFromVec3(const Vec3& vec) {
  KmlFactory* factory = KmlFactory::GetFactory();
  CoordinatesPtr coordinates = factory->CreateCoordinates();
  if (vec.has_altitude()) {
    coordinates->add_latlngalt(vec.get_latitude(), vec.get_longitude(),
                               vec.get_altitude());
  } else {
    coordinates->add_latlng(vec.get_latitude(), vec.get_longitude());
  }
  PointPtr point = factory->CreatePoint();
  point->set_coordinates(coordinates);
  return point;
}

PointPtr CreatePointLatLon(double lat, double lon) {
  KmlFactory* factory = KmlFactory::GetFactory();
  CoordinatesPtr coordinates = factory->CreateCoordinates();
  coordinates->add_latlng(lat, lon);
  PointPtr point = factory->CreatePoint();
  point->set_coordinates(coordinates);
  return point;
}

// This is a convenience function to create a Point Placemark.
PlacemarkPtr CreatePointPlacemark(const string& name, double lat,
                                  double lon) {
  PlacemarkPtr placemark = KmlFactory::GetFactory()->CreatePlacemark();
  placemark->set_name(name);
  placemark->set_geometry(CreatePointLatLon(lat, lon));
  return placemark;
}

// This creates a Region at the given bounding box with the given Lod range.
RegionPtr CreateRegion2d(double north, double south, double east, double west,
                         double minlodpixels, double maxlodpixels) {
  KmlFactory* factory = KmlFactory::GetFactory();
  RegionPtr region = factory->CreateRegion();
  LatLonAltBoxPtr latlonaltbox = factory->CreateLatLonAltBox();
  latlonaltbox->set_north(north);
  latlonaltbox->set_south(south);
  latlonaltbox->set_east(east);
  latlonaltbox->set_west(west);
  LodPtr lod = factory->CreateLod();
  lod->set_minlodpixels(minlodpixels);
  lod->set_maxlodpixels(maxlodpixels);
  region->set_latlonaltbox(latlonaltbox);
  region->set_lod(lod);
  return region;
}

GxFlyToPtr CreateFlyTo(const AbstractViewPtr& abstractview, double duration) {
  GxFlyToPtr flyto = KmlFactory::GetFactory()->CreateGxFlyTo();
  flyto->set_gx_duration(duration);
  AbstractViewPtr av = kmldom::AsAbstractView(kmlengine::Clone(abstractview));
  flyto->set_abstractview(av);
  return flyto;
}

GxFlyToPtr CreateFlyToForFeature(const FeaturePtr& feature, double duration) {
  if (!feature) {
    return NULL;
  }
  if (feature->has_abstractview()) {
    return CreateFlyTo(feature->get_abstractview(), duration);
  }
  LookAtPtr lookat = kmlengine::ComputeFeatureLookAt(feature);
  if (!lookat) {
    return NULL;
  }
  return CreateFlyTo(lookat, duration);
}

GxWaitPtr CreateWait(double duration) {
  GxWaitPtr wait = KmlFactory::GetFactory()->CreateGxWait();
  wait->set_gx_duration(duration);
  return wait;
}

bool GetExtendedDataValue(const FeaturePtr& feature,
                          const string& name,
                          string* value) {
  if (value && feature->has_extendeddata()) {
    ExtendedDataPtr extendeddata = feature->get_extendeddata();
    for (size_t i = 0; i < extendeddata->get_data_array_size(); ++i) {
      DataPtr data = extendeddata->get_data_array_at(i);
      if (data->has_name() && name == data->get_name()) {
        *value = data->get_value();
        return true;
      }
    }
  }
  return false;
}

void SetExtendedDataValue(const string& name, const string& value,
                          FeaturePtr feature) {
  if (!feature) {
    return;
  }
  feature->set_extendeddata(KmlFactory::GetFactory()->CreateExtendedData());
  AddExtendedDataValue(name, value, feature);
}

PlacemarkPtr CreatePointPlacemarkWithTimeStamp(const PointPtr& point,
                                               const DateTime& date_time,
                                               const char* style_id) {
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  PlacemarkPtr placemark = kml_factory->CreatePlacemark();
  // <name>
  placemark->set_name(date_time.GetXsdTime());
  // <styleUrl>
  placemark->set_styleurl(string("#") + style_id);
  // <TimeStamp>
  TimeStampPtr time_stamp = kml_factory->CreateTimeStamp();
  time_stamp->set_when(date_time.GetXsdDateTime());
  placemark->set_timeprimitive(time_stamp);
  // <ExtendedData>
  AddExtendedDataValue("date", date_time.GetXsdDate(), placemark);
  AddExtendedDataValue("time", date_time.GetXsdTime(), placemark);
  // <Point>
  placemark->set_geometry(point);
  return placemark;
}

void SimplifyCoordinates(const CoordinatesPtr& src,
                         CoordinatesPtr dest, double merge_tolerance) {
  if (!src || !dest) {
    return;
  }
  // Remember the last coordinate.
  Vec3 last_vec;
  for (size_t i = 0; i < src->get_coordinates_array_size(); ++i) {
    // If this is the first tuple, just append it to the result vec.
    if (i == 0) {
      dest->add_vec3(src->get_coordinates_array_at(i));
      last_vec = src->get_coordinates_array_at(i);
      continue;
    }
    // If the distance between the position of the last point and the current
    // point is greater than merge_tolerance, do not append it to the vector.
    if (merge_tolerance > 0.0) {
      Vec3 this_vec = src->get_coordinates_array_at(i);
      if (merge_tolerance >= kmlbase::DistanceBetweenPoints3d(
            last_vec.get_latitude(), last_vec.get_longitude(),
            last_vec.get_altitude(), this_vec.get_latitude(),
            this_vec.get_longitude(), this_vec.get_altitude())) {
        last_vec = src->get_coordinates_array_at(i);
        continue;
      }
    }
    last_vec = src->get_coordinates_array_at(i);
    dest->add_vec3(src->get_coordinates_array_at(i));
  }
}

}  // end namespace kmlconvenience
