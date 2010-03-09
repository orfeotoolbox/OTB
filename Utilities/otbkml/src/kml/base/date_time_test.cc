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

// This file contains the unit tests for the DateTime class.

#include "kml/base/date_time.h"
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"

namespace kmlbase {

class DateTimeTest : public testing::Test {
 protected:
  boost::scoped_ptr<DateTime> date_time_;
};

TEST_F(DateTimeTest, TestCreate) {
  // Call the method under test.
  const string kDateTime("2008-10-03T09:25:42Z");
  date_time_.reset(DateTime::Create(kDateTime));
  ASSERT_TRUE(date_time_.get());
  ASSERT_EQ(static_cast<time_t>(1223025942), date_time_->GetTimeT());
  ASSERT_EQ(string("09:25:42"), date_time_->GetXsdTime());
  ASSERT_EQ(string("2008-10-03"), date_time_->GetXsdDate());
  ASSERT_EQ(kDateTime, date_time_->GetXsdDateTime());
}

// 2007-01-14T22:57:31.000Z

// Verify expected behavior on invalid input.
TEST_F(DateTimeTest, TestBad) {
  date_time_.reset(DateTime::Create("garbage"));
  ASSERT_FALSE(date_time_.get());
}

TEST_F(DateTimeTest, TestToTimeT) {
  ASSERT_EQ(1223025942, DateTime::ToTimeT("2008-10-03T09:25:42Z"));
  ASSERT_EQ(0, DateTime::ToTimeT("complete invalid input"));
}


}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
