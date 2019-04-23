/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


/* Example usage:
./DEMHandlerExample Input/DEM/srtm_directory Input/DEM/egm96.grd 40 8.434583 44.647083 383.580313671 0.001
*/


// OTB relies on OSSIM for elevation handling. Since release 3.16, there is a
// single configuration class \doxygen{otb}{DEMHandler} to manage elevation (in
// image projections or localization functions for example).  This configuration
// is managed by the a proper instantiation and parameters setting of this
// class.  These instantiations must be done before any call to geometric
// filters or functionalities. Ossim internal accesses to elevation are also
// configured by this class and this will ensure consistency throughout the
// library.

#include "otbDEMHandler.h"

int main(int argc, char* argv[])
{
  if (argc != 8)
  {
    std::cerr << "Usage: " << argv[0] << " demdir[path|no] geoid[path|no] defaultHeight longitude latitude targetValue tolerance" << std::endl;
    return EXIT_FAILURE;
  }

  std::string demdir        = argv[1];
  std::string geoid         = argv[2];
  double      defaultHeight = atof(argv[3]);
  double      longitude     = atof(argv[4]);
  double      latitude      = atof(argv[5]);
  double      target        = atof(argv[6]);
  double      tolerance     = atof(argv[7]);

  // This class is a singleton, the New() method is deprecated and will be removed
  // in future release. We need to use the \code{Instance()} method instead.

  otb::DEMHandler::Pointer demHandler = otb::DEMHandler::Instance();

  bool fail = false;

  // It allows configuring a directory containing DEM tiles (DTED or SRTM
  // supported) using the \code{OpenDEMDirectory()} method. The \code{OpenGeoidFile()} method
  // allows inputting a geoid file as well. Last, a default height above ellipsoid
  // can be set using the \code{SetDefaultHeightAboveEllipsoid()} method.

  demHandler->SetDefaultHeightAboveEllipsoid(defaultHeight);

  if (!demHandler->IsValidDEMDirectory(demdir.c_str()))
  {
    std::cerr << "IsValidDEMDirectory(" << demdir << ") = false" << std::endl;
    fail = true;
  }

  demHandler->OpenDEMDirectory(demdir);
  demHandler->OpenGeoidFile(geoid);

  // We can now retrieve height above ellipsoid or height above Mean Sea Level
  // (MSL) using the methods \code{GetHeightAboveEllipsoid()} and
  // \code{GetHeightAboveMSL()}.  Outputs of these methods depend on the
  // configuration of the class \doxygen{otb}{DEMHandler} and the different
  // cases are:
  //
  // For \code{GetHeightAboveEllipsoid()}:
  //
  // \begin{itemize}
  // \item DEM and geoid both available: $dem\_value + geoid\_offset$
  // \item No DEM but geoid available: geoid\_offset
  // \item DEM available, but no geoid: dem\_value
  // \item No DEM and no geoid available: default height above ellipsoid
  // \end{itemize}
  //
  // For \code{GetHeightAboveMSL()}:
  //
  // \begin{itemize}
  // \item DEM and geoid both available: srtm\_value
  // \item No DEM but geoid available: $0$
  // \item DEM available, but no geoid: srtm\_value
  // \item No DEM and no geoid available: $0$
  // \end{itemize}

  otb::DEMHandler::PointType point;
  point[0] = longitude;
  point[1] = latitude;

  double height = -32768;

  height = demHandler->GetHeightAboveMSL(point);
  std::cout << "height above MSL (" << longitude << "," << latitude << ") = " << height << " meters" << std::endl;

  height = demHandler->GetHeightAboveEllipsoid(point);
  std::cout << "height above ellipsoid (" << longitude << ", " << latitude << ") = " << height << " meters" << std::endl;

  // Note that OSSIM internal calls for sensor
  // modelling use the height above ellipsoid, and follow the same logic as the
  // \code{GetHeightAboveEllipsoid()} method.

  // Check for Nan
  if (vnl_math_isnan(height))
  {
    std::cerr << "Computed value is NaN" << std::endl;
    fail = true;
  }

  double error = std::abs(height - target);

  if (error > tolerance)
  {
    std::cerr << "Target value is " << target << " meters, computed value is " << height << " meters. error (" << error << " meters) > tolerance (" << tolerance
              << " meters)" << std::endl;
    fail = true;
  }

  if (fail)
  {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
