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

// This file contains the implementation of the Location, Orientation,
// Scale, ResourceMap, Alias and Model elements.

#include "kml/dom/model.h"
#include "kml/dom/attributes.h"
#include "kml/dom/kml22.h"
#include "kml/dom/link.h"
#include "kml/dom/serializer.h"
#include "kml/dom/xsd.h"

namespace kmldom {

Location::Location()
  : longitude_(0.0), has_longitude_(false),
    latitude_(0.0), has_latitude_(false),
    altitude_(0.0), has_altitude_(false)
{}

Location::~Location() {}

void Location::AddElement(Element* element) {
  if (!element) {
    return;
  }
  switch (element->Type()) {
    case Type_longitude:
      has_longitude_ = element->SetDouble(&longitude_);
      break;
    case Type_latitude:
      has_latitude_ = element->SetDouble(&latitude_);
      break;
    case Type_altitude:
      has_altitude_ = element->SetDouble(&altitude_);
      break;
    default:
      Object::AddElement(element);
      break;
  }
}

void Location::Serialize(Serializer& serializer) const {
  Attributes attributes;
  Object::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  Object::Serialize(serializer);
  if (has_longitude()) {
    serializer.SaveFieldById(Type_longitude, longitude());
  }
  if (has_latitude()) {
    serializer.SaveFieldById(Type_latitude, latitude());
  }
  if (has_altitude()) {
    serializer.SaveFieldById(Type_altitude, altitude());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

Orientation::Orientation()
  : heading_(0.0), has_heading_(false),
    tilt_(0.0), has_tilt_(false),
    roll_(0.0), has_roll_(false) {
}

Orientation::~Orientation() {}

void Orientation::AddElement(Element* element) {
  if (!element) {
    return;
  }
  switch (element->Type()) {
    case Type_heading:
      has_heading_ = element->SetDouble(&heading_);
      break;
    case Type_tilt:
      has_tilt_ = element->SetDouble(&tilt_);
      break;
    case Type_roll:
      has_roll_ = element->SetDouble(&roll_);
      break;
    default:
      Object::AddElement(element);
      break;
  }
}

void Orientation::Serialize(Serializer& serializer) const {
  Attributes attributes;
  Object::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  Object::Serialize(serializer);
  if (has_heading()) {
    serializer.SaveFieldById(Type_heading, heading());
  }
  if (has_tilt()) {
    serializer.SaveFieldById(Type_tilt, tilt());
  }
  if (has_roll()) {
    serializer.SaveFieldById(Type_roll, roll());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

Scale::Scale()
  : x_(1.0), has_x_(false),
    y_(1.0), has_y_(false),
    z_(1.0), has_z_(false) {
}

Scale::~Scale() {}

void Scale::AddElement(Element* element) {
  if (!element) {
    return;
  }
  switch (element->Type()) {
    case Type_x:
      has_x_ = element->SetDouble(&x_);
      break;
    case Type_y:
      has_y_ = element->SetDouble(&y_);
      has_y_ = true;
      break;
    case Type_z:
      has_z_ = element->SetDouble(&z_);
      break;
    default:
      Object::AddElement(element);
      break;
  }
}

void Scale::Serialize(Serializer& serializer) const {
  Attributes attributes;
  Object::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  Object::Serialize(serializer);
  if (has_x()) {
    serializer.SaveFieldById(Type_x, x());
  }
  if (has_y()) {
    serializer.SaveFieldById(Type_y, y());
  }
  if (has_z()) {
    serializer.SaveFieldById(Type_z, z());
  }
  Element::SerializeUnknown(serializer);
  serializer.End();
}

Alias::Alias()
  : has_targethref_(false), has_sourcehref_(false)
{}

Alias::~Alias() {}

void Alias::AddElement(Element* element) {
  if (!element) {
    return;
  }
  switch (element->Type()) {
    case Type_targetHref:
      has_targethref_ = element->SetString(&targethref_);
      break;
    case Type_sourceHref:
      has_sourcehref_ = element->SetString(&sourcehref_);
      break;
    default:
      Object::AddElement(element);
  }
}

void Alias::Serialize(Serializer& serializer) const {
  Attributes attributes;
  Object::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  Object::Serialize(serializer);
  if (has_targethref()) {
    serializer.SaveFieldById(Type_targetHref, targethref());
  }
  if (has_sourcehref()) {
    serializer.SaveFieldById(Type_sourceHref, sourcehref());
  }
  Element::SerializeUnknown(serializer);
  serializer.End();
}

ResourceMap::ResourceMap() {}

ResourceMap::~ResourceMap() {
  for (size_t i = 0; i < alias_array_.size(); ++i) {
    delete alias_array_[i];
  }
}

void ResourceMap::add_alias(Alias* alias) {
  if (alias && alias->IsA(Type_Alias) && alias->set_parent(this)) {
    alias_array_.push_back(alias);
  }
}

void ResourceMap::AddElement(Element* element) {
  if (!element) {
    return;
  }
  if (element->Type() == Type_Alias) {
    add_alias(static_cast<Alias*>(element));
  } else {
    Object::AddElement(element);
  }
}

void ResourceMap::Serialize(Serializer& serializer) const {
  Attributes attributes;
  Object::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  Object::Serialize(serializer);
  for (size_t i = 0; i < alias_array_.size(); ++i) {
    serializer.SaveElement(*alias_array_at(i));
  }
  Element::SerializeUnknown(serializer);
  serializer.End();
}

Model::Model()
  : location_(NULL), orientation_(NULL), scale_(NULL), link_(NULL),
    resourcemap_(NULL) {
}

Model::~Model() {
  delete location_;
  delete orientation_;
  delete scale_;
  delete link_;
  delete resourcemap_;
}

void Model::AddElement(Element* element) {
  if (!element) {
    return;
  }
  switch (element->Type()) {
    case Type_Location:
      set_location(static_cast<Location*>(element));
      break;
    case Type_Orientation:
      set_orientation(static_cast<Orientation*>(element));
      break;
    case Type_Scale:
      set_scale(static_cast<Scale*>(element));
      break;
    case Type_Link:
      set_link(static_cast<Link*>(element));
      break;
    case Type_ResourceMap:
      set_resourcemap(static_cast<ResourceMap*>(element));
      break;
    default:
      AltitudeGeometryCommon::AddElement(element);
  }
}

void Model::Serialize(Serializer& serializer) const {
  Attributes attributes;
  Geometry::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  Geometry::Serialize(serializer);
  if (has_altitudemode()) {
    serializer.SaveEnum(Type_altitudeMode, altitudemode());
  }
  if (has_location()) {
    serializer.SaveElement(*location());
  }
  if (has_orientation()) {
    serializer.SaveElement(*orientation());
  }
  if (has_scale()) {
    serializer.SaveElement(*scale());
  }
  if (has_link()) {
    serializer.SaveElement(*link());
  }
  if (has_resourcemap()) {
    serializer.SaveElement(*resourcemap());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

}  // end namespace kmldom
