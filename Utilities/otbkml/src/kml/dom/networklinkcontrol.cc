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

// This file contains the implementation of NetworkLinkControl and its
// children.

#include "kml/dom/networklinkcontrol.h"
#include "kml/dom/attributes.h"
#include "kml/dom/abstractview.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/serializer.h"

namespace kmldom {

// UpdateOperation
UpdateOperation::UpdateOperation() {}

UpdateOperation::~UpdateOperation() {}

// <Create>
Create::Create() {}

Create::~Create() {}

void Create::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->IsA(Type_Container)) {
    add_container(AsContainer(element));
  } else {
    Element::AddElement(element);
  }
}

void Create::Serialize(Serializer& serializer) const {
  Attributes attributes;
  serializer.BeginById(Type(), attributes);
  for (size_t i = 0; i < container_array_.size(); ++i) {
    serializer.SaveElement(get_container_array_at(i));
  }
  SerializeUnknown(serializer);
  serializer.End();
}

// <Delete>
Delete::Delete() {}

Delete::~Delete() {}

void Delete::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->IsA(Type_Feature)) {
    add_feature(AsFeature(element));
  } else {
    Element::AddElement(element);
  }
}

void Delete::Serialize(Serializer& serializer) const {
  Attributes attributes;
  serializer.BeginById(Type(), attributes);
  for (size_t i = 0; i < feature_array_.size(); ++i) {
    serializer.SaveElement(get_feature_array_at(i));
  }
  SerializeUnknown(serializer);
  serializer.End();
}

// <Change>
Change::Change() {}

Change::~Change() {}

void Change::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->IsA(Type_Object)) {
    add_object(AsObject(element));
  } else {
    Element::AddElement(element);
  }
}

void Change::Serialize(Serializer& serializer) const {
  Attributes attributes;
  serializer.BeginById(Type(), attributes);
  for (size_t i = 0; i < object_array_.size(); ++i) {
    serializer.SaveElement(get_object_array_at(i));
  }
  SerializeUnknown(serializer);
  serializer.End();
}

// <Update>
Update::Update()
  : has_targethref_(false) {
}

Update::~Update() {}

void Update::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  switch (element->Type()) {
    case Type_targetHref:
      has_targethref_ = element->SetString(&targethref_);
      break;
    case Type_Create:
      add_updateoperation(AsCreate(element));
      break;
    case Type_Delete:
      add_updateoperation(AsDelete(element));
      break;
    case Type_Change:
      add_updateoperation(AsChange(element));
      break;
    default:
      Element::AddElement(element);
      break;
  }
}

void Update::Serialize(Serializer& serializer) const {
  Attributes attributes;
  serializer.BeginById(Type(), attributes);
  if (has_targethref()) {
    serializer.SaveFieldById(Type_targetHref, get_targethref());
  }
  for (size_t i = 0; i < updateoperation_array_.size(); ++i) {
    serializer.SaveElement(get_updateoperation_array_at(i));
  }
  SerializeUnknown(serializer);
  serializer.End();
}

// <NetworkLinkControl>
NetworkLinkControl::NetworkLinkControl()
  : minrefreshperiod_(0.0),
    has_minrefreshperiod_(false),
    maxsessionlength_(0.0),
    has_maxsessionlength_(false),
    has_cookie_(false),
    has_message_(false),
    has_linkname_(false),
    has_linkdescription_(false),
    linksnippet_(NULL),
    has_expires_(false),
    update_(NULL),
    abstractview_(NULL) {
}

NetworkLinkControl::~NetworkLinkControl() {}

void NetworkLinkControl::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  if (element->IsA(Type_AbstractView)) {
    set_abstractview(AsAbstractView(element));
    return;
  }
  switch (element->Type()) {
    case Type_minRefreshPeriod:
      has_minrefreshperiod_ = element->SetDouble(&minrefreshperiod_);
      break;
    case Type_maxSessionLength:
      has_maxsessionlength_ = element->SetDouble(&maxsessionlength_);
      break;
    case Type_cookie:
      has_cookie_ = element->SetString(&cookie_);
      break;
    case Type_message:
      has_message_ = element->SetString(&message_);
      break;
    case Type_linkName:
      has_linkname_ = element->SetString(&linkname_);
      break;
    case Type_linkDescription:
      has_linkdescription_ = element->SetString(&linkdescription_);
      break;
    case Type_linkSnippet:
      set_linksnippet(AsLinkSnippet(element));
      break;
    case Type_expires:
      has_expires_ = element->SetString(&expires_);
      break;
    case Type_Update:
      set_update(AsUpdate(element));
      break;
    default:
      Element::AddElement(element);
      break;
  }
}

void NetworkLinkControl::Serialize(Serializer& serializer) const {
  Attributes attributes;
  Element::GetAttributes(&attributes);
  serializer.BeginById(Type(), attributes);
  if (has_minrefreshperiod_) {
    serializer.SaveFieldById(Type_minRefreshPeriod, minrefreshperiod_);
  }
  if (has_maxsessionlength_) {
    serializer.SaveFieldById(Type_maxSessionLength, maxsessionlength_);
  }
  if (has_cookie_) {
    serializer.SaveFieldById(Type_cookie, cookie_);
  }
  if (has_message_) {
    serializer.SaveFieldById(Type_message, message_);
  }
  if (has_linkname_) {
    serializer.SaveFieldById(Type_linkName, linkname_);
  }
  if (has_linkdescription_) {
    serializer.SaveFieldById(Type_linkDescription, linkdescription_);
  }
  if (linksnippet_) {
    serializer.SaveElement(get_linksnippet());
  }
  if (has_expires_) {
    serializer.SaveFieldById(Type_expires, expires_);
  }
  if (update_) {
    serializer.SaveElement(get_update());
  }
  if (abstractview_) {
    serializer.SaveElement(get_abstractview());
  }
  SerializeUnknown(serializer);
  serializer.End();
}

}  // end namespace kmldom
