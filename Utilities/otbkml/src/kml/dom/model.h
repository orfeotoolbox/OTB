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

// This file declares the Model, Location, Orientation, Scale, ResourceMap
// and Alias elements.

#ifndef KML_DOM_MODEL_H__
#define KML_DOM_MODEL_H__

#include <string>
#include <vector>
#include "kml/dom/geometry.h"
#include "kml/dom/kml22.h"
#include "kml/dom/link.h"
#include "kml/dom/object.h"

namespace kmldom {

// <Location>
class Location : public Object {
 public:
  virtual ~Location();
  virtual KmlDomType Type() const { return Type_Location; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Location || Object::IsA(type);
  }

  // <longitude>
  double longitude() const {
    return longitude_;
  }
  bool has_longitude() const {
    return has_longitude_;
  }
  void set_longitude(double longitude) {
    longitude_ = longitude;
    has_longitude_ = true;
  }
  void clear_longitude() {
    longitude_ = 0.0;
    has_longitude_ = false;
  }

  // <latitude>
  double latitude() const {
    return latitude_;
  }
  bool has_latitude() const {
    return has_latitude_;
  }
  void set_latitude(double latitude) {
    latitude_ = latitude;
    has_latitude_ = true;
  }
  void clear_latitude() {
    latitude_ = 0.0;
    has_latitude_ = false;
  }

  // <altitude>
  double altitude() const {
    return altitude_;
  }
  bool has_altitude() const {
    return has_altitude_;
  }
  void set_altitude(double altitude) {
    altitude_ = altitude;
    has_altitude_ = true;
  }
  void clear_altitude() {
    altitude_ = 0.0;
    has_altitude_ = false;
  }

 private:
  friend class KmlFactory;
  Location();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  double longitude_;
  bool has_longitude_;
  double latitude_;
  bool has_latitude_;
  double altitude_;
  bool has_altitude_;
  DISALLOW_EVIL_CONSTRUCTORS(Location);
};

// <Orientation>
class Orientation : public Object {
 public:
  virtual ~Orientation();
  virtual KmlDomType Type() const { return Type_Orientation; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Orientation || Object::IsA(type);
  }

  // <heading>
  double heading() const {
    return heading_;
  }
  bool has_heading() const {
    return has_heading_;
  }
  void set_heading(double heading) {
    heading_ = heading;
    has_heading_ = true;
  }
  void clear_heading() {
    heading_ = 0.0;
    has_heading_ = false;
  }

  // <tilt>
  double tilt() const {
    return tilt_;
  }
  bool has_tilt() const {
    return has_tilt_;
  }
  void set_tilt(double tilt) {
    tilt_ = tilt;
    has_tilt_ = true;
  }
  void clear_tilt() {
    tilt_ = 0.0;
    has_tilt_ = false;
  }

  // <roll>
  double roll() const {
    return roll_;
  }
  bool has_roll() const {
    return has_roll_;
  }
  void set_roll(double roll) {
    roll_ = roll;
    has_roll_ = true;
  }
  void clear_roll() {
    roll_ = 0.0;
    has_roll_ = false;
  }

 private:
  friend class KmlFactory;
  Orientation();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  double heading_;
  bool has_heading_;
  double tilt_;
  bool has_tilt_;
  double roll_;
  bool has_roll_;
  DISALLOW_EVIL_CONSTRUCTORS(Orientation);
};

// <Scale>
class Scale : public Object {
 public:
  virtual ~Scale();
  virtual KmlDomType Type() const { return Type_Scale; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Scale || Object::IsA(type);
  }

  // <x>
  double x() const {
    return x_;
  }
  bool has_x() const {
    return has_x_;
  }
  void set_x(double x) {
    x_ = x;
    has_x_ = true;
  }
  void clear_x() {
    x_ = 1.0;
    has_x_ = false;
  }

  // <y>
  double y() const {
    return y_;
  }
  bool has_y() const {
    return has_y_;
  }
  void set_y(double y) {
    y_ = y;
    has_y_ = true;
  }
  void clear_y() {
    y_ = 1.0;
    has_y_ = false;
  }

  // <z>
  double z() const {
    return z_;
  }
  bool has_z() const {
    return has_z_;
  }
  void set_z(double z) {
    z_ = z;
    has_z_ = true;
  }
  void clear_z() {
    z_ = 1.0;
    has_z_ = false;
  }

 private:
  friend class KmlFactory;
  Scale();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  double x_;
  bool has_x_;
  double y_;
  bool has_y_;
  double z_;
  bool has_z_;
  DISALLOW_EVIL_CONSTRUCTORS(Scale);
};

// <Alias>
class Alias : public Object {
 public:
  virtual ~Alias();
  virtual KmlDomType Type() const { return Type_Alias; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Alias || Object::IsA(type);
  }

  // <targetHref>
  const std::string& targethref() const {
    return targethref_;
  }
  bool has_targethref() const {
    return has_targethref_;
  }
  void set_targethref(const std::string& targethref) {
    targethref_ = targethref;
    has_targethref_ = true;
  }
  void clear_targethref() {
    targethref_.clear();
    has_targethref_ = false;
  }

  // <sourceHref>
  const std::string& sourcehref() const {
    return sourcehref_;
  }
  bool has_sourcehref() const {
    return has_sourcehref_;
  }
  void set_sourcehref(const std::string& sourcehref) {
    sourcehref_ = sourcehref;
    has_sourcehref_ = true;
  }
  void clear_sourcehref() {
    sourcehref_.clear();
    has_sourcehref_ = false;
  }

 private:
  friend class KmlFactory;
  Alias();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  std::string targethref_;
  bool has_targethref_;
  std::string sourcehref_;
  bool has_sourcehref_;
  DISALLOW_EVIL_CONSTRUCTORS(Alias);
};

// <ResourceMap>
class ResourceMap : public Object {
 public:
  virtual ~ResourceMap();
  virtual KmlDomType Type() const { return Type_ResourceMap; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_ResourceMap || Object::IsA(type);
  }

  void add_alias(Alias* alias);

  const size_t alias_array_size() const {
    return alias_array_.size();
  }

  const Alias* alias_array_at(unsigned int index) const {
    return alias_array_[index];
  }

 private:
  friend class KmlFactory;
  ResourceMap();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  std::vector<Alias*> alias_array_;
  DISALLOW_EVIL_CONSTRUCTORS(ResourceMap);
};

// <Model>
class Model : public AltitudeGeometryCommon {
 public:
  virtual ~Model();
  virtual KmlDomType Type() const { return Type_Model; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Model || Geometry::IsA(type);
  }

  // <Location>
  const Location* location() const { return location_; }
  bool has_location() const { return location_ != NULL; }
  void set_location(Location* location) {
    if (location == NULL) {
      clear_location();
    } else if (location->set_parent(this)) {
      delete location_;
      location_ = location;
    }
  }
  void clear_location() {
    delete location_;
    location_ = NULL;
  }

  // <Orientation>
  const Orientation* orientation() const { return orientation_; }
  bool has_orientation() const { return orientation_ != NULL; }
  void set_orientation(Orientation* orientation) {
    if (orientation == NULL) {
      clear_orientation();
    } else if (orientation->set_parent(this)) {
      delete orientation_;
      orientation_ = orientation;
    }
  }
  void clear_orientation() {
    delete orientation_;
    orientation_ = NULL;
  }

  // <Scale>
  const Scale* scale() const { return scale_; }
  bool has_scale() const { return scale_ != NULL; }
  void set_scale(Scale* scale) {
    if (scale == NULL) {
      clear_scale();
    } else if (scale->set_parent(this)) {
      delete scale_;
      scale_ = scale;
    }
  }
  void clear_scale() {
    delete scale_;
    scale_ = NULL;
  }

  // <Link>
  const Link* link() const { return link_; }
  bool has_link() const { return link_ != NULL; }
  void set_link(Link* link) {
    if (link == NULL) {
      clear_link();
    } else if (link->set_parent(this)) {
      delete link_;
      link_ = link;
    }
  }
  void clear_link() {
    delete link_;
    link_ = NULL;
  }

  // <ResourceMap>
  const ResourceMap* resourcemap() const { return resourcemap_; }
  bool has_resourcemap() const { return resourcemap_ != NULL; }
  void set_resourcemap(ResourceMap* resourcemap) {
    if (resourcemap == NULL) {
      clear_resourcemap();
    } else if (resourcemap->set_parent(this)) {
      delete resourcemap_;
      resourcemap_ = resourcemap;
    }
  }
  void clear_resourcemap() {
    delete resourcemap_;
    resourcemap_ = NULL;
  }

 private:
  friend class KmlFactory;
  Model();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  Location* location_;
  Orientation* orientation_;
  Scale* scale_;
  Link* link_;
  ResourceMap* resourcemap_;
  DISALLOW_EVIL_CONSTRUCTORS(Model);
};

}  // end namespace kmldom

#endif  // KML_DOM_MODEL_H__
