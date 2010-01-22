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

// This file contains the implementation of the free functions used by
// boost::intrusive_ptr.  See boost/intrusive_ptr.hpp for more information.

#include "kml/base/referent.h"

namespace kmlbase {

// This function is used from within boost::intrusive_ptr to increment the
// reference count when a new intrusive_ptr to a Referent-derived object is
// created.  This function is to be used only from within boost::intrusive_ptr.
void intrusive_ptr_add_ref(kmlbase::Referent* r) {
  r->add_ref();
} 

// This function is used from within boost::intrusive_ptr to decrement the
// reference count when an intrusive_ptr to a Referent-derived object goes out
// of scope.  This is the only call to delete of a Referent-derived type.
// This function is to be used only from within boost::intrusive_ptr.
void intrusive_ptr_release(kmlbase::Referent* r) {
  // Strictly speaking this need only be "if (r->release() == 0)" given that
  // under normal operations with no direct use of these functions or
  // methods on Referent the reference count should never go negative.
  // A full "non-negative" here makes the implementation more robust.
  // An alternative implementation might assert r->release >= 0 to catch
  // usage that goes around the API in some way.
  if (r->release() <= 0) {
    delete r;
  }
} 

}  // end namespace kmlbase
