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
#include "kml/base/attributes.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/link.h"
#include "kml/dom/serializer.h"
#include "kml/dom/xsd.h"

using kmlbase::Attributes;

namespace kmldom {

Location::Location()
  : longitude_(0.0), has_longitude_(false),
    latitude_(0.0), has_latitude_(false),
    altitude_(0.0), has_altitude_(false)
{}

Location::~Location() {}

void Location::AddElement(const ElementPtr& element) {
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
  ElementSerializer element_serializer(*this, serializer);
  Object::Serialize(serializer);
  if (has_longitude()) {
    serializer.SaveFieldById(Type_longitude, get_longitude());
  }
  if (has_latitude()) {
    serializer.SaveFieldById(Type_latitude, get_latitude());
  }
  if (has_altitude()) {
    serializer.SaveFieldById(Type_altitude, get_altitude());
  }
}

void Location::Accept(Visitor* visitor) {
  visitor->VisitLocation(LocationPtr(this));
}

Orientation::Orientation()
  : heading_(0.0), has_heading_(false),
    tilt_(0.0), has_tilt_(false),
    roll_(0.0), has_roll_(false) {
}

Orientation::~Orientation() {}

void Orientation::AddElement(const ElementPtr& element) {
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
  ElementSerializer element_serializer(*this, serializer);
  Object::Serialize(serializer);
  if (has_heading()) {
    serializer.SaveFieldById(Type_heading, get_heading());
  }
  if (has_tilt()) {
    serializer.SaveFieldById(Type_tilt, get_tilt());
  }
  if (has_roll()) {
    serializer.SaveFieldById(Type_roll, get_roll());
  }
}

void Orientation::Accept(Visitor* visitor) {
  visitor->VisitOrientation(OrientationPtr(this));
}

Scale::Scale()
  : x_(1.0), has_x_(false),
    y_(1.0), has_y_(false),
    z_(1.0), has_z_(false) {
}

Scale::~Scale() {}

void Scale::AddElement(const ElementPtr& element) {
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
  ElementSerializer element_serializer(*this, serializer);
  Object::Serialize(serializer);
  if (has_x()) {
    serializer.SaveFieldById(Type_x, get_x());
  }
  if (has_y()) {
    serializer.SaveFieldById(Type_y, get_y());
  }
  if (has_z()) {
    serializer.SaveFieldById(Type_z, get_z());
  }
}

void Scale::Accept(Visitor* visitor) {
  visitor->VisitScale(ScalePtr(this));
}

Alias::Alias()
  : has_targethref_(false), has_sourcehref_(false)
{}

Alias::~Alias() {}

void Alias::AddElement(const ElementPtr& element) {
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
  ElementSerializer element_serializer(*this, serializer);
  Object::Serialize(serializer);
  if (has_targethref()) {
    serializer.SaveFieldById(Type_targetHref, get_targethref());
  }
  if (has_sourcehref()) {
    serializer.SaveFieldById(Type_sourceHref, get_sourcehref());
  }
}

void Alias::Accept(Visitor* visitor) {
  visitor->VisitAlias(AliasPtr(this));
}

ResourceMap::ResourceMap() {}

ResourceMap::~ResourceMap() {}

void ResourceMap::add_alias(const AliasPtr& alias) {
  AddComplexChild(alias, &alias_array_);
}

void ResourceMap::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->Type() == Type_Alias) {
    add_alias(AsAlias(element));
  } else {
    Object::AddElement(element);
  }
}

void ResourceMap::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  Object::Serialize(serializer);
  serializer.SaveElementArray(alias_array_);
}

void ResourceMap::Accept(Visitor* visitor) {
  visitor->VisitResourceMap(ResourceMapPtr(this));
}

void ResourceMap::AcceptChildren(VisitorDriver* driver) {
  Object::AcceptChildren(driver);
  Element::AcceptRepeated<AliasPtr>(&alias_array_, driver);
}

Model::Model() {}

Model::~Model() {}

void Model::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  switch (element->Type()) {
    case Type_Location:
      set_location(AsLocation(element));
      break;
    case Type_Orientation:
      set_orientation(AsOrientation(element));
      break;
    case Type_Scale:
      set_scale(AsScale(element));
      break;
    case Type_Link:
      set_link(AsLink(element));
      break;
    case Type_ResourceMap:
      set_resourcemap(AsResourceMap(element));
      break;
    default:
      AltitudeGeometryCommon::AddElement(element);
  }
}

void Model::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  Geometry::Serialize(serializer);
  if (has_altitudemode()) {
    serializer.SaveEnum(Type_altitudeMode, get_altitudemode());
  }
  if (has_gx_altitudemode()) {
    serializer.SaveEnum(Type_GxAltitudeMode, get_gx_altitudemode());
  }
  if (has_location()) {
    serializer.SaveElement(get_location());
  }
  if (has_orientation()) {
    serializer.SaveElement(get_orientation());
  }
  if (has_scale()) {
    serializer.SaveElement(get_scale());
  }
  if (has_link()) {
    serializer.SaveElement(get_link());
  }
  if (has_resourcemap()) {
    serializer.SaveElement(get_resourcemap());
  }
}

void Model::Accept(Visitor* visitor) {
  visitor->VisitModel(ModelPtr(this));
}

void Model::AcceptChildren(VisitorDriver* driver) {
  AltitudeGeometryCommon::AcceptChildren(driver);
  if (has_location()) {
    driver->Visit(get_location());
  }
  if (has_orientation()) {
    driver->Visit(get_orientation());
  }
  if (has_scale()) {
    driver->Visit(get_scale());
  }
  if (has_link()) {
    driver->Visit(get_link());
  }
  if (has_resourcemap()) {
    driver->Visit(get_resourcemap());
  }
}

}  // end namespace kmldom
