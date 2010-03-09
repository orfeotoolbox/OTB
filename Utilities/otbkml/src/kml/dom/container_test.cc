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

// This file contains the unit tests for the abstract Container element.

#include "kml/dom/container.h"
#include "gtest/gtest.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/placemark.h"

namespace kmldom {

class ContainerTest : public testing::Test {
 protected:
  virtual void SetUp() {
    container_ = new TestContainer();
  }

  // Container is abstract, hence its constructor is protected.
  class TestContainer : public Container {
  };
  boost::intrusive_ptr<TestContainer> container_;
};

TEST_F(ContainerTest, TestType) {
  ASSERT_TRUE(container_->IsA(Type_Container));
  ASSERT_TRUE(container_->IsA(Type_Feature));
  ASSERT_TRUE(container_->IsA(Type_Object));
}

TEST_F(ContainerTest, TestAddGetFeatures) {
  // Put each type of Feature in the Container.
  KmlFactory* factory = KmlFactory::GetFactory();
  container_->add_feature(factory->CreateDocument());
  container_->add_feature(factory->CreateFolder());
  container_->add_feature(factory->CreateGroundOverlay());
  container_->add_feature(factory->CreateNetworkLink());
  container_->add_feature(factory->CreatePhotoOverlay());
  container_->add_feature(factory->CreatePlacemark());
  container_->add_feature(factory->CreateScreenOverlay());
  // Verify order is preserved.
  ASSERT_EQ((size_t)7, container_->get_feature_array_size());
  ASSERT_EQ(Type_Document,
            container_->get_feature_array_at(0)->Type());
  ASSERT_EQ(Type_Folder,
            container_->get_feature_array_at(1)->Type());
  ASSERT_EQ(Type_GroundOverlay,
            container_->get_feature_array_at(2)->Type());
  ASSERT_EQ(Type_NetworkLink,
            container_->get_feature_array_at(3)->Type());
  ASSERT_EQ(Type_PhotoOverlay,
            container_->get_feature_array_at(4)->Type());
  ASSERT_EQ(Type_Placemark,
            container_->get_feature_array_at(5)->Type());
  ASSERT_EQ(Type_ScreenOverlay,
            container_->get_feature_array_at(6)->Type());
  // Deleting the container deletes all Features in the array.
}

TEST_F(ContainerTest, TestDeleteFeatureByIdOne) {
  KmlFactory* factory = KmlFactory::GetFactory();
  PlacemarkPtr placemark = factory->CreatePlacemark();
  const string id("placemark123");
  placemark->set_id(id);
  container_->add_feature(placemark);
  ASSERT_EQ(static_cast<size_t>(1), container_->get_feature_array_size());
  ASSERT_EQ(Type_Placemark, container_->get_feature_array_at(0)->Type());
  FeaturePtr feature = container_->DeleteFeatureById(id);
  ASSERT_EQ(static_cast<size_t>(0), container_->get_feature_array_size());
  ASSERT_EQ(id, feature->get_id());
  ASSERT_FALSE(container_->DeleteFeatureById("no-such-feature"));
}

// A proper XML id cannot be simply a number.
static string CreateId(size_t i) {
  return string("i") + kmlbase::ToString(i);
}

static const kmldom::KmlDomType kFeatures[] = {
  kmldom::Type_Folder, kmldom::Type_NetworkLink, kmldom::Type_GroundOverlay,
  kmldom::Type_Document, kmldom::Type_ScreenOverlay, kmldom::Type_Placemark,
  kmldom::Type_PhotoOverlay
};

// Create a Feature and give it an id based on i.
static FeaturePtr CreateFeature(size_t i) {
  int num_features = sizeof(kFeatures)/sizeof(kFeatures[0]);
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  FeaturePtr feature =
    AsFeature(kml_factory->CreateElementById(kFeatures[i % num_features]));
  feature->set_id(CreateId(i));
  return feature;
} 

TEST_F(ContainerTest, TestDeleteFeatureByIdMany) {
  const size_t kNumFeatures(123);
  for (size_t i = 0; i < kNumFeatures; ++i) {
    container_->add_feature(CreateFeature(i));
  }
  ASSERT_EQ(kNumFeatures, container_->get_feature_array_size());
  std::vector<FeaturePtr> deleted_features;
  for (size_t i = 0; i < kNumFeatures; i += 2) {
    deleted_features.push_back(container_->DeleteFeatureById(CreateId(i)));
  }
  const size_t new_size = container_->get_feature_array_size();
  ASSERT_EQ(kNumFeatures - deleted_features.size(), new_size);
  // Verify the container only has the odd features.
  for (size_t i = 0; i < new_size; ++i) {
    ASSERT_EQ(CreateId(2*i + 1), container_->get_feature_array_at(i)->get_id());
  }
  // Verfiy the deleted features are all even.
  for (size_t i = 0; i < deleted_features.size(); ++i) {
    ASSERT_EQ(CreateId(2*i), deleted_features[i]->get_id());
  }
  // TODO: Verify deleted features are dis-parented.
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
