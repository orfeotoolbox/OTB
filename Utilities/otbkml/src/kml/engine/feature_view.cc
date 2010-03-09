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

// This file contains the implementation of the ComputeFeatureLookAt function.

#include "kml/engine/feature_view.h"
#include "kml/base/math_util.h"
#include "kml/dom/kml_factory.h"
#include "kml/engine/bbox.h"
#include "kml/engine/location_util.h"

using kmldom::LookAtPtr;
using kmldom::KmlFactory;
using kmldom::FeaturePtr;

namespace kmlengine {

// The range of the LookAt that emcompasses the feature's extents depends on
// the field of view of the virtual camera. Google Earth has a 60 degree
// horizontal FOV (equivalent to looking through a 30mm lens) and that value
// is used here.
static const double kFieldOfView = 60.0;

// To avoid zooming in to far to point features or features that are spatially
// small, we clap the computed range to a minimum value. Google Earth has
// always used 1 km as its minimum.
static const double kMinRange = 1000.0;

LookAtPtr ComputeFeatureLookAt(const FeaturePtr& feature) {
  Bbox bbox;
  if (!GetFeatureBounds(feature, &bbox)) {
    return NULL;
  }
  // The distance from the center point to the northern bound.
  const double d_north = kmlbase::DistanceBetweenPoints(
      bbox.GetCenterLat(), bbox.GetCenterLon(),
      bbox.get_north(), bbox.GetCenterLon());
  // The distance from the center point to the western bound.
  const double d_west = kmlbase::DistanceBetweenPoints(
      bbox.GetCenterLat(), bbox.GetCenterLon(),
      bbox.GetCenterLat(), bbox.get_west());
  // The distance from the center point to the north-western bound.
  const double d_nw_corner = sqrt(pow(d_north, 2) + pow(d_west, 2));
  // Scale the result by 1.2 to provide a margin around the feature.
  double range = d_nw_corner * tan(kmlbase::DegToRad(kFieldOfView)) * 1.1;
  if (range < kMinRange) {
    range = kMinRange;  // Clamp to minimum range.
  }
  LookAtPtr lookat = KmlFactory::GetFactory()->CreateLookAt();
  lookat->set_longitude(bbox.GetCenterLon());
  lookat->set_latitude(bbox.GetCenterLat());
  lookat->set_range(range);
  lookat->set_altitudemode(kmldom::ALTITUDEMODE_RELATIVETOGROUND);
  // We don't explcitly set the altitude, tilt or heading, relying instead on
  // their defaults of 0. This produces a LookAt that is north-up, looking
  // straight down.
  return lookat;
}

}  // end namespace kmlengine
