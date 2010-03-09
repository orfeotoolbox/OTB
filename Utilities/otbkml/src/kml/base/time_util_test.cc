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

// Uncomment this #define to enable output of timing results.
// #define PRINT_TIME_RESULTS
#ifdef PRINT_TIME_RESULTS
#include <iostream>
#endif

#include "kml/base/time_util.h"
#include <time.h>
#include "gtest/gtest.h"

namespace kmlbase {

class TimeUtilTest : public testing::Test {
};

// This verifies the GetMicroTime() function.
TEST_F(TimeUtilTest, TestGetMicroTime) {
  // Get the posix time (second resolution).
  time_t now = time(NULL);
  // Get the micro time (microsecond resolution).
  double later = GetMicroTime();
  // Assert that time has passed.
  ASSERT_PRED_FORMAT2(testing::DoubleLE, static_cast<double>(now), later);
  // Snapshot the microtime in rapid succession.
  double even_later = GetMicroTime();
  double later_still = GetMicroTime();
  // Verify that time does not go backwards.
  ASSERT_PRED_FORMAT2(testing::DoubleLE, later, even_later);
  ASSERT_PRED_FORMAT2(testing::DoubleLE, even_later, later_still);

  // Here are some values 2.16 GHz MacBook Pro running Mac OS X 10.5.3.
  //  now         1215742903
  //  later       1215742903.291807
  //  even_later  1215742903.291839
  //  later_still 1215742903.291839

#ifdef PRINT_TIME_RESULTS
  std::cerr << now << std::endl;
  std::cerr.precision(16);
  std::cerr << later << std::endl;
  std::cerr << even_later << std::endl;
  std::cerr << later_still << std::endl;
#endif
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
