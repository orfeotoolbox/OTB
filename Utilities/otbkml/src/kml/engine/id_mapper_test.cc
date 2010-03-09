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

// This file contains the unit tests for the GetElementsById() function
// and the IdMapper class.

#include "kml/engine/id_mapper.h"
#include "kml/engine/id_mapper_internal.h"
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"
#include "kml/base/file.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kmldom.h"
#include "kml/dom/xsd.h"
#include "kml/engine/find.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

using kmlbase::File;
using kmldom::CoordinatesPtr;
using kmldom::ElementPtr;
using kmldom::FolderPtr;
using kmldom::GroundOverlayPtr;
using kmldom::KmlFactory;
using kmldom::ObjectPtr;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;

namespace kmlengine {

class IdMapperTest : public testing::Test {
 protected:
  virtual void SetUp() {
    coordinates_ = KmlFactory::GetFactory()->CreateCoordinates();
    folder0_ = KmlFactory::GetFactory()->CreateFolder();
    folder1_ = KmlFactory::GetFactory()->CreateFolder();
    groundoverlay_ = KmlFactory::GetFactory()->CreateGroundOverlay();
    placemark0_ = KmlFactory::GetFactory()->CreatePlacemark();
    placemark1_ = KmlFactory::GetFactory()->CreatePlacemark();
    placemark2_ = KmlFactory::GetFactory()->CreatePlacemark();
    point0_ = KmlFactory::GetFactory()->CreatePoint();
    point1_ = KmlFactory::GetFactory()->CreatePoint();
    id_mapper_.reset(new IdMapper(&object_id_map_, &dup_id_vector_));
  }

  CoordinatesPtr coordinates_;
  FolderPtr folder0_;
  FolderPtr folder1_;
  GroundOverlayPtr groundoverlay_;
  PlacemarkPtr placemark0_;
  PlacemarkPtr placemark1_;
  PlacemarkPtr placemark2_;
  PointPtr point0_;
  PointPtr point1_;
  ObjectIdMap object_id_map_;
  ElementVector dup_id_vector_;
  boost::scoped_ptr<IdMapper> id_mapper_;
};

TEST_F(IdMapperTest, TestIdMapperEmpty) {
  // Save an element with no id.
  id_mapper_->SaveElement(folder0_);
  ASSERT_TRUE(dup_id_vector_.empty());
  ASSERT_TRUE(object_id_map_.empty());
}

TEST_F(IdMapperTest, TestIdMapperOne) {
  // Save an element with an id.
  const string kPlacemark0Id("id-of-placemark0");
  placemark0_->set_id(kPlacemark0Id);
  id_mapper_->SaveElement(placemark0_);
  ASSERT_TRUE(dup_id_vector_.empty());
  ASSERT_EQ(static_cast<size_t>(1), object_id_map_.size());
  ASSERT_EQ(kPlacemark0Id, object_id_map_[kPlacemark0Id]->get_id());
}

// Verify MapIds() on an element with an id.
TEST_F(IdMapperTest, TestMapIdsOne) {
  // Save an element with an id.
  const string kPlacemark0Id("id-of-placemark0");
  placemark0_->set_id(kPlacemark0Id);
  MapIds(placemark0_, &object_id_map_, &dup_id_vector_);
  ASSERT_TRUE(dup_id_vector_.empty());
  ASSERT_EQ(static_cast<size_t>(1), object_id_map_.size());
  ASSERT_EQ(kPlacemark0Id, object_id_map_[kPlacemark0Id]->get_id());
}

// Verify MapIds() is well behaved when presented with no dup vector pointer.
TEST_F(IdMapperTest, TestMapIdsNoDupVector) {
  // Save an element with an id.
  const string kPlacemark0Id("id-of-placemark0");
  placemark0_->set_id(kPlacemark0Id);
  MapIds(placemark0_, &object_id_map_, NULL);
  ASSERT_EQ(static_cast<size_t>(1), object_id_map_.size());
  ASSERT_EQ(kPlacemark0Id, object_id_map_[kPlacemark0Id]->get_id());
}

TEST_F(IdMapperTest, TestIdMapperDupe) {
  // Save an element whose child has the same id.
  const string kDupeId("xml-does-not-like-duplicate-ids");
  point0_->set_id(kDupeId);
  placemark0_->set_id(kDupeId);
  placemark0_->set_geometry(point0_);
  id_mapper_->SaveElement(placemark0_);
  // The dupe will be the parent due element simply because it comes first.
  ASSERT_EQ(static_cast<size_t>(1), dup_id_vector_.size());
  ASSERT_EQ(kmldom::Type_Placemark, dup_id_vector_[0]->Type());
  // The id map will hold the last item of this id.
  ASSERT_EQ(kDupeId, object_id_map_[kDupeId]->get_id());
  ASSERT_EQ(kmldom::Type_Point, object_id_map_[kDupeId]->Type());
}

TEST_F(IdMapperTest, TestNoIds) {
  MapIds(folder0_, &object_id_map_, &dup_id_vector_);
  ASSERT_TRUE(object_id_map_.empty());
  ASSERT_TRUE(dup_id_vector_.empty());
  folder0_->add_feature(placemark0_);
  folder0_->add_feature(placemark1_);
  folder0_->add_feature(placemark2_);
  // Still nothing with an id.
  MapIds(folder0_, &object_id_map_, &dup_id_vector_);
  ASSERT_TRUE(object_id_map_.empty());
  ASSERT_TRUE(dup_id_vector_.empty());
}

TEST_F(IdMapperTest, TestSomeIds) {
  const string kId0("id0");
  const string kName0("name0");
  placemark0_->set_id(kId0);
  placemark0_->set_name(kName0);
  const string kId1("id1");
  const string kName1("name1");
  placemark1_->set_id(kId1);
  placemark1_->set_name(kName1);
  folder0_->add_feature(placemark0_);
  folder0_->add_feature(placemark1_);
  MapIds(folder0_, &object_id_map_, &dup_id_vector_);
  // Verify the map has exactly the two expected mappings.
  ASSERT_EQ(static_cast<size_t>(2), object_id_map_.size());
  // Verifies 1) it was mapped, 2) it's a Placemark, 3) it has the right name.
  ASSERT_EQ(kName0, AsPlacemark(object_id_map_[kId0])->get_name());
  ASSERT_EQ(kName1, AsPlacemark(object_id_map_[kId1])->get_name());
  ASSERT_TRUE(dup_id_vector_.empty());
}

TEST_F(IdMapperTest, TestAllObjects) {
  string kml;
  ASSERT_TRUE(File::ReadFileToString(
      File::JoinPaths(DATADIR, File::JoinPaths("kml",
                                               "all-unknown-attrs-input.kml")),
      &kml));
  ElementPtr root = kmldom::Parse(kml, NULL);
  MapIds(root, &object_id_map_, &dup_id_vector_);
  ASSERT_TRUE(dup_id_vector_.empty());
  ASSERT_EQ(static_cast<size_t>(44), object_id_map_.size());
  ObjectIdMap::const_iterator iter = object_id_map_.begin();
  for (; iter != object_id_map_.end(); ++iter) {
    const ObjectPtr& object = iter->second;
    const string& id = iter->first;
    // The id of the object is the id that maps to the object.
    ASSERT_EQ(id, object->get_id());
    // Each id in all-unknown-attrs-input.kml is the same as the element name.
    ASSERT_EQ(kmldom::Xsd::GetSchema()->ElementId(id), object->Type());
  }
}

TEST_F(IdMapperTest, TestDupeDetection) {
  // Create a KML hierarchy with duplicate ids.
  const string kDupeId("some-id-to-duplicate");
  placemark0_->set_id(kDupeId);
  folder0_->set_id(kDupeId);
  folder0_->add_feature(placemark0_);
  // Call the method under test.
  MapIds(folder0_, &object_id_map_, &dup_id_vector_);
  ASSERT_EQ(static_cast<size_t>(1), dup_id_vector_.size());
}

TEST_F(IdMapperTest, TestClearIds) {
  placemark0_->set_id("placemark123");
  const string kFolderName("folder name");
  folder0_->set_id("folder123");
  folder0_->set_name(kFolderName);
  folder0_->add_feature(placemark0_);
  FolderPtr folder = AsFolder(ClearIds(folder0_));
  ASSERT_TRUE(folder);
  ASSERT_TRUE(folder->has_name());
  ASSERT_EQ(kFolderName, folder->get_name());
  ASSERT_EQ(static_cast<size_t>(1), folder->get_feature_array_size());
  ASSERT_FALSE(placemark0_->has_id());
  ASSERT_FALSE(folder0_->has_id());
}

TEST_F(IdMapperTest, TestClearManyIds) {
  string kml;
  ASSERT_TRUE(File::ReadFileToString(
      File::JoinPaths(DATADIR, File::JoinPaths("kml",
                                               "all-unknown-attrs-input.kml")),
      &kml));
  ElementPtr root = kmldom::Parse(kml, NULL);
  MapIds(root, &object_id_map_, NULL);
  ASSERT_EQ(static_cast<size_t>(44), object_id_map_.size());

  // Call the method under test.
  ClearIds(root);
  object_id_map_.clear();
  MapIds(root, &object_id_map_, NULL);
  ASSERT_TRUE(object_id_map_.empty());
}

// Verify well defined behavior on NULL/empty inputs.
TEST_F(IdMapperTest, TestRemapIdsNull) {
  kmlbase::StringMap id_map;
  ASSERT_EQ(0, RemapIds(object_id_map_, id_map, NULL));
}

// Verify remapping for a simple case.
TEST_F(IdMapperTest, TestRemapIdsSimple) {
  const string kId0("id0");
  const string kName0("name0");
  placemark0_->set_id(kId0);
  placemark0_->set_name(kName0);
  const string kId1("id1");
  const string kName1("name1");
  placemark1_->set_id(kId1);
  placemark1_->set_name(kName1);
  folder0_->add_feature(placemark0_);
  folder0_->add_feature(placemark1_);
  MapIds(folder0_, &object_id_map_, NULL);

  kmlbase::StringMap id_map;
  id_map[kId0] = "newid0";
  id_map[kId1] = "newid1";
  ASSERT_EQ(0, RemapIds(object_id_map_, id_map, NULL));
  ASSERT_TRUE(placemark0_->has_id());
  ASSERT_EQ(id_map[kId0], placemark0_->get_id());
  ASSERT_TRUE(placemark1_->has_id());
  ASSERT_EQ(id_map[kId1], placemark1_->get_id());
}

// Verify remapping for a simple case with output object_id_map.
TEST_F(IdMapperTest, TestRemapIdsSimpleWithOutput) {
  const string kId0("id0");
  const string kName0("name0");
  placemark0_->set_id(kId0);
  placemark0_->set_name(kName0);
  const string kId1("id1");
  const string kName1("name1");
  placemark1_->set_id(kId1);
  placemark1_->set_name(kName1);
  const string kFolderId("folder-id");
  folder0_->set_id(kFolderId);
  folder0_->add_feature(placemark0_);
  folder0_->add_feature(placemark1_);
  MapIds(folder0_, &object_id_map_, NULL);

  // This map has mappings for 2 of the ids and one extra.
  kmlbase::StringMap id_map;
  id_map[kId0] = "newid0";
  id_map[kId1] = "newid1";
  id_map["no-such-id"] = "still-no-such-id";
  ObjectIdMap output_object_id_map;
  // One Object's id was cleared...
  ASSERT_EQ(1, RemapIds(object_id_map_, id_map, &output_object_id_map));
  // Two were re-mapped.
  ASSERT_EQ(static_cast<size_t>(2), output_object_id_map.size());
  ASSERT_TRUE(placemark0_->has_id());
  ASSERT_EQ(id_map[kId0], placemark0_->get_id());
  ASSERT_TRUE(placemark1_->has_id());
  ASSERT_EQ(id_map[kId0], output_object_id_map[id_map[kId0]]->get_id());
  ASSERT_EQ(id_map[kId1], output_object_id_map[id_map[kId1]]->get_id());
  // Any Object w/o an id mapping is cleared.
  ASSERT_FALSE(folder0_->has_id());
}

TEST_F(IdMapperTest, TestRemapManyIds) {
  string kml;
  ASSERT_TRUE(File::ReadFileToString(
      File::JoinPaths(DATADIR, File::JoinPaths("kml",
                                               "all-unknown-attrs-input.kml")),
      &kml));
  ElementPtr root = kmldom::Parse(kml, NULL);
  MapIds(root, &object_id_map_, NULL);
  ASSERT_EQ(static_cast<size_t>(44), object_id_map_.size());

  // Create a "newid-OLDID" for half the objects in the file.
  const string kNewIdBase("newid-");
  kmlbase::StringMap id_map;
  ObjectIdMap::const_iterator iter = object_id_map_.begin();
  for (; iter != object_id_map_.end(); ++iter, ++iter) {
    id_map[iter->first] = kNewIdBase + iter->first;
  }

  ObjectIdMap output_object_id_map;
  ASSERT_EQ(22, RemapIds(object_id_map_, id_map, &output_object_id_map));
  ASSERT_EQ(static_cast<size_t>(22), output_object_id_map.size());

  ElementVector all_objects;
  GetElementsById(root, kmldom::Type_Object, &all_objects);
  ASSERT_EQ(static_cast<size_t>(110), all_objects.size());
  ElementVector::const_iterator element_iter = all_objects.begin();
  int clear_id_count = 0;
  for (; element_iter != all_objects.end(); ++element_iter) {
    const kmldom::ObjectPtr& object = AsObject(*element_iter);
    if (object->has_id()) {

      ASSERT_EQ(kNewIdBase, object->get_id().substr(0, kNewIdBase.size()));
    } else {
      ++clear_id_count;
    }
  }
  ASSERT_EQ(88, clear_id_count);
}

TEST_F(IdMapperTest, TestUnknownElements) {
  const string kStyleId("hist1_b");
  const string kIconStyleId("khIconStyle671");
  const string kKml(string("<Style id='") + kStyleId + "'>"
    "<IconStyle id='" + kIconStyleId + "'>"
    "<color>ff00f6ff</color>"
    "<scale>0.7</scale>"
    "<Icon>"
    "<href>root://icons/palette-4.png</href>"
    "<x>64</x>"
    "<y>128</y>"
    "<w>32</w>"
    "<h>32</h>"
    "</Icon>"
    "</IconStyle>"
    "</Style>");
  ElementPtr root = kmldom::Parse(kKml, NULL);
  ASSERT_TRUE(root);
  MapIds(root, &object_id_map_, NULL);
  ASSERT_EQ(static_cast<size_t>(2), object_id_map_.size());

  ObjectIdMap::const_iterator find = object_id_map_.find(kIconStyleId);
  ASSERT_FALSE(find == object_id_map_.end());
  kmldom::IconStylePtr iconstyle =
      kmldom::AsIconStyle(object_id_map_[kIconStyleId]);
  ASSERT_TRUE(iconstyle);
  ASSERT_EQ(kIconStyleId, iconstyle->get_id());

  find = object_id_map_.find(kStyleId);
  ASSERT_FALSE(find == object_id_map_.end());
  kmldom::StylePtr style = kmldom::AsStyle(object_id_map_[kStyleId]);
  ASSERT_TRUE(iconstyle);
  ASSERT_EQ(kStyleId, style->get_id());
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
