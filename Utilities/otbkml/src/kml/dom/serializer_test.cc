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

// This file contains the unit tests for the abstract Serializer base class.

#include "kml/dom/serializer.h"
#include "kml/base/attributes.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kmldom.h"
#include "kml/dom/stats_serializer.h"
#include "gtest/gtest.h"

using kmlbase::Attributes;

namespace kmldom {

class SerializerTest : public testing::Test {
 protected:
  virtual void SetUp() {
    kml_factory_ = KmlFactory::GetFactory();
    document_ = kml_factory_->CreateDocument();
    folder_ = kml_factory_->CreateFolder();
    placemark_ = kml_factory_->CreatePlacemark();
    point_ = kml_factory_->CreatePoint();
    region_ = kml_factory_->CreateRegion();
    style_ = kml_factory_->CreateStyle();
    style_map_ = kml_factory_->CreateStyleMap();
  }

  KmlFactory* kml_factory_;
  DocumentPtr document_;
  FolderPtr folder_;
  PlacemarkPtr placemark_;
  PointPtr point_;
  RegionPtr region_;
  StylePtr style_;
  StyleMapPtr style_map_;
};

// The NullSerializer implementation overrides no Serializer virtual methods.
// This verifies that Serializer has no pure virtual methods.
class NullSerializer : public Serializer {
};

// This Serializer implementation provides implementations for all virtual
// methods.  This should build and run and do nothing.
class MaximalSerializer : public Serializer {
 public:
  virtual void BeginById(int type_id, const Attributes& attributes) {}
  virtual void End() {}
  virtual void SaveElement(const ElementPtr& element) {}
  virtual void SaveElementGroup(const ElementPtr& element, int group_id) {}
  virtual void SaveStringFieldById(int type_id, string value) {}
  virtual void SaveContent(const string& content, bool maybe_quote) {}
  virtual void SaveVec3(const kmlbase::Vec3& vec3) {}
  virtual void Indent() {}
  virtual void SaveColor(int type_id, const kmlbase::Color32& color) {}
  virtual void BeginElementArray(int type_id, size_t element_count) {}
  virtual void EndElementArray(int type_id) {}
  virtual void BeginElementGroupArray(int group_id, size_t element_count) {}
  virtual void EndElementGroupArray(int group_id) {}
};

typedef std::vector<KmlDomType> TypeIdVector;

// By default Serializer recurses down every complex element.  By appending
// each visited complex element to a list in its BeginById() method this
// serializer provides the means to verify proper depth-first order recursion.
class HierarchicalSerializer : public Serializer {
 public:
  virtual void BeginById(int type_id, const Attributes& attributes) {
    type_id_vector_.push_back(static_cast<KmlDomType>(type_id));
  }

  const TypeIdVector& get_type_id_vector() const {
    return type_id_vector_;
  }

 private:
  TypeIdVector type_id_vector_;
};

typedef std::vector<ElementPtr> ElementVector;

// This serializer implements SaveElement() and does not recurse on complex
// child elements, but it does append to the list of those visited.
class FlatSerializer : public Serializer {
 public:
  virtual void SaveElement(const ElementPtr& element) {
    element_vector_.push_back(element);
  }

  const ElementVector& get_element_vector() const {
    return element_vector_;
  }

 private:
  ElementVector element_vector_;
};

// This serializer implements SaveColor() only.
typedef std::pair<int, kmlbase::Color32> TypeColorPair;
typedef std::vector<TypeColorPair> ColorVector;
class ColorSerializer : public Serializer {
 public:
  virtual void SaveColor(int type_id, const kmlbase::Color32& color) {
    color_vector_.push_back(std::make_pair(type_id, color));
  }

  const ColorVector& get_color_vector() const { return color_vector_; }
 
 private:
  ColorVector color_vector_;
};

// This exists because Serialize is public only on Element.
static void CallSerializer(const ElementPtr& element, Serializer* serializer) {
  ASSERT_TRUE(element);  // This is basically an internal check.
  ASSERT_TRUE(serializer);  // This is basically an internal check.
  element->Serialize(*serializer);
}

// Assert the basic SaveElement is well-behaved when passed a NULL element.
TEST_F(SerializerTest, TestSerializeOfNullElement) {
  Serializer serializer;
  serializer.SaveElement(NULL);
}

// Verify that the default Serializer properly does nothing.
TEST_F(SerializerTest, TestNullSerializer) {
  Serializer serializer;
  CallSerializer(placemark_, &serializer);
  NullSerializer null_serializer;
  CallSerializer(placemark_, &null_serializer);
  MaximalSerializer max_serializer;
  CallSerializer(placemark_, &max_serializer);
}

// Verify that the framework calls out to the Serializer-based class the
// expected number of times for the very simple case of a complex element
// with no fields or child elements.
TEST_F(SerializerTest, TestStatsSerializerOnEmptyElement) {
  StatsSerializer stats_serializer;
  CallSerializer(placemark_, &stats_serializer);
  // Once for <Placemark>
  ASSERT_EQ(1, stats_serializer.get_begin_count());
  // Once for </Placemark>
  ASSERT_EQ(1, stats_serializer.get_end_count());
  // No child simple elements (fields).
  ASSERT_EQ(0, stats_serializer.get_field_count());
  // No child complex elements.
  ASSERT_EQ(0, stats_serializer.get_element_count());
  // No child complex elements in substitution groups.
  ASSERT_EQ(0, stats_serializer.get_element_group_count());
}

// Verify that the framework calls out to the Serializer-based class as
// expected for a complex element with some fields.
TEST_F(SerializerTest, TestStatsSerializerOnFields) {
  StatsSerializer stats_serializer;
  placemark_->set_id("id");  // This is known to be an attribute.
  placemark_->set_name("hi");  // This is known to be a field (<name>).
  // This is known to be a field (<visibility>).
  placemark_->set_visibility(true);
  CallSerializer(placemark_, &stats_serializer);
  // 1: <Placemark>
  ASSERT_EQ(1, stats_serializer.get_begin_count());
  // 1: </Placemark>
  ASSERT_EQ(1, stats_serializer.get_end_count());
  // 2: <name>, <visibility>
  ASSERT_EQ(2, stats_serializer.get_field_count());
  // No child complex elements.
  ASSERT_EQ(0, stats_serializer.get_element_count());
  // No child complex elements in substitution groups.
  ASSERT_EQ(0, stats_serializer.get_element_group_count());
}

// Verify that the framework calls out to the Serializer-based class as
// expected for a hierarchy of complex elements.
TEST_F(SerializerTest, TestStatsSerializerOnChildren) {
  StatsSerializer stats_serializer;
  placemark_->set_geometry(point_);
  placemark_->set_region(region_);
  folder_->add_feature(placemark_);
  CallSerializer(folder_, &stats_serializer);
  // 3: <Folder> <Placemark> <Region> <Point>
  ASSERT_EQ(4, stats_serializer.get_begin_count());
  // 3: </Point> </Region> </Placemark> </Folder>
  ASSERT_EQ(4, stats_serializer.get_end_count());
  // 0: none of the complex elements have attributes or fields
  ASSERT_EQ(0, stats_serializer.get_field_count());
  // 2: 1 for Folder's Placemark + 1 for Placemark's Point + 1 for Placemark's
  // Region.
  ASSERT_EQ(3, stats_serializer.get_element_count());
  // Placemark is a Feature in Folder, and Point is Geometry in Placemark.
  // Region is not in a group.
  ASSERT_EQ(2, stats_serializer.get_element_group_count());

  // Verify that a serializer which provides no implementation of SaveElement()
  // recurses down the hierarchy of complex elements.
  HierarchicalSerializer hier_serializer;
  CallSerializer(folder_, &hier_serializer);
  const TypeIdVector& type_id_vector = hier_serializer.get_type_id_vector();
  ASSERT_EQ(static_cast<size_t>(4), type_id_vector.size());
  ASSERT_EQ(Type_Folder, type_id_vector[0]);
  ASSERT_EQ(Type_Placemark, type_id_vector[1]);
  ASSERT_EQ(Type_Region, type_id_vector[2]);
  ASSERT_EQ(Type_Point, type_id_vector[3]);

  // Verify that a serializer which provides a non-recursing implementation
  // of SaveElement() merely visits each complex element.
  FlatSerializer flat_serializer;
  CallSerializer(placemark_, &flat_serializer);
  const ElementVector& element_vector = flat_serializer.get_element_vector();
  ASSERT_EQ(static_cast<size_t>(2), element_vector.size());
  ASSERT_EQ(Type_Region, element_vector[0]->Type());
  ASSERT_EQ(Type_Point, element_vector[1]->Type());
}

TEST_F(SerializerTest, TestSaveColor) {
  const kmlbase::Color32 kOpaqueWhite(0xffffffff);
  const kmlbase::Color32 kOpaqueBlack(0xff000000);
  const kmlbase::Color32 kOpaqueBlue(0xffff0000);
  ColorSerializer color_serializer;
  color_serializer.SaveColor(Type_bgColor, kOpaqueWhite);
  color_serializer.SaveColor(Type_color, kOpaqueBlack);
  color_serializer.SaveColor(Type_textColor, kOpaqueBlue);

  const ColorVector& color_vector = color_serializer.get_color_vector();
  ASSERT_EQ(static_cast<size_t>(3), color_vector.size()); 
  ASSERT_EQ(Type_bgColor, color_vector[0].first);
  ASSERT_TRUE(kOpaqueWhite == color_vector[0].second);
  ASSERT_EQ(Type_color, color_vector[1].first);
  ASSERT_TRUE(kOpaqueBlack == color_vector[1].second);
  ASSERT_EQ(Type_textColor, color_vector[2].first);
  ASSERT_TRUE(kOpaqueBlue == color_vector[2].second);
}

// This Serializer implementation provides implementations for virtual methods
// used to serialize element arrays.  This simply logs every id of everything
// it sees.
typedef std::vector<int> IntVector;
class ArraySerializer : public Serializer {
 public:
  ArraySerializer(IntVector* int_vector)
    : int_vector_(int_vector) {
  }

  // Called for each non-substitution-group element including each element
  // in an array.
  virtual void SaveElement(const ElementPtr& element) {
    int_vector_->push_back(element->Type());
  }

  // Called for each substitution-group element including each element
  // in an array.
  virtual void SaveElementGroup(const ElementPtr& element, int group_id) {
    int_vector_->push_back(element->Type());
    int_vector_->push_back(group_id);
  }

  // Called before calling SaveElement on each element in an array.  The
  // element_count is the number of elements in the array and the type_id
  // is the type of each element.
  virtual void BeginElementArray(int type_id, size_t element_count) {
    int_vector_->push_back(type_id);
    int_vector_->push_back(static_cast<int>(element_count));
  }

  // Called after saving each element in an array.  Every element was of
  // the given type.
  virtual void EndElementArray(int type_id) {
    int_vector_->push_back(type_id);
  }

  // Called before calling SaveElementGroup on each element in an array.  The
  // element_count is the number of elements in the array and the group_id
  // is the substitution group type of each element.  Examples of group_id
  // in KML include Type_Feature, Type_Object, and Type_StyleSelector.
  virtual void BeginElementGroupArray(int group_id, size_t element_count) {
    int_vector_->push_back(group_id);
    int_vector_->push_back(static_cast<int>(element_count));
  }

  // Called after saving each group element in an array.  Every element was of
  // the given group type.
  virtual void EndElementGroupArray(int group_id) {
    int_vector_->push_back(group_id);
  }

 private:
  IntVector* int_vector_;
};

// Test Serializer::SaveElementGroupArray.
TEST_F(SerializerTest, TestSaveElementGroupArray) {
  document_->set_region(region_);
  document_->add_feature(kml_factory_->CreatePlacemark());
  document_->add_feature(kml_factory_->CreateGroundOverlay());
  document_->add_feature(kml_factory_->CreateScreenOverlay());

  IntVector int_vector;
  ArraySerializer array_serializer(&int_vector);
  CallSerializer(document_, &array_serializer);

  ASSERT_EQ(static_cast<size_t>(10), int_vector.size());
  // The order presumes that of KML Document.
  // SaveElement(Region)
  ASSERT_EQ(Type_Region, int_vector[0]);
  // BeginElementGroupArray(Type_Feature, 3)
  ASSERT_EQ(Type_Feature, int_vector[1]);
  ASSERT_EQ(3, int_vector[2]);
  // SaveElementGroup(Placemark)
  ASSERT_EQ(Type_Placemark, int_vector[3]);
  ASSERT_EQ(Type_Feature, int_vector[4]);
  // SaveElementGroup(GroundOverlay)
  ASSERT_EQ(Type_GroundOverlay, int_vector[5]);
  ASSERT_EQ(Type_Feature, int_vector[6]);
  // SaveElementGroup(ScreenOverlay)
  ASSERT_EQ(Type_ScreenOverlay, int_vector[7]);
  ASSERT_EQ(Type_Feature, int_vector[8]);
  // EndElementGroupArray(Type_Feature)
  ASSERT_EQ(Type_Feature, int_vector[9]);

}

// Test Serializer::SaveElementArray.
TEST_F(SerializerTest, TestSaveElementArray) {
  document_->set_region(region_);
  document_->add_schema(kml_factory_->CreateSchema());
  document_->add_schema(kml_factory_->CreateSchema());

  IntVector int_vector;
  ArraySerializer array_serializer(&int_vector);
  CallSerializer(document_, &array_serializer);

  ASSERT_EQ(static_cast<size_t>(6), int_vector.size());
  // SaveElement(Region)
  ASSERT_EQ(Type_Region, int_vector[0]);
  // BeginElementArray(Type_Schema, 2)
  ASSERT_EQ(Type_Schema, int_vector[1]);
  ASSERT_EQ(2, int_vector[2]);
  // SaveElement(Schema) x 2
  ASSERT_EQ(Type_Schema, int_vector[3]);
  ASSERT_EQ(Type_Schema, int_vector[4]);
  // EndElementArray(Type_Schema)
  ASSERT_EQ(Type_Schema, int_vector[5]);
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
