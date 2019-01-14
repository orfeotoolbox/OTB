/*
 * Copyright (C) 2005-2019 by Centre National d'Etudes Spatiales (CNES)
 *
 * This file is licensed under MIT license:
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#include <RadarSat/Leader/LeaderFactory.h>

#include <RadarSat/CommonRecord/DataHistogramSignalData.h>
#include <RadarSat/CommonRecord/DataQuality.h>
#include <RadarSat/CommonRecord/DataSetSummary.h>
#include <RadarSat/CommonRecord/FileDescriptor.h>
#include <RadarSat/Leader/DataHistogramProcessedData.h>
#include <RadarSat/CommonRecord/ProcessingParameters.h>
#include <RadarSat/Leader/PlatformPositionData.h>
#include <RadarSat/CommonRecord/AttitudeData.h>
#include <RadarSat/CommonRecord/RadiometricData.h>
#include <RadarSat/CommonRecord/RadiometricCompensationData.h>

namespace ossimplugins
{
LeaderFactory::LeaderFactory()
{
	RegisterRecord(9, new RadiometricData());
	RegisterRecord(10, new RadiometricCompensationData());
	RegisterRecord(8, new AttitudeData());
	RegisterRecord(7, new PlatformPositionData());
	RegisterRecord(6, new ProcessingParameters());
	RegisterRecord(5, new DataHistogramProcessedData());
	RegisterRecord(4, new DataHistogramSignalData());
	RegisterRecord(3, new DataQuality());
	RegisterRecord(2, new DataSetSummary());
	RegisterRecord(1, new FileDescriptor());
}

LeaderFactory::~LeaderFactory()
{

  std::map<int, RadarSatRecord*>::iterator i = _availableRecords.begin();

  while (i != _availableRecords.end())
  {
     if ( (*i).second )
     {
        delete (*i).second;
        (*i).second = 0;
     }
     ++i;
  }
  _availableRecords.clear();

}
}
