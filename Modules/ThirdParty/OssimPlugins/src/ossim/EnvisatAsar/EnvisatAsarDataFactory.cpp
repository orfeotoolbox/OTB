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


#include <EnvisatAsar/EnvisatAsarDataFactory.h>
#include <EnvisatAsar/EnvisatAsarData.h>

#include <EnvisatAsar/sph.h>
#include <EnvisatAsar/mph.h>
#include <EnvisatAsar/SQ_ADSR.h>
#include <EnvisatAsar/MainProcessingParameters.h>
#include <EnvisatAsar/SRGRConversionParameters.h>
#include <EnvisatAsar/DopplerCentroidParameters.h>
#include <EnvisatAsar/ChirpParameters.h>
#include <EnvisatAsar/AntennaElevationPatterns.h>
#include <EnvisatAsar/GeolocationGrid.h>

namespace ossimplugins
{

EnvisatAsarDataFactory::EnvisatAsarDataFactory()
{
	RegisterRecord("MPH"							, new mph());
	RegisterRecord("SPH"							, new sph());
	RegisterRecord("MDS1 SQ ADS                 "	, new SQ_ADSR());
	RegisterRecord("MDS2 SQ ADS                 "	, new SQ_ADSR());
	RegisterRecord("MAIN PROCESSING PARAMS ADS  "	, new MainProcessingParameters());
	RegisterRecord("DOP CENTROID COEFFS ADS     "	, new DopplerCentroidParameters());
	RegisterRecord("SR GR ADS                   "	, new SRGRConversionParameters());
	RegisterRecord("CHIRP PARAMS ADS            "	, new ChirpParameters());
	RegisterRecord("MDS1 ANTENNA ELEV PATT ADS  "	, new AntennaElevationPatterns());
	RegisterRecord("MDS2 ANTENNA ELEV PATT ADS  "	, new AntennaElevationPatterns());
	RegisterRecord("GEOLOCATION GRID ADS        "	, new GeolocationGrid());
}

EnvisatAsarDataFactory::~EnvisatAsarDataFactory()
{

}

}

