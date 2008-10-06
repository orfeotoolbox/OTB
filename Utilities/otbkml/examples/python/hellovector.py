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

# This program demonstrates use of the KML DOM Python SWIG bindings
# for creating and accessing array values such as <coordinates>
# and <Folder>.

import sys
import kmldom

def main():
  print '== This is %s' % sys.argv[0]

  factory = kmldom.KmlFactory_GetFactory()

  print 'Coordinates is a vector of Vec3...'

  coordinates = factory.CreateCoordinates()
  coordinates.add_latlng(1,1)
  coordinates.add_latlng(2,2)
  coordinates.add_latlng(3,3)

  for i in range(coordinates.get_coordinates_array_size()):
    print 'longitude',coordinates.get_coordinates_array_at(i).get_longitude()
    print 'latitude',coordinates.get_coordinates_array_at(i).get_latitude()
    print 'altitude',coordinates.get_coordinates_array_at(i).get_altitude()

  print kmldom.SerializePretty(coordinates)

  # Python deletes coordinates.

  print 'Create a Folder with some Features...'

  folder = factory.CreateFolder()
  folder.add_feature(factory.CreatePlacemark())
  folder.add_feature(factory.CreateFolder())

  for i in range(folder.get_feature_array_size()):
    print 'feature Type',folder.get_feature_array_at(i).Type()

  print kmldom.SerializePretty(folder)

  # Python deletes folder and the child features.

if __name__ == '__main__':
  main()
