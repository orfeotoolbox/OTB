#!/usr/bin/env python

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

# This program demonstrates use of the KML DOM Python SWIG bindings for
# creating and accessing simple elements and attributes such as
# Feature's <name> and <visibility> and Object's id= as in <Placemark>.

import sys
import kmldom

def main():
  print '== This is %s' % sys.argv[0]
  
  factory = kmldom.KmlFactory_GetFactory()
  
  print 'Creating a Placemark...'
  placemark = factory.CreatePlacemark()
  
  print 'Get the Type()...'
  print 'Placemark.Type()',placemark.Type()
  
  print 'has_xxx() all return false...'
  print 'Placemark.has_id()',placemark.has_id()
  print 'Placemark.has_name()',placemark.has_name()
  print 'Placemark.has_visibility()',placemark.has_visibility()
  print 'Placemark.has_geometry()',placemark.has_geometry()
  
  print 'Set some fields...'
  # Object std::string
  placemark.set_id("pm123")
  # Feature std::string
  placemark.set_name("hi")
  # Feature bool
  placemark.set_visibility(False)  # True by default
  
  print 'has_xxx() now return true...'
  print 'Placemark.has_id()',placemark.has_id()
  print 'Placemark.has_name()',placemark.has_name()
  print 'Placemark.has_id()',placemark.has_id()
  
  print 'Get some fields...'
  print 'Placemark.id()',placemark.get_id()
  print 'Placemark.name()',placemark.get_name()
  print 'Placemark.visibility()',placemark.get_visibility()
  
  print 'Clear fields...'
  placemark.clear_name()
  placemark.clear_id()
  placemark.clear_visibility()
  print 'Placemark.has_id()',placemark.has_id()
  print 'Placemark.has_name()',placemark.has_name()
  print 'placemark.has_visibility()',placemark.has_visibility()
  
  print 'visibility is true by default...'
  print 'placemark.visibility()',placemark.get_visibility()

  # Python deletes the placemark.
  
if __name__ == '__main__':
  main()
