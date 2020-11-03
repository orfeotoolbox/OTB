/*
 * Copyright (C) 2020 Centre National d'Etudes Spatiales (CNES)
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

#include "otbGeomMetadataSupplier.h"

int otbGeomMetadataSupplierTest(int itkNotUsed(argc), char* argv[])
{
  // Verify the number of parameters in the command line
  const char* fileName = argv[1];
  const char* outputFilename = argv[2];

  // List of simple keys
  std::vector<std::string> simpleKeys =
    {"bias_error", "ce90_absolute", "ce90_relative", "current_adjustment", "height_off",
     "height_scale", "image_id", "lat_off", "lat_scale", "line_off", "line_scale"};
  // List of adjustment keys
  std::vector<std::string> adjustmentKeys =
    {"center", "description", "lock_flag", "parameter", "sigma", "units"};

  // Instantiate the supplier
  otb::GeomMetadataSupplier mds(fileName);

  bool hasValue;
  std::ofstream file;
  file.open(outputFilename);

  for(auto const& val : simpleKeys)
    file << val << " : " << mds.GetMetadataValue(val, hasValue) << "\n";

  std::ostringstream key;
  std::ostringstream key2;
  for(unsigned int loop = 0 ; loop < 5 ; ++loop)
  {
    key.str("");
    key << "adjustment_0.adj_param_" << loop << ".";
    for (auto const& val : adjustmentKeys)
    {
      key2.str("");
      key2 << key.str() << val;
      file << key2.str() << " : " << mds.GetMetadataValue(key2.str(), hasValue) << "\n";
    }
  }

  file << '\n';

  otb::ImageMetadata imd;
  mds.FetchRPC(imd);
  file << boost::any_cast<otb::Projection::RPCParam>(imd[otb::MDGeom::RPC]).ToJSON(true) << '\n';

  file.close();
  return EXIT_SUCCESS;
}
