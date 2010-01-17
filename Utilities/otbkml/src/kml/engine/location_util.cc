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

// This file contains the implementation of location-related utility functions.
// TODO: This is a precursor to Feature/Geometry bounding box and camera
// which are clearly KML semantic items and not pure convenience.

#include "kml/engine/location_util.h"
#include "kml/dom.h"

using kmldom::AbstractLatLonBoxPtr;
using kmldom::CoordinatesPtr;
using kmldom::FeaturePtr;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;
using kmldom::Vec3;

namespace kmlengine {

bool GetFeatureLatLon(const FeaturePtr& feature, double* lat, double* lon) {
  if (PlacemarkPtr placemark = kmldom::AsPlacemark(feature)) {
    return GetPlacemarkLatLon(placemark, lat, lon);
  }
  // TODO: all other Features.
  return false;
}

bool GetPlacemarkLatLon(const PlacemarkPtr& placemark,
                        double* lat, double* lon) {
  if (placemark && placemark->has_geometry()) {
    switch (placemark->get_geometry()->Type()) {
      default:
        return false;
      case kmldom::Type_Point:
        return GetPointLatLon(kmldom::AsPoint(placemark->get_geometry()),
                              lat, lon);
      // TODO: all other Geometries
    }
  }
  return false;
}

bool GetPointLatLon(const PointPtr& point, double* lat, double* lon) {
  if (point) {
    if (CoordinatesPtr coordinates = point->get_coordinates()) {
      if (coordinates->get_coordinates_array_size() > 0) {
        Vec3 point = coordinates->get_coordinates_array_at(0);
        if (lat) {
          *lat = point.get_latitude();
        }
        if (lon) {
          *lon = point.get_longitude();
        }
        return true;
      }
    }
  }
  return false;
}

void GetCenter(const AbstractLatLonBoxPtr& allb, double* lat, double* lon) {
  if (!allb) {
    return;
  }
  if (lat) {
    *lat = (allb->get_north() + allb->get_south())/2.0;
  }
  if (lon) {
    *lon = (allb->get_east() + allb->get_west())/2.0;
  }
}

}  // end namespace kmlengine
