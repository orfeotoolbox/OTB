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

#ifndef otbWrapperTags_h
#define otbWrapperTags_h

#include <string>

namespace otb
{
namespace Wrapper
{
namespace Tags
{

static const std::string Analysis           = "Image Analysis";
static const std::string Calibration        = "Calibration";
static const std::string ChangeDetection    = "Change Detection";
static const std::string Coordinates        = "Coordinates";
static const std::string DimensionReduction = "Dimensionality Reduction";
static const std::string FeatureExtraction  = "Feature Extraction";
static const std::string Filter             = "Image Filtering";
static const std::string Geometry           = "Geometry";
static const std::string Hyperspectral      = "Hyperspectral";
static const std::string Learning           = "Learning";
static const std::string Manip              = "Image Manipulation";
static const std::string Meta               = "Image MetaData";
static const std::string Multi              = "Image MultiResolution";
static const std::string Pansharpening      = "Pansharpening";
static const std::string Raster             = "Raster";
static const std::string SAR                = "SAR";
static const std::string Stereo             = "Stereo";
static const std::string Segmentation       = "Segmentation";
static const std::string Vector             = "Vector Data Manipulation";
static const std::string Deprecated         = "Deprecated";

} // end namespace Wrappers
} // end namespace Tags
} // end namespace otb

#endif // otbWrapperTags_h_
