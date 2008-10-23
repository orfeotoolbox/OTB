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


// An example of coding from/to parse/serialize of enumerations.
// For example <altitudeMode>absolute</altitudeMode>.

#include <assert.h>
#include <iostream>
#include <string>
#include "kml/dom.h"

using kmldom::ElementPtr;
using kmldom::KmlFactory;
using kmldom::LinkPtr;
using kmldom::LookAtPtr;

// Parse KML with enum and verify through the DOM API.
void CheckParseLookAt() {
  // Parse some KML with an enum
  std::string kml("<LookAt><altitudeMode>absolute</altitudeMode></LookAt>");
  ElementPtr root = kmldom::Parse(kml, NULL);

  // Verify the parse went fine.
  assert(root);  // The kml is perfect.

  // Verify the root is <LookAt>
  const LookAtPtr lookat = kmldom::AsLookAt(root);
  assert(lookat);

  // Verify the altitudeMode exists and is correct.
  assert(lookat->has_altitudemode());
  assert(kmldom::ALTITUDEMODE_ABSOLUTE == lookat->get_altitudemode());
}

void CheckParseLink() {
  // Parse a Link.  These are non-default values.
  std::string kml("<Link>"
                  "<href>foo.kml</href>"
                  "<refreshMode>onInterval</refreshMode>"
                  "<viewRefreshMode>onStop</viewRefreshMode>"
                  "</Link>");
  ElementPtr root = kmldom::Parse(kml, NULL);  // No error string because...
  assert(root);  // ...we expect a perfect parse.

  // Verify some things.
  const LinkPtr link = kmldom::AsLink(root);
  assert(true == link->has_refreshmode());
  assert(kmldom::REFRESHMODE_ONINTERVAL == link->get_refreshmode());
  assert(true == link->has_viewrefreshmode());
  assert(kmldom::VIEWREFRESHMODE_ONSTOP == link->get_viewrefreshmode());
}

// Create KML with the DOM API, serialize and verify the output.
void CheckSerializeLookAt() {
  // Create a LookAt and set its altitudeMode
  KmlFactory* factory = KmlFactory::GetFactory();
  LookAtPtr lookat = factory->CreateLookAt();
  lookat->set_altitudemode(kmldom::ALTITUDEMODE_RELATIVETOGROUND);

  // Read back altitudeMode with the DOM API.
  assert(lookat->has_altitudemode());
  assert(kmldom::ALTITUDEMODE_RELATIVETOGROUND == lookat->get_altitudemode());

  // Serialize it out to a string
  std::string kml = kmldom::SerializePretty(lookat);
  std::cout << kml << std::endl;
  // <LookAt>
  //   <altitudeMode>relativeToGround</altitudeMode>
  // </LookAt>

  // Verify the string is correct.  (SerializePretty() adds 2 space indent).
  assert(kml.find("<LookAt>") == 0);
  assert(kml.find("<altitudeMode>") == 11);
  assert(kml.find("relativeToGround") == 25);
  assert(kml.find("</altitudeMode>") == 41);
  assert(kml.find("</LookAt>") == 57);
}

void CheckLinkFactory() {
  // Create a <Link/>.
  KmlFactory* factory = KmlFactory::GetFactory();
  LinkPtr link = factory->CreateLink();

  // Verify defaults.
  assert(false == link->has_refreshmode());
  assert(kmldom::REFRESHMODE_ONCHANGE == link->get_refreshmode());
  assert(false == link->has_viewrefreshmode());
  assert(kmldom::VIEWREFRESHMODE_NEVER == link->get_viewrefreshmode());

  // Change a few things.
  const char kGooKml[] = "goo.kml";
  link->set_href(kGooKml);
  link->set_refreshmode(kmldom::REFRESHMODE_ONEXPIRE);
  link->set_viewrefreshmode(kmldom::VIEWREFRESHMODE_ONREGION);

  // Serialize and verify elements have changed:
  std::string changed_kml = kmldom::SerializePretty(link);
  std::cout << changed_kml << std::endl;

  // <Link>
  //   <href>goo.kml</href>
  //   <refreshMode>onExpire</refreshMode>
  //   <viewRefreshMode>onRegion</viewRefreshMode>
  // </Link>

  assert(0 == changed_kml.find("<Link>"));
  assert(9 == changed_kml.find("<href>"));
  assert(15 == changed_kml.find(kGooKml));
  assert(22 == changed_kml.find("</href>"));
  assert(32 == changed_kml.find("<refreshMode>"));
  assert(45 == changed_kml.find("onExpire"));
  assert(53 == changed_kml.find("</refreshMode>"));
  assert(70 == changed_kml.find("<viewRefreshMode>"));
  assert(87 == changed_kml.find("onRegion"));
  assert(95 == changed_kml.find("</viewRefreshMode>"));
  assert(114 == changed_kml.find("</Link>"));
}

int main(int argc, char** argv) {
  CheckParseLookAt();
  CheckParseLink();
  CheckSerializeLookAt();
  CheckLinkFactory();
  return 0;  // All is well if we got this far.
}
