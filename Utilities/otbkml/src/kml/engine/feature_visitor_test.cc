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

// This file contains the unit tests for the GetRootFeature() and
// VisitFeatureHierarchy() functions.

#include "kml/engine/feature_visitor.h"
#include <vector>
#include "kml/dom.h"
#include "kml/base/unit_test.h"

using kmldom::DocumentPtr;
using kmldom::FeaturePtr;
using kmldom::FolderPtr;
using kmldom::KmlFactory;
using kmldom::KmlPtr;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;

namespace kmlengine {

class FeatureVisitorTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(FeatureVisitorTest);
  CPPUNIT_TEST(TestGetRootFeature);
  CPPUNIT_TEST(TestVisitFeatureHierarchy);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestGetRootFeature();
  void TestVisitFeatureHierarchy();

 public:
  // Called before each test.
  void setUp() {
    factory_ = KmlFactory::GetFactory();
    document_ = factory_->CreateDocument();
    folder_ = factory_->CreateFolder();
    kml_ = factory_->CreateKml();
    placemark_ = factory_->CreatePlacemark();
    point_ = factory_->CreatePoint();
  }

  // Called after each test.
  void tearDown() {
    // Nothing to tear down, all fields are values.
  }

 private:
  KmlFactory* factory_;
  DocumentPtr document_;
  FolderPtr folder_;
  KmlPtr kml_;
  PlacemarkPtr placemark_;
  PointPtr point_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(FeatureVisitorTest);

typedef std::vector<FeaturePtr> feature_vector_t;

class TestFeatureVisitor : public FeatureVisitor {
 public:
  TestFeatureVisitor(feature_vector_t* feature_vector)
      : feature_vector_(feature_vector) {}

  virtual void VisitFeature(const kmldom::FeaturePtr& feature) {
    feature_vector_->push_back(feature);
  }

 private:
  feature_vector_t* feature_vector_;
};

void FeatureVisitorTest::TestGetRootFeature() {
  // Verify NULL is properly returned for non-kml and non-Feature args.
  CPPUNIT_ASSERT(!GetRootFeature(NULL));
  CPPUNIT_ASSERT(!GetRootFeature(point_));

  // Verify NULL is returned for an empty kml element.
  CPPUNIT_ASSERT(!GetRootFeature(kml_));

  // Verify the Placemark is found as the root feature.
  kml_->set_feature(placemark_);
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Placemark, GetRootFeature(kml_)->Type());

  // Verify the Document is found as the root feature.
  kml_->set_feature(document_);
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Document, GetRootFeature(kml_)->Type());
}

void FeatureVisitorTest::TestVisitFeatureHierarchy() {
  // Create a Feature hierachy with both Features, non-Feature complex elements
  // and simple elements.
  placemark_->set_geometry(point_);  // Yes, this is not a Feature.
  placemark_->set_name("names are not confusing");
  placemark_->set_abstractview(factory_->CreateLookAt());
  folder_->add_feature(placemark_);
  folder_->set_description("description is not visited");
  folder_->set_timeprimitive(factory_->CreateTimeStamp());
  document_->add_feature(folder_);
  document_->set_snippet(factory_->CreateSnippet());
  document_->set_region(factory_->CreateRegion());
  document_->add_styleselector(factory_->CreateStyle());
  document_->add_styleselector(factory_->CreateStyleMap());

  // Create a test FeatureVisitor.
  feature_vector_t feature_vector;
  TestFeatureVisitor test_feature_visitor(&feature_vector);

  // Visit the hierarchy.
  VisitFeatureHierarchy(document_, test_feature_visitor);

  // Verify all Features were visited in the expected order.
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), feature_vector.size());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Document, feature_vector[0]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Folder, feature_vector[1]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Placemark, feature_vector[2]->Type());

  // Verify that nothing is properly found when passed NULL.
  feature_vector.clear();
  VisitFeatureHierarchy(NULL, test_feature_visitor);
  CPPUNIT_ASSERT(feature_vector.empty());

  // Verify that only 1 Feature is found when passed a non-Container.
  VisitFeatureHierarchy(placemark_, test_feature_visitor);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), feature_vector.size());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Placemark, feature_vector[0]->Type());
}

}  // end namespace kmlengine

TEST_MAIN
