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

// This file declares the Region, LatLonAltBox and Lod elements.
// LatLonAltBox and Lod are here because they are used only with Region.

#ifndef KML_DOM_REGION_H__
#define KML_DOM_REGION_H__

#include "kml/dom/abstractlatlonbox.h"
#include "kml/dom/kml22.h"
#include "kml/dom/object.h"
#include "kml/util/util.h"

namespace kmldom {

// <LatLonAltBox>
class LatLonAltBox : public AbstractLatLonBox {
 public:
  virtual ~LatLonAltBox();
  virtual KmlDomType Type() const { return Type_LatLonAltBox; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_LatLonAltBox || AbstractLatLonBox::IsA(type);
  }

  // <minAltitude>
  double minaltitude() const {
    return minaltitude_;
  }
  bool has_minaltitude() const {
    return has_minaltitude_;
  }
  void set_minaltitude(double minaltitude) {
    minaltitude_ = minaltitude;
    has_minaltitude_ = true;
  }
  void clear_minaltitude() {
    minaltitude_ = 0.0;
    has_minaltitude_ = false;
  }

  // <maxAltitude>
  double maxaltitude() const {
    return maxaltitude_;
  }
  bool has_maxaltitude() const {
    return has_maxaltitude_;
  }
  void set_maxaltitude(double maxaltitude) {
    maxaltitude_ = maxaltitude;
    has_maxaltitude_ = true;
  }
  void clear_maxaltitude() {
    maxaltitude_ = 0.0;
    has_maxaltitude_ = false;
  }

  // <altitudeMode>
  int altitudemode() const {
    return altitudemode_;
  }
  bool has_altitudemode() const {
    return has_altitudemode_;
  }
  void set_altitudemode(int altitudemode) {
    altitudemode_ = altitudemode;
    has_altitudemode_ = true;
  }
  void clear_altitudemode() {
    altitudemode_ = ALTITUDEMODE_CLAMPTOGROUND;
    has_altitudemode_ = false;
  }

 private:
  friend class KmlFactory;
  LatLonAltBox();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  double minaltitude_;
  bool has_minaltitude_;
  double maxaltitude_;
  bool has_maxaltitude_;
  int altitudemode_;
  bool has_altitudemode_;
  DISALLOW_EVIL_CONSTRUCTORS(LatLonAltBox);
};

// <Lod>
class Lod : public Object {
 public:
  virtual ~Lod();
  virtual KmlDomType Type() const { return Type_Lod; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Lod || Object::IsA(type);
  }

  // <minLodPixels>
  double minlodpixels() const {
    return minlodpixels_;
  }
  bool has_minlodpixels() const {
    return has_minlodpixels_;
  }
  void set_minlodpixels(double minlodpixels) {
    minlodpixels_ = minlodpixels;
    has_minlodpixels_ = true;
  }
  void clear_minlodpixels() {
    minlodpixels_ = 0.0;
    has_minlodpixels_ = false;
  }

  // <maxLodPixels>
  double maxlodpixels() const {
    return maxlodpixels_;
  }
  bool has_maxlodpixels() const {
    return has_maxlodpixels_;
  }
  void set_maxlodpixels(double minlodpixels) {
    maxlodpixels_ = minlodpixels;
    has_maxlodpixels_ = true;
  }
  void clear_maxlodpixels() {
    maxlodpixels_ = -1.0;
    has_maxlodpixels_ = false;
  }

  // <minFadeExtent>
  double minfadeextent() const {
    return minfadeextent_;
  }
  bool has_minfadeextent() const {
    return has_minfadeextent_;
  }
  void set_minfadeextent(double minlodpixels) {
    minfadeextent_ = minlodpixels;
    has_minfadeextent_ = true;
  }
  void clear_minfadeextent() {
    minfadeextent_ = 0.0;
    has_minfadeextent_ = false;
  }

  // <maxFadeExtent>
  double maxfadeextent() const {
    return maxfadeextent_;
  }
  bool has_maxfadeextent() const {
    return has_maxfadeextent_;
  }
  void set_maxfadeextent(double maxlodpixels) {
    maxfadeextent_ = maxlodpixels;
    has_maxfadeextent_ = true;
  }
  void clear_maxfadeextent() {
    maxfadeextent_ = 0.0;
    has_maxfadeextent_ = false;
  }

 private:
  friend class KmlFactory;
  Lod();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  double minlodpixels_;
  bool has_minlodpixels_;
  double maxlodpixels_;
  bool has_maxlodpixels_;
  double minfadeextent_;
  bool has_minfadeextent_;
  double maxfadeextent_;
  bool has_maxfadeextent_;
  DISALLOW_EVIL_CONSTRUCTORS(Lod);
};

// <Region>
class Region : public Object {
 public:
  virtual ~Region();
  virtual KmlDomType Type() const { return Type_Region; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Region || Object::IsA(type);
  }

  // <LatLonAltBox>
  const LatLonAltBox* latlonaltbox() const { return latlonaltbox_; }
  bool has_latlonaltbox() const { return latlonaltbox_ != NULL; }
  void set_latlonaltbox(LatLonAltBox* latlonaltbox) {
    if (latlonaltbox == NULL) {
      clear_latlonaltbox();
    } else if (latlonaltbox->set_parent(this)) {
      delete latlonaltbox_;
      latlonaltbox_ = latlonaltbox;
    }
  }
  void clear_latlonaltbox() {
    delete latlonaltbox_;
    latlonaltbox_ = NULL;
  }

  // <Lod>
  const Lod* lod() const { return lod_; }
  bool has_lod() const { return lod_ != NULL; }
  void set_lod(Lod* lod) {
    if (lod == NULL) {
      clear_lod();
    } else if (lod->set_parent(this)) {
      delete lod_;
      lod_ = lod;
    }
  }
  void clear_lod() {
    delete lod_;
    lod_ = NULL;
  }

 private:
  friend class KmlFactory;
  Region();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  LatLonAltBox* latlonaltbox_;
  Lod* lod_;
  DISALLOW_EVIL_CONSTRUCTORS(Region);
};

}  // end namespace kmldom

#endif  // KML_DOM_REGION_H__
