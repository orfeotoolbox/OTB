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

// This sample program gathers all <coordinates> in any <LineString> elements
// in the passed file and concatenates them into a single LineString Placemark.
// As there can be only one altitudeMode, the program chooses the
// altitudeMode of the first LineString element encountered.
//
// You can pass in an optional argument that permits nearby points to be
// merged into one. This is especially useful where the file contains
// multiple LineStrings where the end of one line is coincident with the
// start of the next. The algorithm considers only points that appear in
// series within the <coordinates> element(s). It will not elide points
// that are spatially coincident but not adjacent in the file.
//
// For example, to merge adjacent points if they are closer than 1 meter:
// ./simplifylines infile.kml outfile.kml 1.0
//
// The default behaviour is not to elide points (tolerance=0.0).

#include <iostream>
#include <vector>
#include "kml/base/file.h"
#include "kml/base/math_util.h"
#include "kml/convenience/convenience.h"
#include "kml/dom.h"
#include "kml/engine.h"

using kmldom::CoordinatesPtr;
using kmldom::KmlFactory;
using kmldom::LineStringPtr;
using kmldom::PlacemarkPtr;
using kmldom::KmlPtr;
using kmlengine::KmlFile;
using kmlengine::KmlFilePtr;
using std::cerr;
using std::cout;
using std::endl;

// This FeatureVisitor collects any LineString coordinates contained in the
// KmlFile and appends them to concatenated_coords.
class LineStringVisitor : public kmlengine::FeatureVisitor {
 public:
  LineStringVisitor(const KmlFilePtr& kml_file,
                    CoordinatesPtr concatenated_coordinates)
    : kml_file_(kml_file),
      concatenated_coordinates_(concatenated_coordinates),
      altitudemode_set_(false),
      altitudemode_(kmldom::ALTITUDEMODE_CLAMPTOGROUND) {
  }

  const int get_altitudemode() {
    return altitudemode_;
  }

  // The callback from VisitFeatureHierarchy. If we have a Placemark with
  // a LineString that has coordinates, append the coordinates to a
  // vector.
  virtual void VisitFeature(const kmldom::FeaturePtr& f) {
    if (f->Type() == kmldom::Type_Placemark) {
      PlacemarkPtr p = kmldom::AsPlacemark(f);
      if (p->has_geometry() &&
          p->get_geometry()->Type() == kmldom::Type_LineString) {
        LineStringPtr ls = kmldom::AsLineString(p->get_geometry());
        // If this is the first linestring, set the altitudeMode for the final
        // line from here.
        if (!altitudemode_set_) {
          altitudemode_ = ls->get_altitudemode();
          altitudemode_set_ = true;
        }
        if (ls->has_coordinates()) {
          CoordinatesPtr c = ls->get_coordinates();
          for (size_t i = 0; i < c->get_coordinates_array_size(); ++i) {
            concatenated_coordinates_->add_vec3(c->get_coordinates_array_at(i));
          }
        }
      }
    }
  }

 private:
  const KmlFilePtr kml_file_;
  CoordinatesPtr concatenated_coordinates_;
  bool altitudemode_set_;
  int altitudemode_;
};

static LineStringPtr CreateSimplifiedLineString(const KmlFilePtr& kmlfile,
                                                double merge_tolerance) {
  KmlFactory* factory = KmlFactory::GetFactory();
  CoordinatesPtr concatenated_coordinates = factory->CreateCoordinates();

  LineStringVisitor linestring_visitor(kmlfile, concatenated_coordinates);
  kmlengine::VisitFeatureHierarchy(
      kmlengine::GetRootFeature(kmlfile->get_root()), linestring_visitor);

  LineStringPtr linestring = factory->CreateLineString();
  linestring->set_altitudemode(linestring_visitor.get_altitudemode());

  CoordinatesPtr simplified_coordinates = factory->CreateCoordinates();
  kmlconvenience::SimplifyCoordinates(
      concatenated_coordinates, simplified_coordinates, merge_tolerance);
  linestring->set_coordinates(simplified_coordinates);

  return linestring;
}

static bool WriteFile(const LineStringPtr& linestring, const char* outfile) {
  PlacemarkPtr placemark = KmlFactory::GetFactory()->CreatePlacemark();
  placemark->set_geometry(linestring);
  KmlPtr kml = KmlFactory::GetFactory()->CreateKml();
  kml->set_feature(placemark);
  KmlFilePtr kmlfile = KmlFile::CreateFromImport(kml);
  if (!kmlfile) {
    cerr << "error: could not create kml file" << endl;
    return false;
  }
  std::string kml_data;
  kmlfile->SerializeToString(&kml_data);
  if (!kmlbase::File::WriteStringToFile(kml_data, outfile)) {
    cerr << "error: write of " << outfile << " failed" << endl;
    return false;
  }
  return true;
}

int kmlsimplifylines(int argc, char* argv[]) {
  if (argc != 3 && argc != 4) {
    cerr << "usage: " << argv[0] <<
      " input.kml output.kml [tolerance in meters]" <<
      endl;
    return -1;
  }

  std::string kml_data;
  if (!kmlbase::File::ReadFileToString(argv[1], &kml_data)) {
    cerr << "error: read of " << argv[1] << " failed" << endl;
    return false;
  }

  std::string errors;
  KmlFilePtr kml_file = KmlFile::CreateFromParse(kml_data, &errors);
  if (!kml_file || !errors.empty()) {
    cerr << "parse failed: " << errors << endl;;
    return -1;
  }

  double merge_tolerance = argc == 4 ? strtod(argv[3], NULL) : 0.0;

  LineStringPtr linestring = CreateSimplifiedLineString(
      kml_file, merge_tolerance);

  return WriteFile(linestring, argv[2]) ? 0 : 1;
}

