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

// This file is derived from swig-1.3.35/Lib/shared_ptr.i
// This file can and should be removed from libkml if and when this file or
// equivalent functionality appears in a future version of SWIG.

// [The following is from swig-1.3.35/LICENSE]
//
// SWIG is distributed under the following terms:
// 
// I.  
// 
// Copyright (c) 1995-1998
// The University of Utah and the Regents of the University of California
// All Rights Reserved
// 
// Permission is hereby granted, without written agreement and without
// license or royalty fees, to use, copy, modify, and distribute this
// software and its documentation for any purpose, provided that 
// (1) The above copyright notice and the following two paragraphs
// appear in all copies of the source code and (2) redistributions
// including binaries reproduces these notices in the supporting
// documentation.   Substantial modifications to this software may be
// copyrighted by their authors and need not follow the licensing terms
// described here, provided that the new terms are clearly indicated in
// all files where they apply.
// 
// IN NO EVENT SHALL THE AUTHOR, THE UNIVERSITY OF CALIFORNIA, THE 
// UNIVERSITY OF UTAH OR DISTRIBUTORS OF THIS SOFTWARE BE LIABLE TO ANY
// PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL
// DAMAGES ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION,
// EVEN IF THE AUTHORS OR ANY OF THE ABOVE PARTIES HAVE BEEN ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
// 
// THE AUTHOR, THE UNIVERSITY OF CALIFORNIA, AND THE UNIVERSITY OF UTAH
// SPECIFICALLY DISCLAIM ANY WARRANTIES,INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE. THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND 
// THE AUTHORS AND DISTRIBUTORS HAVE NO OBLIGATION TO PROVIDE MAINTENANCE,
// SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
// 
// 
// II. 
// 
// This software includes contributions that are Copyright (c) 1998-2005
// University of Chicago.
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// 
// Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.  Redistributions
// in binary form must reproduce the above copyright notice, this list of
// conditions and the following disclaimer in the documentation and/or
// other materials provided with the distribution.  Neither the name of
// the University of Chicago nor the names of its contributors may be
// used to endorse or promote products derived from this software without
// specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY OF CHICAGO AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UNIVERSITY OF
// CHICAGO OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// 
// III.  
// 
// This software includes contributions that are Copyright (c) 2005-2006
// Arizona Board of Regents (University of Arizona).
// All Rights Reserved
// 
// Permission is hereby granted, without written agreement and without
// license or royalty fees, to use, copy, modify, and distribute this
// software and its documentation for any purpose, provided that 
// (1) The above copyright notice and the following two paragraphs
// appear in all copies of the source code and (2) redistributions
// including binaries reproduces these notices in the supporting
// documentation.   Substantial modifications to this software may be
// copyrighted by their authors and need not follow the licensing terms
// described here, provided that the new terms are clearly indicated in
// all files where they apply.
// 
// THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY OF ARIZONA AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE UNIVERSITY OF
// ARIZONA OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// [End of swig-1.3.35/LICENSE]

// intrusive_ptr namespaces could be boost or std or std::tr1
#if !defined(SWIG_INTRUSIVE_PTR_NAMESPACE)
# define SWIG_INTRUSIVE_PTR_NAMESPACE boost
#endif

#if defined(SWIG_INTRUSIVE_PTR_SUBNAMESPACE)
# define SWIG_INTRUSIVE_PTR_QNAMESPACE SWIG_INTRUSIVE_PTR_NAMESPACE::SWIG_INTRUSIVE_PTR_SUBNAMESPACE
#else
# define SWIG_INTRUSIVE_PTR_QNAMESPACE SWIG_INTRUSIVE_PTR_NAMESPACE
#endif

namespace SWIG_INTRUSIVE_PTR_NAMESPACE {
#if defined(SWIG_INTRUSIVE_PTR_SUBNAMESPACE)
  namespace SWIG_INTRUSIVE_PTR_SUBNAMESPACE {
#endif
    template <class T> class intrusive_ptr {
    };
#if defined(SWIG_INTRUSIVE_PTR_SUBNAMESPACE)
  }
#endif
}

%fragment("SWIG_null_deleter", "header") {
struct SWIG_null_deleter {
  void operator() (void const *) const {
  }
};
%#define SWIG_NO_NULL_DELETER_0 , SWIG_null_deleter()
%#define SWIG_NO_NULL_DELETER_1
%#define SWIG_NO_NULL_DELETER_SWIG_POINTER_NEW
%#define SWIG_NO_NULL_DELETER_SWIG_POINTER_OWN
}


// Main user macro for defining intrusive_ptr typemaps for both const and non-const pointer types
// For plain classes, do not use for derived classes
%define SWIG_INTRUSIVE_PTR(PROXYCLASS, TYPE...)
SWIG_INTRUSIVE_PTR_TYPEMAPS(PROXYCLASS, , TYPE)
SWIG_INTRUSIVE_PTR_TYPEMAPS(PROXYCLASS, const, TYPE)
%enddef

// Main user macro for defining intrusive_ptr typemaps for both const and non-const pointer types
// For derived classes
%define SWIG_INTRUSIVE_PTR_DERIVED(PROXYCLASS, BASECLASSTYPE, TYPE...)
SWIG_INTRUSIVE_PTR_TYPEMAPS(PROXYCLASS, , TYPE)
SWIG_INTRUSIVE_PTR_TYPEMAPS(PROXYCLASS, const, TYPE)
%types(SWIG_INTRUSIVE_PTR_NAMESPACE::intrusive_ptr< TYPE > = SWIG_INTRUSIVE_PTR_NAMESPACE::intrusive_ptr< BASECLASSTYPE >) %{
  *newmemory = SWIG_CAST_NEW_MEMORY;
  return (void *) new SWIG_INTRUSIVE_PTR_NAMESPACE::intrusive_ptr< BASECLASSTYPE >(*(SWIG_INTRUSIVE_PTR_NAMESPACE::intrusive_ptr< TYPE > *)$from);
  %}
%extend TYPE {
  static SWIG_INTRUSIVE_PTR_NAMESPACE::intrusive_ptr< BASECLASSTYPE > SWIGSharedPtrUpcast(SWIG_INTRUSIVE_PTR_NAMESPACE::intrusive_ptr< TYPE > swigSharedPtrUpcast) {
    return swigSharedPtrUpcast;
  }
}
%enddef

