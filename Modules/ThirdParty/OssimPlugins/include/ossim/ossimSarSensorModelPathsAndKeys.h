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


#ifndef ossimSarSensorModelPathsAndKeys_h
#define ossimSarSensorModelPathsAndKeys_h

#include <string>

namespace ossimplugins {
   extern const std::string HEADER_PREFIX       ;
   extern const std::string SUPPORT_DATA_PREFIX ;
   extern const std::string GEOM_PREFIX         ;
   extern const std::string SRD_PREFIX          ;
   extern const std::string SR_PREFIX           ;
   extern const std::string GR_PREFIX           ;
   extern const std::string BURST_PREFIX        ;
   extern const std::string BURST_NUMBER_KEY    ;
   extern const std::string ORBIT_PREFIX        ;
   extern const std::string ORBIT_NUMBER_KEY    ;
   extern const std::string GCP_PREFIX          ;
   extern const std::string GCP_NUMBER_KEY      ;
   extern const std::string NUMBER_KEY          ;

   extern const std::string keySr0              ;
   extern const std::string keyGr0              ;
   extern const std::string keyAzimuthTime      ;
   extern const std::string keySlantRangeTime   ;
   extern const std::string keyImPtX            ;
   extern const std::string keyImPtY            ;
   extern const std::string keyWorldPtLat       ;
   extern const std::string keyWorldPtLon       ;
   extern const std::string keyWorldPtHgt       ;
   extern const std::string keyTime             ;
   extern const std::string keyPosX             ;
   extern const std::string keyPosY             ;
   extern const std::string keyPosZ             ;
   extern const std::string keyVelX             ;
   extern const std::string keyVelY             ;
   extern const std::string keyVelZ             ;
   extern const std::string keyStartLine        ;
   extern const std::string keyEndLine          ;
   extern const std::string keyStartSample      ;
   extern const std::string keyEndSample        ;
   extern const std::string keyAzimuthStartTime ;
   extern const std::string keyAzimuthStopTime  ;
} // ossimplugins namespace


#endif // ossimSarSensorModelPathsAndKeys_h
