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

// This file declares the KmlHandler specialization of ExpatHandler.
// This is used internally to the Parse() function.  KmlHandler is constructed
// with a list of ParserObservers which essentially layer on an Element-level
// SAX parse as the DOM is built.

// Note: although the parser itself is SAX-driven, we make a best effort
// to preserve all unknown (non-KML) elements found during the parse, and
// will serialize those elements after the known elements on output.

#ifndef KML_DOM_KML_HANDLER_H__
#define KML_DOM_KML_HANDLER_H__

#include <stack>
#include "kml/base/expat_handler.h"
#include "kml/dom/element.h"
#include "kml/dom/kml_ptr.h"
#include "kml/dom/parser_observer.h"

namespace kmldom {

class KmlFactory;

// This class implements the expat handlers for parsing KML.  This class is
// handed to expat in the ExpatParser() function.
class KmlHandler : public kmlbase::ExpatHandler {
public:
  KmlHandler(parser_observer_vector_t& observers);
  ~KmlHandler();

  // ExpatHandler methods
  virtual void StartElement(const string& name,
                            const kmlbase::StringVector& atts);
  virtual void EndElement(const string& name);
  virtual void CharData(const string& s);

  // This destructively removes the Element on the top of the stack and
  // transfers ownership of it to the caller.  The intention is to use this
  // after a successful parse.
  ElementPtr PopRoot();

private:
  const KmlFactory& kml_factory_;
  std::stack<ElementPtr> stack_;
  // Char data is managed as a stack to allow for gathering all character data
  // inside unknown elements.
  std::stack<string> char_data_;
  // Helpers for handling unknown elements:
  void InsertUnknownStartElement(const string& name,
                                 const kmlbase::StringVector& atts);
  void InsertUnknownEndElement(const string& name);
  unsigned int skip_depth_;
  unsigned int in_description_;
  unsigned int nesting_depth_;

  // This calls the NewElement() method of each ParserObserver.  If any
  // ParserObserver::NewElement() returns false this immediately returns false.
  // If all ParserObserver::NewElement()'s return true this returns true.
  bool CallNewElementObservers(const parser_observer_vector_t& observers,
                               const ElementPtr& element);

  // This calls the EndElement() method of each ParserObserver.  If any
  // ParserObserver::EndElement() returns false the child is NOT added to
  // the parent.
  bool CallEndElementObservers(const parser_observer_vector_t& observers,
                               const ElementPtr& parent,
                               const ElementPtr& child);

  // This calls the AddChild() method of each ParserObserver.  If any
  // ParserObserver::AddChild() returns false this immediately returns false.
  // If all ParserObserver::AddChild()'s return true this returns true.
  bool CallAddChildObservers(const parser_observer_vector_t& observers,
                             const ElementPtr& parent,
                             const ElementPtr& child);

  const parser_observer_vector_t& observers_;
  LIBKML_DISALLOW_EVIL_CONSTRUCTORS(KmlHandler);
};

} // end namespace kmldom

#endif  // KML_DOM_KML_HANDLER_H__
