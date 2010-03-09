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

#include "kml/engine/location_util.h"
#include "kml/dom.h"
#include "kml/engine/bbox.h"

using kmlbase::Vec3;
using kmldom::AbstractLatLonBoxPtr;
using kmldom::ContainerPtr;
using kmldom::CoordinatesPtr;
using kmldom::FeaturePtr;
using kmldom::GeometryPtr;
using kmldom::LinearRingPtr;
using kmldom::LineStringPtr;
using kmldom::ModelPtr;
using kmldom::MultiGeometryPtr;
using kmldom::PhotoOverlayPtr;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;
using kmldom::PolygonPtr;

namespace kmlengine {

bool GetCoordinatesBounds(const CoordinatesPtr& coordinates, Bbox* bbox) {
  if (!coordinates) {
    return false;
  }
  size_t num_coords = coordinates->get_coordinates_array_size();
  if (bbox) {
    for (size_t i = 0; i < num_coords; ++i) {
      const Vec3& vec3 = coordinates->get_coordinates_array_at(i);
      bbox->ExpandLatLon(vec3.get_latitude(), vec3.get_longitude());
    }
  }
  return num_coords != 0;
}

bool GetFeatureBounds(const FeaturePtr& feature, Bbox* bbox) {
  if (PlacemarkPtr placemark = kmldom::AsPlacemark(feature)) {
    return GetGeometryBounds(placemark->get_geometry(), bbox);
  } else if (PhotoOverlayPtr photooverlay = kmldom::AsPhotoOverlay(feature)) {
    return GetCoordinatesParentBounds(photooverlay->get_point(), bbox);
  } else if (ContainerPtr container = kmldom::AsContainer(feature)) {
    // TODO: unify feature hierarchy walking with public API for such
    size_t num_features = container->get_feature_array_size();
    for (size_t i = 0; i < num_features; ++num_features) {
      bool has_bounds = false;  // Turns true on any Feature w/ bounds.
      for (size_t i = 0; i < num_features; ++i) {
        if (GetFeatureBounds(container->get_feature_array_at(i), bbox)) {
          has_bounds = true;
        }
      }
      return has_bounds;
    }
  }
  // TODO: other GroundOverlay
  return false;
}

bool GetFeatureLatLon(const FeaturePtr& feature, double* lat, double* lon) {
  Bbox bbox;
  if (GetFeatureBounds(feature, &bbox)) {
    if (lat) {
      *lat = bbox.GetCenterLat();
    }
    if (lon) {
      *lon = bbox.GetCenterLon();
    }
    return true;
  }
  return false;
}

bool GetGeometryBounds(const GeometryPtr& geometry, Bbox* bbox) {
  if (!geometry) {
    return false;
  }
  // TODO: Arguably the bounds of a Geometry includes extrusion...
  if (PointPtr point = AsPoint(geometry)) {
    return GetCoordinatesParentBounds(point, bbox);
  } else if (LineStringPtr linestring = AsLineString(geometry)) {
    return GetCoordinatesParentBounds(linestring, bbox);
  } else if (LinearRingPtr linearring = AsLinearRing(geometry)) {
    return GetCoordinatesParentBounds(linearring, bbox);
  } else if (PolygonPtr polygon = AsPolygon(geometry)) {
    return polygon->has_outerboundaryis() &&
        polygon->get_outerboundaryis()->has_linearring(),
        GetCoordinatesParentBounds(
            polygon->get_outerboundaryis()->get_linearring(), bbox);
  } else if (ModelPtr model = AsModel(geometry)) {
    return GetModelBounds(model, bbox);
  } else if (MultiGeometryPtr multigeometry = AsMultiGeometry(geometry)) {
    bool has_bounds = false;  // Turns true on any Geometry w/ bounds.
    size_t size = multigeometry->get_geometry_array_size();
    for (size_t i = 0; i < size; ++i) {
      if (GetGeometryBounds(multigeometry->get_geometry_array_at(i), bbox)) {
        has_bounds = true;
      }
    }
    return has_bounds;
  }
  return false;
}

bool GetGeometryLatLon(const GeometryPtr& geometry, double* lat, double* lon) {
  Bbox bbox;
  if (GetGeometryBounds(geometry, &bbox)) {
    if (lat) {
      *lat = bbox.GetCenterLat();
    }
    if (lon) {
      *lon = bbox.GetCenterLon();
    }
    return true;
  }
  return false;
}

bool GetPlacemarkLatLon(const PlacemarkPtr& placemark,
                        double* lat, double* lon) {
  return GetGeometryLatLon(placemark->get_geometry(), lat, lon);
}

bool GetModelBounds(const ModelPtr& model, Bbox* bbox) {
  double lat, lon;
  if (GetModelLatLon(model, &lat, &lon)) {
    if (bbox) {
      bbox->ExpandLatLon(lat, lon);
    }
    return true;
  }
  return false;
}

bool GetModelLatLon(const ModelPtr& model, double* lat, double* lon) {
  if (model) {
    if (model->has_location()) {
      if (lat) {
        *lat = model->get_location()->get_latitude();
      }
      if (lon) {
        *lon = model->get_location()->get_longitude();
      }
      return true;
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
