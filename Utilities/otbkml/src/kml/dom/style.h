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

#ifndef KML_DOM_STYLE_H__
#define KML_DOM_STYLE_H__

#include "kml/dom/balloonstyle.h"
#include "kml/dom/iconstyle.h"
#include "kml/dom/kml22.h"
#include "kml/dom/labelstyle.h"
#include "kml/dom/liststyle.h"
#include "kml/dom/linestyle.h"
#include "kml/dom/polystyle.h"
#include "kml/dom/styleselector.h"

namespace kmldom {

class Serializer;

class Style : public StyleSelector {
 public:
  virtual ~Style();
  virtual KmlDomType Type() const { return Type_Style; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Style || StyleSelector::IsA(type);
  }

  // <IconStyle>
  const IconStyle* iconstyle() const { return iconstyle_; }
  bool has_iconstyle() const { return iconstyle_ != NULL; }
  void set_iconstyle(IconStyle* iconstyle) {
    if (iconstyle == NULL) {
      clear_iconstyle();
    } else if (iconstyle->set_parent(this)) {
      delete iconstyle_;
      iconstyle_ = iconstyle;
    }
  }
  void clear_iconstyle() {
    delete iconstyle_;
    iconstyle_ = NULL;
  }

  // <LabelStyle>
  const LabelStyle* labelstyle() const { return labelstyle_; }
  bool has_labelstyle() const { return labelstyle_ != NULL; }
  void set_labelstyle(LabelStyle* labelstyle) {
    if (labelstyle == NULL) {
      clear_labelstyle();
    } else if (labelstyle->set_parent(this)) {
      delete labelstyle_;
      labelstyle_ = labelstyle;
    }
  }
  void clear_labelstyle() {
    delete labelstyle_;
    labelstyle_ = NULL;
  }

  // <LineStyle>
  const LineStyle* linestyle() const { return linestyle_; }
  bool has_linestyle() const { return linestyle_ != NULL; }
  void set_linestyle(LineStyle* linestyle) {
    if (linestyle == NULL) {
      clear_linestyle();
    } else if (linestyle->set_parent(this)) {
      delete linestyle_;
      linestyle_ = linestyle;
    }
  }
  void clear_linestyle() {
    delete linestyle_;
    linestyle_ = NULL;
  }

  // <PolyStyle>
  const PolyStyle* polystyle() const { return polystyle_; }
  bool has_polystyle() const { return polystyle_ != NULL; }
  void set_polystyle(PolyStyle* polystyle) {
    if (polystyle == NULL) {
      clear_polystyle();
    } else if (polystyle->set_parent(this)) {
      delete polystyle_;
      polystyle_ = polystyle;
    }
  }
  void clear_polystyle() {
    delete polystyle_;
    polystyle_ = NULL;
  }

  // <BalloonStyle>
  const BalloonStyle* balloonstyle() const { return balloonstyle_; }
  bool has_balloonstyle() const { return balloonstyle_ != NULL; }
  void set_balloonstyle(BalloonStyle* balloonstyle) {
    if (balloonstyle == NULL) {
      clear_balloonstyle();
    } else if (balloonstyle->set_parent(this)) {
      delete balloonstyle_;
      balloonstyle_ = balloonstyle;
    }
  }
  void clear_balloonstyle() {
    delete balloonstyle_;
    balloonstyle_ = NULL;
  }

  // <ListStyle>
  const ListStyle* liststyle() const { return liststyle_; }
  bool has_liststyle() const { return liststyle_ != NULL; }
  void set_liststyle(ListStyle* liststyle) {
    if (liststyle == NULL) {
      clear_liststyle();
    } else if (liststyle->set_parent(this)) {
      delete liststyle_;
      liststyle_ = liststyle;
    }
  }
  void clear_liststyle() {
    delete liststyle_;
    liststyle_ = NULL;
  }

 private:
  friend class KmlFactory;
  Style ();
  friend class KmlHandler;
  virtual void AddElement(Element* element);
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  IconStyle* iconstyle_;
  LabelStyle* labelstyle_;
  LineStyle* linestyle_;
  PolyStyle* polystyle_;
  BalloonStyle* balloonstyle_;
  ListStyle* liststyle_;
  DISALLOW_EVIL_CONSTRUCTORS(Style);
};

}  // end namespace kmldom

#endif  // KML_DOM_STYLE_H__
