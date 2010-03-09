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

// This file defines the Regionator class which implements the core
// "regionation" algorithm.  See region_handler.h for how this operates.

#include "kml/regionator/regionator.h"
#include <sstream>
#include "kml/base/file.h"
#include "kml/base/util.h"
#include "kml/dom.h"
#include "kml/regionator/regionator_qid.h"
#include "kml/regionator/regionator_util.h"

using kmldom::DocumentPtr;
using kmldom::ElementPtr;
using kmldom::FeaturePtr;
using kmldom::KmlPtr;
using kmldom::RegionPtr;

namespace kmlregionator {

// A Regionator instance is created from a class derived from RegionHandler
// and descends over a Region hierarchy as specified.
Regionator::Regionator(RegionHandler& rhandler, const RegionPtr& region)
    : rhandler_(rhandler), region_count_(0) {
  root_region_ = CloneRegion(region);
  root_region_->set_id(Qid::CreateRoot().str());
}

Regionator::~Regionator() {
}

// This provides the relative path name to the KML file for the given Region.
// This is the name baked into the NetworkLink/Link/href to the .kml for the
// give Region.
string Regionator::RegionFilename(const RegionPtr& region) {
  Qid qid(region->get_id());
  std::stringstream str;
  str << qid_map_[qid.str()];
  return str.str() + ".kml";
}

// This is an internal method to recurse down to a child of the given Region.
// This will save the child region to output vector if the child has data.
void Regionator::Recurse(const RegionPtr& parent, quadrant_t quadrant,
                         region_vector_t* children) {
  kmldom::RegionPtr child = CreateChildRegion(parent, quadrant);
  Qid qid(parent->get_id());
  child->set_id(qid.CreateChild(quadrant).str());
  if (_Regionate(child)) {
    children->push_back(child);
  }
}

// This is an internal method to visit a given Region.  All calls to
// the RegionHandler are within this method.  This method returns false
// if there is no data at this Region or below.
bool Regionator::_Regionate(const RegionPtr& region) {
  // Ask the RegionHandler if this region has any data.
  if (rhandler_.HasData(region) == false) {
    return false;  // No data here or below so no recursing on any children.
  }

  Qid qid(region->get_id());
  qid_map_[qid.str()] = ++region_count_;

  // Recurse on each child region saving each with data in the vector.
  region_vector_t children;
  Recurse(region, NW, &children);
  Recurse(region, NE, &children);
  Recurse(region, SW, &children);
  Recurse(region, SE, &children);

  // Create the Document that will be the root feature of the KML file for this
  // Region.
  DocumentPtr document = CreateRegionDocument(region);
  document->set_name(region->get_id());

  // Create a NetworkLink to the KML file for each child region with data.
  for (size_t i = 0; i < children.size(); ++i) {
    string href = RegionFilename(children[i]);
    document->add_feature(CreateRegionNetworkLink(children[i], href));
  }

  // Ask the RegionHandler for the Feature for this region.
  FeaturePtr feature = rhandler_.GetFeature(region);
  if (feature == NULL) {
    return false;  // This region has no data.
  }
  document->add_feature(feature);

  // Create the root element for the KML file and set the Document as the root
  // feature.  Hand the completed KML file to the RegionHandler for it to save.
  KmlPtr kml = kmldom::KmlFactory::GetFactory()->CreateKml();
  kml->set_feature(document);
  string filename(RegionFilename(region));
  if (output_directory_) {
    filename = kmlbase::File::JoinPaths(output_directory_, filename);
  }
  rhandler_.SaveKml(kml, filename);
  
  return true;  // This region has data.
}

// This is the public API to start the "regionation" at the Region supplied
// in the constructor.
bool Regionator::Regionate(const char* output_directory) {
  output_directory_ = const_cast<char*>(output_directory);
  _Regionate(root_region_);
  return true;
}

// static
bool Regionator::RegionateAligned(RegionHandler& rhandler,
                                  const RegionPtr& region,
                                  const char* output_directory) {
  kmldom::LatLonAltBoxPtr llab = CloneLatLonAltBox(region->get_latlonaltbox());
  if (!CreateAlignedAbstractLatLonBox(region->get_latlonaltbox(), llab)) {
    return false;
  }
  kmldom::RegionPtr aligned_region =
      kmldom::KmlFactory::GetFactory()->CreateRegion();
  aligned_region->set_latlonaltbox(llab);
  aligned_region->set_lod(CloneLod(region->get_lod()));
  boost::scoped_ptr<Regionator> regionator(new Regionator(rhandler,
                                                          aligned_region));
  return regionator->Regionate(output_directory);
}

}  // end namespace kmlregionator
