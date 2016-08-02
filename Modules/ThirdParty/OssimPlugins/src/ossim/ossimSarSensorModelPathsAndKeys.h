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
   extern const std::string HEADER_PREFIX       ;
   extern const std::string SUPPORT_DATA_PREFIX ;
   extern const std::string GEOM_PREFIX         ;
   extern const std::string SRD_PREFIX          ;
   extern const std::string SR_PREFIX           ;
   extern const std::string GR_PREFIX           ;
   extern const std::string BURST_PREFIX        ;
   extern const std::string BURST_NUMBER_KEY    ;
   extern const std::string GCP_PREFIX          ;
   extern const std::string GCP_NUMBER_KEY      ;
   extern const std::string NUMBER_KEY          ;
   extern const std::string PRODUCT_XML_FILE_KW ;

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
   extern const std::string keyAzimuthStartTime ;
   extern const std::string keyAzimuthStopTime  ;
} // ossimplugins namespace


#endif // ossimSarSensorModelPathsAndKeys_h
