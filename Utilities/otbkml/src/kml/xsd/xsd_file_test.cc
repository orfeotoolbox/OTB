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

// This file contains the unit tests for the XsdFile class.

#include "kml/xsd/xsd_file.h"
#include "boost/scoped_ptr.hpp"
#include "kml/base/attributes.h"
#include "gtest/gtest.h"
#include "kml/xsd/xsd_util.h"

using kmlbase::Attributes;

namespace kmlxsd {

// This class is the unit test fixture for the XsdFile class.
class XsdFileTest : public testing::Test {
 protected:
  virtual void SetUp() {
    xsd_file_.reset(new XsdFile);
  }

  void SetTestSchema();
  void AddTestComplexTypes();
  void AddTestElements();
  void InitTestXsd();
  boost::scoped_ptr<XsdFile> xsd_file_;
};

TEST_F(XsdFileTest, TestConstructor) {
  ASSERT_TRUE(xsd_file_.get());
}

// Verify CreateFromParse().
TEST_F(XsdFileTest, TestCreateFromParse) {
  // Verify known bad XSD returns NULL.
  string errors;
  ASSERT_FALSE(XsdFile::CreateFromParse("not xsd", &errors));
  ASSERT_FALSE(errors.empty());

  // TODO: more
}

// Verify set_schema(),
TEST_F(XsdFileTest, TestSetSchema) {
  const string kPrefix("myml");
  const string kNamespace("my:own:namespace");
  const XsdSchemaPtr xsd_schema = CreateXsdSchema(kPrefix, kNamespace);

  // Call the method under test
  xsd_file_->set_schema(xsd_schema);

  // Setting the <xs:schema> sets the namespace and prefix for this file.
  ASSERT_EQ(kPrefix, xsd_file_->get_target_namespace_prefix());
  ASSERT_EQ(kNamespace, xsd_file_->get_target_namespace());
}

// Verify add_type(), FindElement(), and FindElementType().
TEST_F(XsdFileTest, TestFindElementTypeByName) {
  // Create the <xs:schema ... >
  const string kPrefix("myml");
  const string kNamespace("my:own:namespace");
  xsd_file_->set_schema(CreateXsdSchema(kPrefix, kNamespace));
  // Create an <xs:complexType name="MyCoolType"/>
  const string kMyType("MyCoolType");
  xsd_file_->add_type(CreateXsdComplexType(kMyType));
  const string kMyElement("MyCoolness");
  // <xs:element name="MyCoolness" type="myml:MyCoolType"/>
  xsd_file_->add_element(CreateXsdElement(kMyElement, kPrefix + ":" + kMyType));

  const XsdElementPtr element = xsd_file_->FindElement(kMyElement);
  ASSERT_TRUE(element);
  const XsdTypePtr complex_type = xsd_file_->FindElementType(element);
  ASSERT_TRUE(complex_type);
  ASSERT_EQ(kMyType, complex_type->get_name());
}

// Verify add_type() and GetChildElements().
TEST_F(XsdFileTest, TestAddComplexType) {
  // This is an instance document this XSD-let describes:
  // <MyCoolness>
  //   <lon>-120.123</lon>
  //   <lat>37.37</lon>
  // </MyCoolness>

  // This is the XSD for the type of MyCoolness:
  // <xs:complexType name="MyCoolType">
  //   <complexContent>
  //     <sequence>
  //       <element name="lon" type="double"/>
  //       <element name="lat" type="double"/>
  //     </sequence>
  //   </complexContent>
  // </xs:complexType">
  const std:: string kMyElement("MyCoolness");
  const std:: string kMyType("MyCoolType");
  const string kLon("lon");
  const string kLat("lat");
  // Create the <xs:schema ... >
  const string kPrefix("myml");
  const string kNamespace("my:own:namespace");
  xsd_file_->set_schema(CreateXsdSchema(kPrefix, kNamespace));
  XsdComplexTypePtr xsd_complex_type = CreateXsdComplexType(kMyType);
  xsd_complex_type->add_element(CreateXsdElement(kLon, "double"));
  xsd_complex_type->add_element(CreateXsdElement(kLat, "double"));
  xsd_file_->add_type(xsd_complex_type);

  // <xs:element name="MyCoolness" type="MyCoolType"/>
  xsd_file_->add_element(CreateXsdElement(kMyElement, kPrefix + ":" + kMyType));

  XsdElementVector child_elements;
  xsd_file_->GetChildElements(kMyElement, &child_elements);
  ASSERT_EQ(static_cast<size_t>(2), child_elements.size());
  ASSERT_EQ(kLon, child_elements[0]->get_name());
  ASSERT_EQ(kLat, child_elements[1]->get_name());
}

// Verify ResolveRef.
TEST_F(XsdFileTest, TestResolveRef) {
  const string kPrefix("myml");
  const string kNamespace("my:own:namespace");
  const string kMyElement("MyCoolness");

  // Verify failure if no XsdSchema set for this XsdFile.
  ASSERT_FALSE(xsd_file_->ResolveRef(kPrefix + ":" + kMyElement));

  // Build enough of an XsdFile to successfully use ResolveRef().
  // Create the <xs:schema ... >
  xsd_file_->set_schema(CreateXsdSchema(kPrefix, kNamespace));
  // Create a global <xs:element name="myCoolness" type="string"/>
  xsd_file_->add_element(CreateXsdElement(kMyElement, "string"));

  // Call the method under test for a good case.
  XsdElementPtr element = xsd_file_->ResolveRef(kPrefix + ":" + kMyElement);
  ASSERT_TRUE(element);
  ASSERT_EQ(kMyElement, element->get_name());

  // Call the method under test for some failure cases.
  ASSERT_FALSE(xsd_file_->ResolveRef("nosuchprefix:" + kMyElement));
  ASSERT_FALSE(xsd_file_->ResolveRef(kPrefix + ":nosuchelement"));
}

static const char* kTestPrefix = "myml";
static const char* kTestTargetNamespace = "my:own:namespace";

static const struct {
  const char* type_name;
  const char* extension_base;  // NULL if no base type.
} kTestComplexTypes[] = {
  { "FeatureType", "myml:ObjectType" },
  { "GeometryType", "myml:ObjectType" },
  { "LineStringType", "myml:GeometryType" },
  { "ObjectType", NULL },
  { "PlacemarkType", "myml:FeatureType" },
  { "PointType", "myml:GeometryType" }
};

static const struct {
  const char* name;
  const char* type;
  const char* abstract;
} kTestElements[] = {
  { "altitude", "double", "false" },
  { "altitudeMode", "myml:altitudeModeEnum", "false" },
  { "name", "string", "false" },
  { "visibility", "boolean", "false" },
  { "FeatureGroup", "myml:FeatureType", "true" },
  { "GeometryGroup", "myml:GeometryType", "true" },
  { "LineString", "myml:LineStringType", "false" },
  { "ObjectGroup", "myml:ObjectType", "true" },
  { "Placemark", "myml:PlacemarkType", "false" },
  { "Point", "myml:PointType", "false" }
};

void XsdFileTest::SetTestSchema() {
  // Build enough of an XsdFile to successfull use ResolveRef(), and
  // GetTypeHierarchy().
  // Create the <xs:schema ... >
  xsd_file_->set_schema(CreateXsdSchema(kTestPrefix, kTestTargetNamespace));
}

// This is an internal utility to add the kTestComplexTypes entries to the
// complex types map in the kml_file_.
void XsdFileTest::AddTestComplexTypes() {
  // Create some <xs:complexTypes> which extend each other.
  size_t size = sizeof(kTestComplexTypes)/sizeof(kTestComplexTypes[0]);
  for (size_t i = 0; i < size; ++i) {
    XsdComplexTypePtr complex_type =
        CreateXsdComplexType(kTestComplexTypes[i].type_name);
    if (kTestComplexTypes[i].extension_base) {
      complex_type->set_extension_base(kTestComplexTypes[i].extension_base);
    }
    xsd_file_->add_type(complex_type);
  }
}

void XsdFileTest::AddTestElements() {
  size_t size = sizeof(kTestElements)/sizeof(kTestElements[0]);
  for (size_t i = 0; i < size; ++i) {
    Attributes attributes;
    attributes.SetString(kName, kTestElements[i].name);
    attributes.SetString(kType, kTestElements[i].type);
    attributes.SetString(kAbstract, kTestElements[i].abstract);
    XsdElementPtr element = XsdElement::Create(attributes);
    ASSERT_TRUE(element);
    xsd_file_->add_element(element);
  }
}

void XsdFileTest::InitTestXsd() {
  SetTestSchema();
  AddTestComplexTypes();
  AddTestElements();
}

// Verify GetTypeHierarchy().
TEST_F(XsdFileTest, TestGetTypeHierarchy) {
  InitTestXsd();

  std::vector<XsdComplexTypePtr> hier;
  const XsdElementPtr element = xsd_file_->FindElement("Placemark");
  XsdTypePtr derived = xsd_file_->FindElementType(element);
  ASSERT_TRUE(derived);
  XsdComplexTypePtr complex_type = XsdComplexType::AsComplexType(derived);
  ASSERT_TRUE(complex_type);
  ASSERT_TRUE(xsd_file_->GetTypeHierarchy(complex_type, &hier));
  ASSERT_EQ(static_cast<size_t>(2), hier.size());
  ASSERT_EQ(string("FeatureType"), hier[0]->get_name());
  ASSERT_EQ(string("ObjectType"), hier[1]->get_name());
}

// XXX Verify FindElementType().  ByName() we do above...
TEST_F(XsdFileTest, TestFindElementType) {
#if 0
  SetTestSchema();
  AddTestComplexTypes();
  AddTestElements();
  ASSERT_EQ(string("PlacemarkType"),
                       xsd_file_->FindElementTypeByName("Placemark"));
  ASSERT_EQ(string("PointType"),
                       xsd_file_->FindElementTypeByName("Point"));
#endif
}

// Verify GetAbstractElements().
TEST_F(XsdFileTest, TestGetAbstractElements) {
  XsdElementVector elements;
  xsd_file_->GetAbstractElements(&elements);
  // Empty XsdFile, no elements, no abstract elements, no nuthin.
  ASSERT_TRUE(elements.empty());

  // Add the test elements to the XsdFile.
  AddTestElements();
  xsd_file_->GetAbstractElements(&elements);
  ASSERT_EQ(static_cast<size_t>(3), elements.size());
  ASSERT_EQ(string("FeatureGroup"), elements[0]->get_name());
  ASSERT_EQ(string("GeometryGroup"), elements[1]->get_name());
  ASSERT_EQ(string("ObjectGroup"), elements[2]->get_name());

}

// Verify GetComplexElements().
TEST_F(XsdFileTest, TestGetComplexElements) {
  XsdElementVector element_names;
  xsd_file_->GetComplexElements(&element_names);
  // Empty XsdFile, no abstract elements.
  ASSERT_TRUE(element_names.empty());

  // Add the test namespace, types, and elements to the XsdFile.
  InitTestXsd();
  xsd_file_->GetComplexElements(&element_names);
  ASSERT_EQ(static_cast<size_t>(3), element_names.size());
  ASSERT_EQ(string("LineString"), element_names[0]->get_name());
  ASSERT_EQ(string("Placemark"), element_names[1]->get_name());
  ASSERT_EQ(string("Point"), element_names[2]->get_name());
}

// Verify GetSimpleElements().
TEST_F(XsdFileTest, TestGetSimpleElements) {
  XsdElementVector element_names;
  xsd_file_->GetSimpleElements(&element_names);
  // Empty XsdFile, no simple elements.
  ASSERT_TRUE(element_names.empty());

  // Add the test namespace, types, and elements to the XsdFile.
  InitTestXsd();
  xsd_file_->GetSimpleElements(&element_names);
  ASSERT_EQ(static_cast<size_t>(3), element_names.size());
}

// Verify TestSetGetAlias();
TEST_F(XsdFileTest, TestSetGetAlias) {
  const string kFeature("Feature");
  const string kAbstractFeatureGroup("AbstractFeatureGroup");
  const string kGeometry("Geometry");
  const string kAbstractGeometryGroup("AbstractGeometryGroup");

  // An empty XsdFile has no aliases:
  ASSERT_EQ(string(""),
                       xsd_file_->get_alias(kAbstractFeatureGroup));

  // Add aliases using set_alias().
  xsd_file_->set_alias(kAbstractFeatureGroup, kFeature);
  xsd_file_->set_alias(kAbstractGeometryGroup, kGeometry);

  // Verify the proper result for get_alias().
  ASSERT_EQ(kFeature, xsd_file_->get_alias(kAbstractFeatureGroup));
  ASSERT_EQ(kGeometry, xsd_file_->get_alias(kAbstractGeometryGroup));
}

// Verify GetAllTypes().
TEST_F(XsdFileTest, TestGetAllTypes) {
  // Add the test namespace and types the XsdFile.
  SetTestSchema();
  AddTestComplexTypes();

  XsdTypeVector types;
  xsd_file_->GetAllTypes(&types);
  ASSERT_EQ(static_cast<size_t>(6), types.size());
}

// Verify TestFindChildElements().
TEST_F(XsdFileTest, TestFindChildElements) {
  // Verify NULL conditions: NULL complex_type and nothing in XsdFile.
  XsdComplexTypePtr complex_type;
  XsdElementVector children;
  xsd_file_->FindChildElements(complex_type, &children);
  ASSERT_TRUE(children.empty());

  // Add the test namespace and types the XsdFile.
  SetTestSchema();
  AddTestComplexTypes();
  xsd_file_->FindChildElements(complex_type, &children);
  ASSERT_TRUE(children.empty());

  // Look for the given type.
  const string kPlacemarkType("PlacemarkType");
  complex_type = XsdComplexType::AsComplexType(
      xsd_file_->FindType(kPlacemarkType));
  ASSERT_TRUE(complex_type);
  ASSERT_EQ(kPlacemarkType, complex_type->get_name());

  // This has no children.
  xsd_file_->FindChildElements(complex_type, &children);
  ASSERT_TRUE(children.empty());

  // Give it some children.
  const string kLatitude("latitude");
  const string kLongitude("longitude");
  const string kDouble("double");
  complex_type->add_element(CreateXsdElement(kLatitude, kDouble));
  complex_type->add_element(CreateXsdElement(kLongitude, kDouble));
  
  // This now has some children.
  xsd_file_->FindChildElements(complex_type, &children);
  ASSERT_EQ(static_cast<size_t>(2), children.size());
  ASSERT_EQ(kLatitude, children[0]->get_name());
  ASSERT_EQ(kLongitude, children[1]->get_name());
}

TEST_F(XsdFileTest, TestSearchTypeHierarchy) {
  InitTestXsd();
  const XsdComplexTypePtr& point_type =
      XsdComplexType::AsComplexType(xsd_file_->FindType("PointType"));
  ASSERT_TRUE(point_type);
  const XsdComplexTypePtr& geometry_type =
      XsdComplexType::AsComplexType(xsd_file_->FindType("GeometryType"));
  ASSERT_TRUE(geometry_type);
  const XsdComplexTypePtr& object_type =
      XsdComplexType::AsComplexType(xsd_file_->FindType("ObjectType"));
  ASSERT_TRUE(object_type);
  ASSERT_TRUE(xsd_file_->SearchTypeHierarchy(point_type, geometry_type));
  ASSERT_TRUE(xsd_file_->SearchTypeHierarchy(point_type, object_type));
}


TEST_F(XsdFileTest, TestGetElementsOfType) {
  // Add the test namespace, types, and elements to the XsdFile.
  InitTestXsd();
  const XsdComplexTypePtr geometry_type =
      XsdComplexType::AsComplexType(xsd_file_->FindType("GeometryType"));
  ASSERT_TRUE(geometry_type);
  XsdElementVector geometry_elements;
  xsd_file_->GetElementsOfType(geometry_type, &geometry_elements);
  ASSERT_EQ(static_cast<size_t>(3), geometry_elements.size());
  ASSERT_EQ(string("GeometryGroup"), geometry_elements[0]->get_name());
  ASSERT_EQ(string("LineString"), geometry_elements[1]->get_name());
  ASSERT_EQ(string("Point"), geometry_elements[2]->get_name());

  // Verify NULL element vector does not crash.
  xsd_file_->GetElementsOfType(geometry_type, NULL);
}

TEST_F(XsdFileTest, TestGetElementsOfTypeByName) {
  // Add the test namespace, types, and elements to the XsdFile.
  InitTestXsd();
  XsdElementVector object_elements;
  xsd_file_->GetElementsOfTypeByName("ObjectType", &object_elements);
  ASSERT_EQ(static_cast<size_t>(6), object_elements.size());
  ASSERT_EQ(string("FeatureGroup"), object_elements[0]->get_name());
  ASSERT_EQ(string("GeometryGroup"), object_elements[1]->get_name());
  ASSERT_EQ(string("LineString"), object_elements[2]->get_name());
  ASSERT_EQ(string("ObjectGroup"), object_elements[3]->get_name());
  ASSERT_EQ(string("Placemark"), object_elements[4]->get_name());
  ASSERT_EQ(string("Point"), object_elements[5]->get_name());

  object_elements.clear();
  xsd_file_->GetElementsOfTypeByName("NoSuchType", &object_elements);
  ASSERT_TRUE(object_elements.empty());

  // Verify NULL elements vector pointer doesn't crash.
  xsd_file_->GetElementsOfTypeByName("ObjectType", NULL);
  xsd_file_->GetElementsOfTypeByName("NoSuchType", NULL);
}

}  // end namespace kmlxsd

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
