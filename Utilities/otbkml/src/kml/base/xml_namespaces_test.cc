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

#include "kml/base/xml_namespaces.h"
#include "gtest/gtest.h"

namespace kmlbase {

TEST(XmlNamespacesTest, TestFindXmlNamespaceAndPrefix) {
  string prefix;
  string xml_namespace;
  ASSERT_FALSE(FindXmlNamespaceAndPrefix(XMLNS_NONE, NULL, NULL));
  ASSERT_TRUE(FindXmlNamespaceAndPrefix(XMLNS_KML22, NULL, NULL));
  ASSERT_TRUE(FindXmlNamespaceAndPrefix(XMLNS_KML22, &prefix, NULL));
  ASSERT_TRUE(!prefix.empty());
  ASSERT_EQ(string("kml"), prefix);
  ASSERT_TRUE(FindXmlNamespaceAndPrefix(XMLNS_KML22, NULL, &xml_namespace));
  ASSERT_EQ(string("http://www.opengis.net/kml/2.2"), xml_namespace);
  prefix.clear();
  xml_namespace.clear();
  ASSERT_TRUE(FindXmlNamespaceAndPrefix(XMLNS_KML22, &prefix, &xml_namespace));
  ASSERT_EQ(string("kml"), prefix);
  ASSERT_EQ(string("http://www.opengis.net/kml/2.2"), xml_namespace);
  ASSERT_FALSE(FindXmlNamespaceAndPrefix(static_cast<XmlnsId>(1231231), NULL,
                                         NULL));
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
