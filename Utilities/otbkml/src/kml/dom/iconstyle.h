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

#ifndef KML_DOM_ICONSTYLE_H__
#define KML_DOM_ICONSTYLE_H__

#include "kml/dom/colorstyle.h"
#include "kml/dom/hotspot.h"
#include "kml/dom/kml22.h"
#include "kml/dom/link.h"
#include "kml/util/util.h"

namespace kmldom {

// <IconStyle>
class IconStyle : public ColorStyle {
 public:
  virtual ~IconStyle();
  virtual KmlDomType Type() const { return Type_IconStyle; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_IconStyle || ColorStyle::IsA(type);
  }

  // <scale>
  double scale() const {
    return scale_;
  }
  bool has_scale() const {
    return has_scale_;
  }
  void set_scale(double scale) {
    scale_ = scale;
    has_scale_ = true;
  }
  void clear_scale() {
    scale_ = 1.0;
    has_scale_ = false;
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

  // <Icon> (different than Overlay Icon)
  const IconStyleIcon* icon() const { return icon_; }
  bool has_icon() const { return icon_ != NULL; }
  void set_icon(IconStyleIcon* icon) {
    if (icon == NULL) {
      clear_icon();
    } else if (icon->set_parent(this)) {
      delete icon_;  // note: "last one wins".
      icon_ = icon;
    }
  }
  void clear_icon() {
    delete icon_;
    icon_ = NULL;
  }

  // <hotSpot>
  const HotSpot* hotspot() const { return hotspot_; }
  bool has_hotspot() const { return hotspot_ != NULL; }
  void set_hotspot(HotSpot* hotspot) {
    if (hotspot == NULL) {
      clear_hotspot();
      return;
    } else if (hotspot->set_parent(this)) {
      delete hotspot_;  // note: "last one wins".
      hotspot_ = hotspot;
    }
  }
  void clear_hotspot() {
    delete hotspot_;
    hotspot_ = NULL;
  }

 private:
  friend class KmlFactory;
  IconStyle();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serialize) const;
  double scale_;
  bool has_scale_;
  double heading_;
  bool has_heading_;
  IconStyleIcon* icon_;
  HotSpot* hotspot_;
  DISALLOW_EVIL_CONSTRUCTORS(IconStyle);
};

}  // end namespace kmldom

#endif // KML_DOM_ICONSTYLE_H__
