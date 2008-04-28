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

env.Append(SWIGFLAGS=['-ruby'])

libname = 'kmldom'

if env['PLATFORM'] == 'darwin':
  # 8.0 is Tiger, 9.0 is Leopard.
  env.Append(CPPPATH=['/usr/lib/ruby/1.8/universal-darwin8.0',
                      '/usr/lib/ruby/1.8/universal-darwin9.0'])
  # Ruby on Mac OS X has to link with the ruby library and wants to
  # require "kmldom.bundle".
  env.Append(LIBS=['ruby'])
  libname = '%s.bundle' % libname

if env['PLATFORM'] == 'posix':
  env.Append(CPPPATH=['/usr/lib/ruby/1.8/i486-linux'])
  # Ruby on Linux wants to require "kmldom.so", not "libkmldom.so".
  env.Replace(SHLIBPREFIX='')

# Mac OS X wants a bundle, not a shared lib.
# TODO: ensure this creates .dll on windows.
env.LoadableModule(libname, ['kmldom.i'])
