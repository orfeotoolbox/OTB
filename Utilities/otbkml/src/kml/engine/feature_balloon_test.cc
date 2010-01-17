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

// This file contains the unit tests for the CreateBalloonText function.

#include "kml/engine/feature_balloon.h"
#include "boost/scoped_ptr.hpp"
#include "kml/base/file.h"
#include "kml/base/unit_test.h"
#include "kml/engine/kml_file.h"

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

using kmlbase::File;
using kmldom::DocumentPtr;
using kmldom::FeaturePtr;
using kmldom::PlacemarkPtr;

namespace kmlengine {

class FeatureBalloonTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(FeatureBalloonTest);
  CPPUNIT_TEST(TestBasicCreateBalloonText);
  CPPUNIT_TEST(TestComplexCreateBalloonText);
  CPPUNIT_TEST(TestAllBalloonsFile);
  CPPUNIT_TEST_SUITE_END();

 protected:
  void TestBasicCreateBalloonText();
  void TestComplexCreateBalloonText();
  void TestAllBalloonsFile();

 private:
  KmlFilePtr kml_file_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(FeatureBalloonTest);

// A struct of the KML content and the expected balloon text.
const static struct {
  const char* kml;
  const char* balloontext;
} kKml[] = {
  {
    // A Feature with neither name nor description has no balloon text.
    // TODO: geDirections is there by default?
    "<Placemark/>",
    NULL
  },
  {
    "<Placemark><name>foo</name></Placemark>",
    "<h3>foo</h3><br/><br/>"
  },
  {
    "<Placemark><name>bar</name><description>goo</description></Placemark>",
    "<h3>bar</h3><br/><br/>goo"
  },
};
void FeatureBalloonTest::TestBasicCreateBalloonText() {
  for (size_t i = 0; i < sizeof(kKml)/sizeof(kKml[0]); ++i) {
    // Parse the file into a KmlFile object.
    kml_file_ = KmlFile::CreateFromParse(kKml[i].kml, NULL);
    // Extract the Placemark.
    const PlacemarkPtr placemark = kmldom::AsPlacemark(kml_file_->root());
    // Verify that that balloon text has been composited as expected.
    const std::string kResult(CreateBalloonText(kml_file_, placemark));
    if (kKml[i].balloontext == NULL) {
      CPPUNIT_ASSERT(kResult.empty());
    } else {
      CPPUNIT_ASSERT_EQUAL(kResult,
                           static_cast<std::string>(kKml[i].balloontext));
    }
  }
}

void FeatureBalloonTest::TestComplexCreateBalloonText() {
  // A hunk of KML that requires the ballon to be composited from a
  // BalloonStyle templated on Data and Schema/SchemaData.
  const std::string kKitchenSinkKml(
      "<Document>"
      "<Schema name=\"TrailHeadType\" id=\"TrailHeadTypeId\">"
      "<SimpleField type=\"string\" name=\"TrailHeadName\">"
      "<displayName><![CDATA[<b>Trail Head Name</b>]]></displayName>"
      "</SimpleField>"
      "<SimpleField type=\"double\" name=\"TrailLength\">"
      "<displayName><![CDATA[<i>The length in miles</i>]]></displayName>"
      "</SimpleField>"
      "<SimpleField type=\"int\" name=\"ElevationGain\">"
      "<displayName><![CDATA[<i>change in altitude</i>]]></displayName>"
      "</SimpleField>"
      "</Schema>"
      "<Placemark id=\"foo\" targetId=\"bar\">"
      "<name>__NAME__</name>"
      "<address>__ADDRESS__</address>"
      "<Snippet>__SNIPPET__</Snippet>"
      "<description>__DESCRIPTION__</description>"
      "<Style>"
      "<BalloonStyle>"
      "<text><![CDATA["
      "<b>$[name]</b><br/><br/>"
      "<i>$[description]<i><br/><br/>"
      "The $[TrailHeadType/TrailHeadName/displayName] is"
      "<i>$[TrailHeadType/TrailHeadName]</i><br/>"
      "The trail is $[TrailHeadType/TrailLength] miles<br/>"
      "The climb is $[TrailHeadType/ElevationGain] meters<br/><br/>"
      "$[geDirections]"  // TODO.
      "]]></text>"
      "</BalloonStyle>"
      "</Style>"
      "<ExtendedData>"
      "<Data name=\"holeNumber\">"
      "<value>1</value>"
      "</Data>"
      "<Data name=\"holeYardage\">"
      "<value>234</value>"
      "</Data>"
      "<SchemaData schemaUrl=\"#TrailHeadTypeId\">"
      "<SimpleData name=\"TrailHeadName\">Mount Everest</SimpleData>"
      "<SimpleData name=\"TrailLength\">347.45</SimpleData>"
      "<SimpleData name=\"ElevationGain\">10000</SimpleData>"
      "</SchemaData>"
      "</ExtendedData>"
      "<Point>"
      "<coordinates>-123.115786, 49.283323</coordinates>"
      "</Point>"
      "</Placemark>"
      "</Document>");

  // The resultant balloon text from the above KML.
  const std::string kKitchenSinkBalloonText(
      "<b>__NAME__</b><br/><br/>"
      "<i>__DESCRIPTION__<i><br/><br/>"
      "The <b>Trail Head Name</b> is"
      "<i>Mount Everest</i><br/>"
      "The trail is 347.45 miles<br/>"
      "The climb is 10000 meters<br/><br/>"
      "$[geDirections]");  // TODO: geDirections.

  // Parse the file into a KmlFile object.
  kml_file_ = KmlFile::CreateFromParse(kKitchenSinkKml, NULL);

  // Extract the Placemark.
  const DocumentPtr doc = kmldom::AsDocument(kml_file_->root());
  const PlacemarkPtr placemark =
      kmldom::AsPlacemark(doc->get_feature_array_at(0));

  // Verify that that balloon text has been composited as expected.
  CPPUNIT_ASSERT_EQUAL(kKitchenSinkBalloonText,
                       CreateBalloonText(kml_file_, placemark));
}

// A struct of the Feature ID from which we'll pull the KML content, and the
// expected balloon text.
const static struct {
  const char* featureid;
  const char* balloontext;
} kFeatures[] = {
  {
  "empty-placemark",
  NULL
  },
  {
  "empty-name",
  "<h3></h3><br/><br/>"
  },
  {
  "empty-description",
  NULL
  },
  {
  "empty-name-description",
  "<h3></h3><br/><br/>"
  },
  {
  "name",
  "<h3>named</h3><br/><br/>"
  },
  {
  "name-description",
  "<h3>named</h3><br/><br/>described"
  },
  {
  "name-description-entity",
  // TODO: the one br is kind of odd...
  "<h3>named</h3><br/><br/>name is named<br />id is name-description-entity"
  },
  {
  "simple-balloon-style",
  // TODO: same case as above
  "name is named<br />id is simple-balloon-style"
  },
  {
  "extendeddata-data",
  "to be or not to be that is the question"
  },
  {
  "extendeddata-schemadata",
  "She was called Balloon Girl and she had just turned 23"
  }
};

void FeatureBalloonTest::TestAllBalloonsFile() {
  const std::string kBalloonKml = std::string(DATADIR) +
                                  "/balloon/all-balloons.kml";
  std::string data;
  std::string errors;
  CPPUNIT_ASSERT(kmlbase::File::ReadFileToString(kBalloonKml, &data));
  kml_file_ = KmlFile::CreateFromParse(data, &errors);
  CPPUNIT_ASSERT(!data.empty());
  CPPUNIT_ASSERT(errors.empty());
  CPPUNIT_ASSERT(kml_file_);
  for (size_t i = 0; i < sizeof(kFeatures)/sizeof(kFeatures[0]); ++i) {
    // Extract the Placemark.
    const FeaturePtr feature=
        kmldom::AsFeature(kml_file_->GetObjectById(kFeatures[i].featureid));
    // Verify that that balloon text has been composited as expected.
    const std::string kActual(CreateBalloonText(kml_file_, feature));
    if (kFeatures[i].balloontext == NULL) {
      CPPUNIT_ASSERT(kActual.empty());
      CPPUNIT_ASSERT_EQUAL(std::string(""), kActual);
    } else {
      CPPUNIT_ASSERT_EQUAL(static_cast<std::string>(kFeatures[i].balloontext),
                           kActual);
    }
  }
}

}  // end namespace kmlengine

TEST_MAIN
