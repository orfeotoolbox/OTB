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
#include "boost/optional.hpp"
#include <bitset>
#include <utility>
#include <cstdlib>
#include "otbStopwatch.h"

int otbImageMetadataTest(int, char*[])
{
  using namespace otb;

  MetaData::Time mytime;
  int year, month;
  char buffer[] = "2009-08-10T10:30:08.142149Z";

  std::string bufferStr("2009-08-10T10:30:08.142149Z");
  
  try
    {
    mytime = boost::lexical_cast<MetaData::Time>(buffer);
    }
  catch(boost::bad_lexical_cast&)
    {
    std::cout << "Bad cast into MetaData::Time\n";
    }
  std::cout << "mytime : "<< mytime << "\n";

  try
    {
    mytime = Utils::LexicalCast<MetaData::Time,char*>(buffer, std::string("T"));
    mytime = Utils::LexicalCast<MetaData::Time,std::string>(bufferStr, std::string("T"));
    }
  catch(std::runtime_error&)
    {
    std::cout << "Bad Utils::LexicalCast into MetaData::Time\n";
    }

  std::cout << "mytime : "<< mytime << "\n";

  MDNum someKey;
  someKey = static_cast<MDNum>(3);
  if (someKey == MDNum::PhysicalGain)
    {
    std::cout << "Found physical gain\n";
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

  std::cout << "First try size: "<<sizeof(FirstTry)<< "\n";
  std::cout << "Second try size: "<<sizeof(SecondTry)<< "\n";
  std::cout << "Third try size: "<<sizeof(ThirdTry)<< "\n";

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
  std::cout << "Third try chrono : "<< chrono.GetElapsedMilliseconds() << "\n";

  ImageMetadata md;
  md.Add(MDStr::SensorID, "PHR");
  md.Add(MDGeom::ProjectionWKT, std::string("UTM projRef"));

  ImageMetadataBase bmd;
  bmd.Add(MDStr::BandName, "B3");
  bmd.Add(MDNum::PhysicalGain , 2.0);
  bmd.Add(MDNum::PhysicalBias,  1.0);
  md.Bands.push_back(bmd);

  bmd.Add(MDStr::BandName, "B2");
  bmd.Add(MDNum::PhysicalGain , 3.0);
  bmd.Add(MDNum::PhysicalBias,  2.0);
  md.Bands.push_back(bmd);

  bmd.Add(MDStr::BandName, "B1");
  bmd.Add(MDNum::NoData, -10000.0);
  bmd.Add(MDNum::PhysicalGain , 4.0);
  bmd.Add(MDNum::PhysicalBias,  3.0);
  md.Bands.push_back(bmd);

  ImageMetadata md2;

  md2 = md;

  md.Add(MDGeom::ProjectionWKT, std::string("Lambert projRef"));
  //~ md.GeoTransform[5] = 1.0;

  std::cout << "md2: "<< md2 << "\n";
  
  return 0;
}
