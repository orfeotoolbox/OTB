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
#include "gtest/gtest.h"

namespace kmlengine {

class ObjectIdParserObserverTest : public testing::Test {
 protected:
  virtual void SetUp() {
    object_id_parser_observer_.reset(
        new ObjectIdParserObserver(&object_id_map_, false));
  }

  ObjectIdMap object_id_map_;
  boost::scoped_ptr<ObjectIdParserObserver> object_id_parser_observer_;
};

// Verify the proper operation of NewElement() for an Object with an id.
TEST_F(ObjectIdParserObserverTest, TestBasicNewElementUsage) {
  // Create an Object with an id.
  const string kPlacemarkId("pm0");
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
  kmldom::PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_id(kPlacemarkId);

  // Pass the Object to ObjectIdParserObserver::NewElement().
  bool status = object_id_parser_observer_->NewElement(placemark);
  ASSERT_EQ(status, true);
  // Verify that there is only one entry in the map.
  ASSERT_EQ(static_cast<size_t>(1), object_id_map_.size());
  // Verify that the id maps to the Object.
  kmldom::ObjectPtr object = object_id_map_[kPlacemarkId];
  ASSERT_EQ(kPlacemarkId, object->get_id());
  ASSERT_EQ(kmldom::Type_Placemark, object->Type());
}

// Verify that NewElement() properly permits a duplicate Object id in its
// default state.
TEST_F(ObjectIdParserObserverTest, TestNewElementPermitsDupeId) {
  // By default strict parsing is not enabled.
  // Create 2 Objects with the same id.
  const string kId("some-id");
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
  kmldom::PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_id(kId);
  kmldom::PointPtr point = factory->CreatePoint();
  point->set_id(kId);

  // Verify that the first use of NewElement() accepts the Object.
  ASSERT_TRUE(object_id_parser_observer_->NewElement(placemark));
  // Verify that the 2nd use of NewElement() also accepts the Object.
  ASSERT_TRUE(object_id_parser_observer_->NewElement(point));

  // Verify that the map has just the second object.
  // TODO: multimap would allow proper storage and access of duplicate objects.
  ASSERT_EQ(static_cast<size_t>(1), object_id_map_.size());
  kmldom::ObjectPtr object = object_id_map_[kId];
  ASSERT_EQ(kId, object->get_id());
  ASSERT_EQ(kmldom::Type_Point, object->Type());
}

// Verify that NewElement() properly fails on a duplicate Object if
// strict parsing has been enabled.
TEST_F(ObjectIdParserObserverTest, TestNewElementFailsDupeId) {
  // Enable strict parsing.
  object_id_parser_observer_.reset(
      new ObjectIdParserObserver(&object_id_map_, true));
  // Create 2 Objects with the same id.
  const string kId("some-id");
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
  kmldom::PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_id(kId);
  kmldom::PointPtr point = factory->CreatePoint();
  point->set_id(kId);

  // Verify that the first use of NewElement() accepts the Object.
  ASSERT_TRUE(object_id_parser_observer_->NewElement(placemark));
  // Verify that the 2nd use of NewElement() rejects the Object.
  ASSERT_FALSE(object_id_parser_observer_->NewElement(point));

  // Verify that the map has just the first object.
  ASSERT_EQ(static_cast<size_t>(1), object_id_map_.size());
  kmldom::ObjectPtr object = object_id_map_[kId];
  ASSERT_EQ(kId, object->get_id());
  ASSERT_EQ(kmldom::Type_Placemark, object->Type());
}

// Verify that ObjectIdParserObserver does nothing with AddChild().
TEST_F(ObjectIdParserObserverTest, TestAddChild) {
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
  kmldom::PlacemarkPtr placemark = factory->CreatePlacemark();
  kmldom::PlacemarkPtr point = factory->CreatePlacemark();

  // The default AddChild() simply always returns true.
  ASSERT_TRUE(object_id_parser_observer_->AddChild(placemark, point));
  ASSERT_TRUE(object_id_parser_observer_->AddChild(placemark, point));
  ASSERT_TRUE(object_id_parser_observer_->AddChild(point, placemark));
  ASSERT_TRUE(object_id_parser_observer_->AddChild(NULL, NULL));
}

// Verify that the destructor does not affect the map.
TEST_F(ObjectIdParserObserverTest, TestDestructor) {
  // Use NewElement() to populate the map.
  const string kPlacemarkId("pm0");
  const string kPointId("pt0");
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
  kmldom::PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_id(kPlacemarkId);
  kmldom::PointPtr point = factory->CreatePoint();
  point->set_id(kPointId);
  ASSERT_TRUE(object_id_parser_observer_->NewElement(placemark));
  ASSERT_TRUE(object_id_parser_observer_->NewElement(point));

  // Force delete of the underlying object_id_parser_observer
  object_id_parser_observer_.reset();

  // Verify that the object map has exactly the 2 expected mappings.
  ASSERT_EQ(static_cast<size_t>(2), object_id_map_.size());
  ASSERT_EQ(kmldom::Type_Placemark, object_id_map_[kPlacemarkId]->Type());
  ASSERT_EQ(kmldom::Type_Point, object_id_map_[kPointId]->Type());
}

// Verify that NewElement ignores a non-Object with id attribute.
TEST_F(ObjectIdParserObserverTest, TestNonObjectWithId) {
  // The Parse() function in kmldom preserves id of a non object as an
  // unknown attribute.
  const string kId("kml-is-not-an-object");
  kmldom::ElementPtr root = kmldom::Parse("<kml id=\"" + kId + "\"/>", NULL);

  // Verify that the parse succeeded and created a non-Object.
  ASSERT_EQ(kmldom::Type_kml, root->Type());
  ASSERT_FALSE(root->IsA(kmldom::Type_Object));
  // TODO: use the unknown attribute API to verify the id.
  ASSERT_TRUE(object_id_parser_observer_->NewElement(root));

  // Verify that the object map has no mappings.
  ASSERT_TRUE(object_id_map_.empty());

  // Verify that a map with an Object with this id is not effected by
  // a call to NewElement() with a non-Object which happens to have an
  // id attribute with this same value.
  kmldom::KmlFactory* factory = kmldom::KmlFactory::GetFactory();
  kmldom::PlacemarkPtr placemark = factory->CreatePlacemark();
  placemark->set_id(kId);
  ASSERT_TRUE(object_id_parser_observer_->NewElement(placemark));
  // Verify that a non-Object Element with an id does not return false.
  ASSERT_TRUE(object_id_parser_observer_->NewElement(root));

  // Verify that only a proper Object is in the map.
  ASSERT_EQ(static_cast<size_t>(1), object_id_map_.size());
  ASSERT_EQ(kmldom::Type_Placemark, object_id_map_[kId]->Type());
  ASSERT_EQ(kId, AsObject(object_id_map_[kId])->get_id());
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
