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

%include <intrusive_ptr.i>

%define SWIG_INTRUSIVE_PTR_TYPEMAPS(PROXYCLASS, CONST, TYPE...)

%naturalvar TYPE;
%naturalvar SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >;

// destructor mods
%feature("unref") TYPE 
//"if (debug_shared) { cout << \"deleting use_count: \" << (*smartarg1).use_count() << \" [\" << (boost::get_deleter<SWIG_null_deleter>(*smartarg1) ? std::string(\"CANNOT BE DETERMINED SAFELY\") : ( (*smartarg1).get() ? (*smartarg1)->getValue() : std::string(\"NULL PTR\") )) << \"]\" << endl << flush; }\n"
                               "(void)arg1; delete smartarg1;"

%feature("smartptr", noblock=1) TYPE { SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > }

// plain value
%typemap(in) CONST TYPE (void *argp, int res = 0) {
  int newmem = 0;
  res = SWIG_ConvertPtrAndOwn($input, &argp, $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), %convertptr_flags, &newmem);
  if (!SWIG_IsOK(res)) {
    %argument_fail(res, "$type", $symname, $argnum); 
  }
  if (!argp) {
    %argument_nullref("$type", $symname, $argnum);
  } else {
    $1 = *(%reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *)->get());
    if (newmem & SWIG_CAST_NEW_MEMORY) delete %reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *);
  }
}
%typemap(out) CONST TYPE {
  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *smartresult = new SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >(new $1_ltype(($1_ltype &)$1));
  %set_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), SWIG_POINTER_OWN));
}

%typemap(varin) CONST TYPE {
  void *argp = 0;
  int newmem = 0;
  int res = SWIG_ConvertPtrAndOwn($input, &argp, $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), %convertptr_flags, &newmem);
  if (!SWIG_IsOK(res)) {
    %variable_fail(res, "$type", "$name");
  }
  if (!argp) {
    %argument_nullref("$type", $symname, $argnum);
  } else {
    $1 = *(%reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *)->get());
    if (newmem & SWIG_CAST_NEW_MEMORY) delete %reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *);
  }
}
%typemap(varout) CONST TYPE {
  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *smartresult = new SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >(new $1_ltype(($1_ltype &)$1));
  %set_varoutput(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), SWIG_POINTER_OWN));
}

// plain pointer
// Note: $disown not implemented as it will lead to a memory leak of the intrusive_ptr instance
%typemap(in) CONST TYPE * (void  *argp = 0, int res = 0, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > tempshared, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *smartarg = 0) {
  int newmem = 0;
  res = SWIG_ConvertPtrAndOwn($input, &argp, $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), %convertptr_flags, &newmem);
  if (!SWIG_IsOK(res)) {
    %argument_fail(res, "$type", $symname, $argnum); 
  }
  if (newmem & SWIG_CAST_NEW_MEMORY) {
    tempshared = *%reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *);
    delete %reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *);
    $1 = %const_cast(tempshared.get(), $1_ltype);
  } else {
    smartarg = %reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *);
    $1 = %const_cast((smartarg ? smartarg->get() : 0), $1_ltype);
  }
}
%typemap(out, fragment="SWIG_null_deleter") CONST TYPE * {
  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *smartresult = $1 ? new SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >($1 SWIG_NO_NULL_DELETER_$owner) : 0;
  %set_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), $owner | SWIG_POINTER_OWN));
}

%typemap(varin) CONST TYPE * {
  void *argp = 0;
  int newmem = 0;
  int res = SWIG_ConvertPtrAndOwn($input, &argp, $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), %convertptr_flags, &newmem);
  if (!SWIG_IsOK(res)) {
    %variable_fail(res, "$type", "$name");
  }
  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > tempshared;
  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *smartarg = 0;
  if (newmem & SWIG_CAST_NEW_MEMORY) {
    tempshared = *%reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *);
    delete %reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *);
    $1 = %const_cast(tempshared.get(), $1_ltype);
  } else {
    smartarg = %reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *);
    $1 = %const_cast((smartarg ? smartarg->get() : 0), $1_ltype);
  }
}
%typemap(varout, fragment="SWIG_null_deleter") CONST TYPE * {
  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *smartresult = $1 ? new SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >($1 SWIG_NO_NULL_DELETER_0) : 0;
  %set_varoutput(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), SWIG_POINTER_OWN));
}

// plain reference
%typemap(in) CONST TYPE & (void  *argp = 0, int res = 0, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > tempshared) {
  int newmem = 0;
  res = SWIG_ConvertPtrAndOwn($input, &argp, $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), %convertptr_flags, &newmem);
  if (!SWIG_IsOK(res)) {
    %argument_fail(res, "$type", $symname, $argnum); 
  }
  if (!argp) { %argument_nullref("$type", $symname, $argnum); }
  if (newmem & SWIG_CAST_NEW_MEMORY) {
    tempshared = *%reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *);
    delete %reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *);
    $1 = %const_cast(tempshared.get(), $1_ltype);
  } else {
    $1 = %const_cast(%reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *)->get(), $1_ltype);
  }
}
%typemap(out, fragment="SWIG_null_deleter") CONST TYPE & {
  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *smartresult = new SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >($1 SWIG_NO_NULL_DELETER_$owner);
  %set_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), SWIG_POINTER_OWN));
}

%typemap(varin) CONST TYPE & {
  void *argp = 0;
  int newmem = 0;
  int res = SWIG_ConvertPtrAndOwn($input, &argp, $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), %convertptr_flags, &newmem);
  if (!SWIG_IsOK(res)) {
    %variable_fail(res, "$type", "$name");
  }
  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > tempshared;
  if (!argp) { %argument_nullref("$type", $symname, $argnum); }
  if (newmem & SWIG_CAST_NEW_MEMORY) {
    tempshared = *%reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *);
    delete %reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *);
    $1 = *%const_cast(tempshared.get(), $1_ltype);
  } else {
    $1 = *%const_cast(%reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *)->get(), $1_ltype);
  }
}
%typemap(varout, fragment="SWIG_null_deleter") CONST TYPE & {
  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *smartresult = new SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >(&$1 SWIG_NO_NULL_DELETER_0);
  %set_varoutput(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), SWIG_POINTER_OWN));
}

// plain pointer by reference
// Note: $disown not implemented as it will lead to a memory leak of the intrusive_ptr instance
%typemap(in) CONST TYPE *& (void  *argp = 0, int res = 0, $*1_ltype temp = 0, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > tempshared) {
  int newmem = 0;
  res = SWIG_ConvertPtrAndOwn($input, &argp, $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), %convertptr_flags, &newmem);
  if (!SWIG_IsOK(res)) {
    %argument_fail(res, "$type", $symname, $argnum); 
  }
  if (newmem & SWIG_CAST_NEW_MEMORY) {
    tempshared = *%reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *);
    delete %reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *);
    temp = %const_cast(tempshared.get(), $*1_ltype);
  } else {
    temp = %const_cast(%reinterpret_cast(argp, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *)->get(), $*1_ltype);
  }
  $1 = &temp;
}
%typemap(out, fragment="SWIG_null_deleter") CONST TYPE *& {
  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *smartresult = new SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >(*$1 SWIG_NO_NULL_DELETER_$owner);
  %set_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), SWIG_POINTER_OWN));
}

%typemap(varin) CONST TYPE *& %{
#error "varin typemap not implemented"
%}
%typemap(varout) CONST TYPE *& %{
#error "varout typemap not implemented"
%}

// intrusive_ptr by value
%typemap(in) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > (void *argp, int res = 0) {
  int newmem = 0;
  res = SWIG_ConvertPtrAndOwn($input, &argp, $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), %convertptr_flags, &newmem);
  if (!SWIG_IsOK(res)) {
    %argument_fail(res, "$type", $symname, $argnum); 
  }
  if (argp) $1 = *(%reinterpret_cast(argp, $&ltype));
  if (newmem & SWIG_CAST_NEW_MEMORY) delete %reinterpret_cast(argp, $&ltype);
}
%typemap(out) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > {
  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *smartresult = $1 ? new SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >($1) : 0;
  %set_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), SWIG_POINTER_OWN));
}

%typemap(varin) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > {
  int newmem = 0;
  void *argp = 0;
  int res = SWIG_ConvertPtrAndOwn($input, &argp, $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), %convertptr_flags, &newmem);
  if (!SWIG_IsOK(res)) {
    %variable_fail(res, "$type", "$name");
  }
  $1 = argp ? *(%reinterpret_cast(argp, $&ltype)) : SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE >();
  if (newmem & SWIG_CAST_NEW_MEMORY) delete %reinterpret_cast(argp, $&ltype);
}
%typemap(varout) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > {
  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *smartresult = $1 ? new SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >($1) : 0;
  %set_varoutput(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), SWIG_POINTER_OWN));
}

// intrusive_ptr by reference
%typemap(in) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > & (void *argp, int res = 0, $*1_ltype tempshared) {
  int newmem = 0;
  res = SWIG_ConvertPtrAndOwn($input, &argp, $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), %convertptr_flags, &newmem);
  if (!SWIG_IsOK(res)) {
    %argument_fail(res, "$type", $symname, $argnum); 
  }
  if (newmem & SWIG_CAST_NEW_MEMORY) {
    if (argp) tempshared = *%reinterpret_cast(argp, $ltype);
    delete %reinterpret_cast(argp, $ltype);
    $1 = &tempshared;
  } else {
    $1 = (argp) ? %reinterpret_cast(argp, $ltype) : &tempshared;
  }
}
%typemap(out) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > & {
  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *smartresult = *$1 ? new SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >(*$1) : 0;
  %set_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), SWIG_POINTER_OWN));
}

%typemap(varin) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > & %{
#error "varin typemap not implemented"
%}
%typemap(varout) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > & %{
#error "varout typemap not implemented"
%}

// intrusive_ptr by pointer
%typemap(in) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > * (void *argp, int res = 0, $*1_ltype tempshared) {
  int newmem = 0;
  res = SWIG_ConvertPtrAndOwn($input, &argp, $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), %convertptr_flags, &newmem);
  if (!SWIG_IsOK(res)) {
    %argument_fail(res, "$type", $symname, $argnum); 
  }
  if (newmem & SWIG_CAST_NEW_MEMORY) {
    if (argp) tempshared = *%reinterpret_cast(argp, $ltype);
    delete %reinterpret_cast(argp, $ltype);
    $1 = &tempshared;
  } else {
    $1 = (argp) ? %reinterpret_cast(argp, $ltype) : &tempshared;
  }
}
%typemap(out) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > * {
  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *smartresult = $1 && *$1 ? new SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >(*$1) : 0;
  %set_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), SWIG_POINTER_OWN));
  if ($owner) delete $1;
}

%typemap(varin) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > * %{
#error "varin typemap not implemented"
%}
%typemap(varout) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > * %{
#error "varout typemap not implemented"
%}

// intrusive_ptr by pointer reference
%typemap(in) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *& (void *argp, int res = 0, SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > tempshared, $*1_ltype temp = 0) {
  int newmem = 0;
  res = SWIG_ConvertPtrAndOwn($input, &argp, $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), %convertptr_flags, &newmem);
  if (!SWIG_IsOK(res)) {
    %argument_fail(res, "$type", $symname, $argnum); 
  }
  if (argp) tempshared = *%reinterpret_cast(argp, $*ltype);
  if (newmem & SWIG_CAST_NEW_MEMORY) delete %reinterpret_cast(argp, $*ltype);
  temp = &tempshared;
  $1 = &temp;
}
%typemap(out) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *& {
  SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *smartresult = *$1 && **$1 ? new SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >(**$1) : 0;
  %set_output(SWIG_NewPointerObj(%as_voidptr(smartresult), $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), SWIG_POINTER_OWN));
}

%typemap(varin) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *& %{
#error "varin typemap not implemented"
%}
%typemap(varout) SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *& %{
#error "varout typemap not implemented"
%}

// Typecheck typemaps
// Note: SWIG_ConvertPtr with void ** parameter set to 0 instead of using SWIG_ConvertPtrAndOwn, so that the casting 
// function is not called thereby avoiding a possible smart pointer copy constructor call when casting up the inheritance chain.
%typemap(typecheck,precedence=SWIG_TYPECHECK_POINTER,noblock=1) 
                      CONST TYPE,
                      CONST TYPE &,
                      CONST TYPE *,
                      CONST TYPE *&,
                      SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >,
                      SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > &,
                      SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *,
                      SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE > *& {
  int res = SWIG_ConvertPtr($input, 0, $descriptor(SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< TYPE > *), 0);
  $1 = SWIG_CheckState(res);
}


// various missing typemaps - If ever used (unlikely) ensure compilation error rather than runtime bug
%typemap(in) CONST TYPE[], CONST TYPE[ANY], CONST TYPE (CLASS::*) %{
#error "typemaps for $1_type not available"
%}
%typemap(out) CONST TYPE[], CONST TYPE[ANY], CONST TYPE (CLASS::*) %{
#error "typemaps for $1_type not available"
%}


%template() SWIG_INTRUSIVE_PTR_QNAMESPACE::intrusive_ptr< CONST TYPE >;
%enddef

