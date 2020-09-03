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

typedef otb::Image<double>                 ImageType;
typedef otb::ImageFileReader<ImageType>    ReaderType;

int otbGDALRPCTransformerTest(int itkNotUsed(argc), char* argv[])
{
  bool success = true;

  double LineOffset = 15834.0, SampleOffset = 13464.0,
         LatOffset = -42.8607, LonOffset = 147.2588, HeightOffset = 300.0,
         LineScale = 15834.0, SampleScale = 13464.0,
		 LatScale = 0.0715, LonScale = 0.0828, HeightScale = 970.0;
  const double line_num_coeff[20] = {
		  -0.000539636886315094, 0.00262771165447159, -1.00287836503009, -0.0340303311076584, 0.00523658598538616,
		  0.00210057328569037, 0.00311664695421511, 0.000406267962891555, -0.00550089873884607, 5.26202553862825e-05,
		  2.5958707865627e-06, -2.23632198653199e-06, 2.02822452334703e-05, -5.24009408417096e-06, 2.16913023637956e-05,
		  -2.36002554032361e-05, 1.56703932477487e-06, -6.14095576183939e-06, 1.8422191786661e-05, 2.01615774485329e-07};
  const double line_den_coeff[20] = {
		  1, -0.00520769693945429, 0.00549138470185778, -0.00472718007439368, -2.02057365543208e-05,
		  8.39509604990839e-06, -3.43728317291176e-05, -1.94598141718645e-05, 2.35660616018416e-05, 3.71653924834455e-06,
		  7.08406853351595e-08, -5.72002373463043e-09, 8.71363385561163e-09, -4.39082343385483e-09, 1.39161295297537e-08,
		  -1.24914964124471e-08, 4.6171143198849e-08, 1.70067569381897e-08, -2.37449991316111e-08, 9.05460084990073e-10};
  const double samp_num_coeff[20] = {
		  0.00121386429106107, 1.0047652205584, 0.00363040578479236, -0.033702055558379, 0.00665808526532754,
		  -0.0036225246184886, -0.000251174639124558, -0.00523232964739848, 1.74629130199178e-05, 0.000131049715749837,
		  -2.8029900276216e-05, -1.95273546561053e-05, 2.91977037724833e-05, 2.78736117374265e-07, -2.64005759423042e-05,
		  7.08277764663844e-08, 1.12211574945991e-06, 1.15330773275834e-06, 7.15463416011193e-07, -2.42741993962372e-08};
  const double samp_den_coeff[20] = {
		  1, -0.00520769693945429, 0.00549138470185778, -0.00472718007439368, -2.02057365543208e-05,
		  8.39509604990839e-06, -3.43728317291176e-05, -1.94598141718645e-05, 2.35660616018416e-05, 3.71653924834455e-06,
		  7.08406853351595e-08, -5.72002373463043e-09, 8.71363385561163e-09, -4.39082343385483e-09, 1.39161295297537e-08,
		  -1.24914964124471e-08, 4.6171143198849e-08, 1.70067569381897e-08, -2.37449991316111e-08, 9.05460084990073e-10};

  otb::GDALRPCTransformer transformer(LineOffset, SampleOffset, LatOffset, LonOffset, HeightOffset,
                                      LineScale, SampleScale, LatScale, LonScale, HeightScale,
                                      line_num_coeff, line_den_coeff, samp_num_coeff, samp_den_coeff);

  // Test ForwardTransform
  std::vector<double> x = {20, 20};
  std::vector<double> y = {10, 10};
  std::vector<double> z = {0, 0};
  double exp_x = 147.1753248795781;
  double exp_y = -42.78881952208293;
  double exp_z = 0.0;
  if(!transformer.ForwardTransform(x.data(), y.data(), z.data(), 2))
  {
    std::cout << "ForwardTransform failed, returned False." << '\n';
    success = false;
  }
  if (!itk::Math::FloatAlmostEqual(x[0], exp_x))
  {
    std::cout << "Bad value for latitude. Expected " << exp_x << " but computed " << x[0] << "\n";
    success = false;
  }
  if (!itk::Math::FloatAlmostEqual(y[0], exp_y))
  {
    std::cout << "Bad value for longitude. Expected " << exp_y << " but computed " << y[0] << "\n";
    success = false;
  }
  if (!itk::Math::FloatAlmostEqual(z[0], exp_z))
  {
    std::cout << "Bad value for height. Expected " << exp_z << " but computed " << z[0] << "\n";
    success = false;
  }
  otb::GDALRPCTransformer::PointType zePoint;
  zePoint[0] = 20.0;
  zePoint[1] = 10.0;
  zePoint[2] = 0.0;
  transformer.ForwardTransform(zePoint);
  if (!itk::Math::FloatAlmostEqual(zePoint[0], exp_x))
  {
    std::cout << "Bad value for latitude. Expected " << exp_x << " but computed " << zePoint[0] << "\n";
    success = false;
  }
  if (!itk::Math::FloatAlmostEqual(zePoint[1], exp_y))
  {
    std::cout << "Bad value for longitude. Expected " << exp_y << " but computed " << zePoint[1] << "\n";
    success = false;
  }
  if (!itk::Math::FloatAlmostEqual(zePoint[2], exp_z))
  {
    std::cout << "Bad value for height. Expected " << exp_z << " but computed " << zePoint[2] << "\n";
    success = false;
  }

  // Test InverseTransform
  x = {exp_x, exp_x};
  y = {exp_y, exp_y};
  zePoint[0] = exp_x;
  zePoint[1] = exp_y;
  exp_x = 20.01139515217801;
  exp_y = 9.984517889590279;
  if(!transformer.InverseTransform(x.data(), y.data(), z.data(), 2))
  {
    std::cout << "ForwardTransform failed, returned False." << '\n';
    success = false;
  }
  if (!itk::Math::FloatAlmostEqual(x[0], exp_x))
  {
    std::cout << "Bad value for latitude. Expected " << exp_x << " but computed " << x[0] << "\n";
    success = false;
  }
  if (!itk::Math::FloatAlmostEqual(y[0], exp_y))
  {
    std::cout << "Bad value for longitude. Expected " << exp_y << " but computed " << y[0] << "\n";
    success = false;
  }
  if (!itk::Math::FloatAlmostEqual(z[0], exp_z))
  {
    std::cout << "Bad value for height. Expected " << exp_z << " but computed " << z[0] << "\n";
    success = false;
  }
  transformer.ForwardTransform(zePoint);
  if (!itk::Math::FloatAlmostEqual(zePoint[0], exp_x))
  {
    std::cout << "Bad value for latitude. Expected " << exp_x << " but computed " << zePoint[0] << "\n";
    success = false;
  }
  if (!itk::Math::FloatAlmostEqual(zePoint[1], exp_y))
  {
    std::cout << "Bad value for longitude. Expected " << exp_y << " but computed " << zePoint[1] << "\n";
    success = false;
  }
  if (!itk::Math::FloatAlmostEqual(zePoint[2], exp_z))
  {
    std::cout << "Bad value for height. Expected " << exp_z << " but computed " << zePoint[2] << "\n";
    success = false;
  }

  if (success)
    return EXIT_SUCCESS;
  else
    return EXIT_FAILURE;
}
