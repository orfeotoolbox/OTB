// Copyright 2009, Google Inc. All rights reserved.
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

// This file contains the unit tests for the abstract Object element.

#include "kml/dom/visitor.h"
#include "kml/dom/kmldom.h"
#include "kml/dom/kml_factory.h"
#include "gtest/gtest.h"

namespace kmldom {

class TestVisitor : public Visitor {
 public:
  TestVisitor()
      : visit_element_was_called_(false),
        visit_feature_was_called_(false),
        visit_container_was_called_(false) {
  }
  virtual ~TestVisitor() { }

  virtual void VisitElement(const ElementPtr& element) {
    visit_element_was_called_ = true;
  }

  virtual void VisitFeature(const FeaturePtr& element) {
    visit_feature_was_called_ = true;
    Visitor::VisitFeature(element);
  }

  virtual void VisitContainer(const ContainerPtr& element) {
    visit_container_was_called_ = true;
    Visitor::VisitContainer(element);
  }

  bool visit_element_was_called_;
  bool visit_feature_was_called_;
  bool visit_container_was_called_;
};

TEST(Visitor, TestVisitElement) {
  KmlFactory* factory = KmlFactory::GetFactory();
  KmlPtr element = factory->CreateKml();
  TestVisitor visitor;
  element->Accept(&visitor);
  EXPECT_TRUE(visitor.visit_element_was_called_);
  EXPECT_FALSE(visitor.visit_feature_was_called_);
  EXPECT_FALSE(visitor.visit_container_was_called_);
}

TEST(Visitor, TestVisitFeature) {
  KmlFactory* factory = KmlFactory::GetFactory();
  FeaturePtr feature = factory->CreatePlacemark();
  TestVisitor visitor;
  feature->Accept(&visitor);
  EXPECT_TRUE(visitor.visit_element_was_called_);
  EXPECT_TRUE(visitor.visit_feature_was_called_);
  EXPECT_FALSE(visitor.visit_container_was_called_);
}

TEST(Visitor, TestVisitContainer) {
  KmlFactory* factory = KmlFactory::GetFactory();
  DocumentPtr container = factory->CreateDocument();
  TestVisitor visitor;
  container->Accept(&visitor);
  EXPECT_TRUE(visitor.visit_element_was_called_);
  EXPECT_TRUE(visitor.visit_feature_was_called_);
  EXPECT_TRUE(visitor.visit_container_was_called_);
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
