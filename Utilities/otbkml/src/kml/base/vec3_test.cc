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

// This file contains the unit tests for the Vec3 class.

#include "kml/base/vec3.h"
#include "boost/scoped_ptr.hpp"
#include "gtest/gtest.h"

namespace kmlbase {

TEST(Vec3Test, TestConstructEmpty) {
  Vec3 vec3;
  ASSERT_EQ(0.0, vec3.get_latitude());
  ASSERT_EQ(0.0, vec3.get_longitude());
  ASSERT_EQ(0.0, vec3.get_altitude());
  ASSERT_EQ(false, vec3.has_altitude());
}

TEST(Vec3Test, TestConstruct2d) {
  const double kLatitude(-12.12);
  const double kLongitude(23.46);
  Vec3 vec3(kLongitude, kLatitude);
  ASSERT_EQ(kLatitude, vec3.get_latitude());
  ASSERT_EQ(kLongitude, vec3.get_longitude());
  ASSERT_EQ(0.0, vec3.get_altitude());
  ASSERT_EQ(false, vec3.has_altitude());
}

TEST(Vec3Test, TestConstruct3d) {
  const double kLatitude(-12.12);
  const double kLongitude(23.46);
  const double kAltitude(54321.0987);
  Vec3 vec3(kLongitude, kLatitude, kAltitude);
  ASSERT_EQ(kLatitude, vec3.get_latitude());
  ASSERT_EQ(kLongitude, vec3.get_longitude());
  ASSERT_EQ(kAltitude, vec3.get_altitude());
  ASSERT_EQ(true, vec3.has_altitude());
}

TEST(Vec3Test, TestSetClearAltitude) {
  const double kAltitude(54321.0987);
  Vec3 vec3(1,2);
  vec3.set_altitude(kAltitude);
  ASSERT_EQ(kAltitude, vec3.get_altitude());
  ASSERT_EQ(true, vec3.has_altitude());
  vec3.clear_altitude();
  ASSERT_EQ(0.0, vec3.get_altitude());
  ASSERT_EQ(false, vec3.has_altitude());
}

TEST(Vec3Test, TestSet3d) {
  const double k0(0.0);
  const double k1(1.0);
  const double k2(2.0);
  Vec3 vec;
  vec.set(0, k0);
  vec.set(1, k1);
  vec.set(2, k2);
  ASSERT_EQ(k0, vec.get_longitude());
  ASSERT_EQ(k1, vec.get_latitude());
  ASSERT_TRUE(vec.has_altitude());
  ASSERT_EQ(k2, vec.get_altitude());
}

TEST(Vec3Test, TestSet2d) {
  const double k0(0.0);
  const double k1(1.0);
  Vec3 vec;
  vec.set(0, k0);
  vec.set(1, k1);
  ASSERT_EQ(k0, vec.get_longitude());
  ASSERT_EQ(k1, vec.get_latitude());
  ASSERT_FALSE(vec.has_altitude());
}

TEST(Vec3Test, TestEquivalence) {
  Vec3 vec0(0.1, 1.2, 2.3);
  Vec3 vec1(1.1, 1.2, 2.3);
  Vec3 vec2(1.1, 1.2, 0.0);
  Vec3 vec3(1.1, 1.2);
  Vec3 vec4(1.1, 1.2, 0.1);
  ASSERT_FALSE(vec0 == vec1);
  ASSERT_TRUE(vec0 == vec0);
  ASSERT_TRUE(vec1 == vec1);
  ASSERT_TRUE(vec3 == vec3);
  ASSERT_TRUE(vec2 == vec3);
  ASSERT_FALSE(vec3 == vec4);
}

TEST(Vec3Test, TestCopyConstruct3d) {
  Vec3 vec3d(0.1, 1.2, 2.3);
  Vec3 copy3d(vec3d);
  ASSERT_EQ(0.1, copy3d.get_longitude());
  ASSERT_EQ(1.2, copy3d.get_latitude());
  ASSERT_TRUE(copy3d.has_altitude());
  ASSERT_EQ(2.3, copy3d.get_altitude());
}

TEST(Vec3Test, TestCopyConstruct2d) {
  Vec3 vec2d(10.11, -12.13);
  Vec3 copy2d(vec2d);
  ASSERT_EQ(10.11, copy2d.get_longitude());
  ASSERT_EQ(-12.13, copy2d.get_latitude());
  ASSERT_FALSE(copy2d.has_altitude());
  ASSERT_EQ(0, copy2d.get_altitude());
}

TEST(Vec3Test, TestAssign3d) {
  Vec3 vec3d(0.1, 1.2, 2.3);
  Vec3 copy3d;
  copy3d = vec3d;
  ASSERT_EQ(0.1, copy3d.get_longitude());
  ASSERT_EQ(1.2, copy3d.get_latitude());
  ASSERT_TRUE(copy3d.has_altitude());
  ASSERT_EQ(2.3, copy3d.get_altitude());
}

TEST(Vec3Test, TestAssign2d) {
  Vec3 vec2d(10.11, -12.13);
  Vec3 copy2d;
  copy2d = vec2d;
  ASSERT_EQ(10.11, copy2d.get_longitude());
  ASSERT_EQ(-12.13, copy2d.get_latitude());
  ASSERT_FALSE(copy2d.has_altitude());
  ASSERT_EQ(0, copy2d.get_altitude());
}

}  // end namespace kmlbase

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
