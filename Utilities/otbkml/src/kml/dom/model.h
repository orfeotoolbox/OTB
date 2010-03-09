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

#include <vector>
#include "kml/dom/geometry.h"
#include "kml/dom/kml22.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/link.h"
#include "kml/dom/object.h"

namespace kmldom {

class Visitor;
class VisitorDriver;

// <Location>
class Location : public Object {
 public:
  virtual ~Location();
  virtual KmlDomType Type() const { return Type_Location; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Location || Object::IsA(type);
  }

  // <longitude>
  double get_longitude() const {
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
  double get_latitude() const {
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
  double get_altitude() const {
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

  // Visitor API methods, see visitor.h.
  virtual void Accept(Visitor* visitor);

 private:
  friend class KmlFactory;
  Location();
  friend class KmlHandler;
  virtual void AddElement(const ElementPtr& element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  double longitude_;
  bool has_longitude_;
  double latitude_;
  bool has_latitude_;
  double altitude_;
  bool has_altitude_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(Location);
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
  double get_heading() const {
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
  double get_tilt() const {
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
  double get_roll() const {
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

  // Visitor API methods, see visitor.h.
  virtual void Accept(Visitor* visitor);

 private:
  friend class KmlFactory;
  Orientation();
  friend class KmlHandler;
  virtual void AddElement(const ElementPtr& element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  double heading_;
  bool has_heading_;
  double tilt_;
  bool has_tilt_;
  double roll_;
  bool has_roll_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(Orientation);
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
  double get_x() const {
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
  double get_y() const {
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
  double get_z() const {
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

  // Visitor API methods, see visitor.h.
  virtual void Accept(Visitor* visitor);

 private:
  friend class KmlFactory;
  Scale();
  friend class KmlHandler;
  virtual void AddElement(const ElementPtr& element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  double x_;
  bool has_x_;
  double y_;
  bool has_y_;
  double z_;
  bool has_z_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(Scale);
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
  const string& get_targethref() const {
    return targethref_;
  }
  bool has_targethref() const {
    return has_targethref_;
  }
  void set_targethref(const string& targethref) {
    targethref_ = targethref;
    has_targethref_ = true;
  }
  void clear_targethref() {
    targethref_.clear();
    has_targethref_ = false;
  }

  // <sourceHref>
  const string& get_sourcehref() const {
    return sourcehref_;
  }
  bool has_sourcehref() const {
    return has_sourcehref_;
  }
  void set_sourcehref(const string& sourcehref) {
    sourcehref_ = sourcehref;
    has_sourcehref_ = true;
  }
  void clear_sourcehref() {
    sourcehref_.clear();
    has_sourcehref_ = false;
  }

  // Visitor API methods, see visitor.h.
  virtual void Accept(Visitor* visitor);

 private:
  friend class KmlFactory;
  Alias();
  friend class KmlHandler;
  virtual void AddElement(const ElementPtr& element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  string targethref_;
  bool has_targethref_;
  string sourcehref_;
  bool has_sourcehref_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(Alias);
};

// <ResourceMap>
class ResourceMap : public Object {
 public:
  virtual ~ResourceMap();
  virtual KmlDomType Type() const { return Type_ResourceMap; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_ResourceMap || Object::IsA(type);
  }

  void add_alias(const AliasPtr& alias);

  size_t get_alias_array_size() const {
    return alias_array_.size();
  }

  const AliasPtr& get_alias_array_at(size_t index) const {
    return alias_array_[index];
  }

  // Visitor API methods, see visitor.h.
  virtual void Accept(Visitor* visitor);
  virtual void AcceptChildren(VisitorDriver* driver);

 private:
  friend class KmlFactory;
  ResourceMap();
  friend class KmlHandler;
  virtual void AddElement(const ElementPtr& element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  std::vector<AliasPtr> alias_array_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(ResourceMap);
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
  const LocationPtr& get_location() const { return location_; }
  bool has_location() const { return location_ != NULL; }
  void set_location(const LocationPtr& location) {
    SetComplexChild(location, &location_);
  }
  void clear_location() {
    set_location(NULL);
  }

  // <Orientation>
  const OrientationPtr& get_orientation() const { return orientation_; }
  bool has_orientation() const { return orientation_ != NULL; }
  void set_orientation(const OrientationPtr& orientation) {
    SetComplexChild(orientation, &orientation_);
  }
  void clear_orientation() {
    set_orientation(NULL);
  }

  // <Scale>
  const ScalePtr& get_scale() const { return scale_; }
  bool has_scale() const { return scale_ != NULL; }
  void set_scale(const ScalePtr& scale) {
    SetComplexChild(scale, &scale_);
  }
  void clear_scale() {
    set_scale(NULL);
  }

  // <Link>
  const LinkPtr& get_link() const { return link_; }
  bool has_link() const { return link_ != NULL; }
  void set_link(const LinkPtr& link) {
    SetComplexChild(link, &link_);
  }
  void clear_link() {
    set_link(NULL);
  }

  // <ResourceMap>
  const ResourceMapPtr& get_resourcemap() const { return resourcemap_; }
  bool has_resourcemap() const { return resourcemap_ != NULL; }
  void set_resourcemap(const ResourceMapPtr& resourcemap) {
    SetComplexChild(resourcemap, &resourcemap_);
  }
  void clear_resourcemap() {
    resourcemap_ = NULL;
  }

  // Visitor API methods, see visitor.h.
  virtual void Accept(Visitor* visitor);
  virtual void AcceptChildren(VisitorDriver* driver);

 private:
  friend class KmlFactory;
  Model();
  friend class KmlHandler;
  virtual void AddElement(const ElementPtr& element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  LocationPtr location_;
  OrientationPtr orientation_;
  ScalePtr scale_;
  LinkPtr link_;
  ResourceMapPtr resourcemap_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(Model);
};

}  // end namespace kmldom

#endif  // KML_DOM_MODEL_H__
