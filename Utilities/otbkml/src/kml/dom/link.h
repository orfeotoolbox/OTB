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

// This file contains the declarations for the the classes for the Link, Icon,
// and Url elements.

#ifndef KML_DOM_LINK_H__
#define KML_DOM_LINK_H__

#include <string>
#include "kml/dom/kml22.h"
#include "kml/dom/object.h"

namespace kmldom {

// This is BasicLinkType in the KML 2.2 XSD.
class BasicLink : public Object {
 public:
  virtual ~BasicLink();
  virtual KmlDomType Type() const { return Type_BasicLink; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_BasicLink || Object::IsA(type);
  }

  // <href>
  const std::string& href() const {
    return href_;
  }
  bool has_href() const {
    return has_href_;
  }
  void set_href(const std::string& href) {
    href_ = href;
    has_href_ = true;
  }
  void clear_href() {
    href_.clear();
    has_href_ = false;
  }

 protected:
  // Internal class, not for direct instantiation.
  BasicLink();
  virtual void AddElement(Element* element);
  virtual void Serialize(Serializer& serializer) const;

 private:
  std::string href_;
  bool has_href_;
  DISALLOW_EVIL_CONSTRUCTORS(BasicLink);
};

// This is LinkType in the KML 2.2 XSD.  LinkType is the only XSD complexType
// instantiated as more than one element.
class AbstractLink : public BasicLink {
 public:
  virtual ~AbstractLink();

  // <refreshMode>
  const int refreshmode() const {
    return refreshmode_;
  }
  bool has_refreshmode() const {
    return has_refreshmode_;
  }
  void set_refreshmode(const int refreshmode) {
    refreshmode_ = refreshmode;
    has_refreshmode_ = true;
  }
  void clear_refreshmode() {
    refreshmode_ = REFRESHMODE_ONCHANGE;
    has_refreshmode_ = false;
  }

  // <refreshInterval>
  const double refreshinterval() const {
    return refreshinterval_;
  }
  bool has_refreshinterval() const {
    return has_refreshinterval_;
  }
  void set_refreshinterval(const double refreshinterval) {
    refreshinterval_ = refreshinterval;
    has_refreshinterval_ = true;
  }
  void clear_refreshinterval() {
    refreshinterval_ = 4.0;
    has_refreshinterval_ = false;
  }

  // <viewRefreshMode>
  const int viewrefreshmode() const {
    return viewrefreshmode_;
  }
  bool has_viewrefreshmode() const {
    return has_viewrefreshmode_;
  }
  void set_viewrefreshmode(const int viewrefreshmode) {
    viewrefreshmode_ = viewrefreshmode;
    has_viewrefreshmode_ = true;
  }
  void clear_viewrefreshmode() {
    viewrefreshmode_ = VIEWREFRESHMODE_NEVER;
    has_viewrefreshmode_ = false;
  }

  // <viewRefreshTime>
  const double viewrefreshtime() const {
    return viewrefreshtime_;
  }
  bool has_viewrefreshtime() const {
    return has_viewrefreshtime_;
  }
  void set_viewrefreshtime(const double viewrefreshtime) {
    viewrefreshtime_ = viewrefreshtime;
    has_viewrefreshtime_ = true;
  }
  void clear_viewrefreshtime() {
    viewrefreshtime_ = 4.0;
    has_viewrefreshtime_ = false;
  }

  // <viewBoundScale>
  const double viewboundscale() const {
    return viewboundscale_;
  }
  bool has_viewboundscale() const {
    return has_viewboundscale_;
  }
  void set_viewboundscale(const double viewboundscale) {
    viewboundscale_ = viewboundscale;
    has_viewboundscale_ = true;
  }
  void clear_viewboundscale() {
    viewboundscale_ = 1.0;
    has_viewboundscale_ = false;
  }

  // <viewformat>
  const std::string& viewformat() const {
    return viewformat_;
  }
  bool has_viewformat() const {
    return has_viewformat_;
  }
  void set_viewformat(const std::string& viewformat) {
    viewformat_ = viewformat;
    has_viewformat_ = true;
  }
  void clear_viewformat() {
    viewformat_.clear();
    has_viewformat_ = false;
  }

  // <httpquery>
  const std::string& httpquery() const {
    return httpquery_;
  }
  bool has_httpquery() const {
    return has_httpquery_;
  }
  void set_httpquery(const std::string& httpquery) {
    httpquery_ = httpquery;
    has_httpquery_ = true;
  }
  void clear_httpquery() {
    httpquery_.clear();
    has_httpquery_ = false;
  }

 protected:
  // Internal class, not for direct instantiation.
  AbstractLink();
  virtual void AddElement(Element* element);
  virtual void Serialize(Serializer& serializer) const;

 private:
  bool has_href_;
  int refreshmode_;
  bool has_refreshmode_;
  double refreshinterval_;
  bool has_refreshinterval_;
  int viewrefreshmode_;
  bool has_viewrefreshmode_;
  double viewrefreshtime_;
  bool has_viewrefreshtime_;
  double viewboundscale_;
  bool has_viewboundscale_;
  std::string viewformat_;
  bool has_viewformat_;
  std::string httpquery_;
  bool has_httpquery_;
  DISALLOW_EVIL_CONSTRUCTORS(AbstractLink);
};

// <Link> in NetworkLink and Model
class Link : public AbstractLink {
 public:
  virtual ~Link();
  virtual KmlDomType Type() const { return Type_Link; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Link || AbstractLink::IsA(type);
  }

 private:
  friend class KmlFactory;
  Link();
  DISALLOW_EVIL_CONSTRUCTORS(Link);
};

// <Icon> in Overlay
class Icon : public AbstractLink {
 public:
  virtual ~Icon();
  virtual KmlDomType Type() const { return Type_Icon; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Icon || AbstractLink::IsA(type);
  }

 private:
  friend class KmlFactory;
  Icon();
  DISALLOW_EVIL_CONSTRUCTORS(Icon);
};

// <Url> in NetworkLink
class Url : public AbstractLink {
 public:
  virtual ~Url();
  virtual KmlDomType Type() const { return Type_Url; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_Url || AbstractLink::IsA(type);
  }

 private:
  friend class KmlFactory;
  Url();
  DISALLOW_EVIL_CONSTRUCTORS(Url);
};

// <Icon> in <IconStyle>
// This is the only case in KML of a non-global element.  The <Icon>
// of any Overlay is the same as <Link> with all refresh modes, etc.
// The <Icon> of <IconStyle> is just an <href> to an icon as the
// derivation from BasicLink suggests.
class IconStyleIcon : public BasicLink {
 public:
  virtual ~IconStyleIcon();
  virtual KmlDomType Type() const { return Type_IconStyleIcon; }
  virtual bool IsA(KmlDomType type) const {
    return type == Type_IconStyleIcon || BasicLink::IsA(type);
  }

 private:
  friend class KmlFactory;
  IconStyleIcon();
  friend class Serializer;
  virtual void Serialize(Serializer& serializer) const;
  DISALLOW_EVIL_CONSTRUCTORS(IconStyleIcon);
};

}  // end namespace kmldom

#endif  // KML_DOM_LINK_H__
