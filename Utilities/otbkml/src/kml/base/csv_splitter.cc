// Copyright 2010, Google Inc. All rights reserved.
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

// This file contains the implementation of the CsvSplitter class.

#include "kml/base/csv_splitter.h"

#include "kml/base/string_util.h"

namespace kmlbase {

CsvSplitter::CsvSplitter(const string& csv_data)
  : csv_data_(csv_data),
    current_line_(0) {
}

CsvSplitter::~CsvSplitter() {}

size_t CsvSplitter::FindNextLine(size_t* this_end) const {
  size_t nl = csv_data_.find_first_of("\r\n", current_line_);
  if (nl == string::npos) {
    if (this_end) {
      *this_end = csv_data_.size();
    }
  } else {
    if (this_end) {
      *this_end = nl;
    }
    return csv_data_.find_first_not_of("\r\n", nl);
  }
  return nl;
}

bool CsvSplitter::SplitCurrentLine(StringVector* cols) {
  if (current_line_ > csv_data_.size()) {
    return false;
  }
  size_t this_begin = current_line_;
  size_t this_end;
  current_line_ = FindNextLine(&this_end);
  if (cols) {
    SplitQuotedUsing(csv_data_.data() + this_begin, this_end - this_begin, ',',
                     cols);
  }
  return true;
}

}  // end namespace kmlbase
