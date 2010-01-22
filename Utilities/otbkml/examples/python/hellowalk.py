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
# for walking the feature hierarchy of a KML file.

import sys
import kmldom

argc = len(sys.argv)
if argc != 2:
  print 'usage: %s input.kml' % sys.argv[0]
  sys.exit(1)

inputkml = sys.argv[1]

def ReadFile(filename):
  f = open(filename, 'r')
  data = f.read()
  return data

def Indent(depth):
  while depth:
    print '  ',
    depth -= 1

def PrFeatureType(type):
  if type == kmldom.Type_Placemark:
    print 'Placemark',
  else:
    print 'some other Feature',

# This visits a feature.  The type of feature is printed.  If the feature
# is a container such is visited recursively.
def VisitFeature(feature, depth):
  Indent(depth)
  PrFeatureType(feature.Type())
  print 'id=',feature.get_id()
  container = kmldom.AsContainer(feature)
  if container:
    WalkContainer(container, depth+1)

# This visits each feature in the given container (<Document> or <Folder>).
def WalkContainer(container, depth):
  for i in range(container.get_feature_array_size()):
    VisitFeature(container.get_feature_array_at(i), depth)

# The root feature of a KML file is the child feature of the <kml>
# element or the root xml element if that is a Feature.
# If element is neither <kml> nor a feature None is returned.
def GetRootFeature(element):
  kml = kmldom.AsKml(element)
  if kml:
    if kml.has_feature():
      return kml.get_feature()
    else:
      return None
  feature = kmldom.AsFeature(element)
  if feature:
    return feature
  return None

# Program main: read the file to memory, parse it, get and visit
# the root feature if such exists.
def main():
  feature = GetRootFeature(kmldom.ParseKml(ReadFile(inputkml)))
  if feature:
    VisitFeature(feature, 0)
    # Python deletes the feature and all of its descendant elements in turn.
  else:
    # The file is a KML fragment.
    print 'No root feature in %s' % inputkml

if __name__ == '__main__':
  main()

