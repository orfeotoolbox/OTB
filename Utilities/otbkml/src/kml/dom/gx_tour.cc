// Copyright 2009, Google Inc. All rights reserved.
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

// This file contains the implementation of the <gx:Tour> and related elements.

#include "kml/dom/gx_tour.h"
#include "kml/base/attributes.h"
#include "kml/base/xml_namespaces.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/serializer.h"

using kmlbase::Attributes;

namespace kmldom {

// <gx:Tour>

GxTour::GxTour() {
  set_xmlns(kmlbase::XMLNS_GX22);
}

GxTour::~GxTour() {}

void GxTour::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->Type() == Type_GxPlaylist) {
    set_gx_playlist(AsGxPlaylist(element));
  } else {
    Feature::AddElement(element);
  }
}

void GxTour::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  Feature::Serialize(serializer);
  if (has_gx_playlist()) {
    serializer.SaveElement(get_gx_playlist());
  }
}

void GxTour::Accept(Visitor* visitor) {
  visitor->VisitGxTour(GxTourPtr(this));
}

void GxTour::AcceptChildren(VisitorDriver* driver) {
  Feature::AcceptChildren(driver);
  if (has_gx_playlist()) {
    driver->Visit(get_gx_playlist());
  }
}

// <gx:Playlist>

GxPlaylist::GxPlaylist() {
  set_xmlns(kmlbase::XMLNS_GX22);
}

GxPlaylist::~GxPlaylist() {}

void GxPlaylist::add_gx_tourprimitive(
    const GxTourPrimitivePtr& gx_tourprimitive) {
  gx_tourprimitive_array_.push_back(gx_tourprimitive);
}

size_t GxPlaylist::get_gx_tourprimitive_array_size() const {
  return gx_tourprimitive_array_.size();
}

const GxTourPrimitivePtr&
    GxPlaylist::get_gx_tourprimitive_array_at(size_t index) const{
  return gx_tourprimitive_array_[index];
}

void GxPlaylist::AddElement(const ElementPtr& element) {
  if (GxTourPrimitivePtr gx_tourprimitive = AsGxTourPrimitive(element)) {
    add_gx_tourprimitive(gx_tourprimitive);
  } else {
    Element::AddElement(element);
  }
}

void GxPlaylist::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  for (size_t i = 0; i < get_gx_tourprimitive_array_size(); ++i) {
    serializer.SaveElementGroup(get_gx_tourprimitive_array_at(i),
                                Type_GxTourPrimitive);
  }
}

void GxPlaylist::Accept(Visitor* visitor) {
  visitor->VisitGxPlaylist(GxPlaylistPtr(this));
}

void GxPlaylist::AcceptChildren(VisitorDriver* driver) {
  Object::AcceptChildren(driver);
  Element::AcceptRepeated<GxTourPrimitivePtr>(&gx_tourprimitive_array_, driver);
}

// TourPrimitiveCommon

void GxTourPrimitiveCommon::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->Type() == Type_GxDuration) {
    has_gx_duration_ = element->SetDouble(&gx_duration_);
    return;
  }
  GxTourPrimitive::AddElement(element);
}

void GxTourPrimitiveCommon::Serialize(Serializer& serializer) const {
  if (has_gx_duration()) {
    serializer.SaveFieldById(Type_GxDuration, get_gx_duration());
  }
}

// <gx:AnimatedUpdate>

GxAnimatedUpdate::GxAnimatedUpdate() {
  set_xmlns(kmlbase::XMLNS_GX22);
}

GxAnimatedUpdate::~GxAnimatedUpdate() {}

void GxAnimatedUpdate::AddElement(const ElementPtr& element) {
  if (UpdatePtr update = AsUpdate(element)) {
    set_update(update);
    return;
  }
  GxTourPrimitiveCommon::AddElement(element);
}

void GxAnimatedUpdate::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  GxTourPrimitiveCommon::Serialize(serializer);
  if (has_update()) {
    serializer.SaveElement(get_update());
  }
}

void GxAnimatedUpdate::Accept(Visitor* visitor) {
  visitor->VisitGxAnimatedUpdate(GxAnimatedUpdatePtr(this));
}

void GxAnimatedUpdate::AcceptChildren(VisitorDriver* driver) {
  GxTourPrimitiveCommon::AcceptChildren(driver);
  if (has_update()) {
    driver->Visit(get_update());
  }
}

// <gx:FlyTo>

GxFlyTo::GxFlyTo()
  : gx_flytomode_(GX_FLYTOMODE_BOUNCE), has_gx_flytomode_(false) {
  set_xmlns(kmlbase::XMLNS_GX22);
}

GxFlyTo::~GxFlyTo() {}

void GxFlyTo::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->Type() == Type_GxFlyToMode) {
    has_gx_flytomode_ = element->SetEnum(&gx_flytomode_);
    return;
  }
  if (AbstractViewPtr abstractview = AsAbstractView(element)) {
    set_abstractview(abstractview);
    return;
  }
  GxTourPrimitiveCommon::AddElement(element);
}

void GxFlyTo::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  GxTourPrimitiveCommon::Serialize(serializer);
  if (has_gx_flytomode()) {
    serializer.SaveEnum(Type_GxFlyToMode, get_gx_flytomode());
  }
  if (has_abstractview()) {
    serializer.SaveElement(get_abstractview());
  }
}

void GxFlyTo::Accept(Visitor* visitor) {
  visitor->VisitGxFlyTo(GxFlyToPtr(this));
}

void GxFlyTo::AcceptChildren(VisitorDriver* driver) {
  GxTourPrimitiveCommon::AcceptChildren(driver);
  if (has_abstractview()) {
    driver->Visit(get_abstractview());
  }
}

// <gx:Wait>

GxWait::GxWait() {
  set_xmlns(kmlbase::XMLNS_GX22);
}

GxWait::~GxWait() {}

void GxWait::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  GxTourPrimitiveCommon::Serialize(serializer);
}

void GxWait::Accept(Visitor* visitor) {
  visitor->VisitGxWait(GxWaitPtr(this));
}

// <gx:SoundCue>

GxSoundCue::GxSoundCue()
  : has_href_(false) {
  set_xmlns(kmlbase::XMLNS_GX22);
}

GxSoundCue::~GxSoundCue() {}

void GxSoundCue::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->Type() == Type_href) {
    has_href_ = element->SetString(&href_);
    return;
  }
  GxTourPrimitive::AddElement(element);
}

void GxSoundCue::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  GxTourPrimitive::Serialize(serializer);
  if (has_href()) {
    serializer.SaveFieldById(Type_href, get_href());
  }
}

void GxSoundCue::Accept(Visitor* visitor) {
  visitor->VisitGxSoundCue(GxSoundCuePtr(this));
}

// <gx:TourControl>

GxTourControl::GxTourControl()
  : gx_playmode_(GX_PLAYMODE_PAUSE), has_gx_playmode_(false) {
  set_xmlns(kmlbase::XMLNS_GX22);
}

GxTourControl::~GxTourControl() {}

void GxTourControl::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->Type() == Type_GxPlayMode) {
    has_gx_playmode_ = element->SetEnum(&gx_playmode_);
    return;
  }
  GxTourPrimitive::AddElement(element);
}

void GxTourControl::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  GxTourPrimitive::Serialize(serializer);
  if (has_gx_playmode()) {
    serializer.SaveEnum(Type_GxPlayMode, get_gx_playmode());
  }
}

void GxTourControl::Accept(Visitor* visitor) {
  visitor->VisitGxTourControl(GxTourControlPtr(this));
}

}  // end namespace kmldom

