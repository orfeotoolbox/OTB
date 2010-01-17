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

// This file is derived from swig-1.3.35/Lib/python/boost_shared_ptr.i
// This file can and should be removed from libkml If and when this file or
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

%include <intrusive_ptr.i>

%define SWIG_INTRUSIVE_PTR_TYPEMAPS(PROXYCLASS, CONST, TYPE...)

%naturalvar TYPE;
%naturalvar SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >;

// destructor mods
%feature("unref") TYPE 
//"if (debug_shared) { cout << \"deleting use_count: \" << (*smartarg1).use_count() << \" [\" << (boost::get_deleter<SWIG_null_deleter>(*smartarg1) ? std::string(\"CANNOT BE DETERMINED SAFELY\") : ( (*smartarg1).get() ? (*smartarg1)->getValue() : std::string(\"NULL PTR\") )) << \"]\" << endl << flush; }\n"
                               "(void)arg1; delete smartarg1;"


// plain value
%typemap(in) CONST TYPE ($&1_type argp = 0) %{
  argp = (*(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > **)&$input) ? (*(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > **)&$input)->get() : 0;
  if (!argp) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "Attempt to dereference null $1_type");
    return $null;
  }
  $1 = *argp; %}
%typemap(out) CONST TYPE 
%{ *(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > **)&$result = new SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >(new $1_ltype(($1_ltype &)$1)); %}

// plain pointer
%typemap(in) CONST TYPE * (SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *smartarg = 0) %{
  smartarg = *(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > **)&$input;
  $1 = (TYPE *)(smartarg ? smartarg->get() : 0); %}
%typemap(out, fragment="SWIG_null_deleter") CONST TYPE * %{
  *(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > **)&$result = $1 ? new SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >($1 SWIG_NO_NULL_DELETER_$owner) : 0;
%}

// plain reference
%typemap(in) CONST TYPE & %{
  $1 = ($1_ltype)((*(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > **)&$input) ? (*(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > **)&$input)->get() : 0);
  if(!$1) {
    SWIG_JavaThrowException(jenv, SWIG_JavaNullPointerException, "$1_type reference is null");
    return $null;
  } %}
%typemap(out, fragment="SWIG_null_deleter") CONST TYPE &
%{ *(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > **)&$result = new SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >($1 SWIG_NO_NULL_DELETER_$owner); %}

// plain pointer by reference
%typemap(in) CONST TYPE *& ($*1_ltype temp = 0)
%{ temp = ((*(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > **)&$input) ? (*(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > **)&$input)->get() : 0);
   $1 = &temp; %}
%typemap(out, fragment="SWIG_null_deleter") CONST TYPE *&
%{ *(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > **)&$result = new SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >(*$1 SWIG_NO_NULL_DELETER_$owner); %}

// intrusive_ptr by value
%typemap(in) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > ($&1_type argp)
%{ argp = *($&1_ltype*)&$input; 
   if (argp) $1 = *argp; %}
%typemap(out) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >
%{ *($&1_ltype*)&$result = $1 ? new $1_ltype($1) : 0; %}

// intrusive_ptr by reference
%typemap(in) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > & ($*1_ltype tempnull)
%{ $1 = $input ? *($&1_ltype)&$input : &tempnull; %}
%typemap(out) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > &
%{ *($&1_ltype)&$result = *$1 ? new $*1_ltype(*$1) : 0; %} 

// intrusive_ptr by pointer
%typemap(in) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > * ($*1_ltype tempnull)
%{ $1 = $input ? *($&1_ltype)&$input : &tempnull; %}
%typemap(out) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *
%{ *($&1_ltype)&$result = ($1 && *$1) ? new $*1_ltype(*$1) : 0;
   if ($owner) delete $1; %}

// intrusive_ptr by pointer reference
%typemap(in) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *& (SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > tempnull, $*1_ltype temp = 0)
%{ temp = $input ? *($1_ltype)&$input : &tempnull;
   $1 = &temp; %}
%typemap(out) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *&
%{ *($1_ltype)&$result = (*$1 && **$1) ? new SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >(**$1) : 0; %} 

// various missing typemaps - If ever used (unlikely) ensure compilation error rather than runtime bug
%typemap(in) CONST TYPE[], CONST TYPE[ANY], CONST TYPE (CLASS::*) %{
#error "typemaps for $1_type not available"
%}
%typemap(out) CONST TYPE[], CONST TYPE[ANY], CONST TYPE (CLASS::*) %{
#error "typemaps for $1_type not available"
%}


%typemap (jni)    SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >, 
                  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > &,
                  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *,
                  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *& "jlong"
%typemap (jtype)  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >, 
                  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > &,
                  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *,
                  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *& "long"
%typemap (jstype) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >, 
                  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > &,
                  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *,
                  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *& "PROXYCLASS"

%typemap(javain) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >, 
                 SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > &,
                 SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *,
                 SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *& "PROXYCLASS.getCPtr($javainput)"

%typemap(javaout) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > {
    long cPtr = $jnicall;
    return (cPtr == 0) ? null : new PROXYCLASS(cPtr, true);
  }
%typemap(javaout) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > & {
    long cPtr = $jnicall;
    return (cPtr == 0) ? null : new PROXYCLASS(cPtr, true);
  }
%typemap(javaout) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > * {
    long cPtr = $jnicall;
    return (cPtr == 0) ? null : new PROXYCLASS(cPtr, true);
  }
%typemap(javaout) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *& {
    long cPtr = $jnicall;
    return (cPtr == 0) ? null : new PROXYCLASS(cPtr, true);
  }


%typemap(javaout) CONST TYPE {
    return new PROXYCLASS($jnicall, true);
  }
%typemap(javaout) CONST TYPE & {
    return new PROXYCLASS($jnicall, true);
  }
%typemap(javaout) CONST TYPE * {
    long cPtr = $jnicall;
    return (cPtr == 0) ? null : new PROXYCLASS(cPtr, true);
  }
%typemap(javaout) CONST TYPE *& {
    long cPtr = $jnicall;
    return (cPtr == 0) ? null : new PROXYCLASS(cPtr, true);
  }

// Base proxy classes
%typemap(javabody) TYPE %{
  private long swigCPtr;
  private boolean swigCMemOwnBase;

  protected $javaclassname(long cPtr, boolean cMemoryOwn) {
    swigCMemOwnBase = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr($javaclassname obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }
%}

// Derived proxy classes
%typemap(javabody_derived) TYPE %{
  private long swigCPtr;
  private boolean swigCMemOwnDerived;

  protected $javaclassname(long cPtr, boolean cMemoryOwn) {
    super($imclassname.$javaclassname_SWIGSharedPtrUpcast(cPtr), true);
    swigCMemOwnDerived = cMemoryOwn;
    swigCPtr = cPtr;
  }

  protected static long getCPtr($javaclassname obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }
%}

%typemap(javadestruct, methodname="delete", methodmodifiers="public synchronized") TYPE {
    if(swigCPtr != 0 && swigCMemOwnBase) {
      swigCMemOwnBase = false;
      $jnicall;
    }
    swigCPtr = 0;
  }

%typemap(javadestruct_derived, methodname="delete", methodmodifiers="public synchronized") TYPE {
    if(swigCPtr != 0 && swigCMemOwnDerived) {
      swigCMemOwnDerived = false;
      $jnicall;
    }
    swigCPtr = 0;
    super.delete();
  }

// CONST version needed ???? also for C#
%typemap(jtype, nopgcpp="1") SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > swigSharedPtrUpcast "long"
%typemap(jtype, nopgcpp="1") SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > swigSharedPtrUpcast "long"


%template() SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >;
%enddef

