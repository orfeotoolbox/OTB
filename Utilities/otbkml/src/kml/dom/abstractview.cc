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
#include "kml/dom/attributes.h"
#include "kml/dom/serializer.h"

namespace kmldom {

LookAt::LookAt()
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
    range_(0.0),
    has_range_(false),
    altitudemode_(ALTITUDEMODE_CLAMPTOGROUND),
    has_altitudemode_(false) {
}

LookAt::~LookAt() {}

void LookAt::AddElement(const ElementPtr& element) {
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
    case Type_range:
      has_range_ = element->SetDouble(&range_);
      break;
    case Type_altitudeMode:
      has_altitudemode_ = element->SetEnum(&altitudemode_);
      break;
    default:
      AbstractView::AddElement(element);
      break;
  }
}

void LookAt::Serialize(Serializer& serializer) const {
  Attributes attributes;
  AbstractView::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
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
  if (has_range()) {
    serializer.SaveFieldById(Type_range, get_range());
  }
  if (has_altitudemode()) {
    serializer.SaveEnum(Type_altitudeMode, get_altitudemode());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

Camera::Camera()
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
    roll_(0.0),
    has_roll_(false),
    altitudemode_(ALTITUDEMODE_CLAMPTOGROUND),
    has_altitudemode_(false) {
}

Camera::~Camera() {}

void Camera::AddElement(const ElementPtr& element) {
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
    case Type_roll:
      has_roll_ = element->SetDouble(&roll_);
      break;
    case Type_altitudeMode:
      has_altitudemode_ = element->SetEnum(&altitudemode_);
      break;
    default:
      AbstractView::AddElement(element);
      break;
  }
}

void Camera::Serialize(Serializer& serializer) const {
  Attributes attributes;
  AbstractView::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
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
  if (has_roll()) {
    serializer.SaveFieldById(Type_roll, get_roll());
  }
  if (has_altitudemode()) {
    serializer.SaveEnum(Type_altitudeMode, get_altitudemode());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

}  // end namespace kmldom
