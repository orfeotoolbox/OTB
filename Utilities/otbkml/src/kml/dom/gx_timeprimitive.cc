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

// This file contains the implementation of the concrete elements GxTimeSpan
// and GxTimeStamp.

#include "kml/dom/gx_timeprimitive.h"
#include "kml/base/attributes.h"

using kmlbase::Attributes;

namespace kmldom {

GxTimeSpan::GxTimeSpan() {
  set_xmlns(kmlbase::XMLNS_GX22);
}

GxTimeSpan::~GxTimeSpan() {}

void GxTimeSpan::Accept(Visitor* visitor) {
  visitor->VisitGxTimeSpan(GxTimeSpanPtr(this));
}

GxTimeStamp::GxTimeStamp() {
  set_xmlns(kmlbase::XMLNS_GX22);
}

GxTimeStamp::~GxTimeStamp() {}

void GxTimeStamp::Accept(Visitor* visitor) {
  visitor->VisitGxTimeStamp(GxTimeStampPtr(this));
}

}  // end namespace kmldom
