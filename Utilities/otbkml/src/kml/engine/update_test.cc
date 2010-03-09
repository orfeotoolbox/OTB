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

#include "kml/engine/update.h"
#include "gtest/gtest.h"
#include "kml/base/file.h"
#include "kml/base/string_util.h"
#include "kml/base/vec3.h"
#include "kml/dom.h"
#include "kml/engine/kml_file.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

using kmlbase::File;
using kmlbase::Vec3;
using kmldom::ChangePtr;
using kmldom::ContainerPtr;
using kmldom::CoordinatesPtr;
using kmldom::DeletePtr;
using kmldom::FeaturePtr;
using kmldom::FolderPtr;
using kmldom::KmlFactory;
using kmldom::LineStringPtr;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;
using kmldom::UpdatePtr;
using kmlengine::KmlFile;
using kmlengine::KmlFilePtr;

namespace kmlengine {

static const char source_change[] =
    "<Update>"
    "<targetHref/>"
    "<Change>"
    "<Placemark targetId=\"p\">"
    "<name>NEW NAME</name>"
    "</Placemark>"
    "</Change>"
    "</Update>";

static const char target_change[] = 
    "<Placemark id=\"p\"><name>hi</name></Placemark>";

TEST(UpdateTest, TestProcessUpdateNull) {
  ProcessUpdate(NULL, NULL);
}

TEST(UpdateTest, TestSingleSimpleChange) {
  KmlFilePtr target_file = KmlFile::CreateFromParse(target_change, NULL);
  ASSERT_TRUE(target_file);
  PlacemarkPtr target_placemark =
      kmldom::AsPlacemark(target_file->GetObjectById("p"));
  ASSERT_EQ(string("hi"), target_placemark->get_name());
  ASSERT_TRUE(target_placemark);
  UpdatePtr update = AsUpdate(kmldom::Parse(source_change, NULL));
  ASSERT_TRUE(update);
  ProcessUpdate(update, target_file);
  ASSERT_EQ(string("NEW NAME"), target_placemark->get_name());
  ASSERT_FALSE(target_placemark->has_targetid());
}

static const char source_create[] =
    "<Update>"
    "<targetHref/>"
    "<Create>"
    "<Folder targetId=\"f\">"
    "<Placemark id=\"px\">"
    "<name>Update-Created Placemark</name>"
    "<Point>"
    "<coordinates>-11.11,22,22</coordinates>"
    "</Point>"
    "</Placemark>"
    "</Folder>"
    "</Create>"
    "</Update>";

static const char target_create[] = "<Folder id=\"f\"/>";

TEST(UpdateTest, TestSingleSimpleCreate) {
  KmlFilePtr target_file = KmlFile::CreateFromParse(target_create, NULL);
  ASSERT_TRUE(target_file);
  FolderPtr folder = kmldom::AsFolder(target_file->get_root());
  ASSERT_TRUE(folder);
  ASSERT_EQ(static_cast<size_t>(0), folder->get_feature_array_size());
  UpdatePtr update = AsUpdate(kmldom::Parse(source_create, NULL));
  ASSERT_TRUE(update);
  ProcessUpdate(update, target_file);
  ASSERT_EQ(static_cast<size_t>(1), folder->get_feature_array_size());
  ASSERT_EQ(string("Update-Created Placemark"),
            folder->get_feature_array_at(0)->get_name());
}

// Add one of each kind of Feature to the Container.
static void AddFeatures(ContainerPtr container) {
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  container->add_feature(kml_factory->CreateDocument());
  container->add_feature(kml_factory->CreateFolder());
  container->add_feature(kml_factory->CreateGroundOverlay());
  container->add_feature(kml_factory->CreateNetworkLink());
  container->add_feature(kml_factory->CreatePhotoOverlay());
  container->add_feature(kml_factory->CreatePlacemark());
  container->add_feature(kml_factory->CreateScreenOverlay());
}

static void CheckFeatures(const ContainerPtr& container, size_t offset) {
  ASSERT_LT(offset + 6, container->get_feature_array_size());
  ASSERT_EQ(kmldom::Type_Document,
            container->get_feature_array_at(offset)->Type());
  ASSERT_EQ(kmldom::Type_Folder,
            container->get_feature_array_at(offset + 1)->Type());
  ASSERT_EQ(kmldom::Type_GroundOverlay,
            container->get_feature_array_at(offset + 2)->Type());
  ASSERT_EQ(kmldom::Type_NetworkLink,
            container->get_feature_array_at(offset + 3)->Type());
  ASSERT_EQ(kmldom::Type_PhotoOverlay,
            container->get_feature_array_at(offset + 4)->Type());
  ASSERT_EQ(kmldom::Type_Placemark,
            container->get_feature_array_at(offset + 5)->Type());
  ASSERT_EQ(kmldom::Type_ScreenOverlay,
            container->get_feature_array_at(offset + 6)->Type());
}

TEST(UpdateTest, TestCopyFeatures) {
  const string kTargetName("target name");
  const string kTargetFeature0Name("0th feature");
  const string kTargetFeature1Name("1th feature");
  const string kTargetFeature9Name("9th feature");
  const string kSourceName("source name");
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  ContainerPtr source = kml_factory->CreateDocument();
  source->set_name(kSourceName);
  ContainerPtr target = kml_factory->CreateFolder();
  target->set_name(kTargetName);
  PlacemarkPtr placemark = kml_factory->CreatePlacemark();
  placemark->set_name(kTargetFeature0Name);
  target->add_feature(placemark);
  placemark = kml_factory->CreatePlacemark();
  placemark->set_name(kTargetFeature1Name);
  target->add_feature(placemark);
  AddFeatures(source);
  // Run the function under test.
  CopyFeatures(source, target);
  ASSERT_EQ(static_cast<size_t>(9), target->get_feature_array_size());
  // Only Features are copied.
  ASSERT_EQ(kTargetName, target->get_name());
  ASSERT_EQ(kSourceName, source->get_name());
  // Verify that original features are intact in target.
  ASSERT_EQ(kTargetFeature0Name, target->get_feature_array_at(0)->get_name());
  ASSERT_EQ(kTargetFeature1Name, target->get_feature_array_at(1)->get_name());
  // Verify that the copied features are now in the target.
  CheckFeatures(target, 2);
  // Add another Feature to the target directly.
  placemark = kml_factory->CreatePlacemark();
  placemark->set_name(kTargetFeature9Name);
  target->add_feature(placemark);
  // Copy from the source once again.
  CopyFeatures(source, target);
  ASSERT_EQ(static_cast<size_t>(17), target->get_feature_array_size());
  // Verify that original features are intact in target.
  ASSERT_EQ(kTargetFeature0Name, target->get_feature_array_at(0)->get_name());
  ASSERT_EQ(kTargetFeature1Name, target->get_feature_array_at(1)->get_name());
  ASSERT_EQ(kTargetFeature9Name, target->get_feature_array_at(9)->get_name());
  // Verify that the copied features are now in the target.
  CheckFeatures(target, 2);
  CheckFeatures(target, 10);
}

static const char source_delete[] =
    "<Update>"
    "<targetHref/>"
    "<Delete>"
    "<Placemark targetId=\"p\"/>"
    "</Delete>"
    "</Update>";

static const char target_delete[] = "<Folder><Placemark id=\"p\"/></Folder>";

TEST(UpdateTest, TestSingleSimpleDelete) {
  KmlFilePtr target_file = KmlFile::CreateFromParse(target_delete, NULL);
  ASSERT_TRUE(target_file);
  UpdatePtr update = AsUpdate(kmldom::Parse(source_delete, NULL));
  ASSERT_TRUE(update);
  ProcessUpdate(update, target_file);
  // Verify the Placemark has been removed from the Folder.
  FolderPtr folder = kmldom::AsFolder(target_file->get_root());
  ASSERT_TRUE(folder);
  ASSERT_EQ(static_cast<size_t>(0), folder->get_feature_array_size());
  // TODO: actually remove the object from the KmlFile's map(s).
  // Verify the KmlFile's id mapping for the Placemark is gone.
  // ASSERT_FALSE(target_file->GetObjectById("p"));
}

static const kmldom::KmlDomType kFeatures[] = {
  kmldom::Type_Placemark, kmldom::Type_Folder, kmldom::Type_Document,
  kmldom::Type_NetworkLink, kmldom::Type_GroundOverlay,
  kmldom::Type_ScreenOverlay, kmldom::Type_PhotoOverlay
};

// Create a Feature and give it an id based on i.  If id set id=, else targetId.
static FeaturePtr CreateFeature(int i, bool id) {
  int num_features = sizeof(kFeatures)/sizeof(kFeatures[0]);
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  FeaturePtr feature = AsFeature(kml_factory->CreateElementById(
      kFeatures[i % num_features]));
  const string kId(string("i") + kmlbase::ToString(i));
  if (id) {
    feature->set_id(kId);
  } else {
    feature->set_targetid(kId);
  }
  return feature;
}

TEST(UpdateTest, TestManyDeletes) {
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  FolderPtr folder = kml_factory->CreateFolder();
  const int kNumFeatures = 1237;
  for (int i = 0; i < kNumFeatures; ++i) {
    folder->add_feature(CreateFeature(i, true));  // Set id=
  }
  ASSERT_EQ(static_cast<size_t>(kNumFeatures),
            folder->get_feature_array_size());
  ASSERT_EQ(string("i0"), folder->get_feature_array_at(0)->get_id());
  ASSERT_EQ(kmldom::Type_Placemark, folder->get_feature_array_at(0)->Type());
  KmlFilePtr kml_file = KmlFile::CreateFromImport(folder);
  ASSERT_TRUE(kml_file);
  for (int i = 0; i < kNumFeatures; ++i) {
    DeletePtr deleet = kml_factory->CreateDelete();
    deleet->add_feature(CreateFeature(i, false));  // Set targetId=
    UpdatePtr update = kml_factory->CreateUpdate();
    update->set_targethref("");
    update->add_updateoperation(deleet);
    ProcessUpdate(update, kml_file);
    ASSERT_EQ(static_cast<size_t>(kNumFeatures - i - 1),
              folder->get_feature_array_size());
  }
}

// Update/Change on <coordinates> replaces the contents in the target from the
// source.
TEST(UpdateTest, TestChangeCoordinates) {
  KmlFactory* kml_factory = KmlFactory::GetFactory();

  // Create our target KML file.
  CoordinatesPtr coordinates = kml_factory->CreateCoordinates();
  const double kOrigLat(38.38);
  const double kOrigLon(-122.122);
  coordinates->add_latlng(kOrigLat, kOrigLon);
  PointPtr point = kml_factory->CreatePoint();
  point->set_coordinates(coordinates);
  PlacemarkPtr placemark = kml_factory->CreatePlacemark();
  const string kId("placemark123");
  const string kName("placemark name");
  placemark->set_id(kId);
  placemark->set_name(kName);
  placemark->set_geometry(point);
  KmlFilePtr kml_file = KmlFile::CreateFromImport(placemark);
  ASSERT_TRUE(kml_file);

  // Create the <Update> KML.
  coordinates = kml_factory->CreateCoordinates();
  const double kNewLat(-38.38);
  const double kNewLon(122.122);
  coordinates->add_latlng(kNewLat, kNewLon);
  point = kml_factory->CreatePoint();
  point->set_coordinates(coordinates);
  placemark = kml_factory->CreatePlacemark();
  placemark->set_targetid(kId);
  placemark->set_geometry(point);
  ChangePtr change = kml_factory->CreateChange();
  change->add_object(placemark);
  UpdatePtr update = kml_factory->CreateUpdate();
  update->add_updateoperation(change);

  // Call the function under test.
  ProcessUpdate(update, kml_file);
  // Verify the KML file's contents have changed.
  ASSERT_TRUE(kml_file);
  placemark = AsPlacemark(kml_file->get_root());
  ASSERT_TRUE(placemark);
  ASSERT_EQ(kId, placemark->get_id());
  ASSERT_EQ(kName, placemark->get_name());
  ASSERT_TRUE(placemark->has_geometry());
  point = AsPoint(placemark->get_geometry());
  ASSERT_TRUE(point);
  ASSERT_TRUE(point->has_coordinates());
  coordinates = point->get_coordinates();
  ASSERT_EQ(static_cast<size_t>(1), coordinates->get_coordinates_array_size());
  const Vec3& vec3 = coordinates->get_coordinates_array_at(0);
  ASSERT_EQ(kNewLat, vec3.get_latitude());
  ASSERT_EQ(kNewLon, vec3.get_longitude());
}

static const struct {
  const char* target_file_;  // Any valid KML file.
  const char* source_file_;  // <Update> is root element.
  const char* check_file_;  // Result of applying source to target.
} kTestCases [] = {
  {
    "/update/placemark.kml", "/update/change-placemark-styleurl.kml",
    "/update/change-placemark-styleurl-check.kml"
  },
  {
    "/update/placemark.kml", "/update/change-placemark-geometry.kml",
    "/update/change-placemark-geometry-check.kml"
  },
  {
    "/update/placemark.kml", "/update/change-placemark-geometry2x.kml",
    "/update/change-placemark-geometry2x-check.kml"
  },
  {
    "/update/california.kml", "/update/change-california-a.kml",
    "/update/change-california-a-check.kml"
  },
  {
    "/update/california.kml", "/update/california-delete-ad.kml",
    "/update/california-delete-ad-check.kml"
  },
  {
    "/update/california.kml", "/update/california-change-linestring.kml",
    "/update/california-change-linestring-check.kml"
  },
  {
    "/kml/kmlsamples.kml", "/update/kmlsamples-delete-many.kml",
    "/update/kmlsamples-delete-many-check.kml"
  },
  {
    "/kml/kmlsamples.kml", "/update/kmlsamples-change-many.kml",
    "/update/kmlsamples-change-many-check.kml"
  },
  {
    "/kml/kmlsamples.kml", "/update/kmlsamples-multi-update.kml",
    "/update/kmlsamples-multi-update-check.kml"
  }
};

// This is a utility function to parse the given string KML file.
static KmlFilePtr ParseTestCaseFile(const string& filename) {
  string kml_data;
  if (File::ReadFileToString(string(DATADIR) + filename, &kml_data)) {
    return KmlFile::CreateFromParse(kml_data, NULL);
  }
  return NULL;
}

// This function verifies all test cases in the kTestCases table.
TEST(UpdateTest, TestFiles) {
  const size_t size = sizeof(kTestCases)/sizeof(kTestCases[0]);
  for (size_t i = 0; i < size; ++i) {
    KmlFilePtr target = ParseTestCaseFile(kTestCases[i].target_file_);
    ASSERT_TRUE(target);
    KmlFilePtr source = ParseTestCaseFile(kTestCases[i].source_file_);
    ASSERT_TRUE(source);
    UpdatePtr update = AsUpdate(source->get_root());
    ASSERT_TRUE(update);
    ProcessUpdate(update, target);
    string actual;
    ASSERT_TRUE(target->SerializeToString(&actual));
    string expected;
    ASSERT_TRUE(
        File::ReadFileToString(string(DATADIR) + kTestCases[i].check_file_,
                               &expected));
    ASSERT_EQ(expected, actual);
  }
}

TEST(UpdateTest, TestProcessUpdateWithIdMapNull) {
  ProcessUpdateWithIdMap(NULL, NULL, NULL);
}

TEST(UpdateTest, TestProcessUpdateWithIdMapBasic) {
  KmlFilePtr kml_file(KmlFile::CreateFromString(
      "<Placemark id=\"inner\"><name>old name</name></Placemark>"));
  ASSERT_TRUE(kml_file);

  kmldom::UpdatePtr update = kmldom::AsUpdate(kmldom::ParseKml(
      "<Update>"
      "  <Change>"
      "    <Placemark targetId=\"outer\">"
      "      <name>new name</name>"
      "    </Placemark>"
      "  </Change>"
      "</Update>"));
  ASSERT_TRUE(update);

  kmlbase::StringMap id_map;
  id_map["outer"] = "inner";

  ProcessUpdateWithIdMap(update, &id_map, kml_file);

  PlacemarkPtr placemark = AsPlacemark(kml_file->get_root());
  ASSERT_TRUE(placemark);
  ASSERT_TRUE(placemark->has_name());
  ASSERT_EQ(string("new name"), placemark->get_name());
  ASSERT_TRUE(placemark->has_id());
  ASSERT_EQ(string("inner"), placemark->get_id());
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
