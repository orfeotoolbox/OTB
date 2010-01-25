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
#include <string>
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_ptr.h"
#include "kml/base/unit_test.h"

namespace kmldom {

// Test TimePrimitive.
class TimePrimitiveTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(TimePrimitiveTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST_SUITE_END();

  // Derive a test class since TimePrimitive is abstract.
  class TestTimePrimitive : public TimePrimitive {
  };

 public:
  // Called before each test.
  void setUp() {
    timeprimitive_ = new TestTimePrimitive();
  }

  // Called after each test.
  void tearDown() {
    delete timeprimitive_;
  }

 protected:
  void TestType();

 private:
  TestTimePrimitive* timeprimitive_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(TimePrimitiveTest);

void TimePrimitiveTest::TestType() {
  CPPUNIT_ASSERT(true == timeprimitive_->IsA(Type_TimePrimitive));
  CPPUNIT_ASSERT(true == timeprimitive_->IsA(Type_Object));
}

// Test TimeSpan.
class TimeSpanTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(TimeSpanTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    timespan_ = KmlFactory::GetFactory()->CreateTimeSpan();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  TimeSpanPtr timespan_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(TimeSpanTest);

void TimeSpanTest::TestType() {
  CPPUNIT_ASSERT(Type_TimeSpan == timespan_->Type());
  CPPUNIT_ASSERT(true == timespan_->IsA(Type_TimeSpan));
  CPPUNIT_ASSERT(true == timespan_->IsA(Type_TimePrimitive));
  CPPUNIT_ASSERT(true == timespan_->IsA(Type_Object));
}

// Verify proper defaults:
void TimeSpanTest::TestDefaults() {
  CPPUNIT_ASSERT(false == timespan_->has_begin());
  CPPUNIT_ASSERT(false == timespan_->has_end());
}

// Verify setting default makes has_xxx() true:
void TimeSpanTest::TestSetToDefaultValues() {
  TestDefaults();
  timespan_->set_begin(timespan_->get_begin());
  CPPUNIT_ASSERT(true == timespan_->has_begin());
  timespan_->set_end(timespan_->get_end());
  CPPUNIT_ASSERT(true == timespan_->has_end());
}

// Verify set, get, has, clear:
void TimeSpanTest::TestSetGetHasClear() {
  const std::string begin = "2008-01";
  const std::string end = "2008-02";

  // Set all fields:
  timespan_->set_begin(begin);
  timespan_->set_end(end);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == timespan_->has_begin());
  CPPUNIT_ASSERT(begin == timespan_->get_begin());
  CPPUNIT_ASSERT(true == timespan_->has_end());
  CPPUNIT_ASSERT(end == timespan_->get_end());

  // Clear all fields:
  timespan_->clear_begin();
  timespan_->clear_end();

  // Verify now in default state:
  TestDefaults();
}

// Test TimeStamp.
class TimeStampTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(TimeStampTest);
  CPPUNIT_TEST(TestType);
  CPPUNIT_TEST(TestDefaults);
  CPPUNIT_TEST(TestSetToDefaultValues);
  CPPUNIT_TEST(TestSetGetHasClear);
  CPPUNIT_TEST_SUITE_END();

 public:
  // Called before each test.
  void setUp() {
    timestamp_ = KmlFactory::GetFactory()->CreateTimeStamp();
  }

  // Called after each test.
  void tearDown() {
  }

 protected:
  void TestType();
  void TestDefaults();
  void TestSetToDefaultValues();
  void TestSetGetHasClear();

 private:
  TimeStampPtr timestamp_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(TimeStampTest);

void TimeStampTest::TestType() {
  CPPUNIT_ASSERT(Type_TimeStamp == timestamp_->Type());
  CPPUNIT_ASSERT(true == timestamp_->IsA(Type_TimeStamp));
  CPPUNIT_ASSERT(true == timestamp_->IsA(Type_TimePrimitive));
  CPPUNIT_ASSERT(true == timestamp_->IsA(Type_Object));
}

// Verify proper defaults:
void TimeStampTest::TestDefaults() {
  CPPUNIT_ASSERT(false == timestamp_->has_when());
}

// Verify setting default makes has_xxx() true:
void TimeStampTest::TestSetToDefaultValues() {
  TestDefaults();
  timestamp_->set_when(timestamp_->get_when());
  CPPUNIT_ASSERT(true == timestamp_->has_when());
}

// Verify set, get, has, clear:
void TimeStampTest::TestSetGetHasClear() {
  const std::string when = "2008-01-23T18:13:07Z";

  // Set all fields:
  timestamp_->set_when(when);

  // Verify getter and has_xxx():
  CPPUNIT_ASSERT(true == timestamp_->has_when());
  CPPUNIT_ASSERT(when == timestamp_->get_when());

  // Clear all fields:
  timestamp_->clear_when();

  // Verify now in default state:
  TestDefaults();
}

}  // end namespace kmldom

TEST_MAIN
