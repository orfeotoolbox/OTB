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

// This file contains the implementation of the FeatureList class.
// While STL list is not directly exposed the name FeatureList is as such
// to suggest list-like performance.

#include "kml/convenience/feature_list.h"
#include <stdlib.h>
#include <algorithm>
#include <fstream>
#include <list>
#include <sstream>
#include "kml/dom.h"
#include "kml/convenience/convenience.h"
#include "kml/engine.h"

using kmldom::ContainerPtr;
using kmldom::DataPtr;
using kmldom::ExtendedDataPtr;
using kmldom::FeaturePtr;
using kmldom::LatLonAltBoxPtr;
using kmldom::KmlFactory;
using kmldom::RegionPtr;
using kmlengine::Bbox;

namespace kmlconvenience {

static const char* kFeatureScoreName = "kml.FeatureScore";

int GetFeatureScore(FeaturePtr feature) {
  string score;
  if (GetExtendedDataValue(feature, kFeatureScoreName, &score)) {
    return atoi(score.c_str());
  }
  return 0;
}

void SetFeatureScore(const string& score, FeaturePtr feature) {
  SetExtendedDataValue(kFeatureScoreName, score, feature);
}

void FeatureList::PushBack(const FeaturePtr& feature) {
  if (feature) {
    feature_list_.push_back(feature);
  }
}

size_t FeatureList::Save(ContainerPtr container) const {
  size_t count = 0;
  feature_list_t::const_iterator iter;
  for (iter = feature_list_.begin(); iter != feature_list_.end(); ++iter) {
    ++count;
    container->add_feature(*iter);
  }
  return count;
}

size_t FeatureList::BboxSplit(const Bbox& bbox, size_t max,
                              FeatureList* output) {
  if (max == 0) {  // "0" has the special meaning of "all".
    max = feature_list_.size();
  }
  size_t count = 0;
  // See Effective STL, by Scott Meyers, Item 9, page 46.  A while loop is used
  // instead of for to make explicit the advancement of iter within the loop.
  // If the item is erased then list.erase() returns the advanced iter,
  // else we advance it oursevles.
  feature_list_t::iterator iter = feature_list_.begin();
  while (iter != feature_list_.end()) {
    double lat, lon;
    if (kmlengine::GetFeatureLatLon(*iter, &lat, &lon) &&
        bbox.Contains(lat,lon)) {
      if (output) {
        output->PushBack(*iter);
      }
      iter = feature_list_.erase(iter);  // list.erase() advances to next.
      ++count;
      if (--max == 0) {  // max guaranteed to be > 0.
        break;
      }
    } else {
      ++iter;  // Not erasing so we advance to next explicitely.
    }
  }
  return count;
}

size_t FeatureList::RegionSplit(const RegionPtr& region, size_t max,
                                FeatureList* output) {
  if (region && region->has_latlonaltbox()) {
    LatLonAltBoxPtr llab = region->get_latlonaltbox();
    Bbox bbox(llab->get_north(), llab->get_south(),
              llab->get_east(), llab->get_west());
    return BboxSplit(bbox, max, output);
  }
  return 0;
}

// This function object is used by STL sort() to order Features
// by score.  Results in sort of highest score first.
struct CompareFeatures
  : public std::binary_function<const kmldom::FeaturePtr&,
                                const kmldom::FeaturePtr&,
                                bool> {
  bool operator()(const kmldom::FeaturePtr& a,
                  const kmldom::FeaturePtr& b) {
    return GetFeatureScore(a) > GetFeatureScore(b);
  }
};

// Sort the internal list of features based on score.
void FeatureList::Sort() {
  // See Effective STL, by Scott Meyers, Item 44, page 192.
  feature_list_.sort(CompareFeatures());
}

// Return the number of features held in the internal list.
size_t FeatureList::Size() const {
  return feature_list_.size();
}

// Expand the bounds of the given bbox based on the features in the list.
void FeatureList::ComputeBoundingBox(Bbox* bbox) const {
  if (!bbox) {
    return;
  }
  feature_list_t::const_iterator iter;
  for (iter = feature_list_.begin(); iter != feature_list_.end(); ++iter) {
    double lat, lon;
    if (kmlengine::GetFeatureLatLon(*iter, &lat, &lon)) {
      bbox->ExpandLatLon(lat, lon);
    }
  }
}

}  // end namespace kmlconvenience
