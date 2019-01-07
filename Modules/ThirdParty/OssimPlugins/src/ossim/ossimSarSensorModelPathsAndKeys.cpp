/*
 * Copyright (C) 2005-2017 by Centre National d'Etudes Spatiales (CNES)
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


#include "ossim/ossimSarSensorModelPathsAndKeys.h"

namespace ossimplugins {
  const std::string HEADER_PREFIX       = "header.";
  const std::string SUPPORT_DATA_PREFIX = "support_data.";
  const std::string GEOM_PREFIX         = "support_data.geom";
  const std::string SRD_PREFIX          = "support_data.geom.srd";
  const std::string SR_PREFIX           = "support_data.geom.srd.coords.sr";
  const std::string GR_PREFIX           = "support_data.geom.srd.coords.gr";
  const std::string BURST_PREFIX        = "support_data.geom.bursts.burst";
  const std::string BURST_NUMBER_KEY    = "support_data.geom.bursts.number";
  const std::string ORBIT_PREFIX        = "orbitList.orbit";
  const std::string ORBIT_NUMBER_KEY    = "orbitList.nb_orbits";
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
  const std::string keyStartSample      = "start_sample";
  const std::string keyEndSample        = "end_sample";
  const std::string keyAzimuthStartTime = "azimuth_start_time";
  const std::string keyAzimuthStopTime  = "azimuth_stop_time";
} // ossimplugins namespace

