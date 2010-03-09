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

#include "kml/regionator/feature_list_region_handler.h"
#include <map>
#include "kml/base/file.h"
#include "kml/dom.h"
#include "kml/engine.h"
#include "kml/regionator/region_handler.h"
#include "kml/regionator/regionator.h"
#include "kml/regionator/regionator_qid.h"
#include "kml/regionator/regionator_util.h"

using kmldom::FeaturePtr;
using kmldom::FolderPtr;
using kmldom::KmlPtr;
using kmldom::KmlFactory;
using kmldom::PlacemarkPtr;
using kmldom::RegionPtr;
using kmlconvenience::FeatureList;
using kmlengine::Bbox;
using kmlregionator::RegionHandler;
using kmlregionator::Regionator;

namespace kmlregionator {

// This is the maximum number of features per region.
static const int kMaxPer = 10;

bool FeatureListRegionHandler::HasData(const RegionPtr& region) {
  FeatureList this_region;
  if (feature_list_.RegionSplit(region, kMaxPer, &this_region) > 0) {
    FolderPtr folder = KmlFactory::GetFactory()->CreateFolder();
    this_region.Save(folder);
    feature_map_[region->get_id()] = folder;
    return true;
  }
  return false;
} 

FeaturePtr FeatureListRegionHandler::GetFeature(const RegionPtr& region) {
  // The Regionator calls GetFeature for a region iff we previously had
  // returned true on this region in HasData().  If we have data in a region
  // we saved the folder of that data in HasData() which we return now.
  return feature_map_[region->get_id()];
}
  
void FeatureListRegionHandler::SaveKml(const KmlPtr& kml,
                                       const string& filename) {
  string kml_data(kmldom::SerializePretty(kml));
  kmlbase::File::WriteStringToFile(kml_data, filename);
}

}  // end namespace kmlregionator
