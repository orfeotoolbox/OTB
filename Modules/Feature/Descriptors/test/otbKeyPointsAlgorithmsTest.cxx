/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
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

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "itkScalableAffineTransform.h"
#include "otbStreamingResampleImageFilter.h"
#include "otbBCOInterpolateImageFunction.h"
#include "otbKeyPointSetsMatchingFilter.h"
#include "otbSiftFastImageFilter.h"
#include "otbImageToSIFTKeyPointSetFilter.h"
#include "otbImageToSURFKeyPointSetFilter.h"
#include "itkPointSet.h"

#include <tuple>

using ImageType          = otb::Image<double>;
using ReaderType         = otb::ImageFileReader<ImageType>;
using TransformType      = itk::ScalableAffineTransform<double, 2>;
using ResamplerType      = otb::StreamingResampleImageFilter<ImageType, ImageType, double>;
using InterpolatorType   = otb::BCOInterpolateImageFunction<ImageType, double>;
using VectorType         = itk::VariableLengthVector<double>;
using PointSetType       = itk::PointSet<VectorType, 2>;
using SiftFastFilterType = otb::SiftFastImageFilter<ImageType, PointSetType>;
using SiftFilterType     = otb::ImageToSIFTKeyPointSetFilter<ImageType, PointSetType>;
using SurfFilterType     = otb::ImageToSURFKeyPointSetFilter<ImageType, PointSetType>;
using MatchingFilterType = otb::KeyPointSetsMatchingFilter<PointSetType>;


/** Generate a pair of images, one beeing slightly warped wrt the
 * other */
auto generateImagePair(const std::string& infname, double rotation, double scaling)
{
  // Read reference image
  auto reader = ReaderType::New();
  reader->SetFileName(infname);
  reader->Update();
  ImageType::Pointer reference = reader->GetOutput();

  // Create secondary image

  // Setup transform
  auto transform = TransformType::New();

  // Set rotation center as image center
  auto origin  = reference->GetOrigin();
  auto spacing = reference->GetSpacing();
  auto size    = reference->GetLargestPossibleRegion().GetSize();
  auto center  = origin;
  center[0] += 0.5 * spacing[0] * size[0];
  center[1] += 0.5 * spacing[1] * size[1];
  transform->SetCenter(center);

  // Set rotation angle
  transform->Rotate2D(rotation * otb::CONST_PI_180);

  // Set scale
  ImageType::SpacingType scalingVector;
  scalingVector.Fill(scaling);
  transform->Scale(scalingVector);

  // Invert transform
  auto inverse = TransformType::New();
  bool ok      = transform->GetInverse(inverse);
  if (!ok)
    throw std::logic_error("Could not inverse transform");
  // Setup interpolator
  auto interpolator = InterpolatorType::New();

  // Setup resampler
  auto resampler = ResamplerType::New();
  resampler->SetInput(reference);
  resampler->SetTransform(transform);
  resampler->SetInterpolator(interpolator);

  // Since rotation and scaling are small, use same image parameter
  // for secondary image
  resampler->SetOutputOrigin(origin);
  resampler->SetOutputSize(size);
  resampler->SetOutputSpacing(spacing);
  resampler->Update();
  ImageType::Pointer secondary = resampler->GetOutput();

  return std::make_tuple(reference, secondary, transform);
}

/** Perform checks for one keypoints algorithm */
template <typename TKeyPointsFilter, typename TParameterSetter>
                                                                                                                                                            bool checkKeyPointsFilter(const ImageType* reference, const ImageType* secondary, const TransformType* transform, const TParameterSetter& configureFilter, double match_rate_thresh, double good_match_rate_thresh)
{
  // Keypoints on first image
  auto filterReference = TKeyPointsFilter::New();
  filterReference->SetInput(reference);
  configureFilter(filterReference);

  // Keypoints on secondary image
  auto filterSecondary = TKeyPointsFilter::New();
  filterSecondary->SetInput(secondary);
  configureFilter(filterSecondary);

  // Match keypoints
  auto matcher = MatchingFilterType::New();
  matcher->SetUseBackMatching(false);
  matcher->SetDistanceThreshold(0.6);
  matcher->SetInput1(filterReference->GetOutput());
  matcher->SetInput2(filterSecondary->GetOutput());
  matcher->Update();

  PointSetType::Pointer         referencePoints   = filterReference->GetOutput();
  const size_t nbReferencePoints = referencePoints->GetPoints()->Size();

  typename PointSetType::Pointer secondaryPoints   = filterSecondary->GetOutput();
  const size_t nbSecondaryPoints = secondaryPoints->GetPoints()->Size();

  std::cout << "Found " << nbReferencePoints << " points in reference image and " << nbSecondaryPoints << " points in secondary image" << std::endl;

  auto         matches    = matcher->GetOutput();
  const size_t nb_matches = matches->Size();

  size_t       good_matches             = 0;
  const double threshold_for_good_match = 0.5; // pixels

  // Count good and bad matches
  for (auto it = matches->Begin(); it != matches->End(); ++it)
  {
    const auto p1 = it.Get()->GetPoint1();
    const auto p2 = it.Get()->GetPoint2();

    const auto p2_mapped = transform->TransformPoint(p2);

    // Check that matches are good up to 0.1 pixel
    if (std::sqrt((p1[0] - p2_mapped[0]) * (p1[0] - p2_mapped[0]) + (p1[1] - p2_mapped[1]) * (p1[1] - p2_mapped[1])) <= threshold_for_good_match)
      ++good_matches;
  }

  // Performances metrics
  const float reference_match_rate = nb_matches / static_cast<float>(nbReferencePoints);
  const float secondary_match_rate = nb_matches / static_cast<float>(nbSecondaryPoints);
  const float good_match_rate      = good_matches / static_cast<float>(nb_matches);



  std::cout << "Found " << nb_matches << " matches with " << good_matches << " valid matches (tolerance of 0.5 pixels)"<< std::endl;

  // Quality gate
  auto printResult = [](bool value) { return value ? "Ok" : "Nok"; };

  bool current_test = reference_match_rate > match_rate_thresh;
  bool overall_status = current_test;
  std::cout << "More than "<<100*match_rate_thresh<<"% of reference points have a match:\t" << printResult(current_test)<<" ("<< 100*reference_match_rate<<"%)"<<std::endl;

  current_test = secondary_match_rate > match_rate_thresh;
  std::cout << "More than "<<100*match_rate_thresh<<"% of secondary points have a match:\t" << printResult(current_test)<<" ("<< 100*secondary_match_rate<<"%)"<<std::endl;
  overall_status = overall_status && current_test;

  current_test = good_match_rate > good_match_rate_thresh;
  std::cout << "More than "<<good_match_rate_thresh*100<<"% of matches are good:             \t" << printResult(current_test) << " ("<< 100*good_match_rate<< "% at 0.5 pixel accuracy)"
            << "\n";
  overall_status = overall_status && current_test;
  return overall_status;
}


int otbKeyPointsAlgorithmsTest(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "Usage: " << argv[0] << " infname" << std::endl;
    return EXIT_FAILURE;
  }
  const char* infname = argv[1];

  // Generate reference and secondary image
  ImageType::Pointer     reference, secondary;
  TransformType::Pointer transform;

  // Small rotation and scaling
  const double rotation = 2.5; // 5°
  const double scaling  = 0.99;

  std::tie(reference, secondary, transform) = generateImagePair(infname,rotation, scaling);

  std::cout << "Secondary image generated by applying a rotation of "<<rotation<<" degrees and scaling of "<<scaling<< "."<<std::endl;

  bool status = true;

  // Test Surf filter
  std::cout << "Checking Surf implementation:" << std::endl;
  std::cout << "=============================" << std::endl;

  // Lambda to configure surf algorithm
  auto configureSurf = [](SurfFilterType* filter) {
    filter->SetOctavesNumber(4);
    filter->SetScalesNumber(8);
  };

  status = checkKeyPointsFilter<SurfFilterType>(reference, secondary, transform, configureSurf,0.15,0.65) && status;

  // Test Sift filter
  std::cout << "Checking Sift implementation:" << std::endl;
  std::cout << "=============================" << std::endl;

  // Lambda to configure sift algorithm
  auto configureSift = [](SiftFilterType* filter) {
    filter->SetOctavesNumber(4);
    filter->SetScalesNumber(8);
    filter->SetDoGThreshold(0.01);
    filter->SetEdgeThreshold(10.);
  };

  status = checkKeyPointsFilter<SiftFilterType>(reference, secondary, transform, configureSift,0.45,0.85) && status;

#ifdef OTB_USE_SIFTFAST
  // Test SiftFast filter
  std::cout << "Checking SiftFast implementation:" << std::endl;
  std::cout << "=================================" << std::endl;

  // lambda to set specific filter parameter
  auto configureSiftFast = [](SiftFastFilterType* filter) 
    { 
      filter->SetScalesNumber(8);
    };

  status = checkKeyPointsFilter<SiftFastFilterType>(reference, secondary, transform, configureSiftFast,0.65,0.95) && status;
#endif

  return status ? EXIT_SUCCESS : EXIT_FAILURE;
}
