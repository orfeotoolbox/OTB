/*
 * Copyright (C) 2005-2020 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef otbWrapperMapProjectionParametersHandler_h
#define otbWrapperMapProjectionParametersHandler_h

#include "OTBApplicationEngineExport.h"

#include "otbWrapperApplication.h"

namespace otb
{
namespace Wrapper
{

// enum used for the map projection choices
enum
{
  Map_Utm,
  Map_Lambert2,
  Map_Lambert93,
  // Map_Transmercator,
  Map_WGS84,
  Map_Epsg
};


/** \class MapProjectionParametersHandler
 *  \brief This class represent a helper class.
 *
 * This class allow creating a Group with several mapProjection
 * parameters
 *
 * \ingroup OTBApplicationEngine
 */
class MapProjectionParametersHandler
{
public:
  /**
    * Add a Group containing several choices for map projections
    *
    */
  static OTBApplicationEngine_EXPORT void AddMapProjectionParameters(Application::Pointer app, const std::string& key);

  /**
    * Helper method : Compute the ProjectionRef knowing the map
    * projection picked up by the user
    *
    */
  static OTBApplicationEngine_EXPORT const std::string GetProjectionRefFromChoice(const Application::Pointer app, const std::string& key);


  /**
    * Helper method : Compute the UTM parameters relative an image
    * Note: The key of the image must be set to be able to get the image.
    *       The key must be totally if the InputImageParameter belongs
    *       to a ParameterGroup, ie set io.in
    */
  static OTBApplicationEngine_EXPORT void InitializeUTMParameters(Application::Pointer app, const std::string& imageKey, const std::string& mapKey);

protected:
  MapProjectionParametersHandler();          // not implemented
  virtual ~MapProjectionParametersHandler(); // not implemented
};
}
}


#endif // otbWrapperMapProjectionParametersHandler_h_
