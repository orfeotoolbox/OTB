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
#include <string>
#include "kml/dom/attributes.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kmldom.h"
#include "kml/base/unit_test.h"

namespace kmldom {

class SerializerTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(SerializerTest);
  CPPUNIT_TEST(TestNullSerializer);
  CPPUNIT_TEST(TestStatsSerializerOnEmptyElement);
  CPPUNIT_TEST(TestStatsSerializerOnFields);
  CPPUNIT_TEST(TestStatsSerializerOnChildren);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestNullSerializer();
  void TestStatsSerializerOnEmptyElement();
  void TestStatsSerializerOnFields();
  void TestStatsSerializerOnChildren();

 public:
  // Called before each test.
  void setUp() {
    folder_ = KmlFactory::GetFactory()->CreateFolder();
    placemark_ = KmlFactory::GetFactory()->CreatePlacemark();
    point_ = KmlFactory::GetFactory()->CreatePoint();
  }

  // Called after each test.
  void tearDown() {
    // Nothing to tear down due to use of smart pointers.
  }

 private:
  FolderPtr folder_;
  PlacemarkPtr placemark_;
  PointPtr point_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(SerializerTest);

// This Serializer implementation counts begin and end tags of complex elements
// and a count of all simple elements (fields).
class StatsSerializer : public Serializer {
 public:
  StatsSerializer() : begin_count_(0), end_count_(0), field_count_(0) {}
  virtual void BeginById(int type_id, const Attributes& attributes) {
    ++begin_count_;
  }
  virtual void End() {
    ++end_count_;
  }
  virtual void SaveStringFieldById(int type_id, std::string value) {
    ++field_count_;
  }
  virtual void SaveContent(const std::string& content, bool maybe_quote) {
    ++content_count_;
  }
  virtual void SaveElement(const ElementPtr& element) {
    ++element_count_;
    Serializer::SaveElement(element);
  }
  int get_begin_count() const {
    return begin_count_;
  }
  int get_end_count() const {
    return end_count_;
  }
  int get_field_count() const {
    return field_count_;
  }

 private:
  int begin_count_;
  int end_count_;
  int field_count_;
  int element_count_;
  int content_count_;
};

// This exists because Serialize is public only on Element.
static void CallSerializer(const ElementPtr& element, Serializer* serializer) {
  CPPUNIT_ASSERT(element);  // This is basically an internal check.
  CPPUNIT_ASSERT(serializer);  // This is basically an internal check.
  element->Serialize(*serializer);
}

// Verify that the default Serializer properly does nothing.
void SerializerTest::TestNullSerializer() {
  Serializer null_serializer;
  CallSerializer(placemark_, &null_serializer);
}

// Verify that the framework calls out to the Serializer-based class the
// expected number of times for the very simple case of a complex element
// with no fields or child elements.
void SerializerTest::TestStatsSerializerOnEmptyElement() {
  StatsSerializer stats_serializer;
  CallSerializer(placemark_, &stats_serializer);
  // Once for <Placemark>
  CPPUNIT_ASSERT_EQUAL(1, stats_serializer.get_begin_count());
  // Once for </Placemark>
  CPPUNIT_ASSERT_EQUAL(1, stats_serializer.get_end_count());
  // No child elements.
  CPPUNIT_ASSERT_EQUAL(0, stats_serializer.get_field_count());
}

// Verify that the framework calls out to the Serializer-based class as
// expected for a complex element with some fields.
void SerializerTest::TestStatsSerializerOnFields() {
  StatsSerializer stats_serializer;
  placemark_->set_id("id");  // This is known to be an attribute.
  placemark_->set_name("hi");  // This is known to be a field (<name>).
  // This is known to be a field (<visibility>).
  placemark_->set_visibility(true);
  CallSerializer(placemark_, &stats_serializer);
  // 1: <Placemark>
  CPPUNIT_ASSERT_EQUAL(1, stats_serializer.get_begin_count());
  // 1: </Placemark>
  CPPUNIT_ASSERT_EQUAL(1, stats_serializer.get_end_count());
  // 2: <name>, <visibility>
  CPPUNIT_ASSERT_EQUAL(2, stats_serializer.get_field_count());
}

// Verify that the framework calls out to the Serializer-based class as
// expected for a hierarchy of complex elements.
void SerializerTest::TestStatsSerializerOnChildren() {
  StatsSerializer stats_serializer;
  placemark_->set_geometry(point_);
  folder_->add_feature(placemark_);
  CallSerializer(folder_, &stats_serializer);
  // 3: <Folder> <Placemark> <Point>
  CPPUNIT_ASSERT_EQUAL(3, stats_serializer.get_begin_count());
  // 3: </Point> </Placemark> </Folder>
  CPPUNIT_ASSERT_EQUAL(3, stats_serializer.get_end_count());
  // 0: none of the complex elements have attributes or fields
  CPPUNIT_ASSERT_EQUAL(0, stats_serializer.get_field_count());
}

}  // end namespace kmldom

TEST_MAIN
