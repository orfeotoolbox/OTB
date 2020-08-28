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

#include "otbImageMetadata.h"
#include <iostream>
#include <fstream>
#include "boost/optional.hpp"
#include <bitset>
#include <utility>
#include <cstdlib>
#include "otbStopwatch.h"
#include "otbTestTools.h"

void SetUpImageMetadata(otb::ImageMetadata& md, unsigned int nbBands)
{
  using namespace otb;
  std::ostringstream oss;

  MetaData::Time mytime = Utils::LexicalCast<MetaData::Time,std::string>(std::string("2009-08-10T10:30:08.142149Z"), std::string("T"));

  md.Add(MDStr::SensorID, "PHR");
  md.Add(MDGeom::ProjectionWKT, std::string("UTM projRef"));
  md.Add(MDTime::ProductionDate, mytime);
  md.Add(std::string("Comment"), std::string("Test Extrakeys"));

  MetaDataKey::VariableLengthVectorType PhysicalGain(nbBands);

  for(unsigned int bandId = 0 ; bandId < nbBands ; bandId++)
  {
    ImageMetadataBase bmd;
    oss.str("");
    oss << "B" << bandId;
    bmd.Add(MDStr::BandName, oss.str());
    bmd.Add(MDNum::PhysicalBias,  bandId + 1.0);
    bmd.Add(MDNum::NoData, -10000.0);
    md.Bands.push_back(bmd);
    PhysicalGain.SetElement(bandId, bandId + 2.0);
  }
  md.Add(MDNum::PhysicalGain , PhysicalGain);
}

void otbMetadataKeyTest(char* argv[])
{
  using namespace otb;
  const char*   outFileName = argv[2];
  std::ofstream outfile(outFileName);

  MetaData::Time mytime;

  std::string bufferStr("2009-08-10T10:30:08.142149Z");

  try
  {
    mytime = Utils::LexicalCast<MetaData::Time,std::string>(bufferStr, std::string("T"));
  }
  catch(std::runtime_error&)
  {
    outfile << "Bad Utils::LexicalCast into MetaData::Time\n";
  }

  outfile << "mytime : "<< mytime << "\n";

  MDNum someKey = static_cast<MDNum>(3);
  if (someKey == MDNum::PhysicalGain)
  {
    outfile << "Found physical gain\n";
  }
  
  struct FirstTry
  {
    std::array< boost::optional<double>, static_cast<int>(MDNum::END) > NumKeys;
  };

  struct SecondTry
  {
    std::bitset< static_cast<int>(MDNum::END) > NumFlag;
    std::array<double, static_cast<int>(MDNum::END) > NumKeys;
  };

  class ThirdTry
  {
    public:
    std::pair<
      std::bitset< static_cast<int>(MDNum::END) >,
      std::array<double, static_cast<int>(MDNum::END) > >NumKeys;
  };

  outfile << "First try size: "<<sizeof(FirstTry)<< "\n";
  outfile << "Second try size: "<<sizeof(SecondTry)<< "\n";
  outfile << "Third try size: "<<sizeof(ThirdTry)<< "\n";

  FirstTry firstStruct;
  SecondTry secondStruct;
  ThirdTry thirdStruct;

  int total = static_cast<int>(MDNum::END);
  int loops = 10000000;
  Stopwatch chrono;
  double val= 0.0;
  chrono.Start();
  for (int i=0 ; i< loops ; ++i)
  {
    int pos = rand() % total;
    if (firstStruct.NumKeys[pos] == boost::none)
    {
      firstStruct.NumKeys[pos] = val+1.0;
    }
    else
    {
      firstStruct.NumKeys[pos] = firstStruct.NumKeys[pos].get() + val;
    }
  }
  chrono.Stop();
  std::cout << "First try chrono : "<< chrono.GetElapsedMilliseconds() << "\n";

  val= 0.0;
  chrono.Restart();
  for (int i=0 ; i< loops ; ++i)
  {
    int pos = rand() % total;
    if (secondStruct.NumFlag[pos] == false)
    {
      secondStruct.NumKeys[pos] = val+1.0;
      secondStruct.NumFlag[pos] = true;
    }
    else
    {
      secondStruct.NumKeys[pos] = secondStruct.NumKeys[pos] + val;
    }
  }
  chrono.Stop();
  std::cout << "Second try chrono : "<< chrono.GetElapsedMilliseconds() << "\n";

  val= 0.0;
  chrono.Restart();
  for (int i=0 ; i< loops ; ++i)
  {
    int pos = rand() % total;
    if (thirdStruct.NumKeys.first[pos] == false)
    {
      thirdStruct.NumKeys.second[pos] = val+1.0;
      thirdStruct.NumKeys.first[pos] = true;
    }
    else
    {
      thirdStruct.NumKeys.second[pos] = thirdStruct.NumKeys.second[pos] + val;
    }
  }
  chrono.Stop();
  std::cout << "Third try chrono : "<< chrono.GetElapsedMilliseconds();
  outfile.close();
}

void otbImageMetadataSliceTest(char* argv[])
{
  using namespace otb;

  const char*   outFileName = argv[2];
  std::ofstream outfile(outFileName);

  ImageMetadata md;
  SetUpImageMetadata(md, 3);

  ImageMetadata md2 = md.slice(0, 1);
  otb::testtools::PrintMetadata(md2, outfile);
  outfile.close();
}

void otbImageMetadataAppendTest(char* argv[])
{
  using namespace otb;

  const char*   outFileName = argv[2];
  std::ofstream outfile(outFileName);

  ImageMetadata md;
  SetUpImageMetadata(md, 2);

  ImageMetadata md2;
  SetUpImageMetadata(md2, 3);

  md.append(md2);
  otb::testtools::PrintMetadata(md, outfile);
  outfile.close();
}

void otbImageMetadataMergeTest(char* argv[])
{
  using namespace otb;

  const char*   outFileName = argv[2];
  std::ofstream outfile(outFileName);

  ImageMetadata md;
  SetUpImageMetadata(md, 2);

  ImageMetadata md2;
  SetUpImageMetadata(md2, 3);

  // Modifies some fields to actually test something.
  // The case where metadatas are identical is trivial in Merge()
  
  md.Add(MDStr::SensorID, "SPOT");
  md2.Add(MDStr::Polarization, "Polarization");
  md2.Bands[0].Add(MDNum::PhysicalBias, 3.1);
  md2.Bands[1].Add(MDNum::SunAzimuth, 20);
  
  md.Merge(md2);
  
  otb::testtools::PrintMetadata(md, outfile);
  outfile.close();
}
void otbImageMetadataToFromKeywordlistTest(char* argv[])
{
  using namespace otb;

  const char*   outFileName = argv[2];
  std::ofstream outfile(outFileName);

  ImageMetadata md;
  SetUpImageMetadata(md, 3);
  md.Add(MDGeom::ProjectionEPSG, 4326);
  md.Add(MDGeom::ProjectionProj, std::string("+proj=longlat +datum=WGS84 +no_defs "));
  Projection::RPCParam rpcStruct;
  md.Add(MDGeom::RPC, rpcStruct);
  Projection::GCPParam gcpStruct;
  gcpStruct.GCPs.push_back(GCP());
  md.Add(MDGeom::GCP, gcpStruct);
  MetaData::LUT1D lut1d;
  lut1d.Axis[0].Size = 3;
  lut1d.Axis[0].Origin = 0.;
  lut1d.Axis[0].Spacing = 1.;
  std::vector<double>array({1.0, 2.0, 3.0});
  lut1d.Array = {1.0, 2.0, 3.0};
  md.Add(MDL1D::SpectralSensitivity, lut1d);

  ImageMetadata::KeywordlistVector kwlVect;
  md.AppendToKeywordlists(kwlVect);
  ImageMetadata md2;
  md2.FromKeywordlists(kwlVect);
  otb::testtools::PrintMetadata(md2, outfile);
  outfile.close();
}

void otbImageMetadataCompactTest(char* argv[])
{
  using namespace otb;

  const char*   outFileName = argv[2];
  std::ofstream outfile(outFileName);

  ImageMetadata md;
  md.compact();
  SetUpImageMetadata(md, 3);
  MetaData::Time mytime = Utils::LexicalCast<MetaData::Time,std::string>(std::string("2009-08-05T20:34:38.236478Z"), std::string("T"));
  for (auto& band : md.Bands)
  {
    band.Add(MDStr::Polarization, "Polarization");
    band.Add(std::string("Information"), std::string("Very important"));
    band.Add(MDTime::AcquisitionDate, mytime);
  }

  md.compact();
  otb::testtools::PrintMetadata(md, outfile);
  outfile.close();
}

int otbImageMetadataTest(int argc, char* argv[])
{
  if (argc < 2)
    return EXIT_FAILURE;

  std::string testName(argv[1]);
  if(testName == "otbMetadataKeyTest")
    otbMetadataKeyTest(argv);
  else if (testName == "otbImageMetadataSliceTest")
    otbImageMetadataSliceTest(argv);
  else if (testName == "otbImageMetadataAppendTest")
    otbImageMetadataAppendTest(argv);
  else if (testName == "otbImageMetadataMergeTest")
    otbImageMetadataMergeTest(argv);
  else if (testName == "otbImageMetadataToFromKeywordlistTest")
    otbImageMetadataToFromKeywordlistTest(argv);
  else if (testName == "otbImageMetadataCompactTest")
    otbImageMetadataCompactTest(argv);
  else
  {
    std::cout << "Unknown test name " << testName;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
