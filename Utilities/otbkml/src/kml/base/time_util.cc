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

#include "kml/base/time_util.h"
#include <cstddef>  // NULL
#ifdef WIN32
#include <winsock2.h>  // This header MUST come before windows.h. See
#include <windows.h>   // http://code.google.com/p/libkml/issues/detail?id=31
#include <time.h>
#else
#include <sys/time.h>
#endif

namespace kmlbase {

#ifdef WIN32
// http://forums.msdn.microsoft.com/en/vcgeneral/thread/430449b3-f6dd-4e18-84de-eebd26a8d668/
#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
#else
#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
#endif
// 
void gettimeofday(struct timeval* tv, void *) {
  FILETIME file_time;
  GetSystemTimeAsFileTime(&file_time);

  // First covert file_time to a single 64 bit value.
  unsigned __int64 file_time64 = 0;
  file_time64 |= file_time.dwHighDateTime;
  file_time64 <<= 32;
  file_time64 |= file_time.dwLowDateTime;
 
  /* Now convert file_time64 to unix time. */
  file_time64 /= 10;  /* Convert into microseconds. */
  file_time64 -= DELTA_EPOCH_IN_MICROSECS;
  tv->tv_sec = (long)(file_time64 / 1000000UL);
  tv->tv_usec = (long)(file_time64 % 1000000UL);
}
#endif

// This is here because Windows has no gettimeofday().
double GetMicroTime() {
  struct timeval now_tv;
  gettimeofday(&now_tv, NULL);
  // Make this one double with secs.microseconds.
  return (double)now_tv.tv_sec + (double)now_tv.tv_usec/1000000;
}

}  // end namespace kmlbase
