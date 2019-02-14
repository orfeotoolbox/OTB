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


#include "ossimCosmoSkymedModel.h"
#include "ossim/ossimTraceHelpers.h"
#include "ossim/ossimKeyWordListUtilities.h"
#include "ossim/ossimSarSensorModelPathsAndKeys.h"
#include <ossim/base/ossimDirectory.h>
#include <ossim/base/ossimString.h>
#include <iostream>
#include <cassert>

#include "otbStringUtils.h"

/* GDAL Libraries */
#include "gdal.h"
#include <gdal_priv.h>

// otbSystem.h
#include "otbSystem.h"
#include "itksys/SystemTools.hxx"

#if defined(_MSC_VER)
#include "ossimWin32FindFileHandle.h"
#endif

#if defined(USE_BOOST_TIME)
            using boost::posix_time::microseconds;
            using boost::posix_time::seconds;
#else
            using ossimplugins::time::microseconds;
            using ossimplugins::time::seconds;
#endif
namespace {// Anonymous namespace
   ossimTrace traceExec  ("ossimCosmoSkymedModel:exec");
   ossimTrace traceDebug ("ossimCosmoSkymedModel:debug");
   const ossimString attAdsHeader        = "adsHeader";
   const ossimString attAzimuthTime      = "azimuthTime";
   const ossimString attFirstValidSample = "firstValidSample";
   const ossimString attLastValidSample   = "lastValidSample";
   const ossimString attGr0              = "gr0";
   const ossimString attGrsrCoefficients = "grsrCoefficients";
   const ossimString attHeight           = "height";
   const ossimString attLatitude         = "latitude";
   const ossimString attLine             = "line";
   const ossimString attLongitude        = "longitude";
   const ossimString attPixel            = "pixel";
   const ossimString attPosition         = "position";
   const ossimString attSlantRangeTime   = "slantRangeTime";
   const ossimString attSr0              = "sr0";
   const ossimString attSrgrCoefficients = "srgrCoefficients";
   const ossimString attTime             = "time";
   const ossimString attVelocity         = "velocity";
   const ossimString attX                = "x";
   const ossimString attY                = "y";
   const ossimString attZ                = "z";

   // const char LOAD_FROM_PRODUCT_FILE_KW[] = "load_from_product_file_flag";
   // const char PRODUCT_XML_FILE_KW[] = "product_xml_filename";
}// Anonymous namespace

namespace ossimplugins
{
   RTTI_DEF1(ossimCosmoSkymedModel, "ossimCosmoSkymedModel", ossimSarSensorModel);

//*************************************************************************************************
// Constructor
//*************************************************************************************************
   ossimCosmoSkymedModel::ossimCosmoSkymedModel()
      : ossimSarSensorModel()
      , theSLC(false)
      , theTOPSAR(false)
   {
      // theManifestDoc = new ossimXmlDocument();
   }

   void ossimCosmoSkymedModel::clearFields()
   {
      theSLC    = false;
      theTOPSAR = false;
      theProductKwl.clear();
   }

//*************************************************************************************************
// Infamous DUP
//*************************************************************************************************
   ossimObject* ossimCosmoSkymedModel::dup() const
   {
      return new ossimCosmoSkymedModel(*this);
   }

//*************************************************************************************************
// Print
//*************************************************************************************************
   std::ostream& ossimCosmoSkymedModel::print(std::ostream& out) const
   {
      // Capture stream flags since we are going to mess with them.
      std::ios_base::fmtflags f = out.flags();

      out << "\nDump of ossimCosmoSkymedModel at address " << hex << this
          << dec
          << "\n------------------------------------------------"
          << "\n  theImageID            = " << theImageID
          << "\n  theImageSize          = " << theImageSize

          << "\n------------------------------------------------"
          << "\n  " << endl;

      // Set the flags back.
      out.flags(f);
      return ossimSarSensorModel::print(out);
   }

//*************************************************************************************************
// Save State
//*************************************************************************************************
   bool ossimCosmoSkymedModel::saveState(ossimKeywordlist& kwl,
                                      const char* prefix) const
   {
      static const char MODULE[] = "ossimplugins::ossimCosmoSkymedModel::saveState";
      SCOPED_LOG(traceDebug, MODULE);

      kwl.add(prefix,
              ossimKeywordNames::TYPE_KW,
              "ossimCosmoSkymedModel",
              true);

   
      kwl.add("support_data.",
              "calibration_lookup_flag",
              "false",
              false);

      kwl.addList(theProductKwl,  true);
      
      return ossimSarSensorModel::saveState(kwl, prefix);
   }


//*************************************************************************************************
// Load State
//*************************************************************************************************
   bool ossimCosmoSkymedModel::loadState(const ossimKeywordlist& kwl,
                                      const char* prefix)
   {
     return ossimSarSensorModel::loadState(kwl, prefix);
   }

//*************************************************************************************************
// open
//*************************************************************************************************
   bool ossimCosmoSkymedModel::open(const ossimFilename& file)
   {
     // Check extension (hdf5 expected)
     const ossimString ext = file.ext().downcase();
     if ( !file.exists() || (ext != "h5"))
       {
         return false;
       }
       
     // Check SubDatasets (For COSMO, we need //S01/SBI dataset)
     bool withSBI = false;
     GDALDataset * dataset_Global = static_cast<GDALDataset*>(GDALOpen(file.c_str(), GA_ReadOnly));

     char** papszMetadataG;
     papszMetadataG = dataset_Global->GetMetadata("SUBDATASETS");

     // Have we find some dataSet ?
     // This feature is supported only hdf5 file
     if ( (CSLCount(papszMetadataG) > 0) &&
	  (strcmp(dataset_Global->GetDriver()->GetDescription(),"HDF5") == 0))
       {
	 for (int cpt = 0; papszMetadataG[cpt] != nullptr; ++cpt)
	   {
	     std::string key, name;
	     if (otb::System::ParseHdfSubsetName(papszMetadataG[cpt], key, name))
	       {
		 // check if this is a dataset name
		 if (key.find("_NAME") != std::string::npos) 
		   {
		     std::size_t found = name.find("//S01/SBI");
		     if (found!=std::string::npos)
		       {
			 withSBI = true;
		       }
		   }
	       }
	   }
       }
     
     GDALClose(static_cast<GDALDatasetH>(dataset_Global));

     if (! withSBI)
       {
	 ossimNotify(ossimNotifyLevel_WARN) << 
	   "HDF5 file does not contain the expected subdataset (//S01/SBI)!\n";
	 return false;
       }

     // Read the expected Subdataset : ://S01/SBI
     std::string file_SBI = "HDF5:" + file + "://S01/SBI";

     bool ret = readSBI(file_SBI);
     
     if (!ret)
       {
	 ossimNotify(ossimNotifyLevel_WARN) << 
	   "Probleme with metadata extraction!\n";
	 return false;
       }
    
      return true;
   }

//*************************************************************************************************
// readSBI
//*************************************************************************************************
  bool ossimCosmoSkymedModel::readSBI(std::string fileSBI)
  {
    static const char MODULE[] = "ossimplugins::ossimCosmoSkymedModel::open";
    SCOPED_LOG(traceDebug, MODULE);
    
    // Create GDALImageIO to retrieve all metadata (from .h5 input file) 
    std::map<std::string, std::string> metadataDataSet;
    std::vector<std::map<std::string, std::string> > metadataBands;
    
    GDALDataset * dataset = static_cast<GDALDataset*>(GDALOpen(fileSBI.c_str(), GA_ReadOnly));

    // Metadata for dataset
    char** papszMetadata = dataset->GetMetadata(nullptr);
    for (int cpt = 0; papszMetadata[cpt] != nullptr; ++cpt)
      {
	std::string key, value;
	if (otb::System::ParseHdfSubsetName(papszMetadata[cpt], key, value))
	  {
	    metadataDataSet[key] = value;
	  }
      }

    int nbRasterCount = dataset->GetRasterCount();

    // Metadata for each Band
    for (int iBand = 0; iBand < dataset->GetRasterCount(); iBand++)
      {
	std::map<std::string, std::string> mapCurrentBand;

	GDALRasterBand * Band = dataset->GetRasterBand(iBand + 1);

	papszMetadata = Band->GetMetadata(nullptr);
	for (int cpt = 0; papszMetadata[cpt] != nullptr; ++cpt)
	  {
	    std::string key, value;
	    if (otb::System::ParseHdfSubsetName(papszMetadata[cpt], key, value))
	      {
		mapCurrentBand[key] = value;
	      }
	  }
      
	metadataBands.push_back(mapCurrentBand);
      }

    GDALClose(static_cast<GDALDatasetH>(dataset));

    // Check Mission Id, acquisition mode and porduct type
    if(! (metadataDataSet["Mission_ID"] == "CSK" )) 
      {
	ossimNotify(ossimNotifyLevel_WARN)
	  << "Not a valid missionId : '"
	  << metadataDataSet["Mission_ID"] << "'\n" ;
	return false;
      }
    
    if( (metadataDataSet["Acquisition_Mode"] != "HIMAGE") &&
        (metadataDataSet["Acquisition_Mode"] != "SPOTLIGHT") && 
	(metadataDataSet["Acquisition_Mode"] != "ENHANCED SPOTLIGHT")) 
      {
	ossimNotify(ossimNotifyLevel_WARN)
	  << "Not an expected acquisition mode (only HIMAGE and SPOTLIGHT expected)" << "'\n" ;
	return false;
      }

    if( (metadataDataSet["Product_Type"] != "SCS_B")) 
      {
	ossimNotify(ossimNotifyLevel_WARN)
	  << "Not an expected product type (only SCS_B expected)" << "'\n" ;
	return false;
      }

    ////////////////// Add General Parameters ////////////////
    add(theProductKwl, "sensor", "CSK"); 
    add(theProductKwl, "sample_type", "COMPLEX");

    add(theProductKwl, HEADER_PREFIX, "polarisation", metadataDataSet["S01_Polarisation"]);
    add(theProductKwl, HEADER_PREFIX, "annotation");
    add(theProductKwl, HEADER_PREFIX, "swath", "S1");
    add(theProductKwl, HEADER_PREFIX, "image_id", std::stoi(metadataDataSet["Programmed_Image_ID"]));
    add(theProductKwl, HEADER_PREFIX, "meters_per_pixel_x", 
	std::stod(metadataBands[0]["S01_SBI_Column_Spacing"]));
    add(theProductKwl, HEADER_PREFIX, "meters_per_pixel_y",  
	std::stod(metadataBands[0]["S01_SBI_Line_Spacing"]));
    add(theProductKwl, HEADER_PREFIX, "version", thePluginVersion);

    add(theProductKwl, SUPPORT_DATA_PREFIX, "product_type", metadataDataSet["Product_Type"]);
    add(theProductKwl, SUPPORT_DATA_PREFIX, "swath", "S1");
    add(theProductKwl, SUPPORT_DATA_PREFIX, "slice_num", "1");
    add(theProductKwl, SUPPORT_DATA_PREFIX, "orbit_pass", metadataDataSet["Orbit_Direction"]);
    add(theProductKwl, SUPPORT_DATA_PREFIX, "abs_orbit", std::stoi(metadataDataSet["Orbit_Number"]));
    add(theProductKwl, SUPPORT_DATA_PREFIX, "acquisition_mode", metadataDataSet["Acquisition_Mode"]);
    add(theProductKwl, SUPPORT_DATA_PREFIX, "azimuth_bandwidth", 
	std::stod(metadataDataSet["S01_Azimuth_Focusing_Bandwidth"]));
    add(theProductKwl, SUPPORT_DATA_PREFIX, "range_bandwidth",
	std::stod(metadataDataSet["S01_Range_Focusing_Bandwidth"]));

    add(theProductKwl, SUPPORT_DATA_PREFIX, "mds1_tx_rx_polar", metadataDataSet["S01_Polarisation"]);

    add(theProductKwl, SUPPORT_DATA_PREFIX, "line_time_interval", 
    std::stod(metadataBands[0]["S01_SBI_Line_Time_Interval"]));

    add(theProductKwl, SUPPORT_DATA_PREFIX, "range_spacing", 
	std::stod(metadataBands[0]["S01_SBI_Column_Spacing"]));
    add(theProductKwl, SUPPORT_DATA_PREFIX, "azimuth_spacing", 
	std::stod(metadataBands[0]["S01_SBI_Line_Spacing"]));
 
    add(theProductKwl, SUPPORT_DATA_PREFIX, "range_sampling_rate", 
	std::stod(metadataDataSet["S01_Sampling_Rate"]));
    add(theProductKwl, SUPPORT_DATA_PREFIX, "radar_frequency",  std::stod(metadataDataSet["Radar_Frequency"]));
    add(theProductKwl, SUPPORT_DATA_PREFIX, "slant_range_to_first_pixel", 
	std::stod(metadataBands[0]["S01_SBI_Zero_Doppler_Range_First_Time"]));

    add(theProductKwl, SUPPORT_DATA_PREFIX, "pulse_repetition_frequency", 
	std::stod(metadataDataSet["S01_PRF"]));

    // Size
    int sizex = dataset->GetRasterXSize();
    int sizey = dataset->GetRasterYSize();

    add(theProductKwl, SUPPORT_DATA_PREFIX, "number_samples", sizex);
    add(theProductKwl, SUPPORT_DATA_PREFIX, "number_lines", sizey);
    
    add(theProductKwl,"number_samples", sizex);
    add(theProductKwl, "number_lines", sizey);
    
    // Time
    int pos = metadataDataSet["Reference_UTC"].find(" ");
    std::string reference_UTC = metadataDataSet["Reference_UTC"].substr(0, pos);
 
    double total_seconds = std::stod(metadataBands[0]["S01_SBI_Zero_Doppler_Azimuth_First_Time"]);
    int hour = static_cast<int> (total_seconds/3600.0);
    int minutes = static_cast<int> ((total_seconds-hour*3600)/60.0);
    double seconds = total_seconds - hour*3600 - minutes*60;

    std::string first_line_time = reference_UTC + "T" + std::to_string(hour) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);
    
    total_seconds = std::stod(metadataBands[0]["S01_SBI_Zero_Doppler_Azimuth_Last_Time"]);
    hour = static_cast<int> (total_seconds/3600.0);
    minutes = static_cast<int> ((total_seconds-hour*3600)/60.0);
    seconds = total_seconds - hour*3600 - minutes*60;

    std::string last_line_time = reference_UTC + "T" + std::to_string(hour) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);

    add(theProductKwl, HEADER_PREFIX, "first_line_time", first_line_time);
    add(theProductKwl, HEADER_PREFIX, "last_line_time",  last_line_time);

    add(theProductKwl, SUPPORT_DATA_PREFIX, "first_line_time", first_line_time);
    add(theProductKwl, SUPPORT_DATA_PREFIX, "last_line_time",  last_line_time);

    std::string reference = reference_UTC  + "T00:00:00.0000";
    
    add(theProductKwl, SUPPORT_DATA_PREFIX, "date", reference);

    ////////////////// Add Orbit List ////////////////
    // Get elements
    int stateVectorList_size = std::stoi(metadataDataSet["Number_of_State_Vectors"]);
    std::string state_times = metadataDataSet["State_Vectors_Times"];
    std::string ecef_satellite_pos = metadataDataSet["ECEF_Satellite_Position"];
    std::string ecef_satellite_vel = metadataDataSet["ECEF_Satellite_Velocity"];

    // Convert std::string to vector
    std::vector<std::string> vTimes;
    otb::Utils::ConvertStringToVector(state_times, vTimes, "State_Vectors_Times", " ");
    
    std::vector<std::string> vECEF_sat_pos;
    otb::Utils::ConvertStringToVector(ecef_satellite_pos, vECEF_sat_pos, "ECEF_Satellite_Position", " ");

    std::vector<std::string> vECEF_sat_vel;
    otb::Utils::ConvertStringToVector(ecef_satellite_vel, vECEF_sat_vel, "ECEF_Satellite_Velocity", " ");
    

    char orbit_prefix_[256];
    for (int i = 0; i != stateVectorList_size ; ++i)
      {
	//orbit_state_vectors
	const int pos = s_printf(orbit_prefix_, "orbitList.orbit[%d].", int(i));
	assert(pos > 0 && pos < 256);
	const std::string orbit_prefix(orbit_prefix_, pos);
	
	total_seconds = std::stod(vTimes[i]);
	hour = static_cast<int> (total_seconds/3600.0);
	minutes = static_cast<int> ((total_seconds-hour*3600)/60.0);
	seconds = total_seconds - hour*3600 - minutes*60;


	std::string time = reference_UTC + "T" + std::to_string(hour) + ":" + std::to_string(minutes) + ":" + std::to_string(seconds);

	add(theProductKwl, orbit_prefix + keyTime, time);
        
	add(theProductKwl, orbit_prefix + keyPosX, vECEF_sat_pos[i*3 + 0]);
	add(theProductKwl, orbit_prefix + keyPosY, vECEF_sat_pos[i*3 + 1]);
	add(theProductKwl, orbit_prefix + keyPosZ, vECEF_sat_pos[i*3 + 2]);

	add(theProductKwl, orbit_prefix + keyVelX, vECEF_sat_vel[i*3 + 0]);
	add(theProductKwl, orbit_prefix + keyVelY, vECEF_sat_vel[i*3 + 1]);
	add(theProductKwl, orbit_prefix + keyVelZ, vECEF_sat_vel[i*3 + 2]);
      }

    add(theProductKwl,"orbitList.nb_orbits", stateVectorList_size);


    ////////////////// Add Burst Records : one for the moment ////////////////
    add(theProductKwl, BURST_NUMBER_KEY,                       ossimString("1"));
    BurstRecordType burstRecord;
    burstRecord.startLine        = add(theProductKwl, BURST_PREFIX, "[0].start_line", 0);
    burstRecord.endLine          = add(theProductKwl, BURST_PREFIX, "[0].end_line", sizey-1);

    burstRecord.startSample = add(theProductKwl, BURST_PREFIX, "[0].start_sample", 0);
    burstRecord.endSample   = add(theProductKwl, BURST_PREFIX, "[0].end_sample",  sizex-1);

    burstRecord.azimuthStartTime = add(theProductKwl, BURST_PREFIX, "[0].azimuth_start_time", 
				       ossimplugins::time::toModifiedJulianDate(first_line_time));
    burstRecord.azimuthStopTime  = add(theProductKwl, BURST_PREFIX, "[0].azimuth_stop_time",  
				       ossimplugins::time::toModifiedJulianDate(last_line_time));

    theBurstRecords.push_back(burstRecord);
    
    const std::string BURST_NUMBER_LINES_KEY    = "support_data.geom.bursts.number_lines_per_burst";
    add(theProductKwl, BURST_NUMBER_LINES_KEY, sizey);

    const std::string BURST_NUMBER_SAMPLES_KEY    = "support_data.geom.bursts.number_samples_per_burst";
    add(theProductKwl, BURST_NUMBER_SAMPLES_KEY, sizex);


    //////////////// Add GCPs one for the moment ////////////////
    add(theProductKwl, GCP_NUMBER_KEY, ossimString("1"));
     char prefix[1024];
     pos = s_printf(prefix, "%s[%d].", GCP_PREFIX.c_str(), 0);

     add(theProductKwl, prefix, attAzimuthTime, ossimplugins::time::toModifiedJulianDate(first_line_time));
     add(theProductKwl, prefix, keyImPtX, ossimString("0"));
     add(theProductKwl, prefix, keyImPtY, ossimString("0"));
     add(theProductKwl, prefix, keySlantRangeTime, 
		  metadataBands[0]["S01_SBI_Zero_Doppler_Range_First_Time"]);
    
     std::string geoCoor = metadataBands[0]["S01_SBI_Top_Left_Geodetic_Coordinates"];
     std::vector<std::string> vGeoCoor;
     otb::Utils::ConvertStringToVector(geoCoor, vGeoCoor, "S01_SBI_Top_Left_Geodetic_Coordinates", " ");

     
     add(theProductKwl, prefix, keyWorldPtLat, vGeoCoor[0]);
     add(theProductKwl, prefix, keyWorldPtLon,  vGeoCoor[1]);
     add(theProductKwl, prefix, keyWorldPtHgt, vGeoCoor[2]);

     ////////////////// Load the kwl ////////////////
     loadState(theProductKwl);
     
     // Update Sensor and image Id
     theSensorID = "CSK";
     theImageID = std::to_string(std::stoi(metadataDataSet["Programmed_Image_ID"]));

     return true;
  }

//*************************************************************************************************
// initImageSize
//*************************************************************************************************
   bool ossimCosmoSkymedModel::initImageSize(ossimIpt& imageSize) const
   {
      std::string const& samples_cstr = theProductKwl.findKey(SUPPORT_DATA_PREFIX, ossimKeywordNames::NUMBER_SAMPLES_KW);
      std::string const& lines_cstr   = theProductKwl.findKey(SUPPORT_DATA_PREFIX, ossimKeywordNames::NUMBER_LINES_KW);

      imageSize.samp = to<int>(samples_cstr, "decoding sample number from KWL");
      imageSize.line = to<int>(lines_cstr, "decoding line number from KWL");

      return true;
   }
 
//*************************************************************************************************
// imagingRay
//*************************************************************************************************
   void ossimCosmoSkymedModel::imagingRay(const ossimDpt& image_point, ossimEcefRay&   image_ray) const
   {
      // NOT YET IMPLEMENTED
      setErrorStatus();
   }

} //end namespace
