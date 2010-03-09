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

// This file contains the unit tests for the XmlFile class.

#include "kml/base/xml_file.h"
#include "gtest/gtest.h"

namespace kmlbase {

// Derive the simplest possible class to test XmlFile.
class TestFile : public XmlFile {
 public:
  // Reflect the otherwise protected set_url out to public for unit testing.
  void set_url(const string& url) {
    XmlFile::set_url(url);
  }
  bool set_root(const XmlElementPtr& element) {
    return XmlFile::set_root(element);
  }
};
typedef boost::intrusive_ptr<TestFile> TestFilePtr;

class TestElement : public XmlElement {
 public:
  TestElement(int id) : id_(id) {}
  int get_id() const {
    return id_;
  }
 private:
  int id_;
};
typedef boost::intrusive_ptr<TestElement> TestElementPtr;

TEST(XmlFileTest, TestDefault) {
  TestFilePtr xml_file = new TestFile;
  ASSERT_TRUE(xml_file->get_url().empty());
  ASSERT_FALSE(xml_file->get_root());
}

TEST(XmlFileTest, TestSetGet) {
  const string kUrl("http://example.com");
  const int kId = 42;
  TestFilePtr xml_file = new TestFile;
  xml_file->set_url(kUrl);
  xml_file->set_root(new TestElement(kId));
  ASSERT_EQ(kUrl, xml_file->get_url());
  TestElementPtr element =
    boost::static_pointer_cast<TestElement>(xml_file->get_root());
  ASSERT_EQ(kId, element->get_id());
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
