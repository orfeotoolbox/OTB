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

// This file contains the definition of the ObjectIdParserObserver class.

#ifndef KML_ENGINE_OBJECT_ID_PARSER_OBSERVER_H__
#define KML_ENGINE_OBJECT_ID_PARSER_OBSERVER_H__

#include <map>
#include <string>
#include "kml/dom.h"
#include "kml/dom/parser_observer.h"
#include "kml/engine/engine_types.h"

namespace kmlengine {

// The ObjectIdParserObserver is a kmldom::ParserObserver which gathers all
// all Objects with id's into the given map.  An Object whose id is already
// in the map causes NewElement() to return false.  When used with
// kmldom::Parse::AddObserver() this immediately terminates the parse.
class ObjectIdParserObserver : public kmldom::ParserObserver {
 public:
  ObjectIdParserObserver(ObjectIdMap* object_id_map)
    : object_id_map_(object_id_map) {}  // TODO: NULL check, or use reference

  virtual ~ObjectIdParserObserver() {}

  // This is ParserObserver::NewElement().  If the Element is an Object with an
  // id and a mapping for this id already exists return false, else add this
  // id to Object mapping to the object_id_map and return true.
  virtual bool NewElement(const kmldom::ElementPtr& element) {
    if (kmldom::ObjectPtr object = kmldom::AsObject(element)) {
      if (object->has_id()) {
        if (object_id_map_->find(object->get_id()) != object_id_map_->end()) {
          // TODO: create an error message
          return false;  // Duplicate id, fail parse.
        }
        (*object_id_map_)[object->get_id()] = object;
      }
    }
    return true;  // Not a duplicate id, keep parsing.
  }

  // The default implementation of Parser::AddChild() is essentially a nop.
 
 private:
  // An ObjectIdParserObserver only exists for a short time and operates
  // on a persistent ObjectIdMap.
  ObjectIdMap* object_id_map_;
};

}  // end namespace kmlengine

#endif  // KML_ENGINE_OBJECT_ID_PARSER_OBSERVER_H__
