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

// This file contains the declarations of various string utility functions.

#include "kml/base/string_util.h"
#include <stdlib.h>  // strtod()
#include <string.h>  // memcpy, strchr

namespace kmlbase {

void b2a_hex(uint32_t i, char* out) {
  char map[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                  'a', 'b', 'c', 'd', 'e', 'f'};
  out[0] = map[(i >> 4) & 0xf];
  out[1] = map[(i) & 0xf];
}

string CreateExpandedStrings(const string& in,
                                  const StringMap& string_map,
                                  const string& start,
                                  const string& end) {
  string out(in);
  StringMap::const_iterator itr = string_map.begin();
  for (itr = string_map.begin(); itr != string_map.end(); ++itr)  {
    string candidate(start + itr->first + end);
    size_t start_pos = out.find(candidate);
    while (start_pos != string::npos) {
      out.replace(start_pos, candidate.size(), itr->second);
      start_pos = out.find(candidate, start_pos + candidate.size());
    }
  }
  return out;
}

void SplitStringUsing(const string& input, const string& split_string,
                      std::vector<string>* output) {
  string iter = input;
  size_t separator;
  while ((separator = iter.find(split_string)) != string::npos) {
    output->push_back(iter.substr(0, separator));
    iter = iter.substr(separator+1);
  }
  output->push_back(iter);
}

template<>
void FromString(const string& str, bool* out) {
  if (out) {
    size_t skip_ws = SkipLeadingWhitespaceString(str);
    *out = (str.compare(skip_ws, 4, "true") == 0)
            || (str.compare(skip_ws, 1, "1") == 0);
  }
}


template<>
void FromString(const string& str, double* out) {
  if (out) {
    *out = strtod(str.c_str(), NULL);
  }
}

template<>
void FromString(const string& str, int* out) {
  if (out) {
    *out = atoi(str.c_str());
  }
}

template<>
void FromString(const string& str, string* out) {
  if (out) {
    *out = str;
  }
}

bool StringEndsWith(const string& str, const string& end) {
  if (str.empty() || end.empty()) {
    return false;
  }
  if (str.size() == end.size()) {
    return str == end;
  }
  return end.size() < str.size() && str.substr(str.size() - end.size()) == end;
}

bool StringCaseEqual(const string& a, const string& b) {
#ifdef WIN32
# define strncasecmp(s1, s2, n) _strnicmp (s1, s2, n)
#endif
  return a.size() == b.size() && strncasecmp(a.data(), b.data(), a.size()) == 0;
}

bool StringToDouble(const string& number, double* output) {
  if (!IsDecimalDoubleString(number)) {
    return false;
  }
  if (output) {
    // TODO: consider protobuf's NoLocaleStrtod.
    *output = strtod(number.c_str(), NULL);
  }
  return true;
}

bool IsDecimalDoubleString(const string& number) {
  if (number.empty()) {
    return false;
  }
  char* cp = const_cast<char*>(number.data());
  const char* end = cp + number.size();
  if (*cp == '-' && ++cp == end) {
    return false;
  }
  if (*cp == '.' && ++cp == end) {
    return false;
  }
  return isdigit(*cp) != 0;
}

size_t SkipLeadingWhitespace(const char* begin, const char* end) {
  const char* cp = begin;
  for (; cp < end; cp++) {
    if (!isspace(*cp)) {
      break;
    }
  }
  return cp - begin;
}

size_t SkipLeadingWhitespaceString(const string& str) {
  return SkipLeadingWhitespace(str.data(), str.data() + str.size());
}

// Derived from and compatible with google3's SplitCSVLineWithDelimiter.
void SplitQuotedUsing(const char* input, size_t nbytes, const char delimiter,
                      std::vector<string>* cols) {
  if (input == NULL) {
    return;
  }
  // Copy the line to a buffer we can write into.
  char *copy = (char*)malloc(nbytes + 1);
  memcpy(copy, static_cast<const void*>(input), nbytes);
  copy[nbytes] = 0;

  char* end;
  char* start;

  const char* end_of_line = copy + nbytes;
  for (char* line = copy; line < end_of_line; line++) {
    // Skip leading whitespace, unless said whitespace is the delimiter.
    while (isspace(*line) && *line != delimiter)
      ++line;

    if (*line == '"' && delimiter == ',') {     // Quoted value...
      start = ++line;
      end = start;
      for (; *line; line++) {
        if (*line == '"') {
          line++;
          if (*line != '"')  // [""] is an escaped ["]
            break;           // but just ["] is end of value
        }
        *end++ = *line;
      }
      // All characters after the closing quote and before the comma
      // are ignored.
      line = strchr(line, delimiter);
      if (!line) line = const_cast<char*>(end_of_line);
    } else {
      start = line;
      line = strchr(line, delimiter);
      if (!line) line = const_cast<char*>(end_of_line);
      // Skip all trailing whitespace, unless said whitespace is the delimiter.
      for (end = line;
           end > start && isspace(end[-1]) && end[-1] != delimiter; --end)
        ;
    }
    const bool need_another_column =
      (*line == delimiter) && (line == end_of_line - 1);
    *end = '\0';
    cols->push_back(start);
    // If line was something like [paul,] (comma is the last character
    // and is not proceeded by whitespace or quote) then we are about
    // to eliminate the last column (which is empty). This would be
    // incorrect.
    if (need_another_column)
      cols->push_back(end);
  }
  free(copy);
}

void SplitQuotedUsingFromString(const string& linestr, const char delimiter,
                                std::vector<string>* cols) {
  SplitQuotedUsing(linestr.data(), linestr.size(), delimiter, cols);
}

}  // end namespace kmlbase
