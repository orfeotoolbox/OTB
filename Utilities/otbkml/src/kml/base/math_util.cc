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

#include "kml/base/math_util.h"

// The mean radius of the Earth in meters.
// Equatorial = 6378137, polar = 6356752.
static const unsigned int kEarthRadius = 6366710;

namespace kmlbase {

double AzimuthBetweenPoints(double lat1, double lng1,
                            double lat2, double lng2) {
  const double lat1_r = DegToRad(lat1);
  const double lng1_r = DegToRad(lng1);
  const double lat2_r = DegToRad(lat2);
  const double lng2_r = DegToRad(lng2);
  return RadToDeg(fmod(atan2(sin(lng2_r - lng1_r) * cos(lat2_r),
                  cos(lat1_r) * sin(lat2_r) - sin(lat1_r) *
                  cos(lat2_r) * cos(lng2_r - lng1_r)), 2 * M_PI));
}

double DistanceBetweenPoints(double lat1, double lng1,
                             double lat2, double lng2) {
  const double lat1_r = DegToRad(lat1);
  const double lng1_r = DegToRad(lng1);
  const double lat2_r = DegToRad(lat2);
  const double lng2_r = DegToRad(lng2);
  return RadiansToMeters(2 * asin(sqrt(pow(sin((lat1_r - lat2_r)/2), 2) +
                         cos(lat1_r) * cos(lat2_r) *
                         pow(sin((lng1_r - lng2_r) / 2), 2))));
}

double DistanceBetweenPoints3d(double lat1, double lng1,
                               double alt1, double lat2,
                               double lng2, double alt2) {
  double surface_distance = DistanceBetweenPoints(lat1, lng1, lat2, lng2);
  return sqrt(pow(surface_distance, 2) + pow(alt2 - alt1, 2));
}

double ElevationBetweenPoints(double lat1, double lng1, double alt1,
                              double lat2, double lng2, double alt2) {
  // Naive implementation, accurate only over short distances.
  // TODO: see header comment about curvature.
  double distance = DistanceBetweenPoints(lat1, lng1, lat2, lng2);
  return RadToDeg(atan((alt2 - alt1) / distance));
}

double GroundDistanceFromRangeAndElevation(double range, double elevation) {
  return fabs(cos(DegToRad(elevation)) * range);
}

double HeightFromRangeAndElevation(double range, double elevation) {
  return fabs(sin(DegToRad(elevation)) * range);
}

Vec3 LatLngOnRadialFromPoint(double lat, double lng,
                             double distance, double radial) {
  const double lat_r = DegToRad(lat);
  const double lng_r = DegToRad(lng);
  const double distance_r = MetersToRadians(distance);
  const double radial_r = DegToRad(radial);
  const double radial_lat = asin(sin(lat_r) * cos(distance_r) +
                    cos(lat_r) * sin(distance_r) * cos(radial_r));
  const double delta_lng = atan2(sin(radial_r) * sin(distance_r) * cos(lat_r),
                           cos(distance_r) - sin(lat_r) * sin(radial_lat));
  const double radial_lng = fmod(lng_r + delta_lng + M_PI, 2 * M_PI) - M_PI;
  return Vec3(RadToDeg(radial_lng), RadToDeg(radial_lat));
}

double DegToRad(double degrees) { return degrees * M_PI / 180.0; }
double RadToDeg(double radians) {  return radians * 180.0 / M_PI; }
double MetersToRadians(double meters) {  return meters / kEarthRadius; }
double RadiansToMeters(double radians) {  return radians * kEarthRadius; }

}  // end namespace kmlbase
