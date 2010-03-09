// Copyright 2009, Google Inc. All rights reserved.
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

// This file contains the implementation of the GoogleMapsData class.

#include "kml/convenience/google_maps_data.h"

#include <vector>
#include "kml/base/mimetypes.h"
#include "kml/base/string_util.h"
#include "kml/convenience/atom_util.h"
#include "kml/convenience/http_client.h"
#include "kml/dom.h"
#include "kml/engine.h"

namespace kmlconvenience {

static const char* kServiceName = "local";

static const char* kScope = "http://maps.google.com";

static const char* kMetaFeedUri = "/maps/feeds/maps/default/full";

// static
GoogleMapsData* GoogleMapsData::Create(HttpClient* http_client) {
  GoogleMapsData* mds = new GoogleMapsData;
  // The HttpClient must exist.
  if (http_client) {
    http_client->AddHeader("GData-Version", "2.0");
    mds->http_client_.reset(http_client);
    return mds;
  }
  delete http_client;
  delete mds;
  return NULL;
}

static string GetScope() {
  if (const char* scope = getenv("GOOGLE_MAPS_DATA_SCOPE")) {
    return scope;
  }
  return kScope;
}

GoogleMapsData::GoogleMapsData()
  : scope_(GetScope()) {
}

// Keep POI of scoped_ptr<GoogleHttpClient>'s dtor out of .h
GoogleMapsData::~GoogleMapsData() {
}

// static
const char* GoogleMapsData::get_service_name() {
  return kServiceName;
}

// static
const char* GoogleMapsData::get_metafeed_uri() {
  return kMetaFeedUri;
}

const string& GoogleMapsData::get_scope() const {
  return scope_;
}

HttpClient* GoogleMapsData::get_http_client() const {
  return http_client_.get();
}

bool GoogleMapsData::GetMetaFeedXml(string* atom_feed) const {
  return http_client_->SendRequest(HTTP_GET, scope_ + kMetaFeedUri, NULL, NULL,
                                   atom_feed);
}

kmldom::AtomFeedPtr GoogleMapsData::GetMetaFeed() const {
  string meta_feed;
  if (GetMetaFeedXml(&meta_feed)) {
    return kmldom::AsAtomFeed(kmldom::ParseAtom(meta_feed, NULL));
  }
  return NULL;
}

// static
bool GoogleMapsData::GetFeatureFeedUri(const kmldom::AtomEntryPtr& map_entry,
                                       string* feature_feed_uri) {
  if (map_entry.get() && map_entry->has_content()) {
    const kmldom::AtomContentPtr& content = map_entry->get_content();
    if (content->has_src()) {
      if (feature_feed_uri) {
        *feature_feed_uri = content->get_src();
      }
      return true;
    }
  }
  return false;
}

bool GoogleMapsData::GetFeatureFeedXml(const string& feature_feed_uri,
                                       string* atom_feed) const {
  return http_client_->SendRequest(HTTP_GET, feature_feed_uri, NULL, NULL,
                                   atom_feed);
}

kmldom::AtomFeedPtr GoogleMapsData::GetFeatureFeedByUri(
    const string& feature_feed_uri) const {
  string feature_feed;
  if (GetFeatureFeedXml(feature_feed_uri, &feature_feed)) {
    return kmldom::AsAtomFeed(kmldom::ParseAtom(feature_feed, NULL));
  }
  return NULL;
}

// static
kmldom::FeaturePtr GoogleMapsData::GetEntryFeature(
    const kmldom::AtomEntryPtr& entry) {
  if (entry.get() && entry->has_content() &&
      entry->get_content()->get_misplaced_elements_array_size() > 0) {
    return kmldom::AsFeature(
        entry->get_content()->get_misplaced_elements_array_at(0));
  }
  return NULL;
}

// static
int GoogleMapsData::GetMapKml(const kmldom::AtomFeedPtr& feature_feed,
                              kmldom::ContainerPtr container) {
  if (!container.get() || !feature_feed.get()) {
    return -1;  // Not much to do w/o both a feature feed and container.
  }
  int feature_count = 0;
  // TODO: set container's <atom:link> to the feature feed?
  // A Google My Maps Feature Feed has one KML Feature in each <entry>.
  for (size_t i = 0; i < feature_feed->get_entry_array_size(); ++i) {
    // TODO: set <atom:link> in the cloned Feature?
    const kmldom::FeaturePtr feature =
        GetEntryFeature(feature_feed->get_entry_array_at(i));
    if (feature.get()) {
      ++feature_count;
      // Must clone because libkml strictly prevents any element from having
      // more than one parent element.
      container->add_feature(kmldom::AsFeature(kmlengine::Clone(feature)));
    }
  }
  return feature_count;
}

// static
kmldom::DocumentPtr GoogleMapsData::CreateDocumentOfMapFeatures(
    const kmldom::AtomFeedPtr& feature_feed) {
  if (!feature_feed.get()) {
    return NULL;
  }
  kmldom::DocumentPtr document =
      kmldom::KmlFactory::GetFactory()->CreateDocument();
  // TODO: set <atom:link>

  kmldom::AtomFeedPtr this_feed = feature_feed;
  do {
    GetMapKml(this_feed, document);
  } while (this_feed = AtomUtil::GetNextFeed(this_feed, *http_client_));

  return document;
}

bool GoogleMapsData::CreateMap(const string& title,
                               const string& summary,
                               string* entry) {
  // Create the <atom:entry> for the new map.
  kmlengine::KmlFilePtr kml_file = kmlengine::KmlFile::CreateFromImport(
      AtomUtil::CreateBasicEntry(title, summary));
  if (!kml_file.get()) {
    return false;
  }

  // Get the Atom in XML form.
  string post_data;
  kml_file->SerializeToString(&post_data);

  // Indicate that we're posting XML.
  StringPairVector headers;
  HttpClient::PushHeader("Content-Type", kmlbase::kAtomMimeType, &headers);

  // Send off the HTTP POST and save the result to the user supplied buffer.
  return http_client_->SendRequest(HTTP_POST, scope_ + kMetaFeedUri, &headers,
                                   &post_data, entry);
}

bool GoogleMapsData::AddFeature(const string& feature_feed_post_uri,
                                const kmldom::FeaturePtr& feature,
                                string* feature_entry_xml) {
  // Create an <atom:content> to hold the Feature.
  kmldom::AtomContentPtr content =
      kmldom::KmlFactory::GetFactory()->CreateAtomContent();
  content->AddElement(feature);
  content->set_type(kmlbase::kKmlMimeType);

  // Create an <atom:entry> to hold the <atom:content>.  Set the <atom:title>
  // from the Feature's <name> and <atom:description> from the Feature's
  // <description>.
  kmldom::AtomEntryPtr entry =
      AtomUtil::CreateBasicEntry(feature->get_name(),
                                 feature->get_description());
  entry->set_content(content);

  // Get the Atom in XML form.  Use KmlFile's serializer to get proper xmlns
  // headers.
  kmlengine::KmlFilePtr kml_file =
      kmlengine::KmlFile::CreateFromImport(entry);
  string post_data;
  kml_file->SerializeToString(&post_data);

  // Indicate that we're posting XML.
  StringPairVector headers;
  HttpClient::PushHeader("Content-Type", kmlbase::kAtomMimeType, &headers);

  // Send off the HTTP POST and save the result to the user supplied buffer.
  return http_client_->SendRequest(HTTP_POST, feature_feed_post_uri,
                                   &headers, &post_data, feature_entry_xml);
}

int GoogleMapsData::PostPlacemarks(const kmldom::FeaturePtr& root_feature,
                                   const string& feature_feed_uri) {
  // Because GetElementsById looks only _below_ the root feature.
  if (kmldom::Type_Placemark == root_feature->Type()) {
    return AddFeature(feature_feed_uri, root_feature, NULL) ? 1 : 0;
  }

  // Dig out all <Placemarks>.  Everything else is ignored, essentially
  // flattening Container hierarchies.
  kmlengine::ElementVector placemarks;
  kmlengine::GetElementsById(root_feature, kmldom::Type_Placemark, &placemarks);

  int placemark_count = 0;
  for (size_t i = 0; i < placemarks.size(); ++i) {
    const kmldom::PlacemarkPtr placemark = kmldom::AsPlacemark(placemarks[i]);
    if (placemark->has_geometry()) {
      string dummy;
      if (AddFeature(feature_feed_uri, placemark, &dummy)) {
        ++placemark_count;
      }
    }
  }
  return placemark_count;
}

// The Google Maps Data API Search Feeds section documents the search feed uri
// as based on the feature feed uri.
// See: http://code.google.com/apis/maps/documentation/mapsdata/developers_guide_protocol.html#Search.
// A feature feed is of this form:
//   http://maps.google.com/maps/feeds/features/userID/mapID/full
// A search feed is of this form:
//   http://maps.google.com/maps/feeds/features/userID/mapID/snippet?searchParameters

// static
bool GoogleMapsData::GetSearchFeedUri(const kmldom::AtomEntryPtr& map_entry,
                                      string* search_feed_uri) {
  string feature_feed_uri;
  if (!GetFeatureFeedUri(map_entry, &feature_feed_uri)) {
    return false;
  }
  size_t last_slash = feature_feed_uri.find_last_of('/');
  if (last_slash == string::npos) {
    return false;
  }
  if (search_feed_uri) {
    *search_feed_uri = feature_feed_uri.substr(0, last_slash+1) + "snippet";
  }
  return true;
}

// TODO: abstract the overall search URL and use UriParser
bool GoogleMapsData::GetSearchFeed(const string& search_feed_uri,
                                   const string& search_parameters,
                                   string* atom_feed) {
  // TODO: enhance kmlbase::UriParser to provide support for
  // http://uriparser.sourceforge.net/doc/html/#querystrings
  const string uri = search_feed_uri + "?" + search_parameters;
  return http_client_->SendRequest(HTTP_GET, uri, NULL, NULL, atom_feed);
}

// static
void GoogleMapsData::AppendBoxParameter(const double north, const double south,
                               const double east, const double west,
                               string* search_parameters) {
  if (!search_parameters) {
    return;
  }
  search_parameters->append("box=" + kmlbase::ToString(west) + "," +
                                      kmlbase::ToString(south) + "," +
                                      kmlbase::ToString(east) + "," +
                                      kmlbase::ToString(north));
}

// static
void GoogleMapsData::AppendBoxParameterFromBbox(const kmlengine::Bbox& bbox,
                                       string* search_parameters) {
  AppendBoxParameter(bbox.get_north(), bbox.get_south(), bbox.get_east(),
                     bbox.get_west(), search_parameters);
}

kmldom::AtomFeedPtr GoogleMapsData::SearchMapByBbox(
    const kmldom::AtomEntryPtr& map_entry, const kmlengine::Bbox& bbox) {
  string search_feed_uri;
  if (!GetSearchFeedUri(map_entry, &search_feed_uri)) {
    return NULL;
  }
  string search_parameters;
  AppendBoxParameterFromBbox(bbox, &search_parameters);
  string atom_feed;
  if (!GetSearchFeed(search_feed_uri, search_parameters, &atom_feed)) {
    return NULL;
  }
  return kmldom::AsAtomFeed(kmldom::ParseAtom(atom_feed, NULL));
}

kmldom::AtomEntryPtr GoogleMapsData::PostMedia(const string& slug,
                                               const string& content_type,
                                               const string& data,
                                               string* errors) {
  kmlconvenience::StringPairVector headers;
  kmlconvenience::HttpClient::PushHeader("Content-Type", content_type,
                                         &headers);
  kmlconvenience::HttpClient::PushHeader("Slug", slug, &headers);
  string map_entry_xml;
  if (!http_client_->SendRequest(kmlconvenience::HTTP_POST,
                                 scope_ + kMetaFeedUri, &headers, &data,
                                 &map_entry_xml)) {
    return NULL;
  }
  if (errors) {
    *errors = map_entry_xml;
  }
  return kmldom::AsAtomEntry(kmldom::ParseAtom(map_entry_xml, NULL));
}

kmldom::AtomEntryPtr GoogleMapsData::PostCsv(const string& title,
                                             const string& csv_data,
                                             string* errors) {
  return PostMedia(title, kmlbase::kCsvMimeType, csv_data, errors);
}

kmldom::AtomEntryPtr GoogleMapsData::PostKml(const string& title,
                                             const string& kml_data) {
  return PostMedia(title, kmlbase::kKmlMimeType, kml_data, NULL);
}

// static
bool GoogleMapsData::GetKmlUri(const kmldom::AtomEntryPtr& map_entry,
                               string* kml_uri) {
  // Until such time the <atom:entry> contains a rel="kml" or similar we crack
  // apart the rel="self" and create the Google My Maps KML export link.
  // See: http://code.google.com/apis/maps/documentation/mapsdata/reference.html#Feeds
  // This is the form of the rel="self" href:
  // http://maps.google.com/maps/feeds/maps/${user_id}/full/${map_id}
  std::string rel_self;
  if (!map_entry ||
      !kmlconvenience::AtomUtil::FindRelUrl(*map_entry, "self", &rel_self)) {
    return false;
  }

  const size_t mfm_size = 17;  // strlen("/maps/feeds/maps/")
  size_t mfm = rel_self.find("/maps/feeds/maps/");
  if (mfm == string::npos) {
    return false;
  }

  kmlbase::StringVector user_map;
  kmlbase::SplitStringUsing(rel_self.substr(mfm + mfm_size), "/", &user_map);
  if (user_map.size() != 3) {
    return false;
  }

  // Until such time Google Maps Data API itself provides a raw KML media
  // export we use the "View in Google Earth" link in Google My Maps.
  // http://maps.google.com/maps/ms?msa=0&output=kml&msid=${user_id}.${map_id}
  if (kml_uri) {
    *kml_uri = string("http://maps.google.com/maps/ms?msa=0&output=kml&msid=")
      + user_map[0] + "." + user_map[2];
  }

  return true;

}

}  // end namespace kmlconvenience
