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

// This program demonstrates some capabilities of resource management of
// elements in the KML DOM.  An array (std::vector) of PlacemarkPtrs into
// parsed DOM is created.  The DOM root is discarded leaving the only
// references to the underlying Placemarks in the array.  The Placemarks
// are finally discarded when the array goes out of scope.

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "kml/dom.h"
#include "kml/engine.h"
#include "kml/base/file.h"

using kmldom::ContainerPtr;
using kmldom::ElementPtr;
using kmldom::FeaturePtr;
using kmldom::KmlPtr;
using kmldom::PlacemarkPtr;
using kmlengine::KmlFile;
using kmlengine::KmlFilePtr;
using kmlengine::KmzFile;
using kmlengine::KmzFilePtr;
using std::cout;
using std::endl;

// Declare the types and functions defined in this file.
typedef std::vector<PlacemarkPtr> placemark_vector_t;
static FeaturePtr GetKmlFileRootFeature(const char* kmlfile);
static void SavePlacemarks(const FeaturePtr& feature,
                           placemark_vector_t* placemarks);

// Save the Feature to the placemarks vector if it is a Placemark.  If the
// Feature is a Container call ourselves recursively for each Feature in the
// Container.
static void SavePlacemarks(const FeaturePtr& feature,
                           placemark_vector_t* placemarks) {
  if (PlacemarkPtr placemark = kmldom::AsPlacemark(feature)) {
    placemarks->push_back(placemark);
  } else if (const ContainerPtr container = kmldom::AsContainer(feature)) {
    for (size_t i = 0; i < container->get_feature_array_size(); ++i) {
      SavePlacemarks(container->get_feature_array_at(i), placemarks);
    }
  }
}

// Return a FeaturePtr to the root Feature in the kmlfile.  If the kmlfile
// does not parse or has no root Feature then an empty FeaturePtr is returned.
static FeaturePtr GetKmlFileRootFeature(const char* kmlfile) {
  // Read the file.
  std::string file_data;
  if (!kmlbase::File::ReadFileToString(kmlfile, &file_data)) {
    cout << kmlfile << " read failed" << endl;
    return NULL;
  }

  // If the file was KMZ, extract the KML file.
  std::string kml;
  if (KmzFile::IsKmz(file_data)) {
    KmzFilePtr kmz_file = KmzFile::OpenFromString(kmlfile);
    if (!kmz_file) {
      cout << "Failed opening KMZ file" << endl;
      return NULL;
    }
    if (!kmz_file->ReadKml(&kml)) {
      cout << "Failed to read KML from KMZ" << endl;
      return NULL;
    }
  } else {
    kml = file_data;
  }

  // Parse it.
  std::string errors;
  KmlFilePtr kml_file = KmlFile::CreateFromParse(kml, &errors);
  if (!kml_file) {
    cout << errors << endl;
    return FeaturePtr();
  }

  // Get the root
  return kmlengine::GetRootFeature(kml_file->root());
}

// This function object is used by STL sort() to alphabetize Placemarks
// by <name>.
struct ComparePlacemarks
  : public
      std::binary_function<const PlacemarkPtr&, const PlacemarkPtr&, bool> {
  bool operator()(const PlacemarkPtr& a, const PlacemarkPtr& b) {
    return a->get_name() < b->get_name();
  }
};

int main(int argc, char** argv) {
  if (argc != 2) {
    cout << "usage: " << argv[0] << " kmlfile" << endl;
    return 1;
  }

  placemark_vector_t placemark_vector;
  // Note that the FeaturePtr returned from GetKmlFileRootFeature() is
  // a temporary and goes out of scope and is destroyed after SavePlacemarks()
  // returns.  As such the only references to the Placemarks found in the
  // file are those in the placemark_vector array.  That is, the resource
  // management model of the KML DOM is such that it is completely safe to
  // save off pointers into the KML DOM even after the root of a given parse
  // has been released.
  SavePlacemarks(GetKmlFileRootFeature(argv[1]), &placemark_vector);
  sort(placemark_vector.begin(), placemark_vector.end(), ComparePlacemarks());
  for (size_t i = 0; i < placemark_vector.size(); ++i) {
    cout << i << " " << placemark_vector[i]->get_id() << " " <<
      placemark_vector[i]->get_name() << endl;
  }
  cout << argv[1] << " has " << placemark_vector.size() << " Placemarks." <<
    endl;

  // While we don't really care about resource leaks here because main() exits,
  // note that placemark_vector's destructor calls the destructor
  // of each PlacemarkPtr.  This in turn releases the final reference to
  // each Placemark.  Thus, the last reference to any KML DOM element is
  // released as main goes out of scope.

  return 0;
}
