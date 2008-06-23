#include <ossim/imaging/EnvisatAsar/EnvisatAsarDataFactory.h>
#include <ossim/imaging/EnvisatAsar/EnvisatAsarData.h>

#include <ossim/imaging/EnvisatAsar/sph.h>
#include <ossim/imaging/EnvisatAsar/mph.h>
#include <ossim/imaging/EnvisatAsar/SQ_ADSR.h>
#include <ossim/imaging/EnvisatAsar/MainProcessingParameters.h>
#include <ossim/imaging/EnvisatAsar/SRGRConversionParameters.h>
#include <ossim/imaging/EnvisatAsar/DopplerCentroidParameters.h>
#include <ossim/imaging/EnvisatAsar/ChirpParameters.h>
#include <ossim/imaging/EnvisatAsar/AntennaElevationPatterns.h>
#include <ossim/imaging/EnvisatAsar/GeolocationGrid.h>


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