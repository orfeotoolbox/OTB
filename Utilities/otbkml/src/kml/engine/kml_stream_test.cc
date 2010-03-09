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

// This file contains the unit tests for the KmlStream class.

#include "kml/engine/kml_stream.h"
#include <istream>
#include <sstream>
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"
#include "kml/dom.h"

using kmldom::AsFolder;
using kmldom::AsKml;
using kmldom::AsPlacemark;
using kmldom::AsPoint;
using kmldom::ElementPtr;
using kmldom::FolderPtr;
using kmldom::KmlPtr;
using kmldom::ParserObserver;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;

namespace kmlengine {

TEST(KmlStreamTest, TestBasicParseFromIstream) {
  const string kPlacemark(
    "<Placemark>"
    "<name>hello</name>"
    "<Point>"
    "<coordinates>1,2,3</coordinates>"
    "</Point>"
    "</Placemark>");
  std::istringstream string_stream(kPlacemark);
  string errors;
  boost::scoped_ptr<KmlStream>
      kml_stream(KmlStream::ParseFromIstream(&string_stream, &errors, NULL));
  ASSERT_TRUE(kml_stream.get());
  ASSERT_TRUE(errors.empty());
  ASSERT_TRUE(string_stream.eof());
  ElementPtr root = kml_stream->get_root();
  ASSERT_TRUE(root);
  PlacemarkPtr placemark = AsPlacemark(root);
  ASSERT_TRUE(placemark);
  ASSERT_EQ(string("hello"), placemark->get_name());
  ASSERT_TRUE(placemark->has_geometry());
  PointPtr point = AsPoint(placemark->get_geometry());
  ASSERT_TRUE(point);
  ASSERT_TRUE(point->has_coordinates());
  ASSERT_EQ(static_cast<size_t>(1),
            point->get_coordinates()->get_coordinates_array_size());
}

TEST(KmlStreamTest, TestBadParseFromIstream) {
  const string kEmpty;
  std::istringstream kNothing(kEmpty);
  string errors;
  ASSERT_FALSE(KmlStream::ParseFromIstream(&kNothing, &errors, NULL));
  ASSERT_FALSE(errors.empty());
}

static char kBegin[] = "<kml><Folder>";
static char kPlacemark[] = "<Placemark/>";
static char kEnd[] = "</Folder></kml>";

// This class acts like a streambuf.
class TestStreamBuf : public std::streambuf {
 public:
  TestStreamBuf(size_t placemark_count)
    : underflow_count_(0),
      placemark_count_(placemark_count + 2) {}
 protected:
  // This is the key streambuf method to override.  This method uses setg()
  // to point the streambuf user at the next chunk of data.  There are 4
  // possibilities: 1) first time: send kBegin, 2) last time: send kEnd,
  // 3) after first and before last: send kPlacemark (possibly N times),
  // 4) end of stream.
  virtual int underflow() {
   if (++underflow_count_ > placemark_count_) {
     return traits_type::eof();  // This is how to indicate end of stream.
   }
   if (underflow_count_ == 1) {
      setg(kBegin, kBegin, kBegin + strlen(kBegin));
   } else if (underflow_count_ < placemark_count_) {
      setg(kPlacemark, kPlacemark, kPlacemark + strlen(kPlacemark));
   } else if (underflow_count_ == placemark_count_) {
      setg(kEnd, kEnd, kEnd + strlen(kEnd));
   }
   return *gptr();
  }
 private:
  unsigned int underflow_count_;
  const size_t placemark_count_;
};

TEST(KmlStreamTest, TestBigParseFromIstream) {
  const size_t kFeatureCount(123456);
  TestStreamBuf test_stream_buf(kFeatureCount);
  std::istream test_istream(&test_stream_buf);
  boost::scoped_ptr<KmlStream>
      kml_stream(KmlStream::ParseFromIstream(&test_istream, NULL, NULL));
  ASSERT_TRUE(kml_stream.get());
  ElementPtr root = kml_stream->get_root();
  ASSERT_TRUE(root);
  KmlPtr kml = AsKml(root);
  ASSERT_TRUE(kml);
  ASSERT_TRUE(kml->has_feature());
  FolderPtr folder = AsFolder(kml->get_feature());
  ASSERT_TRUE(folder);
  ASSERT_EQ(kFeatureCount, folder->get_feature_array_size());
}

TEST(KmlStreamTest, TestBigParseFromIstreamWithObserver) {
  // The default ParserObserver is basically a NOP.
  ParserObserver parser_observer;
  const size_t kFeatureCount(123456);
  TestStreamBuf test_stream_buf(kFeatureCount);
  std::istream test_istream(&test_stream_buf);
  boost::scoped_ptr<KmlStream> kml_stream(
      KmlStream::ParseFromIstream(&test_istream, NULL, &parser_observer));
  ASSERT_TRUE(kml_stream.get());
  ElementPtr root = kml_stream->get_root();
  ASSERT_TRUE(root);
  KmlPtr kml = AsKml(root);
  ASSERT_TRUE(kml);
  ASSERT_TRUE(kml->has_feature());
  FolderPtr folder = AsFolder(kml->get_feature());
  ASSERT_TRUE(folder);
  ASSERT_EQ(kFeatureCount, folder->get_feature_array_size());
}

// This ParserObserver discards all Features.
class DiscardFeatures : public kmldom::ParserObserver {
 public:
  DiscardFeatures()
    : feature_counter_(0) {
  }
  virtual bool EndElement(const ElementPtr& parent, const ElementPtr& child) {
    return AsFeature(child) ? ++feature_counter_, false : true;
  }
  size_t get_feature_count() const {
    return feature_counter_;
  }
 private:
  size_t feature_counter_;
};

TEST(KmlStreamTest, TestParseFromIstreamWithObserver) {
  DiscardFeatures parser_observer;
  const size_t kFeatureCount(123456);
  TestStreamBuf test_stream_buf(kFeatureCount);
  std::istream test_istream(&test_stream_buf);
  boost::scoped_ptr<KmlStream> kml_stream(
      KmlStream::ParseFromIstream(&test_istream, NULL, &parser_observer));
  ASSERT_TRUE(kml_stream.get());
  ElementPtr root = kml_stream->get_root();
  ASSERT_TRUE(root);
  KmlPtr kml = AsKml(root);
  ASSERT_TRUE(kml);
  ASSERT_FALSE(kml->has_feature());  // Folder is a (discarded) Feature.
  ASSERT_EQ(kFeatureCount + 1, parser_observer.get_feature_count());
}

}  // end namespace kmlengine

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
