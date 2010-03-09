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

// This file contains the implementation of the CsvParser class.

#include "kml/convenience/csv_parser.h"

#include <vector>
#include "boost/scoped_ptr.hpp"
#include "kml/base/csv_splitter.h"
#include "kml/base/string_util.h"
#include "kml/convenience/convenience.h"
#include "kml/dom.h"
#include "kml/engine.h"

namespace kmlconvenience {

static const size_t npos = -1;

// static
bool CsvParser::ParseCsv(kmlbase::CsvSplitter* csv_splitter,
                     CsvParserHandler* csv_parser_handler) {
  if (!csv_splitter || !csv_parser_handler) {
    return false;
  }
  kmlbase::StringVector schema;
  if (!csv_splitter->SplitCurrentLine(&schema)) {
    return false;
  }
  boost::scoped_ptr<CsvParser> csv_parser(
      new CsvParser(csv_splitter, csv_parser_handler));
  CsvParserStatus schema_status = csv_parser->SetSchema(schema);
  // Send the schema parsing status out just like any other line.
  if (schema_status != CSV_PARSER_STATUS_OK) {
    csv_parser_handler->HandleLine(1, schema_status, NULL);
    return false;
  }
  return csv_parser->ParseCsvData();
}

// private
CsvParser::CsvParser(kmlbase::CsvSplitter* csv_splitter,
                     CsvParserHandler* csv_parser_handler)
  : csv_splitter_(csv_splitter),
    csv_parser_handler_(csv_parser_handler),
    name_col_(npos),
    description_col_(npos),
    lat_col_(npos),
    lon_col_(npos) {
}

// private
CsvParserStatus CsvParser::SetSchema(const kmlbase::StringVector& csv_schema) {
  if (csv_schema.empty()) {
    return CSV_PARSER_STATUS_BLANK_LINE;
  }
  schema_size_ = csv_schema.size();
  for (size_t i = 0; i < schema_size_; ++i) {
    const string& this_col = csv_schema[i];
    if (kmlbase::StringCaseEqual(this_col, "name")) {
      name_col_ = i;
    } else if (kmlbase::StringCaseEqual(this_col, "description")) {
      description_col_ = i;
    } else if (kmlbase::StringCaseEqual(this_col, "latitude")) {
      lat_col_ = i;
    } else if (kmlbase::StringCaseEqual(this_col, "longitude")) {
      lon_col_ = i;
    } else {
      csv_schema_[i] = this_col;
    }
  }
  if (lat_col_ == npos || lon_col_ == npos) {
    return CSV_PARSER_STATUS_NO_LAT_LON;
  }
  return CSV_PARSER_STATUS_OK;
}

CsvParserStatus CsvParser::CsvLineToPlacemark(
    kmlbase::StringVector& csv_line, kmldom::PlacemarkPtr placemark) const {
  if (csv_line.size() != schema_size_) {
    return CSV_PARSER_STATUS_INVALID_DATA;
  }

  // Note that StringToDouble returns false on non-numeric strings.
  double lat;
  double lon;
  if (lat_col_ != npos && lon_col_ != npos &&
      csv_line.size() > lat_col_ && csv_line.size() > lon_col_ &&
      kmlbase::StringToDouble(csv_line[lat_col_], &lat) &&
      kmlbase::StringToDouble(csv_line[lon_col_], &lon)) {
    placemark->set_geometry(CreatePointLatLon(lat, lon));
  } else {
    return CSV_PARSER_STATUS_BAD_LAT_LON;
  }
  if (name_col_ != npos) {
    placemark->set_name(csv_line[name_col_]);
  }
  if (description_col_ != npos) {
    placemark->set_description(csv_line[description_col_]);
  }
  // Walk the actual line cols to handle non-strict mode.
  for (size_t i = 0; i < csv_line.size(); ++i) {
    CsvSchema::const_iterator iter = csv_schema_.find(i);
    if (iter != csv_schema_.end()) {
      AddExtendedDataValue(iter->second, csv_line[iter->first], placemark);
    }
  }
  return CSV_PARSER_STATUS_OK;
}

bool CsvParser::ParseCsvData() {
  int line_number = 2;  // Schema line is 1, first data line is 2.
  kmlbase::StringVector cols;
  while (csv_splitter_->SplitCurrentLine(&cols)) {
    kmldom::PlacemarkPtr p = kml_factory_->CreatePlacemark();
    CsvParserStatus status = CsvLineToPlacemark(cols, p);
    if (!csv_parser_handler_->HandleLine(line_number, status, p)) {
      return false;
    }
    ++line_number;
    cols.clear();
  }
  return true;
}

}  // end kmlconvenience
