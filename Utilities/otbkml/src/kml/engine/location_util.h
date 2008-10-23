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

// This file contains the declarations of location-related utilities.
// Since the location of a Feature or Geometry is closely tied to KML
// these functions are considered part of the KML Engine and not
// simply pure convenenience.

#ifndef KML_ENGINE_LOCATION_UTIL_H__
#define KML_ENGINE_LOCATION_UTIL_H__

#include "kml/dom.h"

namespace kmlengine {

// This returns the latitude half way between the north and south and
// the longitude half way between the east and west of the given LatLonBox
// or LatLonAltBox.
void GetCenter(const kmldom::AbstractLatLonBoxPtr& allb,
               double* lat, double* lon);

// Return the location of the Feature.
bool GetFeatureLatLon(const kmldom::FeaturePtr& placemark,
                      double* lat, double* lon);

// Return the location of the Placemark.
bool GetPlacemarkLatLon(const kmldom::PlacemarkPtr& placemark,
                        double* lat, double* lon);

// Return the location of the Point.
bool GetPointLatLon(const kmldom::PointPtr& point,
                    double* lat, double* lon);

}  // end namespace kmlengine

#endif  // KML_ENGINE_LOCATION_UTIL_H__
