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
#ifndef __otbWrapperTags_h
#define __otbWrapperTags_h

#include <string>

namespace otb
{
namespace Wrapper
{
namespace Tags
{

static const std::string Analysis="Image Analysis";
static const std::string Calibration="Calibration";
static const std::string ChangeDetection="Change Detection";
static const std::string Coordinates="Coordinates";
static const std::string DimensionReduction="Dimensionality Reduction";
static const std::string FeatureExtraction="Feature Extraction";
static const std::string Filter="Image Filtering";
static const std::string Geometry="Geometry";
static const std::string Hyperspectral="Hyperspectral";
static const std::string Learning="Learning";
static const std::string Manip="Image Manipulation";
static const std::string Meta="Image MetaData";
static const std::string Multi="Image MultiResolution";
static const std::string Pansharpening="Pansharpening";
static const std::string Raster="Raster";
static const std::string SAR="SAR";
static const std::string Stereo="Stereo";
static const std::string Segmentation="Segmentation";
static const std::string Vector="Vector Data Manipulation";

} // end namespace Wrappers
} // end namespace Tags
} // end namespace otb

#endif // __otbWrapperTags_h_
