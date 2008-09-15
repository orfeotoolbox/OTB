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

#include "kml/dom/document.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/base/unit_test.h"

namespace kmldom {

class DocumentTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(DocumentTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestSchema);
  CPPUNIT_TEST(TestStyleSelectors);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    document_ = KmlFactory::GetFactory()->CreateDocument();
  }
  void tearDown() {
  }

 protected:
  void TestType();
  void TestSchema();
  void TestStyleSelectors();

 private:
  DocumentPtr document_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(DocumentTest);

void DocumentTest::TestType() {
  CPPUNIT_ASSERT(Type_Document == document_->Type());
  CPPUNIT_ASSERT(true == document_->IsA(Type_Document));
  CPPUNIT_ASSERT(true == document_->IsA(Type_Container));
  CPPUNIT_ASSERT(true == document_->IsA(Type_Feature));
  CPPUNIT_ASSERT(true == document_->IsA(Type_Object));
}

void DocumentTest::TestSchema() {
  CPPUNIT_ASSERT(0 == document_->get_schema_array_size());
  SchemaPtr s0(KmlFactory::GetFactory()->CreateSchema());
  SchemaPtr s1(KmlFactory::GetFactory()->CreateSchema());
  SchemaPtr s3(KmlFactory::GetFactory()->CreateSchema());
  document_->add_schema(s0);
  document_->add_schema(s1);
  document_->add_schema(s3);
  CPPUNIT_ASSERT(3 == document_->get_schema_array_size());
  CPPUNIT_ASSERT(Type_Schema == document_->get_schema_array_at(0)->Type());
  CPPUNIT_ASSERT(Type_Schema == document_->get_schema_array_at(1)->Type());
  CPPUNIT_ASSERT(Type_Schema == document_->get_schema_array_at(2)->Type());
}

void DocumentTest::TestStyleSelectors() {
  CPPUNIT_ASSERT(0 == document_->get_styleselector_array_size());
  StylePtr s0(KmlFactory::GetFactory()->CreateStyle());
  StylePtr s1(KmlFactory::GetFactory()->CreateStyle());
  StyleMapPtr sm(KmlFactory::GetFactory()->CreateStyleMap());
  document_->add_styleselector(s0);
  document_->add_styleselector(s1);
  document_->add_styleselector(sm);
  CPPUNIT_ASSERT(3 == document_->get_styleselector_array_size());
  CPPUNIT_ASSERT(Type_Style ==
                 document_->get_styleselector_array_at(0)->Type());
  CPPUNIT_ASSERT(Type_Style ==
                 document_->get_styleselector_array_at(1)->Type());
  CPPUNIT_ASSERT(Type_StyleMap ==
                 document_->get_styleselector_array_at(2)->Type());
}

}  // end namespace kmldom

TEST_MAIN
