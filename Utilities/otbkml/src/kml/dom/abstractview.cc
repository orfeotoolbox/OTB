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

// This file contains the implementation of the LookAt and Camera elements.

#include "kml/dom/abstractview.h"
#include "kml/base/attributes.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/serializer.h"

using kmlbase::Attributes;

namespace kmldom {

// AbstractView
void AbstractView::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->IsA(Type_TimePrimitive)) {
    set_gx_timeprimitive(AsTimePrimitive(element));
    return;
  }
  Object::AddElement(element);
}

void AbstractView::Serialize(Serializer& serializer) const {
  if (has_gx_timeprimitive()) {
    serializer.SaveElementGroup(get_gx_timeprimitive(), Type_TimePrimitive);
  }
}

void AbstractView::AcceptChildren(VisitorDriver* driver) {
  Object::AcceptChildren(driver);
  if (has_gx_timeprimitive()) {
    driver->Visit(get_gx_timeprimitive());
  }
}

// AbstractViewCommon
AbstractViewCommon::AbstractViewCommon()
  : longitude_(0.0),
    has_longitude_(false),
    latitude_(0.0),
    has_latitude_(false),
    altitude_(0.0),
    has_altitude_(false),
    heading_(0.0),
    has_heading_(false),
    tilt_(0.0),
    has_tilt_(false),
    altitudemode_(ALTITUDEMODE_CLAMPTOGROUND),
    has_altitudemode_(false),
    gx_altitudemode_(GX_ALTITUDEMODE_CLAMPTOSEAFLOOR),
    has_gx_altitudemode_(false) {
}

void AbstractViewCommon::AddElement(const ElementPtr& element) {
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
    case Type_heading:
      has_heading_ = element->SetDouble(&heading_);
      break;
    case Type_tilt:
      has_tilt_ = element->SetDouble(&tilt_);
      break;
    case Type_altitudeMode:
      has_altitudemode_ = element->SetEnum(&altitudemode_);
      break;
    case Type_GxAltitudeMode:
      has_gx_altitudemode_ = element->SetEnum(&gx_altitudemode_);
      break;
    default:
      AbstractView::AddElement(element);
      break;
  }
}

void AbstractViewCommon::SerializeBeforeR(Serializer& serializer) const {
  AbstractView::Serialize(serializer);
  if (has_longitude()) {
    serializer.SaveFieldById(Type_longitude, get_longitude());
  }
  if (has_latitude()) {
    serializer.SaveFieldById(Type_latitude, get_latitude());
  }
  if (has_altitude()) {
    serializer.SaveFieldById(Type_altitude, get_altitude());
  }
  if (has_heading()) {
    serializer.SaveFieldById(Type_heading, get_heading());
  }
  if (has_tilt()) {
    serializer.SaveFieldById(Type_tilt, get_tilt());
  }
}

void AbstractViewCommon::SerializeAfterR(Serializer& serializer) const {
  if (has_altitudemode()) {
    serializer.SaveEnum(Type_altitudeMode, get_altitudemode());
  }
  if (has_gx_altitudemode()) {
    serializer.SaveEnum(Type_GxAltitudeMode, get_gx_altitudemode());
  }
}

// <LookAt>
LookAt::LookAt()
 :  range_(0.0),
    has_range_(false) {
}

void LookAt::AddElement(const ElementPtr& element) {
  if (element && element->Type() == Type_range) {
    has_range_ = element->SetDouble(&range_);
  } else {
    AbstractViewCommon::AddElement(element);
  }
}

void LookAt::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  AbstractViewCommon::SerializeBeforeR(serializer);
  if (has_range()) {
    serializer.SaveFieldById(Type_range, get_range());
  }
  AbstractViewCommon::SerializeAfterR(serializer);
}

void LookAt::Accept(Visitor* visitor) {
  visitor->VisitLookAt(LookAtPtr(this));
}

// <Camera>
Camera::Camera()
 :  roll_(0.0),
    has_roll_(false) {
}

void Camera::AddElement(const ElementPtr& element) {
  if (element && element->Type() == Type_roll) {
    has_roll_ = element->SetDouble(&roll_);
  } else {
    AbstractViewCommon::AddElement(element);
  }
}

void Camera::Serialize(Serializer& serializer) const {
  ElementSerializer element_serializer(*this, serializer);
  AbstractViewCommon::SerializeBeforeR(serializer);
  if (has_roll()) {
    serializer.SaveFieldById(Type_roll, get_roll());
  }
  AbstractViewCommon::SerializeAfterR(serializer);
}

void Camera::Accept(Visitor* visitor) {
  visitor->VisitCamera(CameraPtr(this));
}

}  // end namespace kmldom
