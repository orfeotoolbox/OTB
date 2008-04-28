# Copyright 2008, Google Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
#
#  1. Redistributions of source code must retain the above copyright notice, 
#     this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright notice,
#     this list of conditions and the following disclaimer in the documentation
#     and/or other materials provided with the distribution.
#  3. Neither the name of Google Inc. nor the names of its contributors may be
#     used to endorse or promote products derived from this software without
#     specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Import('env')
env = env.Clone()

env.Append(SWIGFLAGS = ['-java'])

# Directory(s) for jni.h, jni_md.h
if env['PLATFORM'] == 'darwin':
  env.Append(CPPPATH = ['/System/Library/Frameworks/JavaVM.framework/Headers'])
if env['PLATFORM'] == 'posix':
  # TODO: what's the right way to do this? Ideally we have an equivalent to
  # ./configure --java-sdk-path or similar. Hard-coding these paths is merely
  # a development convenience.
  env.Append(CPPPATH = ['/usr/lib/j2sdk1.5-sun/include',
                        '/usr/lib/j2sdk1.5-sun/include/linux'])
if env['PLATFORM'] == 'win32':
  env.Append(CPPPATH = ['C:\Program Files\Java\jdk1.6.0_04\include',
                        'C:\Program Files\Java\jdk1.6.0_04\include\win32'])

if env['PLATFORM'] == 'darwin':
  env.Replace(SHLIBSUFFIX = '.jnilib')

# TODO: Windows

env.SharedLibrary('kmldom', ['kmldom.i'])
