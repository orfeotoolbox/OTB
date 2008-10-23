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

// This file contains the unit tests for the ObjectIdParserObserver class.

#include "kml/engine/object_id_parser_observer.h"
#include "boost/scoped_ptr.hpp"
#include "kml/dom/kml_funcs.h"  // For kmldom::Parse()
#include "kml/dom/kml_factory.h"
#include "kml/base/unit_test.h"

namespace kmlengine {

class ObjectIdParserObserverTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ObjectIdParserObserverTest);
  CPPUNIT_TEST(TestBasicNewElementUsage);
  CPPUNIT_TEST(TestNewElementDetectsDupeId);
  CPPUNIT_TEST(TestAddChild);
  CPPUNIT_TEST(TestDestructor);
  CPPUNIT_TEST(TestNonObjectWithId);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    object_id_parser_observer_.reset(new ObjectIdParserObserver(&object_id_map_));
  }

  // Called after each test.
  void tearDown() {
    // scoped_ptr dtor deletes object_id_parser_observer_'s underlying pointer.
  }

 protected:
  void TestBasicNewElementUsage();
  void TestNewElementDetectsDupeId();
  void TestAddChild();
  void TestDestructor();
  void TestNonObjectWithId();

 private:
  ObjectIdMap object_id_map_;
  boost::scoped_ptr<ObjectIdParserObserver> object_id_parser_observer_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ObjectIdParserObserverTest);

// Verify the proper operation of NewElement() for an Object with an id.
void ObjectIdParserObserverTest::TestBasicNewElementUsage() {
  // Create an Object with an id.
  const std::string kPlacemarkId("pm0");
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
  kmldom::PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_id(kPlacemarkId);

  // Pass the Object to ObjectIdParserObserver::NewElement().
  bool status = object_id_parser_observer_->NewElement(placemark);
  CPPUNIT_ASSERT_EQUAL(status, true);
  // Verify that there is only one entry in the map.
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), object_id_map_.size());
  // Verify that the id maps to the Object.
  kmldom::ObjectPtr object = object_id_map_[kPlacemarkId];
  CPPUNIT_ASSERT_EQUAL(kPlacemarkId, object->get_id());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Placemark, object->Type());
}

// Verify that NewElement() properly detects a duplicate Object id.
void ObjectIdParserObserverTest::TestNewElementDetectsDupeId() {
  // Create 2 Objects with the same id.
  const std::string kId("some-id");
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
  kmldom::PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_id(kId);
  kmldom::PointPtr point = factory->CreatePoint();
  point->set_id(kId);

  // Verify that the first use of NewElement() accepts the Object.
  CPPUNIT_ASSERT_EQUAL(true,
                       object_id_parser_observer_->NewElement(placemark));
  // Verify that the 2nd use of NewElement() rejects the Object.
  CPPUNIT_ASSERT_EQUAL(false,
                       object_id_parser_observer_->NewElement(point));

  // Verify that the map has just the first object.
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), object_id_map_.size());
  kmldom::ObjectPtr object = object_id_map_[kId];
  CPPUNIT_ASSERT_EQUAL(kId, object->get_id());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Placemark, object->Type());
}

// Verify that ObjectIdParserObserver does nothing with AddChild().
void ObjectIdParserObserverTest::TestAddChild() {
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
  kmldom::PlacemarkPtr placemark = factory->CreatePlacemark();
  kmldom::PlacemarkPtr point = factory->CreatePlacemark();

  // The default AddChild() simply always returns true.
  CPPUNIT_ASSERT_EQUAL(true,
                       object_id_parser_observer_->AddChild(placemark, point));
  CPPUNIT_ASSERT_EQUAL(true,
                       object_id_parser_observer_->AddChild(placemark, point));
  CPPUNIT_ASSERT_EQUAL(true,
                       object_id_parser_observer_->AddChild(point, placemark));
  CPPUNIT_ASSERT_EQUAL(true,
                       object_id_parser_observer_->AddChild(NULL, NULL));
}

// Verify that the destructor does not affect the map.
void ObjectIdParserObserverTest::TestDestructor() {
  // Use NewElement() to populate the map.
  const std::string kPlacemarkId("pm0");
  const std::string kPointId("pt0");
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
  kmldom::PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_id(kPlacemarkId);
  kmldom::PointPtr point = factory->CreatePoint();
  point->set_id(kPointId);
  CPPUNIT_ASSERT_EQUAL(true,
                       object_id_parser_observer_->NewElement(placemark));
  CPPUNIT_ASSERT_EQUAL(true,
                       object_id_parser_observer_->NewElement(point));

  // Force delete of the underlying object_id_parser_observer
  object_id_parser_observer_.reset();

  // Verify that the object map has exactly the 2 expected mappings.
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), object_id_map_.size());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Placemark,
                       object_id_map_[kPlacemarkId]->Type());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Point, object_id_map_[kPointId]->Type());
}

// Verify that NewElement ignores a non-Object with id attribute.
void ObjectIdParserObserverTest::TestNonObjectWithId() {
  // The Parse() function in kmldom preserves id of a non object as an
  // unknown attribute.
  const std::string kId("kml-is-not-an-object");
  kmldom::ElementPtr root = kmldom::Parse("<kml id=\"" + kId + "\"/>", NULL);

  // Verify that the parse succeeded and created a non-Object.
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_kml, root->Type());
  CPPUNIT_ASSERT(!root->IsA(kmldom::Type_Object));
  // TODO: use the unknown attribute API to verify the id.
  CPPUNIT_ASSERT_EQUAL(true, object_id_parser_observer_->NewElement(root));

  // Verify that the object map has no mappings.
  CPPUNIT_ASSERT(object_id_map_.empty());

  // Verify that a map with an Object with this id is not effected by
  // a call to NewElement() with a non-Object which happens to have an
  // id attribute with this same value.
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
  kmldom::PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_id(kId);
  CPPUNIT_ASSERT_EQUAL(true, object_id_parser_observer_->NewElement(placemark));
  // Verify that a non-Object Element with an id does not return false.
  CPPUNIT_ASSERT_EQUAL(true, object_id_parser_observer_->NewElement(root));

  // Verify that only a proper Object is in the map.
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(1), object_id_map_.size());
  CPPUNIT_ASSERT_EQUAL(kmldom::Type_Placemark,
                       object_id_map_[kId]->Type());
  CPPUNIT_ASSERT_EQUAL(kId, AsObject(object_id_map_[kId])->get_id());
}

}  // end namespace kmlengine

TEST_MAIN
