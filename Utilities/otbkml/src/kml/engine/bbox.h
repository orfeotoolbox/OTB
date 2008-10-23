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

// This file contains the definition of the Bbox class.

#ifndef KML_ENGINE_BBOX_H__
#define KML_ENGINE_BBOX_H__

namespace kmlengine {

const double kMinLat = -180.0;
const double kMaxLat = 180.0;
const double kMinLon = -180.0;
const double kMaxLon = 180.0;

// This class maintains a simple geographic bounding box.  Example usage:
//   Bbox bbox;
//   bbox.ExpandLatLon(lat, lon);  // 0 or more times for a set of lat,lon.
//
//   // Inquire some things about the bounding box:
//   double mid_lat, mid_lon;
//   bbox.GetCenter(&mid_lat, &mid_lon);
//   double north = bbox.get_north()  // Same for s,e,w
//   bool contains = bbox.Contains(lat, lon);
//
// NOTE: There is no provision for the ante-meridian nor for the validity
// of any latitude or longitude value.
class Bbox {
 public:
  // Construct a default bounding box.  The mininums and maximums are set such
  // that any valid latitude/longitude are handled properly.
  Bbox() : north_(kMinLat), south_(kMaxLat), east_(kMinLon), west_(kMaxLon) {}

  // Construct a bounding box of a given extent.  There are no checks for
  // the validity of these parameters.
  Bbox(double north, double south, double east, double west)
     : north_(north), south_(south), east_(east), west_(west) {}

  // This returns true if the bbox contains the given latitude,longitude.
  bool Contains(double latitude, double longitude) const {
    return north_ >= latitude && south_ <= latitude &&
           east_ >= longitude && west_ <= longitude;
  }

  // This expands the bounding box to include the given latitude.
  void ExpandLatitude(double latitude) {
    if (latitude > north_) {
      north_ = latitude;
    }
    if (latitude < south_) {
      south_ = latitude;
    }
  }

  // This expands the bounding box to include the given longitude.
  void ExpandLongitude(double longitude) {
    if (longitude > east_) {
      east_ = longitude;
    }
    if (longitude < west_) {
      west_ = longitude;
    }
  }

  // This expands the bounding box to include the given latitude and longitude.
  void ExpandLatLon(double latitude, double longitude) {
    ExpandLatitude(latitude);
    ExpandLongitude(longitude);
  }

  double get_north() const {
    return north_;
  }
  double get_south() const {
    return south_;
  }
  double get_east() const {
    return east_;
  }
  double get_west() const {
    return west_;
  }

  // This returns the center of the bounding box.
  void GetCenter(double* latitude, double* longitude) const {
    if (latitude) {
      *latitude = (north_ + south_)/2.0;
    }
    if (longitude) {
      *longitude = (east_ + west_)/2.0;
    }
  }

 private:
  double north_, south_, east_, west_;
};

}  // end namespace kmlengine

#endif  // KML_ENGINE_BBOX_H__
