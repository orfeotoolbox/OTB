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

// This file contains the unittests for Model, Location, Orientation, Scale,
// ResourceMap and Alias.
// TODO: Parse,Serialize tests

#include "gtest/gtest.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/model.h"

namespace kmldom {

// This tests the Location class:
class LocationTest : public testing::Test {
 protected:
  // Called before all tests.
  virtual void SetUp() {
    location_ = KmlFactory::GetFactory()->CreateLocation();
  }

  LocationPtr location_;
};

TEST_F(LocationTest, TestType) {
  ASSERT_EQ(Type_Location, location_->Type());
  ASSERT_TRUE(location_->IsA(Type_Location));
  ASSERT_TRUE(location_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(LocationTest, TestDefaults) {
  ASSERT_FALSE(location_->has_id());
  ASSERT_FALSE(location_->has_targetid());
  ASSERT_FALSE(location_->has_longitude());
  ASSERT_DOUBLE_EQ(0.0, location_->get_longitude());
  ASSERT_FALSE(location_->has_latitude());
  ASSERT_DOUBLE_EQ(0.0, location_->get_latitude());
  ASSERT_FALSE(location_->has_altitude());
  ASSERT_EQ(ALTITUDEMODE_CLAMPTOGROUND, location_->get_altitude());
}

// Verify setting default makes has_xxx() true:
TEST_F(LocationTest, TestSetToDefaultValues) {
  // Verify that location_ is in default state:
  ASSERT_FALSE(location_->has_longitude());
  ASSERT_FALSE(location_->has_latitude());
  ASSERT_FALSE(location_->has_altitude());
  location_->set_longitude(location_->get_longitude());
  location_->set_latitude(location_->get_latitude());
  location_->set_altitude(location_->get_altitude());
  ASSERT_TRUE(location_->has_longitude());
  ASSERT_TRUE(location_->has_latitude());
  ASSERT_TRUE(location_->has_altitude());
}

// Verify set, get, has, clear:
TEST_F(LocationTest, TestSetGetHasClear) {
  // Non-default values:
  const double longitude = 22.33;
  const double latitude = -54.321;
  const double altitude = 6543.21;

  // Set all fields:
  location_->set_longitude(longitude);
  location_->set_latitude(latitude);
  location_->set_altitude(altitude);

  // Verify getter and has_xxx():
  ASSERT_TRUE(location_->has_longitude());
  ASSERT_DOUBLE_EQ(longitude, location_->get_longitude());
  ASSERT_TRUE(location_->has_latitude());
  ASSERT_DOUBLE_EQ(latitude, location_->get_latitude());
  ASSERT_TRUE(location_->has_altitude());
  ASSERT_DOUBLE_EQ(altitude, location_->get_altitude());

  // Clear all fields:
  location_->clear_longitude();
  location_->clear_latitude();
  location_->clear_altitude();
}

// This tests the Orientation class:
class OrientationTest : public testing::Test {
 protected:
  virtual void SetUp() {
    orientation_ = KmlFactory::GetFactory()->CreateOrientation();
  }

  OrientationPtr orientation_;
};

TEST_F(OrientationTest, TestType) {
  ASSERT_EQ(Type_Orientation, orientation_->Type());
  ASSERT_TRUE(orientation_->IsA(Type_Orientation));
  ASSERT_TRUE(orientation_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(OrientationTest, TestDefaults) {
  ASSERT_FALSE(orientation_->has_id());
  ASSERT_FALSE(orientation_->has_targetid());
  ASSERT_FALSE(orientation_->has_heading());
  ASSERT_DOUBLE_EQ(0.0, orientation_->get_heading());
  ASSERT_FALSE(orientation_->has_tilt());
  ASSERT_DOUBLE_EQ(0.0, orientation_->get_tilt());
  ASSERT_FALSE(orientation_->has_roll());
  ASSERT_EQ(ALTITUDEMODE_CLAMPTOGROUND, orientation_->get_roll());
}

// Verify setting default makes has_xxx() true:
TEST_F(OrientationTest, TestSetToDefaultValues) {
  // Verify that orientation_ is in default state:
  ASSERT_FALSE(orientation_->has_heading());
  ASSERT_FALSE(orientation_->has_tilt());
  ASSERT_FALSE(orientation_->has_roll());
  orientation_->set_heading(orientation_->get_heading());
  orientation_->set_tilt(orientation_->get_tilt());
  orientation_->set_roll(orientation_->get_roll());
  ASSERT_TRUE(orientation_->has_heading());
  ASSERT_TRUE(orientation_->has_tilt());
  ASSERT_TRUE(orientation_->has_roll());
}

// Verify set, get, has, clear:
TEST_F(OrientationTest, TestSetGetHasClear) {
  // Non-default values:
  const double heading = -5.46;
  const double tilt = 12.45;
  const double roll = -45.6789;

  // Set all fields:
  orientation_->set_heading(heading);
  orientation_->set_tilt(tilt);
  orientation_->set_roll(roll);

  // Verify getter and has_xxx():
  ASSERT_TRUE(orientation_->has_heading());
  ASSERT_DOUBLE_EQ(heading, orientation_->get_heading());
  ASSERT_TRUE(orientation_->has_tilt());
  ASSERT_DOUBLE_EQ(tilt, orientation_->get_tilt());
  ASSERT_TRUE(orientation_->has_roll());
  ASSERT_DOUBLE_EQ(roll, orientation_->get_roll());

  // Clear all fields:
  orientation_->clear_heading();
  orientation_->clear_tilt();
  orientation_->clear_roll();
}

// This tests the Scale class:
class ScaleTest : public testing::Test {
 protected:
  virtual void SetUp() {
    scale_ = KmlFactory::GetFactory()->CreateScale();
  }

  ScalePtr scale_;
};

TEST_F(ScaleTest, TestType) {
  ASSERT_EQ(Type_Scale, scale_->Type());
  ASSERT_TRUE(scale_->IsA(Type_Scale));
  ASSERT_TRUE(scale_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(ScaleTest, TestDefaults) {
  ASSERT_FALSE(scale_->has_id());
  ASSERT_FALSE(scale_->has_targetid());
  ASSERT_FALSE(scale_->has_x());
  ASSERT_DOUBLE_EQ(1.0, scale_->get_x());
  ASSERT_FALSE(scale_->has_y());
  ASSERT_DOUBLE_EQ(1.0, scale_->get_y());
  ASSERT_FALSE(scale_->has_z());
  ASSERT_DOUBLE_EQ(1.0, scale_->get_z());
}

// Verify setting default makes has_xxx() true:
TEST_F(ScaleTest, TestSetToDefaultValues) {
  // Verify that scale_ in default state:
  ASSERT_FALSE(scale_->has_x());
  ASSERT_FALSE(scale_->has_y());
  ASSERT_FALSE(scale_->has_z());
  scale_->set_x(scale_->get_x());
  scale_->set_y(scale_->get_y());
  scale_->set_z(scale_->get_z());
  ASSERT_TRUE(scale_->has_x());
  ASSERT_TRUE(scale_->has_y());
  ASSERT_TRUE(scale_->has_z());
}

// Verify set, get, has, clear:
TEST_F(ScaleTest, TestSetGetHasClear) {
  // Non-default values:
  const double x = 1.23;
  const double y = -2.41;
  const double z = 0.4;

  // Set all fields:
  scale_->set_x(x);
  scale_->set_y(y);
  scale_->set_z(z);

  // Verify getter and has_xxx():
  ASSERT_TRUE(scale_->has_x());
  ASSERT_DOUBLE_EQ(x, scale_->get_x());
  ASSERT_TRUE(scale_->has_y());
  ASSERT_DOUBLE_EQ(y, scale_->get_y());
  ASSERT_TRUE(scale_->has_z());
  ASSERT_DOUBLE_EQ(z, scale_->get_z());

  // Clear all fields:
  scale_->clear_x();
  scale_->clear_y();
  scale_->clear_z();
}

// This tests the Alias class:
class AliasTest : public testing::Test {
 protected:
  virtual void SetUp() {
    alias_ = KmlFactory::GetFactory()->CreateAlias();
  }

  AliasPtr alias_;
};

TEST_F(AliasTest, TestType) {
  ASSERT_EQ(Type_Alias, alias_->Type());
  ASSERT_TRUE(alias_->IsA(Type_Alias));
  ASSERT_TRUE(alias_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(AliasTest, TestDefaults) {
  ASSERT_FALSE(alias_->has_id());
  ASSERT_FALSE(alias_->has_targetid());
  ASSERT_FALSE(alias_->has_targethref());
  ASSERT_EQ(string(""), alias_->get_targethref());
  ASSERT_FALSE(alias_->has_sourcehref());
  ASSERT_EQ(string(""), alias_->get_sourcehref());
}

// Verify setting default makes has_xxx() true:
TEST_F(AliasTest, TestSetToDefaultValues) {
  // Verify that alias_ is in default state:
  ASSERT_FALSE(alias_->has_targethref());
  ASSERT_FALSE(alias_->has_sourcehref());
  alias_->set_targethref(alias_->get_targethref());
  alias_->set_sourcehref(alias_->get_sourcehref());
  ASSERT_TRUE(alias_->has_targethref());
  ASSERT_TRUE(alias_->has_sourcehref());
}

// Verify set, get, has, clear:
TEST_F(AliasTest, TestSetGetHasClear) {
  // Non-default values:
  const string targethref(
      "../textures/CU-Macky-Center-StairsnoCulling.jpg");
  const string sourcehref(
      "../files/CU-Macky---Center-StairsnoCulling.jpg");

  // Set all fields:
  alias_->set_targethref(targethref);
  alias_->set_sourcehref(sourcehref);

  // Verify getter and has_xxx():
  ASSERT_TRUE(alias_->has_targethref());
  ASSERT_EQ(targethref, alias_->get_targethref());
  ASSERT_TRUE(alias_->has_sourcehref());
  ASSERT_EQ(sourcehref, alias_->get_sourcehref());

  // Clear all fields:
  alias_->clear_targethref();
  alias_->clear_sourcehref();
}

// Verify Serialize.
TEST_F(AliasTest, TestSerialize) {
  ASSERT_EQ(string("<Alias/>"), SerializeRaw(alias_));
}

// This tests the ResourceMap class:
class ResourceMapTest : public testing::Test {
 protected:
  virtual void SetUp() {
    resourcemap_ = KmlFactory::GetFactory()->CreateResourceMap();
  }

  ResourceMapPtr resourcemap_;
};

TEST_F(ResourceMapTest, TestType) {
  ASSERT_EQ(Type_ResourceMap, resourcemap_->Type());
  ASSERT_TRUE(resourcemap_->IsA(Type_ResourceMap));
  ASSERT_TRUE(resourcemap_->IsA(Type_Object));
}

TEST_F(ResourceMapTest, TestDefaults) {
  ASSERT_FALSE(resourcemap_->has_id());
  ASSERT_FALSE(resourcemap_->has_targetid());
  ASSERT_EQ(static_cast<size_t>(0), resourcemap_->get_alias_array_size());
}

TEST_F(ResourceMapTest, TestAddAliases) {
  // Verify proper initial conditions for this test:
  ASSERT_EQ(static_cast<size_t>(0), resourcemap_->get_alias_array_size());

  // Create an Alias:
  AliasPtr alias = KmlFactory::GetFactory()->CreateAlias();
  const string targethref0("target0.jpg");
  const string sourcehref0("source0.jpg");
  alias->set_targethref(targethref0);
  alias->set_sourcehref(sourcehref0);

  // Add it to ResourceMap:
  resourcemap_->add_alias(alias);

  // Verify this is the one and only item on the array:
  ASSERT_EQ(static_cast<size_t>(1), resourcemap_->get_alias_array_size());

  // Verify the alias is the one we set:
  ASSERT_EQ(targethref0,
            resourcemap_->get_alias_array_at(0)->get_targethref());
  ASSERT_EQ(sourcehref0,
            resourcemap_->get_alias_array_at(0)->get_sourcehref());

  // Create another Alias and add to ResourceMap
  alias = KmlFactory::GetFactory()->CreateAlias();
  const string targethref1("target1.jpg");
  const string sourcehref1("source1.jpg");
  alias->set_targethref(targethref1);
  alias->set_sourcehref(sourcehref1);
  resourcemap_->add_alias(alias);

  // Verify the overall state of the alais array:
  ASSERT_EQ(static_cast<size_t>(2), resourcemap_->get_alias_array_size());
  ASSERT_EQ(targethref0,
            resourcemap_->get_alias_array_at(0)->get_targethref());
  ASSERT_EQ(sourcehref0,
            resourcemap_->get_alias_array_at(0)->get_sourcehref());
  ASSERT_EQ(targethref1,
            resourcemap_->get_alias_array_at(1)->get_targethref());
  ASSERT_EQ(sourcehref1,
            resourcemap_->get_alias_array_at(1)->get_sourcehref());

  // aliases will be deleted when resourcemap_ is deleted
}

// This tests the Model class:
class ModelTest : public testing::Test {
 protected:
  virtual void SetUp() {
    model_ = KmlFactory::GetFactory()->CreateModel();
  }

  ModelPtr model_;
};

// Verify type is correct:
TEST_F(ModelTest, TestType) {
  ASSERT_EQ(Type_Model, model_->Type());
  ASSERT_TRUE(model_->IsA(Type_Model));
  ASSERT_TRUE(model_->IsA(Type_Geometry));
  ASSERT_TRUE(model_->IsA(Type_Object));
}

// Verify all child elements are in default state:
TEST_F(ModelTest, TestDefaults) {
  ASSERT_FALSE(model_->has_id());
  ASSERT_FALSE(model_->has_targetid());
  ASSERT_EQ(ALTITUDEMODE_CLAMPTOGROUND, model_->get_altitudemode());
  ASSERT_EQ(GX_ALTITUDEMODE_CLAMPTOSEAFLOOR, model_->get_gx_altitudemode());
  ASSERT_FALSE(model_->has_location());
  ASSERT_FALSE(model_->has_orientation());
  ASSERT_FALSE(model_->has_scale());
  ASSERT_FALSE(model_->has_link());
  ASSERT_FALSE(model_->has_resourcemap());
}

// Verify set, get, has, clear methods:
TEST_F(ModelTest, TestSetGetHasClear) {
  // Set the only simple child to a non-default value:
  model_->set_altitudemode(ALTITUDEMODE_ABSOLUTE);
  ASSERT_TRUE(model_->has_altitudemode());
  ASSERT_EQ(ALTITUDEMODE_ABSOLUTE, model_->get_altitudemode());
  model_->set_gx_altitudemode(GX_ALTITUDEMODE_RELATIVETOSEAFLOOR);
  ASSERT_TRUE(model_->has_gx_altitudemode());
  ASSERT_EQ(GX_ALTITUDEMODE_RELATIVETOSEAFLOOR, model_->get_gx_altitudemode());

  // Create all possible complex children and give to the Model:
  model_->set_location(KmlFactory::GetFactory()->CreateLocation());
  model_->set_orientation(KmlFactory::GetFactory()->CreateOrientation());
  model_->set_scale(KmlFactory::GetFactory()->CreateScale());
  model_->set_link(KmlFactory::GetFactory()->CreateLink());
  model_->set_resourcemap(KmlFactory::GetFactory()->CreateResourceMap());
  ASSERT_TRUE(model_->has_location());
  ASSERT_TRUE(model_->has_orientation());
  ASSERT_TRUE(model_->has_scale());
  ASSERT_TRUE(model_->has_link());
  ASSERT_TRUE(model_->has_resourcemap());

  // Clear all children:
  model_->clear_altitudemode();
  model_->clear_gx_altitudemode();
  model_->clear_location();
  model_->clear_orientation();
  model_->clear_scale();
  model_->clear_link();
  model_->clear_resourcemap();

  // Verify Model is now in default state:
}

// Verify that 2 Models can't take the same Location, Orientation, Scale,
// Link or ResourceMap.
// (This tests the internal set_parent() method.)
TEST_F(ModelTest, TestSetParent) {
  KmlFactory* factory = KmlFactory::GetFactory();
  LocationPtr location = factory->CreateLocation();
  OrientationPtr orientation = factory->CreateOrientation();
  ScalePtr scale = factory->CreateScale();
  LinkPtr link = factory->CreateLink();
  ResourceMapPtr resourcemap = factory->CreateResourceMap();

  // Give these all to model_.
  model_->set_location(location);
  model_->set_orientation(orientation);
  model_->set_scale(scale);
  model_->set_link(link);
  model_->set_resourcemap(resourcemap);

  // Try to give these all to another model.
  ModelPtr model2 = factory->CreateModel();
  model2->set_location(location);
  model2->set_orientation(orientation);
  model2->set_scale(scale);
  model2->set_link(link);
  model2->set_resourcemap(resourcemap);

  // Verify that model_ has each child.
  ASSERT_TRUE(model_->has_location());
  ASSERT_TRUE(model_->has_orientation());
  ASSERT_TRUE(model_->has_scale());
  ASSERT_TRUE(model_->has_link());
  ASSERT_TRUE(model_->has_resourcemap());

  // Verify that model2 has no children.
  ASSERT_FALSE(model2->has_location());
  ASSERT_FALSE(model2->has_orientation());
  ASSERT_FALSE(model2->has_scale());
  ASSERT_FALSE(model2->has_link());
  ASSERT_FALSE(model2->has_resourcemap());

  // smart pointer deletes model_ and the children created here.
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
