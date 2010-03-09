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

// This file contains the implementation of the abstract Feature element.

#include "kml/dom/feature.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/serializer.h"

namespace kmldom {

Feature::Feature() :
  has_name_(false),
  visibility_(true),
  has_visibility_(false),
  open_(false),
  has_open_(false),
  has_address_(false),
  has_phonenumber_(false),
  has_description_(false),
  has_styleurl_(false),
  gx_balloonvisibility_(false),
  has_gx_balloonvisibility_(false) {
}

Feature::~Feature() {}

void Feature::AddElement(const ElementPtr& element) {
  if (!element) {
    return;
  }
  // Substitution groups.
  if (element->IsA(Type_AbstractView)) {
    set_abstractview(AsAbstractView(element));
    return;
  }
  if (element->IsA(Type_TimePrimitive)) {
    set_timeprimitive(AsTimePrimitive(element));
    return;
  }
  if (element->IsA(Type_StyleSelector)) {
    set_styleselector(AsStyleSelector(element));
    return;
  }

  // Explicit child elements.
  switch(element->Type()) {
    case Type_name:
      has_name_ = element->SetString(&name_);
      break;
    case Type_visibility:
      has_visibility_ = element->SetBool(&visibility_);
      break;
    case Type_open:
      has_open_ = element->SetBool(&open_);
      break;
    case Type_AtomAuthor:
      set_atomauthor(AsAtomAuthor(element));
      break;
    case Type_AtomLink:
      set_atomlink(AsAtomLink(element));
      break;
    case Type_address:
      has_address_ = element->SetString(&address_);
      break;
    case Type_XalAddressDetails:
      set_xaladdressdetails(AsXalAddressDetails(element));
      break;
    case Type_phoneNumber:
      has_phonenumber_ = element->SetString(&phonenumber_);
      break;
    case Type_Snippet:
      set_snippet(AsSnippet(element));
      break;
// TODO: intentionally do not process <snippet> and let it fall to unknown
#if 0
    case Type_snippet:
      // Recognize (little) <snippet> and save as a big <Snippet>.
      SnippetPtr snippet = KmlFactory::GetFactory()->CreateSnippet();
      snippet->set_text(element->get_char_data());
      set_snippet(snippet);
#endif
      break;
    case Type_description:
      has_description_ = element->SetString(&description_);
      break;
    case Type_styleUrl:
      has_styleurl_ = element->SetString(&styleurl_);
      break;
    case Type_Region:
      set_region(AsRegion(element));
      break;
// TODO: intentionally do not process <Metadata> and let it fall to unknown
#if 0
    case Type_Metadata:
      // Recognize <Metdata> and save into <ExtendedData>.
      break;
#endif
    case Type_ExtendedData:
      set_extendeddata(AsExtendedData(element));
      break;
    case Type_GxBalloonVisibility:
      has_gx_balloonvisibility_ = element->SetBool(&gx_balloonvisibility_);
      break;
    default:
      Object::AddElement(element);
  }
}

void Feature::SerializeBeforeStyleSelector(Serializer& serializer) const {
  if (has_name()) {
    serializer.SaveFieldById(Type_name, name_);
  }
  if (has_visibility()) {
    serializer.SaveFieldById(Type_visibility, visibility_);
  }
  if (has_open()) {
    serializer.SaveFieldById(Type_open, open_);
  }
  if (has_atomauthor()) {
    serializer.SaveElement(get_atomauthor());
  }
  if (has_atomlink()) {
    serializer.SaveElement(get_atomlink());
  }
  if (has_address()) {
    serializer.SaveFieldById(Type_address, get_address());
  }
  if (has_phonenumber()) {
    serializer.SaveFieldById(Type_phoneNumber, get_phonenumber());
  }
  if (has_xaladdressdetails()) {
    serializer.SaveElement(get_xaladdressdetails());
  }
  if (has_snippet()) {
    serializer.SaveElement(get_snippet());
  }
  if (has_description()) {
    serializer.SaveFieldById(Type_description, description_);
  }
  if (has_abstractview()) {
    serializer.SaveElementGroup(get_abstractview(), Type_AbstractView);
  }
  if (has_timeprimitive()) {
    serializer.SaveElementGroup(get_timeprimitive(), Type_TimePrimitive);
  }
  if (has_styleurl()) {
    serializer.SaveFieldById(Type_styleUrl, styleurl_);
  }
}

void Feature::SerializeAfterStyleSelector(Serializer& serializer) const {
  if (has_region()) {
    serializer.SaveElement(get_region());
  }
  if (has_extendeddata()) {
    serializer.SaveElement(get_extendeddata());
  }
  if (has_gx_balloonvisibility()) {
    serializer.SaveFieldById(Type_GxBalloonVisibility, gx_balloonvisibility_);
  }
}

void Feature::Serialize(Serializer& serializer) const {
  Feature::SerializeBeforeStyleSelector(serializer);
  if (has_styleselector()) {
    serializer.SaveElementGroup(get_styleselector(), Type_StyleSelector);
  }
  Feature::SerializeAfterStyleSelector(serializer);
}

void Feature::AcceptChildren(VisitorDriver* driver) {
  Object::AcceptChildren(driver);
  if (has_snippet()) {
    driver->Visit(get_snippet());
  }
  if (has_abstractview()) {
    driver->Visit(get_abstractview());
  }
  if (has_timeprimitive()) {
    driver->Visit(get_timeprimitive());
  }
  if (has_styleselector()) {
    driver->Visit(get_styleselector());
  }
  if (has_region()) {
    driver->Visit(get_region());
  }
  if (has_extendeddata()) {
    driver->Visit(get_extendeddata());
  }
}

}  // namespace kmldom
