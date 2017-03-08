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

#ifndef otbCvRTreesWrapper_h
#define otbCvRTreesWrapper_h

#include "otbOpenCVUtils.h"
#include <vector>

namespace otb
{

/** \class CvRTreesWrapper
 * \brief Wrapper for OpenCV Random Trees
 *
 * \ingroup OTBSupervised
 */
class OTBSupervised_EXPORT CvRTreesWrapper : public CvRTrees
{
public:
  typedef std::vector<unsigned int> VotesVectorType;
  CvRTreesWrapper();
  ~CvRTreesWrapper() ITK_OVERRIDE;

  /** Compute the number of votes for each class. */
  void get_votes(const cv::Mat& sample,
                 const cv::Mat& missing,
                 VotesVectorType& vote_count) const;

  /** Predict the confidence of the classifcation by computing the proportion
      of trees which voted for the majority class.
  */
  float predict_confidence(const cv::Mat& sample,
                           const cv::Mat& missing =
                           cv::Mat()) const;

  /** Predict the confidence margin of the classifcation by computing the
      difference in votes between the first and second most voted classes.
      This measure is preferred to the proportion of votes of the majority
      class, since it provides information about the conflict between the
      most likely classes.
  */
  float predict_margin(const cv::Mat& sample,
                          const cv::Mat& missing =
                          cv::Mat()) const;
};

}

#endif
