//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL-2
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------

#include "ossimSarSensorModelPathsAndKeys.h"

namespace ossimplugins {
   const std::string HEADER_PREFIX       = "header.";
   const std::string SUPPORT_DATA_PREFIX = "support_data.";
   const std::string GEOM_PREFIX         = "support_data.geom";
   const std::string SRD_PREFIX          = "support_data.geom.srd";
   const std::string SR_PREFIX           = "support_data.geom.srd.coords.sr";
   const std::string GR_PREFIX           = "support_data.geom.srd.coords.gr";
   const std::string BURST_PREFIX        = "support_data.geom.bursts.burst";
   const std::string BURST_NUMBER_KEY    = "support_data.geom.bursts.number";
   const std::string GCP_PREFIX          = "support_data.geom.gcp";
   const std::string GCP_NUMBER_KEY      = "support_data.geom.gcp.number";
   const std::string NUMBER_KEY          = "number";

   const std::string keySr0              = "sr0";
   const std::string keyGr0              = "gr0";
   const std::string keyAzimuthTime      = "azimuthTime";
   const std::string keySlantRangeTime   = "slant_range_time";
   const std::string keyImPtX            = "im_pt.x";
   const std::string keyImPtY            = "im_pt.y";
   const std::string keyWorldPtLat       = "world_pt.lat";
   const std::string keyWorldPtLon       = "world_pt.lon";
   const std::string keyWorldPtHgt       = "world_pt.hgt";
   const std::string keyTime             = "time";
   const std::string keyPosX             = "x_pos";
   const std::string keyPosY             = "y_pos";
   const std::string keyPosZ             = "z_pos";
   const std::string keyVelX             = "x_vel";
   const std::string keyVelY             = "y_vel";
   const std::string keyVelZ             = "z_vel";
   const std::string keyStartLine        = "start_line";
   const std::string keyEndLine          = "end_line";
   const std::string keyAzimuthStartTime = "azimuth_start_time";
   const std::string keyAzimuthStopTime  = "azimuth_stop_time";
} // ossimplugins namespace

