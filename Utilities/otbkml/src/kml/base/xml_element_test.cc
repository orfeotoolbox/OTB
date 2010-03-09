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

// This file contains the unit tests for the XmlElement class.

#include "kml/base/xml_element.h"
#include "boost/intrusive_ptr.hpp"
#include "gtest/gtest.h"
#include "kml/base/xml_file.h"

namespace kmlbase {

// Forward declare TestElement for use the TestElementPtr typedef.
class TestElement;

// XmlElement derives from Referent such that any derived class can use
// boost::intrusive_ptr.
typedef boost::intrusive_ptr<TestElement> TestElementPtr;

// The typical use case of XmlElement is to derive a concrete (or abstract)
// XML element class.
class TestElement : public XmlElement {
 public:
  bool SetChild(const TestElementPtr& child) {
    // Any XmlElement taking on a child XmlElement should check parentage of
    // the potential child using XmlElement::SetParent:
    if (!child_ && child->SetParent(this)) {
      // This child had no existing parent so we can make this our own.
      child_ = child;
      return true;
    }
    return false;
  }
 private:
  TestElementPtr child_;
};

// A most basic XmlFile.
class TestFile : public XmlFile {
};

typedef boost::intrusive_ptr<TestFile> TestFilePtr;

TEST(XmlElementTest, TestDefault) {
  TestElementPtr element = new TestElement;
  ASSERT_FALSE(element->GetParent());
  ASSERT_FALSE(element->GetXmlFile());
  ASSERT_TRUE(element->InSameXmlFile(element));
}

TEST(XmlElementTest, TestSetGetParent) {
  TestElementPtr parent = new TestElement;
  TestElementPtr child = new TestElement;
  ASSERT_TRUE(parent->SetChild(child));
  ASSERT_TRUE(child->GetParent());
  ASSERT_FALSE(parent->SetChild(child));
}

TEST(XmlElementTest, TestSetSameXmlFile) {
  TestElementPtr e0 = new TestElement;
  TestElementPtr e1 = new TestElement;
  TestFilePtr f0 = new TestFile;
  TestFilePtr f1 = new TestFile;
  ASSERT_TRUE(e0->SetXmlFile(f0.get()));
  ASSERT_TRUE(e1->SetXmlFile(f1.get()));
  ASSERT_FALSE(e1->InSameXmlFile(e0));
  ASSERT_FALSE(e0->InSameXmlFile(e1));
  ASSERT_TRUE(e0->InSameXmlFile(e0));
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
