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

// This file contains the unit tests for the Regionator and RegionHandler
// classes.

#include "kml/regionator/regionator.h"
#include <map>
#include <string>
#include "kml/convenience/convenience.h"
#include "kml/dom.h"
#include "kml/engine/location_util.h"
#include "kml/regionator/region_handler.h"
#include "kml/regionator/regionator_qid.h"
#include "kml/regionator/regionator_util.h"
#include "kml/base/unit_test.h"

namespace kmlregionator {

using kmldom::DocumentPtr;
using kmldom::FeaturePtr;
using kmldom::FolderPtr;
using kmldom::KmlPtr;
using kmldom::NetworkLinkPtr;
using kmldom::RegionPtr;

// This map is basically used as a mini in-core filesystem.
typedef std::map<std::string, KmlPtr> kml_file_map_t;

// This is a simple RegionHandler used in the unit tests in this file.
// Create the output map, create a PointRegionHandler pointing to the map,
// and create and run a Regionator with this handler.  After the Regionator
// has completed the client code can discard both the Regionator and the
// PointRegionHandler and use the output KML in the map as desired.
class PointRegionHandler : public RegionHandler {
public:
  // The depth is how many levels of Region hierarchy to create.
  // The map is where to save the generated KML.
  PointRegionHandler(size_t depth, kml_file_map_t* kml_file_map)
    : depth_(depth), kml_file_map_(kml_file_map) {
  }

  // The determination of whether this region has data is based solely
  // on the depth of the region here in this test.
  bool HasData(const RegionPtr& region) {
    Qid qid(region->get_id());
    if (qid.depth() > depth_) {
      return false;
    }
    return true;
  }

  // The feature for a given region is a LineString box around the edges
  // and a Point Placemark in the center with the name of the qid.
  FeaturePtr GetFeature(const RegionPtr& region) {
    double lat, lon;
    kmlengine::GetCenter(region->get_latlonaltbox(), &lat, &lon);
    FolderPtr folder = kmldom::KmlFactory::GetFactory()->CreateFolder();
    folder->add_feature(kmlconvenience::CreatePointPlacemark(region->get_id(),
                                                             lat, lon));
    folder->add_feature(CreateLineStringBox(region->get_id(), region));
    return folder;
  }

  // The generated KML is saved off to the map supplied in the constructor.
  void SaveKml(const KmlPtr& kml, const std::string& filename) {
    (*kml_file_map_)[filename] = kml;
  }

 private:
  size_t depth_;
  kml_file_map_t* kml_file_map_;
};


// This class is the unit test fixture for the KmlHandler class.
class RegionatorTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(RegionatorTest);
  CPPUNIT_TEST(TwoLevelPointRegionatorTest);
  CPPUNIT_TEST(FourLevelPointRegionatorTest);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  kml_file_map_t kml_file_map_;
  void TwoLevelPointRegionatorTest();
  void FourLevelPointRegionatorTest();
};

CPPUNIT_TEST_SUITE_REGISTRATION(RegionatorTest);

void RegionatorTest::TwoLevelPointRegionatorTest() {
  // This RegionHandler saves each KML file out to a map.
  PointRegionHandler depth2(2, &kml_file_map_);
  Regionator rtor(depth2, kmlconvenience::CreateRegion2d(10,0,10,0,128,-1));
  // Run the regionator algorithm from the given region on our RegionHandler.
  rtor.Regionate(NULL);

  // A 2 level RbNL hierarchy has one root and 4 children.
  CPPUNIT_ASSERT(5 == kml_file_map_.size());

  // Verify that the 5 files have the expected name and each has <kml> as root.
  // This essentially a test of Regionator::RegionFilename().
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_kml, kml_file_map_["1.kml"]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_kml, kml_file_map_["2.kml"]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_kml, kml_file_map_["3.kml"]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_kml, kml_file_map_["4.kml"]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_kml, kml_file_map_["5.kml"]->Type());

  // Each KML file has a Document whose name is the region qid.
  // This is essentially a test of Regionator::_Recurse().
  CPPUNIT_ASSERT_EQUAL(std::string("q0"),
                       kml_file_map_["1.kml"]->get_feature()->get_name());
  CPPUNIT_ASSERT_EQUAL(std::string("q00"),
                       kml_file_map_["2.kml"]->get_feature()->get_name());
  CPPUNIT_ASSERT_EQUAL(std::string("q01"),
                       kml_file_map_["3.kml"]->get_feature()->get_name());
  CPPUNIT_ASSERT_EQUAL(std::string("q02"),
                       kml_file_map_["4.kml"]->get_feature()->get_name());
  CPPUNIT_ASSERT_EQUAL(std::string("q03"),
                       kml_file_map_["5.kml"]->get_feature()->get_name());

  // The Document in the root KML has 5 features.
  DocumentPtr document1 =
      kmldom::AsDocument(kml_file_map_["1.kml"]->get_feature());
  CPPUNIT_ASSERT(document1);
  CPPUNIT_ASSERT(5 == document1->get_feature_array_size()); 
  // The 5 features are 4 x NetworkLinks + the Folder returned by 
  // PointRegionHandler's GetFeature
  NetworkLinkPtr nw = kmldom::AsNetworkLink(document1->get_feature_array_at(0));
  NetworkLinkPtr ne = kmldom::AsNetworkLink(document1->get_feature_array_at(1));
  NetworkLinkPtr sw = kmldom::AsNetworkLink(document1->get_feature_array_at(2));
  NetworkLinkPtr se = kmldom::AsNetworkLink(document1->get_feature_array_at(3));
  CPPUNIT_ASSERT_EQUAL(std::string("2.kml"), nw->get_link()->get_href());
  CPPUNIT_ASSERT_EQUAL(std::string("3.kml"), ne->get_link()->get_href());
  CPPUNIT_ASSERT_EQUAL(std::string("4.kml"), sw->get_link()->get_href());
  //CPPUNIT_ASSERT_EQUAL(std::string("5.kml"), se->get_link()->get_href());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Folder,
                       document1->get_feature_array_at(4)->Type());
}

void RegionatorTest::FourLevelPointRegionatorTest() {
  // This RegionHandler saves each KML file out to a map.
  PointRegionHandler depth4(4, &kml_file_map_);
  Regionator rtor(depth4, kmlconvenience::CreateRegion2d(10,0,10,0,128,-1));
  // Run the regionator algorithm from the given region on our RegionHandler.
  rtor.Regionate(NULL);

  // A 4 level RbNL hierarchy has one root, 4 children, 16 grand-children,
  // and 64 great-grand-children.  This is partly particular to
  // PointRegionHandler as it always creates 4 children if it creates any.
  CPPUNIT_ASSERT(85 == kml_file_map_.size());
}

}  // end namespace kmlregionator

TEST_MAIN
