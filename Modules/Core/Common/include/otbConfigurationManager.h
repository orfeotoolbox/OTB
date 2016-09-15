/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _otbConfigurationManager_h
#define _otbConfigurationManager_h

#include "itkVersion.h"

#if ITK_VERSION_MAJOR < 4 || (ITK_VERSION_MAJOR == 4 && ITK_VERSION_MINOR <= 8)
#include "itksys/FundamentalType.h"
#else
#include "itk_kwiml.h"
#endif

#include <string>
#include <boost/cstdint.hpp>

#include "OTBCommonExport.h"

namespace otb
{
/**
 * \brief Retrieve configuration values from env var or default values
 *
 * This is a simple helper class to retrieve configuration values from
 * environment variables if they are set, or from default values if
 * not.
 *
 * Please refer to each static method documentation for available
 * configuration values and related environment variables.
 */
class OTBCommon_EXPORT ConfigurationManager
{
public:
#if ITK_VERSION_MAJOR < 4 || (ITK_VERSION_MAJOR == 4 && ITK_VERSION_MINOR <= 8)
  typedef ::itksysFundamentalType_UInt64 RAMValueType;
#else
  typedef KWIML_INT_uint64_t RAMValueType;
#endif

  /**
   * DEMDirectory is a directory were DEM tiles are stored.
   *
   * If environment variable OTB_DEM_DIRECTORY is defined,
   * returns it contents as a string
   * Else, returns an empty string
   */
  static std::string GetDEMDirectory();

  /**
   * GeoidFile is path to a geoid file.
   *
   * If environment variable OTB_GEOID_FILE is defined,
   * returns it contents as a string
   * Else, returns an empty string
   */
  static std::string GetGeoidFile();

  /**
   * MaxRAMHint denotes the maximum memory OTB should use for
   * processing, expressed in MegaBytes.
   *
   * If environment variable OTB_MAX_RAM_HINT is defined and could be
   * converted to int, return its content as a 64 bits unsigned int.
   * Else, returns default value, which is 128 Mb
   *
   */
  static RAMValueType GetMaxRAMHint();

private:
  ConfigurationManager(); //purposely not implemented
  ~ConfigurationManager(); //purposely not implemented
  ConfigurationManager(const ConfigurationManager&); //purposely not implemented
  void operator =(const ConfigurationManager&); //purposely not implemented


};
}

#endif
