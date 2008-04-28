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

import os
import os.path

Import('env')
env = env.Clone()

find_perl_path = "perl -e 'use Config; print $Config{archlib};'"
perl_path = os.path.join(os.popen(find_perl_path).readlines()[0], 'CORE')

env.Append(SWIGFLAGS=['-perl'])

libname = 'kmldom'

if env['PLATFORM'] == 'darwin':
  env.Append(CPPPATH=[perl_path])
  env.Append(LIBPATH=[perl_path])
  env.Append(LIBS=['perl'])

if env['PLATFORM'] == 'posix':
  env.Append(CPPPATH=[perl_path])

if env['PLATFORM'] == 'win32':
  # TODO
  pass

# Mac OS X wants a bundle, not a shared lib.
# TODO: ensure this creates .dll on windows.
env.LoadableModule(libname, ['kmldom.i'])
