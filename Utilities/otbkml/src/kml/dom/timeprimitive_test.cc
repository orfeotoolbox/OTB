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

// This file contains the unit tests for TimeSpan and TimeStamp.

#include "kml/dom/timeprimitive.h"
#include "boost/scoped_ptr.hpp"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "gtest/gtest.h"

namespace kmldom {

// Test TimePrimitive.
class TimePrimitiveTest : public testing::Test {
 protected:
  virtual void SetUp() {
    timeprimitive_.reset(new TestTimePrimitive());
  }

  // Derive a test class since TimePrimitive is abstract.
  class TestTimePrimitive : public TimePrimitive {
  };
  boost::scoped_ptr<TestTimePrimitive> timeprimitive_;
};

TEST_F(TimePrimitiveTest, TestType) {
  ASSERT_TRUE(timeprimitive_->IsA(Type_TimePrimitive));
  ASSERT_TRUE(timeprimitive_->IsA(Type_Object));
}

// Test TimeSpan.
class TimeSpanTest : public testing::Test {
 protected:
  virtual void SetUp() {
    timespan_ = KmlFactory::GetFactory()->CreateTimeSpan();
  }

  TimeSpanPtr timespan_;
};

TEST_F(TimeSpanTest, TestType) {
  ASSERT_EQ(Type_TimeSpan, timespan_->Type());
  ASSERT_TRUE(timespan_->IsA(Type_TimeSpan));
  ASSERT_TRUE(timespan_->IsA(Type_TimePrimitive));
  ASSERT_TRUE(timespan_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(TimeSpanTest, TestDefaults) {
  ASSERT_FALSE(timespan_->has_begin());
  ASSERT_FALSE(timespan_->has_end());
}

// Verify setting default makes has_xxx() true:
TEST_F(TimeSpanTest, TestSetToDefaultValues) {
  timespan_->set_begin(timespan_->get_begin());
  ASSERT_TRUE(timespan_->has_begin());
  timespan_->set_end(timespan_->get_end());
  ASSERT_TRUE(timespan_->has_end());
}

// Verify set, get, has, clear:
TEST_F(TimeSpanTest, TestSetGetHasClear) {
  const string begin = "2008-01";
  const string end = "2008-02";

  // Set all fields:
  timespan_->set_begin(begin);
  timespan_->set_end(end);

  // Verify getter and has_xxx():
  ASSERT_TRUE(timespan_->has_begin());
  ASSERT_EQ(begin, timespan_->get_begin());
  ASSERT_TRUE(timespan_->has_end());
  ASSERT_EQ(end, timespan_->get_end());

  // Clear all fields:
  timespan_->clear_begin();
  timespan_->clear_end();
}

// Test TimeStamp.
class TimeStampTest : public testing::Test {
 protected:
  virtual void SetUp() {
    timestamp_ = KmlFactory::GetFactory()->CreateTimeStamp();
  }

  TimeStampPtr timestamp_;
};

TEST_F(TimeStampTest, TestType) {
  ASSERT_EQ(Type_TimeStamp, timestamp_->Type());
  ASSERT_TRUE(timestamp_->IsA(Type_TimeStamp));
  ASSERT_TRUE(timestamp_->IsA(Type_TimePrimitive));
  ASSERT_TRUE(timestamp_->IsA(Type_Object));
}

// Verify proper defaults:
TEST_F(TimeStampTest, TestDefaults) {
  ASSERT_FALSE(timestamp_->has_when());
}

// Verify setting default makes has_xxx() true:
TEST_F(TimeStampTest, TestSetToDefaultValues) {
  timestamp_->set_when(timestamp_->get_when());
  ASSERT_TRUE(timestamp_->has_when());
}

// Verify set, get, has, clear:
TEST_F(TimeStampTest, TestSetGetHasClear) {
  const string when = "2008-01-23T18:13:07Z";

  // Set all fields:
  timestamp_->set_when(when);

  // Verify getter and has_xxx():
  ASSERT_TRUE(timestamp_->has_when());
  ASSERT_EQ(when, timestamp_->get_when());

  // Clear all fields:
  timestamp_->clear_when();
}

}  // end namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
