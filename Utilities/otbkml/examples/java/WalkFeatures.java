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
// for walking the feature hierarchy of a KML file.   KML DOM API
// aspects shown here include: parsing, element type inspection, arrays,
// getting attributes and simple elements.

public class WalkFeatures {

  static {
    try {
      System.loadLibrary("kmldom");
    } catch (UnsatisfiedLinkError e) {
      System.err.println("Failed to load kmldom. " + e);
      System.exit(1);
    }
  }

  public static void Indent(int depth) {
    while (depth-- > 0) {
      System.out.print("  ");
    }
  }

  public static Feature GetRootFeature(Element element) {
    Feature feature = kmldom.AsFeature(element);
    if (feature != null) {
      return feature;
    }
    Kml kml = kmldom.AsKml(element);
    if (kml != null && kml.has_feature()) {
      return kml.get_feature();
    }
    return null;
  }

  public static void VisitFeature(Feature feature, int depth) {
    Indent(depth);
    if (feature.has_id()) {
      System.out.print("id= ");
      System.out.print(feature.get_id());
      System.out.print(" ");
    }
    if (feature.has_name()) {
      System.out.print("name= ");
      System.out.print(feature.get_name());
      System.out.print(" ");
    }
    System.out.println(feature.Type());
    if (feature.IsA(KmlDomType.Type_Container)) {
      WalkContainer(kmldom.AsContainer(feature), depth+1);
    }
  }

  public static void WalkContainer(Container container, int depth) {
    for (int i = 0; i < container.get_feature_array_size(); ++i) {
      VisitFeature(container.get_feature_array_at(i), depth);
    }
  }

  public static void main(String argv[]) {
    Element root = kmldom.ParseKml(
      "<kml>" +
        "<Folder><name>folder</name>" +
          "<Document id=\"d\">" +
            "<Placemark id=\"p\"/>" +
            "<NetworkLink id=\"n\"/>" +
            "<PhotoOverlay>" +
              "<name>photooverlay</name>" +
            "</PhotoOverlay>" +
          "</Document>" +
          "<GroundOverlay id=\"g\"/>" +
          "<ScreenOverlay id=\"s\"/>" +
        "</Folder>" +
      "</kml>");

    Feature feature = GetRootFeature(root);
    if (feature != null) {
      VisitFeature(feature, 0);
    }
  }
}
