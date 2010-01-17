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

// This program demonstrates use of the KML DOM Java SWIG bindings
// for creating a Placemark and serializing to XML.  This demonstrates
// use of the factory, setting complex child elements, setting
// simple elements, and serializing out to XML.

public class CreatePlacemark {

  static {
    try {
      System.loadLibrary("kmldom");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Failed to load kmldom. " + e);
      System.exit(1);
    }
  }

  public static void main(String argv[]) {
    // NOTE: KmlFactory does _not_ use kmldom.
    KmlFactory factory = KmlFactory.GetFactory();

    // <coordinates>1,2</coordinates>
    Coordinates coordinates = factory.CreateCoordinates();
    coordinates.add_latlng(37.0, -122.0);

    // <Point id="pt0">...
    Point point = factory.CreatePoint();
    point.set_id("pt0");
    point.set_coordinates(coordinates);

    // <Placemark id="pm123"><name>my place</name>...
    Placemark placemark = factory.CreatePlacemark();
    placemark.set_name("my placemark");
    placemark.set_id("pm123");
    placemark.set_geometry(point);

    // <kml>...
    Kml kml = factory.CreateKml();
    kml.set_feature(placemark);

    // NOTE: this _requires_ kmldom.
    System.out.print(kmldom.SerializePretty(kml));
  }
}
