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

#include "otbGDALRPCTransformer.h"
#include "otbMacro.h"
#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageMetadata.h"
#include "gdal_utils.h"
#include <gdal_priv.h>
#include "otbDEMHandler.h"

typedef otb::Image<double>                 ImageType;
typedef otb::ImageFileReader<ImageType>    ReaderType;

int otbGDALRPCTransformerTest(int itkNotUsed(argc), char* argv[])
{
  bool success = true;

  double LineOffset = 16201.0, SampleOffset = 15184.0,
         LatOffset = 39.7792, LonOffset = 125.7510, HeightOffset = 97.0,
         LineScale = 16480.0, SampleScale = 15217.0,
		 LatScale = 0.0900, LonScale = 0.1096, HeightScale = 501.0;
  const double line_num_coeff[20] = {
		  +5.105608E-04, -2.921055E-02, -1.010407E+00, -1.743729E-02, -6.604239E-05,
		  -7.871396E-05, +3.027877E-04, -4.323587E-04, -2.624751E-04, +6.186490E-06,
		  +1.084676E-06, +5.389738E-05, +4.145232E-06, +3.911486E-07, +1.772434E-05,
		  +3.302960E-06, +3.006106E-06, +1.662606E-05, +6.051677E-06, -2.657667E-08};
  const double line_den_coeff[20] = {
		  +1.000000E+00, -9.652128E-05, +2.488346E-04, +3.089019E-04, -2.120170E-06,
		  +4.117913E-07,  +1.370009E-06, +1.357281E-05, -4.174324E-06, -3.146787E-06,
		  -7.724587E-06, +3.524480E-04, -1.303224E-05, -8.507679E-07, -1.670972E-05,
		  +6.781061E-06, +5.602262E-07, +1.161421E-05, +4.681872E-06, +5.593931E-08};
  const double samp_num_coeff[20] = {
		  -2.429563E-04, +1.028320E+00, -3.360972E-02, +3.519600E-03, -6.568341E-04,
		  +5.951139E-04, -3.875716E-04, +1.260622E-04, -5.273817E-05, -4.418981E-06,
		  -3.520581E-06, -2.502760E-04, -4.167704E-05, -5.973233E-05, -1.438949E-04,
		  +7.603041E-06, +2.358136E-06, -2.275274E-05,  +1.602657E-06, -1.716541E-07};
  const double samp_den_coeff[20] = {
		  +1.000000E+00, +7.765620E-05, +6.568707E-04, -6.270621E-04, +5.163170E-05,
		  +6.979463E-06, +2.476334E-07, +1.083558E-04, -4.043734E-05, -5.819288E-05,
		  +1.778201E-07, +5.665202E-05, +6.927205E-06, +6.793485E-07, +3.604209E-05,
		  -4.057103E-07, -8.291254E-07, +1.010650E-05, -2.875552E-06, +5.142751E-08};

  otb::GDALRPCTransformer transformer(LineOffset, SampleOffset, LatOffset, LonOffset, HeightOffset,
                                      LineScale, SampleScale, LatScale, LonScale, HeightScale,
                                      line_num_coeff, line_den_coeff, samp_num_coeff, samp_den_coeff);

  // Test ForwardTransform
  std::vector<double> x = {20.5, 20.5};
  std::vector<double> y = {10.5, 10.5};
  std::vector<double> z = {0, 0};
  double exp_x = 125.64830100509131;
  double exp_y = 39.869433991997553;
  double exp_z = 0.0;
  std::cout << "Test ForwardTransform with double interface." << '\n';
  if(!transformer.ForwardTransform(x.data(), y.data(), z.data(), 2))
  {
    std::cout << "ForwardTransform failed, returned False." << '\n';
    success = false;
  } else {
    if (x[0] - exp_x > 0.000001)
    {
      std::cout << "Bad value for latitude. Expected " << exp_x << " but computed " << x[0] << "\n";
      success = false;
    }
    if (y[0] - exp_y > 0.000001)
    {
      std::cout << "Bad value for longitude. Expected " << exp_y << " but computed " << y[0] << "\n";
      success = false;
    }
    if (z[0] - exp_z > 0.1)
    {
      std::cout << "Bad value for height. Expected " << exp_z << " but computed " << z[0] << "\n";
      success = false;
    }
  }
  otb::GDALRPCTransformer::PointType zePoint;
  zePoint[0] = 20.5;
  zePoint[1] = 10.5;
  zePoint[2] = 0.0;
  std::cout << "Test ForwardTransform with point interface." << '\n';
  zePoint = transformer.ForwardTransform(zePoint);
  if (zePoint[0] - exp_x > 0.000001)
  {
    std::cout << "Bad value for latitude. Expected " << exp_x << " but computed " << zePoint[0] << "\n";
    success = false;
  }
  if (zePoint[1] - exp_y > 0.000001)
  {
    std::cout << "Bad value for longitude. Expected " << exp_y << " but computed " << zePoint[1] << "\n";
    success = false;
  }
  if (zePoint[2] - exp_z > 0.1)
  {
    std::cout << "Bad value for height. Expected " << exp_z << " but computed " << zePoint[2] << "\n";
    success = false;
  }

  // Test InverseTransform
  x = {exp_x, exp_x};
  y = {exp_y, exp_y};
  zePoint[0] = exp_x;
  zePoint[1] = exp_y;
  zePoint[2] = 0.0;
  exp_x = 20.5;
  exp_y = 10.5;
  std::cout << "Test InverseTransform with double interface." << '\n';
  if(!transformer.InverseTransform(x.data(), y.data(), z.data(), 2))
  {
    std::cout << "InverseTransform failed, returned False." << '\n';
    success = false;
  } else {
    if (x[0] - exp_x > 0.05)
    {
      std::cout << "Bad value for latitude. Expected " << exp_x << " but computed " << x[0] << "\n";
      success = false;
    }
    if (y[0] - exp_y > 0.05)
    {
      std::cout << "Bad value for longitude. Expected " << exp_y << " but computed " << y[0] << "\n";
      success = false;
    }
    if (z[0] - exp_z > 0.1)
    {
      std::cout << "Bad value for height. Expected " << exp_z << " but computed " << z[0] << "\n";
      success = false;
    }
  }
  std::cout << "Test InverseTransform with point interface." << '\n';
  zePoint = transformer.InverseTransform(zePoint);
  if (zePoint[0] - exp_x > 0.05)
  {
    std::cout << "Bad value for latitude. Expected " << exp_y << " but computed " << zePoint[1] << "\n";
    success = false;
  }
  if (zePoint[1] - exp_y > 0.05)
  {
    std::cout << "Bad value for longitude. Expected " << exp_y << " but computed " << zePoint[1] << "\n";
    success = false;
  }
  if (zePoint[2] - exp_z > 0.1)
  {
    std::cout << "Bad value for height. Expected " << exp_z << " but computed " << zePoint[2] << "\n";
    success = false;
  }

  // Test with a DEM
  otb::DEMHandler& dem = otb::DEMHandler::GetInstance();
  dem.OpenDEMFile(argv[1]);
  zePoint[0] = 20.5;
  zePoint[1] = 10.5;
  zePoint[2] = 0.0;
  exp_x = 125.64828521533849;
  exp_y = 39.869345204440144;
  transformer.SetOption("RPC_DEM", dem.DEM_DATASET_PATH);
  transformer.SetOption("RPC_HEIGHT_SCALE", "2");

  std::cout << "Test ForwardTransform with DEM." << '\n';
  zePoint = transformer.ForwardTransform(zePoint);
  if (zePoint[0] - exp_x > 0.000001)
  {
    std::cout << "Bad value for latitude. Expected " << exp_y << " but computed " << zePoint[1] << "\n";
    success = false;
  }
  if (zePoint[1] - exp_y > 0.000001)
  {
    std::cout << "Bad value for longitude. Expected " << exp_y << " but computed " << zePoint[1] << "\n";
    success = false;
  }
  if (zePoint[2] - exp_z > 0.1)
  {
    std::cout << "Bad value for height. Expected " << exp_z << " but computed " << zePoint[2] << "\n";
    success = false;
  }

  exp_x = 20.5;
  exp_y = 10.5;
  std::cout << "Test InverseTransform with DEM." << '\n';
  zePoint = transformer.InverseTransform(zePoint);
  if (zePoint[0] - exp_x > 0.05)
  {
    std::cout << "Bad value for latitude. Expected " << exp_y << " but computed " << zePoint[1] << "\n";
    success = false;
  }
  if (zePoint[1] - exp_y > 0.05)
  {
    std::cout << "Bad value for longitude. Expected " << exp_y << " but computed " << zePoint[1] << "\n";
    success = false;
  }
  if (zePoint[2] - exp_z > 0.1)
  {
    std::cout << "Bad value for height. Expected " << exp_z << " but computed " << zePoint[2] << "\n";
    success = false;
  }

  if (success)
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;
}
