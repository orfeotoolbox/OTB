/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define BOOST_TEST_MODULE "SarSensorModel unit testing"
#define BOOST_TEST_DYN_LINK
#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Woverloaded-virtual"
#include <boost/test/unit_test.hpp>
#pragma GCC diagnostic pop
#else
#include <boost/test/unit_test.hpp>
#endif

#include "otbSarSensorModel.h"

#include "otbSentinel1ImageMetadataInterface.h"


#if defined(__GNUC__) || defined(__clang__)
# pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wunused-parameter"
#   pragma GCC diagnostic ignored "-Woverloaded-virtual"
#   pragma GCC diagnostic ignored "-Wshadow"
#include "ossimSentinel1Model.h"
#include "ossimPluginProjectionFactory.h"
# pragma GCC diagnostic pop
#else
#include "ossimSentinel1Model.h"
#include "ossimPluginProjectionFactory.h"
#endif


using namespace boost::unit_test;

BOOST_AUTO_TEST_CASE(SARDeburst)
{
  BOOST_TEST_REQUIRE( framework::master_test_suite().argc == 2 );

  const std::string annotationXml = framework::master_test_suite().argv[1];

  auto imi = otb::Sentinel1ImageMetadataInterface::New();
  otb::XMLMetadataSupplier mds(annotationXml);

  const auto & productType = mds.GetAs<std::string>("product.adsHeader.productType");

  otb::SARParam sarParam;
  otb::Projection::GCPParam gcpParam;

  imi->ReadSarParamAndGCPs(mds, sarParam, gcpParam);


  otb::SarSensorModel model(productType, sarParam, gcpParam);

  std::vector<std::pair<unsigned long, unsigned long> > deburstLines;
  std::pair<unsigned long, unsigned long> deburstSamples;

  std::cout << "Deburst with OTB" << std::endl;
  BOOST_TEST(model.Deburst(deburstLines, deburstSamples));

  std::cout << "Deburst lines: ";
  for (const auto & elem: deburstLines)
  {
    std::cout << "[" << elem.first << "," << elem.second << "] ";
  }
  std::cout << std::endl;
  std::cout << "Deburst Samples: " << deburstSamples.first << " " << deburstSamples.second << std::endl;


  // the code below uses OssimPlugins to validate the results OTB (see ossimSentinel1ModelTest).
  // TODO: generate baselines instead.
  std::cout << "Deburst with OSSIM" << std::endl;
  std::auto_ptr<ossimProjection> projection
         (ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(annotationXml, 42));
  if (!projection.get()) 
  {
    throw std::runtime_error("Cannot read annotation file ("+annotationXml+"). Cannot create a projection from it.");
  }

  ossimplugins::ossimSentinel1Model * sensor = dynamic_cast<ossimplugins::ossimSentinel1Model*>(projection.get());
  if (!sensor) 
  {
    throw std::runtime_error(
      "Unlike Expectations, the annotation file ("+annotationXml+") is not a Sentinel Annotation File");
  }

  ossimKeywordlist kwl;
  sensor->saveState(kwl, "S1.");
  sensor->loadState(kwl, "S1.");

  std::vector<std::pair<unsigned long, unsigned long> > deburstLinesOssim;
  std::pair<unsigned long, unsigned long> deburstSamplesOssim;

  std::cout<<"Trying to deburst data ..."<<std::endl;
  bool deburstOk = sensor->deburst(deburstLinesOssim, deburstSamplesOssim);
  std::cout<<"Deburst succeed: "<<(deburstOk?"yes":"no")<<std::endl;


  for (const auto & elem: deburstLinesOssim)
  {
    std::cout << "[" << elem.first << "," << elem.second << "] ";
  }
  std::cout << std::endl;
  std::cout << deburstSamplesOssim.first << " " << deburstSamplesOssim.second << std::endl;

  std::cout << "-------------------------------------------------" << std::endl;

  BOOST_CHECK(deburstSamples == deburstSamplesOssim);
  BOOST_CHECK(deburstLines.size() == deburstLinesOssim.size() 
              && std::equal(deburstLines.begin(), deburstLines.end(), deburstLinesOssim.begin()));


}