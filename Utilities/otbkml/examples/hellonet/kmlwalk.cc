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

// Walk a KML NetworkLink hierarchy.
// Fetches and parses the NetworkLinks in a given file recursively.
// Prints a count of the number of KML files fetched and the total number
// of each kind of Feature in the hierachy.

#include <iostream>
#include <map>
#include <string>
#include "kml/dom.h"
#include "kml/dom/xsd.h"  // TODO: expose Xsd::ElementName() properly
#include "kml/engine.h"
#include "curlfetch.h"

using kmldom::ElementPtr;
using kmldom::FeaturePtr;
using kmldom::NetworkLinkPtr;
using kmldom::OverlayPtr;
using kmldom::StylePtr;
using kmlengine::KmlFile;
using kmlengine::KmlFilePtr;
using kmlengine::KmlCache;
using std::cout;
using std::endl;

static void CountFeature(int type_id);
static void PrintFeatureCounts();
static void PrintFileCount();
static void WalkKmlFile(const KmlFilePtr& kml_file);

static int file_count;
static int feature_count;
static size_t data_size;
static size_t style_size;
static size_t balloon_size;

static void PrintFileCount() {
  cout << "files " << file_count << endl;
  cout << "features " << feature_count << endl;
  cout << "data bytes " << data_size << endl;
  cout << "style bytes " << style_size << endl;
  cout << "balloon bytes " << balloon_size << endl;
}

typedef std::map<int,int> feature_counter_t;
feature_counter_t feature_counter;

static void CountFeature(int type_id) {
  ++feature_count;
  feature_counter_t::const_iterator entry = feature_counter.find(type_id);
  if (entry == feature_counter.end()) {
    feature_counter[type_id] = 1;
  } else {
    ++feature_counter[type_id];
  }
}

static void PrintFeatureCounts() {
  for (feature_counter_t::const_iterator iter = feature_counter.begin();
       iter != feature_counter.end(); ++iter) {
    std::string element_name;
    cout << kmldom::Xsd::GetSchema()->ElementName(iter->first) << " "
      << iter->second << endl;
  }
}

class FeatureCounter : public kmlengine::FeatureVisitor {
 public:
  FeatureCounter(const KmlFilePtr& kml_file)
      : kml_file_(kml_file) {}

  virtual void VisitFeature(const kmldom::FeaturePtr& feature) {
    CountFeature(feature->Type());
    StylePtr style = CreateResolvedStyle(feature, kml_file_,
                                         kmldom::STYLESTATE_NORMAL);
    std::string style_string = kmldom::SerializePretty(style);
    style_size += style_string.size();
    std::string balloon_text = CreateBalloonText(kml_file_, feature);
    balloon_size += balloon_text.size();
    if (OverlayPtr overlay = AsOverlay(feature)) {
      std::string href;
      if (kmlengine::GetIconParentHref(overlay, &href)) {
        std::string url;
        if (kmlengine::ResolveUri(kml_file_->get_url(), href, &url)) {
          std::string data;
          if (!kml_file_->get_kml_cache()->FetchData(url.c_str(), &data)) {
            cout << "fetch failed " << url << endl;
            return;
          }
          cout << href << " bytes " << data.size() << endl;
          data_size += data.size();
        }
      }
    }
  }

 private:
  const KmlFilePtr kml_file_;
};

static void HandleFile(const KmlFilePtr& kml_file) {
  cout << kml_file->get_url() << endl;
  ++file_count;
  FeatureCounter feature_counter(kml_file);
  kmlengine::VisitFeatureHierarchy(kmlengine::GetRootFeature(kml_file->root()),
                                   feature_counter);
}

static void WalkNetworkLinks(const KmlFilePtr& kml_file) {
  const kmlengine::ElementVector& link_vector =
      kml_file->get_link_parent_vector();
  for (size_t i = 0; i < link_vector.size(); ++i) {
    if (NetworkLinkPtr networklink = AsNetworkLink(link_vector[i])) {
      std::string href;
      if (kmlengine::GetLinkParentHref(networklink, &href)) {
         std::string kml_url;
         if (kmlengine::ResolveUri(kml_file->get_url(), href, &kml_url)) {
           WalkKmlFile(kml_file->get_kml_cache()->FetchKml(kml_url));
         }
      }
    }
  }
}

static void WalkKmlFile(const KmlFilePtr& kml_file) {
  // First walk through this KmlFile's Features.
  HandleFile(kml_file);
  // Then walk recursively through all of its NetworkLinks.
  WalkNetworkLinks(kml_file);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    cout << "usage: " << argv[0] << " url" << endl;
    return 1;
  }
  const char* kml_url = argv[1];
  CurlNetFetcher curl_net_fetcher;
  KmlCache kml_cache(&curl_net_fetcher, 30);
  const KmlFilePtr kml_file = kml_cache.FetchKml(kml_url);
  if (!kml_file) {
    std::cerr << "failed: " << kml_url << std::endl;
    return 1;
  }
  WalkKmlFile(kml_file);
  PrintFileCount();
  PrintFeatureCounts();
}
