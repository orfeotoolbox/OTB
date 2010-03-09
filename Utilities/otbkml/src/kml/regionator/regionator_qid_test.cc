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

// This file contains the unit tests for the internal Qid class.

#include "kml/regionator/regionator_qid.h"
#include "gtest/gtest.h"

namespace kmlregionator {

// This class is the unit test fixture for the KmlHandler class.
class RegionatorQidTest : public testing::Test {
 protected:
  virtual void SetUp() {
    root_ = Qid::CreateRoot();
  }

  Qid root_;
};

// This tests the CreateRoot(), depth(), and str() methods of class Qid.
TEST_F(RegionatorQidTest, TestRoot) {
  ASSERT_EQ(static_cast<size_t>(1), root_.depth());
  ASSERT_EQ(string("q0"), root_.str());
}

// This tests the CreateChild() method of class Qid.
TEST_F(RegionatorQidTest, TestCreateChild) {
  Qid nw = root_.CreateChild(NW);
  Qid ne = root_.CreateChild(NE);
  Qid sw = root_.CreateChild(SW);
  Qid se = root_.CreateChild(SE);
  ASSERT_EQ(string("q00"), nw.str());
  ASSERT_EQ(string("q01"), ne.str());
  ASSERT_EQ(string("q02"), sw.str());
  ASSERT_EQ(string("q03"), se.str());
  ASSERT_EQ(static_cast<size_t>(2), nw.depth());
  ASSERT_EQ(static_cast<size_t>(2), ne.depth());
  ASSERT_EQ(static_cast<size_t>(2), sw.depth());
  ASSERT_EQ(static_cast<size_t>(2), se.depth());
}

// This tests a few more normal usage scenarios.
TEST_F(RegionatorQidTest, TestCreateChildVarious) {
  Qid q0123("q0123");
  Qid q0123_nw = q0123.CreateChild(NW);
  ASSERT_EQ(string("q01230"), q0123_nw.str());
  ASSERT_EQ(static_cast<size_t>(5), q0123_nw.depth());

  Qid deep("q01233211231231231231");
  Qid deep_ne = deep.CreateChild(NE);
  ASSERT_EQ(string("q012332112312312312311"), deep_ne.str());
  ASSERT_EQ(static_cast<size_t>(21), deep_ne.depth());
}

}  // end namespace kmlregionator

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
