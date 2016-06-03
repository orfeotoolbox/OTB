//----------------------------------------------------------------------------
//
// "Copyright Centre National d'Etudes Spatiales"
//
// License:  LGPL-2
//
// See LICENSE.txt file in the top level directory for more details.
//
//----------------------------------------------------------------------------

#ifndef ossimSarSensorModelPathsAndKeys_h
#define ossimSarSensorModelPathsAndKeys_h

#include <string>

namespace ossimplugins {
   const std::string HEADER_PREFIX       ;
   const std::string SUPPORT_DATA_PREFIX ;
   const std::string GEOM_PREFIX         ;
   const std::string SRD_PREFIX          ;
   const std::string SR_PREFIX           ;
   const std::string GR_PREFIX           ;
   const std::string BURST_PREFIX        ;
   const std::string BURST_NUMBER_KEY    ;
   const std::string GCP_PREFIX          ;
   const std::string GCP_NUMBER_KEY      ;
   const std::string NUMBER_KEY          ;

   const std::string keySr0              ;
   const std::string keyGr0              ;
   const std::string keyAzimuthTime      ;
   const std::string keySlantRangeTime   ;
   const std::string keyImPtX            ;
   const std::string keyImPtY            ;
   const std::string keyWorldPtLat       ;
   const std::string keyWorldPtLon       ;
   const std::string keyWorldPtHgt       ;
   const std::string keyTime             ;
   const std::string keyPosX             ;
   const std::string keyPosY             ;
   const std::string keyPosZ             ;
   const std::string keyVelX             ;
   const std::string keyVelY             ;
   const std::string keyVelZ             ;
   const std::string keyStartLine        ;
   const std::string keyEndLine          ;
   const std::string keyAzimuthStartTime ;
   const std::string keyAzimuthStopTime  ;
} // ossimplugins namespace


#endif // ossimSarSensorModelPathsAndKeys_h
