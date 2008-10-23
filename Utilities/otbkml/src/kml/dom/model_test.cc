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

#include <string>
#include "kml/base/unit_test.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/model.h"

namespace kmldom {

// This tests the Location class:
class LocationTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(LocationTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before all tests.
  void setUp() {
    location_ = KmlFactory::GetFactory()->CreateLocation();
  }

  // Called after all tests.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  LocationPtr location_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(LocationTest);

void LocationTest::TestType() {
  CPPUNIT_ASSERT(Type_Location == location_->Type());
  CPPUNIT_ASSERT(true == location_->IsA(Type_Location));
  CPPUNIT_ASSERT(true == location_->IsA(Type_Object));
}

// Verify proper defaults:
void LocationTest::TestDefaults() {
  CPPUNIT_ASSERT(false == location_->has_id());
  CPPUNIT_ASSERT(false == location_->has_targetid());
  CPPUNIT_ASSERT(false == location_->has_longitude());
  CPPUNIT_ASSERT(0.0 == location_->get_longitude());
  CPPUNIT_ASSERT(false == location_->has_latitude());
  CPPUNIT_ASSERT(0.0 == location_->get_latitude());
  CPPUNIT_ASSERT(false == location_->has_altitude());
  CPPUNIT_ASSERT(ALTITUDEMODE_CLAMPTOGROUND == location_->get_altitude());
}

// Verify setting default makes has_xxx() true:
void LocationTest::TestSetToDefaultValues() {
  // Verify that location_ is in default state:
  CPPUNIT_ASSERT(false == location_->has_longitude());
  CPPUNIT_ASSERT(false == location_->has_latitude());
  CPPUNIT_ASSERT(false == location_->has_altitude());
  location_->set_longitude(location_->get_longitude());
  location_->set_latitude(location_->get_latitude());
  location_->set_altitude(location_->get_altitude());
  CPPUNIT_ASSERT(true == location_->has_longitude());
  CPPUNIT_ASSERT(true == location_->has_latitude());
  CPPUNIT_ASSERT(true == location_->has_altitude());
}

// Verify set, get, has, clear:
void LocationTest::TestSetGetHasClear() {
  // Non-default values:
  const double longitude = 22.33;
  const double latitude = -54.321;
  const double altitude = 6543.21;

  // Set all fields:
  location_->set_longitude(longitude);
  location_->set_latitude(latitude);
  location_->set_altitude(altitude);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == location_->has_longitude());
  CPPUNIT_ASSERT(longitude == location_->get_longitude());
  CPPUNIT_ASSERT(true == location_->has_latitude());
  CPPUNIT_ASSERT(latitude == location_->get_latitude());
  CPPUNIT_ASSERT(true == location_->has_altitude());
  CPPUNIT_ASSERT(altitude == location_->get_altitude());

  // Clear all fields:
  location_->clear_longitude();
  location_->clear_latitude();
  location_->clear_altitude();

  // Verify now in default state:
  TestDefaults();
}

// This tests the Orientation class:
class OrientationTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(OrientationTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before all tests.
  void setUp() {
    orientation_ = KmlFactory::GetFactory()->CreateOrientation();
  }

  // Called after all tests.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  OrientationPtr orientation_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(OrientationTest);

void OrientationTest::TestType() {
  CPPUNIT_ASSERT(Type_Orientation == orientation_->Type());
  CPPUNIT_ASSERT(true == orientation_->IsA(Type_Orientation));
  CPPUNIT_ASSERT(true == orientation_->IsA(Type_Object));
}

// Verify proper defaults:
void OrientationTest::TestDefaults() {
  CPPUNIT_ASSERT(false == orientation_->has_id());
  CPPUNIT_ASSERT(false == orientation_->has_targetid());
  CPPUNIT_ASSERT(false == orientation_->has_heading());
  CPPUNIT_ASSERT(0.0 == orientation_->get_heading());
  CPPUNIT_ASSERT(false == orientation_->has_tilt());
  CPPUNIT_ASSERT(0.0 == orientation_->get_tilt());
  CPPUNIT_ASSERT(false == orientation_->has_roll());
  CPPUNIT_ASSERT(ALTITUDEMODE_CLAMPTOGROUND == orientation_->get_roll());
}

// Verify setting default makes has_xxx() true:
void OrientationTest::TestSetToDefaultValues() {
  // Verify that orientation_ is in default state:
  CPPUNIT_ASSERT(false == orientation_->has_heading());
  CPPUNIT_ASSERT(false == orientation_->has_tilt());
  CPPUNIT_ASSERT(false == orientation_->has_roll());
  orientation_->set_heading(orientation_->get_heading());
  orientation_->set_tilt(orientation_->get_tilt());
  orientation_->set_roll(orientation_->get_roll());
  CPPUNIT_ASSERT(true == orientation_->has_heading());
  CPPUNIT_ASSERT(true == orientation_->has_tilt());
  CPPUNIT_ASSERT(true == orientation_->has_roll());
}

// Verify set, get, has, clear:
void OrientationTest::TestSetGetHasClear() {
  // Non-default values:
  const double heading = -5.46;
  const double tilt = 12.45;
  const double roll = -45.6789;

  // Set all fields:
  orientation_->set_heading(heading);
  orientation_->set_tilt(tilt);
  orientation_->set_roll(roll);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == orientation_->has_heading());
  CPPUNIT_ASSERT(heading == orientation_->get_heading());
  CPPUNIT_ASSERT(true == orientation_->has_tilt());
  CPPUNIT_ASSERT(tilt == orientation_->get_tilt());
  CPPUNIT_ASSERT(true == orientation_->has_roll());
  CPPUNIT_ASSERT(roll == orientation_->get_roll());

  // Clear all fields:
  orientation_->clear_heading();
  orientation_->clear_tilt();
  orientation_->clear_roll();

  // Verify now in default state:
  TestDefaults();
}

// This tests the Scale class:
class ScaleTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ScaleTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before all tests.
  void setUp() {
    scale_ = KmlFactory::GetFactory()->CreateScale();
  }

  // Called after all tests.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  ScalePtr scale_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ScaleTest);

void ScaleTest::TestType() {
  CPPUNIT_ASSERT(Type_Scale == scale_->Type());
  CPPUNIT_ASSERT(true == scale_->IsA(Type_Scale));
  CPPUNIT_ASSERT(true == scale_->IsA(Type_Object));
}

// Verify proper defaults:
void ScaleTest::TestDefaults() {
  CPPUNIT_ASSERT(false == scale_->has_id());
  CPPUNIT_ASSERT(false == scale_->has_targetid());
  CPPUNIT_ASSERT(false == scale_->has_x());
  CPPUNIT_ASSERT(1.0 == scale_->get_x());
  CPPUNIT_ASSERT(false == scale_->has_y());
  CPPUNIT_ASSERT(1.0 == scale_->get_y());
  CPPUNIT_ASSERT(false == scale_->has_z());
  CPPUNIT_ASSERT(1.0 == scale_->get_z());
}

// Verify setting default makes has_xxx() true:
void ScaleTest::TestSetToDefaultValues() {
  // Verify that scale_ in default state:
  CPPUNIT_ASSERT(false == scale_->has_x());
  CPPUNIT_ASSERT(false == scale_->has_y());
  CPPUNIT_ASSERT(false == scale_->has_z());
  scale_->set_x(scale_->get_x());
  scale_->set_y(scale_->get_y());
  scale_->set_z(scale_->get_z());
  CPPUNIT_ASSERT(true == scale_->has_x());
  CPPUNIT_ASSERT(true == scale_->has_y());
  CPPUNIT_ASSERT(true == scale_->has_z());
}

// Verify set, get, has, clear:
void ScaleTest::TestSetGetHasClear() {
  // Non-default values:
  const double x = 1.23;
  const double y = -2.41;
  const double z = 0.4;

  // Set all fields:
  scale_->set_x(x);
  scale_->set_y(y);
  scale_->set_z(z);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == scale_->has_x());
  CPPUNIT_ASSERT(x == scale_->get_x());
  CPPUNIT_ASSERT(true == scale_->has_y());
  CPPUNIT_ASSERT(y == scale_->get_y());
  CPPUNIT_ASSERT(true == scale_->has_z());
  CPPUNIT_ASSERT(z == scale_->get_z());

  // Clear all fields:
  scale_->clear_x();
  scale_->clear_y();
  scale_->clear_z();

  // Verify now in default state:
  TestDefaults();
}

// This tests the Alias class:
class AliasTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(AliasTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before all tests.
  void setUp() {
    alias_ = KmlFactory::GetFactory()->CreateAlias();
  }

  // Called after all tests.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  AliasPtr alias_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(AliasTest);

void AliasTest::TestType() {
  CPPUNIT_ASSERT(Type_Alias == alias_->Type());
  CPPUNIT_ASSERT(true == alias_->IsA(Type_Alias));
  CPPUNIT_ASSERT(true == alias_->IsA(Type_Object));
}

// Verify proper defaults:
void AliasTest::TestDefaults() {
  CPPUNIT_ASSERT(false == alias_->has_id());
  CPPUNIT_ASSERT(false == alias_->has_targetid());
  CPPUNIT_ASSERT(false == alias_->has_targethref());
  CPPUNIT_ASSERT("" == alias_->get_targethref());
  CPPUNIT_ASSERT(false == alias_->has_sourcehref());
  CPPUNIT_ASSERT("" == alias_->get_sourcehref());
}

// Verify setting default makes has_xxx() true:
void AliasTest::TestSetToDefaultValues() {
  // Verify that alias_ is in default state:
  CPPUNIT_ASSERT(false == alias_->has_targethref());
  CPPUNIT_ASSERT(false == alias_->has_sourcehref());
  alias_->set_targethref(alias_->get_targethref());
  alias_->set_sourcehref(alias_->get_sourcehref());
  CPPUNIT_ASSERT(true == alias_->has_targethref());
  CPPUNIT_ASSERT(true == alias_->has_sourcehref());
}

// Verify set, get, has, clear:
void AliasTest::TestSetGetHasClear() {
  // Non-default values:
  const std::string targethref(
      "../textures/CU-Macky-Center-StairsnoCulling.jpg");
  const std::string sourcehref(
      "../files/CU-Macky---Center-StairsnoCulling.jpg");

  // Set all fields:
  alias_->set_targethref(targethref);
  alias_->set_sourcehref(sourcehref);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == alias_->has_targethref());
  CPPUNIT_ASSERT(targethref == alias_->get_targethref());
  CPPUNIT_ASSERT(true == alias_->has_sourcehref());
  CPPUNIT_ASSERT(sourcehref == alias_->get_sourcehref());

  // Clear all fields:
  alias_->clear_targethref();
  alias_->clear_sourcehref();

  // Verify now in default state:
  TestDefaults();
}

// This tests the ResourceMap class:
class ResourceMapTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ResourceMapTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestAddAliases);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before all tests.
  void setUp() {
    resourcemap_ = KmlFactory::GetFactory()->CreateResourceMap();
  }

  // Called after all tests.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestAddAliases();

 private:
  ResourceMapPtr resourcemap_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ResourceMapTest);

void ResourceMapTest::TestType() {
  CPPUNIT_ASSERT(Type_ResourceMap == resourcemap_->Type());
  CPPUNIT_ASSERT(true == resourcemap_->IsA(Type_ResourceMap));
  CPPUNIT_ASSERT(true == resourcemap_->IsA(Type_Object));
}

void ResourceMapTest::TestDefaults() {
  CPPUNIT_ASSERT(false == resourcemap_->has_id());
  CPPUNIT_ASSERT(false == resourcemap_->has_targetid());
  CPPUNIT_ASSERT(0 == resourcemap_->get_alias_array_size());
}

void ResourceMapTest::TestAddAliases() {
  // Verify proper initial conditions for this test:
  CPPUNIT_ASSERT(0 == resourcemap_->get_alias_array_size());

  // Create an Alias:
  AliasPtr alias = KmlFactory::GetFactory()->CreateAlias();
  const std::string targethref0("target0.jpg");
  const std::string sourcehref0("source0.jpg");
  alias->set_targethref(targethref0);
  alias->set_sourcehref(sourcehref0);

  // Add it to ResourceMap:
  resourcemap_->add_alias(alias);

  // Verify this is the one and only item on the array:
  CPPUNIT_ASSERT(1 == resourcemap_->get_alias_array_size());

  // Verify the alias is the one we set:
  CPPUNIT_ASSERT(targethref0 ==
                 resourcemap_->get_alias_array_at(0)->get_targethref());
  CPPUNIT_ASSERT(sourcehref0 ==
                 resourcemap_->get_alias_array_at(0)->get_sourcehref());

  // Create another Alias and add to ResourceMap
  alias = KmlFactory::GetFactory()->CreateAlias();
  const std::string targethref1("target1.jpg");
  const std::string sourcehref1("source1.jpg");
  alias->set_targethref(targethref1);
  alias->set_sourcehref(sourcehref1);
  resourcemap_->add_alias(alias);

  // Verify the overall state of the alais array:
  CPPUNIT_ASSERT(2 == resourcemap_->get_alias_array_size());
  CPPUNIT_ASSERT(targethref0 ==
                 resourcemap_->get_alias_array_at(0)->get_targethref());
  CPPUNIT_ASSERT(sourcehref0 ==
                 resourcemap_->get_alias_array_at(0)->get_sourcehref());
  CPPUNIT_ASSERT(targethref1 ==
                 resourcemap_->get_alias_array_at(1)->get_targethref());
  CPPUNIT_ASSERT(sourcehref1 ==
                 resourcemap_->get_alias_array_at(1)->get_sourcehref());

  // aliases will be deleted when resourcemap_ is deleted
}

// This tests the Model class:
class ModelTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(ModelTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST(TestSetParent);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before all tests.
  void setUp() {
    model_ = KmlFactory::GetFactory()->CreateModel();
  }

  // Called after all tests.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetGetHasClear();
  void TestSetParent();

 private:
  ModelPtr model_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ModelTest);

// Verify type is correct:
void ModelTest::TestType() {
  CPPUNIT_ASSERT(Type_Model == model_->Type());
  CPPUNIT_ASSERT(true == model_->IsA(Type_Model));
  CPPUNIT_ASSERT(true == model_->IsA(Type_Geometry));
  CPPUNIT_ASSERT(true == model_->IsA(Type_Object));
}

// Verify all child elements are in default state:
void ModelTest::TestDefaults() {
  CPPUNIT_ASSERT(false == model_->has_id());
  CPPUNIT_ASSERT(false == model_->has_targetid());
  CPPUNIT_ASSERT(ALTITUDEMODE_CLAMPTOGROUND == model_->get_altitudemode());
  CPPUNIT_ASSERT(false == model_->has_location());
  CPPUNIT_ASSERT(false == model_->has_orientation());
  CPPUNIT_ASSERT(false == model_->has_scale());
  CPPUNIT_ASSERT(false == model_->has_link());
  CPPUNIT_ASSERT(false == model_->has_resourcemap());
}

// Verify set, get, has, clear methods:
void ModelTest::TestSetGetHasClear() {
  // Set the only simple child to a non-default value:
  model_->set_altitudemode(ALTITUDEMODE_ABSOLUTE);
  CPPUNIT_ASSERT(true == model_->has_altitudemode());
  CPPUNIT_ASSERT(ALTITUDEMODE_ABSOLUTE == model_->get_altitudemode());

  // Create all possible complex children and give to the Model:
  model_->set_location(KmlFactory::GetFactory()->CreateLocation());
  model_->set_orientation(KmlFactory::GetFactory()->CreateOrientation());
  model_->set_scale(KmlFactory::GetFactory()->CreateScale());
  model_->set_link(KmlFactory::GetFactory()->CreateLink());
  model_->set_resourcemap(KmlFactory::GetFactory()->CreateResourceMap());
  CPPUNIT_ASSERT(true == model_->has_location());
  CPPUNIT_ASSERT(true == model_->has_orientation());
  CPPUNIT_ASSERT(true == model_->has_scale());
  CPPUNIT_ASSERT(true == model_->has_link());
  CPPUNIT_ASSERT(true == model_->has_resourcemap());

  // Clear all children:
  model_->clear_altitudemode();
  model_->clear_location();
  model_->clear_orientation();
  model_->clear_scale();
  model_->clear_link();
  model_->clear_resourcemap();

  // Verify Model is now in default state:
  TestDefaults();
}

// Verify that 2 Models can't take the same Location, Orientation, Scale,
// Link or ResourceMap.
// (This tests the internal set_parent() method.)
void ModelTest::TestSetParent() {
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
  CPPUNIT_ASSERT(model_->has_location());
  CPPUNIT_ASSERT(model_->has_orientation());
  CPPUNIT_ASSERT(model_->has_scale());
  CPPUNIT_ASSERT(model_->has_link());
  CPPUNIT_ASSERT(model_->has_resourcemap());

  // Verify that model2 has no children.
  CPPUNIT_ASSERT(false == model2->has_location());
  CPPUNIT_ASSERT(false == model2->has_orientation());
  CPPUNIT_ASSERT(false == model2->has_scale());
  CPPUNIT_ASSERT(false == model2->has_link());
  CPPUNIT_ASSERT(false == model2->has_resourcemap());

  // smart pointer deletes model_ and the children created here.
}

}  // end namespace kmldom

TEST_MAIN
