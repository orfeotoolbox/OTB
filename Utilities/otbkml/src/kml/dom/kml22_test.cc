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

#include "kml/dom/kml22.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/xsd.h"
#include "kml/dom/serializer.h"
#include "gtest/gtest.h"

namespace kmldom {

// Yes, must exactly match kml22.h
static const int kNumberOfComplexElements = 88;

class Kml22Test : public testing::Test {
 protected:
  virtual void SetUp() {
    xsd_ = Xsd::GetSchema();
  }

  Xsd* xsd_;

  void AssertXmlNamespaceForRange(KmlDomType begin_id, KmlDomType end_id,
                                  kmlbase::XmlnsId xmlns_id);
};

// Verify proper enum defaults:
TEST_F(Kml22Test, TestEnumDefaults) {
  ASSERT_EQ(string("clampToGround"),
            xsd_->EnumValue(Type_altitudeMode, ALTITUDEMODE_CLAMPTOGROUND));
  ASSERT_EQ(string("normal"),
            xsd_->EnumValue(Type_colorMode, COLORMODE_NORMAL));
  ASSERT_EQ(string("default"),
            xsd_->EnumValue(Type_displayMode, DISPLAYMODE_DEFAULT));
  ASSERT_EQ(string("lowerLeft"),
            xsd_->EnumValue(Type_gridOrigin, GRIDORIGIN_LOWERLEFT));
  ASSERT_EQ(string("open"),
            xsd_->EnumValue(Type_state, ITEMICONSTATE_OPEN));
  ASSERT_EQ(string("check"),
            xsd_->EnumValue(Type_listItemType, LISTITEMTYPE_CHECK));
  ASSERT_EQ(string("onChange"),
            xsd_->EnumValue(Type_refreshMode, REFRESHMODE_ONCHANGE));
  ASSERT_EQ(string("rectangle"),
            xsd_->EnumValue(Type_shape, SHAPE_RECTANGLE));
  ASSERT_EQ(string("normal"),
            xsd_->EnumValue(Type_key, STYLESTATE_NORMAL));
  ASSERT_EQ(string("fraction"),
            xsd_->EnumValue(Type_units, UNITS_FRACTION));
  ASSERT_EQ(string("never"),
            xsd_->EnumValue(Type_viewRefreshMode, VIEWREFRESHMODE_NEVER));
  ASSERT_EQ(string("bounce"),
            xsd_->EnumValue(Type_GxFlyToMode, GX_FLYTOMODE_BOUNCE));
  ASSERT_EQ(string("pause"),
            xsd_->EnumValue(Type_GxPlayMode, GX_PLAYMODE_PAUSE));
}

// Test NULL arg on AddElement(), ParseAttributes(), and SerializeAttributes()
// on all complex elements.  These are all methods that take pointer arguments.
TEST_F(Kml22Test, TestElementNull) {
  int complex_count = 0;
  int element_type_id = static_cast<int>(Type_Unknown) + 1;
  const int end_id = static_cast<int>(Type_Invalid);
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  for (; element_type_id != end_id; ++element_type_id) {
    // Only complex elements return non-NULL.
    if (ElementPtr element = kml_factory->CreateElementById(
            static_cast<KmlDomType>(element_type_id))) {
      // All KmlFactory-created elements are known.
      ASSERT_FALSE(element->IsA(kmldom::Type_Unknown));
      // Verify the element's abstract base types.
      if (element->IsA(kmldom::Type_ColorStyle)) {
        ASSERT_TRUE(element->IsA(kmldom::Type_SubStyle));
      }
      if (element->IsA(kmldom::Type_Container)) {
        ASSERT_TRUE(element->IsA(kmldom::Type_Feature));
      }
      if (element->IsA(kmldom::Type_Overlay)) {
        ASSERT_TRUE(element->IsA(kmldom::Type_Feature));
      }
      if (element->IsA(kmldom::Type_AbstractLatLonBox) ||
          element->IsA(kmldom::Type_AbstractView) ||
          element->IsA(kmldom::Type_BasicLink) ||
          element->IsA(kmldom::Type_Feature) ||
          element->IsA(kmldom::Type_Geometry) ||
          element->IsA(kmldom::Type_StyleSelector) ||
          element->IsA(kmldom::Type_SubStyle) ||
          element->IsA(kmldom::Type_TimePrimitive)) {
        ASSERT_TRUE(element->IsA(kmldom::Type_Object));
      }
      // Verify NULL pointers do no harm.
      element->AddElement(NULL);
      element->ParseAttributes(NULL);
      element->SerializeAttributes(NULL);
      ++complex_count;
    }
  }
  ASSERT_EQ(kNumberOfComplexElements, complex_count);
}

// This Serializer is specifically designed to capture the output of the
// serialization of any complex element.
class MockElementSerializer : public Serializer {
 public:
  MockElementSerializer()
    : begin_count_(0),
      end_count_(0),
      type_id_(Type_Invalid) {
  }
  virtual void BeginById(int type_id, const kmlbase::Attributes& attributes) {
    ++begin_count_;
    type_id_ = type_id;
  }
  virtual void End() {
    ++end_count_;
  }
  int get_type_id() const {
    return type_id_;
  }
  int get_begin_count() const {
    return begin_count_;
  }
  int get_end_count() const {
    return end_count_;
  }
 private:
  int begin_count_;
  int end_count_;
  int type_id_;
};

// This calls the Serialize method on every complex element on the
// MockElementSerializer to verify that Serialize always emits BeginById() with
// the expected element type id and always emits End() and both exactly once.
TEST_F(Kml22Test, TestElementSerializerEmpty) {
  int complex_count = 0;
  int element_type_id = static_cast<int>(Type_Unknown) + 1;
  const int end_id = static_cast<int>(Type_Invalid);
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  for (; element_type_id != end_id; ++element_type_id) {
    // Only complex elements return non-NULL.
    if (ElementPtr element = kml_factory->CreateElementById(
            static_cast<KmlDomType>(element_type_id))) {
      // Every complex element has a Serialize method that further calls
      // BeginById(type), and End().
      MockElementSerializer mock;
      element->Serialize(mock);
      ASSERT_EQ(element->Type(), mock.get_type_id());
      ASSERT_EQ(1, mock.get_begin_count());
      ASSERT_EQ(1, mock.get_end_count());
      ++complex_count;
    }
  }
  ASSERT_EQ(kNumberOfComplexElements, complex_count);
}

void Kml22Test::AssertXmlNamespaceForRange(KmlDomType begin_dom_type,
                                           KmlDomType end_dom_type,
                                           kmlbase::XmlnsId xmlns_id) {
  // There's obviously an assumption here about the organization of the
  // KmlDomType enum
  int element_type_id = static_cast<int>(begin_dom_type);
  const int end_id = static_cast<int>(end_dom_type) + 1;
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  for (; element_type_id != end_id; ++element_type_id) {
    ElementPtr element = kml_factory->CreateElementById(
        static_cast<KmlDomType>(element_type_id));
    ASSERT_TRUE(element);
    ASSERT_EQ(xmlns_id, element->get_xmlns())
        << xsd_->ElementName(element_type_id);
  }
}

TEST_F(Kml22Test, TestElementXmlNamespaces) {
  AssertXmlNamespaceForRange(Type_Alias, Type_ViewVolume,
                             kmlbase::XMLNS_KML22);
  AssertXmlNamespaceForRange(Type_AtomAuthor, Type_AtomLink,
                             kmlbase::XMLNS_ATOM);
  AssertXmlNamespaceForRange(Type_XalAddressDetails, Type_XalThoroughfare,
                             kmlbase::XMLNS_XAL);
  AssertXmlNamespaceForRange(Type_GxAnimatedUpdate, Type_GxWait,
                             kmlbase::XMLNS_GX22);
}

// Every complex element preserves unknown children.
TEST_F(Kml22Test, TestSaveUnknown) {
  int complex_count = 0;
  int element_type_id = static_cast<int>(Type_Unknown) + 1;
  const int end_id = static_cast<int>(Type_Invalid);
  KmlFactory* kml_factory = KmlFactory::GetFactory();
  for (; element_type_id != end_id; ++element_type_id) {
    // Only complex elements return non-NULL.
    if (ElementPtr element = kml_factory->CreateElementById(
        static_cast<KmlDomType>(element_type_id))) {
      ++complex_count;
    }
  }
  ASSERT_EQ(kNumberOfComplexElements, complex_count);
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
