//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------
// $Id$

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

