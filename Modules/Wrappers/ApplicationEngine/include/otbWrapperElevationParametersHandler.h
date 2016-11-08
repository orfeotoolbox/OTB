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
#ifndef otbWrapperElevationParametersHandler_h
#define otbWrapperElevationParametersHandler_h

#include "otbWrapperApplication.h"

namespace otb
{
namespace Wrapper
{

/** \class ElevationParametersHandler
 *  \brief This class represent a helper class for elevation modes. It
 *  add the parameters automatically to the application where it is
 *  used, provide method to get the value needed.
 *
 * This class allow creating a Choice list with several Elevation
 * modes to be used, supported modes are DEM, Geoid file, average
 * elevation. The user can get the value relative to each mode
 * using one the following methods Get{AverageElevation, DEMDirectory, GeoidFile}.
 *
 *
 *
 * \ingroup OTBApplicationEngine
 */

class ElevationParametersHandler
{
public:
  /**
    * Add a Group containing several choices for map projections
    *
    */
  static OTBApplicationEngine_EXPORT void AddElevationParameters(Application::Pointer app, const std::string & key);

  /** Method for getting the value of the elevation mode selected */
  static OTBApplicationEngine_EXPORT float GetDefaultElevation(const Application::Pointer app, const std::string& key);
  static OTBApplicationEngine_EXPORT const std::string GetGeoidFile(const Application::Pointer app, const std::string& key);
  static OTBApplicationEngine_EXPORT const std::string GetDEMDirectory(const Application::Pointer app, const std::string& key);
  static OTBApplicationEngine_EXPORT bool IsGeoidUsed(const Application::Pointer app, const std::string& key);
  static OTBApplicationEngine_EXPORT bool IsDEMUsed(const Application::Pointer app, const std::string & key);

  static OTBApplicationEngine_EXPORT void SetupDEMHandlerFromElevationParameters(const Application::Pointer app, const std::string& key);

protected:
  ElevationParametersHandler(); // not implemented
  virtual ~ElevationParametersHandler(); // not implemented
};

}
}


#endif // otbWrapperElevationParametersHandler_h_
