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

#include <iostream>
#include "otbVectorImage.h"
#include "otbImageFileWriter.h"

#include "otbWrapperApplicationRegistry.h"

#include "otbImageToGenericRSOutputParameters.h"

using VectorImageType        = otb::VectorImage<unsigned char>;
using PixelType              = VectorImageType::PixelType;
using ApplicationPointerType = otb::Wrapper::Application::Pointer;

/* This function is creating and filling a georeferenced vector image */
VectorImageType::Pointer create_vector_image(VectorImageType::SizeType size, int nb_comp, unsigned char value, const OGRSpatialReference& projRef,
                                             VectorImageType::PointType origin, VectorImageType::SpacingType spacing)
{

  VectorImageType::IndexType index;
  index.Fill(0);
  VectorImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(index);

  auto image = VectorImageType::New();

  image->SetLargestPossibleRegion(region);
  image->SetBufferedRegion(region);
  image->SetRequestedRegion(region);
  image->SetNumberOfComponentsPerPixel(nb_comp);
  image->Allocate();
  PixelType val(nb_comp);
  val.Fill(value);
  image->FillBuffer(val);

  char* projRefWkt = nullptr;

  projRef.exportToPrettyWkt(&projRefWkt);
  image->SetProjectionRef(projRefWkt);

  image->SetOrigin(origin);

  image->SetSignedSpacing(spacing);

  return image;
}

// Compare the ROI computed by the application with a reference
bool check_roi(ApplicationPointerType app, int ref_startx, int ref_starty, int ref_sizex, int ref_sizey)
{
  auto app_startx = app->GetParameterInt("startx");
  auto app_starty = app->GetParameterInt("starty");
  auto app_sizex  = app->GetParameterInt("sizex");
  auto app_sizey  = app->GetParameterInt("sizey");

  if (app_startx != ref_startx || app_starty != ref_starty || app_sizex != ref_sizex || app_sizey != ref_sizey)
  {
    std::cout << "The ROI computed by the application is incorrect." << std::endl;
    std::cout << "Computed: "
              << "startx " << app_startx << ", starty " << app_starty << ", sizex " << app_sizex << ", sizey " << app_sizey << std::endl;
    std::cout << "Expected: "
              << "startx " << ref_startx << ", starty " << ref_starty << ", sizex " << ref_sizex << ", sizey " << ref_sizey << std::endl;

    return false;
  }

  return true;
}

// App configuration with the standard mode
void extract_roi_standard(ApplicationPointerType app, unsigned int startx, unsigned int starty, unsigned int sizex, unsigned int sizey)
{
  app->SetParameterString("mode", "standard");
  app->UpdateParameters();
  app->SetParameterInt("startx", startx);
  app->UpdateParameters();
  app->SetParameterInt("starty", starty);
  app->UpdateParameters();
  app->SetParameterInt("sizex", sizex);
  app->UpdateParameters();
  app->SetParameterInt("sizey", sizey);
  app->UpdateParameters();
  app->Execute();
}

// App configuration with the extent mode
void extract_roi_extent(ApplicationPointerType app, std::string unit, double ulx, double uly, double lrx, double lry)
{
  app->SetParameterString("mode", "extent");
  app->UpdateParameters();
  app->SetParameterFloat("mode.extent.ulx", ulx);
  app->UpdateParameters();
  app->SetParameterFloat("mode.extent.uly", uly);
  app->UpdateParameters();
  app->SetParameterFloat("mode.extent.lrx", lrx);
  app->UpdateParameters();
  app->SetParameterFloat("mode.extent.lry", lry);
  app->UpdateParameters();
  app->SetParameterString("mode.extent.unit", unit);
  app->UpdateParameters();
  app->Execute();
}

// App configuration with the radius mode
void extract_roi_radius(ApplicationPointerType app, std::string unitc, std::string unitr, double r, double cx, double cy)
{
  app->SetParameterString("mode", "radius");
  app->UpdateParameters();
  app->SetParameterFloat("mode.radius.r", r);
  app->UpdateParameters();
  app->SetParameterFloat("mode.radius.cx", cx);
  app->UpdateParameters();
  app->SetParameterFloat("mode.radius.cy", cy);
  app->UpdateParameters();
  app->SetParameterString("mode.radius.unitc", unitc);
  app->UpdateParameters();
  app->SetParameterString("mode.radius.unitr", unitr);
  app->UpdateParameters();
  app->Execute();
}

/** This function tests the ExtractROI application. It verifies that, for different sets of parameters,
 * the ROI computed by the application (startx, starty,sizex and sizey) is correct. The tested mode are :
 * - Standard mode with invalid size (output is the full image)
 * - Extent mode with invalid size (output is the full image)
 * - Radius mode with invalid size (output is the full image)
 * - Standard mode
 * - Extent mode with pixel units
 * - Extent mode with lonlat unit
 * - Extent mode with phy unit
 * - radius mode with pixel center and radius
 * - radius mode with phy center and radius
 * - radius mode with lonlat center and pixel radius
 * - fit mode with image input
 * For all these tests the application executed, but nothing is written to disk, and the output image is not updated.
 * This means that the multiChannelExtractROI filter is not updated.
 * Finally, in a last test, the channel option is set and the output is updated.
 * Note that this test does not test the fit mode with vector data, which is tested in another test.
*/

int otbExtractROIAppTests(int, char* argv[])
{
  unsigned int imageSize = std::stoi(argv[2]);

  // ROI Definition
  unsigned int sizex  = std::stoi(argv[3]);
  unsigned int sizey  = std::stoi(argv[4]);
  unsigned int startx = std::stoi(argv[5]);
  unsigned int starty = std::stoi(argv[6]);

  // The ROI should be contained in the input image
  assert(imageSize > startx + sizex && imageSize > starty + sizey);
  // sizex must be odd (for radius computation)
  assert(sizex % 2 == 1);

  // Create the input image

  // Proj Ref and origin
  OGRSpatialReference projRef;

  projRef.SetWellKnownGeogCS("WGS84");
  projRef.SetUTM(31, TRUE);

  // VectorImageType::PointType origin;
  itk::Point<float, 2> ul_phy;
  ul_phy[0] = 452256.55;
  ul_phy[1] = 5412621.36;

  VectorImageType::SpacingType spacing;
  spacing[0] = 1;
  spacing[1] = 1;

  VectorImageType::SizeType size;
  size.Fill(imageSize);
  auto inImg = create_vector_image(size, 3, 0, projRef, ul_phy, spacing);

  otb::Wrapper::ApplicationRegistry::SetApplicationPath(argv[1]);
  auto app = otb::Wrapper::ApplicationRegistry::CreateApplication("ExtractROI");

  // Set the input image
  app->SetParameterInputImage("in", inImg);
  app->UpdateParameters();

  // Test 1 : input image, no other parameters
  std::cout << "Test: input Image set with no other parameters" << std::endl;

  if (!check_roi(app, 0, 0, imageSize, imageSize))
    return EXIT_FAILURE;

  std::cout << "Test: standard mode with too big size" << std::endl;

  extract_roi_standard(app, 0, 0, 10000000, imageSize);
  if (!check_roi(app, 0, 0, imageSize, imageSize))
    return EXIT_FAILURE;

  std::cout << "Test: extent mode with too big extent" << std::endl;
  extract_roi_extent(app, "pxl", 0, 0, imageSize + 1000000, imageSize);
  if (!check_roi(app, 0, 0, imageSize, imageSize))
    return EXIT_FAILURE;

  std::cout << "Test: radius mode with too big radius" << std::endl;

  extract_roi_radius(app, "pxl", "pxl", 10000000, imageSize / 2, imageSize / 2);
  if (!check_roi(app, 0, 0, imageSize, imageSize))
    return EXIT_FAILURE;


  std::cout << "Test: standard mode" << std::endl;
  extract_roi_standard(app, startx, starty, sizex, sizey);
  if (!check_roi(app, startx, starty, sizex, sizey))
    return EXIT_FAILURE;


  // -------------- Extent mode test --------------------------------

  // Indexes of the extent corresponding to the input parameters
  VectorImageType::IndexType uli, lri;
  uli[0] = startx;
  uli[1] = starty;
  lri[0] = startx + sizex - 1;
  lri[1] = starty + sizey - 1;

  std::cout << "Test: extent mode with pixel unit" << std::endl;
  extract_roi_extent(app, "pxl", uli[0], uli[1], lri[0], lri[1]);
  if (!check_roi(app, startx, starty, sizex, sizey))
    return EXIT_FAILURE;


  std::cout << "Test: extent mode with physical unit" << std::endl;
  // Find cartographic coordinated corresponding to the input image coordinates.

  itk::Point<float, 2> ulp, lrp;

  inImg->TransformIndexToPhysicalPoint(uli, ulp);
  inImg->TransformIndexToPhysicalPoint(lri, lrp);

  // We may need to swap coordinates to have a non inverted extent (if the image has negative spacing).
  extract_roi_extent(app, "phy", std::min(ulp[0], lrp[0]), std::min(ulp[1], lrp[1]), std::max(ulp[0], lrp[0]), std::max(ulp[1], lrp[1]));
  if (!check_roi(app, startx, starty, sizex, sizey))
    return EXIT_FAILURE;

  std::cout << "Test: extent mode with long/lat unit" << std::endl;

  // Proj Ref and origin
  char* projRefWkt = nullptr;

  projRef.exportToPrettyWkt(&projRefWkt);
  auto transform = otb::GenericRSTransform<>::New();
  transform->SetInputProjectionRef(projRefWkt);
  transform->SetOutputProjectionRef(otb::SpatialReference::FromWGS84().ToWkt());
  transform->InstantiateTransform();
  auto ulp_lonlat = transform->TransformPoint(ulp);
  auto lrp_lonlat = transform->TransformPoint(lrp);

  // We may need to swap coordinates to have a non inverted extent (if the image has negative spacing).
  extract_roi_extent(app, "lonlat", std::min(ulp_lonlat[0], lrp_lonlat[0]), std::min(ulp_lonlat[1], lrp_lonlat[1]), std::max(ulp_lonlat[0], lrp_lonlat[0]),
                     std::max(ulp_lonlat[1], lrp_lonlat[1]));
  if (!check_roi(app, startx, starty, sizex, sizey))
    return EXIT_FAILURE;

  std::cout << "Test: radius mode with pxl center and pxl radius" << std::endl;

  VectorImageType::IndexType center_pxl;
  center_pxl[0]  = startx + (sizex - 1) / 2;
  center_pxl[1]  = starty + (sizex - 1) / 2;
  unsigned int r = (sizex - 1) / 2;

  extract_roi_radius(app, "pxl", "pxl", r, center_pxl[0], center_pxl[1]);
  if (!check_roi(app, startx, starty, sizex, sizex))
    return EXIT_FAILURE;

  float r_phy = r * inImg->GetSignedSpacing()[0];

  itk::Point<float, 2> center_phy;
  inImg->TransformIndexToPhysicalPoint(center_pxl, center_phy);

  std::cout << "Test: radius mode with phy center and phy radius" << std::endl;

  extract_roi_radius(app, "phy", "phy", r_phy, center_phy[0], center_phy[1]);
  if (!check_roi(app, startx, starty, sizex, sizex))
    return EXIT_FAILURE;

  auto center_lonlat = transform->TransformPoint(center_phy);

  std::cout << "Test: radius mode with lonlat center and pxl radius" << std::endl;

  extract_roi_radius(app, "lonlat", "pxl", r, center_lonlat[0], center_lonlat[1]);
  if (!check_roi(app, startx, starty, sizex, sizex))
    return EXIT_FAILURE;

  // -------------- Fit mode test --------------------------------

  VectorImageType::SizeType fitSize;
  fitSize[0] = sizex;
  fitSize[1] = sizey;
  itk::Point<float, 2> fitOrig;
  fitOrig[0] = ulp[0];
  fitOrig[1] = ulp[1];

  std::cout << "Test: fit mode with reference image" << std::endl;

  auto fitImg = create_vector_image(fitSize, 3, 0, projRef, ulp, spacing);
  app->SetParameterString("mode", "fit");
  app->UpdateParameters();
  app->SetParameterInputImage("mode.fit.im", fitImg);
  app->UpdateParameters();
  app->Execute();
  if (!check_roi(app, startx, starty, sizex, sizey))
    return EXIT_FAILURE;

  // Finally, test update the output to test the multiChannelExtractROI and the channel option.
  std::cout << "Test: cl parameter" << std::endl;

  app->SetParameterStringList("cl", {"Channel1", "Channel2"});
  app->UpdateParameters();
  app->Execute();

  auto output = app->GetParameterImageBase("out");
  output->Update();

  // Check that the output has the requested number of bands
  if (output->GetNumberOfComponentsPerPixel() != 2)
  {
    std::cout << "Wrong number of components after ExtractROI" << std::endl;
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
