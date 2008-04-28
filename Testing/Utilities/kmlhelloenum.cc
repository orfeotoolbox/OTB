/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


    Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
    See OTBCopyright.txt for details.

    Some parts of this code are derived from kml library examples. See KMLCopyright.txt
    for details.

    This software is distributed WITHOUT ANY WARRANTY; without even
    the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
    PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// this file defines the otbMultiScaleTest for the test driver
// and all it expects is that you have a function called RegisterTests
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif
#include <iostream>
#include "otbTestMain.h"

void RegisterTests()
{
REGISTER_TEST(kmlhelloenum);
}


// An example of coding from/to parse/serialize of enumerations.
// For example <altitudeMode>absolute</altitudeMode>.

//#include <assert.h>
#include <iostream>
#include <string>
#include "kml/dom.h"

using kmldom::Element;
using kmldom::KmlFactory;
using kmldom::Link;
using kmldom::LookAt;

#include "kml/util/kmlprint.h"

// Parse KML with enum and verify through the DOM API.
void CheckParseLookAt() {
  // Parse some KML with an enum
  std::string kml("<LookAt><altitudeMode>absolute</altitudeMode></LookAt>");
  Element* root = kmldom::Parse(kml, NULL);

  // Verify the parse went fine.
  assert(root);  // The kml is perfect.

  // Verify the root is <LookAt>
  const LookAt* lookat = kmldom::AsLookAt(root);
  assert(lookat);

  // Verify the altitudeMode exists and is correct.
  assert(lookat->has_altitudemode());
  assert(kmldom::ALTITUDEMODE_ABSOLUTE == lookat->altitudemode());

  // Free all resources allocated by the parser.
  delete root;
}

void CheckParseLink() {
  // Parse a Link.  These are non-default values.
  std::string kml("<Link>"
                  "<href>foo.kml</href>"
                  "<refreshMode>onInterval</refreshMode>"
                  "<viewRefreshMode>onStop</viewRefreshMode>"
                  "</Link>");
  Element* root = kmldom::Parse(kml, NULL);  // No error string because...
  assert(root);  // ...we expect a perfect parse.

  // Verify some things.
  const Link* link = kmldom::AsLink(root);
  assert(true == link->has_refreshmode());
  assert(kmldom::REFRESHMODE_ONINTERVAL == link->refreshmode());
  assert(true == link->has_viewrefreshmode());
  assert(kmldom::VIEWREFRESHMODE_ONSTOP == link->viewrefreshmode());

  // Free everything.
  KmlFactory::GetFactory()->DeleteElement(root);
}

// Create KML with the DOM API, serialize and verify the output.
void CheckSerializeLookAt() {
  // Create a LookAt and set its altitudeMode
  KmlFactory* factory = KmlFactory::GetFactory();
  LookAt* lookat = factory->CreateLookAt();
  lookat->set_altitudemode(kmldom::ALTITUDEMODE_RELATIVETOGROUND);

  // Read back altitudeMode with the DOM API.
  assert(lookat->has_altitudemode());
  assert(kmldom::ALTITUDEMODE_RELATIVETOGROUND == lookat->altitudemode());

  // Serialize it out to a string
  std::string kml = kmldom::SerializePretty(*lookat);
  G_kmlprint.flux << kml << std::endl;
  // <LookAt>
  //   <altitudeMode>relativeToGround</altitudeMode>
  // </LookAt>

  // Verify the string is correct.  (SerializePretty() adds 2 space indent).
  assert(kml.find("<LookAt>") == 0);
  assert(kml.find("<altitudeMode>") == 11);
  assert(kml.find("relativeToGround") == 25);
  assert(kml.find("</altitudeMode>") == 41);
  assert(kml.find("</LookAt>") == 57);

  // Free all resources allocated by the factory.
  factory->DeleteElement(lookat);
}

void CheckLinkFactory() {
  // Create a <Link/>.
  KmlFactory* factory = KmlFactory::GetFactory();
  Link* link = factory->CreateLink();

  // Verify defaults.
  assert(false == link->has_refreshmode());
  assert(kmldom::REFRESHMODE_ONCHANGE == link->refreshmode());
  assert(false == link->has_viewrefreshmode());
  assert(kmldom::VIEWREFRESHMODE_NEVER == link->viewrefreshmode());

  // Change a few things.
  const char kGooKml[] = "goo.kml";
  link->set_href(kGooKml);
  link->set_refreshmode(kmldom::REFRESHMODE_ONEXPIRE);
  link->set_viewrefreshmode(kmldom::VIEWREFRESHMODE_ONREGION);

  // Serialize and verify elements have changed:
  std::string changed_kml = kmldom::SerializePretty(*link);
  G_kmlprint.flux << changed_kml << std::endl;

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

  factory->DeleteElement(link);
}

int kmlhelloenum(int argc, char* argv[]) {
  G_kmlprint.flux.open(argv[1]);
  CheckParseLookAt();
  CheckParseLink();
  CheckSerializeLookAt();
  CheckLinkFactory();
  G_kmlprint.flux.close();
  return EXIT_SUCCESS;  // All is well if we got this far.
}
