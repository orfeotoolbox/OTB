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

// This file contains the unit tests for gx:TimeSpan and gx:TimeStamp.

#include "kml/dom/gx_timeprimitive.h"
#include "boost/scoped_ptr.hpp"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "gtest/gtest.h"

namespace kmldom {

// Test GxTimeSpan.
class GxTimeSpanTest : public testing::Test {
 protected:
  virtual void SetUp() {
    gx_timespan_ = KmlFactory::GetFactory()->CreateGxTimeSpan();
  }

  GxTimeSpanPtr gx_timespan_;
};

TEST_F(GxTimeSpanTest, TestType) {
  ASSERT_EQ(Type_GxTimeSpan, gx_timespan_->Type());
  ASSERT_TRUE(gx_timespan_->IsA(Type_GxTimeSpan));
  ASSERT_TRUE(gx_timespan_->IsA(Type_TimePrimitive));
  ASSERT_TRUE(gx_timespan_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(GxTimeSpanTest, TestDefaults) {
  ASSERT_FALSE(gx_timespan_->has_begin());
  ASSERT_FALSE(gx_timespan_->has_end());
}

// Verify setting default makes has_xxx() true:
TEST_F(GxTimeSpanTest, TestSetToDefaultValues) {
  gx_timespan_->set_begin(gx_timespan_->get_begin());
  ASSERT_TRUE(gx_timespan_->has_begin());
  gx_timespan_->set_end(gx_timespan_->get_end());
  ASSERT_TRUE(gx_timespan_->has_end());
}

// Verify set, get, has, clear:
TEST_F(GxTimeSpanTest, TestSetGetHasClear) {
  const string begin = "2008-01";
  const string end = "2008-02";

  // Set all fields:
  gx_timespan_->set_begin(begin);
  gx_timespan_->set_end(end);

  // Verify getter and has_xxx():
  ASSERT_TRUE(gx_timespan_->has_begin());
  ASSERT_EQ(begin, gx_timespan_->get_begin());
  ASSERT_TRUE(gx_timespan_->has_end());
  ASSERT_EQ(end, gx_timespan_->get_end());

  // Clear all fields:
  gx_timespan_->clear_begin();
  gx_timespan_->clear_end();
}

// Test GxTimeStamp.
class GxTimeStampTest : public testing::Test {
 protected:
  virtual void SetUp() {
    gx_timestamp_ = KmlFactory::GetFactory()->CreateGxTimeStamp();
  }

  GxTimeStampPtr gx_timestamp_;
};

TEST_F(GxTimeStampTest, TestType) {
  ASSERT_EQ(Type_GxTimeStamp, gx_timestamp_->Type());
  ASSERT_TRUE(gx_timestamp_->IsA(Type_GxTimeStamp));
  ASSERT_TRUE(gx_timestamp_->IsA(Type_TimePrimitive));
  ASSERT_TRUE(gx_timestamp_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(GxTimeStampTest, TestDefaults) {
  ASSERT_FALSE(gx_timestamp_->has_when());
}

// Verify setting default makes has_xxx() true:
TEST_F(GxTimeStampTest, TestSetToDefaultValues) {
  gx_timestamp_->set_when(gx_timestamp_->get_when());
  ASSERT_TRUE(gx_timestamp_->has_when());
}

// Verify set, get, has, clear:
TEST_F(GxTimeStampTest, TestSetGetHasClear) {
  const string when = "2008-01-23T18:13:07Z";

  // Set all fields:
  gx_timestamp_->set_when(when);

  // Verify getter and has_xxx():
  ASSERT_TRUE(gx_timestamp_->has_when());
  ASSERT_EQ(when, gx_timestamp_->get_when());

  // Clear all fields:
  gx_timestamp_->clear_when();
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
