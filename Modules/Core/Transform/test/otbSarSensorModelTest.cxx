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
#include "otbImageFileReader.h"

//TODO: replace by baselines
#include "otbSarSensorModelAdapter.h"

using namespace boost::unit_test;


BOOST_AUTO_TEST_CASE(SARSensorModel_parameters)
{
  
  BOOST_TEST_REQUIRE( framework::master_test_suite().argc == 5 );
  //BOOST_TEST_MESSAGE( "'argv[0]' contains " << framework::master_test_suite().argv[0] );
  //BOOST_TEST_REQUIRE( framework::master_test_suite().argv[1] == "--input" );
  std::cout << "argv[1] " << framework::master_test_suite().argv[1] << std::endl;

}

BOOST_AUTO_TEST_CASE(SARSensorModel_WorldToLineSample)
{
  using ImageType = otb::Image<unsigned int, 2>;
  using ReaderType = otb::ImageFileReader<ImageType>;

  auto reader = ReaderType::New();
  reader->SetFileName(framework::master_test_suite().argv[1]);
  reader->GenerateOutputInformation();
  const auto & imd = reader->GetOutput()->GetImageMetadata();

  otb::SarSensorModel model(imd);

  otb::SarSensorModel::Point3DType inWorldPoint;

  inWorldPoint[0] = std::stod(framework::master_test_suite().argv[2]);
  inWorldPoint[1] = std::stod(framework::master_test_suite().argv[3]);
  inWorldPoint[2] = std::stod(framework::master_test_suite().argv[4]);

  otb::SarSensorModel::Point2DType outLineSampleOssim;

  std::cout << "test with ossim" << std::endl;
  auto ossimModel = otb::SarSensorModelAdapter::New();
  ossimModel->LoadState(reader->GetOutput()->GetImageKeywordlist());
  ossimModel->WorldToLineSample(inWorldPoint, outLineSampleOssim);

  std::cout << "test with otb" << std::endl;
  otb::SarSensorModel::Point2DType outLineSampleOtb;
  model.WorldToLineSample(inWorldPoint, outLineSampleOtb);

  //TODO compare results with a baseline

}


BOOST_AUTO_TEST_CASE(SARSensorModel_auto_validate_inverse_transform )
{
  double lineTol = 1.;
  double sampleTol = 1.;

  using ImageType = otb::Image<unsigned int, 2>;
  using ReaderType = otb::ImageFileReader<ImageType>;

  auto reader = ReaderType::New();
  reader->SetFileName(framework::master_test_suite().argv[1]);
  reader->GenerateOutputInformation();
  const auto & imd = reader->GetOutput()->GetImageMetadata();

  otb::SarSensorModel model(imd);

  for (const auto & gcp : imd.GetGCPParam().GCPs)
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

    BOOST_TEST(std::abs(lineSample[0] - lineSampleBaseline[0]) < lineTol); 
    BOOST_TEST(std::abs(lineSample[1] - lineSampleBaseline[1]) < sampleTol); 
  }

}