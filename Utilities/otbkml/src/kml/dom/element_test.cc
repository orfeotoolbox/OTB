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

// This file contains the unit tests for the Element and Field classes.

#include "kml/dom/element.h"
#include "boost/intrusive_ptr.hpp"
#include "gtest/gtest.h"
#include "kml/base/attributes.h"
#include "kml/base/xml_namespaces.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_funcs.h"
#include "kml/dom/stats_serializer.h"

using kmlbase::Attributes;

namespace kmldom {

// Any complex child is derived from Element.  This one takes an int in
// the constructor to uniquely identify instances within this test.
class ComplexChild : public Element {
 public:
  ComplexChild(int id) : id_(id) {}
  int id() {
    return id_;
  }
 private:
  int id_;
};

// A complex child in the DOM API has a typedef like this:
typedef boost::intrusive_ptr<ComplexChild> ComplexChildPtr;

const char kEgo[] = "ego";

// This is a sample element with both a single-valued complex child
// and an array of complex children.
class TestElement : public Element {
 public:
  TestElement() : has_ego_(false) {}
  // This method exemplifies usage of SetComplexChild().
  void set_child(const ComplexChildPtr& child) {
    SetComplexChild(child, &child_);  // This is the method under test.
  }
  // This method exemplifies how a child is cleared.
  void clear_child() {
    set_child(NULL);  // Setting to NULL is well defined for intrusive_ptr.
  }
  // This method exemplifies how a complex child is accessed.
  // Note the use of const reference.
  const ComplexChildPtr& get_child() {
    return child_;
  }
  // This method exemplifies how a complex array child is added.
  // Note the use of const reference.
  void add_child(const ComplexChildPtr& child) {
    AddComplexChild(child, &child_array_);
  }
  // This method exemplifies how a complex array child is accessed.
  const ComplexChildPtr& get_child_array_at(int i) const {
    return child_array_[i];
  }
  // This method exemplifies how attributes are parsed.
  virtual void ParseAttributes(Attributes* attributes) {
    if (attributes) {
      has_ego_ = attributes->CutValue(kEgo, &ego_);
      Element::AddUnknownAttributes(attributes);
    }
  }
  // This method exemplifies how attributes are serialized.
  virtual void SerializeAttributes(Attributes* attributes) const {
    Element::SerializeAttributes(attributes);
    if (has_ego_) {
      attributes->SetValue(kEgo, get_ego());
    }
  }

  bool has_ego() const {
    return has_ego_;
  }
  string get_ego() const {
    return ego_;
  }

 private:

  // A given single complex child is managed by a smart pointer whose
  // destructor releases this element's reference to the underlying element.
  ComplexChildPtr child_;
  // A given array valued complex child is held in an STL vector whose
  // destructor calls the destructor of each array element thus releasing
  // the reference to each underlying element.
  std::vector<ComplexChildPtr> child_array_;
  // This element keeps the value of any "ego" attribute here.
  string ego_;
  bool has_ego_;
};

typedef boost::intrusive_ptr<TestElement> TestElementPtr;

// This tests the Element class.
class ElementTest : public testing::Test {
 protected:
  virtual void SetUp() {
    element_ = new TestElement();
    child1_ = new ComplexChild(1);
    child2_ = new ComplexChild(2);
    child3_ = new ComplexChild(3);
  }

  // Smart pointer memory management is used within the test fixture as well.
  TestElementPtr element_;
  ComplexChildPtr child1_, child2_, child3_;
};

// The test Elements here do not set their type and are hence Type_Unknown.
TEST_F(ElementTest, TestTypeUnknown) {
  ASSERT_EQ(kmldom::Type_Unknown, element_->Type());
  ASSERT_TRUE(element_->IsA(kmldom::Type_Unknown));
  ASSERT_EQ(kmldom::Type_Unknown, child1_->Type());
  ASSERT_TRUE(child1_->IsA(kmldom::Type_Unknown));
}

TEST_F(ElementTest, TestAddGetUnknowns) {
  // Unrecognised elements:
  const string unknown1("<unknown>zzz<Foo/></unknown>");
  const string unknown2("<unknownBar/>");
  element_->AddUnknownElement(unknown1);
  element_->AddUnknownElement(unknown2);
  ASSERT_EQ(static_cast<size_t>(2),
                       element_->get_unknown_elements_array_size());
  ASSERT_EQ(unknown1, element_->get_unknown_elements_array_at(0));
  ASSERT_EQ(unknown2, element_->get_unknown_elements_array_at(1));

  // Recognized but misplaced elements:
  Element* legal_name = KmlFactory::GetFactory()->CreateFieldById(Type_name);
  Element* legal_open = KmlFactory::GetFactory()->CreateFieldById(Type_open);
  element_->AddElement(legal_name);
  element_->AddElement(legal_open);
  ASSERT_EQ(static_cast<size_t>(2),
                       element_->get_misplaced_elements_array_size());
  ASSERT_EQ(
      Type_name, element_->get_misplaced_elements_array_at(0)->Type());
  ASSERT_EQ(
      Type_open, element_->get_misplaced_elements_array_at(1)->Type());
}

// This tests the SetComplexChild() method.
TEST_F(ElementTest, TestSetComplexChild) {
  // set_child() calls SetComplexChild.
  element_->set_child(child1_);
  // Verify the child is child 1.
  ASSERT_EQ(1, element_->get_child()->id());
  ASSERT_EQ(2, child1_->get_ref_count());

  // Set again releases reference of previous.
  element_->set_child(child2_);
  ASSERT_EQ(2, element_->get_child()->id());
  ASSERT_EQ(1, child1_->get_ref_count());
  ASSERT_EQ(2, child2_->get_ref_count());

  // Set to NULL also release reference of previously set child.
  element_->clear_child();
  ASSERT_EQ(ComplexChildPtr(NULL), element_->get_child());
  ASSERT_EQ(1, child2_->get_ref_count());
}

// This tests the AddComplexChild() method.
TEST_F(ElementTest, TestAddComplexChild) {
  element_->add_child(child1_);
  element_->add_child(child2_);
  element_->add_child(child3_);
  element_->add_child(NULL);  // NOP, but should not crash.
  ASSERT_EQ(1, element_->get_child_array_at(0)->id());
  ASSERT_EQ(2, element_->get_child_array_at(0)->get_ref_count());
  ASSERT_EQ(2, element_->get_child_array_at(1)->id());
  ASSERT_EQ(2, element_->get_child_array_at(1)->get_ref_count());
  ASSERT_EQ(3, element_->get_child_array_at(2)->id());
  ASSERT_EQ(2, element_->get_child_array_at(2)->get_ref_count());
}

// This tests the ParseAttributes() method.
TEST_F(ElementTest, TestParseAttributes) {
  // Initial conditions: TestElement has no ego= attribute and base element
  // has no id= or ego= in its "unparsed" attributes array.
  ASSERT_FALSE(element_->has_ego());
  Attributes attributes;
  element_->SerializeAttributes(&attributes);
  string val;
  ASSERT_FALSE(attributes.GetValue("ego", &val));
  ASSERT_FALSE(attributes.GetValue("id", &val));

  // Create and parse attributes.
  const char* kAttrs[] = { "ego", "major", "id", "none", NULL };
  element_->ParseAttributes(Attributes::Create(kAttrs));
  // Verify that TestElement grabbed the ego= attr
  ASSERT_TRUE(element_->has_ego());
  ASSERT_EQ(string(kAttrs[1]), element_->get_ego());
  // Verify serialization picked up both attributes.
  element_->SerializeAttributes(&attributes);
  ASSERT_TRUE(attributes.GetValue("ego", &val));
  ASSERT_TRUE(attributes.GetValue("id", &val));
  // Verify that the base Element grabbed the id= attr _and_ did _not_ also
  // wind up with the ego attr.  (Note: earlier versions of libkml did pass
  // _all_ attribute name-value pairs up to Element::ParseAttributes even if
  // a derived class accepted one or more or all attributes).
  const Attributes* unknown = element_->GetUnknownAttributes();
  ASSERT_TRUE(unknown);
  ASSERT_EQ(static_cast<size_t>(1), unknown->GetSize());
  ASSERT_FALSE(unknown->GetValue("ego", &val));
  ASSERT_TRUE(unknown->GetValue("id", &val));
}

// This tests GetXmlns() and xmlns handling of SerializeAttributes().
TEST_F(ElementTest, TestXmlns) {
  std::map<string, string> source_map;
  const string kXmlns("xmlns");
  const string kGx("gx");
  const string kXx("xx");
  source_map[kXmlns] = "default-namespace";
  source_map[kGx] = "extension-namespace";
  source_map[kXx] = "yet-another-namespace";
  // Create and parse xmlns attributes.
  Attributes* attributes = new Attributes();  // Element takes ownership.
  attributes->SetValue("xmlns", source_map[kXmlns]);
  attributes->SetValue(string("xmlns:") + kGx, source_map[kGx]);
  attributes->SetValue(string("xmlns:") + kXx, source_map[kXx]);
  element_->ParseAttributes(attributes);

  // Verify the xmlns Attributes returned from GetXmlns().
  const Attributes* xmlns = element_->GetXmlns();
  ASSERT_TRUE(xmlns);
  // STL set does not permit dupes.
  std::set<string> found_keys;
  kmlbase::StringMapIterator iter = xmlns->CreateIterator();
  for (; !iter.AtEnd(); iter.Advance()) {
    found_keys.insert(iter.Data().first);
    ASSERT_EQ(source_map[iter.Data().first], iter.Data().second);
  }
  ASSERT_EQ(source_map.size(), found_keys.size());
  // Verify that no xmlns attributes fell to the fully unknown attributes list.
  ASSERT_FALSE(element_->GetUnknownAttributes());

  // Verify the xmlns attributes output from SerializeAttributes().
  Attributes serialized_attrs;
  element_->SerializeAttributes(&serialized_attrs);
  ASSERT_EQ(source_map.size(), serialized_attrs.GetSize());
}

TEST_F(ElementTest, TestGetParent) {
  ASSERT_FALSE(child1_->GetParent());
  element_->set_child(child1_);
  ASSERT_EQ(element_, child1_->GetParent());
  ASSERT_FALSE(child2_->GetParent());
#if 0 // TODO
  element_->set_child(child2_);
  ASSERT_EQ(element_, child2_->GetParent());
  ASSERT_FALSE(child1_->GetParent());
#endif
}

// This tests the MergeXmlns method on an Element with as yet no xmlns info
// and adds no prefix/namespace pairs.
TEST_F(ElementTest, TestMergeXmlnsNull) {
  Attributes xmlns;
  // An Element has no xmlns attributes to start with:
  ASSERT_FALSE(element_->GetXmlns());
  // Merging in no xmlns attributes should not crash
  element_->MergeXmlns(xmlns);
  // Simple calling MergeXmlns _does_ create an xmlns
  ASSERT_TRUE(element_->GetXmlns());

  // ...even though it's empty
  ASSERT_EQ(static_cast<size_t>(0), element_->GetXmlns()->GetSize());
}

// This tests the MergeXmls method on an Element with as yet no xmlns info
// and adds exactly one prefix/namespace pair.
TEST_F(ElementTest, TestMergeXmlnsOne) {
  const string kPrefix("kmx");
  const string kNamespace("http://example.com/km/x");
  Attributes xmlns;
  xmlns.SetValue(kPrefix, kNamespace);
  element_->MergeXmlns(xmlns);
  ASSERT_TRUE(element_->GetXmlns());
  ASSERT_EQ(static_cast<size_t>(1), element_->GetXmlns()->GetSize());
  string xml_namespace;
  ASSERT_TRUE(element_->GetXmlns()->GetValue(kPrefix, &xml_namespace));
  ASSERT_EQ(kNamespace, xml_namespace);
}

TEST_F(ElementTest, TestMergeXmlnsMultiple) {
  // Create an Attributes with several xmlns prefix/namespaces.
  Attributes xmlns;
  const kmlbase::XmlnsId kXmlnsIds[] = {
      kmlbase::XMLNS_ATOM, kmlbase::XMLNS_KML22, kmlbase::XMLNS_GX22,
      kmlbase::XMLNS_XAL };
  const size_t xmlnsid_size = sizeof(kXmlnsIds)/sizeof(kmlbase::XmlnsId);
  for (size_t i = 0; i < xmlnsid_size; ++i) {
    string prefix;
    string xml_namespace;
    ASSERT_TRUE(FindXmlNamespaceAndPrefix(kXmlnsIds[i], &prefix,
                                          &xml_namespace)) << kXmlnsIds[i];
    xmlns.SetValue(prefix, xml_namespace);
    ASSERT_EQ(static_cast<size_t>(i+1), xmlns.GetSize());
    // Merge in the whole set each time, and...
    element_->MergeXmlns(xmlns);
    // ...verify that it only grows by one item each time, and...
    ASSERT_EQ(static_cast<size_t>(i+1), element_->GetXmlns()->GetSize());
    // ...verify that that item is in the element's xmlns.
    string got_namespace;
    ASSERT_TRUE(element_->GetXmlns()->GetValue(prefix, &got_namespace));
    ASSERT_EQ(xml_namespace, got_namespace);
  }
}

TEST_F(ElementTest, TestSerializeUnknown) {
  // This Serializer is special-cased to assert the behavior of Element's
  // SerializeUnknown() method on fully unknown children.
  typedef std::vector<string> StringVector;
  class UnknownSerializer : public Serializer {
   public:
    UnknownSerializer()
      : begin_element_array_count_(0),
        end_element_array_count_(0),
        element_count_(0),
        in_unknown_element_array_(false) {
    }

    virtual void BeginElementArray(int type_id, size_t element_count) {
      ASSERT_FALSE(in_unknown_element_array_);
      ASSERT_EQ(Type_Unknown, type_id);
      ASSERT_EQ(static_cast<size_t>(0), element_count_);

      ++begin_element_array_count_;
      element_count_ = element_count;
      in_unknown_element_array_ = true;
    }

    virtual void EndElementArray(int type_id) {
      ASSERT_TRUE(in_unknown_element_array_);
      ASSERT_EQ(Type_Unknown, type_id);
      ASSERT_EQ(static_cast<size_t>(0), element_count_);

      ++end_element_array_count_;
    }

    virtual void SaveContent(const string& content, bool escape) {
      ASSERT_TRUE(in_unknown_element_array_);
      --element_count_;
      unknown_content_.push_back(content);
    }

    int get_begin_element_array_count() const {
      return begin_element_array_count_;
    }
    int get_end_element_array_count() const {
      return end_element_array_count_;
    }
    const StringVector& get_unknown_content() const {
      return unknown_content_;
    }

   private:
    int begin_element_array_count_;
    int end_element_array_count_;
    size_t element_count_;
    bool in_unknown_element_array_;
    std::vector<string> unknown_content_;
  } unknown_serializer;

  element_->SerializeUnknown(unknown_serializer);
  ASSERT_EQ(0, unknown_serializer.get_begin_element_array_count());
  ASSERT_TRUE(unknown_serializer.get_unknown_content().empty());
  ASSERT_EQ(0, unknown_serializer.get_end_element_array_count());
  ASSERT_TRUE(unknown_serializer.get_unknown_content().empty());

  const string kUnknown1("<hi>there</hi>");
  element_->AddUnknownElement(kUnknown1);
  const string kUnknown2("<how>are</how>");
  element_->AddUnknownElement(kUnknown2);
  element_->SerializeUnknown(unknown_serializer);
  ASSERT_EQ(1, unknown_serializer.get_begin_element_array_count());
  ASSERT_EQ(static_cast<size_t>(2),
            unknown_serializer.get_unknown_content().size());
  ASSERT_EQ(1, unknown_serializer.get_end_element_array_count());
  ASSERT_EQ(kUnknown1, unknown_serializer.get_unknown_content()[0]);
  ASSERT_EQ(kUnknown2, unknown_serializer.get_unknown_content()[1]);
}

// This is a complex element whose only role is to call the most basic
// Serialize implementation possible: that provided by ElementSerializer.
class ComplexChildWithSerializer : public Element {
 public:
  ComplexChildWithSerializer(int id)
     : Element(static_cast<KmlDomType>(id)) {
  }
  virtual void Serialize(Serializer& serializer) const {
    // Calls BeginById(), End()
    ElementSerializer element_serializer(*this, serializer);
  }
};

TEST_F(ElementTest, TestSerializeMisplaced) {
  // This Serializer is special-cased to assert the behavior of Element's
  // SerializeUnknown() method on misplaced children.
  typedef std::vector<int> IntVector;
  class MisplacedSerializer : public Serializer {
   public:
    virtual void BeginById(int type_id,
                           const kmlbase::Attributes& attributes) {
      id_vector_.push_back(type_id);
    };

    const IntVector& get_id_vector() const {
      return id_vector_;
    }

   private:
    IntVector id_vector_;
  } misplaced_serializer;

  // Nothing in, nothing out.
  element_->SerializeUnknown(misplaced_serializer);
  ASSERT_TRUE(misplaced_serializer.get_id_vector().empty());

  // 3 things in, 3 things out.
  // AddElement on Element adds the Element to the misplaced elements array.
  element_->AddElement(new ComplexChildWithSerializer(3));
  element_->AddElement(new ComplexChildWithSerializer(2));
  element_->AddElement(new ComplexChildWithSerializer(1));
  // Call the method under test.
  element_->SerializeUnknown(misplaced_serializer);
  // Verify all is as expected.
  ASSERT_EQ(static_cast<size_t>(3),
            misplaced_serializer.get_id_vector().size());
  ASSERT_EQ(3, misplaced_serializer.get_id_vector()[0]);
  ASSERT_EQ(2, misplaced_serializer.get_id_vector()[1]);
  ASSERT_EQ(1, misplaced_serializer.get_id_vector()[2]);
}

class ElementSerializerTest : public testing::Test {
 protected:
  virtual void SetUp() {
    test_element_ = new TestElement();
    child1_ = new ComplexChild(1);
    child2_ = new ComplexChild(2);
    child3_ = new ComplexChild(3);
  }

  TestElementPtr test_element_;
  StatsSerializer stats_serializer_;
  ComplexChildPtr child1_, child2_, child3_;
};

TEST_F(ElementSerializerTest, TestSimpleUsage) {
  // This the intended usage of ElementSerializer typically within the
  // Serialize method of an Element-derived class.
  {
    ElementSerializer element_serializer(*test_element_, stats_serializer_);
  }
  // This is equivalent to serializing an empty element.
  ASSERT_EQ(1, stats_serializer_.get_begin_count());
  ASSERT_EQ(1, stats_serializer_.get_end_count());
  ASSERT_EQ(0, stats_serializer_.get_field_count());
  ASSERT_EQ(0, stats_serializer_.get_element_count());
  ASSERT_EQ(0, stats_serializer_.get_element_group_count());
}

TEST_F(ElementSerializerTest, TestChildren) {
  {
    ElementSerializer element_serializer(*test_element_, stats_serializer_);
    stats_serializer_.SaveElement(child1_);
    stats_serializer_.SaveFieldById(42, 42);
    stats_serializer_.SaveElement(child2_);
    stats_serializer_.SaveFieldById(137, 137);
    stats_serializer_.SaveElement(child3_);
  }
  ASSERT_EQ(1, stats_serializer_.get_begin_count());
  ASSERT_EQ(1, stats_serializer_.get_end_count());
  ASSERT_EQ(2, stats_serializer_.get_field_count());
  ASSERT_EQ(3, stats_serializer_.get_element_count());
  ASSERT_EQ(0, stats_serializer_.get_element_group_count());
}

// This tests Field's SetBool() method.
TEST(FieldTest, TestSetBool) {
  KmlFactory* factory = KmlFactory::GetFactory();
  FieldPtr field = factory->CreateFieldById(Type_open);
  // Pathological, but well defined case.  Note: SetBool always deletes field.
  ASSERT_EQ(false, field->SetBool(NULL));

  // Handle the 5 variants of bool: "1", "true", "0", "false", garbage
  bool open;

  // <open>1</open>
  // Parser finds "<open>" and maps to Type_open which is simple and thus
  // it creates a Field.
  field = factory->CreateFieldById(Type_open);
  // Parser gathers character data.
  field->set_char_data("1");
  // Parse passes Field to parent element which accepts and converts
  // and deletes the Field.
  open = false;  // Verify that SetBool changes to true.
  ASSERT_TRUE(field->SetBool(&open));  // SetBool deletes field.
  ASSERT_TRUE(open);

  // <open>0</open>
  field = factory->CreateFieldById(Type_open);
  field->set_char_data("0");
  open = true;
  ASSERT_TRUE(field->SetBool(&open));  // SetBool deletes field.
  ASSERT_FALSE(open);

  // <open>true</open>
  field = factory->CreateFieldById(Type_open);
  field->set_char_data("true");
  open = false;
  ASSERT_TRUE(field->SetBool(&open));  // SetBool deletes field.
  ASSERT_TRUE(open);

  // <open>false</open>
  field = factory->CreateFieldById(Type_open);
  field->set_char_data("false");
  open = true;
  ASSERT_TRUE(field->SetBool(&open));  // SetBool deletes field.
  ASSERT_FALSE(open);

  // <open>garbage</open>
  field = factory->CreateFieldById(Type_open);
  field->set_char_data("garbage");
  open = true;
  ASSERT_TRUE(field->SetBool(&open));  // SetBool deletes field.
  ASSERT_FALSE(open);
}

// This tests Field's SetDouble() method.
TEST(FieldTest, TestSetDouble) {
  KmlFactory* factory = KmlFactory::GetFactory();
  FieldPtr field = factory->CreateFieldById(Type_north);
  // Pathological, but well defined case.  Note: SetDouble always deletes field.
  ASSERT_EQ(false, field->SetDouble(NULL));

  // <north>37.123</north>
  field = factory->CreateFieldById(Type_north);
  field->set_char_data("37.123");
  double north = 1.1;
  ASSERT_TRUE(field->SetDouble(&north));  // SetDouble() deletes field.
  ASSERT_EQ(37.123, north);
}

// This tests Field's SetInt() method.
TEST(FieldTest, TestSetInt) {
  KmlFactory* factory = KmlFactory::GetFactory();
  FieldPtr field = factory->CreateFieldById(Type_drawOrder);
  // Pathological, but well defined case.  Note: SetInt always deletes field.
  ASSERT_EQ(false, field->SetInt(NULL));

  // <drawOrder>10</drawOrder>
  field = factory->CreateFieldById(Type_drawOrder);
  field->set_char_data("10");
  int draworder = 11;  // not 10
  ASSERT_TRUE(field->SetInt(&draworder));  // SetInt() deletes field.
  ASSERT_EQ(10, draworder);
}

// This tests Field's SetEnum() method.
TEST(FieldTest, TestSetEnum) {
  KmlFactory* factory = KmlFactory::GetFactory();
  FieldPtr field = factory->CreateFieldById(Type_altitudeMode);
  // Pathological, but well defined case: null pointer to enum val.
  // Note: SetEnum always deletes field.
  ASSERT_EQ(false, field->SetEnum(NULL));

  int altitudemode;

  // A normal case:
  // <altitudeMode>relativeToGround</altitudeMode>
  field = factory->CreateFieldById(Type_altitudeMode);
  field->set_char_data("relativeToGround");
  altitudemode = kmldom::ALTITUDEMODE_ABSOLUTE;  // Not relativeToGround.
  ASSERT_TRUE(field->SetEnum(&altitudemode));
  // Note: SetEnum() deletes field.
  ASSERT_EQ(static_cast<int>(ALTITUDEMODE_RELATIVETOGROUND),
                       altitudemode);

  // Unknown enum val behaves as is that enum did not exist:
  // <altitudeMode>reach-for-the-stars</altitudeMode>
  field = factory->CreateFieldById(Type_altitudeMode);
  field->set_char_data("reach-for-the-stars");
  altitudemode = kmldom::ALTITUDEMODE_ABSOLUTE;
  ASSERT_EQ(false, field->SetEnum(&altitudemode));
  // Note: SetEnum() deletes field.
  // altitudemode remains unchanged:
  ASSERT_EQ(static_cast<int>(ALTITUDEMODE_ABSOLUTE),
                       altitudemode);

  // Pathologiccal case: Field is not enum.  The addressed val is not
  // touched and false is returned.
  field = factory->CreateFieldById(Type_visibility);
  int not_touched = 42;
  ASSERT_EQ(false, field->SetEnum(&not_touched));
  ASSERT_EQ(42, not_touched);
}

// This tests Field's SetString() method.
TEST(FieldTest, TestSetString) {
  KmlFactory* factory = KmlFactory::GetFactory();
  FieldPtr field = factory->CreateFieldById(Type_description);
  // Pathological, but well defined case: null pointer to string val.
  // Note: SetString always deletes field.
  ASSERT_EQ(false, field->SetString(NULL));

  string name;

  // <name>my name</name>
  field = factory->CreateFieldById(Type_name);
  const char* kMyName = "my name";
  field->set_char_data(kMyName);
  ASSERT_EQ(true, field->SetString(&name));
  ASSERT_EQ(string(kMyName), name);
}

// This tests Field's Serialize() method.
TEST(FieldTest, TestSerialize) {
  const string kContent("stuff in little snippet");
  KmlFactory* factory = KmlFactory::GetFactory();
  FieldPtr field = factory->CreateFieldById(Type_snippet);
  // Test empty field is serialized as nil element.
  ASSERT_EQ(string("<snippet/>"), SerializeRaw(field));

  // Give it content and verify serializing of content-full field.
  field->set_char_data(kContent);
  const string kExpectedXml(
      string("<snippet>") + kContent + "</snippet>");
  ASSERT_EQ(kExpectedXml, SerializeRaw(field));
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
