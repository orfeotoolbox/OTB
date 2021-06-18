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

template<class T>
void PrintVectorOfPair(T input, const std::string & name)
{
  std::cout << name << ": ";
  for (const auto & elem: input)
  {
    std::cout << "[" << elem.first << "," << elem.second << "] ";
  }
  std::cout << std::endl;
}




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



  std::cout << "OTB Tests" << std::endl;

  std::cout << "Deburst with OTB" << std::endl;
  std::vector<std::pair<unsigned long, unsigned long> > deburstLines;
  std::pair<unsigned long, unsigned long> deburstSamples;

  {
  otb::SarSensorModel model(productType, sarParam, gcpParam);
  BOOST_TEST(model.Deburst(deburstLines, deburstSamples));
  }

  PrintVectorOfPair(deburstLines, "Deburst lines");

  std::cout << "Deburst Samples: " << deburstSamples.first << " " << deburstSamples.second << std::endl;
  

  std::cout << "Burst extraction" << std::endl;
  unsigned int burstIndex = 3;
  std::pair<unsigned long, unsigned long> burstExtractionLines;
  std::pair<unsigned long, unsigned long> burstExtractionSamples;

  bool allPixel = false;
  {
  otb::SarSensorModel model(productType, sarParam, gcpParam);
  BOOST_TEST(model.BurstExtraction(burstIndex, burstExtractionLines, burstExtractionSamples, allPixel));
  }


  std::cout << "Deburst and concatenate" << std::endl;
  std::vector<std::pair<unsigned long,unsigned long> > linesBursts;
  std::vector<std::pair<unsigned long,unsigned long> > samplesBursts;
  unsigned int linesOffset;
  unsigned int first_burstInd = 3;
  bool inputWithInvalidPixels = false;

  {
  otb::SarSensorModel model(productType, sarParam, gcpParam);
  BOOST_TEST(model.DeburstAndConcatenate(linesBursts, samplesBursts, linesOffset, first_burstInd, inputWithInvalidPixels));
  }

  PrintVectorOfPair(linesBursts, "DeburstAndConcatenate: LinesBursts");
  PrintVectorOfPair(samplesBursts, "DeburstAndConcatenate: SamplesBursts");
  std::cout << "DeburstAndConcatenate: lines Offset: " << linesOffset << std::endl;

  // the code below uses OssimPlugins to validate the results OTB (see ossimSentinel1ModelTest).
  // TODO: generate baselines instead.

  std::cout << "Ossim Tests" << std::endl;
  std::auto_ptr<ossimProjection> projection
         (ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(annotationXml, 42));
  if (!projection.get()) 
  {
    throw std::runtime_error("Cannot read annotation file ("+annotationXml+"). Cannot create a projection from it.");
  }

  auto * sensor = dynamic_cast<ossimplugins::ossimSentinel1Model*>(projection.get());
  if (!sensor) 
  {
    throw std::runtime_error(
      "Unlike Expectations, the annotation file ("+annotationXml+") is not a Sentinel Annotation File");
  }

  ossimKeywordlist kwl;
  std::vector<std::pair<unsigned long, unsigned long> > deburstLinesOssim;
  std::pair<unsigned long, unsigned long> deburstSamplesOssim;

  std::cout<<"Trying to deburst data ..."<<std::endl;
  bool deburstOk = sensor->deburst(deburstLinesOssim, deburstSamplesOssim);
  std::cout<<"Deburst succeed: "<<(deburstOk?"yes":"no")<<std::endl;


  PrintVectorOfPair(deburstLinesOssim, "deburstLinesOssim");
  std::cout << deburstSamplesOssim.first << " " << deburstSamplesOssim.second << std::endl;

  BOOST_CHECK(deburstSamples == deburstSamplesOssim);
  BOOST_CHECK(deburstLines.size() == deburstLinesOssim.size() 
              && std::equal(deburstLines.begin(), deburstLines.end(), deburstLinesOssim.begin()));


  std::cout << "Burst extraction" << std::endl;


  std::auto_ptr<ossimProjection> projection2
         (ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(annotationXml, 42));
  auto * sensor2 = dynamic_cast<ossimplugins::ossimSentinel1Model*>(projection2.get());
  if (!sensor2) 
  {
    throw std::runtime_error(
      "Unlike Expectations, the annotation file ("+annotationXml+") is not a Sentinel Annotation File");
  }
  std::pair<unsigned long, unsigned long> ossimBurstExtractionLines;
  std::pair<unsigned long, unsigned long> ossimBurstExtractionSamples;
  BOOST_TEST(sensor2->burstExtraction(burstIndex, ossimBurstExtractionLines, ossimBurstExtractionSamples, allPixel));

  std::cout << "OTB : burst extraction lines " << burstExtractionLines.first << " " 
                                                 << burstExtractionLines.second << std::endl;

  std::cout << "OTB : burst extraction samples " << burstExtractionSamples.first << " " 
                                                 << burstExtractionSamples.second << std::endl;

  std::cout << "OSSIM : burst extraction lines " << ossimBurstExtractionLines.first << " " 
                                                 << ossimBurstExtractionLines.second << std::endl;

  std::cout << "OSSIM : burst extraction samples " << ossimBurstExtractionSamples.first << " " 
                                                 << ossimBurstExtractionSamples.second << std::endl;

  BOOST_CHECK(burstExtractionLines == ossimBurstExtractionLines);
  BOOST_CHECK(burstExtractionSamples == ossimBurstExtractionSamples);

  std::auto_ptr<ossimProjection> projection3
         (ossimplugins::ossimPluginProjectionFactory::instance()->createProjection(annotationXml, 42));
  auto * sensor3 = dynamic_cast<ossimplugins::ossimSentinel1Model*>(projection3.get());
  if (!sensor3) 
  {
    throw std::runtime_error(
      "Unlike Expectations, the annotation file ("+annotationXml+") is not a Sentinel Annotation File");
  }

  std::cout << "Deburst and concatenate" << std::endl;

  unsigned int ossimLinesOffset;

  std::vector<std::pair<unsigned long,unsigned long> > ossimLinesBursts;
  std::vector<std::pair<unsigned long,unsigned long> > ossimSamplesBursts;
  BOOST_TEST(sensor3->deburstAndConcatenate(ossimLinesBursts, ossimSamplesBursts, ossimLinesOffset, first_burstInd, inputWithInvalidPixels));

  PrintVectorOfPair(ossimLinesBursts, "DeburstAndConcatenate: ossimLinesBursts");
  PrintVectorOfPair(ossimSamplesBursts, "DeburstAndConcatenate: ossimSamplesBursts");
  std::cout << "DeburstAndConcatenate: lines Offset ossim: " << linesOffset << std::endl;

  BOOST_CHECK(linesBursts == ossimLinesBursts);
  BOOST_CHECK(samplesBursts == ossimSamplesBursts);
  BOOST_CHECK(linesOffset == ossimLinesOffset);
}