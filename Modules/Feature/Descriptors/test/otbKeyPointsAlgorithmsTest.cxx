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

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbExtractROI.h"
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
using ExtractType        = otb::ExtractROI<double, double>;
using TransformType      = itk::ScalableAffineTransform<double, 2>;
using ResamplerType      = otb::StreamingResampleImageFilter<ImageType, ImageType, double>;
using InterpolatorType   = otb::BCOInterpolateImageFunction<ImageType, double>;
using VectorType         = itk::VariableLengthVector<double>;
using PointSetType       = itk::PointSet<VectorType, 2>;
using SiftFastFilterType = otb::SiftFastImageFilter<ImageType, PointSetType>;
using SiftFilterType     = otb::ImageToSIFTKeyPointSetFilter<ImageType, PointSetType>;
using SurfFilterType     = otb::ImageToSURFKeyPointSetFilter<ImageType, PointSetType>;
using MatchingFilterType = otb::KeyPointSetsMatchingFilter<PointSetType>;

auto printResult = [](bool value) { return value ? "Ok" : "Nok"; };

bool testMatchingFilter()
{
  auto ps1 = PointSetType::New();
  auto ps2 = PointSetType::New();

  PointSetType::PointType p1, p2, p3, p4, p5, p6;
  p1.Fill(1.);
  p2.Fill(2.);
  p3.Fill(3.);
  p4.Fill(4.);
  p5.Fill(5.);
  p6.Fill(6.);
  ps1->SetPoint(0, p1);
  ps1->SetPoint(1, p2);
  ps1->SetPoint(2, p3);
  ps2->SetPoint(0, p4);
  ps2->SetPoint(1, p5);
  ps2->SetPoint(2, p6);


  VectorType d1(1), d2(1), d3(1), d4(1), d5(1), d6(1);
  d1[0] = 0.7;
  d2[0] = 0.8;
  d3[0] = 10.;
  d4[0] = 0.;
  d5[0] = 1.;
  d6[0] = 11.;

  ps1->SetPointData(0, d1);
  ps1->SetPointData(1, d2);
  ps1->SetPointData(2, d3);
  ps2->SetPointData(0, d4);
  ps2->SetPointData(1, d5);
  ps2->SetPointData(2, d6);

  auto filter1 = MatchingFilterType::New();
  filter1->SetDistanceThreshold(0.6);
  filter1->SetUseBackMatching(false);
  filter1->SetInput1(ps1);
  filter1->SetInput2(ps2);
  filter1->Update();

  auto matches1 = filter1->GetOutput();

  std::cout << "Matches without backmatching: " << std::endl;

  for (auto it = matches1->Begin(); it != matches1->End(); ++it)
  {
    std::cout << it.Get()->GetPoint1() << " <-> " << it.Get()->GetPoint2() << std::endl;
  }

  auto filter2 = MatchingFilterType::New();
  filter2->SetDistanceThreshold(0.6);
  filter2->SetUseBackMatching(true);
  filter2->SetInput1(ps1);
  filter2->SetInput2(ps2);
  filter2->Update();

  auto matches2 = filter2->GetOutput();

  std::cout << "Matches with backmatching: " << std::endl;

  for (auto it = matches2->Begin(); it != matches2->End(); ++it)
  {
    std::cout << it.Get()->GetPoint1() << " <-> " << it.Get()->GetPoint2() << std::endl;
  }

  bool success = true;

  // Without backmatching, matches should be:
  // p1 <-> p5
  // p2 <-> p5
  // p3 <-> p6
  unsigned int nb_matches = matches1->Size();

  bool test = nb_matches == 3;
  std::cout << "Without backmatching, the number of matches is 3:\t" << printResult(test) << std::endl;
  success = success && test;

  test = nb_matches > 1 && matches1->GetNthElement(0)->GetPoint1() == p1 && matches1->GetNthElement(0)->GetPoint2() == p5;
  std::cout << "Without backmatching, p1 matches with p5:\t\t" << printResult(test) << std::endl;
  success = success && test;

  test = nb_matches > 0 && matches1->GetNthElement(1)->GetPoint1() == p2 && matches1->GetNthElement(1)->GetPoint2() == p5;
  std::cout << "Without backmatching, p2 matches with p5:\t\t" << printResult(test) << std::endl;
  success = success && test;


  test = nb_matches > 2 && matches1->GetNthElement(2)->GetPoint1() == p3 && matches1->GetNthElement(2)->GetPoint2() == p6;
  std::cout << "Without backmatching, p3 matches with p6:\t\t" << printResult(test) << std::endl;
  success = success && test;

  // With back-matching there should be only 2 matches:
  // p2 <-> p5
  // p3 <-> p6
  test = matches2->Size() == 2;
  std::cout << "With backmatching, the number of matches is 2:\t" << printResult(test) << std::endl;
  success = success && test;

  test = matches2->GetNthElement(0)->GetPoint1() == p2 && matches2->GetNthElement(0)->GetPoint2() == p5;
  std::cout << "With backmatching, p2 matches with p5:\t\t" << printResult(test) << std::endl;
  success = success && test;

  test = matches2->GetNthElement(1)->GetPoint1() == p3 && matches2->GetNthElement(1)->GetPoint2() == p6;
  std::cout << "With backmatching, p3 matches with p6:\t\t" << printResult(test) << std::endl;
  success = success && test;


  return success;
}


/** Generate a pair of images, one being slightly warped wrt the
 * other */
auto generateImagePair(const std::string& infname, double rotation, double scaling)
{
  // Read reference image
  auto reader = ReaderType::New();
  reader->SetFileName(infname);

  auto extractor = ExtractType::New();
  extractor->SetInput(reader->GetOutput());
  extractor->SetSizeX(50);
  extractor->SetSizeY(50);
  extractor->Update();

  ImageType::Pointer reference = extractor->GetOutput();

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
bool checkKeyPointsFilter(const ImageType* reference, const ImageType* secondary, const TransformType* transform, const TParameterSetter& configureFilter,
                          unsigned int nb_points_thresh, double match_rate_thresh, double good_match_rate_thresh)
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

  PointSetType::Pointer referencePoints   = filterReference->GetOutput();
  const size_t          nbReferencePoints = referencePoints->GetPoints()->Size();

  typename PointSetType::Pointer secondaryPoints   = filterSecondary->GetOutput();
  const size_t                   nbSecondaryPoints = secondaryPoints->GetPoints()->Size();

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
    if ((p1[0] - p2_mapped[0]) * (p1[0] - p2_mapped[0]) + (p1[1] - p2_mapped[1]) * (p1[1] - p2_mapped[1]) <=
        threshold_for_good_match * threshold_for_good_match)
      ++good_matches;
  }

  // Performances metrics
  const float reference_match_rate = nb_matches / static_cast<float>(nbReferencePoints);
  const float secondary_match_rate = nb_matches / static_cast<float>(nbSecondaryPoints);
  const float good_match_rate      = good_matches / static_cast<float>(nb_matches);


  std::cout << "Found " << nb_matches << " matches with " << good_matches << " valid matches (tolerance of 0.5 pixels)" << std::endl;

  // Quality gate
  bool current_test = nbReferencePoints >= nb_points_thresh;
  std::cout << "More than " << nb_points_thresh << " points found in reference image:\t" << printResult(current_test) << " (" << nbReferencePoints << ")"
            << std::endl;
  bool overall_status = current_test;

  current_test = nbSecondaryPoints > nb_points_thresh;
  std::cout << "More than " << nb_points_thresh << " points found in secondary image:\t" << printResult(current_test) << " (" << nbSecondaryPoints << ")"
            << std::endl;
  overall_status = overall_status && current_test;

  current_test = reference_match_rate > match_rate_thresh;
  std::cout << "More than " << 100 * match_rate_thresh << "% of reference points have a match:\t" << printResult(current_test) << " ("
            << 100 * reference_match_rate << "%)" << std::endl;
  overall_status = overall_status && current_test;

  current_test = secondary_match_rate > match_rate_thresh;
  std::cout << "More than " << 100 * match_rate_thresh << "% of secondary points have a match:\t" << printResult(current_test) << " ("
            << 100 * secondary_match_rate << "%)" << std::endl;
  overall_status = overall_status && current_test;

  current_test = good_match_rate > good_match_rate_thresh;
  std::cout << "More than " << good_match_rate_thresh * 100 << "% of matches are good:             \t" << printResult(current_test) << " ("
            << 100 * good_match_rate << "% at 0.5 pixel accuracy)"
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

  // First test matching filter alone
  std::cout << "Checking matching filter:" << std::endl;
  std::cout << "=========================" << std::endl;
  bool status = testMatchingFilter();

  std::tie(reference, secondary, transform) = generateImagePair(infname, rotation, scaling);

  std::cout << "Secondary image generated by applying a rotation of " << rotation << " degrees and scaling of " << scaling << "." << std::endl;

  // Test Surf filter
  std::cout << "Checking Surf implementation:" << std::endl;
  std::cout << "=============================" << std::endl;

  // Lambda to configure surf algorithm
  auto configureSurf = [](SurfFilterType* filter) {
    filter->SetOctavesNumber(4);
    filter->SetScalesNumber(8);
  };

  status = checkKeyPointsFilter<SurfFilterType>(reference, secondary, transform, configureSurf, 95, 0.13, 0.64) && status;

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

  status = checkKeyPointsFilter<SiftFilterType>(reference, secondary, transform, configureSift, 120, 0.44, 0.82) && status;

#ifdef OTB_USE_SIFTFAST
  // Test SiftFast filter
  std::cout << "Checking SiftFast implementation:" << std::endl;
  std::cout << "=================================" << std::endl;

  // lambda to set specific filter parameter
  auto configureSiftFast = [](SiftFastFilterType* filter) { filter->SetScalesNumber(8); };

  status = checkKeyPointsFilter<SiftFastFilterType>(reference, secondary, transform, configureSiftFast, 100, 0.40, 0.95) && status;
#endif

  return status ? EXIT_SUCCESS : EXIT_FAILURE;
}
