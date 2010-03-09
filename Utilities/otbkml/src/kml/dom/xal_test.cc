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

// This file contains the unit tests for the <xal:AddressDetails> elements.

#include "kml/dom/xal.h"
#include "kml/base/file.h"
#include "kml/dom/kml_cast.h"
#include "kml/dom/kml_factory.h"
#include "kml/dom/kml_funcs.h"
#include "gtest/gtest.h"

// The following define is a convenience for testing inside Google.
#ifdef GOOGLE_INTERNAL
#include "kml/base/google_internal_test.h"
#endif

#ifndef DATADIR
#error *** DATADIR must be defined! ***
#endif

using kmlbase::File;

namespace kmldom {

// <xal:AddressDetails>
class XalAddressDetailsTest : public testing::Test {
 protected:
  virtual void SetUp() {
    xaladdressdetails_ = KmlFactory::GetFactory()->CreateXalAddressDetails();
  }

  XalAddressDetailsPtr xaladdressdetails_;
};

TEST_F(XalAddressDetailsTest, TestType) {
  ASSERT_EQ(Type_XalAddressDetails, xaladdressdetails_->Type());
  ASSERT_TRUE(xaladdressdetails_->IsA(Type_XalAddressDetails));
}

TEST_F(XalAddressDetailsTest, TestDefault) {
  ASSERT_FALSE(xaladdressdetails_->has_country());
  ASSERT_FALSE(xaladdressdetails_->get_country());
  ASSERT_EQ(kmlbase::XMLNS_XAL, xaladdressdetails_->get_xmlns());
}

TEST_F(XalAddressDetailsTest, TestSetGetHasClear) {
  XalCountryPtr country(KmlFactory::GetFactory()->CreateXalCountry());
  xaladdressdetails_->set_country(country);
  ASSERT_TRUE(xaladdressdetails_->has_country());
  ASSERT_EQ(country, xaladdressdetails_->get_country());
  xaladdressdetails_->clear_country();
  ASSERT_FALSE(xaladdressdetails_->has_country());
  ASSERT_FALSE(xaladdressdetails_->get_country());
}

TEST_F(XalAddressDetailsTest, TestParseSerialize) {
  const string kAddressDetails(
      "<xal:AddressDetails>"
      "<xal:Country/>"
      "</xal:AddressDetails>");
  ASSERT_EQ(kAddressDetails,
            kmldom::SerializeRaw(kmldom::Parse(kAddressDetails, NULL)));
}

// This verifies that all expected elements in a test file parse and are
// accessible from the dom API.
TEST_F(XalAddressDetailsTest, TestParseDom) {
  string gaddr_content;
  const string kXalGaddr(
      File::JoinPaths(DATADIR, File::JoinPaths("xal", "gaddr.kml")));
  ASSERT_TRUE(File::ReadFileToString(kXalGaddr, &gaddr_content));
  ElementPtr root = kmldom::Parse(gaddr_content, NULL);
  ASSERT_TRUE(root);
  KmlPtr kml = AsKml(root);
  ASSERT_TRUE(kml);
  ASSERT_TRUE(kml->has_feature());
  DocumentPtr document = AsDocument(kml->get_feature());
  ASSERT_EQ(static_cast<size_t>(1), document->get_feature_array_size());
  PlacemarkPtr placemark = AsPlacemark(document->get_feature_array_at(0));
  ASSERT_TRUE(placemark);
  XalAddressDetailsPtr xaladdressdetails =
    AsXalAddressDetails(placemark->get_xaladdressdetails());
  ASSERT_TRUE(xaladdressdetails);
  ASSERT_TRUE(xaladdressdetails->has_country());
  XalCountryPtr country = xaladdressdetails->get_country();
  ASSERT_TRUE(country->has_countrynamecode());
  ASSERT_EQ(string("US"), country->get_countrynamecode());
  XalAdministrativeAreaPtr administrativearea =
      country->get_administrativearea();
  ASSERT_TRUE(administrativearea);
  ASSERT_EQ(string("CA"),
      administrativearea->get_administrativeareaname());
  XalSubAdministrativeAreaPtr subadministrativearea =
      administrativearea->get_subadministrativearea();
  ASSERT_TRUE(subadministrativearea);
  ASSERT_EQ(string("Santa Clara"),
            subadministrativearea->get_subadministrativeareaname());
  XalLocalityPtr locality = subadministrativearea->get_locality();
  ASSERT_TRUE(locality);
  ASSERT_EQ(string("Mountain View"), locality->get_localityname());
  XalThoroughfarePtr thoroughfare = locality->get_thoroughfare();
  ASSERT_TRUE(thoroughfare);
  ASSERT_EQ(string("Amphitheatre Pkwy"),
            thoroughfare->get_thoroughfarename());
  ASSERT_EQ(string("1600"),
            thoroughfare->get_thoroughfarenumber());
  XalPostalCodePtr postalcode = locality->get_postalcode();
  ASSERT_TRUE(postalcode);
  ASSERT_EQ(string("94043"), postalcode->get_postalcodenumber());
}

TEST_F(XalAddressDetailsTest, TestDomSerialize) {
  xaladdressdetails_->set_country(KmlFactory::GetFactory()->CreateXalCountry());
  ASSERT_EQ("<xal:AddressDetails><xal:Country/></xal:AddressDetails>",
            kmldom::SerializeRaw(xaladdressdetails_));
}

// <xal:AdministrativeArea>
class XalAdministrativeAreaTest : public testing::Test {
 protected:
  virtual void SetUp() {
    xaladministrativearea_ =
        KmlFactory::GetFactory()->CreateXalAdministrativeArea();
  }

  XalAdministrativeAreaPtr xaladministrativearea_;
};

TEST_F(XalAdministrativeAreaTest, TestType) {
  ASSERT_EQ(Type_XalAdministrativeArea, xaladministrativearea_->Type());
  ASSERT_TRUE(xaladministrativearea_->IsA(Type_XalAdministrativeArea));
}

TEST_F(XalAdministrativeAreaTest, TestDefault) {
  ASSERT_FALSE(xaladministrativearea_->has_administrativeareaname());
  ASSERT_TRUE(xaladministrativearea_->get_administrativeareaname().empty());
  ASSERT_FALSE(xaladministrativearea_->has_locality());
  ASSERT_FALSE(xaladministrativearea_->get_locality());
  ASSERT_FALSE(xaladministrativearea_->has_subadministrativearea());
  ASSERT_FALSE(xaladministrativearea_->get_subadministrativearea());
  ASSERT_EQ(kmlbase::XMLNS_XAL, xaladministrativearea_->get_xmlns());
}

// <xal:Country>
class XalCountryTest : public testing::Test {
 protected:
  virtual void SetUp() {
    xalcountry_ = KmlFactory::GetFactory()->CreateXalCountry();
  }

  XalCountryPtr xalcountry_;
};

TEST_F(XalCountryTest, TestType) {
  ASSERT_EQ(Type_XalCountry, xalcountry_->Type());
  ASSERT_TRUE(xalcountry_->IsA(Type_XalCountry));
}

TEST_F(XalCountryTest, TestDefault) {
  ASSERT_FALSE(xalcountry_->has_countrynamecode());
  ASSERT_FALSE(xalcountry_->has_administrativearea());
  ASSERT_TRUE(xalcountry_->get_countrynamecode().empty());
  ASSERT_FALSE(xalcountry_->get_administrativearea());
  ASSERT_EQ(kmlbase::XMLNS_XAL, xalcountry_->get_xmlns());
}

TEST_F(XalCountryTest, TestSetGetHasClear) {
  XalAdministrativeAreaPtr administrativearea(
      KmlFactory::GetFactory()->CreateXalAdministrativeArea());
  const string kCountryCodeName("US");
  xalcountry_->set_countrynamecode(kCountryCodeName);
  xalcountry_->set_administrativearea(administrativearea);
  ASSERT_TRUE(xalcountry_->has_countrynamecode());
  ASSERT_TRUE(xalcountry_->has_administrativearea());
  ASSERT_EQ(kCountryCodeName, xalcountry_->get_countrynamecode());
  ASSERT_EQ(administrativearea, xalcountry_->get_administrativearea());
  xalcountry_->clear_countrynamecode();
  xalcountry_->clear_administrativearea();
  ASSERT_FALSE(xalcountry_->has_countrynamecode());
  ASSERT_FALSE(xalcountry_->has_administrativearea());
  ASSERT_TRUE(xalcountry_->get_countrynamecode().empty());
  ASSERT_FALSE(xalcountry_->get_administrativearea());
}

TEST_F(XalCountryTest, TestParseSerialize) {
  const string kCountry(
      "<xal:Country>"
      "<xal:CountryNameCode>US</xal:CountryNameCode>"
      "<xal:AdministrativeArea/>"
      "</xal:Country>");
  ASSERT_EQ(kCountry,
            kmldom::SerializeRaw(kmldom::Parse(kCountry, NULL)));
}

// <xal:Locality>
class XalLocalityTest : public testing::Test {
 protected:
  virtual void SetUp() {
    locality_ = KmlFactory::GetFactory()->CreateXalLocality();
  }

  XalLocalityPtr locality_;
};

TEST_F(XalLocalityTest, TestType) {
  ASSERT_EQ(Type_XalLocality, locality_->Type());
  ASSERT_TRUE(locality_->IsA(Type_XalLocality));
}

TEST_F(XalLocalityTest, TestDefault) {
  ASSERT_FALSE(locality_->has_localityname());
  ASSERT_TRUE(locality_->get_localityname().empty());
  ASSERT_FALSE(locality_->has_thoroughfare());
  ASSERT_FALSE(locality_->get_thoroughfare());
  ASSERT_FALSE(locality_->has_postalcode());
  ASSERT_FALSE(locality_->get_postalcode());
  ASSERT_EQ(kmlbase::XMLNS_XAL, locality_->get_xmlns());
}

// <xal:PostalCode>
class XalPostalCodeTest : public testing::Test {
 protected:
  virtual void SetUp() {
    postalcode_ = KmlFactory::GetFactory()->CreateXalPostalCode();
  }

  XalPostalCodePtr postalcode_;
};

TEST_F(XalPostalCodeTest, TestType) {
  ASSERT_EQ(Type_XalPostalCode, postalcode_->Type());
  ASSERT_TRUE(postalcode_->IsA(Type_XalPostalCode));
}

TEST_F(XalPostalCodeTest, TestDefault) {
  ASSERT_FALSE(postalcode_->has_postalcodenumber());
  ASSERT_EQ(kmlbase::XMLNS_XAL, postalcode_->get_xmlns());
}

// <xal:SubAdministrativeArea>
class XalSubAdministrativeAreaTest : public testing::Test {
 protected:
  virtual void SetUp() {
    xalsubadministrativearea_ =
        KmlFactory::GetFactory()->CreateXalSubAdministrativeArea();
  }

  XalSubAdministrativeAreaPtr xalsubadministrativearea_;
};

TEST_F(XalSubAdministrativeAreaTest, TestType) {
  ASSERT_EQ(Type_XalSubAdministrativeArea, xalsubadministrativearea_->Type());
  ASSERT_TRUE(xalsubadministrativearea_->IsA(Type_XalSubAdministrativeArea));
}

TEST_F(XalSubAdministrativeAreaTest, TestDefault) {
  ASSERT_FALSE(xalsubadministrativearea_->has_subadministrativeareaname());
  ASSERT_TRUE(
      xalsubadministrativearea_->get_subadministrativeareaname().empty());
  ASSERT_FALSE(xalsubadministrativearea_->has_locality());
  ASSERT_FALSE(xalsubadministrativearea_->get_locality());
  ASSERT_EQ(kmlbase::XMLNS_XAL, xalsubadministrativearea_->get_xmlns());
}

// <xal:Thoroughfare>
class XalThoroughfareTest : public testing::Test {
 protected:
  virtual void SetUp() {
    thoroughfare_ = KmlFactory::GetFactory()->CreateXalThoroughfare();
  }

  XalThoroughfarePtr thoroughfare_;
};

TEST_F(XalThoroughfareTest, TestType) {
  ASSERT_EQ(Type_XalThoroughfare, thoroughfare_->Type());
  ASSERT_TRUE(thoroughfare_->IsA(Type_XalThoroughfare));
}

TEST_F(XalThoroughfareTest, TestDefault) {
  ASSERT_FALSE(thoroughfare_->has_thoroughfarename());
  ASSERT_TRUE(thoroughfare_->get_thoroughfarename().empty());
  ASSERT_FALSE(thoroughfare_->has_thoroughfarenumber());
  ASSERT_TRUE(thoroughfare_->get_thoroughfarenumber().empty());
  ASSERT_EQ(kmlbase::XMLNS_XAL, thoroughfare_->get_xmlns());
}

}  // namespace kmldom

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
