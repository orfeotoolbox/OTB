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

#include "kml/dom/abstractlatlonbox.h"
#include "kml/dom/serializer.h"

namespace kmldom {

// Defaults http://schemas.opengis.net/kml/2.2.0/ogckml22.xsd for
// <north>, <south>, <east>, and <west>.
AbstractLatLonBox::AbstractLatLonBox()
  : north_(180.0), has_north_(false), south_(-180.0), has_south_(false),
    east_(180.0), has_east_(false), west_(-180.0), has_west_(false)
{}

AbstractLatLonBox::~AbstractLatLonBox() {}

void AbstractLatLonBox::AddElement(const ElementPtr& element) {
  switch (element->Type()) {
    case Type_north:
      has_north_ = element->SetDouble(&north_);
      break;
    case Type_south:
      has_south_ = element->SetDouble(&south_);
      break;
    case Type_east:
      has_east_ = element->SetDouble(&east_);
      break;
    case Type_west:
      has_west_ = element->SetDouble(&west_);
      break;
    default:
      Object::AddElement(element);
      break;
  }
}

void AbstractLatLonBox::Serialize(Serializer& serializer) const {
  Object::Serialize(serializer);
  if (has_north()) {
    serializer.SaveFieldById(Type_north, get_north());
  }
  if (has_south()) {
    serializer.SaveFieldById(Type_south, get_south());
  }
  if (has_east()) {
    serializer.SaveFieldById(Type_east, get_east());
  }
  if (has_west()) {
    serializer.SaveFieldById(Type_west, get_west());
  }
}

}  // end namespace kmldom
