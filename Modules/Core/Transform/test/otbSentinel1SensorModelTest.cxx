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
  BOOST_TEST_REQUIRE( framework::master_test_suite().argc == 3 );

  const std::string annotationXml = framework::master_test_suite().argv[1];
  const std::string baselineXml = framework::master_test_suite().argv[2];

  auto imi = otb::Sentinel1ImageMetadataInterface::New();
  otb::XMLMetadataSupplier mds(annotationXml);
  otb::XMLMetadataSupplier baseline(baselineXml);

  const auto & productType = mds.GetAs<std::string>("product.adsHeader.productType");

  otb::SARParam sarParam;
  otb::Projection::GCPParam gcpParam;

  imi->ReadSarParamAndGCPs(mds, sarParam, gcpParam);
  auto numberOfBursts = sarParam.burstRecords.size();

  std::cout << "OTB Tests" << std::endl;

  std::cout << "Deburst with OTB" << std::endl;
  std::vector<std::pair<unsigned long, unsigned long> > deburstLines;
  std::pair<unsigned long, unsigned long> deburstSamples;

  {
  otb::SarSensorModel model(productType, sarParam, gcpParam);
  // Deburst returns false when the number of burst is 1
  BOOST_TEST(model.Deburst(deburstLines, deburstSamples) == (numberOfBursts > 1));
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
  // BurstExtraction returns false when the number of burst is 1
  BOOST_TEST(model.BurstExtraction(burstIndex, burstExtractionLines, burstExtractionSamples, allPixel) 
              == (numberOfBursts > 1));
  }


  std::cout << "Deburst and concatenate" << std::endl;
  std::vector<std::pair<unsigned long,unsigned long> > linesBursts;
  std::vector<std::pair<unsigned long,unsigned long> > samplesBursts;
  unsigned int linesOffset = 0;
  unsigned int first_burstInd = 3;
  bool inputWithInvalidPixels = false;

  {
  otb::SarSensorModel model(productType, sarParam, gcpParam);
  // DeburstAndConcatenate returns false when the number of burst is 1
  BOOST_TEST(model.DeburstAndConcatenate(linesBursts, samplesBursts, linesOffset, first_burstInd, inputWithInvalidPixels)
      == (numberOfBursts > 1) );
  }

  PrintVectorOfPair(linesBursts, "DeburstAndConcatenate: LinesBursts");
  PrintVectorOfPair(samplesBursts, "DeburstAndConcatenate: SamplesBursts");

  std::pair<unsigned long, unsigned long> deburstSamplesOssim;
  deburstSamplesOssim.first = baseline.GetAs<unsigned long>("baseline.deburstSamplesOssim.first");
  deburstSamplesOssim.second = baseline.GetAs<unsigned long>("baseline.deburstSamplesOssim.second");
  BOOST_CHECK(deburstSamples == deburstSamplesOssim);

  std::vector<std::pair<unsigned long,unsigned long> > deburstLinesOssim;
  int deburstLinesCount = baseline.GetAs<int>("baseline.deburstLinesOssim.count");
  std::ostringstream oss;
  for (int listId = 1 ; listId <= deburstLinesCount ; ++listId)
  {
    oss.str("");
    oss << listId;
    std::string path_root = "baseline.deburstLinesOssim.deburstLine_" + oss.str();
    std::pair<unsigned long, unsigned long> deburstLinesPair;
    deburstLinesPair.first = baseline.GetAs<unsigned long>(path_root + ".first");
    deburstLinesPair.second = baseline.GetAs<unsigned long>(path_root + ".second");
    deburstLinesOssim.push_back(deburstLinesPair);
  }
  BOOST_CHECK(deburstLines.size() == deburstLinesOssim.size()
              && std::equal(deburstLines.begin(), deburstLines.end(), deburstLinesOssim.begin()));
//  BOOST_TEST(sensor2->burstExtraction(burstIndex, ossimBurstExtractionLines, ossimBurstExtractionSamples, allPixel)
//      == (numberOfBursts > 1));

  std::cout << "OTB : burst extraction lines " << burstExtractionLines.first << std::endl;

  std::cout << "OTB : burst extraction samples " << burstExtractionSamples.first << std::endl;

//  BOOST_CHECK(burstExtractionLines == ossimBurstExtractionLines);
//  BOOST_CHECK(burstExtractionSamples == ossimBurstExtractionSamples);
//  BOOST_TEST(sensor3->deburstAndConcatenate(ossimLinesBursts, ossimSamplesBursts, ossimLinesOffset, first_burstInd, inputWithInvalidPixels)
//              == (numberOfBursts > 1));

//  BOOST_CHECK(linesBursts == ossimLinesBursts);
//  BOOST_CHECK(samplesBursts == ossimSamplesBursts);
//  BOOST_CHECK(linesOffset == ossimLinesOffset);

}

BOOST_AUTO_TEST_CASE(Sentinel1SensorModel_auto_validate_inverse_transform)
{
  BOOST_TEST_REQUIRE( framework::master_test_suite().argc == 3 );

  const std::string annotationXml = framework::master_test_suite().argv[1];

  auto imi = otb::Sentinel1ImageMetadataInterface::New();
  otb::XMLMetadataSupplier mds(annotationXml);

  const auto & productType = mds.GetAs<std::string>("product.adsHeader.productType");

  otb::SARParam sarParam;
  otb::Projection::GCPParam gcpParam;

  imi->ReadSarParamAndGCPs(mds, sarParam, gcpParam);

  otb::SarSensorModel model(productType, sarParam, gcpParam);

  for (const auto & gcp : gcpParam.GCPs)
  {
    itk::Point<double, 3> geoPoint;
    geoPoint[0] = gcp.m_GCPX;
    geoPoint[1] = gcp.m_GCPY;
    geoPoint[2] = gcp.m_GCPZ;

    itk::Point<double, 2> lineSampleBaseline;
    lineSampleBaseline[0] = gcp.m_GCPCol;
    lineSampleBaseline[1] = gcp.m_GCPRow;

    itk::Point<double, 2> lineSample;

    model.WorldToLineSample(geoPoint, lineSample);

    const double lineTol = 1.;
    const double sampleTol = 1.;

    BOOST_TEST(std::abs(lineSample[0] - lineSampleBaseline[0]) < lineTol); 
    BOOST_TEST(std::abs(lineSample[1] - lineSampleBaseline[1]) < sampleTol); 
  }
}

BOOST_AUTO_TEST_CASE(Sentinel1SensorModel_auto_validate_forward_transform)
{
  double sqResTol = 25;
  const std::string annotationXml = framework::master_test_suite().argv[1];

  auto imi = otb::Sentinel1ImageMetadataInterface::New();
  otb::XMLMetadataSupplier mds(annotationXml);

  const auto & productType = mds.GetAs<std::string>("product.adsHeader.productType");

  otb::SARParam sarParam;
  otb::Projection::GCPParam gcpParam;

  imi->ReadSarParamAndGCPs(mds, sarParam, gcpParam);

  std::vector<otb::GCP> testGCPs;
  otb::Projection::GCPParam productGCPs;

  bool odd = false;

  for (auto gcp: gcpParam.GCPs)
  {
    if (odd)
    {
      productGCPs.GCPs.push_back(gcp);
    }
    else
    {
      testGCPs.push_back(gcp);
    }
    odd = !odd;
  }

  otb::SarSensorModel model(productType, sarParam, productGCPs);

  for (const auto & gcp : testGCPs)
  {
    itk::Point<double, 2> sensorPoint;
    sensorPoint[0] = gcp.m_GCPCol;
    sensorPoint[1] = gcp.m_GCPRow;

    itk::Point<double, 3> geoPointBaseline;
    geoPointBaseline[0] = gcp.m_GCPX;
    geoPointBaseline[1] = gcp.m_GCPY;
    geoPointBaseline[2] = gcp.m_GCPZ;

    itk::Point<double, 3> geoPoint;
    model.LineSampleHeightToWorld(sensorPoint, gcp.m_GCPZ, geoPoint);
    BOOST_TEST(geoPoint.SquaredEuclideanDistanceTo(geoPointBaseline) < sqResTol);
  }

}
